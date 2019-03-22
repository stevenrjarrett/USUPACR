# require CMake 2.8 or greater
cmake_minimum_required(VERSION 2.8)

# Use C++11
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")

# declare detectnet-camera-opencv project
project(detectnet-camera-opencv)

file(GLOB detectnetCameraSources detectnet-camera-opencv.cpp)
file(GLOB detectnetCameraIncludes *.h )

# import packages
find_package(jetson-utils)
find_package(jetson-inference)
find_package(CUDA REQUIRED)
find_package(Qt4 REQUIRED)
find_package(OpenCV REQUIRED)

# setup Qt4 for build
include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

# compile the detectnet-camera-opencv program
cuda_add_executable(detectnet-camera-opencv ${detectnetCameraSources})
# cuda_add_executable(detectnet-camera-opencv detectnet-camera-opencv.cpp)

# includes, required for ... gstreamer?
include_directories(/usr/include/gstreamer-1.0 /usr/lib/aarch64-linux-gnu/gstreamer-1.0/include /usr/include/glib-2.0 /usr/include/libxml2 /usr/lib/aarch64-linux-gnu/glib-2.0/include/ ${OpenCV_INCLUDE_DIRS})

# link detectnet-camera-opencv to jetson-inference library
target_link_libraries(detectnet-camera-opencv jetson-inference ${GST_LIBRARIES} ${OpenCV_LIBS})
