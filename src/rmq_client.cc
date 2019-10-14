// Copyright 2019 Yuming Meng. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "rmq_client.h"

#include <stdint.h>

#include <ev.h>
#include <amqpcpp.h>
#include <amqpcpp/libev.h>
#include <openssl/ssl.h>
#include <openssl/opensslv.h>


namespace librmqclient {

// #define LOG_DEBUG  1

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

RmqClient::~RmqClient() {
  Stop();
  if (!recv_message_.empty()) {
    recv_message_.clear();
  }
}

int RmqClient::SendMessage(const char *message, const int &size) {
  int ret = 0;
  auto *loop = EV_DEFAULT;
  MyHandler handler(loop);
#if OPENSSL_VERSION_NUMBER < 0x10100000L
  SSL_library_init();
#else
  OPENSSL_init_ssl(0, NULL);
#endif
  AMQP::Address address(producer_url_);
  AMQP::TcpConnection connection(&handler, address);
  AMQP::TcpChannel channel(&connection);
  channel.onError([&loop, &ret](const char *err_message) {
      std::cout << "Channel error: " << err_message << std::endl;
      ev_break(loop, EVBREAK_ALL);
      ret = -1; });
  channel.declareExchange(producer_exchange_, AMQP::topic)
      .onError([&](const char* msg) {
          std::cout << "ERROR: " << msg << std::endl;
          ev_break(loop, EVBREAK_ALL); })
      .onSuccess([&]() {
#ifdef LOG_DEBUG
          printf("Send data[%lu]: ", size);
          for (int i = 0; i < size; ++i) {
            printf("%02X ", static_cast<uint8_t>(message[i]));
          }
          printf("\n");
#endif
          channel.publish(producer_exchange_, producer_routekey_,
                          message, size);
          ev_break(loop, EVBREAK_ALL); });
  ev_run(loop, 0);
  // Waiting to exit the loop.
  connection.close();
  return ret;
}

void RmqClient::Run(void) {
  if (service_is_running_ == false) {
    service_is_running_ = true;
    service_thread_ = std::thread(&RmqClient::RecvService, this);
    printf("Start listen service...\n");
  }
}

void RmqClient::Stop(void) {
  if (service_is_running_ == true) {
    service_is_running_ = false;
    if (ev_loop_ptr_ != nullptr) {
      ev_break(reinterpret_cast<struct ev_loop *>(ev_loop_ptr_), EVBREAK_ALL);
      ev_loop_ptr_ = nullptr;
    }
    if (connection_ptr_ != nullptr) {
      reinterpret_cast<AMQP::TcpConnection *>(connection_ptr_)->close();
      connection_ptr_ = nullptr;
    }
    service_thread_.join();
  }
}

void RmqClient::RecvService(void) {
  auto *loop = EV_DEFAULT;
  MyHandler handler(loop);
#if OPENSSL_VERSION_NUMBER < 0x10100000L
  SSL_library_init();
#else
  OPENSSL_init_ssl(0, NULL);
#endif
  AMQP::Address address(consumer_url_);
  AMQP::TcpConnection connection(&handler, address);
  AMQP::TcpChannel channel(&connection);
  channel.onError([&loop](const char *err_message) {
      std::cout << "Channel error: " << err_message << std::endl;
      ev_break(loop, EVBREAK_ALL); });
  channel.declareExchange(consumer_exchange_, AMQP::topic);
  channel.declareQueue(consumer_queue_, AMQP::exclusive);
  channel.bindQueue(consumer_exchange_, consumer_queue_, consumer_routekey_);
  channel.consume(consumer_queue_, AMQP::noack)
      .onReceived([&](const AMQP::Message& m, uint64_t, bool) {
           std::vector<char> message(m.body(), m.body() + m.bodySize());
#ifdef LOG_DEBUG
          printf("Received data[%lu]: ", message.size());
          for (auto ch : message) {
            printf("%02X ", static_cast<uint8_t>(ch));
          }
          printf("\n");
#endif
          recv_message_.push_back(message);
      });
  ev_loop_ptr_ = &loop;
  connection_ptr_ = &connection;
  ev_run(loop, 0);
  // Waiting to exit the loop.
  connection.close();
  service_is_running_ = false;
}

}  // namespace librmqclient

