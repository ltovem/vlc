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
include hlsl/CMakeFiles/HLSL.dir/depend.make

# Include the progress variables for this target.
include hlsl/CMakeFiles/HLSL.dir/progress.make

# Include the compile flags for this target's objects.
include hlsl/CMakeFiles/HLSL.dir/flags.make

hlsl/CMakeFiles/HLSL.dir/hlslAttributes.cpp.o: hlsl/CMakeFiles/HLSL.dir/flags.make
hlsl/CMakeFiles/HLSL.dir/hlslAttributes.cpp.o: hlsl/hlslAttributes.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object hlsl/CMakeFiles/HLSL.dir/hlslAttributes.cpp.o"
	cd /home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang/hlsl && /usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/HLSL.dir/hlslAttributes.cpp.o -c /home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang/hlsl/hlslAttributes.cpp

hlsl/CMakeFiles/HLSL.dir/hlslAttributes.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/HLSL.dir/hlslAttributes.cpp.i"
	cd /home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang/hlsl && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang/hlsl/hlslAttributes.cpp > CMakeFiles/HLSL.dir/hlslAttributes.cpp.i

hlsl/CMakeFiles/HLSL.dir/hlslAttributes.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/HLSL.dir/hlslAttributes.cpp.s"
	cd /home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang/hlsl && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang/hlsl/hlslAttributes.cpp -o CMakeFiles/HLSL.dir/hlslAttributes.cpp.s

hlsl/CMakeFiles/HLSL.dir/hlslParseHelper.cpp.o: hlsl/CMakeFiles/HLSL.dir/flags.make
hlsl/CMakeFiles/HLSL.dir/hlslParseHelper.cpp.o: hlsl/hlslParseHelper.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object hlsl/CMakeFiles/HLSL.dir/hlslParseHelper.cpp.o"
	cd /home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang/hlsl && /usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/HLSL.dir/hlslParseHelper.cpp.o -c /home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang/hlsl/hlslParseHelper.cpp

hlsl/CMakeFiles/HLSL.dir/hlslParseHelper.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/HLSL.dir/hlslParseHelper.cpp.i"
	cd /home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang/hlsl && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang/hlsl/hlslParseHelper.cpp > CMakeFiles/HLSL.dir/hlslParseHelper.cpp.i

hlsl/CMakeFiles/HLSL.dir/hlslParseHelper.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/HLSL.dir/hlslParseHelper.cpp.s"
	cd /home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang/hlsl && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang/hlsl/hlslParseHelper.cpp -o CMakeFiles/HLSL.dir/hlslParseHelper.cpp.s

hlsl/CMakeFiles/HLSL.dir/hlslScanContext.cpp.o: hlsl/CMakeFiles/HLSL.dir/flags.make
hlsl/CMakeFiles/HLSL.dir/hlslScanContext.cpp.o: hlsl/hlslScanContext.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object hlsl/CMakeFiles/HLSL.dir/hlslScanContext.cpp.o"
	cd /home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang/hlsl && /usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/HLSL.dir/hlslScanContext.cpp.o -c /home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang/hlsl/hlslScanContext.cpp

hlsl/CMakeFiles/HLSL.dir/hlslScanContext.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/HLSL.dir/hlslScanContext.cpp.i"
	cd /home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang/hlsl && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang/hlsl/hlslScanContext.cpp > CMakeFiles/HLSL.dir/hlslScanContext.cpp.i

hlsl/CMakeFiles/HLSL.dir/hlslScanContext.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/HLSL.dir/hlslScanContext.cpp.s"
	cd /home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang/hlsl && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang/hlsl/hlslScanContext.cpp -o CMakeFiles/HLSL.dir/hlslScanContext.cpp.s

hlsl/CMakeFiles/HLSL.dir/hlslOpMap.cpp.o: hlsl/CMakeFiles/HLSL.dir/flags.make
hlsl/CMakeFiles/HLSL.dir/hlslOpMap.cpp.o: hlsl/hlslOpMap.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object hlsl/CMakeFiles/HLSL.dir/hlslOpMap.cpp.o"
	cd /home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang/hlsl && /usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/HLSL.dir/hlslOpMap.cpp.o -c /home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang/hlsl/hlslOpMap.cpp

hlsl/CMakeFiles/HLSL.dir/hlslOpMap.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/HLSL.dir/hlslOpMap.cpp.i"
	cd /home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang/hlsl && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang/hlsl/hlslOpMap.cpp > CMakeFiles/HLSL.dir/hlslOpMap.cpp.i

hlsl/CMakeFiles/HLSL.dir/hlslOpMap.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/HLSL.dir/hlslOpMap.cpp.s"
	cd /home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang/hlsl && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang/hlsl/hlslOpMap.cpp -o CMakeFiles/HLSL.dir/hlslOpMap.cpp.s

hlsl/CMakeFiles/HLSL.dir/hlslTokenStream.cpp.o: hlsl/CMakeFiles/HLSL.dir/flags.make
hlsl/CMakeFiles/HLSL.dir/hlslTokenStream.cpp.o: hlsl/hlslTokenStream.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object hlsl/CMakeFiles/HLSL.dir/hlslTokenStream.cpp.o"
	cd /home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang/hlsl && /usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/HLSL.dir/hlslTokenStream.cpp.o -c /home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang/hlsl/hlslTokenStream.cpp

