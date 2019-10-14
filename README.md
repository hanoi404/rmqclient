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
# cmake ../src && make
```

## Secure Connect with SSL

Certificate file dependency, make your own certificate file and replace the listed file:

```bash
/etc/rabbitmq/ssl/ca/cacert.pem
/etc/rabbitmq/ssl/client/rabbitmq_client.cert.pem
/etc/rabbitmq/ssl/client/rabbitmq_client.key.pem
```  

Enable:

In AMQP::Address object, using the `amqps://` protocol:

```cpp
AMQP::Address address("amqps://guest:guest@localhost/");
```

## Reference

- [AMQPCPP](https://github.com/CopernicaMarketingSoftware/AMQP-CPP)
- [examples.amqp-cpp](https://github.com/hoxnox/examples.amqp-cpp)
