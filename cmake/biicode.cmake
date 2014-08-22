#=============================================================================#
#                                                                             #
#         				  biicode.cmake    									  #
#                                                                             #
#=============================================================================#
# This file contains all the macros and functions that make possible to create
# the targets (executables, libraries, etc) and respect the user project
# configuration and/or CMakeLists.txt files


#=============================================================================#
#				General SET and INCLUDE_DIRECTORIES
#=============================================================================#

# Defining variables to save the created binary files
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/../bin)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_HOME_DIRECTORY}/../bin)

# Including /blocks and /deps directories
INCLUDE_DIRECTORIES(${CMAKE_HOME_DIRECTORY}/../blocks)
INCLUDE_DIRECTORIES(${CMAKE_HOME_DIRECTORY}/../deps)

# The BIICODE variable is used to execute (or not) some wished parts of any user
# block CMakeLists. For example:
# 		IF(NOT BIICODE)
#			ADD_SUBDIRECTORY(util)
# 		ENDIF()
SET(BIICODE TRUE)

#=============================================================================#
#
#						PUBLIC FUNCTIONS AND MACROS
#
#=============================================================================#

#=============================================================================#
# [PUBLIC/USER] [USER BLOCK CMAKELISTS]
#
# INIT_BIICODE_BLOCK()
#
# Defines and creates CMAKE vars, that are used by biicode. 
# This macro is exclusive for the root CMakeLists of a block. 
# Should be executed at the beginning, after the biicode.cmake inclusion.
#
# This macro decides if target is a library or an executable file.
#
#=============================================================================#

macro(INIT_BIICODE_BLOCK)
	CREATE_BII_BLOCK_VARS(${CMAKE_CURRENT_SOURCE_DIR})
	SET(vname "${BII_BLOCK_USER}_${BII_BLOCK_NAME}")
	# Load vars.cmake file with variables
	SET(var_file_name "bii_${vname}_vars.cmake")
	if(NOT EXISTS "${CMAKE_HOME_DIRECTORY}/${var_file_name}")
		message(FATAL_ERROR "ERROR: ${CMAKE_HOME_DIRECTORY}/${var_file_name} does not exist") 
	endif()
	INCLUDE(${CMAKE_HOME_DIRECTORY}/${var_file_name})

	# Generic alias for target
	SET(BII_LIB_SRC ${BII_${vname}_SRC})

	# Creates a block target alias for each block executable
	foreach(executable  ${BII_${vname}_EXES} )
		SET(BII_${executable}_SRC ${BII_${vname}_${executable}_SRC})
	endforeach()
	if(BII_${vname}_LIB)
		SET(BII_CREATE_LIB TRUE)
		message(AUTHOR_WARNING "BLOCK LIB IS GOING TO BE CREATED")
	endif()

	# Flag to inform that the block have been initiated
	SET(BII_${vname}_INITIATED TRUE CACHE INTERNAL "biicode")
	SET(BII_${vname}_FINISHED FALSE CACHE INTERNAL "biicode")
endmacro() 

#=============================================================================#
# [PUBLIC/USER] [USER BLOCK CMAKELISTS]
#
# ADD_BIICODE_TARGETS()
#
#
# Adds and selects the target to be created. It's in charge of calling the macros
# which generate a library or a executable file.
#
#=============================================================================#

