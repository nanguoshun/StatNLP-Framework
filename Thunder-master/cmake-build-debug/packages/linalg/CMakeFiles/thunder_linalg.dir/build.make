# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.12

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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/ngs/Documents/cplusproject/Thunder-master

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/ngs/Documents/cplusproject/Thunder-master/cmake-build-debug

# Include any dependencies generated for this target.
include packages/linalg/CMakeFiles/thunder_linalg.dir/depend.make

# Include the progress variables for this target.
include packages/linalg/CMakeFiles/thunder_linalg.dir/progress.make

# Include the compile flags for this target's objects.
include packages/linalg/CMakeFiles/thunder_linalg.dir/flags.make

packages/linalg/CMakeFiles/thunder_linalg.dir/src/cxxblas_level1.cpp.o: packages/linalg/CMakeFiles/thunder_linalg.dir/flags.make
packages/linalg/CMakeFiles/thunder_linalg.dir/src/cxxblas_level1.cpp.o: ../packages/linalg/src/cxxblas_level1.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/ngs/Documents/cplusproject/Thunder-master/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object packages/linalg/CMakeFiles/thunder_linalg.dir/src/cxxblas_level1.cpp.o"
	cd /Users/ngs/Documents/cplusproject/Thunder-master/cmake-build-debug/packages/linalg && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/thunder_linalg.dir/src/cxxblas_level1.cpp.o -c /Users/ngs/Documents/cplusproject/Thunder-master/packages/linalg/src/cxxblas_level1.cpp

packages/linalg/CMakeFiles/thunder_linalg.dir/src/cxxblas_level1.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/thunder_linalg.dir/src/cxxblas_level1.cpp.i"
	cd /Users/ngs/Documents/cplusproject/Thunder-master/cmake-build-debug/packages/linalg && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/ngs/Documents/cplusproject/Thunder-master/packages/linalg/src/cxxblas_level1.cpp > CMakeFiles/thunder_linalg.dir/src/cxxblas_level1.cpp.i

packages/linalg/CMakeFiles/thunder_linalg.dir/src/cxxblas_level1.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/thunder_linalg.dir/src/cxxblas_level1.cpp.s"
	cd /Users/ngs/Documents/cplusproject/Thunder-master/cmake-build-debug/packages/linalg && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/ngs/Documents/cplusproject/Thunder-master/packages/linalg/src/cxxblas_level1.cpp -o CMakeFiles/thunder_linalg.dir/src/cxxblas_level1.cpp.s

packages/linalg/CMakeFiles/thunder_linalg.dir/src/cxxblas_level2.cpp.o: packages/linalg/CMakeFiles/thunder_linalg.dir/flags.make
packages/linalg/CMakeFiles/thunder_linalg.dir/src/cxxblas_level2.cpp.o: ../packages/linalg/src/cxxblas_level2.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/ngs/Documents/cplusproject/Thunder-master/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object packages/linalg/CMakeFiles/thunder_linalg.dir/src/cxxblas_level2.cpp.o"
	cd /Users/ngs/Documents/cplusproject/Thunder-master/cmake-build-debug/packages/linalg && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/thunder_linalg.dir/src/cxxblas_level2.cpp.o -c /Users/ngs/Documents/cplusproject/Thunder-master/packages/linalg/src/cxxblas_level2.cpp

packages/linalg/CMakeFiles/thunder_linalg.dir/src/cxxblas_level2.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/thunder_linalg.dir/src/cxxblas_level2.cpp.i"
	cd /Users/ngs/Documents/cplusproject/Thunder-master/cmake-build-debug/packages/linalg && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/ngs/Documents/cplusproject/Thunder-master/packages/linalg/src/cxxblas_level2.cpp > CMakeFiles/thunder_linalg.dir/src/cxxblas_level2.cpp.i

packages/linalg/CMakeFiles/thunder_linalg.dir/src/cxxblas_level2.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/thunder_linalg.dir/src/cxxblas_level2.cpp.s"
	cd /Users/ngs/Documents/cplusproject/Thunder-master/cmake-build-debug/packages/linalg && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/ngs/Documents/cplusproject/Thunder-master/packages/linalg/src/cxxblas_level2.cpp -o CMakeFiles/thunder_linalg.dir/src/cxxblas_level2.cpp.s

