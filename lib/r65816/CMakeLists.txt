

#find_package(LLVM 3.6 REQUIRED CONFIG)

set(BUILD_OLD TRUE)
if(NOT BUILD_OLD) 
  add_library(r65816_obj OBJECT
    src/table.c
    src/breakpoint.c
    src/cpu.c
    src/cpu_io.c
    src/disassembler.c
    src/memory.c
    src/opcode_misc.c
    src/opcode_pc.c
    src/opcode_read.c
    src/opcode_rmw.c
    src/opcode_write.c
    src/rom.c
    src/algorithms.c
    )

  add_library(r65816 $<TARGET_OBJECTS:r65816_obj>)
else()
  add_library(r65816_obj OBJECT
    src/rom.c
    src/breakpoint.c
    src2/cpu.cpp
    src2/cpu_old.cpp
    )
  add_custom_command(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/src2/bsnes/obj/processor-r65816.o
    COMMAND make obj/processor-r65816.o
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src2/bsnes
    )
  add_custom_target(
    bsnes-r65816
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src2/bsnes/obj/processor-r65816.o
    )
  target_include_directories(r65816_obj PUBLIC src2/bsnes)
  set_property(TARGET bsnes-r65816 PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/bsnes/obj/processor-r65816.o)
  add_dependencies(r65816_obj bsnes-r65816)
  add_library(r65816 $<TARGET_OBJECTS:r65816_obj>)
  target_link_libraries(r65816 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src2/bsnes/obj/processor-r65816.o)
endif()


target_include_directories(r65816_obj PUBLIC include/r65816)
find_path(R65816_INCLUDE_DIR r65816/cpu.h HINTS ${CMAKE_CURRENT_SOURCE_DIR}/include PATH_SUFFIXES include)
