cmake_minimum_required(VERSION 2.8)

add_definitions("-std=c++11 -pthread")

########################################################################
# RabbitMQ

include(ExternalProject)
set(AMQPCPP_ROOT "${CMAKE_CURRENT_BINARY_DIR}/amqpcpp")
ExternalProject_Add(libamqpcpp
  # URL "https://github.com/CopernicaMarketingSoftware/AMQP-CPP/archive/v4.1.5.tar.gz"
  # URL_MD5 "66fa995d432adade65ea6274799f2bed"
  URL "${CMAKE_CURRENT_SOURCE_DIR}/AMQP-CPP-4.1.6.tar.gz"
  URL_MD5 "f8951c111ea3ae2bca188e83295e7200"
  CMAKE_ARGS -Wno-dev "-DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>" "-DAMQP-CPP_LINUX_TCP=ON"
  INSTALL_DIR ${AMQPCPP_ROOT}
)
file(MAKE_DIRECTORY ${AMQPCPP_ROOT}/include)
message(STATUS "Found external dependency AMQP-CPP: " ${AMQPCPP_ROOT}/include)
add_library(amqpcpp-static STATIC IMPORTED GLOBAL)
set_property(TARGET amqpcpp-static PROPERTY IMPORTED_LOCATION ${AMQPCPP_ROOT}/lib/libamqpcpp.a)
set_property(TARGET amqpcpp-static PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${AMQPCPP_ROOT}/include)

########################################################################


########################################################################
# LibEvent

set(Event_ROOT "${CMAKE_CURRENT_BINARY_DIR}/event")
ExternalProject_Add(libevent
  # URL "https://github.com/libevent/libevent/archive/release-2.0.22-stable.tar.gz"
  # URL_MD5 "8913ef56ec329f2c046007bd634c7201"
  URL "${CMAKE_CURRENT_SOURCE_DIR}/libevent-2.1.11-stable.tar.gz"
  URL_MD5 "7f35cfe69b82d879111ec0d7b7b1c531"
  PATCH_COMMAND <SOURCE_DIR>/autogen.sh
  CONFIGURE_COMMAND <SOURCE_DIR>/configure --disable-shared --prefix <INSTALL_DIR>
    OPENSSL_CFLAGS=-I${OpenSSL_ROOT}/include OPENSSL_LIBS="-L${OpenSSL_ROOT}/lib -lcrypto -ldl"
  INSTALL_DIR ${Event_ROOT}
)
file(MAKE_DIRECTORY ${Event_ROOT}/include)
message(STATUS "Found external dependency Event: " ${Event_ROOT}/include)
add_library(event-static STATIC IMPORTED GLOBAL)
set_property(TARGET event-static PROPERTY IMPORTED_LOCATION ${Event_ROOT}/lib/libevent.a)
set_property(TARGET event-static PROPERTY IMPORTED_INTERFACE_LINK_LIBRARIES pthread crypto)
set_property(TARGET event-static PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${Event_ROOT}/include)

add_library(event-pthreads-static STATIC IMPORTED GLOBAL)
set_property(TARGET event-pthreads-static PROPERTY IMPORTED_LOCATION ${Event_ROOT}/lib/libevent_pthreads.a)
set_property(TARGET event-pthreads-static PROPERTY IMPORTED_INTERFACE_LINK_LIBRARIES pthread crypto)
set_property(TARGET event-pthreads-static PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${Event_ROOT}/include)

########################################################################
