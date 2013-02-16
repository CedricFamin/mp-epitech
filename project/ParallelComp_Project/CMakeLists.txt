# CMake version control ------------------------------------------------------------------
cmake_minimum_required(VERSION 2.8)

# Debug ----------------------------------------------------------------------------------
set(CMAKE_BUILD_TYPE Debug)

# Core Project declaration ---------------------------------------------------------------
project(LandySzalayEstimator)
set(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})

# TBB requirements -----------------------------------------------------------------------
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMakeModules/")
#find_package(TBB REQUIRED)
#include_directories(${TBB_INCLUDE_DIRS})
#link_directories(${TBB_LIBRARY_DIRS})


# OPENCL requirement
FIND_PACKAGE (OPENCL REQUIRED)

# Tcmalloc requirements -----------------------------------------------------------------------
find_package(TCMALLOC REQUIRED)
include_directories(${Tcmalloc_DIRS})
link_directories(${Tcmalloc_LIBRARIES})

# EIGEN REQUIREMENTS ---------------------------------------------------------------------
find_package(Eigen3 REQUIRED)
if (EIGEN3_FOUND)
  message("-- Found Eigen3")
else ()
  message(FATAL_ERROR "-- Unable to find Eigen3")
endif()
include_directories(${EIGEN3_INCLUDE_DIR})

# C++11 ----------------------------------------------------------------------------------
if(CMAKE_COMPILER_IS_GNUCXX)
    add_definitions(-std=gnu++0x)
endif()

# Set core files list --------------------------------------------------------------------
file(
	GLOB
	core_source_files
	sources/*
	GLOB
	core_include_files
	includes/*
)

# add executable for the core BLINK system
add_executable(
	LandySzalayEstimator
	${core_include_files}
	${core_source_files}
)

TARGET_LINK_LIBRARIES (
   LandySzalayEstimator
   ${Tcmalloc_LIBRARIES}
   ${OPENCL_LIBRARIES}
)
