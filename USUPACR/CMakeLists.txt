# require CMake 2.8 or greater
cmake_minimum_required(VERSION 2.8)

# Use C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# declare USUPACR project
project(USUPACR)

file(GLOB USUPACR_Sources main.cpp)
file(GLOB USUPACR_Includes *.h )

# import jetson-inference and jetson-utils packages.
# note that if you didn't do "sudo make install"
# while building jetson-inference, this will error.
find_package(jetson-utils)
find_package(jetson-inference)

# CUDA and Qt4 are required
find_package(CUDA)
find_package(Qt4)
find_package (Threads REQUIRED)

# Find gstreamer
# find_package(gstreamer-1.0)

# setup Qt4 for build
include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

# compile the USUPACR program
cuda_add_executable(USUPACR ${USUPACR_Sources})
# cuda_add_executable(USUPACR USUPACR.cpp)

# link USUPACR to jetson-inference library
include_directories(/usr/include/gstreamer-1.0 /usr/lib/aarch64-linux-gnu/gstreamer-1.0/include /usr/include/glib-2.0 /usr/include/libxml2 /usr/lib/aarch64-linux-gnu/glib-2.0/include/)
# target_include_directories(USUPACR PRIVATE ${GST_INCLUDE_DIRS})
target_link_libraries(USUPACR jetson-inference ${CMAKE_THREAD_LIBS_INIT})
