# CMake version control ------------------------------------------------------------------
cmake_minimum_required(VERSION 2.8)

# Debug ----------------------------------------------------------------------------------
set(CMAKE_BUILD_TYPE Debug)

# Core Project declaration ---------------------------------------------------------------
project(LandySzalayEstimator)
set(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMakeModules/")

FIND_PACKAGE (OPENCL REQUIRED)
FIND_PACKAGE (TCMALLOC REQUIRED)
FIND_PACKAGE (Eigen3 REQUIRED)

if (EIGEN3_FOUND)
  message("-- Found Eigen3")
else ()
  message(FATAL_ERROR "-- Unable to find Eigen3")
endif()

message("-- Found OpenCL : " ${OPENCL_FOUND} "(" "${OPENCL_INCLUDE_DIRS}" ")")

ADD_DEFINITIONS(-DCL_SOURCE_DIR="${CMAKE_CURRENT_LIST_DIR}/kernels")

include_directories(
  "${OPENCL_INCLUDE_DIRS}"
  "${EIGEN3_INCLUDE_DIR}"
  )

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
