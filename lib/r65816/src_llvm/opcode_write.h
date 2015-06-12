#ifndef MOCKUP_CPU_WRITE_H
#define MOCKUP_CPU_WRITE_H
#include "cpu.h"

void r65816_op_write_addr_sty_b(r65816_cpu_t* cpu);
void r65816_op_write_addr_sty_w(r65816_cpu_t* cpu);
void r65816_op_write_addr_sta_b(r65816_cpu_t* cpu);
void r65816_op_write_addr_sta_w(r65816_cpu_t* cpu);
void r65816_op_write_addr_stx_b(r65816_cpu_t* cpu);
void r65816_op_write_addr_stx_w(r65816_cpu_t* cpu);
void r65816_op_write_addr_stz_b(r65816_cpu_t* cpu);
void r65816_op_write_addr_stz_w(r65816_cpu_t* cpu);

void r65816_op_write_addrr_stay_b(r65816_cpu_t* cpu);
void r65816_op_write_addrr_stay_w(r65816_cpu_t* cpu);
void r65816_op_write_addrr_stax_b(r65816_cpu_t* cpu);
void r65816_op_write_addrr_stax_w(r65816_cpu_t* cpu);
void r65816_op_write_addrr_stz_b(r65816_cpu_t* cpu);
void r65816_op_write_addrr_stz_w(r65816_cpu_t* cpu);

void r65816_op_write_longr_sta_b(r65816_cpu_t* cpu);
void r65816_op_write_longr_sta_w(r65816_cpu_t* cpu);
void r65816_op_write_longr_stax_b(r65816_cpu_t* cpu);
void r65816_op_write_longr_stax_w(r65816_cpu_t* cpu);

void r65816_op_write_dp_stz_b(r65816_cpu_t* cpu);
void r65816_op_write_dp_stz_w(r65816_cpu_t* cpu);
void r65816_op_write_dp_sty_b(r65816_cpu_t* cpu);
void r65816_op_write_dp_sty_w(r65816_cpu_t* cpu);
void r65816_op_write_dp_sta_b(r65816_cpu_t* cpu);
void r65816_op_write_dp_sta_w(r65816_cpu_t* cpu);
void r65816_op_write_dp_stx_b(r65816_cpu_t* cpu);
void r65816_op_write_dp_stx_w(r65816_cpu_t* cpu);

void r65816_op_write_dpr_stz_b(r65816_cpu_t* cpu);
void r65816_op_write_dpr_stz_w(r65816_cpu_t* cpu);
void r65816_op_write_dpr_sty_b(r65816_cpu_t* cpu);
void r65816_op_write_dpr_sty_w(r65816_cpu_t* cpu);
void r65816_op_write_dpr_sta_b(r65816_cpu_t* cpu);
void r65816_op_write_dpr_sta_w(r65816_cpu_t* cpu);
void r65816_op_write_dpr_stx_b(r65816_cpu_t* cpu);
void r65816_op_write_dpr_stx_w(r65816_cpu_t* cpu);

void r65816_op_sta_idp_b(r65816_cpu_t* cpu);
void r65816_op_sta_idp_w(r65816_cpu_t* cpu);
void r65816_op_sta_ildp_b(r65816_cpu_t* cpu);
void r65816_op_sta_ildp_w(r65816_cpu_t* cpu);
void r65816_op_sta_idpx_b(r65816_cpu_t* cpu);
void r65816_op_sta_idpx_w(r65816_cpu_t* cpu);
void r65816_op_sta_idpy_b(r65816_cpu_t* cpu);
void r65816_op_sta_idpy_w(r65816_cpu_t* cpu);
void r65816_op_sta_ildpy_b(r65816_cpu_t* cpu);
void r65816_op_sta_ildpy_w(r65816_cpu_t* cpu);
void r65816_op_sta_sr_b(r65816_cpu_t* cpu);
void r65816_op_sta_sr_w(r65816_cpu_t* cpu);
void r65816_op_sta_isry_b(r65816_cpu_t* cpu);
void r65816_op_sta_isry_w(r65816_cpu_t* cpu);
#endif //MOCKUP_CPU_WRITE_H
