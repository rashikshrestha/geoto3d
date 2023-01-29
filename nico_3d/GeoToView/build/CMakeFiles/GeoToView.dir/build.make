# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /geoto/nico/nico_3d/GeoToView

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /geoto/nico/nico_3d/GeoToView/build

# Include any dependencies generated for this target.
include CMakeFiles/GeoToView.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/GeoToView.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/GeoToView.dir/flags.make

inc/moc_mainwindow.cxx: ../inc/mainwindow.h
	$(CMAKE_COMMAND) -E cmake_progress_report /geoto/nico/nico_3d/GeoToView/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Generating inc/moc_mainwindow.cxx"
	cd /geoto/nico/nico_3d/GeoToView/build/inc && /usr/lib64/qt4/bin/moc-qt4 @/geoto/nico/nico_3d/GeoToView/build/inc/moc_mainwindow.cxx_parameters

inc/moc_ncapp.cxx: ../inc/ncapp.h
	$(CMAKE_COMMAND) -E cmake_progress_report /geoto/nico/nico_3d/GeoToView/build/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Generating inc/moc_ncapp.cxx"
	cd /geoto/nico/nico_3d/GeoToView/build/inc && /usr/lib64/qt4/bin/moc-qt4 @/geoto/nico/nico_3d/GeoToView/build/inc/moc_ncapp.cxx_parameters

qrc_geotoview.cxx: ../shaders/ball.vert
qrc_geotoview.cxx: ../shaders/ball.geom
qrc_geotoview.cxx: ../shaders/ball.frag
qrc_geotoview.cxx: ../shaders/cylinder.vert
qrc_geotoview.cxx: ../shaders/cylinder.geom
qrc_geotoview.cxx: ../shaders/cylinder.frag
qrc_geotoview.cxx: ../shaders/stereo.vert
qrc_geotoview.cxx: ../shaders/stereo.frag
qrc_geotoview.cxx: ../shaders/blit.vert
qrc_geotoview.cxx: ../shaders/blit.frag
qrc_geotoview.cxx: ../style/style.qss
qrc_geotoview.cxx: geotoview.qrc.depends
qrc_geotoview.cxx: ../geotoview.qrc
	$(CMAKE_COMMAND) -E cmake_progress_report /geoto/nico/nico_3d/GeoToView/build/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Generating qrc_geotoview.cxx"
	/usr/lib64/qt4/bin/rcc -name geotoview -o /geoto/nico/nico_3d/GeoToView/build/qrc_geotoview.cxx /geoto/nico/nico_3d/GeoToView/geotoview.qrc

CMakeFiles/GeoToView.dir/src/main.cpp.o: CMakeFiles/GeoToView.dir/flags.make
CMakeFiles/GeoToView.dir/src/main.cpp.o: ../src/main.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /geoto/nico/nico_3d/GeoToView/build/CMakeFiles $(CMAKE_PROGRESS_4)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/GeoToView.dir/src/main.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/GeoToView.dir/src/main.cpp.o -c /geoto/nico/nico_3d/GeoToView/src/main.cpp

CMakeFiles/GeoToView.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/GeoToView.dir/src/main.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /geoto/nico/nico_3d/GeoToView/src/main.cpp > CMakeFiles/GeoToView.dir/src/main.cpp.i

CMakeFiles/GeoToView.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/GeoToView.dir/src/main.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /geoto/nico/nico_3d/GeoToView/src/main.cpp -o CMakeFiles/GeoToView.dir/src/main.cpp.s

CMakeFiles/GeoToView.dir/src/main.cpp.o.requires:
.PHONY : CMakeFiles/GeoToView.dir/src/main.cpp.o.requires

CMakeFiles/GeoToView.dir/src/main.cpp.o.provides: CMakeFiles/GeoToView.dir/src/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/GeoToView.dir/build.make CMakeFiles/GeoToView.dir/src/main.cpp.o.provides.build
.PHONY : CMakeFiles/GeoToView.dir/src/main.cpp.o.provides

CMakeFiles/GeoToView.dir/src/main.cpp.o.provides.build: CMakeFiles/GeoToView.dir/src/main.cpp.o

CMakeFiles/GeoToView.dir/src/mainwindow.cpp.o: CMakeFiles/GeoToView.dir/flags.make
CMakeFiles/GeoToView.dir/src/mainwindow.cpp.o: ../src/mainwindow.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /geoto/nico/nico_3d/GeoToView/build/CMakeFiles $(CMAKE_PROGRESS_5)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/GeoToView.dir/src/mainwindow.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/GeoToView.dir/src/mainwindow.cpp.o -c /geoto/nico/nico_3d/GeoToView/src/mainwindow.cpp

