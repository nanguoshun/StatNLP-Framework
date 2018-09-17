# Install script for directory: /Users/ngs/Documents/cplusproject/Thunder-master/packages/random

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/Users/ngs/Documents/cplusproject/Thunder-master/cmake-build-debug/packages/random/libthunder_random.dylib")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libthunder_random.dylib" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libthunder_random.dylib")
    execute_process(COMMAND "/usr/bin/install_name_tool"
      -id "libthunder_random.dylib"
      -change "/Users/ngs/Documents/cplusproject/Thunder-master/cmake-build-debug/packages/exception/libthunder_exception.dylib" "libthunder_exception.dylib"
      -change "/Users/ngs/Documents/cplusproject/Thunder-master/cmake-build-debug/packages/serializer/libthunder_serializer.dylib" "libthunder_serializer.dylib"
      -change "/Users/ngs/Documents/cplusproject/Thunder-master/cmake-build-debug/packages/storage/libthunder_storage.dylib" "libthunder_storage.dylib"
      -change "/Users/ngs/Documents/cplusproject/Thunder-master/cmake-build-debug/packages/tensor/libthunder_tensor.dylib" "libthunder_tensor.dylib"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libthunder_random.dylib")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libthunder_random.dylib")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "/Users/ngs/Documents/cplusproject/Thunder-master/packages/random/include/thunder" FILES_MATCHING REGEX "/[^/]*\\.hpp$")
endif()

