# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_SOURCE_DIR = /home/user/Desktop/RG11-pokematf

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/user/Desktop/RG11-pokematf

# Include any dependencies generated for this target.
include CMakeFiles/pokematf.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/pokematf.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/pokematf.dir/flags.make

CMakeFiles/pokematf.dir/pokematf.cpp.o: CMakeFiles/pokematf.dir/flags.make
CMakeFiles/pokematf.dir/pokematf.cpp.o: pokematf.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/user/Desktop/RG11-pokematf/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/pokematf.dir/pokematf.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/pokematf.dir/pokematf.cpp.o -c /home/user/Desktop/RG11-pokematf/pokematf.cpp

CMakeFiles/pokematf.dir/pokematf.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/pokematf.dir/pokematf.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/user/Desktop/RG11-pokematf/pokematf.cpp > CMakeFiles/pokematf.dir/pokematf.cpp.i

CMakeFiles/pokematf.dir/pokematf.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/pokematf.dir/pokematf.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/user/Desktop/RG11-pokematf/pokematf.cpp -o CMakeFiles/pokematf.dir/pokematf.cpp.s

# Object files for target pokematf
pokematf_OBJECTS = \
"CMakeFiles/pokematf.dir/pokematf.cpp.o"

# External object files for target pokematf
pokematf_EXTERNAL_OBJECTS =

libpokematf.a: CMakeFiles/pokematf.dir/pokematf.cpp.o
libpokematf.a: CMakeFiles/pokematf.dir/build.make
libpokematf.a: CMakeFiles/pokematf.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/user/Desktop/RG11-pokematf/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libpokematf.a"
	$(CMAKE_COMMAND) -P CMakeFiles/pokematf.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/pokematf.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/pokematf.dir/build: libpokematf.a

.PHONY : CMakeFiles/pokematf.dir/build

CMakeFiles/pokematf.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/pokematf.dir/cmake_clean.cmake
.PHONY : CMakeFiles/pokematf.dir/clean

CMakeFiles/pokematf.dir/depend:
	cd /home/user/Desktop/RG11-pokematf && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/user/Desktop/RG11-pokematf /home/user/Desktop/RG11-pokematf /home/user/Desktop/RG11-pokematf /home/user/Desktop/RG11-pokematf /home/user/Desktop/RG11-pokematf/CMakeFiles/pokematf.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/pokematf.dir/depend