CMakeFiles/GeoToView.dir/src/mainwindow.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/GeoToView.dir/src/mainwindow.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /geoto/nico/nico_3d/GeoToView/src/mainwindow.cpp > CMakeFiles/GeoToView.dir/src/mainwindow.cpp.i

CMakeFiles/GeoToView.dir/src/mainwindow.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/GeoToView.dir/src/mainwindow.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /geoto/nico/nico_3d/GeoToView/src/mainwindow.cpp -o CMakeFiles/GeoToView.dir/src/mainwindow.cpp.s

CMakeFiles/GeoToView.dir/src/mainwindow.cpp.o.requires:
.PHONY : CMakeFiles/GeoToView.dir/src/mainwindow.cpp.o.requires

CMakeFiles/GeoToView.dir/src/mainwindow.cpp.o.provides: CMakeFiles/GeoToView.dir/src/mainwindow.cpp.o.requires
	$(MAKE) -f CMakeFiles/GeoToView.dir/build.make CMakeFiles/GeoToView.dir/src/mainwindow.cpp.o.provides.build
.PHONY : CMakeFiles/GeoToView.dir/src/mainwindow.cpp.o.provides

CMakeFiles/GeoToView.dir/src/mainwindow.cpp.o.provides.build: CMakeFiles/GeoToView.dir/src/mainwindow.cpp.o

CMakeFiles/GeoToView.dir/src/ncapp.cpp.o: CMakeFiles/GeoToView.dir/flags.make
CMakeFiles/GeoToView.dir/src/ncapp.cpp.o: ../src/ncapp.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /geoto/nico/nico_3d/GeoToView/build/CMakeFiles $(CMAKE_PROGRESS_6)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/GeoToView.dir/src/ncapp.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/GeoToView.dir/src/ncapp.cpp.o -c /geoto/nico/nico_3d/GeoToView/src/ncapp.cpp

CMakeFiles/GeoToView.dir/src/ncapp.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/GeoToView.dir/src/ncapp.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /geoto/nico/nico_3d/GeoToView/src/ncapp.cpp > CMakeFiles/GeoToView.dir/src/ncapp.cpp.i

CMakeFiles/GeoToView.dir/src/ncapp.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/GeoToView.dir/src/ncapp.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /geoto/nico/nico_3d/GeoToView/src/ncapp.cpp -o CMakeFiles/GeoToView.dir/src/ncapp.cpp.s

CMakeFiles/GeoToView.dir/src/ncapp.cpp.o.requires:
.PHONY : CMakeFiles/GeoToView.dir/src/ncapp.cpp.o.requires

CMakeFiles/GeoToView.dir/src/ncapp.cpp.o.provides: CMakeFiles/GeoToView.dir/src/ncapp.cpp.o.requires
	$(MAKE) -f CMakeFiles/GeoToView.dir/build.make CMakeFiles/GeoToView.dir/src/ncapp.cpp.o.provides.build
.PHONY : CMakeFiles/GeoToView.dir/src/ncapp.cpp.o.provides

CMakeFiles/GeoToView.dir/src/ncapp.cpp.o.provides.build: CMakeFiles/GeoToView.dir/src/ncapp.cpp.o

CMakeFiles/GeoToView.dir/inc/moc_mainwindow.cxx.o: CMakeFiles/GeoToView.dir/flags.make
CMakeFiles/GeoToView.dir/inc/moc_mainwindow.cxx.o: inc/moc_mainwindow.cxx
	$(CMAKE_COMMAND) -E cmake_progress_report /geoto/nico/nico_3d/GeoToView/build/CMakeFiles $(CMAKE_PROGRESS_7)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/GeoToView.dir/inc/moc_mainwindow.cxx.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/GeoToView.dir/inc/moc_mainwindow.cxx.o -c /geoto/nico/nico_3d/GeoToView/build/inc/moc_mainwindow.cxx

CMakeFiles/GeoToView.dir/inc/moc_mainwindow.cxx.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/GeoToView.dir/inc/moc_mainwindow.cxx.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /geoto/nico/nico_3d/GeoToView/build/inc/moc_mainwindow.cxx > CMakeFiles/GeoToView.dir/inc/moc_mainwindow.cxx.i

