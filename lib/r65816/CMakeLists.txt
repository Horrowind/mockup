

find_package(LLVM 3.6)

set(BUILD_NEW TRUE)
if(NOT LLVM_FOUND) 
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
    src_llvm/table.c
    src_llvm/breakpoint.c
    src_llvm/cpu.c
    src_llvm/cpu_io.c
    src_llvm/disassembler.c
    src_llvm/memory.c
    src_llvm/opcode_misc.c
    src_llvm/opcode_pc.c
    src_llvm/opcode_read.c
    src_llvm/opcode_rmw.c
    src_llvm/opcode_write.c
    src_llvm/rom.c
    src_llvm/algorithms.c
    )
  add_library(r65816 $<TARGET_OBJECTS:r65816_obj>)
  target_include_directories(r65816_obj PUBLIC ${LLVM_INCLUDE_DIRS})

  get_target_property(TEMP r65816_obj COMPILE_FLAGS)
  if(TEMP STREQUAL "TEMP-NOTFOUND")
    SET(TEMP "") # set to empty string
  else()
    SET(TEMP "${TEMP} ") # a space to cleanly separate from existing content
  endif()
  # append our values
  SET(TEMP "${TEMP} -D__STDC_CONSTANT_MACROS -D__STDC_LIMIT_MACROS" )
  set_target_properties(r65816_obj PROPERTIES COMPILE_FLAGS ${TEMP} )

endif()


target_include_directories(r65816_obj PUBLIC include/r65816)
find_path(R65816_INCLUDE_DIR r65816/cpu.h HINTS ${CMAKE_CURRENT_SOURCE_DIR}/include PATH_SUFFIXES include)
