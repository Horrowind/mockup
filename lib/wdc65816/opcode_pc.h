#ifndef WDC65816_CPU_OPCODE_PC
#define WDC65816_CPU_OPCODE_PC

#include "cpu.h"

void wdc65816_op_branch_bpl(Wdc65816Cpu* cpu);
void wdc65816_op_branch_bmi(Wdc65816Cpu* cpu);
void wdc65816_op_branch_bvc(Wdc65816Cpu* cpu);
void wdc65816_op_branch_bvs(Wdc65816Cpu* cpu);
void wdc65816_op_branch_bcc(Wdc65816Cpu* cpu);
void wdc65816_op_branch_bcs(Wdc65816Cpu* cpu);
void wdc65816_op_branch_bne(Wdc65816Cpu* cpu);
void wdc65816_op_branch_beq(Wdc65816Cpu* cpu);
void wdc65816_op_bra(Wdc65816Cpu* cpu);
void wdc65816_op_brl(Wdc65816Cpu* cpu);
void wdc65816_op_jmp_addr(Wdc65816Cpu* cpu);
void wdc65816_op_jmp_long(Wdc65816Cpu* cpu);
void wdc65816_op_jmp_iaddr(Wdc65816Cpu* cpu);
void wdc65816_op_jmp_iaddrx(Wdc65816Cpu* cpu);
void wdc65816_op_jmp_iladdr(Wdc65816Cpu* cpu);
void wdc65816_op_jsr_addr(Wdc65816Cpu* cpu);
void wdc65816_op_jsr_long_e(Wdc65816Cpu* cpu);
void wdc65816_op_jsr_long_n(Wdc65816Cpu* cpu);
void wdc65816_op_jsr_iaddrx_e(Wdc65816Cpu* cpu);
void wdc65816_op_jsr_iaddrx_n(Wdc65816Cpu* cpu);
void wdc65816_op_rti_e(Wdc65816Cpu* cpu);
void wdc65816_op_rti_n(Wdc65816Cpu* cpu);
void wdc65816_op_rts(Wdc65816Cpu* cpu);
void wdc65816_op_rtl_e(Wdc65816Cpu* cpu);
void wdc65816_op_rtl_n(Wdc65816Cpu* cpu);

#endif //WDC65816_CPU_OPCODE_PC
