# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/nvidia/Documents/USUPACR/follow-demo

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/nvidia/Documents/USUPACR/follow-demo/build

# Include any dependencies generated for this target.
include CMakeFiles/pacr-follow.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/pacr-follow.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/pacr-follow.dir/flags.make

CMakeFiles/pacr-follow.dir/pacr-follow.cpp.o: CMakeFiles/pacr-follow.dir/flags.make
CMakeFiles/pacr-follow.dir/pacr-follow.cpp.o: ../pacr-follow.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/nvidia/Documents/USUPACR/follow-demo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/pacr-follow.dir/pacr-follow.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/pacr-follow.dir/pacr-follow.cpp.o -c /home/nvidia/Documents/USUPACR/follow-demo/pacr-follow.cpp

CMakeFiles/pacr-follow.dir/pacr-follow.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/pacr-follow.dir/pacr-follow.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/nvidia/Documents/USUPACR/follow-demo/pacr-follow.cpp > CMakeFiles/pacr-follow.dir/pacr-follow.cpp.i

CMakeFiles/pacr-follow.dir/pacr-follow.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/pacr-follow.dir/pacr-follow.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/nvidia/Documents/USUPACR/follow-demo/pacr-follow.cpp -o CMakeFiles/pacr-follow.dir/pacr-follow.cpp.s

CMakeFiles/pacr-follow.dir/pacr-follow.cpp.o.requires:

.PHONY : CMakeFiles/pacr-follow.dir/pacr-follow.cpp.o.requires

CMakeFiles/pacr-follow.dir/pacr-follow.cpp.o.provides: CMakeFiles/pacr-follow.dir/pacr-follow.cpp.o.requires
	$(MAKE) -f CMakeFiles/pacr-follow.dir/build.make CMakeFiles/pacr-follow.dir/pacr-follow.cpp.o.provides.build
.PHONY : CMakeFiles/pacr-follow.dir/pacr-follow.cpp.o.provides

CMakeFiles/pacr-follow.dir/pacr-follow.cpp.o.provides.build: CMakeFiles/pacr-follow.dir/pacr-follow.cpp.o


CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/cameraDetection.cpp.o: CMakeFiles/pacr-follow.dir/flags.make
CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/cameraDetection.cpp.o: /home/nvidia/Documents/USUPACR/src/cameraDetection.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/nvidia/Documents/USUPACR/follow-demo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/cameraDetection.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/cameraDetection.cpp.o -c /home/nvidia/Documents/USUPACR/src/cameraDetection.cpp

CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/cameraDetection.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/cameraDetection.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/nvidia/Documents/USUPACR/src/cameraDetection.cpp > CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/cameraDetection.cpp.i

CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/cameraDetection.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/cameraDetection.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/nvidia/Documents/USUPACR/src/cameraDetection.cpp -o CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/cameraDetection.cpp.s

CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/cameraDetection.cpp.o.requires:

.PHONY : CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/cameraDetection.cpp.o.requires

CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/cameraDetection.cpp.o.provides: CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/cameraDetection.cpp.o.requires
	$(MAKE) -f CMakeFiles/pacr-follow.dir/build.make CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/cameraDetection.cpp.o.provides.build
.PHONY : CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/cameraDetection.cpp.o.provides

CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/cameraDetection.cpp.o.provides.build: CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/cameraDetection.cpp.o


CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/XBoxOne.cpp.o: CMakeFiles/pacr-follow.dir/flags.make
CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/XBoxOne.cpp.o: /home/nvidia/Documents/USUPACR/src/XBoxOne.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/nvidia/Documents/USUPACR/follow-demo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/XBoxOne.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/XBoxOne.cpp.o -c /home/nvidia/Documents/USUPACR/src/XBoxOne.cpp

CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/XBoxOne.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/XBoxOne.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/nvidia/Documents/USUPACR/src/XBoxOne.cpp > CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/XBoxOne.cpp.i

CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/XBoxOne.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/XBoxOne.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/nvidia/Documents/USUPACR/src/XBoxOne.cpp -o CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/XBoxOne.cpp.s

CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/XBoxOne.cpp.o.requires:

.PHONY : CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/XBoxOne.cpp.o.requires

CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/XBoxOne.cpp.o.provides: CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/XBoxOne.cpp.o.requires
	$(MAKE) -f CMakeFiles/pacr-follow.dir/build.make CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/XBoxOne.cpp.o.provides.build
.PHONY : CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/XBoxOne.cpp.o.provides

CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/XBoxOne.cpp.o.provides.build: CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/XBoxOne.cpp.o


CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/personTracker.cpp.o: CMakeFiles/pacr-follow.dir/flags.make
CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/personTracker.cpp.o: /home/nvidia/Documents/USUPACR/src/personTracker.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/nvidia/Documents/USUPACR/follow-demo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/personTracker.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/personTracker.cpp.o -c /home/nvidia/Documents/USUPACR/src/personTracker.cpp

CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/personTracker.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/personTracker.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/nvidia/Documents/USUPACR/src/personTracker.cpp > CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/personTracker.cpp.i

CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/personTracker.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/personTracker.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/nvidia/Documents/USUPACR/src/personTracker.cpp -o CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/personTracker.cpp.s

CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/personTracker.cpp.o.requires:

.PHONY : CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/personTracker.cpp.o.requires

CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/personTracker.cpp.o.provides: CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/personTracker.cpp.o.requires
	$(MAKE) -f CMakeFiles/pacr-follow.dir/build.make CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/personTracker.cpp.o.provides.build
.PHONY : CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/personTracker.cpp.o.provides

CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/personTracker.cpp.o.provides.build: CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/personTracker.cpp.o


CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/stopwatch.cpp.o: CMakeFiles/pacr-follow.dir/flags.make
CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/stopwatch.cpp.o: /home/nvidia/Documents/USUPACR/src/stopwatch.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/nvidia/Documents/USUPACR/follow-demo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/stopwatch.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/stopwatch.cpp.o -c /home/nvidia/Documents/USUPACR/src/stopwatch.cpp

CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/stopwatch.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/stopwatch.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/nvidia/Documents/USUPACR/src/stopwatch.cpp > CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/stopwatch.cpp.i

CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/stopwatch.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/stopwatch.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/nvidia/Documents/USUPACR/src/stopwatch.cpp -o CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/stopwatch.cpp.s

CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/stopwatch.cpp.o.requires:

.PHONY : CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/stopwatch.cpp.o.requires

CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/stopwatch.cpp.o.provides: CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/stopwatch.cpp.o.requires
	$(MAKE) -f CMakeFiles/pacr-follow.dir/build.make CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/stopwatch.cpp.o.provides.build
.PHONY : CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/stopwatch.cpp.o.provides

CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/stopwatch.cpp.o.provides.build: CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/stopwatch.cpp.o


CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/libenjoy_linux.c.o: CMakeFiles/pacr-follow.dir/flags.make
CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/libenjoy_linux.c.o: /home/nvidia/Documents/USUPACR/src/libenjoy_linux.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/nvidia/Documents/USUPACR/follow-demo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building C object CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/libenjoy_linux.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/libenjoy_linux.c.o   -c /home/nvidia/Documents/USUPACR/src/libenjoy_linux.c

CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/libenjoy_linux.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/libenjoy_linux.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/nvidia/Documents/USUPACR/src/libenjoy_linux.c > CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/libenjoy_linux.c.i

CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/libenjoy_linux.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/libenjoy_linux.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/nvidia/Documents/USUPACR/src/libenjoy_linux.c -o CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/libenjoy_linux.c.s

CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/libenjoy_linux.c.o.requires:

.PHONY : CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/libenjoy_linux.c.o.requires

CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/libenjoy_linux.c.o.provides: CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/libenjoy_linux.c.o.requires
	$(MAKE) -f CMakeFiles/pacr-follow.dir/build.make CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/libenjoy_linux.c.o.provides.build
.PHONY : CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/libenjoy_linux.c.o.provides

CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/libenjoy_linux.c.o.provides.build: CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/libenjoy_linux.c.o


CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/libenjoy.c.o: CMakeFiles/pacr-follow.dir/flags.make
CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/libenjoy.c.o: /home/nvidia/Documents/USUPACR/src/libenjoy.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/nvidia/Documents/USUPACR/follow-demo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building C object CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/libenjoy.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/libenjoy.c.o   -c /home/nvidia/Documents/USUPACR/src/libenjoy.c

CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/libenjoy.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/libenjoy.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/nvidia/Documents/USUPACR/src/libenjoy.c > CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/libenjoy.c.i

CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/libenjoy.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/libenjoy.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/nvidia/Documents/USUPACR/src/libenjoy.c -o CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/libenjoy.c.s

CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/libenjoy.c.o.requires:

.PHONY : CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/libenjoy.c.o.requires

CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/libenjoy.c.o.provides: CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/libenjoy.c.o.requires
	$(MAKE) -f CMakeFiles/pacr-follow.dir/build.make CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/libenjoy.c.o.provides.build
.PHONY : CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/libenjoy.c.o.provides

CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/libenjoy.c.o.provides.build: CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/libenjoy.c.o


