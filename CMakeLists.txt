#set(CMAKE_VERBOSE_MAKEFILE ON)
cmake_minimum_required (VERSION 2.8.9)
include(ExternalProject)

project(mockup)
enable_testing()

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

#SET ( CMAKE_C_COMPILER  "/usr/bin/clang" )
SET ( CMAKE_C_FLAGS       "-std=c99 -Wno-overflow" )
#SET ( CMAKE_CXX_COMPILER  "/usr/bin/clang++" )
SET ( CMAKE_CXX_FLAGS     "-std=c++11" )

add_custom_target(run
    COMMAND app/mockup
    DEPENDS mockup
    WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
)

add_subdirectory(lib)
add_subdirectory(app)
add_subdirectory(test)
