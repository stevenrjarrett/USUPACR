# License: Apache 2.0. See LICENSE file in root directory.
# Copyright(c) 2019 Intel Corporation. All Rights Reserved.
#  minimum required cmake version: 3.1.0
cmake_minimum_required(VERSION 3.1.0)

project(pacr-manual)

# files / directories
file(GLOB pacr-manual_SOURCES main.cpp ../src/*.cpp ../src/*.c ./src/*.cpp ./src/*.c)

include_directories(${CMAKE_BINARY_DIR} ../include ./include ./)
# file(GLOB pacr-follow_INCLUDES ../include/*.hpp ../include/*.h )

find_package(Threads REQUIRED)


set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# include_directories(/usr/include/gstreamer-1.0 /usr/lib/aarch64-linux-gnu/gstreamer-1.0/include /usr/include/glib-2.0 /usr/include/libxml2 /usr/lib/aarch64-linux-gnu/glib-2.0/include/)

# cuda_add_executable(pacr-manual ${pacr-manual_Sources})
add_executable(pacr-manual ${pacr-manual_SOURCES})

target_link_libraries(pacr-manual Threads::Threads ${CMAKE_THREAD_LIBS_INIT})
# target_link_libraries(pacr-manual jetson-inference ${CMAKE_THREAD_LIBS_INIT})

