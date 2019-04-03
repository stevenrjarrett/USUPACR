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
CMAKE_SOURCE_DIR = /home/nvidia/Documents/USUPACR/detectnet-camera

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/nvidia/Documents/USUPACR/detectnet-camera/build

# Include any dependencies generated for this target.
include CMakeFiles/detectnet-camera.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/detectnet-camera.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/detectnet-camera.dir/flags.make

CMakeFiles/detectnet-camera.dir/detectnet-camera-opencv.cpp.o: CMakeFiles/detectnet-camera.dir/flags.make
CMakeFiles/detectnet-camera.dir/detectnet-camera-opencv.cpp.o: ../detectnet-camera-opencv.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/nvidia/Documents/USUPACR/detectnet-camera/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/detectnet-camera.dir/detectnet-camera-opencv.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/detectnet-camera.dir/detectnet-camera-opencv.cpp.o -c /home/nvidia/Documents/USUPACR/detectnet-camera/detectnet-camera-opencv.cpp

CMakeFiles/detectnet-camera.dir/detectnet-camera-opencv.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/detectnet-camera.dir/detectnet-camera-opencv.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/nvidia/Documents/USUPACR/detectnet-camera/detectnet-camera-opencv.cpp > CMakeFiles/detectnet-camera.dir/detectnet-camera-opencv.cpp.i

CMakeFiles/detectnet-camera.dir/detectnet-camera-opencv.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/detectnet-camera.dir/detectnet-camera-opencv.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/nvidia/Documents/USUPACR/detectnet-camera/detectnet-camera-opencv.cpp -o CMakeFiles/detectnet-camera.dir/detectnet-camera-opencv.cpp.s

CMakeFiles/detectnet-camera.dir/detectnet-camera-opencv.cpp.o.requires:

.PHONY : CMakeFiles/detectnet-camera.dir/detectnet-camera-opencv.cpp.o.requires

CMakeFiles/detectnet-camera.dir/detectnet-camera-opencv.cpp.o.provides: CMakeFiles/detectnet-camera.dir/detectnet-camera-opencv.cpp.o.requires
	$(MAKE) -f CMakeFiles/detectnet-camera.dir/build.make CMakeFiles/detectnet-camera.dir/detectnet-camera-opencv.cpp.o.provides.build
.PHONY : CMakeFiles/detectnet-camera.dir/detectnet-camera-opencv.cpp.o.provides

CMakeFiles/detectnet-camera.dir/detectnet-camera-opencv.cpp.o.provides.build: CMakeFiles/detectnet-camera.dir/detectnet-camera-opencv.cpp.o


# Object files for target detectnet-camera
detectnet__camera_OBJECTS = \
"CMakeFiles/detectnet-camera.dir/detectnet-camera-opencv.cpp.o"

# External object files for target detectnet-camera
detectnet__camera_EXTERNAL_OBJECTS =

detectnet-camera: CMakeFiles/detectnet-camera.dir/detectnet-camera-opencv.cpp.o
detectnet-camera: CMakeFiles/detectnet-camera.dir/build.make
detectnet-camera: /usr/local/cuda-9.0/lib64/libcudart_static.a
detectnet-camera: /usr/lib/aarch64-linux-gnu/librt.so
detectnet-camera: /usr/local/lib/libjetson-inference.so
detectnet-camera: /usr/lib/libopencv_dnn.so.3.3.1
detectnet-camera: /usr/lib/libopencv_ml.so.3.3.1
detectnet-camera: /usr/lib/libopencv_objdetect.so.3.3.1
detectnet-camera: /usr/lib/libopencv_shape.so.3.3.1
detectnet-camera: /usr/lib/libopencv_stitching.so.3.3.1
detectnet-camera: /usr/lib/libopencv_superres.so.3.3.1
detectnet-camera: /usr/lib/libopencv_videostab.so.3.3.1
detectnet-camera: /usr/local/lib/libjetson-utils.so
detectnet-camera: /usr/local/cuda-9.0/lib64/libcudart_static.a
detectnet-camera: /usr/lib/aarch64-linux-gnu/librt.so
detectnet-camera: /usr/lib/aarch64-linux-gnu/libQtGui.so
detectnet-camera: /usr/lib/aarch64-linux-gnu/libQtCore.so
detectnet-camera: /usr/lib/libopencv_calib3d.so.3.3.1
detectnet-camera: /usr/lib/libopencv_features2d.so.3.3.1
detectnet-camera: /usr/lib/libopencv_flann.so.3.3.1
detectnet-camera: /usr/lib/libopencv_highgui.so.3.3.1
detectnet-camera: /usr/lib/libopencv_photo.so.3.3.1
detectnet-camera: /usr/lib/libopencv_video.so.3.3.1
detectnet-camera: /usr/lib/libopencv_videoio.so.3.3.1
detectnet-camera: /usr/lib/libopencv_imgcodecs.so.3.3.1
detectnet-camera: /usr/lib/libopencv_imgproc.so.3.3.1
detectnet-camera: /usr/lib/libopencv_core.so.3.3.1
detectnet-camera: CMakeFiles/detectnet-camera.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/nvidia/Documents/USUPACR/detectnet-camera/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable detectnet-camera"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/detectnet-camera.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/detectnet-camera.dir/build: detectnet-camera

.PHONY : CMakeFiles/detectnet-camera.dir/build

CMakeFiles/detectnet-camera.dir/requires: CMakeFiles/detectnet-camera.dir/detectnet-camera-opencv.cpp.o.requires

.PHONY : CMakeFiles/detectnet-camera.dir/requires

CMakeFiles/detectnet-camera.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/detectnet-camera.dir/cmake_clean.cmake
.PHONY : CMakeFiles/detectnet-camera.dir/clean

CMakeFiles/detectnet-camera.dir/depend:
	cd /home/nvidia/Documents/USUPACR/detectnet-camera/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/nvidia/Documents/USUPACR/detectnet-camera /home/nvidia/Documents/USUPACR/detectnet-camera /home/nvidia/Documents/USUPACR/detectnet-camera/build /home/nvidia/Documents/USUPACR/detectnet-camera/build /home/nvidia/Documents/USUPACR/detectnet-camera/build/CMakeFiles/detectnet-camera.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/detectnet-camera.dir/depend

