#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=clang
CCC=clang++
CXX=clang++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=LLVM/Clang-Linux-x86
CND_DLIB_EXT=so
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/blocks/manu343726/turbo_main/main.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-stdlib=libc++ -lc++abi -ftemplate-backtrace-limit=0
CXXFLAGS=-stdlib=libc++ -lc++abi -ftemplate-backtrace-limit=0

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/turbo

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/turbo: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/turbo ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/blocks/manu343726/turbo_main/main.o: blocks/manu343726/turbo_main/main.cpp 
	${MKDIR} -p ${OBJECTDIR}/blocks/manu343726/turbo_main
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Iblocks -Iblocks/manu343726 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/blocks/manu343726/turbo_main/main.o blocks/manu343726/turbo_main/main.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/turbo

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
