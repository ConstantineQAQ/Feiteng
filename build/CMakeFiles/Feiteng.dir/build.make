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
CMAKE_SOURCE_DIR = /home/kylin/Feiteng

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/kylin/Feiteng/build

# Include any dependencies generated for this target.
include CMakeFiles/Feiteng.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Feiteng.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Feiteng.dir/flags.make

CMakeFiles/Feiteng.dir/Feiteng_autogen/mocs_compilation.cpp.o: CMakeFiles/Feiteng.dir/flags.make
CMakeFiles/Feiteng.dir/Feiteng_autogen/mocs_compilation.cpp.o: Feiteng_autogen/mocs_compilation.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kylin/Feiteng/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Feiteng.dir/Feiteng_autogen/mocs_compilation.cpp.o"
	/usr/bin/aarch64-linux-gnu-g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Feiteng.dir/Feiteng_autogen/mocs_compilation.cpp.o -c /home/kylin/Feiteng/build/Feiteng_autogen/mocs_compilation.cpp

CMakeFiles/Feiteng.dir/Feiteng_autogen/mocs_compilation.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Feiteng.dir/Feiteng_autogen/mocs_compilation.cpp.i"
	/usr/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kylin/Feiteng/build/Feiteng_autogen/mocs_compilation.cpp > CMakeFiles/Feiteng.dir/Feiteng_autogen/mocs_compilation.cpp.i

CMakeFiles/Feiteng.dir/Feiteng_autogen/mocs_compilation.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Feiteng.dir/Feiteng_autogen/mocs_compilation.cpp.s"
	/usr/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kylin/Feiteng/build/Feiteng_autogen/mocs_compilation.cpp -o CMakeFiles/Feiteng.dir/Feiteng_autogen/mocs_compilation.cpp.s

CMakeFiles/Feiteng.dir/src/admindialog.cpp.o: CMakeFiles/Feiteng.dir/flags.make
CMakeFiles/Feiteng.dir/src/admindialog.cpp.o: ../src/admindialog.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kylin/Feiteng/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/Feiteng.dir/src/admindialog.cpp.o"
	/usr/bin/aarch64-linux-gnu-g++  $(CXX_DEFINES) -D__FILE__=\"src/admindialog.cpp\" $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Feiteng.dir/src/admindialog.cpp.o -c /home/kylin/Feiteng/src/admindialog.cpp

CMakeFiles/Feiteng.dir/src/admindialog.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Feiteng.dir/src/admindialog.cpp.i"
	/usr/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) -D__FILE__=\"src/admindialog.cpp\" $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kylin/Feiteng/src/admindialog.cpp > CMakeFiles/Feiteng.dir/src/admindialog.cpp.i

CMakeFiles/Feiteng.dir/src/admindialog.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Feiteng.dir/src/admindialog.cpp.s"
	/usr/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) -D__FILE__=\"src/admindialog.cpp\" $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kylin/Feiteng/src/admindialog.cpp -o CMakeFiles/Feiteng.dir/src/admindialog.cpp.s

CMakeFiles/Feiteng.dir/src/config.cpp.o: CMakeFiles/Feiteng.dir/flags.make
CMakeFiles/Feiteng.dir/src/config.cpp.o: ../src/config.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kylin/Feiteng/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/Feiteng.dir/src/config.cpp.o"
	/usr/bin/aarch64-linux-gnu-g++  $(CXX_DEFINES) -D__FILE__=\"src/config.cpp\" $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Feiteng.dir/src/config.cpp.o -c /home/kylin/Feiteng/src/config.cpp

CMakeFiles/Feiteng.dir/src/config.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Feiteng.dir/src/config.cpp.i"
	/usr/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) -D__FILE__=\"src/config.cpp\" $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kylin/Feiteng/src/config.cpp > CMakeFiles/Feiteng.dir/src/config.cpp.i

