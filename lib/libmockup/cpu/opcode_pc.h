#ifndef MOCKUP_CPU_OPCODE_PC
#define MOCKUP_CPU_OPCODE_PC

#include "cpu.h"

inline void op_branch(cpu_t* cpu, int bit, int val);
inline void op_bra(cpu_t* cpu);
inline void op_brl(cpu_t* cpu);
inline void op_jmp_addr(cpu_t* cpu);
inline void op_jmp_long(cpu_t* cpu);
inline void op_jmp_iaddr(cpu_t* cpu);
inline void op_jmp_iaddrx(cpu_t* cpu);
inline void op_jmp_iladdr(cpu_t* cpu);
inline void op_jsr_addr(cpu_t* cpu);
inline void op_jsr_long_e(cpu_t* cpu);
inline void op_jsr_long_n(cpu_t* cpu);
inline void op_jsr_iaddrx_e(cpu_t* cpu);
inline void op_jsr_iaddrx_n(cpu_t* cpu);
inline void op_rti_e(cpu_t* cpu);
inline void op_rti_n(cpu_t* cpu);
inline void op_rts(cpu_t* cpu);
inline void op_rtl_e(cpu_t* cpu);
inline void op_rtl_n(cpu_t* cpu);

#endif //MOCKUP_CPU_OPCODE_PC
