cmake_minimum_required(VERSION 2.8)

project(OPENCV_Sandbox)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(OPENCV_Sandbox main.cpp)

target_link_libraries(OPENCV_Sandbox ${OpenCV_LIBS})
