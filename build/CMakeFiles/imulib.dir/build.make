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
CMAKE_SOURCE_DIR = /home/stashako/imu_i2c

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/stashako/imu_i2c/build

# Include any dependencies generated for this target.
include CMakeFiles/imulib.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/imulib.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/imulib.dir/flags.make

CMakeFiles/imulib.dir/imuLib/i2c_bno055.c.o: CMakeFiles/imulib.dir/flags.make
CMakeFiles/imulib.dir/imuLib/i2c_bno055.c.o: ../imuLib/i2c_bno055.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/stashako/imu_i2c/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/imulib.dir/imuLib/i2c_bno055.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/imulib.dir/imuLib/i2c_bno055.c.o   -c /home/stashako/imu_i2c/imuLib/i2c_bno055.c

CMakeFiles/imulib.dir/imuLib/i2c_bno055.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/imulib.dir/imuLib/i2c_bno055.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/stashako/imu_i2c/imuLib/i2c_bno055.c > CMakeFiles/imulib.dir/imuLib/i2c_bno055.c.i

CMakeFiles/imulib.dir/imuLib/i2c_bno055.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/imulib.dir/imuLib/i2c_bno055.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/stashako/imu_i2c/imuLib/i2c_bno055.c -o CMakeFiles/imulib.dir/imuLib/i2c_bno055.c.s

# Object files for target imulib
imulib_OBJECTS = \
"CMakeFiles/imulib.dir/imuLib/i2c_bno055.c.o"

# External object files for target imulib
imulib_EXTERNAL_OBJECTS =

libimulib.a: CMakeFiles/imulib.dir/imuLib/i2c_bno055.c.o
libimulib.a: CMakeFiles/imulib.dir/build.make
libimulib.a: CMakeFiles/imulib.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/stashako/imu_i2c/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C static library libimulib.a"
	$(CMAKE_COMMAND) -P CMakeFiles/imulib.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/imulib.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/imulib.dir/build: libimulib.a

.PHONY : CMakeFiles/imulib.dir/build

CMakeFiles/imulib.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/imulib.dir/cmake_clean.cmake
.PHONY : CMakeFiles/imulib.dir/clean

CMakeFiles/imulib.dir/depend:
	cd /home/stashako/imu_i2c/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/stashako/imu_i2c /home/stashako/imu_i2c /home/stashako/imu_i2c/build /home/stashako/imu_i2c/build /home/stashako/imu_i2c/build/CMakeFiles/imulib.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/imulib.dir/depend

