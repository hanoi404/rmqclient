cmake_minimum_required(VERSION 2.8)

add_definitions("-std=c++11 -pthread")

add_executable(rmq_sender
  rmq_sender.cc
)
add_dependencies(rmq_sender libamqpcpp libev openssl)
target_link_libraries(rmq_sender PRIVATE
  openssl-ssl-static
  openssl-crypto-static
  libev-static
  amqpcpp-static
  pthread
  dl
)

add_executable(rmq_receiver
  rmq_receiver.cc
)
add_dependencies(rmq_receiver libamqpcpp libev openssl)
target_link_libraries(rmq_receiver PRIVATE
  openssl-ssl-static
  openssl-crypto-static
  libev-static
  amqpcpp-static
  pthread
  dl
)

