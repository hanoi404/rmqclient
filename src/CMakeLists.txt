cmake_minimum_required(VERSION 2.8)

# Build
set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/bin)

add_library(${PROJECT_NAME}
  rmq_client.cc
)

add_dependencies(${PROJECT_NAME} libamqpcpp libev openssl)

target_link_libraries(${PROJECT_NAME} PRIVATE
  openssl-ssl-static
  openssl-crypto-static
  libev-static
  amqpcpp-static
  pthread
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
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}}/../include/>
  $<INSTALL_INTERFACE:include/>
)
