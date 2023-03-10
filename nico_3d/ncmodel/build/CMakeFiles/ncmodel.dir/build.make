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
CMAKE_SOURCE_DIR = /geoto/nico/nico_3d/ncmodel

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /geoto/nico/nico_3d/ncmodel/build

# Include any dependencies generated for this target.
include CMakeFiles/ncmodel.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/ncmodel.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/ncmodel.dir/flags.make

qrc_ncmodel_shaders.cxx: ../shaders/mdl.vert
qrc_ncmodel_shaders.cxx: ../shaders/mdl.frag
qrc_ncmodel_shaders.cxx: ../shaders/skp.vert
qrc_ncmodel_shaders.cxx: ../shaders/skp.geom
qrc_ncmodel_shaders.cxx: ../shaders/skp.frag
qrc_ncmodel_shaders.cxx: ../shaders/assimp.vert
qrc_ncmodel_shaders.cxx: ../shaders/assimp.geom
qrc_ncmodel_shaders.cxx: ../shaders/assimp.frag
qrc_ncmodel_shaders.cxx: ../shaders/gvm.vert
qrc_ncmodel_shaders.cxx: ../shaders/gvm.geom
qrc_ncmodel_shaders.cxx: ../shaders/gvm.glsl
qrc_ncmodel_shaders.cxx: ../shaders/gvm.frag
qrc_ncmodel_shaders.cxx: ../shaders/gvmd.frag
qrc_ncmodel_shaders.cxx: ncmodel_shaders.qrc.depends
qrc_ncmodel_shaders.cxx: ../ncmodel_shaders.qrc
	$(CMAKE_COMMAND) -E cmake_progress_report /geoto/nico/nico_3d/ncmodel/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Generating qrc_ncmodel_shaders.cxx"
	/usr/lib64/qt4/bin/rcc -name ncmodel_shaders -o /geoto/nico/nico_3d/ncmodel/build/qrc_ncmodel_shaders.cxx /geoto/nico/nico_3d/ncmodel/ncmodel_shaders.qrc

CMakeFiles/ncmodel.dir/src/ncmodel.cpp.o: CMakeFiles/ncmodel.dir/flags.make
CMakeFiles/ncmodel.dir/src/ncmodel.cpp.o: ../src/ncmodel.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /geoto/nico/nico_3d/ncmodel/build/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/ncmodel.dir/src/ncmodel.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/ncmodel.dir/src/ncmodel.cpp.o -c /geoto/nico/nico_3d/ncmodel/src/ncmodel.cpp

CMakeFiles/ncmodel.dir/src/ncmodel.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ncmodel.dir/src/ncmodel.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /geoto/nico/nico_3d/ncmodel/src/ncmodel.cpp > CMakeFiles/ncmodel.dir/src/ncmodel.cpp.i

CMakeFiles/ncmodel.dir/src/ncmodel.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ncmodel.dir/src/ncmodel.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /geoto/nico/nico_3d/ncmodel/src/ncmodel.cpp -o CMakeFiles/ncmodel.dir/src/ncmodel.cpp.s

CMakeFiles/ncmodel.dir/src/ncmodel.cpp.o.requires:
.PHONY : CMakeFiles/ncmodel.dir/src/ncmodel.cpp.o.requires

CMakeFiles/ncmodel.dir/src/ncmodel.cpp.o.provides: CMakeFiles/ncmodel.dir/src/ncmodel.cpp.o.requires
	$(MAKE) -f CMakeFiles/ncmodel.dir/build.make CMakeFiles/ncmodel.dir/src/ncmodel.cpp.o.provides.build
.PHONY : CMakeFiles/ncmodel.dir/src/ncmodel.cpp.o.provides

CMakeFiles/ncmodel.dir/src/ncmodel.cpp.o.provides.build: CMakeFiles/ncmodel.dir/src/ncmodel.cpp.o

CMakeFiles/ncmodel.dir/src/ncassimp.cpp.o: CMakeFiles/ncmodel.dir/flags.make
CMakeFiles/ncmodel.dir/src/ncassimp.cpp.o: ../src/ncassimp.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /geoto/nico/nico_3d/ncmodel/build/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/ncmodel.dir/src/ncassimp.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/ncmodel.dir/src/ncassimp.cpp.o -c /geoto/nico/nico_3d/ncmodel/src/ncassimp.cpp

CMakeFiles/ncmodel.dir/src/ncassimp.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ncmodel.dir/src/ncassimp.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /geoto/nico/nico_3d/ncmodel/src/ncassimp.cpp > CMakeFiles/ncmodel.dir/src/ncassimp.cpp.i

CMakeFiles/ncmodel.dir/src/ncassimp.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ncmodel.dir/src/ncassimp.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /geoto/nico/nico_3d/ncmodel/src/ncassimp.cpp -o CMakeFiles/ncmodel.dir/src/ncassimp.cpp.s

