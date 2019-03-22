# require CMake 2.8 or greater
cmake_minimum_required(VERSION 2.8)

# Use C++11
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")

# declare detectnet-camera project
project(detectnet-camera)

file(GLOB detectnetCameraSources detectnet-camera.cpp)
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

# compile the detectnet-camera program
cuda_add_executable(detectnet-camera ${detectnetCameraSources})
# cuda_add_executable(detectnet-camera detectnet-camera.cpp)

# link detectnet-camera to jetson-inference library
include_directories(/usr/include/gstreamer-1.0 /usr/lib/aarch64-linux-gnu/gstreamer-1.0/include /usr/include/glib-2.0 /usr/include/libxml2 /usr/lib/aarch64-linux-gnu/glib-2.0/include/)
# target_include_directories(detectnet-camera PRIVATE ${GST_INCLUDE_DIRS})
message(GST_INCLUDE_DIRS=${GST_INCLUDE_DIRS})
target_link_libraries(detectnet-camera jetson-inference ${GST_LIBRARIES})
