#ifndef WDC65816_ALGORITHMS_H
#define WDC65816_ALGORITHMS_H

#include "cpu.h"

static void wdc65816_op_adc_b(Wdc65816Cpu* cpu);
static void wdc65816_op_adc_w(Wdc65816Cpu* cpu);
static void wdc65816_op_and_b(Wdc65816Cpu* cpu);
static void wdc65816_op_and_w(Wdc65816Cpu* cpu);
static void wdc65816_op_bit_b(Wdc65816Cpu* cpu);
static void wdc65816_op_bit_w(Wdc65816Cpu* cpu);
static void wdc65816_op_cmp_b(Wdc65816Cpu* cpu);
static void wdc65816_op_cmp_w(Wdc65816Cpu* cpu);
static void wdc65816_op_cpx_b(Wdc65816Cpu* cpu);
static void wdc65816_op_cpx_w(Wdc65816Cpu* cpu);
static void wdc65816_op_cpy_b(Wdc65816Cpu* cpu);
static void wdc65816_op_cpy_w(Wdc65816Cpu* cpu);
static void wdc65816_op_eor_b(Wdc65816Cpu* cpu);
static void wdc65816_op_eor_w(Wdc65816Cpu* cpu);
static void wdc65816_op_lda_b(Wdc65816Cpu* cpu);
static void wdc65816_op_lda_w(Wdc65816Cpu* cpu);
static void wdc65816_op_ldx_b(Wdc65816Cpu* cpu);
static void wdc65816_op_ldx_w(Wdc65816Cpu* cpu);
static void wdc65816_op_ldy_b(Wdc65816Cpu* cpu);
static void wdc65816_op_ldy_w(Wdc65816Cpu* cpu);
static void wdc65816_op_ora_b(Wdc65816Cpu* cpu);
static void wdc65816_op_ora_w(Wdc65816Cpu* cpu);
static void wdc65816_op_sbc_b(Wdc65816Cpu* cpu);
static void wdc65816_op_sbc_w(Wdc65816Cpu* cpu);
static void wdc65816_op_inc_b(Wdc65816Cpu* cpu);
static void wdc65816_op_inc_w(Wdc65816Cpu* cpu);
static void wdc65816_op_dec_b(Wdc65816Cpu* cpu);
static void wdc65816_op_dec_w(Wdc65816Cpu* cpu);
static void wdc65816_op_asl_b(Wdc65816Cpu* cpu);
static void wdc65816_op_asl_w(Wdc65816Cpu* cpu);
static void wdc65816_op_lsr_b(Wdc65816Cpu* cpu);
static void wdc65816_op_lsr_w(Wdc65816Cpu* cpu);
static void wdc65816_op_rol_b(Wdc65816Cpu* cpu);
static void wdc65816_op_rol_w(Wdc65816Cpu* cpu);
static void wdc65816_op_ror_b(Wdc65816Cpu* cpu);
static void wdc65816_op_ror_w(Wdc65816Cpu* cpu);
static void wdc65816_op_trb_b(Wdc65816Cpu* cpu);
static void wdc65816_op_trb_w(Wdc65816Cpu* cpu);
static void wdc65816_op_tsb_b(Wdc65816Cpu* cpu);
static void wdc65816_op_tsb_w(Wdc65816Cpu* cpu);

#endif //WDC65816_ALGORITHMS_H