CMakeFiles/Feiteng.dir/src/config.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Feiteng.dir/src/config.cpp.s"
	/usr/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) -D__FILE__=\"src/config.cpp\" $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kylin/Feiteng/src/config.cpp -o CMakeFiles/Feiteng.dir/src/config.cpp.s

CMakeFiles/Feiteng.dir/src/database.cpp.o: CMakeFiles/Feiteng.dir/flags.make
CMakeFiles/Feiteng.dir/src/database.cpp.o: ../src/database.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kylin/Feiteng/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/Feiteng.dir/src/database.cpp.o"
	/usr/bin/aarch64-linux-gnu-g++  $(CXX_DEFINES) -D__FILE__=\"src/database.cpp\" $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Feiteng.dir/src/database.cpp.o -c /home/kylin/Feiteng/src/database.cpp

CMakeFiles/Feiteng.dir/src/database.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Feiteng.dir/src/database.cpp.i"
	/usr/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) -D__FILE__=\"src/database.cpp\" $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kylin/Feiteng/src/database.cpp > CMakeFiles/Feiteng.dir/src/database.cpp.i

CMakeFiles/Feiteng.dir/src/database.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Feiteng.dir/src/database.cpp.s"
	/usr/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) -D__FILE__=\"src/database.cpp\" $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kylin/Feiteng/src/database.cpp -o CMakeFiles/Feiteng.dir/src/database.cpp.s

CMakeFiles/Feiteng.dir/src/face.cpp.o: CMakeFiles/Feiteng.dir/flags.make
CMakeFiles/Feiteng.dir/src/face.cpp.o: ../src/face.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kylin/Feiteng/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/Feiteng.dir/src/face.cpp.o"
	/usr/bin/aarch64-linux-gnu-g++  $(CXX_DEFINES) -D__FILE__=\"src/face.cpp\" $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Feiteng.dir/src/face.cpp.o -c /home/kylin/Feiteng/src/face.cpp

CMakeFiles/Feiteng.dir/src/face.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Feiteng.dir/src/face.cpp.i"
	/usr/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) -D__FILE__=\"src/face.cpp\" $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kylin/Feiteng/src/face.cpp > CMakeFiles/Feiteng.dir/src/face.cpp.i

CMakeFiles/Feiteng.dir/src/face.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Feiteng.dir/src/face.cpp.s"
	/usr/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) -D__FILE__=\"src/face.cpp\" $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kylin/Feiteng/src/face.cpp -o CMakeFiles/Feiteng.dir/src/face.cpp.s

CMakeFiles/Feiteng.dir/src/log.cpp.o: CMakeFiles/Feiteng.dir/flags.make
CMakeFiles/Feiteng.dir/src/log.cpp.o: ../src/log.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kylin/Feiteng/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/Feiteng.dir/src/log.cpp.o"
	/usr/bin/aarch64-linux-gnu-g++  $(CXX_DEFINES) -D__FILE__=\"src/log.cpp\" $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Feiteng.dir/src/log.cpp.o -c /home/kylin/Feiteng/src/log.cpp

CMakeFiles/Feiteng.dir/src/log.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Feiteng.dir/src/log.cpp.i"
	/usr/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) -D__FILE__=\"src/log.cpp\" $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kylin/Feiteng/src/log.cpp > CMakeFiles/Feiteng.dir/src/log.cpp.i

CMakeFiles/Feiteng.dir/src/log.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Feiteng.dir/src/log.cpp.s"
	/usr/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) -D__FILE__=\"src/log.cpp\" $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kylin/Feiteng/src/log.cpp -o CMakeFiles/Feiteng.dir/src/log.cpp.s

CMakeFiles/Feiteng.dir/src/main.cpp.o: CMakeFiles/Feiteng.dir/flags.make
CMakeFiles/Feiteng.dir/src/main.cpp.o: ../src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kylin/Feiteng/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/Feiteng.dir/src/main.cpp.o"
	/usr/bin/aarch64-linux-gnu-g++  $(CXX_DEFINES) -D__FILE__=\"src/main.cpp\" $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Feiteng.dir/src/main.cpp.o -c /home/kylin/Feiteng/src/main.cpp

