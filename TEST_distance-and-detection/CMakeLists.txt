# License: Apache 2.0. See LICENSE file in root directory.
# Copyright(c) 2019 Intel Corporation. All Rights Reserved.
#  minimum required cmake version: 3.1.0
cmake_minimum_required(VERSION 3.1.0)

project(det_dist)

find_package(Threads REQUIRED)
find_package(realsense2 REQUIRED)
find_package(OpenCV REQUIRED)
find_package(jetson-utils)
find_package(jetson-inference)
find_package(CUDA)
find_package(Qt4)

# setup Qt4 for build
include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

include_directories(${realsense2_INCLUDE_DIRS} ${OpenCV_INCLUDE_DIRS})
# include_directories(/usr/include/gstreamer-1.0 /usr/lib/aarch64-linux-gnu/gstreamer-1.0/include /usr/include/glib-2.0 /usr/include/libxml2 /usr/lib/aarch64-linux-gnu/glib-2.0/include/)

# cuda_add_executable(USUPACR ${USUPACR_Sources})
cuda_add_executable(det_dist det_dist.cpp)

target_link_libraries(det_dist Threads::Threads jetson-inference ${realsense2_LIBRARY} ${OpenCV_LIBS} ${CMAKE_THREAD_LIBS_INIT})
# target_link_libraries(USUPACR jetson-inference ${CMAKE_THREAD_LIBS_INIT})

