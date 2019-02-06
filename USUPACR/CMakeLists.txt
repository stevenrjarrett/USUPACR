cmake_minimum_required(VERSION 2.8)

project(USUPACR)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(USUPACR main.cpp)

target_link_libraries(USUPACR ${OpenCV_LIBS})