CMakeFiles/Feiteng.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Feiteng.dir/src/main.cpp.i"
	/usr/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) -D__FILE__=\"src/main.cpp\" $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kylin/Feiteng/src/main.cpp > CMakeFiles/Feiteng.dir/src/main.cpp.i

CMakeFiles/Feiteng.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Feiteng.dir/src/main.cpp.s"
	/usr/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) -D__FILE__=\"src/main.cpp\" $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kylin/Feiteng/src/main.cpp -o CMakeFiles/Feiteng.dir/src/main.cpp.s

CMakeFiles/Feiteng.dir/src/mainwindow.cpp.o: CMakeFiles/Feiteng.dir/flags.make
CMakeFiles/Feiteng.dir/src/mainwindow.cpp.o: ../src/mainwindow.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kylin/Feiteng/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/Feiteng.dir/src/mainwindow.cpp.o"
	/usr/bin/aarch64-linux-gnu-g++  $(CXX_DEFINES) -D__FILE__=\"src/mainwindow.cpp\" $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Feiteng.dir/src/mainwindow.cpp.o -c /home/kylin/Feiteng/src/mainwindow.cpp

CMakeFiles/Feiteng.dir/src/mainwindow.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Feiteng.dir/src/mainwindow.cpp.i"
	/usr/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) -D__FILE__=\"src/mainwindow.cpp\" $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kylin/Feiteng/src/mainwindow.cpp > CMakeFiles/Feiteng.dir/src/mainwindow.cpp.i

CMakeFiles/Feiteng.dir/src/mainwindow.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Feiteng.dir/src/mainwindow.cpp.s"
	/usr/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) -D__FILE__=\"src/mainwindow.cpp\" $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kylin/Feiteng/src/mainwindow.cpp -o CMakeFiles/Feiteng.dir/src/mainwindow.cpp.s

CMakeFiles/Feiteng.dir/src/person.cpp.o: CMakeFiles/Feiteng.dir/flags.make
CMakeFiles/Feiteng.dir/src/person.cpp.o: ../src/person.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kylin/Feiteng/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/Feiteng.dir/src/person.cpp.o"
	/usr/bin/aarch64-linux-gnu-g++  $(CXX_DEFINES) -D__FILE__=\"src/person.cpp\" $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Feiteng.dir/src/person.cpp.o -c /home/kylin/Feiteng/src/person.cpp

CMakeFiles/Feiteng.dir/src/person.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Feiteng.dir/src/person.cpp.i"
	/usr/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) -D__FILE__=\"src/person.cpp\" $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kylin/Feiteng/src/person.cpp > CMakeFiles/Feiteng.dir/src/person.cpp.i

CMakeFiles/Feiteng.dir/src/person.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Feiteng.dir/src/person.cpp.s"
	/usr/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) -D__FILE__=\"src/person.cpp\" $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kylin/Feiteng/src/person.cpp -o CMakeFiles/Feiteng.dir/src/person.cpp.s

CMakeFiles/Feiteng.dir/src/temperature.cpp.o: CMakeFiles/Feiteng.dir/flags.make
CMakeFiles/Feiteng.dir/src/temperature.cpp.o: ../src/temperature.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kylin/Feiteng/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/Feiteng.dir/src/temperature.cpp.o"
	/usr/bin/aarch64-linux-gnu-g++  $(CXX_DEFINES) -D__FILE__=\"src/temperature.cpp\" $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Feiteng.dir/src/temperature.cpp.o -c /home/kylin/Feiteng/src/temperature.cpp

CMakeFiles/Feiteng.dir/src/temperature.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Feiteng.dir/src/temperature.cpp.i"
	/usr/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) -D__FILE__=\"src/temperature.cpp\" $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kylin/Feiteng/src/temperature.cpp > CMakeFiles/Feiteng.dir/src/temperature.cpp.i

