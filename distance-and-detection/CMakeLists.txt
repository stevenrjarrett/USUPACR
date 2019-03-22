# require CMake 2.8 or greater
cmake_minimum_required(VERSION 2.8)

# Use C++11
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")

# declare detectnet-distance project
project(detectnet-distance)

file(GLOB detectnetCameraSources detectnet-distance.cpp)
file(GLOB detectnetCameraIncludes *.h )

# import jetson-inference and jetson-utils packages.
# note that if you didn't do "sudo make install"
# while building jetson-inference, this will error.
find_package(jetson-utils)
find_package(jetson-inference)

# CUDA and Qt4 are required
find_package(CUDA)
find_package(Qt4)

# Find gstreamer
# find_package(gstreamer-1.0)

# setup Qt4 for build
include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

# compile the detectnet-distance program
cuda_add_executable(detectnet-distance ${detectnetCameraSources})
# cuda_add_executable(detectnet-distance detectnet-distance.cpp)

# link detectnet-distance to jetson-inference library
include_directories(/usr/include/gstreamer-1.0 /usr/lib/aarch64-linux-gnu/gstreamer-1.0/include /usr/include/glib-2.0 /usr/include/libxml2 /usr/lib/aarch64-linux-gnu/glib-2.0/include/)
# target_include_directories(detectnet-distance PRIVATE ${GST_INCLUDE_DIRS})
message(GST_INCLUDE_DIRS=${GST_INCLUDE_DIRS})
target_link_libraries(detectnet-distance jetson-inference ${GST_LIBRARIES})
