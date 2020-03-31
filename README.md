# rmqclient
RabbitMQ client example program using amqpcpp.

## Quick Start

Test environment:

- ubuntu 16.04
- cmake 3.5.1
- rabbitmq-server 3.5.7

Build:

```bash
# mkdir build && cd build
# cmake .. && make
```

BUild example:

```bash
# cd build
# cmake .. -DRMQCLIENT_BUILD_EXAMPLES=ON && make
./examples/rmq_receiver
./examples/rmq_sender
```

## Reference

- [AMQPCPP](https://github.com/CopernicaMarketingSoftware/AMQP-CPP)
- [examples.amqp-cpp](https://github.com/hoxnox/examples.amqp-cpp)