packages/linalg/CMakeFiles/thunder_linalg.dir/src/cxxblas_level3.cpp.o: packages/linalg/CMakeFiles/thunder_linalg.dir/flags.make
packages/linalg/CMakeFiles/thunder_linalg.dir/src/cxxblas_level3.cpp.o: ../packages/linalg/src/cxxblas_level3.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/ngs/Documents/cplusproject/Thunder-master/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object packages/linalg/CMakeFiles/thunder_linalg.dir/src/cxxblas_level3.cpp.o"
	cd /Users/ngs/Documents/cplusproject/Thunder-master/cmake-build-debug/packages/linalg && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/thunder_linalg.dir/src/cxxblas_level3.cpp.o -c /Users/ngs/Documents/cplusproject/Thunder-master/packages/linalg/src/cxxblas_level3.cpp

packages/linalg/CMakeFiles/thunder_linalg.dir/src/cxxblas_level3.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/thunder_linalg.dir/src/cxxblas_level3.cpp.i"
	cd /Users/ngs/Documents/cplusproject/Thunder-master/cmake-build-debug/packages/linalg && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/ngs/Documents/cplusproject/Thunder-master/packages/linalg/src/cxxblas_level3.cpp > CMakeFiles/thunder_linalg.dir/src/cxxblas_level3.cpp.i

packages/linalg/CMakeFiles/thunder_linalg.dir/src/cxxblas_level3.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/thunder_linalg.dir/src/cxxblas_level3.cpp.s"
	cd /Users/ngs/Documents/cplusproject/Thunder-master/cmake-build-debug/packages/linalg && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/ngs/Documents/cplusproject/Thunder-master/packages/linalg/src/cxxblas_level3.cpp -o CMakeFiles/thunder_linalg.dir/src/cxxblas_level3.cpp.s

packages/linalg/CMakeFiles/thunder_linalg.dir/src/linalg.cpp.o: packages/linalg/CMakeFiles/thunder_linalg.dir/flags.make
packages/linalg/CMakeFiles/thunder_linalg.dir/src/linalg.cpp.o: ../packages/linalg/src/linalg.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/ngs/Documents/cplusproject/Thunder-master/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object packages/linalg/CMakeFiles/thunder_linalg.dir/src/linalg.cpp.o"
	cd /Users/ngs/Documents/cplusproject/Thunder-master/cmake-build-debug/packages/linalg && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/thunder_linalg.dir/src/linalg.cpp.o -c /Users/ngs/Documents/cplusproject/Thunder-master/packages/linalg/src/linalg.cpp

packages/linalg/CMakeFiles/thunder_linalg.dir/src/linalg.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/thunder_linalg.dir/src/linalg.cpp.i"
	cd /Users/ngs/Documents/cplusproject/Thunder-master/cmake-build-debug/packages/linalg && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/ngs/Documents/cplusproject/Thunder-master/packages/linalg/src/linalg.cpp > CMakeFiles/thunder_linalg.dir/src/linalg.cpp.i

packages/linalg/CMakeFiles/thunder_linalg.dir/src/linalg.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/thunder_linalg.dir/src/linalg.cpp.s"
	cd /Users/ngs/Documents/cplusproject/Thunder-master/cmake-build-debug/packages/linalg && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/ngs/Documents/cplusproject/Thunder-master/packages/linalg/src/linalg.cpp -o CMakeFiles/thunder_linalg.dir/src/linalg.cpp.s

packages/linalg/CMakeFiles/thunder_linalg.dir/src/blas.f.o: packages/linalg/CMakeFiles/thunder_linalg.dir/flags.make
packages/linalg/CMakeFiles/thunder_linalg.dir/src/blas.f.o: ../packages/linalg/src/blas.f
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/ngs/Documents/cplusproject/Thunder-master/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building Fortran object packages/linalg/CMakeFiles/thunder_linalg.dir/src/blas.f.o"
	cd /Users/ngs/Documents/cplusproject/Thunder-master/cmake-build-debug/packages/linalg && /usr/local/bin/gfortran $(Fortran_DEFINES) $(Fortran_INCLUDES) $(Fortran_FLAGS) -c /Users/ngs/Documents/cplusproject/Thunder-master/packages/linalg/src/blas.f -o CMakeFiles/thunder_linalg.dir/src/blas.f.o

packages/linalg/CMakeFiles/thunder_linalg.dir/src/blas.f.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing Fortran source to CMakeFiles/thunder_linalg.dir/src/blas.f.i"
	cd /Users/ngs/Documents/cplusproject/Thunder-master/cmake-build-debug/packages/linalg && /usr/local/bin/gfortran $(Fortran_DEFINES) $(Fortran_INCLUDES) $(Fortran_FLAGS) -E /Users/ngs/Documents/cplusproject/Thunder-master/packages/linalg/src/blas.f > CMakeFiles/thunder_linalg.dir/src/blas.f.i

