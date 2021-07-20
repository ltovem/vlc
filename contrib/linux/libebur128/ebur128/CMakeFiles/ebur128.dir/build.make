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
CMAKE_SOURCE_DIR = /home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/libebur128

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/libebur128

# Include any dependencies generated for this target.
include ebur128/CMakeFiles/ebur128.dir/depend.make

# Include the progress variables for this target.
include ebur128/CMakeFiles/ebur128.dir/progress.make

# Include the compile flags for this target's objects.
include ebur128/CMakeFiles/ebur128.dir/flags.make

ebur128/CMakeFiles/ebur128.dir/ebur128.c.o: ebur128/CMakeFiles/ebur128.dir/flags.make
ebur128/CMakeFiles/ebur128.dir/ebur128.c.o: ebur128/ebur128.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/libebur128/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object ebur128/CMakeFiles/ebur128.dir/ebur128.c.o"
	cd /home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/libebur128/ebur128 && /usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/ebur128.dir/ebur128.c.o   -c /home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/libebur128/ebur128/ebur128.c

ebur128/CMakeFiles/ebur128.dir/ebur128.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/ebur128.dir/ebur128.c.i"
	cd /home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/libebur128/ebur128 && /usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/libebur128/ebur128/ebur128.c > CMakeFiles/ebur128.dir/ebur128.c.i

ebur128/CMakeFiles/ebur128.dir/ebur128.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/ebur128.dir/ebur128.c.s"
	cd /home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/libebur128/ebur128 && /usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/libebur128/ebur128/ebur128.c -o CMakeFiles/ebur128.dir/ebur128.c.s

# Object files for target ebur128
ebur128_OBJECTS = \
"CMakeFiles/ebur128.dir/ebur128.c.o"

# External object files for target ebur128
ebur128_EXTERNAL_OBJECTS =

libebur128.a: ebur128/CMakeFiles/ebur128.dir/ebur128.c.o
libebur128.a: ebur128/CMakeFiles/ebur128.dir/build.make
libebur128.a: ebur128/CMakeFiles/ebur128.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/libebur128/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C static library ../libebur128.a"
	cd /home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/libebur128/ebur128 && $(CMAKE_COMMAND) -P CMakeFiles/ebur128.dir/cmake_clean_target.cmake
	cd /home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/libebur128/ebur128 && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ebur128.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
ebur128/CMakeFiles/ebur128.dir/build: libebur128.a

.PHONY : ebur128/CMakeFiles/ebur128.dir/build

ebur128/CMakeFiles/ebur128.dir/clean:
	cd /home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/libebur128/ebur128 && $(CMAKE_COMMAND) -P CMakeFiles/ebur128.dir/cmake_clean.cmake
.PHONY : ebur128/CMakeFiles/ebur128.dir/clean

ebur128/CMakeFiles/ebur128.dir/depend:
	cd /home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/libebur128 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/libebur128 /home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/libebur128/ebur128 /home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/libebur128 /home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/libebur128/ebur128 /home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/libebur128/ebur128/CMakeFiles/ebur128.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : ebur128/CMakeFiles/ebur128.dir/depend

