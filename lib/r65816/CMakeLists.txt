set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_library(r65816
  rom.c
  algorithms.c
  cpu.c
  disassembler.c
  memory.c
  table.c
)
