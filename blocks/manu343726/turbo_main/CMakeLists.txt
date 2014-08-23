
# This CMakeLists.txt file helps defining your block building and compiling
#Include the main biicode macros and functions
include(${CMAKE_HOME_DIRECTORY}/biicode.cmake)
# Define block variables
INIT_BIICODE_BLOCK()

# Manipulate here targets files, e.g.: removing or adding files depending on MacOS
# This has to be done BEFORE target definition in ADD_BIICODE_TARGETS
# You can add INCLUDE_DIRECTORIES here too

# Actually create targets: EXEcutables, STATIC libraries.
ADD_BIICODE_TARGETS()

#LLVM/Clang flags: C++11 and LLVM Standard Library
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

#Use LLVM/Clang instead of GCC
set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_COMPILER "clang++")
