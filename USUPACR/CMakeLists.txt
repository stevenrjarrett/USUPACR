cmake_minimum_required(VERSION 2.8)
project(USUPACR)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(OpenCV REQUIRED)
find_package (Threads REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(USUPACR main.cpp)

target_link_libraries(USUPACR ${OpenCV_LIBS})
target_link_libraries (USUPACR ${CMAKE_THREAD_LIBS_INIT})
