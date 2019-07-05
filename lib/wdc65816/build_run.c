#define DEBUG_PRINT_CPU_STATE

#include "algorithms.c"
#include "disassembler.c"
#include "memory.c"
#include "opcode_misc.c"
#include "opcode_pc.c"
#include "opcode_read.c"
#include "opcode_rmw.c"
#include "opcode_write.c"
#if 1
#include "run_table.c"
#if defined(NO_COMPUTED_GOTO)
#include "step_switch.c"
#else
#include "step.c"
#endif
#else
//need work
#include "run.c"
#include "run_jsl.c"
#include "run_jsr.c"
#endif
