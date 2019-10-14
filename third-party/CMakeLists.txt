cmake_minimum_required(VERSION 2.8)

add_definitions("-std=c++11 -pthread")

########################################################################
# RabbitMQ

include(ExternalProject)
set(AMQPCPP_ROOT "${CMAKE_CURRENT_BINARY_DIR}/amqpcpp")
ExternalProject_Add(libamqpcpp
    # URL "https://github.com/CopernicaMarketingSoftware/AMQP-CPP/archive/v4.1.5.tar.gz"
    # URL "${CMAKE_CURRENT_SOURCE_DIR}/libamqpcpp-4.1.5.tar.gz"
    # URL_MD5 "66fa995d432adade65ea6274799f2bed"
    URL "${CMAKE_CURRENT_SOURCE_DIR}/libamqpcpp-4.1.5-ssl-fixed.tar.gz"
    URL_MD5 "2b70ad94afd09424e0e274dddf12733e"
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
# OpenSSL

set(OpenSSL_ROOT "${CMAKE_CURRENT_BINARY_DIR}/openssl")
ExternalProject_Add(openssl
  # URL "https://www.openssl.org/source/openssl-1.1.0h.tar.gz"
  URL "${CMAKE_CURRENT_SOURCE_DIR}/openssl-1.1.0h.tar.gz"
  URL_MD5 "5271477e4d93f4ea032b665ef095ff24"
  CONFIGURE_COMMAND <SOURCE_DIR>/Configure linux-x86_64 --prefix=<INSTALL_DIR> --openssldir=<INSTALL_DIR>/lib/ssl
  no-weak-ssl-ciphers enable-ec_nistp_64_gcc_128 no-shared
  # BUILD_COMMAND make "-ldl"
  INSTALL_DIR ${OpenSSL_ROOT}
)
file(MAKE_DIRECTORY ${OpenSSL_ROOT}/include)
message(STATUS "Found external dependency SSL: " ${OpenSSL_ROOT}/include)
add_library(openssl-crypto-static STATIC IMPORTED GLOBAL)
set_property(TARGET openssl-crypto-static PROPERTY IMPORTED_LOCATION ${OpenSSL_ROOT}/lib/libcrypto.a)
# set_property(TARGET openssl-crypto-static PROPERTY IMPORTED_INTERFACE_LINK_LIBRARIES pthread)
set_property(TARGET openssl-crypto-static PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${OpenSSL_ROOT}/include)
add_library(openssl-ssl-static STATIC IMPORTED GLOBAL)
set_property(TARGET openssl-ssl-static PROPERTY IMPORTED_LOCATION ${OpenSSL_ROOT}/lib/libssl.a)
# set_property(TARGET openssl-ssl-static PROPERTY IMPORTED_INTERFACE_LINK_LIBRARIES pthread)
set_property(TARGET openssl-ssl-static PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${OpenSSL_ROOT}/include)


########################################################################

########################################################################
# LibEv

set(Ev_ROOT "${CMAKE_CURRENT_BINARY_DIR}/ev")
ExternalProject_Add(libev
    # URL "https://fossies.org/linux/misc/libev-4.27.tar.gz"
    URL "${CMAKE_CURRENT_SOURCE_DIR}/libev-4.27.tar.gz"
    URL_MD5 "d38925fbc030153fe674c4e0e864a69a"
    # PATCH_COMMAND <SOURCE_DIR>/autogen.sh
    CONFIGURE_COMMAND <SOURCE_DIR>/configure --prefix=<INSTALL_DIR> -enable-static
    INSTALL_DIR ${Ev_ROOT}
)
file(MAKE_DIRECTORY ${Ev_ROOT}/include)
message(STATUS "Found external dependency Ev: " ${Ev_ROOT}/include)
add_library(libev-static STATIC IMPORTED GLOBAL)
set_property(TARGET libev-static PROPERTY IMPORTED_LOCATION ${Ev_ROOT}/lib/libev.a)
# set_property(TARGET libev-static PROPERTY IMPORTED_INTERFACE_LINK_LIBRARIES pthread crypto)
set_property(TARGET libev-static PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${Ev_ROOT}/include)

########################################################################