macro(ADD_BIICODE_TARGETS)
	set(BII_BLOCK_TARGETS)
	SET(vname "${BII_BLOCK_USER}_${BII_BLOCK_NAME}") 
	message(AUTHOR_WARNING "TARGET CREATION: ${vname}")
	if(BII_${vname}_LIB)
		message("BII_GENERATE_LIB(${BII_BLOCK_USER} ${BII_BLOCK_NAME})")
		BII_GENERATE_LIB(${BII_BLOCK_USER} ${BII_BLOCK_NAME} )
		set(BII_BLOCK_TARGETS ${BII_BLOCK_TARGETS} "${vname}")
		set(BII_LIB_TARGET "${vname}")
		message( "Created lib: ${vname}")
	endif()
	foreach(executable ${BII_${vname}_EXES} )
		BII_GENERATE_EXECUTABLE( ${BII_BLOCK_USER} ${BII_BLOCK_NAME} ${executable})
		message(AUTHOR_WARNING "created executable: ${BII_BLOCK_USER}_${BII_BLOCK_NAME}_${executable}")
		SET(BII_BLOCK_TARGETS ${BII_BLOCK_TARGETS} "${BII_BLOCK_USER}_${BII_BLOCK_NAME}_${executable}")
		set(BII_${executable}_TARGET "${BII_BLOCK_USER}_${BII_BLOCK_NAME}_${executable}")
	endforeach ()
	SET(BII_${vname}_FINISHED TRUE CACHE INTERNAL "biicode")
	message(AUTHOR_WARNING "BII_${vname}_FINISHED created")
endmacro()


#=============================================================================#
# [PUBLIC/USER] [USER BLOCK CMAKELISTS]
#
# BII_ADD_INTERFACE(user block)
#
#        user    - User name
#        block   - block folder name 
#
# Used by user to have access to an interface lib
#
#=============================================================================#

function(BII_ADD_INTERFACE user block)
	message("interface_${user}_${block}_lib accesible from biicode.configure\n")
endfunction() 


#=============================================================================#
# [PUBLIC/USER] [USER BLOCK CMAKELISTS]
#
# BII_CONFIGURE_FILE(config_file_in config_file_out)
#
#        config_file_in    - Existing configure file name to charge
#		 config_file_out   - Output file where will be copied all the necessary
#
# Avoids errors due to the layout of configure. IT SHOULD be used instead of
# configure_file.
#
#=============================================================================#

macro (BII_CONFIGURE_FILE config_file_in config_file_out)
	configure_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/${config_file_in}"
	"${CMAKE_CURRENT_BINARY_DIR}/${config_file_out}"
	)
	SET(vname "${BII_BLOCK_USER}_${BII_BLOCK_NAME}")
	SET(BII_${vname}_INCLUDE_BIN TRUE CACHE INTERNAL "biicode")
endmacro()


#=============================================================================#
# [PUBLIC/USER] [USER BLOCK CMAKELISTS]
#
# BII_FILTER_LIB_SRC(ACCEPTABLE_SOURCES)
#
#        ACCEPTABLE_SOURCES    - List of sources to preserve
#
# Removes from biicode SRC list of calculated sources if not present in
# ACCEPTABLE_SOURCES.
#
#=============================================================================#

macro (BII_FILTER_LIB_SRC ACCEPTABLE_SOURCES)
	set(FILES_TO_REMOVE )
	foreach(_cell ${BII_LIB_SRC})
	  list(FIND ${ACCEPTABLE_SOURCES} ${_cell} contains)
	  if(contains EQUAL  -1)
		list(APPEND FILES_TO_REMOVE ${_cell})
	  endif(contains) 
	endforeach()

	IF(FILES_TO_REMOVE)
		list(REMOVE_ITEM BII_LIB_SRC ${FILES_TO_REMOVE})
	ENDIF()
endmacro()


#=============================================================================#
# [PUBLIC/USER] [MAIN BIICODE CMAKELISTS]
#
# BII_INCLUDE_BLOCK(BLOCK_DIR)
#
#        BLOCK_DIR    - Relative path to block, f.e.: blocks/myuser/simple_block
#
#
# Used by the root CMakeLists.txt.
#
# Initialize the necessary user block variables and validates the specific
# CMakeLists.txt.
#
# If this last one doesn't exist, biicode creates a default CMakLists.txt in
# that block
#
#=============================================================================#

