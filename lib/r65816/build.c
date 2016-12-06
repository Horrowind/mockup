//#define DEBUG_PRINT_CPU_STATE

#include "algorithms.c"
#include "breakpoint.c"
#include "cpu.c"
#include "cpu_io.c"
#include "disassembler.c"
#include "memory.c"
#include "opcode_misc.c"
#include "opcode_pc.c"
#include "opcode_read.c"
#include "opcode_rmw.c"
#include "opcode_write.c"
#include "rom.c"

#if 1
#include "run_table.c"
#include "step.c"
#else //need work
#include "run.c"
#include "run_jsl.c"
#include "run_jsr.c"
#endif
