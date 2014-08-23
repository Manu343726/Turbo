
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

#Example: how to activate C++11. Uncomment one of this
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11") # Win, or linux if not gcc problems
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11  -stdlib=libc++") # MacOS
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wl,--no-as-needed") # Ubuntu if gcc problems

