#ifndef MOCKUP_CPU_ALGORITHMS_H
#define MOCKUP_CPU_ALGORITHMS_H

#include "cpu.h"

void op_adc_b(cpu_t* cpu);
void op_adc_w(cpu_t* cpu);
void op_and_b(cpu_t* cpu);
void op_and_w(cpu_t* cpu);
void op_bit_b(cpu_t* cpu);
void op_bit_w(cpu_t* cpu);
void op_cmp_b(cpu_t* cpu);
void op_cmp_w(cpu_t* cpu);
void op_cpx_b(cpu_t* cpu);
void op_cpx_w(cpu_t* cpu);
void op_cpy_b(cpu_t* cpu);
void op_cpy_w(cpu_t* cpu);
void op_eor_b(cpu_t* cpu);
void op_eor_w(cpu_t* cpu);
void op_lda_b(cpu_t* cpu);
void op_lda_w(cpu_t* cpu);
void op_ldx_b(cpu_t* cpu);
void op_ldx_w(cpu_t* cpu);
void op_ldy_b(cpu_t* cpu);
void op_ldy_w(cpu_t* cpu);
void op_ora_b(cpu_t* cpu);
void op_ora_w(cpu_t* cpu);
void op_sbc_b(cpu_t* cpu);
void op_sbc_w(cpu_t* cpu);
void op_inc_b(cpu_t* cpu);
void op_inc_w(cpu_t* cpu);
void op_dec_b(cpu_t* cpu);
void op_dec_w(cpu_t* cpu);
void op_asl_b(cpu_t* cpu);
void op_asl_w(cpu_t* cpu);
void op_lsr_b(cpu_t* cpu);
void op_lsr_w(cpu_t* cpu);
void op_rol_b(cpu_t* cpu);
void op_rol_w(cpu_t* cpu);
void op_ror_b(cpu_t* cpu);
void op_ror_w(cpu_t* cpu);
void op_trb_b(cpu_t* cpu);
void op_trb_w(cpu_t* cpu);
void op_tsb_b(cpu_t* cpu);
void op_tsb_w(cpu_t* cpu);

#endif //MOCKUP_CPU_ALGORITHMS_H
