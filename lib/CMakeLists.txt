#add_custom_command(
#    OUTPUT ${CMAKE_SOURCE_DIR}/lib/bsnes/obj/processor-r65816.o
#    COMMAND make obj/processor-r65816.o
#    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/lib/bsnes
#)
 
#add_custom_target(
#    bsnes-r65816
#    DEPENDS ${CMAKE_SOURCE_DIR}/lib/bsnes/obj/processor-r65816.o
#)

add_subdirectory(mockup)
add_subdirectory(r65816)

add_dependencies(libmockup r65816)

