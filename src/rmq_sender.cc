#include <ev.h>
#include <amqpcpp.h>
#include <amqpcpp/libev.h>
#include <openssl/ssl.h>
#include <openssl/opensslv.h>


class MyHandler : public AMQP::LibEvHandler {
 public:
  explicit MyHandler(struct ev_loop *loop) : AMQP::LibEvHandler(loop) { }
  MyHandler(const MyHandler&) = delete;
  MyHandler& operator=(const MyHandler&) = delete;
  virtual ~MyHandler() = default;

 private:
  void onError(AMQP::TcpConnection *connection,
      const char *message) override {
    std::cout << "error: " << message << std::endl;
  }
};

class MyTimer {
 public:
  MyTimer(struct ev_loop *loop, AMQP::TcpChannel *channel, std::string queue) :
      channel_(channel), queue_(std::move(queue)) {
    ev_timer_init(&timer_, callback, 0.005, 1.005);
    timer_.data = this;
    ev_timer_start(loop, &timer_);
  }
  MyTimer(const MyTimer&) = delete;
  MyTimer& operator=(const MyTimer&) = delete;
  virtual ~MyTimer() = default;

 private:
  struct ev_timer timer_;
  AMQP::TcpChannel *channel_;
  std::string queue_;
  static void callback(
      struct ev_loop *loop, struct ev_timer *timer, int revents) {
    MyTimer *self = reinterpret_cast<MyTimer*>(timer->data);
    self->channel_->publish("exchange", "routekey", "HelloWorld");
  }
};

int main(void) {
  auto *loop = EV_DEFAULT;
  MyHandler handler(loop);
#if OPENSSL_VERSION_NUMBER < 0x10100000L
  SSL_library_init();
#else
  OPENSSL_init_ssl(0, NULL);
#endif
  AMQP::Address address("amqp://guest:guest@localhost/");
  AMQP::TcpConnection connection(&handler, address);
  AMQP::TcpChannel channel(&connection);
  channel.declareQueue(AMQP::exclusive).onSuccess(
      [&connection, &channel, loop](const std::string &name,
          uint32_t messagecount, uint32_t consumercount) {
            auto *timer = new MyTimer(loop, &channel, name);
          });
  ev_run(loop, 0);
  return 0;
}

