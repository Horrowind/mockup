#ifndef R65816_ALGORITHMS_H
#define R65816_ALGORITHMS_H

#include "cpu.h"

void wdc65816_op_adc_b(WDC65816Cpu* cpu);
void wdc65816_op_adc_w(WDC65816Cpu* cpu);
void wdc65816_op_and_b(WDC65816Cpu* cpu);
void wdc65816_op_and_w(WDC65816Cpu* cpu);
void wdc65816_op_bit_b(WDC65816Cpu* cpu);
void wdc65816_op_bit_w(WDC65816Cpu* cpu);
void wdc65816_op_cmp_b(WDC65816Cpu* cpu);
void wdc65816_op_cmp_w(WDC65816Cpu* cpu);
void wdc65816_op_cpx_b(WDC65816Cpu* cpu);
void wdc65816_op_cpx_w(WDC65816Cpu* cpu);
void wdc65816_op_cpy_b(WDC65816Cpu* cpu);
void wdc65816_op_cpy_w(WDC65816Cpu* cpu);
void wdc65816_op_eor_b(WDC65816Cpu* cpu);
void wdc65816_op_eor_w(WDC65816Cpu* cpu);
void wdc65816_op_lda_b(WDC65816Cpu* cpu);
void wdc65816_op_lda_w(WDC65816Cpu* cpu);
void wdc65816_op_ldx_b(WDC65816Cpu* cpu);
void wdc65816_op_ldx_w(WDC65816Cpu* cpu);
void wdc65816_op_ldy_b(WDC65816Cpu* cpu);
void wdc65816_op_ldy_w(WDC65816Cpu* cpu);
void wdc65816_op_ora_b(WDC65816Cpu* cpu);
void wdc65816_op_ora_w(WDC65816Cpu* cpu);
void wdc65816_op_sbc_b(WDC65816Cpu* cpu);
void wdc65816_op_sbc_w(WDC65816Cpu* cpu);
void wdc65816_op_inc_b(WDC65816Cpu* cpu);
void wdc65816_op_inc_w(WDC65816Cpu* cpu);
void wdc65816_op_dec_b(WDC65816Cpu* cpu);
void wdc65816_op_dec_w(WDC65816Cpu* cpu);
void wdc65816_op_asl_b(WDC65816Cpu* cpu);
void wdc65816_op_asl_w(WDC65816Cpu* cpu);
void wdc65816_op_lsr_b(WDC65816Cpu* cpu);
void wdc65816_op_lsr_w(WDC65816Cpu* cpu);
void wdc65816_op_rol_b(WDC65816Cpu* cpu);
void wdc65816_op_rol_w(WDC65816Cpu* cpu);
void wdc65816_op_ror_b(WDC65816Cpu* cpu);
void wdc65816_op_ror_w(WDC65816Cpu* cpu);
void wdc65816_op_trb_b(WDC65816Cpu* cpu);
void wdc65816_op_trb_w(WDC65816Cpu* cpu);
void wdc65816_op_tsb_b(WDC65816Cpu* cpu);
void wdc65816_op_tsb_w(WDC65816Cpu* cpu);

#endif //R65816_ALGORITHMS_H