# Object files for target pacr-follow
pacr__follow_OBJECTS = \
"CMakeFiles/pacr-follow.dir/pacr-follow.cpp.o" \
"CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/cameraDetection.cpp.o" \
"CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/XBoxOne.cpp.o" \
"CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/personTracker.cpp.o" \
"CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/stopwatch.cpp.o" \
"CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/libenjoy_linux.c.o" \
"CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/libenjoy.c.o"

# External object files for target pacr-follow
pacr__follow_EXTERNAL_OBJECTS =

pacr-follow: CMakeFiles/pacr-follow.dir/pacr-follow.cpp.o
pacr-follow: CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/cameraDetection.cpp.o
pacr-follow: CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/XBoxOne.cpp.o
pacr-follow: CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/personTracker.cpp.o
pacr-follow: CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/stopwatch.cpp.o
pacr-follow: CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/libenjoy_linux.c.o
pacr-follow: CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/libenjoy.c.o
pacr-follow: CMakeFiles/pacr-follow.dir/build.make
pacr-follow: /usr/local/cuda-9.0/lib64/libcudart_static.a
pacr-follow: /usr/lib/aarch64-linux-gnu/librt.so
pacr-follow: /usr/local/lib/libjetson-inference.so
pacr-follow: /usr/local/lib/librealsense2.so.2.10.4
pacr-follow: /usr/lib/libopencv_dnn.so.3.3.1
pacr-follow: /usr/lib/libopencv_ml.so.3.3.1
pacr-follow: /usr/lib/libopencv_objdetect.so.3.3.1
pacr-follow: /usr/lib/libopencv_shape.so.3.3.1
pacr-follow: /usr/lib/libopencv_stitching.so.3.3.1
pacr-follow: /usr/lib/libopencv_superres.so.3.3.1
pacr-follow: /usr/lib/libopencv_videostab.so.3.3.1
pacr-follow: /usr/local/lib/libjetson-utils.so
pacr-follow: /usr/local/cuda-9.0/lib64/libcudart_static.a
pacr-follow: /usr/lib/aarch64-linux-gnu/librt.so
pacr-follow: /usr/lib/aarch64-linux-gnu/libQtGui.so
pacr-follow: /usr/lib/aarch64-linux-gnu/libQtCore.so
pacr-follow: /usr/lib/libopencv_calib3d.so.3.3.1
pacr-follow: /usr/lib/libopencv_features2d.so.3.3.1
pacr-follow: /usr/lib/libopencv_flann.so.3.3.1
pacr-follow: /usr/lib/libopencv_highgui.so.3.3.1
pacr-follow: /usr/lib/libopencv_photo.so.3.3.1
pacr-follow: /usr/lib/libopencv_video.so.3.3.1
pacr-follow: /usr/lib/libopencv_videoio.so.3.3.1
pacr-follow: /usr/lib/libopencv_imgcodecs.so.3.3.1
pacr-follow: /usr/lib/libopencv_imgproc.so.3.3.1
pacr-follow: /usr/lib/libopencv_core.so.3.3.1
pacr-follow: CMakeFiles/pacr-follow.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/nvidia/Documents/USUPACR/follow-demo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Linking CXX executable pacr-follow"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/pacr-follow.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/pacr-follow.dir/build: pacr-follow

.PHONY : CMakeFiles/pacr-follow.dir/build

CMakeFiles/pacr-follow.dir/requires: CMakeFiles/pacr-follow.dir/pacr-follow.cpp.o.requires
CMakeFiles/pacr-follow.dir/requires: CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/cameraDetection.cpp.o.requires
CMakeFiles/pacr-follow.dir/requires: CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/XBoxOne.cpp.o.requires
CMakeFiles/pacr-follow.dir/requires: CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/personTracker.cpp.o.requires
CMakeFiles/pacr-follow.dir/requires: CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/stopwatch.cpp.o.requires
CMakeFiles/pacr-follow.dir/requires: CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/libenjoy_linux.c.o.requires
CMakeFiles/pacr-follow.dir/requires: CMakeFiles/pacr-follow.dir/home/nvidia/Documents/USUPACR/src/libenjoy.c.o.requires

.PHONY : CMakeFiles/pacr-follow.dir/requires

CMakeFiles/pacr-follow.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/pacr-follow.dir/cmake_clean.cmake
.PHONY : CMakeFiles/pacr-follow.dir/clean

CMakeFiles/pacr-follow.dir/depend:
	cd /home/nvidia/Documents/USUPACR/follow-demo/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/nvidia/Documents/USUPACR/follow-demo /home/nvidia/Documents/USUPACR/follow-demo /home/nvidia/Documents/USUPACR/follow-demo/build /home/nvidia/Documents/USUPACR/follow-demo/build /home/nvidia/Documents/USUPACR/follow-demo/build/CMakeFiles/pacr-follow.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/pacr-follow.dir/depend