macro(BII_INCLUDE_BLOCK BLOCK_DIR)
	get_filename_component(bii_hive_dir ${CMAKE_HOME_DIRECTORY} PATH)
	CREATE_BII_BLOCK_VARS( ${bii_hive_dir}/${BLOCK_DIR})

	# Deletes the oldest cached CMakeLists path 
	unset(_CMAKELISTPATH CACHE)

	# Copies the path to block CMakeLists file (if exists)
	file(TO_NATIVE_PATH "../${BLOCK_DIR}/CMakeLists.txt" _CMAKELISTPATH)

	if(NOT EXISTS ${_CMAKELISTPATH})
		message(ERROR, "ERROR, MISSING CMAKELISTS at: ${_CMAKELISTPATH}")
	else()
		ADD_SUBDIRECTORY("../${BLOCK_DIR}" "../build/${BII_BLOCK_USER}/${BII_BLOCK_NAME}")
		VALIDATE_BLOCK_CMAKELIST("../${BLOCK_DIR}")
	endif()
endmacro()


#=============================================================================#
# [PUBLIC/USER] [MAIN BIICODE CMAKELISTS]
#
# BII_PREBUILD_STEP(path)
#
#        path    - Relative path to block, f.e. : blocks/myuser/simple_block
#
# Called by the biicode main CMakeLists.txt, processes the biicode.configure 
# file if exists. Does nothing other case.
#
#=============================================================================#

function(BII_PREBUILD_STEP path)
	if(EXISTS "${CMAKE_HOME_DIRECTORY}/../${path}/biicode.configure")
		BII_CREATE_INTERFACE_LIBS("${CMAKE_HOME_DIRECTORY}/../${path}/biicode.configure")
		include("${CMAKE_HOME_DIRECTORY}/../${path}/biicode.configure")
	endif()
endfunction()



#=============================================================================#
#
#						PRIVATE FUNCTIONS AND MACROS
#
#=============================================================================#


#=============================================================================#
# [PRIVATE/INTERNAL]
#
# BII_CREATE_INTERFACE_LIBS(conf_file)
#
#        conf_file    - Configuration file name
#
# Used by the biicode main CMakeLists: parses the configuration file and creates
# the interfaces for the referenced targets.
#
# The targets for interface libs are "interface_[USER]_[BLOCK]_lib"
#=============================================================================#

function(BII_CREATE_INTERFACE_LIBS conf_file)
	file(READ ${conf_file} config_txt)
	string(REGEX MATCHALL "BII_ADD_INTERFACE\\(([a-z0-9_]+) ([a-z0-9_]+)\\)" match_list ${config_txt})
	FOREACH(match IN LISTS match_list)
		string(REGEX REPLACE "BII_ADD_INTERFACE\\(([a-z0-9_]+) ([a-z0-9_]+)\\)" "\\1" user ${match})
		string(REGEX REPLACE "BII_ADD_INTERFACE\\(([a-z0-9_]+) ([a-z0-9_]+)\\)" "\\2" block ${match})
		BII_ADD_INTERFACE_LIB(${user} ${block})
		MESSAGE("\nPROCESSING: ${match}: user=${user} block=${block} ")
	ENDFOREACH() 
endfunction()



#=============================================================================#
# [PRIVATE/INTERNAL]
#
# BII_ADD_INTERFACE_LIB(USER BLOCK)
#
#        USER    - User name
#        BLOCK   - block folder name 
#
# Creates the interface lib for a block if it does not exist 
#
#=============================================================================#

macro(BII_ADD_INTERFACE_LIB USER BLOCK)
	if(NOT TARGET interface_${USER}_${BLOCK}_lib)
		get_filename_component(parent_dir ${CMAKE_HOME_DIRECTORY} PATH)
		SET(DUMMY_DIR "${parent_dir}/build/${USER}/${BLOCK}/.interface")
		SET(DUMMY_BIN_DIR "${parent_dir}/build/${USER}/${BLOCK}/.interface")
		SET(DUMMY_SRC "${DUMMY_DIR}/cmake_dummy.cpp")
		IF (NOT EXISTS ${DUMMY_SRC})
		  FILE (WRITE  ${DUMMY_SRC} "//a dummy file for several lib interfaces CMake 2.8")
		ENDIF()  
		add_library(interface_${USER}_${BLOCK}_lib ${DUMMY_SRC})
	endif()
endmacro()


