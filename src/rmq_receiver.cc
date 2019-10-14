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

int main(void) {
  auto *loop = EV_DEFAULT;
  MyHandler handler(loop);
#if OPENSSL_VERSION_NUMBER < 0x10100000L
  SSL_library_init();
#else
  OPENSSL_init_ssl(0, NULL);
#endif
  AMQP::Address address("amqp://guest:guest@localhost/");
  // Secure connection.
  // AMQP::Address address("amqps://guest:guest@localhost/");
  AMQP::TcpConnection connection(&handler, address);
  AMQP::TcpChannel channel(&connection);
  const std::string exchange_ = "exchange";
  const std::string queue_ = "queue";
  const std::string routekey_ = "routekey";
  channel.declareExchange(exchange_, AMQP::topic);
  channel.declareQueue(queue_, AMQP::exclusive);
  channel.bindQueue(exchange_, queue_, routekey_);
  channel.consume(queue_, AMQP::noack).onReceived(
      [&](const AMQP::Message& m, uint64_t, bool) {
        std::vector<char> message(m.body(), m.body() + m.bodySize());
        printf("Received data[%lu]: ", message.size());
        for (auto ch : message) {
          printf("%c", static_cast<uint8_t>(ch));
        }
        printf("\n");
      });
  ev_run(loop, 0);
  return 0;
}