CMakeFiles/Feiteng.dir/src/temperature.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Feiteng.dir/src/temperature.cpp.s"
	/usr/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) -D__FILE__=\"src/temperature.cpp\" $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kylin/Feiteng/src/temperature.cpp -o CMakeFiles/Feiteng.dir/src/temperature.cpp.s

CMakeFiles/Feiteng.dir/src/thread.cpp.o: CMakeFiles/Feiteng.dir/flags.make
CMakeFiles/Feiteng.dir/src/thread.cpp.o: ../src/thread.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kylin/Feiteng/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object CMakeFiles/Feiteng.dir/src/thread.cpp.o"
	/usr/bin/aarch64-linux-gnu-g++  $(CXX_DEFINES) -D__FILE__=\"src/thread.cpp\" $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Feiteng.dir/src/thread.cpp.o -c /home/kylin/Feiteng/src/thread.cpp

CMakeFiles/Feiteng.dir/src/thread.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Feiteng.dir/src/thread.cpp.i"
	/usr/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) -D__FILE__=\"src/thread.cpp\" $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kylin/Feiteng/src/thread.cpp > CMakeFiles/Feiteng.dir/src/thread.cpp.i

CMakeFiles/Feiteng.dir/src/thread.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Feiteng.dir/src/thread.cpp.s"
	/usr/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) -D__FILE__=\"src/thread.cpp\" $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kylin/Feiteng/src/thread.cpp -o CMakeFiles/Feiteng.dir/src/thread.cpp.s

CMakeFiles/Feiteng.dir/src/util.cpp.o: CMakeFiles/Feiteng.dir/flags.make
CMakeFiles/Feiteng.dir/src/util.cpp.o: ../src/util.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kylin/Feiteng/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Building CXX object CMakeFiles/Feiteng.dir/src/util.cpp.o"
	/usr/bin/aarch64-linux-gnu-g++  $(CXX_DEFINES) -D__FILE__=\"src/util.cpp\" $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Feiteng.dir/src/util.cpp.o -c /home/kylin/Feiteng/src/util.cpp

CMakeFiles/Feiteng.dir/src/util.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Feiteng.dir/src/util.cpp.i"
	/usr/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) -D__FILE__=\"src/util.cpp\" $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kylin/Feiteng/src/util.cpp > CMakeFiles/Feiteng.dir/src/util.cpp.i

CMakeFiles/Feiteng.dir/src/util.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Feiteng.dir/src/util.cpp.s"
	/usr/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) -D__FILE__=\"src/util.cpp\" $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kylin/Feiteng/src/util.cpp -o CMakeFiles/Feiteng.dir/src/util.cpp.s

# Object files for target Feiteng
Feiteng_OBJECTS = \
"CMakeFiles/Feiteng.dir/Feiteng_autogen/mocs_compilation.cpp.o" \
"CMakeFiles/Feiteng.dir/src/admindialog.cpp.o" \
"CMakeFiles/Feiteng.dir/src/config.cpp.o" \
"CMakeFiles/Feiteng.dir/src/database.cpp.o" \
"CMakeFiles/Feiteng.dir/src/face.cpp.o" \
"CMakeFiles/Feiteng.dir/src/log.cpp.o" \
"CMakeFiles/Feiteng.dir/src/main.cpp.o" \
"CMakeFiles/Feiteng.dir/src/mainwindow.cpp.o" \
"CMakeFiles/Feiteng.dir/src/person.cpp.o" \
"CMakeFiles/Feiteng.dir/src/temperature.cpp.o" \
"CMakeFiles/Feiteng.dir/src/thread.cpp.o" \
"CMakeFiles/Feiteng.dir/src/util.cpp.o"

# External object files for target Feiteng
Feiteng_EXTERNAL_OBJECTS =

