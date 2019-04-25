# License: Apache 2.0. See LICENSE file in root directory.
# Copyright(c) 2019 Intel Corporation. All Rights Reserved.
#  minimum required cmake version: 3.1.0
cmake_minimum_required(VERSION 3.1.0)

project(pacr-follow)

file(GLOB pacr-follow_SOURCES pacr-follow.cpp ../src/*.cpp ../src/*.c)
# file(GLOB pacr-follow_INCLUDES ../include/*.hpp ../include/*.h )

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

include_directories(${realsense2_INCLUDE_DIRS} ${OpenCV_INCLUDE_DIRS} ../include)
# include_directories(/usr/include/gstreamer-1.0 /usr/lib/aarch64-linux-gnu/gstreamer-1.0/include /usr/include/glib-2.0 /usr/include/libxml2 /usr/lib/aarch64-linux-gnu/glib-2.0/include/)

# cuda_add_executable(USUPACR ${USUPACR_Sources})
cuda_add_executable(pacr-follow ${pacr-follow_SOURCES})

target_link_libraries(pacr-follow Threads::Threads jetson-inference ${realsense2_LIBRARY} ${OpenCV_LIBS} ${CMAKE_THREAD_LIBS_INIT})
# target_link_libraries(USUPACR jetson-inference ${CMAKE_THREAD_LIBS_INIT})

