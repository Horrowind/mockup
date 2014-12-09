

#find_package(LLVM 3.6 REQUIRED CONFIG)

add_library(r65816
  src/rom.c
  src/algorithms.c
  src/cpu.c
  src/disassembler.c
  src/memory.c
  src/table.c
)

target_include_directories(r65816 PUBLIC include/r65816)
