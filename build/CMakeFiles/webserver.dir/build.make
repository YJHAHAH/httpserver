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
CMAKE_SOURCE_DIR = /home/yjh/codes/WEBSERVER/code

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/yjh/codes/WEBSERVER/code/build

# Include any dependencies generated for this target.
include CMakeFiles/webserver.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/webserver.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/webserver.dir/flags.make

CMakeFiles/webserver.dir/main.cpp.o: CMakeFiles/webserver.dir/flags.make
CMakeFiles/webserver.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/yjh/codes/WEBSERVER/code/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/webserver.dir/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/webserver.dir/main.cpp.o -c /home/yjh/codes/WEBSERVER/code/main.cpp

CMakeFiles/webserver.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/webserver.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/yjh/codes/WEBSERVER/code/main.cpp > CMakeFiles/webserver.dir/main.cpp.i

CMakeFiles/webserver.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/webserver.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/yjh/codes/WEBSERVER/code/main.cpp -o CMakeFiles/webserver.dir/main.cpp.s

CMakeFiles/webserver.dir/buffer.cpp.o: CMakeFiles/webserver.dir/flags.make
CMakeFiles/webserver.dir/buffer.cpp.o: ../buffer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/yjh/codes/WEBSERVER/code/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/webserver.dir/buffer.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/webserver.dir/buffer.cpp.o -c /home/yjh/codes/WEBSERVER/code/buffer.cpp

CMakeFiles/webserver.dir/buffer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/webserver.dir/buffer.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/yjh/codes/WEBSERVER/code/buffer.cpp > CMakeFiles/webserver.dir/buffer.cpp.i

CMakeFiles/webserver.dir/buffer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/webserver.dir/buffer.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/yjh/codes/WEBSERVER/code/buffer.cpp -o CMakeFiles/webserver.dir/buffer.cpp.s

CMakeFiles/webserver.dir/epoller.cpp.o: CMakeFiles/webserver.dir/flags.make
CMakeFiles/webserver.dir/epoller.cpp.o: ../epoller.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/yjh/codes/WEBSERVER/code/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/webserver.dir/epoller.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/webserver.dir/epoller.cpp.o -c /home/yjh/codes/WEBSERVER/code/epoller.cpp

CMakeFiles/webserver.dir/epoller.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/webserver.dir/epoller.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/yjh/codes/WEBSERVER/code/epoller.cpp > CMakeFiles/webserver.dir/epoller.cpp.i

CMakeFiles/webserver.dir/epoller.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/webserver.dir/epoller.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/yjh/codes/WEBSERVER/code/epoller.cpp -o CMakeFiles/webserver.dir/epoller.cpp.s

CMakeFiles/webserver.dir/HTTPconnection.cpp.o: CMakeFiles/webserver.dir/flags.make
CMakeFiles/webserver.dir/HTTPconnection.cpp.o: ../HTTPconnection.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/yjh/codes/WEBSERVER/code/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/webserver.dir/HTTPconnection.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/webserver.dir/HTTPconnection.cpp.o -c /home/yjh/codes/WEBSERVER/code/HTTPconnection.cpp

CMakeFiles/webserver.dir/HTTPconnection.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/webserver.dir/HTTPconnection.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/yjh/codes/WEBSERVER/code/HTTPconnection.cpp > CMakeFiles/webserver.dir/HTTPconnection.cpp.i

CMakeFiles/webserver.dir/HTTPconnection.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/webserver.dir/HTTPconnection.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/yjh/codes/WEBSERVER/code/HTTPconnection.cpp -o CMakeFiles/webserver.dir/HTTPconnection.cpp.s

CMakeFiles/webserver.dir/HTTPrequest.cpp.o: CMakeFiles/webserver.dir/flags.make
CMakeFiles/webserver.dir/HTTPrequest.cpp.o: ../HTTPrequest.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/yjh/codes/WEBSERVER/code/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/webserver.dir/HTTPrequest.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/webserver.dir/HTTPrequest.cpp.o -c /home/yjh/codes/WEBSERVER/code/HTTPrequest.cpp

CMakeFiles/webserver.dir/HTTPrequest.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/webserver.dir/HTTPrequest.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/yjh/codes/WEBSERVER/code/HTTPrequest.cpp > CMakeFiles/webserver.dir/HTTPrequest.cpp.i

CMakeFiles/webserver.dir/HTTPrequest.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/webserver.dir/HTTPrequest.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/yjh/codes/WEBSERVER/code/HTTPrequest.cpp -o CMakeFiles/webserver.dir/HTTPrequest.cpp.s

CMakeFiles/webserver.dir/HTTPresponse.cpp.o: CMakeFiles/webserver.dir/flags.make
CMakeFiles/webserver.dir/HTTPresponse.cpp.o: ../HTTPresponse.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/yjh/codes/WEBSERVER/code/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/webserver.dir/HTTPresponse.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/webserver.dir/HTTPresponse.cpp.o -c /home/yjh/codes/WEBSERVER/code/HTTPresponse.cpp

CMakeFiles/webserver.dir/HTTPresponse.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/webserver.dir/HTTPresponse.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/yjh/codes/WEBSERVER/code/HTTPresponse.cpp > CMakeFiles/webserver.dir/HTTPresponse.cpp.i

CMakeFiles/webserver.dir/HTTPresponse.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/webserver.dir/HTTPresponse.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/yjh/codes/WEBSERVER/code/HTTPresponse.cpp -o CMakeFiles/webserver.dir/HTTPresponse.cpp.s