packages/linalg/CMakeFiles/thunder_linalg.dir/src/blas.f.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling Fortran source to assembly CMakeFiles/thunder_linalg.dir/src/blas.f.s"
	cd /Users/ngs/Documents/cplusproject/Thunder-master/cmake-build-debug/packages/linalg && /usr/local/bin/gfortran $(Fortran_DEFINES) $(Fortran_INCLUDES) $(Fortran_FLAGS) -S /Users/ngs/Documents/cplusproject/Thunder-master/packages/linalg/src/blas.f -o CMakeFiles/thunder_linalg.dir/src/blas.f.s

# Object files for target thunder_linalg
thunder_linalg_OBJECTS = \
"CMakeFiles/thunder_linalg.dir/src/cxxblas_level1.cpp.o" \
"CMakeFiles/thunder_linalg.dir/src/cxxblas_level2.cpp.o" \
"CMakeFiles/thunder_linalg.dir/src/cxxblas_level3.cpp.o" \
"CMakeFiles/thunder_linalg.dir/src/linalg.cpp.o" \
"CMakeFiles/thunder_linalg.dir/src/blas.f.o"

# External object files for target thunder_linalg
thunder_linalg_EXTERNAL_OBJECTS =

packages/linalg/libthunder_linalg.dylib: packages/linalg/CMakeFiles/thunder_linalg.dir/src/cxxblas_level1.cpp.o
packages/linalg/libthunder_linalg.dylib: packages/linalg/CMakeFiles/thunder_linalg.dir/src/cxxblas_level2.cpp.o
packages/linalg/libthunder_linalg.dylib: packages/linalg/CMakeFiles/thunder_linalg.dir/src/cxxblas_level3.cpp.o
packages/linalg/libthunder_linalg.dylib: packages/linalg/CMakeFiles/thunder_linalg.dir/src/linalg.cpp.o
packages/linalg/libthunder_linalg.dylib: packages/linalg/CMakeFiles/thunder_linalg.dir/src/blas.f.o
packages/linalg/libthunder_linalg.dylib: packages/linalg/CMakeFiles/thunder_linalg.dir/build.make
packages/linalg/libthunder_linalg.dylib: packages/tensor/libthunder_tensor.dylib
packages/linalg/libthunder_linalg.dylib: packages/storage/libthunder_storage.dylib
packages/linalg/libthunder_linalg.dylib: packages/serializer/libthunder_serializer.dylib
packages/linalg/libthunder_linalg.dylib: packages/exception/libthunder_exception.dylib
packages/linalg/libthunder_linalg.dylib: packages/linalg/CMakeFiles/thunder_linalg.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/ngs/Documents/cplusproject/Thunder-master/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking CXX shared library libthunder_linalg.dylib"
	cd /Users/ngs/Documents/cplusproject/Thunder-master/cmake-build-debug/packages/linalg && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/thunder_linalg.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
packages/linalg/CMakeFiles/thunder_linalg.dir/build: packages/linalg/libthunder_linalg.dylib

.PHONY : packages/linalg/CMakeFiles/thunder_linalg.dir/build

packages/linalg/CMakeFiles/thunder_linalg.dir/clean:
	cd /Users/ngs/Documents/cplusproject/Thunder-master/cmake-build-debug/packages/linalg && $(CMAKE_COMMAND) -P CMakeFiles/thunder_linalg.dir/cmake_clean.cmake
.PHONY : packages/linalg/CMakeFiles/thunder_linalg.dir/clean

packages/linalg/CMakeFiles/thunder_linalg.dir/depend:
	cd /Users/ngs/Documents/cplusproject/Thunder-master/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/ngs/Documents/cplusproject/Thunder-master /Users/ngs/Documents/cplusproject/Thunder-master/packages/linalg /Users/ngs/Documents/cplusproject/Thunder-master/cmake-build-debug /Users/ngs/Documents/cplusproject/Thunder-master/cmake-build-debug/packages/linalg /Users/ngs/Documents/cplusproject/Thunder-master/cmake-build-debug/packages/linalg/CMakeFiles/thunder_linalg.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : packages/linalg/CMakeFiles/thunder_linalg.dir/depend
