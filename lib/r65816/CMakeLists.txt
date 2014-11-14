set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_library(r65816
  rom.c
  algorithms.c
  cpu.c
  disassembler.c
  memory.c
  opcode_misc.c
  opcode_pc.c
  opcode_read.c
  opcode_rmw.c
  opcode_write.c
  table.c
)
