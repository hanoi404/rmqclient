cmake_minimum_required(VERSION 2.8)

# Build

add_library(${PROJECT_NAME}
  rmq_client.cc
)

add_dependencies(${PROJECT_NAME} libamqpcpp openssl libevent)

target_link_libraries(${PROJECT_NAME} PRIVATE
  openssl-ssl-static
  openssl-crypto-static
  amqpcpp-static
  pthread
  event-static
  event-openssl-static
  event-pthreads-static
  dl
)


# Install

install(TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}Config ARCHIVE DESTINATION lib
)

# install(DIRECTORY include DESTINATION include
#   FILES_MATCHING PATTERN "*.h"
# )
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/../include/rmq_client.h
  DESTINATION include
)

install(EXPORT ${PROJECT_NAME}Config DESTINATION cmake)
export(TARGETS ${PROJECT_NAME} FILE ${PROJECT_NAME}Config.cmake)

set(DEST_DIR "${CMAKE_INSTALL_PREFIX}")

target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC
  $<BUILD_INTERFACE:${rmqclient_include_dirs}>
  $<INSTALL_INTERFACE:${CMAKE_OUTPUT_INCLUDE_DIR}>
)
