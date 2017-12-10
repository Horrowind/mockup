#ifndef MOCKUP_CPU_OPCODE_PC
#define MOCKUP_CPU_OPCODE_PC

#include "cpu.h"

void wdc65816_op_branch_bpl(WDC65816Cpu* cpu);
void wdc65816_op_branch_bmi(WDC65816Cpu* cpu);
void wdc65816_op_branch_bvc(WDC65816Cpu* cpu);
void wdc65816_op_branch_bvs(WDC65816Cpu* cpu);
void wdc65816_op_branch_bcc(WDC65816Cpu* cpu);
void wdc65816_op_branch_bcs(WDC65816Cpu* cpu);
void wdc65816_op_branch_bne(WDC65816Cpu* cpu);
void wdc65816_op_branch_beq(WDC65816Cpu* cpu);
void wdc65816_op_bra(WDC65816Cpu* cpu);
void wdc65816_op_brl(WDC65816Cpu* cpu);
void wdc65816_op_jmp_addr(WDC65816Cpu* cpu);
void wdc65816_op_jmp_long(WDC65816Cpu* cpu);
void wdc65816_op_jmp_iaddr(WDC65816Cpu* cpu);
void wdc65816_op_jmp_iaddrx(WDC65816Cpu* cpu);
void wdc65816_op_jmp_iladdr(WDC65816Cpu* cpu);
void wdc65816_op_jsr_addr(WDC65816Cpu* cpu);
void wdc65816_op_jsr_long_e(WDC65816Cpu* cpu);
void wdc65816_op_jsr_long_n(WDC65816Cpu* cpu);
void wdc65816_op_jsr_iaddrx_e(WDC65816Cpu* cpu);
void wdc65816_op_jsr_iaddrx_n(WDC65816Cpu* cpu);
void wdc65816_op_rti_e(WDC65816Cpu* cpu);
void wdc65816_op_rti_n(WDC65816Cpu* cpu);
void wdc65816_op_rts(WDC65816Cpu* cpu);
void wdc65816_op_rtl_e(WDC65816Cpu* cpu);
void wdc65816_op_rtl_n(WDC65816Cpu* cpu);

#endif //MOCKUP_CPU_OPCODE_PC