CMakeFiles/GeoToView.dir/inc/moc_mainwindow.cxx.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/GeoToView.dir/inc/moc_mainwindow.cxx.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /geoto/nico/nico_3d/GeoToView/build/inc/moc_mainwindow.cxx -o CMakeFiles/GeoToView.dir/inc/moc_mainwindow.cxx.s

CMakeFiles/GeoToView.dir/inc/moc_mainwindow.cxx.o.requires:
.PHONY : CMakeFiles/GeoToView.dir/inc/moc_mainwindow.cxx.o.requires

CMakeFiles/GeoToView.dir/inc/moc_mainwindow.cxx.o.provides: CMakeFiles/GeoToView.dir/inc/moc_mainwindow.cxx.o.requires
	$(MAKE) -f CMakeFiles/GeoToView.dir/build.make CMakeFiles/GeoToView.dir/inc/moc_mainwindow.cxx.o.provides.build
.PHONY : CMakeFiles/GeoToView.dir/inc/moc_mainwindow.cxx.o.provides

CMakeFiles/GeoToView.dir/inc/moc_mainwindow.cxx.o.provides.build: CMakeFiles/GeoToView.dir/inc/moc_mainwindow.cxx.o

CMakeFiles/GeoToView.dir/inc/moc_ncapp.cxx.o: CMakeFiles/GeoToView.dir/flags.make
CMakeFiles/GeoToView.dir/inc/moc_ncapp.cxx.o: inc/moc_ncapp.cxx
	$(CMAKE_COMMAND) -E cmake_progress_report /geoto/nico/nico_3d/GeoToView/build/CMakeFiles $(CMAKE_PROGRESS_8)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/GeoToView.dir/inc/moc_ncapp.cxx.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/GeoToView.dir/inc/moc_ncapp.cxx.o -c /geoto/nico/nico_3d/GeoToView/build/inc/moc_ncapp.cxx

CMakeFiles/GeoToView.dir/inc/moc_ncapp.cxx.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/GeoToView.dir/inc/moc_ncapp.cxx.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /geoto/nico/nico_3d/GeoToView/build/inc/moc_ncapp.cxx > CMakeFiles/GeoToView.dir/inc/moc_ncapp.cxx.i

CMakeFiles/GeoToView.dir/inc/moc_ncapp.cxx.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/GeoToView.dir/inc/moc_ncapp.cxx.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /geoto/nico/nico_3d/GeoToView/build/inc/moc_ncapp.cxx -o CMakeFiles/GeoToView.dir/inc/moc_ncapp.cxx.s

CMakeFiles/GeoToView.dir/inc/moc_ncapp.cxx.o.requires:
.PHONY : CMakeFiles/GeoToView.dir/inc/moc_ncapp.cxx.o.requires

CMakeFiles/GeoToView.dir/inc/moc_ncapp.cxx.o.provides: CMakeFiles/GeoToView.dir/inc/moc_ncapp.cxx.o.requires
	$(MAKE) -f CMakeFiles/GeoToView.dir/build.make CMakeFiles/GeoToView.dir/inc/moc_ncapp.cxx.o.provides.build
.PHONY : CMakeFiles/GeoToView.dir/inc/moc_ncapp.cxx.o.provides

CMakeFiles/GeoToView.dir/inc/moc_ncapp.cxx.o.provides.build: CMakeFiles/GeoToView.dir/inc/moc_ncapp.cxx.o

CMakeFiles/GeoToView.dir/qrc_geotoview.cxx.o: CMakeFiles/GeoToView.dir/flags.make
CMakeFiles/GeoToView.dir/qrc_geotoview.cxx.o: qrc_geotoview.cxx
	$(CMAKE_COMMAND) -E cmake_progress_report /geoto/nico/nico_3d/GeoToView/build/CMakeFiles $(CMAKE_PROGRESS_9)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/GeoToView.dir/qrc_geotoview.cxx.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/GeoToView.dir/qrc_geotoview.cxx.o -c /geoto/nico/nico_3d/GeoToView/build/qrc_geotoview.cxx

CMakeFiles/GeoToView.dir/qrc_geotoview.cxx.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/GeoToView.dir/qrc_geotoview.cxx.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /geoto/nico/nico_3d/GeoToView/build/qrc_geotoview.cxx > CMakeFiles/GeoToView.dir/qrc_geotoview.cxx.i

CMakeFiles/GeoToView.dir/qrc_geotoview.cxx.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/GeoToView.dir/qrc_geotoview.cxx.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /geoto/nico/nico_3d/GeoToView/build/qrc_geotoview.cxx -o CMakeFiles/GeoToView.dir/qrc_geotoview.cxx.s

