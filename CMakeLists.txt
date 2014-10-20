#set(CMAKE_VERBOSE_MAKEFILE ON)
cmake_minimum_required (VERSION 2.8.9)
include(ExternalProject)

project(mockup)

# TinyXML
#option(USE_SYSTEM_TINYXML "Use system TinyXML library instead of internal." OFF)
#if(USE_SYSTEM_TINYXML)
#    find_library(TINYXML_LIBRARIES tinyxml)
#    find_path(TINYXML_INCLUDE_DIR tinyxml.h)
#    message(STATUS "Found TinyXML: ${TINYXML_LIBRARIES} ${TINYXML_INCLUDE_DIR}")
#    add_definitions (-DTIXML_USE_STL)
#    if(TINYXML_LIBRARIES AND TINYXML_INCLUDE_DIR)
#        include_directories(${TINYXML_INCLUDE_DIR})
#        message(STATUS "Using system TinyXML library.")
#    else()
#        message(FATAL_ERROR "Detection of system TinyXML incomplete.")
#    endif()
#endif()


#SET ( CMAKE_CXX_COMPILER  "/usr/bin/clang++" )
SET ( CMAKE_CXX_FLAGS     "-std=c++11" )

#add_subdirectory(libsmw)

add_custom_command(
    OUTPUT ${PROJECT_SOURCE_DIR}/bsnes/obj/processor-r65816.o
    COMMAND make obj/processor-r65816.o
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/bsnes
)
 
add_custom_target(
    bsnes-r65816
    DEPENDS ${CMAKE_SOURCE_DIR}/bsnes/obj/processor-r65816.o
)

add_custom_target(run
    COMMAND mockup 0 --geometry -0+0
    DEPENDS mockup
    WORKING_DIRECTORY ${CMAKE_PROJECT_DIR/app}
)

#export(TARGETS bsnes-r65816 FILE bsnes-exports.cmake)

include_directories(world-lib)

add_subdirectory(app)
add_dependencies(mockup bsnes-r65816)
