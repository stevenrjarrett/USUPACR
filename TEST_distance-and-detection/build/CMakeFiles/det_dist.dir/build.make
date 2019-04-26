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
CMAKE_SOURCE_DIR = /home/nvidia/Documents/USUPACR/distance-and-detection

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/nvidia/Documents/USUPACR/distance-and-detection/build

# Include any dependencies generated for this target.
include CMakeFiles/det_dist.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/det_dist.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/det_dist.dir/flags.make

CMakeFiles/det_dist.dir/det_dist.cpp.o: CMakeFiles/det_dist.dir/flags.make
CMakeFiles/det_dist.dir/det_dist.cpp.o: ../det_dist.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/nvidia/Documents/USUPACR/distance-and-detection/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/det_dist.dir/det_dist.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/det_dist.dir/det_dist.cpp.o -c /home/nvidia/Documents/USUPACR/distance-and-detection/det_dist.cpp

CMakeFiles/det_dist.dir/det_dist.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/det_dist.dir/det_dist.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/nvidia/Documents/USUPACR/distance-and-detection/det_dist.cpp > CMakeFiles/det_dist.dir/det_dist.cpp.i

CMakeFiles/det_dist.dir/det_dist.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/det_dist.dir/det_dist.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/nvidia/Documents/USUPACR/distance-and-detection/det_dist.cpp -o CMakeFiles/det_dist.dir/det_dist.cpp.s

CMakeFiles/det_dist.dir/det_dist.cpp.o.requires:

.PHONY : CMakeFiles/det_dist.dir/det_dist.cpp.o.requires

CMakeFiles/det_dist.dir/det_dist.cpp.o.provides: CMakeFiles/det_dist.dir/det_dist.cpp.o.requires
	$(MAKE) -f CMakeFiles/det_dist.dir/build.make CMakeFiles/det_dist.dir/det_dist.cpp.o.provides.build
.PHONY : CMakeFiles/det_dist.dir/det_dist.cpp.o.provides

CMakeFiles/det_dist.dir/det_dist.cpp.o.provides.build: CMakeFiles/det_dist.dir/det_dist.cpp.o


# Object files for target det_dist
det_dist_OBJECTS = \
"CMakeFiles/det_dist.dir/det_dist.cpp.o"

# External object files for target det_dist
det_dist_EXTERNAL_OBJECTS =

det_dist: CMakeFiles/det_dist.dir/det_dist.cpp.o
det_dist: CMakeFiles/det_dist.dir/build.make
det_dist: /usr/local/cuda-9.0/lib64/libcudart_static.a
det_dist: /usr/lib/aarch64-linux-gnu/librt.so
det_dist: /usr/local/lib/libjetson-inference.so
det_dist: /usr/local/lib/librealsense2.so.2.10.4
det_dist: /usr/lib/libopencv_dnn.so.3.3.1
det_dist: /usr/lib/libopencv_ml.so.3.3.1
det_dist: /usr/lib/libopencv_objdetect.so.3.3.1
det_dist: /usr/lib/libopencv_shape.so.3.3.1
det_dist: /usr/lib/libopencv_stitching.so.3.3.1
det_dist: /usr/lib/libopencv_superres.so.3.3.1
det_dist: /usr/lib/libopencv_videostab.so.3.3.1
det_dist: /usr/local/lib/libjetson-utils.so
det_dist: /usr/local/cuda-9.0/lib64/libcudart_static.a
det_dist: /usr/lib/aarch64-linux-gnu/librt.so
det_dist: /usr/lib/aarch64-linux-gnu/libQtGui.so
det_dist: /usr/lib/aarch64-linux-gnu/libQtCore.so
det_dist: /usr/lib/libopencv_calib3d.so.3.3.1
det_dist: /usr/lib/libopencv_features2d.so.3.3.1
det_dist: /usr/lib/libopencv_flann.so.3.3.1
det_dist: /usr/lib/libopencv_highgui.so.3.3.1
det_dist: /usr/lib/libopencv_photo.so.3.3.1
det_dist: /usr/lib/libopencv_video.so.3.3.1
det_dist: /usr/lib/libopencv_videoio.so.3.3.1
det_dist: /usr/lib/libopencv_imgcodecs.so.3.3.1
det_dist: /usr/lib/libopencv_imgproc.so.3.3.1
det_dist: /usr/lib/libopencv_core.so.3.3.1
det_dist: CMakeFiles/det_dist.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/nvidia/Documents/USUPACR/distance-and-detection/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable det_dist"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/det_dist.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/det_dist.dir/build: det_dist

.PHONY : CMakeFiles/det_dist.dir/build

CMakeFiles/det_dist.dir/requires: CMakeFiles/det_dist.dir/det_dist.cpp.o.requires

.PHONY : CMakeFiles/det_dist.dir/requires

CMakeFiles/det_dist.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/det_dist.dir/cmake_clean.cmake
.PHONY : CMakeFiles/det_dist.dir/clean

CMakeFiles/det_dist.dir/depend:
	cd /home/nvidia/Documents/USUPACR/distance-and-detection/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/nvidia/Documents/USUPACR/distance-and-detection /home/nvidia/Documents/USUPACR/distance-and-detection /home/nvidia/Documents/USUPACR/distance-and-detection/build /home/nvidia/Documents/USUPACR/distance-and-detection/build /home/nvidia/Documents/USUPACR/distance-and-detection/build/CMakeFiles/det_dist.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/det_dist.dir/depend