CMakeFiles/GeoToView.dir/qrc_geotoview.cxx.o.requires:
.PHONY : CMakeFiles/GeoToView.dir/qrc_geotoview.cxx.o.requires

CMakeFiles/GeoToView.dir/qrc_geotoview.cxx.o.provides: CMakeFiles/GeoToView.dir/qrc_geotoview.cxx.o.requires
	$(MAKE) -f CMakeFiles/GeoToView.dir/build.make CMakeFiles/GeoToView.dir/qrc_geotoview.cxx.o.provides.build
.PHONY : CMakeFiles/GeoToView.dir/qrc_geotoview.cxx.o.provides

CMakeFiles/GeoToView.dir/qrc_geotoview.cxx.o.provides.build: CMakeFiles/GeoToView.dir/qrc_geotoview.cxx.o

# Object files for target GeoToView
GeoToView_OBJECTS = \
"CMakeFiles/GeoToView.dir/src/main.cpp.o" \
"CMakeFiles/GeoToView.dir/src/mainwindow.cpp.o" \
"CMakeFiles/GeoToView.dir/src/ncapp.cpp.o" \
"CMakeFiles/GeoToView.dir/inc/moc_mainwindow.cxx.o" \
"CMakeFiles/GeoToView.dir/inc/moc_ncapp.cxx.o" \
"CMakeFiles/GeoToView.dir/qrc_geotoview.cxx.o"

# External object files for target GeoToView
GeoToView_EXTERNAL_OBJECTS =

bin/GeoToView: CMakeFiles/GeoToView.dir/src/main.cpp.o
bin/GeoToView: CMakeFiles/GeoToView.dir/src/mainwindow.cpp.o
bin/GeoToView: CMakeFiles/GeoToView.dir/src/ncapp.cpp.o
bin/GeoToView: CMakeFiles/GeoToView.dir/inc/moc_mainwindow.cxx.o
bin/GeoToView: CMakeFiles/GeoToView.dir/inc/moc_ncapp.cxx.o
bin/GeoToView: CMakeFiles/GeoToView.dir/qrc_geotoview.cxx.o
bin/GeoToView: CMakeFiles/GeoToView.dir/build.make
bin/GeoToView: /usr/lib64/libQtOpenGL.so
bin/GeoToView: /usr/lib64/libQtGui.so
bin/GeoToView: /usr/lib64/libQtXml.so
bin/GeoToView: /usr/lib64/libQtNetwork.so
bin/GeoToView: /usr/lib64/libQtCore.so
bin/GeoToView: /usr/local/cuda/lib64/libcudart.so
bin/GeoToView: CMakeFiles/GeoToView.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable bin/GeoToView"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/GeoToView.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/GeoToView.dir/build: bin/GeoToView
.PHONY : CMakeFiles/GeoToView.dir/build

CMakeFiles/GeoToView.dir/requires: CMakeFiles/GeoToView.dir/src/main.cpp.o.requires
CMakeFiles/GeoToView.dir/requires: CMakeFiles/GeoToView.dir/src/mainwindow.cpp.o.requires
CMakeFiles/GeoToView.dir/requires: CMakeFiles/GeoToView.dir/src/ncapp.cpp.o.requires
CMakeFiles/GeoToView.dir/requires: CMakeFiles/GeoToView.dir/inc/moc_mainwindow.cxx.o.requires
CMakeFiles/GeoToView.dir/requires: CMakeFiles/GeoToView.dir/inc/moc_ncapp.cxx.o.requires
CMakeFiles/GeoToView.dir/requires: CMakeFiles/GeoToView.dir/qrc_geotoview.cxx.o.requires
.PHONY : CMakeFiles/GeoToView.dir/requires

CMakeFiles/GeoToView.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/GeoToView.dir/cmake_clean.cmake
.PHONY : CMakeFiles/GeoToView.dir/clean

CMakeFiles/GeoToView.dir/depend: inc/moc_mainwindow.cxx
CMakeFiles/GeoToView.dir/depend: inc/moc_ncapp.cxx
CMakeFiles/GeoToView.dir/depend: qrc_geotoview.cxx
	cd /geoto/nico/nico_3d/GeoToView/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /geoto/nico/nico_3d/GeoToView /geoto/nico/nico_3d/GeoToView /geoto/nico/nico_3d/GeoToView/build /geoto/nico/nico_3d/GeoToView/build /geoto/nico/nico_3d/GeoToView/build/CMakeFiles/GeoToView.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/GeoToView.dir/depend