#=============================================================================#
# [PRIVATE/INTERNAL]
#
# BII_SET_OPTION(name)
#
#        name    - Option name to save
#
# Saves in cache the passed option names 
#
#=============================================================================#

function (BII_SET_OPTION name)
	set(${name} ON CACHE BOOL "biicode" FORCE)
endfunction()

#=============================================================================#
# [PRIVATE/INTERNAL]
#
# BII_UNSET_OPTION(name)
#
#        name    - Option name to delete from cache
#
# Deletes from cache the passed option names 
#
#=============================================================================#


function (BII_UNSET_OPTION name)
	set(${name} OFF CACHE BOOL "biicode" FORCE)
endfunction()


#=============================================================================#
# [PRIVATE/INTERNAL]
#
# CREATE_BII_BLOCK_VARS(BII_CURRENT_DIR)
#
#
#        BII_CURRENT_DIR    - Relative path to the root CMakeLists.txt the block
#                             folder, f.e.: ../blocks/myuser/myblock
#
# Gets the user block and prefix from path.
#
#=============================================================================#

macro (CREATE_BII_BLOCK_VARS BII_CURRENT_DIR)
	unset(vname CACHE)
	unset(BII_BLOCK_NAME CACHE)
	unset(BII_BLOCK_USER CACHE)
	unset(BLOCK_DIR CACHE)
	get_filename_component(bii_user_dir ${BII_CURRENT_DIR} PATH)
	get_filename_component(bii_base_dir ${bii_user_dir} PATH)
	get_filename_component(bii_hive_dir ${CMAKE_HOME_DIRECTORY} PATH)
	get_filename_component(BII_BLOCK_NAME ${BII_CURRENT_DIR} NAME)
	get_filename_component(BII_BLOCK_USER ${bii_user_dir} NAME)
	get_filename_component(BII_BLOCK_PREFIX ${bii_base_dir} NAME)
    SET(BII_IMPLICIT_RULES_ENABLED True)
endmacro ()


#=============================================================================#
# [PRIVATE/INTERNAL]
#
# VALIDATE_BLOCK_CMAKELIST(BII_DIR)
#
#        BII_DIR    - Relative path to the root CMakeLists.txt the block folder,
#					  f.e.: ../blocks/myuser/myblock
#
# Validates the possible CMakeLists.txt from user block folder.
#
#=============================================================================#