CMakeFiles/ncmodel.dir/src/ncassimp.cpp.o.requires:
.PHONY : CMakeFiles/ncmodel.dir/src/ncassimp.cpp.o.requires

CMakeFiles/ncmodel.dir/src/ncassimp.cpp.o.provides: CMakeFiles/ncmodel.dir/src/ncassimp.cpp.o.requires
	$(MAKE) -f CMakeFiles/ncmodel.dir/build.make CMakeFiles/ncmodel.dir/src/ncassimp.cpp.o.provides.build
.PHONY : CMakeFiles/ncmodel.dir/src/ncassimp.cpp.o.provides

CMakeFiles/ncmodel.dir/src/ncassimp.cpp.o.provides.build: CMakeFiles/ncmodel.dir/src/ncassimp.cpp.o

CMakeFiles/ncmodel.dir/src/ncmdl.cpp.o: CMakeFiles/ncmodel.dir/flags.make
CMakeFiles/ncmodel.dir/src/ncmdl.cpp.o: ../src/ncmdl.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /geoto/nico/nico_3d/ncmodel/build/CMakeFiles $(CMAKE_PROGRESS_4)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/ncmodel.dir/src/ncmdl.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/ncmodel.dir/src/ncmdl.cpp.o -c /geoto/nico/nico_3d/ncmodel/src/ncmdl.cpp

CMakeFiles/ncmodel.dir/src/ncmdl.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ncmodel.dir/src/ncmdl.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /geoto/nico/nico_3d/ncmodel/src/ncmdl.cpp > CMakeFiles/ncmodel.dir/src/ncmdl.cpp.i

CMakeFiles/ncmodel.dir/src/ncmdl.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ncmodel.dir/src/ncmdl.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /geoto/nico/nico_3d/ncmodel/src/ncmdl.cpp -o CMakeFiles/ncmodel.dir/src/ncmdl.cpp.s

CMakeFiles/ncmodel.dir/src/ncmdl.cpp.o.requires:
.PHONY : CMakeFiles/ncmodel.dir/src/ncmdl.cpp.o.requires

CMakeFiles/ncmodel.dir/src/ncmdl.cpp.o.provides: CMakeFiles/ncmodel.dir/src/ncmdl.cpp.o.requires
	$(MAKE) -f CMakeFiles/ncmodel.dir/build.make CMakeFiles/ncmodel.dir/src/ncmdl.cpp.o.provides.build
.PHONY : CMakeFiles/ncmodel.dir/src/ncmdl.cpp.o.provides

CMakeFiles/ncmodel.dir/src/ncmdl.cpp.o.provides.build: CMakeFiles/ncmodel.dir/src/ncmdl.cpp.o

CMakeFiles/ncmodel.dir/src/ncgvm.cpp.o: CMakeFiles/ncmodel.dir/flags.make
CMakeFiles/ncmodel.dir/src/ncgvm.cpp.o: ../src/ncgvm.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /geoto/nico/nico_3d/ncmodel/build/CMakeFiles $(CMAKE_PROGRESS_5)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/ncmodel.dir/src/ncgvm.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/ncmodel.dir/src/ncgvm.cpp.o -c /geoto/nico/nico_3d/ncmodel/src/ncgvm.cpp

CMakeFiles/ncmodel.dir/src/ncgvm.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ncmodel.dir/src/ncgvm.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /geoto/nico/nico_3d/ncmodel/src/ncgvm.cpp > CMakeFiles/ncmodel.dir/src/ncgvm.cpp.i

CMakeFiles/ncmodel.dir/src/ncgvm.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ncmodel.dir/src/ncgvm.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /geoto/nico/nico_3d/ncmodel/src/ncgvm.cpp -o CMakeFiles/ncmodel.dir/src/ncgvm.cpp.s

CMakeFiles/ncmodel.dir/src/ncgvm.cpp.o.requires:
.PHONY : CMakeFiles/ncmodel.dir/src/ncgvm.cpp.o.requires

CMakeFiles/ncmodel.dir/src/ncgvm.cpp.o.provides: CMakeFiles/ncmodel.dir/src/ncgvm.cpp.o.requires
	$(MAKE) -f CMakeFiles/ncmodel.dir/build.make CMakeFiles/ncmodel.dir/src/ncgvm.cpp.o.provides.build
.PHONY : CMakeFiles/ncmodel.dir/src/ncgvm.cpp.o.provides

CMakeFiles/ncmodel.dir/src/ncgvm.cpp.o.provides.build: CMakeFiles/ncmodel.dir/src/ncgvm.cpp.o

CMakeFiles/ncmodel.dir/qrc_ncmodel_shaders.cxx.o: CMakeFiles/ncmodel.dir/flags.make
CMakeFiles/ncmodel.dir/qrc_ncmodel_shaders.cxx.o: qrc_ncmodel_shaders.cxx
	$(CMAKE_COMMAND) -E cmake_progress_report /geoto/nico/nico_3d/ncmodel/build/CMakeFiles $(CMAKE_PROGRESS_6)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/ncmodel.dir/qrc_ncmodel_shaders.cxx.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/ncmodel.dir/qrc_ncmodel_shaders.cxx.o -c /geoto/nico/nico_3d/ncmodel/build/qrc_ncmodel_shaders.cxx