CMakeFiles/webserver.dir/log.cpp.o: CMakeFiles/webserver.dir/flags.make
CMakeFiles/webserver.dir/log.cpp.o: ../log.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/yjh/codes/WEBSERVER/code/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/webserver.dir/log.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/webserver.dir/log.cpp.o -c /home/yjh/codes/WEBSERVER/code/log.cpp

CMakeFiles/webserver.dir/log.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/webserver.dir/log.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/yjh/codes/WEBSERVER/code/log.cpp > CMakeFiles/webserver.dir/log.cpp.i

CMakeFiles/webserver.dir/log.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/webserver.dir/log.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/yjh/codes/WEBSERVER/code/log.cpp -o CMakeFiles/webserver.dir/log.cpp.s

CMakeFiles/webserver.dir/sqlconnpool.cpp.o: CMakeFiles/webserver.dir/flags.make
CMakeFiles/webserver.dir/sqlconnpool.cpp.o: ../sqlconnpool.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/yjh/codes/WEBSERVER/code/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/webserver.dir/sqlconnpool.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/webserver.dir/sqlconnpool.cpp.o -c /home/yjh/codes/WEBSERVER/code/sqlconnpool.cpp

CMakeFiles/webserver.dir/sqlconnpool.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/webserver.dir/sqlconnpool.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/yjh/codes/WEBSERVER/code/sqlconnpool.cpp > CMakeFiles/webserver.dir/sqlconnpool.cpp.i

CMakeFiles/webserver.dir/sqlconnpool.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/webserver.dir/sqlconnpool.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/yjh/codes/WEBSERVER/code/sqlconnpool.cpp -o CMakeFiles/webserver.dir/sqlconnpool.cpp.s

CMakeFiles/webserver.dir/timer.cpp.o: CMakeFiles/webserver.dir/flags.make
CMakeFiles/webserver.dir/timer.cpp.o: ../timer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/yjh/codes/WEBSERVER/code/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/webserver.dir/timer.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/webserver.dir/timer.cpp.o -c /home/yjh/codes/WEBSERVER/code/timer.cpp

CMakeFiles/webserver.dir/timer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/webserver.dir/timer.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/yjh/codes/WEBSERVER/code/timer.cpp > CMakeFiles/webserver.dir/timer.cpp.i

CMakeFiles/webserver.dir/timer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/webserver.dir/timer.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/yjh/codes/WEBSERVER/code/timer.cpp -o CMakeFiles/webserver.dir/timer.cpp.s

CMakeFiles/webserver.dir/webserver.cpp.o: CMakeFiles/webserver.dir/flags.make
CMakeFiles/webserver.dir/webserver.cpp.o: ../webserver.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/yjh/codes/WEBSERVER/code/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/webserver.dir/webserver.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/webserver.dir/webserver.cpp.o -c /home/yjh/codes/WEBSERVER/code/webserver.cpp

CMakeFiles/webserver.dir/webserver.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/webserver.dir/webserver.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/yjh/codes/WEBSERVER/code/webserver.cpp > CMakeFiles/webserver.dir/webserver.cpp.i

CMakeFiles/webserver.dir/webserver.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/webserver.dir/webserver.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/yjh/codes/WEBSERVER/code/webserver.cpp -o CMakeFiles/webserver.dir/webserver.cpp.s

# Object files for target webserver
webserver_OBJECTS = \
"CMakeFiles/webserver.dir/main.cpp.o" \
"CMakeFiles/webserver.dir/buffer.cpp.o" \
"CMakeFiles/webserver.dir/epoller.cpp.o" \
"CMakeFiles/webserver.dir/HTTPconnection.cpp.o" \
"CMakeFiles/webserver.dir/HTTPrequest.cpp.o" \
"CMakeFiles/webserver.dir/HTTPresponse.cpp.o" \
"CMakeFiles/webserver.dir/log.cpp.o" \
"CMakeFiles/webserver.dir/sqlconnpool.cpp.o" \
"CMakeFiles/webserver.dir/timer.cpp.o" \
"CMakeFiles/webserver.dir/webserver.cpp.o"

# External object files for target webserver
webserver_EXTERNAL_OBJECTS =

webserver: CMakeFiles/webserver.dir/main.cpp.o
webserver: CMakeFiles/webserver.dir/buffer.cpp.o
webserver: CMakeFiles/webserver.dir/epoller.cpp.o
webserver: CMakeFiles/webserver.dir/HTTPconnection.cpp.o
webserver: CMakeFiles/webserver.dir/HTTPrequest.cpp.o
webserver: CMakeFiles/webserver.dir/HTTPresponse.cpp.o
webserver: CMakeFiles/webserver.dir/log.cpp.o
webserver: CMakeFiles/webserver.dir/sqlconnpool.cpp.o
webserver: CMakeFiles/webserver.dir/timer.cpp.o
webserver: CMakeFiles/webserver.dir/webserver.cpp.o
webserver: CMakeFiles/webserver.dir/build.make
webserver: CMakeFiles/webserver.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/yjh/codes/WEBSERVER/code/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Linking CXX executable webserver"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/webserver.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/webserver.dir/build: webserver

.PHONY : CMakeFiles/webserver.dir/build

CMakeFiles/webserver.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/webserver.dir/cmake_clean.cmake
.PHONY : CMakeFiles/webserver.dir/clean

CMakeFiles/webserver.dir/depend:
	cd /home/yjh/codes/WEBSERVER/code/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/yjh/codes/WEBSERVER/code /home/yjh/codes/WEBSERVER/code /home/yjh/codes/WEBSERVER/code/build /home/yjh/codes/WEBSERVER/code/build /home/yjh/codes/WEBSERVER/code/build/CMakeFiles/webserver.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/webserver.dir/depend

