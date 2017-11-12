#ifndef MOCKUP_CPU_RMW_H
#define MOCKUP_CPU_RMW_H
#include "cpu.h"
void wdc65816_op_adjust_imm_inca_b(wdc65816_cpu_t* cpu);
void wdc65816_op_adjust_imm_inca_w(wdc65816_cpu_t* cpu);
void wdc65816_op_adjust_imm_deca_b(wdc65816_cpu_t* cpu);
void wdc65816_op_adjust_imm_deca_w(wdc65816_cpu_t* cpu);
void wdc65816_op_adjust_imm_incx_b(wdc65816_cpu_t* cpu);
void wdc65816_op_adjust_imm_incx_w(wdc65816_cpu_t* cpu);
void wdc65816_op_adjust_imm_decx_b(wdc65816_cpu_t* cpu);
void wdc65816_op_adjust_imm_decx_w(wdc65816_cpu_t* cpu);
void wdc65816_op_adjust_imm_incy_b(wdc65816_cpu_t* cpu);
void wdc65816_op_adjust_imm_incy_w(wdc65816_cpu_t* cpu);
void wdc65816_op_adjust_imm_decy_b(wdc65816_cpu_t* cpu);
void wdc65816_op_adjust_imm_decy_w(wdc65816_cpu_t* cpu);
void wdc65816_op_asl_imm_b(wdc65816_cpu_t* cpu);
void wdc65816_op_asl_imm_w(wdc65816_cpu_t* cpu);
void wdc65816_op_lsr_imm_b(wdc65816_cpu_t* cpu);
void wdc65816_op_lsr_imm_w(wdc65816_cpu_t* cpu);
void wdc65816_op_rol_imm_b(wdc65816_cpu_t* cpu);
void wdc65816_op_rol_imm_w(wdc65816_cpu_t* cpu);
void wdc65816_op_ror_imm_b(wdc65816_cpu_t* cpu);
void wdc65816_op_ror_imm_w(wdc65816_cpu_t* cpu);
void wdc65816_op_adjust_addr_tsb_b(wdc65816_cpu_t* cpu);
void wdc65816_op_adjust_addr_tsb_w(wdc65816_cpu_t* cpu);
void wdc65816_op_adjust_addr_asl_b(wdc65816_cpu_t* cpu);
void wdc65816_op_adjust_addr_asl_w(wdc65816_cpu_t* cpu);
void wdc65816_op_adjust_addr_trb_b(wdc65816_cpu_t* cpu);
void wdc65816_op_adjust_addr_trb_w(wdc65816_cpu_t* cpu);
void wdc65816_op_adjust_addr_rol_b(wdc65816_cpu_t* cpu);
void wdc65816_op_adjust_addr_rol_w(wdc65816_cpu_t* cpu);
void wdc65816_op_adjust_addr_lsr_b(wdc65816_cpu_t* cpu);
void wdc65816_op_adjust_addr_lsr_w(wdc65816_cpu_t* cpu);
void wdc65816_op_adjust_addr_ror_b(wdc65816_cpu_t* cpu);
void wdc65816_op_adjust_addr_ror_w(wdc65816_cpu_t* cpu);
void wdc65816_op_adjust_addr_dec_b(wdc65816_cpu_t* cpu);
void wdc65816_op_adjust_addr_dec_w(wdc65816_cpu_t* cpu);
void wdc65816_op_adjust_addr_inc_b(wdc65816_cpu_t* cpu);
void wdc65816_op_adjust_addr_inc_w(wdc65816_cpu_t* cpu);
void wdc65816_op_adjust_addrx_asl_b(wdc65816_cpu_t* cpu);
void wdc65816_op_adjust_addrx_asl_w(wdc65816_cpu_t* cpu);
void wdc65816_op_adjust_addrx_rol_b(wdc65816_cpu_t* cpu);
void wdc65816_op_adjust_addrx_rol_w(wdc65816_cpu_t* cpu);
void wdc65816_op_adjust_addrx_lsr_b(wdc65816_cpu_t* cpu);
void wdc65816_op_adjust_addrx_lsr_w(wdc65816_cpu_t* cpu);
void wdc65816_op_adjust_addrx_ror_b(wdc65816_cpu_t* cpu);
void wdc65816_op_adjust_addrx_ror_w(wdc65816_cpu_t* cpu);
void wdc65816_op_adjust_addrx_dec_b(wdc65816_cpu_t* cpu);
void wdc65816_op_adjust_addrx_dec_w(wdc65816_cpu_t* cpu);
void wdc65816_op_adjust_addrx_inc_b(wdc65816_cpu_t* cpu);
void wdc65816_op_adjust_addrx_inc_w(wdc65816_cpu_t* cpu);
void wdc65816_op_adjust_dp_tsb_b(wdc65816_cpu_t* cpu);
void wdc65816_op_adjust_dp_tsb_w(wdc65816_cpu_t* cpu);
void wdc65816_op_adjust_dp_asl_b(wdc65816_cpu_t* cpu);
void wdc65816_op_adjust_dp_asl_w(wdc65816_cpu_t* cpu);
void wdc65816_op_adjust_dp_trb_b(wdc65816_cpu_t* cpu);
void wdc65816_op_adjust_dp_trb_w(wdc65816_cpu_t* cpu);
void wdc65816_op_adjust_dp_rol_b(wdc65816_cpu_t* cpu);
void wdc65816_op_adjust_dp_rol_w(wdc65816_cpu_t* cpu);
void wdc65816_op_adjust_dp_lsr_b(wdc65816_cpu_t* cpu);
void wdc65816_op_adjust_dp_lsr_w(wdc65816_cpu_t* cpu);
void wdc65816_op_adjust_dp_ror_b(wdc65816_cpu_t* cpu);
void wdc65816_op_adjust_dp_ror_w(wdc65816_cpu_t* cpu);
void wdc65816_op_adjust_dp_dec_b(wdc65816_cpu_t* cpu);
void wdc65816_op_adjust_dp_dec_w(wdc65816_cpu_t* cpu);
void wdc65816_op_adjust_dp_inc_b(wdc65816_cpu_t* cpu);
void wdc65816_op_adjust_dp_inc_w(wdc65816_cpu_t* cpu);
void wdc65816_op_adjust_dpx_asl_b(wdc65816_cpu_t* cpu);
void wdc65816_op_adjust_dpx_asl_w(wdc65816_cpu_t* cpu);
void wdc65816_op_adjust_dpx_rol_b(wdc65816_cpu_t* cpu);
void wdc65816_op_adjust_dpx_rol_w(wdc65816_cpu_t* cpu);
void wdc65816_op_adjust_dpx_lsr_b(wdc65816_cpu_t* cpu);
void wdc65816_op_adjust_dpx_lsr_w(wdc65816_cpu_t* cpu);
void wdc65816_op_adjust_dpx_ror_b(wdc65816_cpu_t* cpu);
void wdc65816_op_adjust_dpx_ror_w(wdc65816_cpu_t* cpu);
void wdc65816_op_adjust_dpx_dec_b(wdc65816_cpu_t* cpu);
void wdc65816_op_adjust_dpx_dec_w(wdc65816_cpu_t* cpu);
void wdc65816_op_adjust_dpx_inc_b(wdc65816_cpu_t* cpu);
void wdc65816_op_adjust_dpx_inc_w(wdc65816_cpu_t* cpu);
#endif //MOCKUP_CPU_RMW_H