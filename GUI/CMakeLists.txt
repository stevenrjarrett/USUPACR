# require CMake 2.8 or greater
cmake_minimum_required(VERSION 2.8)

# Use C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_CXX_EXTENSIONS OFF)

# declaring files in your project

set ( SOURCES
 pacr-gui.cpp
 mainwindow.cpp
)

set ( MOC_HEADERS
 mainwindow.h
)

set ( UIS
 mainwindow.ui
)

set ( RESOURCES
 main.qrc
)

# Next lines needed for building all Qt projects
find_package( Qt4 REQUIRED )
include( ${QT_USE_FILE} )
add_definitions( ${QT_DEFINITIONS} )
include_directories( ${CMAKE_BINARY_DIR} )


# Next, using precompiler, compiler and linker

# using Qt meta-system (precompiler)
QT4_ADD_RESOURCES( RES_SOURCES ${RESOURCES} )
QT4_WRAP_UI( UI_HEADERS ${UIS} )
QT4_WRAP_CPP( MOC_SRCS ${MOC_HEADERS} )

# compile
add_executable( pacr-gui ${SOURCES} ${MOC_SRCS} ${RES_SOURCES} ${UI_HEADERS} )
# or use line below instead, if you using Windows ™ Operating System.
#add_executable( pacr-gui WIN32 ${SOURCES} ${MOC_SRCS} ${RES_SOURCES} ${UI_HEADERS} )

# build it (link libraries)
target_link_libraries( pacr-gui ${QT_LIBRARIES} )
