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

#ifndef RMQCLIENT_RMQ_CLIENT_H_
#define RMQCLIENT_RMQ_CLIENT_H_

#include <string>
#include <list>
#include <vector>
#include <thread>  // NOLINT


namespace librmqclient {

// URL example:
//   'amqp://guest:guest@localhost/'
//   'amqp://guest:guest@localhost/vhost'
//   'amqps://guest:guest@localhost/'
//   'amqps://guest:guest@localhost/vhost'
// Description:
//   @amqp           ordinary tcp connection type (port 5672);
//   @amqps          secure connection with SSL/TLS (port 5671);
//   @guest:guest    username:password;
//   @localhost      hostname or ip;
//   @vhost          virtual host, optional.
//

class RmqClient {
 public:
  // Constructor.
  //
  RmqClient() = default;
  RmqClient(const std::string &consumer_url,
            const std::string &consumer_exchange,
            const std::string &consumer_queue,
            const std::string &consumer_routekey,
            const std::string &producer_url,
            const std::string &producer_exchange,
            const std::string &producer_routekey) :
      consumer_url_(consumer_url),
      consumer_exchange_(consumer_exchange),
      consumer_queue_(consumer_queue),
      consumer_routekey_(consumer_routekey),
      producer_url_(producer_url),
      producer_exchange_(producer_exchange),
      producer_routekey_(producer_routekey) {
  }
  // Consumer only.
  RmqClient(const std::string &consumer_url,
            const std::string &consumer_exchange,
            const std::string &consumer_queue,
            const std::string &consumer_routekey) :
      consumer_url_(consumer_url),
      consumer_exchange_(consumer_exchange),
      consumer_queue_(consumer_queue),
      consumer_routekey_(consumer_routekey) {
  }
  // Producer only.
  RmqClient(const std::string &producer_url,
            const std::string &producer_exchange,
            const std::string &producer_routekey) :
      producer_url_(producer_url),
      producer_exchange_(producer_exchange),
      producer_routekey_(producer_routekey) {
  }
  // RmqClient is neither copyable nor movable.
  RmqClient(const RmqClient &) = delete;
  RmqClient& operator=(const RmqClient &) = delete;

  // Destructor.
  //
  ~RmqClient();

  // Manual initialization.
  //
  // If using the default constructor, have to use this method to
  // init socket connection information.
  void InitConsumerConnect(const std::string &url) {
    consumer_url_ = url;
  }
  void InitProducerConnect(const std::string &url) {
    producer_url_ = url;
  }
  // If using the default constructor, select one of the following
  // initialization methods.
  void InitConsumer(const std::string &exchange, const std::string &queue,
                    const std::string &routekey) {
    consumer_exchange_ = exchange;
    consumer_queue_ = queue;
    consumer_routekey_ = routekey;
  }
  void InitProducer(const std::string &exchange, const std::string &routekey) {
    producer_exchange_ = exchange;
    producer_routekey_ = routekey;
  }

  // For producer.
  //
  // Use the specified 'exchange' and 'routekey'.
  int SendMessage(const std::string &exchange, const std::string &routekey,
                  const char *message, const int &size) {
    producer_exchange_ = exchange;
    producer_routekey_ = routekey;
    return SendMessage(message, size);
  }
  int SendMessage(const std::string &exchange, const std::string &routekey,
                  const std::vector<char> &message) {
    return SendMessage(exchange, routekey, &message.front(), message.size());
  }
  // Common send message method.
  int SendMessage(const char *message, const int &size);
  int SendMessage(const std::vector<char> &message) {
    return SendMessage(&message.front(), message.size());
  }

  // For consumer.
  //
  // Current message count.
  int message_num(void) const {
    return recv_message_.size();
  }
  // Get the first message in queue.
  int message(std::vector<char> *data) {
    if (recv_message_.empty()) {
      return -1;
    }
    data->clear();
    data->assign(recv_message_.front().begin(), recv_message_.front().end());
    recv_message_.pop_front();
    return 0;
  }
  bool service_is_running(void) { return service_is_running_; }
  void Run(void);
  void Stop(void);

 private:
  // Thread handler.
  void RecvService(void);

  bool service_is_running_ = false;
  // RabbitMQ parameters.
  std::string consumer_url_ = "";
  std::string consumer_exchange_ = "";
  std::string consumer_queue_ = "";
  std::string consumer_routekey_ = "";
  std::string producer_url_ = "";
  std::string producer_exchange_ = "";
  std::string producer_routekey_ = "";
  // Run control.
  void *connection_ptr_ = nullptr;
  std::thread service_thread_;
  std::list<std::vector<char>> recv_message_;
};

}  // namespace librmqclient

#endif  // RMQCLIENT_RMQ_CLIENT_H_
