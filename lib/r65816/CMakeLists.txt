set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(LLVM 3.6 REQUIRED CONFIG)

add_library(r65816
  rom.c
  algorithms.c
  cpu.c
  disassembler.c
  memory.c
  table.c
)
