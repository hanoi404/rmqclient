cmake_minimum_required(VERSION 2.8)

project(rmqclient)

set (VERSION_MAJOR 1)
set (VERSION_MINOR 0)
set (VERSION_PATCH 0)

option(RMQCLIENT_BUILD_EXAMPLES "Build rmqclient examples" OFF)

add_definitions("-std=c++11 -pthread")

set(CMAKE_LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/lib)
set(CMAKE_INCLUDE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/include)

set(rmqclient_include_dirs ${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(SYSTEM ${rmqclient_include_dirs})

add_subdirectory(src)
add_subdirectory(third-party)

if (RMQCLIENT_BUILD_EXAMPLES)
add_subdirectory(examples)
endif (RMQCLIENT_BUILD_EXAMPLES)

