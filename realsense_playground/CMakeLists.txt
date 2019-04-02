# License: Apache 2.0. See LICENSE file in root directory.
# Copyright(c) 2019 Intel Corporation. All Rights Reserved.
#  minimum required cmake version: 3.1.0
cmake_minimum_required(VERSION 3.1.0)

project(rs-play)

find_package(Threads REQUIRED)
find_package(realsense2 REQUIRED)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

add_executable(rs-play rs-measure.cpp ./example.hpp)

target_link_libraries(rs-measure Threads::Threads $(realsense2_LIBRARY))