CMakeFiles/ncmodel.dir/qrc_ncmodel_shaders.cxx.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ncmodel.dir/qrc_ncmodel_shaders.cxx.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /geoto/nico/nico_3d/ncmodel/build/qrc_ncmodel_shaders.cxx > CMakeFiles/ncmodel.dir/qrc_ncmodel_shaders.cxx.i

CMakeFiles/ncmodel.dir/qrc_ncmodel_shaders.cxx.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ncmodel.dir/qrc_ncmodel_shaders.cxx.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /geoto/nico/nico_3d/ncmodel/build/qrc_ncmodel_shaders.cxx -o CMakeFiles/ncmodel.dir/qrc_ncmodel_shaders.cxx.s

CMakeFiles/ncmodel.dir/qrc_ncmodel_shaders.cxx.o.requires:
.PHONY : CMakeFiles/ncmodel.dir/qrc_ncmodel_shaders.cxx.o.requires

CMakeFiles/ncmodel.dir/qrc_ncmodel_shaders.cxx.o.provides: CMakeFiles/ncmodel.dir/qrc_ncmodel_shaders.cxx.o.requires
	$(MAKE) -f CMakeFiles/ncmodel.dir/build.make CMakeFiles/ncmodel.dir/qrc_ncmodel_shaders.cxx.o.provides.build
.PHONY : CMakeFiles/ncmodel.dir/qrc_ncmodel_shaders.cxx.o.provides

CMakeFiles/ncmodel.dir/qrc_ncmodel_shaders.cxx.o.provides.build: CMakeFiles/ncmodel.dir/qrc_ncmodel_shaders.cxx.o

# Object files for target ncmodel
ncmodel_OBJECTS = \
"CMakeFiles/ncmodel.dir/src/ncmodel.cpp.o" \
"CMakeFiles/ncmodel.dir/src/ncassimp.cpp.o" \
"CMakeFiles/ncmodel.dir/src/ncmdl.cpp.o" \
"CMakeFiles/ncmodel.dir/src/ncgvm.cpp.o" \
"CMakeFiles/ncmodel.dir/qrc_ncmodel_shaders.cxx.o"

# External object files for target ncmodel
ncmodel_EXTERNAL_OBJECTS =

lib/libncmodel.a: CMakeFiles/ncmodel.dir/src/ncmodel.cpp.o
lib/libncmodel.a: CMakeFiles/ncmodel.dir/src/ncassimp.cpp.o
lib/libncmodel.a: CMakeFiles/ncmodel.dir/src/ncmdl.cpp.o
lib/libncmodel.a: CMakeFiles/ncmodel.dir/src/ncgvm.cpp.o
lib/libncmodel.a: CMakeFiles/ncmodel.dir/qrc_ncmodel_shaders.cxx.o
lib/libncmodel.a: CMakeFiles/ncmodel.dir/build.make
lib/libncmodel.a: CMakeFiles/ncmodel.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX static library lib/libncmodel.a"
	$(CMAKE_COMMAND) -P CMakeFiles/ncmodel.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ncmodel.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/ncmodel.dir/build: lib/libncmodel.a
.PHONY : CMakeFiles/ncmodel.dir/build

CMakeFiles/ncmodel.dir/requires: CMakeFiles/ncmodel.dir/src/ncmodel.cpp.o.requires
CMakeFiles/ncmodel.dir/requires: CMakeFiles/ncmodel.dir/src/ncassimp.cpp.o.requires
CMakeFiles/ncmodel.dir/requires: CMakeFiles/ncmodel.dir/src/ncmdl.cpp.o.requires
CMakeFiles/ncmodel.dir/requires: CMakeFiles/ncmodel.dir/src/ncgvm.cpp.o.requires
CMakeFiles/ncmodel.dir/requires: CMakeFiles/ncmodel.dir/qrc_ncmodel_shaders.cxx.o.requires
.PHONY : CMakeFiles/ncmodel.dir/requires

CMakeFiles/ncmodel.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/ncmodel.dir/cmake_clean.cmake
.PHONY : CMakeFiles/ncmodel.dir/clean

CMakeFiles/ncmodel.dir/depend: qrc_ncmodel_shaders.cxx
	cd /geoto/nico/nico_3d/ncmodel/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /geoto/nico/nico_3d/ncmodel /geoto/nico/nico_3d/ncmodel /geoto/nico/nico_3d/ncmodel/build /geoto/nico/nico_3d/ncmodel/build /geoto/nico/nico_3d/ncmodel/build/CMakeFiles/ncmodel.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/ncmodel.dir/depend

