#set(CMAKE_VERBOSE_MAKEFILE ON)
cmake_minimum_required (VERSION 2.8.9)
include(ExternalProject)

project(mockup)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

set(make_profiler_build false)
set(use_profiled_build false)
set(use_clang false)

if(use_clang)
SET ( CMAKE_C_COMPILER  "/usr/bin/clang" )
SET ( CMAKE_CXX_COMPILER  "/usr/bin/clang++" )
endif()

if(make_profiler_build) 
SET ( CMAKE_C_FLAGS       "-g -O3 -std=c11 -fprofile-arcs -ftest-coverage")
SET(GCC_COVERAGE_COMPILE_FLAGS "-O3 -std=c11 -fprofile-arcs -ftest-coverage")
SET(GCC_COVERAGE_LINK_FLAGS    "-lgcov")
SET( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}" )
elseif(use_profiled_build)
SET ( CMAKE_C_FLAGS       "-O3 -std=c11 -fprofile-use" )
else()
SET ( CMAKE_C_FLAGS       "-O3 -std=c11" )
endif()
SET ( CMAKE_CXX_FLAGS     "-O3 -std=c++11" )

add_subdirectory(lib)
add_subdirectory(app)
