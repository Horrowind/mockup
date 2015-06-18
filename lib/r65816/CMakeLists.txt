#find_package(LLVM 3.6)

add_library(r65816_obj OBJECT
  src/run.c
  src/run_jsl.c
  src/run_jsr.c
  src/step.c
  src/breakpoint.c
  src/cpu.c
  src/cpu_io.c
  src/disassembler.c
  src/memory.c
  src/opcode_misc.c
  src/opcode_read.c
  src/opcode_pc.c
  src/opcode_rmw.c
  src/opcode_write.c
  src/rom.c
  src/algorithms.c
  )
add_library(r65816 $<TARGET_OBJECTS:r65816_obj>)
#target_include_directories(r65816_obj PUBLIC ${LLVM_INCLUDE_DIRS})
target_include_directories(r65816_obj PUBLIC include/r65816)
find_path(R65816_INCLUDE_DIR r65816/cpu.h HINTS ${CMAKE_CURRENT_SOURCE_DIR}/include PATH_SUFFIXES include)
