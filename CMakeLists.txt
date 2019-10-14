cmake_minimum_required(VERSION 2.8)

project(rmqclient)

set (VERSION_MAJOR 1)
set (VERSION_MINOR 0)
set (VERSION_PATCH 0)

option(RMQCLIENT_BUILD_EXAMPLES "Build rmqclient examples" OFF)

add_definitions("-std=c++11 -pthread")

include_directories(SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_subdirectory(src)
add_subdirectory(third-party)

if (RMQCLIENT_BUILD_EXAMPLES)
add_subdirectory(examples)
endif (RMQCLIENT_BUILD_EXAMPLES)

