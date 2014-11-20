#ifndef MOCKUP_CPU_ALGORITHMS_H
#define MOCKUP_CPU_ALGORITHMS_H

#include "cpu.h"

void r65816_op_adc_b(r65816_cpu_t* cpu);
void r65816_op_adc_w(r65816_cpu_t* cpu);
void r65816_op_and_b(r65816_cpu_t* cpu);
void r65816_op_and_w(r65816_cpu_t* cpu);
void r65816_op_bit_b(r65816_cpu_t* cpu);
void r65816_op_bit_w(r65816_cpu_t* cpu);
void r65816_op_cmp_b(r65816_cpu_t* cpu);
void r65816_op_cmp_w(r65816_cpu_t* cpu);
void r65816_op_cpx_b(r65816_cpu_t* cpu);
void r65816_op_cpx_w(r65816_cpu_t* cpu);
void r65816_op_cpy_b(r65816_cpu_t* cpu);
void r65816_op_cpy_w(r65816_cpu_t* cpu);
void r65816_op_eor_b(r65816_cpu_t* cpu);
void r65816_op_eor_w(r65816_cpu_t* cpu);
void r65816_op_lda_b(r65816_cpu_t* cpu);
void r65816_op_lda_w(r65816_cpu_t* cpu);
void r65816_op_ldx_b(r65816_cpu_t* cpu);
void r65816_op_ldx_w(r65816_cpu_t* cpu);
void r65816_op_ldy_b(r65816_cpu_t* cpu);
void r65816_op_ldy_w(r65816_cpu_t* cpu);
void r65816_op_ora_b(r65816_cpu_t* cpu);
void r65816_op_ora_w(r65816_cpu_t* cpu);
void r65816_op_sbc_b(r65816_cpu_t* cpu);
void r65816_op_sbc_w(r65816_cpu_t* cpu);
void r65816_op_inc_b(r65816_cpu_t* cpu);
void r65816_op_inc_w(r65816_cpu_t* cpu);
void r65816_op_dec_b(r65816_cpu_t* cpu);
void r65816_op_dec_w(r65816_cpu_t* cpu);
void r65816_op_asl_b(r65816_cpu_t* cpu);
void r65816_op_asl_w(r65816_cpu_t* cpu);
void r65816_op_lsr_b(r65816_cpu_t* cpu);
void r65816_op_lsr_w(r65816_cpu_t* cpu);
void r65816_op_rol_b(r65816_cpu_t* cpu);
void r65816_op_rol_w(r65816_cpu_t* cpu);
void r65816_op_ror_b(r65816_cpu_t* cpu);
void r65816_op_ror_w(r65816_cpu_t* cpu);
void r65816_op_trb_b(r65816_cpu_t* cpu);
void r65816_op_trb_w(r65816_cpu_t* cpu);
void r65816_op_tsb_b(r65816_cpu_t* cpu);
void r65816_op_tsb_w(r65816_cpu_t* cpu);

#endif //MOCKUP_CPU_ALGORITHMS_H
