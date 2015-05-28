#set(CMAKE_VERBOSE_MAKEFILE ON)
cmake_minimum_required (VERSION 2.8.9)
include(ExternalProject)

project(mockup)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

#SET ( CMAKE_C_COMPILER  "/usr/bin/clang" )
SET ( CMAKE_C_FLAGS       "-O3 -std=c99" )
#SET ( CMAKE_CXX_COMPILER  "/usr/bin/clang++" )
SET ( CMAKE_CXX_FLAGS     "-O3 -std=c++11" )

add_subdirectory(lib)
add_subdirectory(app)
#add_subdirectory(test)
