#ifndef MOCKUP_CPU_WRITE_H
#define MOCKUP_CPU_WRITE_H

#include "cpu.h"

void op_write_addr_sty_b(cpu_t* cpu);
void op_write_addr_sty_w(cpu_t* cpu);
void op_write_addr_sta_b(cpu_t* cpu);
void op_write_addr_sta_w(cpu_t* cpu);
void op_write_addr_stx_b(cpu_t* cpu);
void op_write_addr_stx_w(cpu_t* cpu);
void op_write_addr_stz_b(cpu_t* cpu);
void op_write_addr_stz_w(cpu_t* cpu);

void op_write_addrr_stay_b(cpu_t* cpu);
void op_write_addrr_stay_w(cpu_t* cpu);
void op_write_addrr_stax_b(cpu_t* cpu);
void op_write_addrr_stax_w(cpu_t* cpu);
void op_write_addrr_stz_b(cpu_t* cpu);
void op_write_addrr_stz_w(cpu_t* cpu);

void op_write_longr_sta_b(cpu_t* cpu);
void op_write_longr_sta_w(cpu_t* cpu);
void op_write_longr_stax_b(cpu_t* cpu);
void op_write_longr_stax_w(cpu_t* cpu);

void op_write_dp_stz_b(cpu_t* cpu);
void op_write_dp_stz_w(cpu_t* cpu);
void op_write_dp_sty_b(cpu_t* cpu);
void op_write_dp_sty_w(cpu_t* cpu);
void op_write_dp_sta_b(cpu_t* cpu);
void op_write_dp_sta_w(cpu_t* cpu);
void op_write_dp_stx_b(cpu_t* cpu);
void op_write_dp_stx_w(cpu_t* cpu);

void op_write_dpr_stz_b(cpu_t* cpu);
void op_write_dpr_stz_w(cpu_t* cpu);
void op_write_dpr_sty_b(cpu_t* cpu);
void op_write_dpr_sty_w(cpu_t* cpu);
void op_write_dpr_sta_b(cpu_t* cpu);
void op_write_dpr_sta_w(cpu_t* cpu);
void op_write_dpr_stx_b(cpu_t* cpu);
void op_write_dpr_stx_w(cpu_t* cpu);


void op_sta_idp_b(cpu_t* cpu);
void op_sta_idp_w(cpu_t* cpu);
void op_sta_ildp_b(cpu_t* cpu);
void op_sta_ildp_w(cpu_t* cpu);
void op_sta_idpx_b(cpu_t* cpu);
void op_sta_idpx_w(cpu_t* cpu);
void op_sta_idpy_b(cpu_t* cpu);
void op_sta_idpy_w(cpu_t* cpu);
void op_sta_ildpy_b(cpu_t* cpu);
void op_sta_ildpy_w(cpu_t* cpu);
void op_sta_sr_b(cpu_t* cpu);
void op_sta_sr_w(cpu_t* cpu);
void op_sta_isry_b(cpu_t* cpu);
void op_sta_isry_w(cpu_t* cpu);

#endif //MOCKUP_CPU_WRITE_H
