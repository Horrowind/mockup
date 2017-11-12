#include "opcode_misc.c"
#include "opcode_pc.c"
#include "opcode_read.c"
#include "opcode_rmw.c"
#include "opcode_write.c"
#if 1
#include "run_table.c"
#ifndef NO_COMPUTED_GOTO
#include "step.c"
#else
#include "step_switch.c"
#endif
#else //need work
#include "run.c"
#include "run_jsl.c"
#include "run_jsr.c"
#endif
