# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.17

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Disable VCS-based implicit rules.
% : %,v


# Disable VCS-based implicit rules.
% : RCS/%


# Disable VCS-based implicit rules.
% : RCS/%,v


# Disable VCS-based implicit rules.
% : SCCS/s.%


# Disable VCS-based implicit rules.
% : s.%


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
CMAKE_COMMAND = /home/davide/Scrivania/GSOC/VlcJB/vlc/extras/tools/build/bin/cmake

# The command to remove a file.
RM = /home/davide/Scrivania/GSOC/VlcJB/vlc/extras/tools/build/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang

# Include any dependencies generated for this target.
include OGLCompilersDLL/CMakeFiles/OGLCompiler.dir/depend.make

# Include the progress variables for this target.
include OGLCompilersDLL/CMakeFiles/OGLCompiler.dir/progress.make

# Include the compile flags for this target's objects.
include OGLCompilersDLL/CMakeFiles/OGLCompiler.dir/flags.make

OGLCompilersDLL/CMakeFiles/OGLCompiler.dir/InitializeDll.cpp.o: OGLCompilersDLL/CMakeFiles/OGLCompiler.dir/flags.make
OGLCompilersDLL/CMakeFiles/OGLCompiler.dir/InitializeDll.cpp.o: OGLCompilersDLL/InitializeDll.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object OGLCompilersDLL/CMakeFiles/OGLCompiler.dir/InitializeDll.cpp.o"
	cd /home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang/OGLCompilersDLL && /usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/OGLCompiler.dir/InitializeDll.cpp.o -c /home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang/OGLCompilersDLL/InitializeDll.cpp

OGLCompilersDLL/CMakeFiles/OGLCompiler.dir/InitializeDll.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/OGLCompiler.dir/InitializeDll.cpp.i"
	cd /home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang/OGLCompilersDLL && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang/OGLCompilersDLL/InitializeDll.cpp > CMakeFiles/OGLCompiler.dir/InitializeDll.cpp.i

OGLCompilersDLL/CMakeFiles/OGLCompiler.dir/InitializeDll.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/OGLCompiler.dir/InitializeDll.cpp.s"
	cd /home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang/OGLCompilersDLL && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang/OGLCompilersDLL/InitializeDll.cpp -o CMakeFiles/OGLCompiler.dir/InitializeDll.cpp.s

# Object files for target OGLCompiler
OGLCompiler_OBJECTS = \
"CMakeFiles/OGLCompiler.dir/InitializeDll.cpp.o"

# External object files for target OGLCompiler
OGLCompiler_EXTERNAL_OBJECTS =

OGLCompilersDLL/libOGLCompiler.a: OGLCompilersDLL/CMakeFiles/OGLCompiler.dir/InitializeDll.cpp.o
OGLCompilersDLL/libOGLCompiler.a: OGLCompilersDLL/CMakeFiles/OGLCompiler.dir/build.make
OGLCompilersDLL/libOGLCompiler.a: OGLCompilersDLL/CMakeFiles/OGLCompiler.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libOGLCompiler.a"
	cd /home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang/OGLCompilersDLL && $(CMAKE_COMMAND) -P CMakeFiles/OGLCompiler.dir/cmake_clean_target.cmake
	cd /home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang/OGLCompilersDLL && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/OGLCompiler.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
OGLCompilersDLL/CMakeFiles/OGLCompiler.dir/build: OGLCompilersDLL/libOGLCompiler.a

.PHONY : OGLCompilersDLL/CMakeFiles/OGLCompiler.dir/build

OGLCompilersDLL/CMakeFiles/OGLCompiler.dir/clean:
	cd /home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang/OGLCompilersDLL && $(CMAKE_COMMAND) -P CMakeFiles/OGLCompiler.dir/cmake_clean.cmake
.PHONY : OGLCompilersDLL/CMakeFiles/OGLCompiler.dir/clean

OGLCompilersDLL/CMakeFiles/OGLCompiler.dir/depend:
	cd /home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang /home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang/OGLCompilersDLL /home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang /home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang/OGLCompilersDLL /home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang/OGLCompilersDLL/CMakeFiles/OGLCompiler.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : OGLCompilersDLL/CMakeFiles/OGLCompiler.dir/depend

