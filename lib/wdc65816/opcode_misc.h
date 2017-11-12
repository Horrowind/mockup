#ifndef R65816_OPCODE_MISC_H
#define R65816_OPCODE_MISC_H
#include "cpu.h"

void wdc65816_op_nop(wdc65816_cpu_t* cpu);
void wdc65816_op_wdm(wdc65816_cpu_t* cpu);
void wdc65816_op_xba(wdc65816_cpu_t* cpu);
void wdc65816_op_move_mvp_b(wdc65816_cpu_t* cpu);
void wdc65816_op_move_mvp_w(wdc65816_cpu_t* cpu);
void wdc65816_op_move_mvn_b(wdc65816_cpu_t* cpu);
void wdc65816_op_move_mvn_w(wdc65816_cpu_t* cpu);
void wdc65816_op_interrupt_brk_e(wdc65816_cpu_t* cpu);
void wdc65816_op_interrupt_brk_n(wdc65816_cpu_t* cpu);
void wdc65816_op_interrupt_cop_e(wdc65816_cpu_t* cpu);
void wdc65816_op_interrupt_cop_n(wdc65816_cpu_t* cpu);
void wdc65816_op_stp(wdc65816_cpu_t* cpu);
void wdc65816_op_wai(wdc65816_cpu_t* cpu);
void wdc65816_op_xce(wdc65816_cpu_t* cpu);
void wdc65816_op_flag(wdc65816_cpu_t* cpu);
void wdc65816_op_flag_clc(wdc65816_cpu_t* cpu);
void wdc65816_op_flag_sec(wdc65816_cpu_t* cpu);
void wdc65816_op_flag_cli(wdc65816_cpu_t* cpu);
void wdc65816_op_flag_sli(wdc65816_cpu_t* cpu);
void wdc65816_op_flag_clv(wdc65816_cpu_t* cpu);
void wdc65816_op_flag_cld(wdc65816_cpu_t* cpu);
void wdc65816_op_flag_sed(wdc65816_cpu_t* cpu);
void wdc65816_op_pflag_rep_e(wdc65816_cpu_t* cpu);
void wdc65816_op_pflag_rep_n(wdc65816_cpu_t* cpu);
void wdc65816_op_pflag_sep_e(wdc65816_cpu_t* cpu);
void wdc65816_op_pflag_sep_n(wdc65816_cpu_t* cpu);
void wdc65816_op_transfer_tsc_b(wdc65816_cpu_t* cpu);
void wdc65816_op_transfer_tsc_w(wdc65816_cpu_t* cpu);
void wdc65816_op_transfer_tcd_b(wdc65816_cpu_t* cpu);
void wdc65816_op_transfer_tcd_w(wdc65816_cpu_t* cpu);
void wdc65816_op_transfer_tdc_b(wdc65816_cpu_t* cpu);
void wdc65816_op_transfer_tdc_w(wdc65816_cpu_t* cpu);
void wdc65816_op_transfer_txa_b(wdc65816_cpu_t* cpu);
void wdc65816_op_transfer_txa_w(wdc65816_cpu_t* cpu);
void wdc65816_op_transfer_tya_b(wdc65816_cpu_t* cpu);
void wdc65816_op_transfer_tya_w(wdc65816_cpu_t* cpu);
void wdc65816_op_transfer_txy_b(wdc65816_cpu_t* cpu);
void wdc65816_op_transfer_txy_w(wdc65816_cpu_t* cpu);
void wdc65816_op_transfer_tay_b(wdc65816_cpu_t* cpu);
void wdc65816_op_transfer_tay_w(wdc65816_cpu_t* cpu);
void wdc65816_op_transfer_tax_b(wdc65816_cpu_t* cpu);
void wdc65816_op_transfer_tax_w(wdc65816_cpu_t* cpu);
void wdc65816_op_transfer_tyx_b(wdc65816_cpu_t* cpu);
void wdc65816_op_transfer_tyx_w(wdc65816_cpu_t* cpu);
void wdc65816_op_tcs_e(wdc65816_cpu_t* cpu);
void wdc65816_op_tcs_n(wdc65816_cpu_t* cpu);
void wdc65816_op_tsx_b(wdc65816_cpu_t* cpu);
void wdc65816_op_tsx_w(wdc65816_cpu_t* cpu);
void wdc65816_op_txs_e(wdc65816_cpu_t* cpu);
void wdc65816_op_txs_n(wdc65816_cpu_t* cpu);
void wdc65816_op_push_pha_b(wdc65816_cpu_t* cpu);
void wdc65816_op_push_pha_w(wdc65816_cpu_t* cpu);
void wdc65816_op_push_phx_b(wdc65816_cpu_t* cpu);
void wdc65816_op_push_phx_w(wdc65816_cpu_t* cpu);
void wdc65816_op_push_phy_b(wdc65816_cpu_t* cpu);
void wdc65816_op_push_phy_w(wdc65816_cpu_t* cpu);
void wdc65816_op_phd_e(wdc65816_cpu_t* cpu);
void wdc65816_op_phd_n(wdc65816_cpu_t* cpu);
void wdc65816_op_phb(wdc65816_cpu_t* cpu);
void wdc65816_op_phk(wdc65816_cpu_t* cpu);
void wdc65816_op_php(wdc65816_cpu_t* cpu);
void wdc65816_op_pull_pla_b(wdc65816_cpu_t* cpu);
void wdc65816_op_pull_pla_w(wdc65816_cpu_t* cpu);
void wdc65816_op_pull_plx_b(wdc65816_cpu_t* cpu);
void wdc65816_op_pull_plx_w(wdc65816_cpu_t* cpu);
void wdc65816_op_pull_ply_b(wdc65816_cpu_t* cpu);
void wdc65816_op_pull_ply_w(wdc65816_cpu_t* cpu);
void wdc65816_op_pld_e(wdc65816_cpu_t* cpu);
void wdc65816_op_pld_n(wdc65816_cpu_t* cpu);
void wdc65816_op_plb(wdc65816_cpu_t* cpu);
void wdc65816_op_plp_e(wdc65816_cpu_t* cpu);
void wdc65816_op_plp_n(wdc65816_cpu_t* cpu);
void wdc65816_op_pea_e(wdc65816_cpu_t* cpu);
void wdc65816_op_pea_n(wdc65816_cpu_t* cpu);
void wdc65816_op_pei_e(wdc65816_cpu_t* cpu);
void wdc65816_op_pei_n(wdc65816_cpu_t* cpu);
void wdc65816_op_per_e(wdc65816_cpu_t* cpu);
void wdc65816_op_per_n(wdc65816_cpu_t* cpu);

#endif //R65816_OPCODE_MISC_H
