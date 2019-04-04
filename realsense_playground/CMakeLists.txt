# License: Apache 2.0. See LICENSE file in root directory.
# Copyright(c) 2019 Intel Corporation. All Rights Reserved.
#  minimum required cmake version: 3.1.0
cmake_minimum_required(VERSION 3.1.0)

project(rs-playground)

find_package(Threads REQUIRED)
find_package(realsense2 REQUIRED)
find_package(OpenCV REQUIRED)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

include_directories(${realsense2_INCLUDE_DIRS} ${OpenCV_INCLUDE_DIRS})

add_executable(rs-playground rs-playground.cpp)

target_link_libraries(rs-playground Threads::Threads ${realsense2_LIBRARY} ${OpenCV_LIBS})