../lib/libFeiteng.so: CMakeFiles/Feiteng.dir/Feiteng_autogen/mocs_compilation.cpp.o
../lib/libFeiteng.so: CMakeFiles/Feiteng.dir/src/admindialog.cpp.o
../lib/libFeiteng.so: CMakeFiles/Feiteng.dir/src/config.cpp.o
../lib/libFeiteng.so: CMakeFiles/Feiteng.dir/src/database.cpp.o
../lib/libFeiteng.so: CMakeFiles/Feiteng.dir/src/face.cpp.o
../lib/libFeiteng.so: CMakeFiles/Feiteng.dir/src/log.cpp.o
../lib/libFeiteng.so: CMakeFiles/Feiteng.dir/src/main.cpp.o
../lib/libFeiteng.so: CMakeFiles/Feiteng.dir/src/mainwindow.cpp.o
../lib/libFeiteng.so: CMakeFiles/Feiteng.dir/src/person.cpp.o
../lib/libFeiteng.so: CMakeFiles/Feiteng.dir/src/temperature.cpp.o
../lib/libFeiteng.so: CMakeFiles/Feiteng.dir/src/thread.cpp.o
../lib/libFeiteng.so: CMakeFiles/Feiteng.dir/src/util.cpp.o
../lib/libFeiteng.so: CMakeFiles/Feiteng.dir/build.make
../lib/libFeiteng.so: /usr/lib/aarch64-linux-gnu/libQt5Widgets.so.5.12.8
../lib/libFeiteng.so: /usr/lib/aarch64-linux-gnu/libQt5Sql.so.5.12.8
../lib/libFeiteng.so: /usr/lib/aarch64-linux-gnu/libQt5SerialPort.so.5.12.8
../lib/libFeiteng.so: /usr/local/lib/libopencv_gapi.so.4.6.0
../lib/libFeiteng.so: /usr/local/lib/libopencv_stitching.so.4.6.0
../lib/libFeiteng.so: /usr/local/lib/libopencv_aruco.so.4.6.0
../lib/libFeiteng.so: /usr/local/lib/libopencv_barcode.so.4.6.0
../lib/libFeiteng.so: /usr/local/lib/libopencv_bgsegm.so.4.6.0
../lib/libFeiteng.so: /usr/local/lib/libopencv_bioinspired.so.4.6.0
../lib/libFeiteng.so: /usr/local/lib/libopencv_ccalib.so.4.6.0
../lib/libFeiteng.so: /usr/local/lib/libopencv_dnn_objdetect.so.4.6.0
../lib/libFeiteng.so: /usr/local/lib/libopencv_dnn_superres.so.4.6.0
../lib/libFeiteng.so: /usr/local/lib/libopencv_dpm.so.4.6.0
../lib/libFeiteng.so: /usr/local/lib/libopencv_face.so.4.6.0
../lib/libFeiteng.so: /usr/local/lib/libopencv_freetype.so.4.6.0
../lib/libFeiteng.so: /usr/local/lib/libopencv_fuzzy.so.4.6.0
../lib/libFeiteng.so: /usr/local/lib/libopencv_hfs.so.4.6.0
../lib/libFeiteng.so: /usr/local/lib/libopencv_img_hash.so.4.6.0
../lib/libFeiteng.so: /usr/local/lib/libopencv_intensity_transform.so.4.6.0
../lib/libFeiteng.so: /usr/local/lib/libopencv_line_descriptor.so.4.6.0
../lib/libFeiteng.so: /usr/local/lib/libopencv_mcc.so.4.6.0
../lib/libFeiteng.so: /usr/local/lib/libopencv_quality.so.4.6.0
../lib/libFeiteng.so: /usr/local/lib/libopencv_rapid.so.4.6.0
../lib/libFeiteng.so: /usr/local/lib/libopencv_reg.so.4.6.0
../lib/libFeiteng.so: /usr/local/lib/libopencv_rgbd.so.4.6.0
../lib/libFeiteng.so: /usr/local/lib/libopencv_saliency.so.4.6.0
../lib/libFeiteng.so: /usr/local/lib/libopencv_stereo.so.4.6.0
../lib/libFeiteng.so: /usr/local/lib/libopencv_structured_light.so.4.6.0
../lib/libFeiteng.so: /usr/local/lib/libopencv_superres.so.4.6.0
../lib/libFeiteng.so: /usr/local/lib/libopencv_surface_matching.so.4.6.0
../lib/libFeiteng.so: /usr/local/lib/libopencv_tracking.so.4.6.0
../lib/libFeiteng.so: /usr/local/lib/libopencv_videostab.so.4.6.0
../lib/libFeiteng.so: /usr/local/lib/libopencv_wechat_qrcode.so.4.6.0
../lib/libFeiteng.so: /usr/local/lib/libopencv_xfeatures2d.so.4.6.0
../lib/libFeiteng.so: /usr/local/lib/libopencv_xobjdetect.so.4.6.0
../lib/libFeiteng.so: /usr/local/lib/libopencv_xphoto.so.4.6.0
../lib/libFeiteng.so: /usr/lib/aarch64-linux-gnu/libQt5Gui.so.5.12.8
../lib/libFeiteng.so: /usr/lib/aarch64-linux-gnu/libQt5Core.so.5.12.8
../lib/libFeiteng.so: /usr/local/lib/libopencv_shape.so.4.6.0
../lib/libFeiteng.so: /usr/local/lib/libopencv_highgui.so.4.6.0
../lib/libFeiteng.so: /usr/local/lib/libopencv_datasets.so.4.6.0
../lib/libFeiteng.so: /usr/local/lib/libopencv_plot.so.4.6.0
../lib/libFeiteng.so: /usr/local/lib/libopencv_text.so.4.6.0
../lib/libFeiteng.so: /usr/local/lib/libopencv_ml.so.4.6.0
../lib/libFeiteng.so: /usr/local/lib/libopencv_phase_unwrapping.so.4.6.0
../lib/libFeiteng.so: /usr/local/lib/libopencv_optflow.so.4.6.0
../lib/libFeiteng.so: /usr/local/lib/libopencv_ximgproc.so.4.6.0
../lib/libFeiteng.so: /usr/local/lib/libopencv_video.so.4.6.0
../lib/libFeiteng.so: /usr/local/lib/libopencv_videoio.so.4.6.0
../lib/libFeiteng.so: /usr/local/lib/libopencv_imgcodecs.so.4.6.0
../lib/libFeiteng.so: /usr/local/lib/libopencv_objdetect.so.4.6.0
../lib/libFeiteng.so: /usr/local/lib/libopencv_calib3d.so.4.6.0
../lib/libFeiteng.so: /usr/local/lib/libopencv_dnn.so.4.6.0
../lib/libFeiteng.so: /usr/local/lib/libopencv_features2d.so.4.6.0
../lib/libFeiteng.so: /usr/local/lib/libopencv_flann.so.4.6.0
../lib/libFeiteng.so: /usr/local/lib/libopencv_photo.so.4.6.0
../lib/libFeiteng.so: /usr/local/lib/libopencv_imgproc.so.4.6.0
../lib/libFeiteng.so: /usr/local/lib/libopencv_core.so.4.6.0
../lib/libFeiteng.so: CMakeFiles/Feiteng.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/kylin/Feiteng/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_13) "Linking CXX shared library ../lib/libFeiteng.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Feiteng.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Feiteng.dir/build: ../lib/libFeiteng.so

.PHONY : CMakeFiles/Feiteng.dir/build

CMakeFiles/Feiteng.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Feiteng.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Feiteng.dir/clean

CMakeFiles/Feiteng.dir/depend:
	cd /home/kylin/Feiteng/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/kylin/Feiteng /home/kylin/Feiteng /home/kylin/Feiteng/build /home/kylin/Feiteng/build /home/kylin/Feiteng/build/CMakeFiles/Feiteng.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Feiteng.dir/depend

