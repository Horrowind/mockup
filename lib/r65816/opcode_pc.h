#ifndef MOCKUP_CPU_OPCODE_PC
#define MOCKUP_CPU_OPCODE_PC

#include "cpu.h"

void r65816_op_branch_bpl(r65816_cpu_t* cpu);
void r65816_op_branch_bmi(r65816_cpu_t* cpu);
void r65816_op_branch_bvc(r65816_cpu_t* cpu);
void r65816_op_branch_bvs(r65816_cpu_t* cpu);
void r65816_op_branch_bcc(r65816_cpu_t* cpu);
void r65816_op_branch_bcs(r65816_cpu_t* cpu);
void r65816_op_branch_bne(r65816_cpu_t* cpu);
void r65816_op_branch_beq(r65816_cpu_t* cpu);
void r65816_op_bra(r65816_cpu_t* cpu);
void r65816_op_brl(r65816_cpu_t* cpu);
void r65816_op_jmp_addr(r65816_cpu_t* cpu);
void r65816_op_jmp_long(r65816_cpu_t* cpu);
void r65816_op_jmp_iaddr(r65816_cpu_t* cpu);
void r65816_op_jmp_iaddrx(r65816_cpu_t* cpu);
void r65816_op_jmp_iladdr(r65816_cpu_t* cpu);
void r65816_op_jsr_addr(r65816_cpu_t* cpu);
void r65816_op_jsr_long_e(r65816_cpu_t* cpu);
void r65816_op_jsr_long_n(r65816_cpu_t* cpu);
void r65816_op_jsr_iaddrx_e(r65816_cpu_t* cpu);
void r65816_op_jsr_iaddrx_n(r65816_cpu_t* cpu);
void r65816_op_rti_e(r65816_cpu_t* cpu);
void r65816_op_rti_n(r65816_cpu_t* cpu);
void r65816_op_rts(r65816_cpu_t* cpu);
void r65816_op_rtl_e(r65816_cpu_t* cpu);
void r65816_op_rtl_n(r65816_cpu_t* cpu);

#endif //MOCKUP_CPU_OPCODE_PC
