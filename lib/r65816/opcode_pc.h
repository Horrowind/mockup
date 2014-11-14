
#ifndef R65816_OPCODE_PC_H
#define R65816_OPCODE_PC_H

#include "cpu.h"

void op_branch_bpl(cpu_t* cpu);
void op_branch_bmi(cpu_t* cpu);
void op_branch_bvc(cpu_t* cpu);
void op_branch_bvs(cpu_t* cpu);
void op_branch_bcc(cpu_t* cpu);
void op_branch_bcs(cpu_t* cpu);
void op_branch_bne(cpu_t* cpu);
void op_branch_beq(cpu_t* cpu);

void op_bra(cpu_t* cpu);
void op_brl(cpu_t* cpu);
void op_jmp_addr(cpu_t* cpu);
void op_jmp_long(cpu_t* cpu);
void op_jmp_iaddr(cpu_t* cpu);
void op_jmp_iaddrx(cpu_t* cpu);
void op_jmp_iladdr(cpu_t* cpu);
void op_jsr_addr(cpu_t* cpu);
void op_jsr_long_e(cpu_t* cpu);
void op_jsr_long_n(cpu_t* cpu);
void op_jsr_iaddrx_e(cpu_t* cpu);
void op_jsr_iaddrx_n(cpu_t* cpu);
void op_rti_e(cpu_t* cpu);
void op_rti_n(cpu_t* cpu);
void op_rts(cpu_t* cpu);
void op_rtl_e(cpu_t* cpu);
void op_rtl_n(cpu_t* cpu);

#endif //R65816_OPCODE_PC_H
