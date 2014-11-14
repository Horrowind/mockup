#ifndef MOCKUP_CPU_RMW_H
#define MOCKUP_CPU_RMW_H

#include "cpu.h"

void op_adjust_imm_inca_b(cpu_t* cpu);
void op_adjust_imm_inca_w(cpu_t* cpu);
void op_adjust_imm_deca_b(cpu_t* cpu);
void op_adjust_imm_deca_w(cpu_t* cpu);
void op_adjust_imm_incx_b(cpu_t* cpu);
void op_adjust_imm_incx_w(cpu_t* cpu);
void op_adjust_imm_decx_b(cpu_t* cpu);
void op_adjust_imm_decx_w(cpu_t* cpu);
void op_adjust_imm_incy_b(cpu_t* cpu);
void op_adjust_imm_incy_w(cpu_t* cpu);
void op_adjust_imm_decy_b(cpu_t* cpu);
void op_adjust_imm_decy_w(cpu_t* cpu);

void op_asl_imm_b(cpu_t* cpu);
void op_asl_imm_w(cpu_t* cpu);
void op_lsr_imm_b(cpu_t* cpu);
void op_lsr_imm_w(cpu_t* cpu);
void op_rol_imm_b(cpu_t* cpu);
void op_rol_imm_w(cpu_t* cpu);
void op_ror_imm_b(cpu_t* cpu);
void op_ror_imm_w(cpu_t* cpu);

void op_adjust_addr_tsb_b(cpu_t* cpu);
void op_adjust_addr_tsb_w(cpu_t* cpu);
void op_adjust_addr_asl_b(cpu_t* cpu);
void op_adjust_addr_asl_w(cpu_t* cpu);
void op_adjust_addr_trb_b(cpu_t* cpu);
void op_adjust_addr_trb_w(cpu_t* cpu);
void op_adjust_addr_rol_b(cpu_t* cpu);
void op_adjust_addr_rol_w(cpu_t* cpu);
void op_adjust_addr_lsr_b(cpu_t* cpu);
void op_adjust_addr_lsr_w(cpu_t* cpu);
void op_adjust_addr_ror_b(cpu_t* cpu);
void op_adjust_addr_ror_w(cpu_t* cpu);
void op_adjust_addr_dec_b(cpu_t* cpu);
void op_adjust_addr_dec_w(cpu_t* cpu);
void op_adjust_addr_inc_b(cpu_t* cpu);
void op_adjust_addr_inc_w(cpu_t* cpu);

void op_adjust_addrx_asl_b(cpu_t* cpu);
void op_adjust_addrx_asl_w(cpu_t* cpu);
void op_adjust_addrx_rol_b(cpu_t* cpu);
void op_adjust_addrx_rol_w(cpu_t* cpu);
void op_adjust_addrx_lsr_b(cpu_t* cpu);
void op_adjust_addrx_lsr_w(cpu_t* cpu);
void op_adjust_addrx_ror_b(cpu_t* cpu);
void op_adjust_addrx_ror_w(cpu_t* cpu);
void op_adjust_addrx_dec_b(cpu_t* cpu);
void op_adjust_addrx_dec_w(cpu_t* cpu);
void op_adjust_addrx_inc_b(cpu_t* cpu);
void op_adjust_addrx_inc_w(cpu_t* cpu);

void op_adjust_dp_tsb_b(cpu_t* cpu);
void op_adjust_dp_tsb_w(cpu_t* cpu);
void op_adjust_dp_asl_b(cpu_t* cpu);
void op_adjust_dp_asl_w(cpu_t* cpu);
void op_adjust_dp_trb_b(cpu_t* cpu);
void op_adjust_dp_trb_w(cpu_t* cpu);
void op_adjust_dp_rol_b(cpu_t* cpu);
void op_adjust_dp_rol_w(cpu_t* cpu);
void op_adjust_dp_lsr_b(cpu_t* cpu);
void op_adjust_dp_lsr_w(cpu_t* cpu);
void op_adjust_dp_ror_b(cpu_t* cpu);
void op_adjust_dp_ror_w(cpu_t* cpu);
void op_adjust_dp_dec_b(cpu_t* cpu);
void op_adjust_dp_dec_w(cpu_t* cpu);
void op_adjust_dp_inc_b(cpu_t* cpu);
void op_adjust_dp_inc_w(cpu_t* cpu);

void op_adjust_dpx_asl_b(cpu_t* cpu);
void op_adjust_dpx_asl_w(cpu_t* cpu);
void op_adjust_dpx_rol_b(cpu_t* cpu);
void op_adjust_dpx_rol_w(cpu_t* cpu);
void op_adjust_dpx_lsr_b(cpu_t* cpu);
void op_adjust_dpx_lsr_w(cpu_t* cpu);
void op_adjust_dpx_ror_b(cpu_t* cpu);
void op_adjust_dpx_ror_w(cpu_t* cpu);
void op_adjust_dpx_dec_b(cpu_t* cpu);
void op_adjust_dpx_dec_w(cpu_t* cpu);
void op_adjust_dpx_inc_b(cpu_t* cpu);
void op_adjust_dpx_inc_w(cpu_t* cpu);

#endif //MOCKUP_CPU_RMW_H
