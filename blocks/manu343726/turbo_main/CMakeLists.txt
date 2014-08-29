
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

# Enable C++11
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

#Under TravisCI the .travis.yml script should define a TRAVIS_CXX variable with the current value of CXX ( export TRAVIS_CXX=$CXX )

IF($TRAVIS_CXX="")
	# No under TravisCI environment.

	IF($CXX="clang++")
	    # Use LLVM libc++
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -lc++abi")
	ENDIF($CXX="clang++")

ELSE($TRAVIS_CXX="")
	# Under TravisCI environment.
	
	set(CMAKE_C_COMPILER "clang")
	set(CMAKE_CXX_COMPILER "clang++")

	IF($TRAVIS_CXX="clang++")
	    # Force Clang usage
	    set(CMAKE_C_COMPILER "clang")
	    	set(CMAKE_C_COMPILER "clang")
		set(CMAKE_CXX_COMPILER "clang++")

		# Use LLVM libc++
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -lc++abi")
	ENDIF($TRAVIS_CXX="clang++")

ENDIF($TRAVIS_CXX="")

