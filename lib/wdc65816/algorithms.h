#ifndef R65816_ALGORITHMS_H
#define R65816_ALGORITHMS_H

#include "cpu.h"

void wdc65816_op_adc_b(wdc65816_cpu_t* cpu);
void wdc65816_op_adc_w(wdc65816_cpu_t* cpu);
void wdc65816_op_and_b(wdc65816_cpu_t* cpu);
void wdc65816_op_and_w(wdc65816_cpu_t* cpu);
void wdc65816_op_bit_b(wdc65816_cpu_t* cpu);
void wdc65816_op_bit_w(wdc65816_cpu_t* cpu);
void wdc65816_op_cmp_b(wdc65816_cpu_t* cpu);
void wdc65816_op_cmp_w(wdc65816_cpu_t* cpu);
void wdc65816_op_cpx_b(wdc65816_cpu_t* cpu);
void wdc65816_op_cpx_w(wdc65816_cpu_t* cpu);
void wdc65816_op_cpy_b(wdc65816_cpu_t* cpu);
void wdc65816_op_cpy_w(wdc65816_cpu_t* cpu);
void wdc65816_op_eor_b(wdc65816_cpu_t* cpu);
void wdc65816_op_eor_w(wdc65816_cpu_t* cpu);
void wdc65816_op_lda_b(wdc65816_cpu_t* cpu);
void wdc65816_op_lda_w(wdc65816_cpu_t* cpu);
void wdc65816_op_ldx_b(wdc65816_cpu_t* cpu);
void wdc65816_op_ldx_w(wdc65816_cpu_t* cpu);
void wdc65816_op_ldy_b(wdc65816_cpu_t* cpu);
void wdc65816_op_ldy_w(wdc65816_cpu_t* cpu);
void wdc65816_op_ora_b(wdc65816_cpu_t* cpu);
void wdc65816_op_ora_w(wdc65816_cpu_t* cpu);
void wdc65816_op_sbc_b(wdc65816_cpu_t* cpu);
void wdc65816_op_sbc_w(wdc65816_cpu_t* cpu);
void wdc65816_op_inc_b(wdc65816_cpu_t* cpu);
void wdc65816_op_inc_w(wdc65816_cpu_t* cpu);
void wdc65816_op_dec_b(wdc65816_cpu_t* cpu);
void wdc65816_op_dec_w(wdc65816_cpu_t* cpu);
void wdc65816_op_asl_b(wdc65816_cpu_t* cpu);
void wdc65816_op_asl_w(wdc65816_cpu_t* cpu);
void wdc65816_op_lsr_b(wdc65816_cpu_t* cpu);
void wdc65816_op_lsr_w(wdc65816_cpu_t* cpu);
void wdc65816_op_rol_b(wdc65816_cpu_t* cpu);
void wdc65816_op_rol_w(wdc65816_cpu_t* cpu);
void wdc65816_op_ror_b(wdc65816_cpu_t* cpu);
void wdc65816_op_ror_w(wdc65816_cpu_t* cpu);
void wdc65816_op_trb_b(wdc65816_cpu_t* cpu);
void wdc65816_op_trb_w(wdc65816_cpu_t* cpu);
void wdc65816_op_tsb_b(wdc65816_cpu_t* cpu);
void wdc65816_op_tsb_w(wdc65816_cpu_t* cpu);

#endif //R65816_ALGORITHMS_H