hlsl/CMakeFiles/HLSL.dir/hlslTokenStream.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/HLSL.dir/hlslTokenStream.cpp.i"
	cd /home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang/hlsl && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang/hlsl/hlslTokenStream.cpp > CMakeFiles/HLSL.dir/hlslTokenStream.cpp.i

hlsl/CMakeFiles/HLSL.dir/hlslTokenStream.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/HLSL.dir/hlslTokenStream.cpp.s"
	cd /home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang/hlsl && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang/hlsl/hlslTokenStream.cpp -o CMakeFiles/HLSL.dir/hlslTokenStream.cpp.s

hlsl/CMakeFiles/HLSL.dir/hlslGrammar.cpp.o: hlsl/CMakeFiles/HLSL.dir/flags.make
hlsl/CMakeFiles/HLSL.dir/hlslGrammar.cpp.o: hlsl/hlslGrammar.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object hlsl/CMakeFiles/HLSL.dir/hlslGrammar.cpp.o"
	cd /home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang/hlsl && /usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/HLSL.dir/hlslGrammar.cpp.o -c /home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang/hlsl/hlslGrammar.cpp

hlsl/CMakeFiles/HLSL.dir/hlslGrammar.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/HLSL.dir/hlslGrammar.cpp.i"
	cd /home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang/hlsl && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang/hlsl/hlslGrammar.cpp > CMakeFiles/HLSL.dir/hlslGrammar.cpp.i

hlsl/CMakeFiles/HLSL.dir/hlslGrammar.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/HLSL.dir/hlslGrammar.cpp.s"
	cd /home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang/hlsl && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang/hlsl/hlslGrammar.cpp -o CMakeFiles/HLSL.dir/hlslGrammar.cpp.s

hlsl/CMakeFiles/HLSL.dir/hlslParseables.cpp.o: hlsl/CMakeFiles/HLSL.dir/flags.make
hlsl/CMakeFiles/HLSL.dir/hlslParseables.cpp.o: hlsl/hlslParseables.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object hlsl/CMakeFiles/HLSL.dir/hlslParseables.cpp.o"
	cd /home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang/hlsl && /usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/HLSL.dir/hlslParseables.cpp.o -c /home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang/hlsl/hlslParseables.cpp

hlsl/CMakeFiles/HLSL.dir/hlslParseables.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/HLSL.dir/hlslParseables.cpp.i"
	cd /home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang/hlsl && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang/hlsl/hlslParseables.cpp > CMakeFiles/HLSL.dir/hlslParseables.cpp.i

hlsl/CMakeFiles/HLSL.dir/hlslParseables.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/HLSL.dir/hlslParseables.cpp.s"
	cd /home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang/hlsl && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang/hlsl/hlslParseables.cpp -o CMakeFiles/HLSL.dir/hlslParseables.cpp.s

# Object files for target HLSL
HLSL_OBJECTS = \
"CMakeFiles/HLSL.dir/hlslAttributes.cpp.o" \
"CMakeFiles/HLSL.dir/hlslParseHelper.cpp.o" \
"CMakeFiles/HLSL.dir/hlslScanContext.cpp.o" \
"CMakeFiles/HLSL.dir/hlslOpMap.cpp.o" \
"CMakeFiles/HLSL.dir/hlslTokenStream.cpp.o" \
"CMakeFiles/HLSL.dir/hlslGrammar.cpp.o" \
"CMakeFiles/HLSL.dir/hlslParseables.cpp.o"

# External object files for target HLSL
HLSL_EXTERNAL_OBJECTS =

hlsl/libHLSL.a: hlsl/CMakeFiles/HLSL.dir/hlslAttributes.cpp.o
hlsl/libHLSL.a: hlsl/CMakeFiles/HLSL.dir/hlslParseHelper.cpp.o
hlsl/libHLSL.a: hlsl/CMakeFiles/HLSL.dir/hlslScanContext.cpp.o
hlsl/libHLSL.a: hlsl/CMakeFiles/HLSL.dir/hlslOpMap.cpp.o
hlsl/libHLSL.a: hlsl/CMakeFiles/HLSL.dir/hlslTokenStream.cpp.o
hlsl/libHLSL.a: hlsl/CMakeFiles/HLSL.dir/hlslGrammar.cpp.o
hlsl/libHLSL.a: hlsl/CMakeFiles/HLSL.dir/hlslParseables.cpp.o
hlsl/libHLSL.a: hlsl/CMakeFiles/HLSL.dir/build.make
hlsl/libHLSL.a: hlsl/CMakeFiles/HLSL.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Linking CXX static library libHLSL.a"
	cd /home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang/hlsl && $(CMAKE_COMMAND) -P CMakeFiles/HLSL.dir/cmake_clean_target.cmake
	cd /home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang/hlsl && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/HLSL.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
hlsl/CMakeFiles/HLSL.dir/build: hlsl/libHLSL.a

.PHONY : hlsl/CMakeFiles/HLSL.dir/build

hlsl/CMakeFiles/HLSL.dir/clean:
	cd /home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang/hlsl && $(CMAKE_COMMAND) -P CMakeFiles/HLSL.dir/cmake_clean.cmake
.PHONY : hlsl/CMakeFiles/HLSL.dir/clean

hlsl/CMakeFiles/HLSL.dir/depend:
	cd /home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang /home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang/hlsl /home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang /home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang/hlsl /home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang/hlsl/CMakeFiles/HLSL.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : hlsl/CMakeFiles/HLSL.dir/depend

