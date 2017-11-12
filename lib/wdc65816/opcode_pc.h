#ifndef MOCKUP_CPU_OPCODE_PC
#define MOCKUP_CPU_OPCODE_PC

#include "cpu.h"

void wdc65816_op_branch_bpl(wdc65816_cpu_t* cpu);
void wdc65816_op_branch_bmi(wdc65816_cpu_t* cpu);
void wdc65816_op_branch_bvc(wdc65816_cpu_t* cpu);
void wdc65816_op_branch_bvs(wdc65816_cpu_t* cpu);
void wdc65816_op_branch_bcc(wdc65816_cpu_t* cpu);
void wdc65816_op_branch_bcs(wdc65816_cpu_t* cpu);
void wdc65816_op_branch_bne(wdc65816_cpu_t* cpu);
void wdc65816_op_branch_beq(wdc65816_cpu_t* cpu);
void wdc65816_op_bra(wdc65816_cpu_t* cpu);
void wdc65816_op_brl(wdc65816_cpu_t* cpu);
void wdc65816_op_jmp_addr(wdc65816_cpu_t* cpu);
void wdc65816_op_jmp_long(wdc65816_cpu_t* cpu);
void wdc65816_op_jmp_iaddr(wdc65816_cpu_t* cpu);
void wdc65816_op_jmp_iaddrx(wdc65816_cpu_t* cpu);
void wdc65816_op_jmp_iladdr(wdc65816_cpu_t* cpu);
void wdc65816_op_jsr_addr(wdc65816_cpu_t* cpu);
void wdc65816_op_jsr_long_e(wdc65816_cpu_t* cpu);
void wdc65816_op_jsr_long_n(wdc65816_cpu_t* cpu);
void wdc65816_op_jsr_iaddrx_e(wdc65816_cpu_t* cpu);
void wdc65816_op_jsr_iaddrx_n(wdc65816_cpu_t* cpu);
void wdc65816_op_rti_e(wdc65816_cpu_t* cpu);
void wdc65816_op_rti_n(wdc65816_cpu_t* cpu);
void wdc65816_op_rts(wdc65816_cpu_t* cpu);
void wdc65816_op_rtl_e(wdc65816_cpu_t* cpu);
void wdc65816_op_rtl_n(wdc65816_cpu_t* cpu);

#endif //MOCKUP_CPU_OPCODE_PC