function(VALIDATE_BLOCK_CMAKELIST BII_DIR)
	CREATE_BII_BLOCK_VARS( ${BII_DIR})
	SET(vname "${BII_BLOCK_USER}_${BII_BLOCK_NAME}")
	message(AUTHOR_WARNING "check: BII_${vname}_INITIATED: ${BII_${vname}_INITIATED}\n")
	message(AUTHOR_WARNING "check: BII_${vname}_FINISHED: ${BII_${vname}_FINISHED}\n")

	# Checking INIT_BIICODE_BLOCK() is saved in the CMakLists.txt
	if(NOT BII_${vname}_INITIATED)
		message(SEND_ERROR "INIT_BIICODE_BLOCK() not found into the Cmakelist of ${vname}.
	****************************************************************************
	The root Cmakelist of a block must include INIT_BIICODE_BLOCK & 
	ADD_BIICODE_TARGETS. Please check the documentation about 
	CMakelist.txt at www.biicode.com
	****************************************************************************
	") 
	endif()

	# Checking ADD_BIICODE_TARGETS() is saved in the CMakLists.txt
	if(NOT BII_${vname}_FINISHED)
		message(SEND_ERROR "ADD_BIICODE_TARGETS() not found in the Cmakelist of ${vname}.\n
		 Please, add that line to your CMakelist") 
	endif()
endfunction()


#=============================================================================#
# [PRIVATE/INTERNAL]
#
# BII_INCLUDE_DATA_FILES(VNAME)
#
#        VNAME    - Base target name, f.e.: myuser_myblock
#
# Checks data files exist. If exist, they're copied to bin folder
#
# It maintains the same folder structure than the original ones.
#
#=============================================================================#

macro(BII_INCLUDE_DATA_FILES VNAME)
	if (BII_${VNAME}_DATA_FILES)
		foreach(datafile  ${BII_${VNAME}_DATA_FILES} )
			SET(SOURCE_DATA_FILE_DIR_BLOCKS ${CMAKE_HOME_DIRECTORY}/../blocks/${datafile})
			SET(SOURCE_DATA_FILE_DIR_DEPS ${CMAKE_HOME_DIRECTORY}/../deps/${datafile})
			if (EXISTS ${SOURCE_DATA_FILE_DIR_BLOCKS})
				SET(SOURCE_DATA_FILE_DIR ${SOURCE_DATA_FILE_DIR_BLOCKS})
			elseif(EXISTS ${SOURCE_DATA_FILE_DIR_DEPS})
				SET(SOURCE_DATA_FILE_DIR ${SOURCE_DATA_FILE_DIR_DEPS})
			else()
				message(FATAL_ERROR "ERROR: Data files don't exist")
			endif()
			SET(AUX_DATA_PATH )

			#Get data file name and get (if exists) the relative path to each one
			get_filename_component(DATA_NAME ${datafile} NAME)
			string(REPLACE ${DATA_NAME} "" AUX_DATA_PATH ${datafile})

			#Set destination folder to save a copy of data files
			set(DESTINATION_DATA_FILE_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${AUX_DATA_PATH}")
			file(COPY ${SOURCE_DATA_FILE_DIR} DESTINATION ${DESTINATION_DATA_FILE_DIR})
		endforeach()
	endif()
endmacro()


#=============================================================================#
# [PRIVATE/INTERNAL]
#
# BII_GENERATE_EXECUTABLE(USER BLOCK FNAME)
#
#        USER    - User name
#        BLOCK   - Block folder name
#        FNAME   - Main file name 
#
# Creates the binary  target name. It's in charge of setting the necessary
# properties to the target and linking with the libraries that target depends on.
#
# It can create the C/C++ and Arduino binary target files.
#
#=============================================================================#

function(BII_GENERATE_EXECUTABLE USER BLOCK FNAME)
	SET(vname "${USER}_${BLOCK}_${FNAME}") 
	SET(aux_src ${BII_${FNAME}_SRC})
	BII_INCLUDE_DATA_FILES(${vname})

    ADD_EXECUTABLE( ${vname} ${aux_src})
    if(BII_${USER}_${BLOCK}_INCLUDE_BIN)
        target_include_directories( ${vname} PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
    endif()

    if(BII_${vname}_BLOCK_DEPS)
        TARGET_LINK_LIBRARIES( ${vname} ${BII_${vname}_BLOCK_DEPS})
        add_dependencies(${vname} ${BII_${vname}_BLOCK_DEPS})
    endif(BII_${vname}_BLOCK_DEPS)

    if(BII_${vname}_SYSTEM_DEPS)
        HANDLE_SYSTEM_DEPS(${vname} "BII_${vname}_SYSTEM_DEPS")
    endif(BII_${vname}_SYSTEM_DEPS)


endfunction() 


#=============================================================================#
# [PRIVATE/INTERNAL]
#
# BII_GENERATE_LIB(USER BLOCK)
#
#        USER    - User name
#        BLOCK   - Block folder name 
#
# Creates the library with the biicode target name. It's in charge of setting 
# the necessary properties to the target and linking with other libraries that
# target depends on.
#
# It can create the C/C++ and Arduino library target files. The libraries'll
# be STATIC by default.
#
#=============================================================================#

function(BII_GENERATE_LIB USER BLOCK)
	SET(vname "${USER}_${BLOCK}") 
	get_filename_component(parent_dir ${CMAKE_HOME_DIRECTORY} PATH)
	SET(DUMMY_DIR "${parent_dir}/build/${USER}/${BLOCK}")
	SET(DUMMY_SRC "${DUMMY_DIR}/cmake_dummy.cpp")
	MESSAGE(STATUS "Checking default cmake_dummy.cpp for building librarie")
	IF (NOT EXISTS ${DUMMY_SRC})
		MESSAGE(STATUS "Writing default cmake_dummy.cpp for building librarie")
		FILE (WRITE  ${DUMMY_SRC} "//a dummy file for building header only libs with CMake 2.8")
	ENDIF()
	SET(aux_src ${BII_LIB_SRC} ${DUMMY_SRC})
	BII_INCLUDE_DATA_FILES(${vname})

    if(${BII_${vname}_INTERFACE_LIB})
       BII_ADD_INTERFACE_LIB(${USER} ${BLOCK})
       set(aux_deps ${aux_deps} interface_${vname}_lib)
    else()
       add_library(${vname} STATIC ${aux_src})
    endif()

    if(BII_${USER}_${BLOCK}_INCLUDE_BIN)
        if(TARGET ${vname})
            target_include_directories( ${vname} PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
        endif()
    endif()
    set(aux_deps ${BII_${vname}_BLOCK_DEPS})
    if(TARGET interface_${vname}_lib)
        set(aux_deps ${aux_deps} interface_${vname}_lib)
    endif()
    if(aux_deps)
        if(TARGET ${vname})
            foreach( aux_dep ${aux_deps})
                TARGET_LINK_LIBRARIES( ${vname} PUBLIC ${aux_dep})
            endforeach()
            add_dependencies(${vname} ${aux_deps})
        endif()
        #message("TARGET_LINK_LIBRARIES( ${vname} ${aux_deps})")
    endif()
    if(BII_${vname}_SYSTEM_DEPS)
        HANDLE_SYSTEM_DEPS(${vname} "BII_${vname}_SYSTEM_DEPS")
    endif(BII_${vname}_SYSTEM_DEPS)

endfunction()


#=============================================================================#
# [PRIVATE/INTERNAL]
#
# DISABLE_BII_IMPLICIT_RULES()
#
#
# Disables the BII_IMPLICIT_RULES_ENABLED (True by default) to link all our targets 
#
#=============================================================================#

macro(DISABLE_BII_IMPLICIT_RULES)
	unset(BII_IMPLICIT_RULES_ENABLED)
endmacro()


#=============================================================================#
# [PRIVATE/INTERNAL]
#
# HANDLE_SYSTEM_DEPS(target_name sys_deps)
#
#        target_name    - Complete target name, f.e.: myuser_myblock_main
#        sys_deps       - System dependencies detected by biicode 
#
# Links the passed targets with the selected system dependencies
#
#=============================================================================#

function(HANDLE_SYSTEM_DEPS target_name sys_deps)
  if(${BII_IMPLICIT_RULES_ENABLED})
  foreach( sys_dep ${${sys_deps}})
    if(sys_dep STREQUAL "m" AND UNIX)
      target_link_libraries(${target_name} PUBLIC ${sys_dep})
    endif()

    if(sys_dep STREQUAL "pthread")
      if(UNIX)
        target_link_libraries(${target_name} PUBLIC ${sys_dep})
      endif()
    endif()

    if(sys_dep STREQUAL "GL")
      if(APPLE)
        FIND_LIBRARY(OpenGL_LIBRARY OpenGL)
        target_link_libraries(${target_name} PUBLIC ${OpenGL_LIBRARY})
      elseif(UNIX)
        target_link_libraries(${target_name} PUBLIC ${sys_dep})
      elseif(WIN32)
        target_link_libraries(${target_name} PUBLIC "opengl32")
        endif()
    endif()

    if(sys_dep STREQUAL "GLU")
      if(UNIX)
        target_link_libraries(${target_name} PUBLIC ${sys_dep})
      endif()
      if(WIN32)
        target_link_libraries(${target_name} PUBLIC "glu32")
      endif()
    endif()

    if(sys_dep STREQUAL "ws2_32")
      if(WIN32)
        target_link_libraries(${target_name} PUBLIC ${sys_dep})
      endif()
    endif()

    if(sys_dep STREQUAL "winmm")
      if(WIN32)
        target_link_libraries(${target_name} PUBLIC ${sys_dep})
      endif()
    endif()
  endforeach( sys_dep ${sys_deps})
endif()

endfunction()


