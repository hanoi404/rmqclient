cmake_minimum_required(VERSION 2.8)


add_executable(rmq_sender
  rmq_sender.cc
)
target_link_libraries(rmq_sender PRIVATE
  rmqclient
)

add_executable(rmq_receiver
  rmq_receiver.cc
)
target_link_libraries(rmq_receiver PRIVATE
  rmqclient
)

