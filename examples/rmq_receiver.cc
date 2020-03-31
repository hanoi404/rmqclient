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


#include <stdio.h>
#include <unistd.h>

#include <vector>

#include "rmq_client.h"


using librmqclient::RmqClient;

int main(void) {
  RmqClient rmq_client;
  rmq_client.InitConsumerConnect("amqp://guest:guest@localhost/");
  rmq_client.InitConsumer("exchange", "queue", "routekey");
  rmq_client.Run();

  std::vector<char> msg;
  while (1) {
    if (rmq_client.message_num() > 0) {
      if (rmq_client.message(&msg) == 0) {
        msg.push_back(0x0);
        printf("Recv: %s\n", &msg.front());
        break;
      }
    }
    sleep(1);
  }
  rmq_client.Stop();

  return 0;
}

