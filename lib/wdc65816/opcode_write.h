#ifndef MOCKUP_CPU_WRITE_H
#define MOCKUP_CPU_WRITE_H
#include "cpu.h"

void wdc65816_op_write_addr_sty_b(wdc65816_cpu_t* cpu);
void wdc65816_op_write_addr_sty_w(wdc65816_cpu_t* cpu);
void wdc65816_op_write_addr_sta_b(wdc65816_cpu_t* cpu);
void wdc65816_op_write_addr_sta_w(wdc65816_cpu_t* cpu);
void wdc65816_op_write_addr_stx_b(wdc65816_cpu_t* cpu);
void wdc65816_op_write_addr_stx_w(wdc65816_cpu_t* cpu);
void wdc65816_op_write_addr_stz_b(wdc65816_cpu_t* cpu);
void wdc65816_op_write_addr_stz_w(wdc65816_cpu_t* cpu);

void wdc65816_op_write_addrr_stay_b(wdc65816_cpu_t* cpu);
void wdc65816_op_write_addrr_stay_w(wdc65816_cpu_t* cpu);
void wdc65816_op_write_addrr_stax_b(wdc65816_cpu_t* cpu);
void wdc65816_op_write_addrr_stax_w(wdc65816_cpu_t* cpu);
void wdc65816_op_write_addrr_stz_b(wdc65816_cpu_t* cpu);
void wdc65816_op_write_addrr_stz_w(wdc65816_cpu_t* cpu);

void wdc65816_op_write_longr_sta_b(wdc65816_cpu_t* cpu);
void wdc65816_op_write_longr_sta_w(wdc65816_cpu_t* cpu);
void wdc65816_op_write_longr_stax_b(wdc65816_cpu_t* cpu);
void wdc65816_op_write_longr_stax_w(wdc65816_cpu_t* cpu);

void wdc65816_op_write_dp_stz_b(wdc65816_cpu_t* cpu);
void wdc65816_op_write_dp_stz_w(wdc65816_cpu_t* cpu);
void wdc65816_op_write_dp_sty_b(wdc65816_cpu_t* cpu);
void wdc65816_op_write_dp_sty_w(wdc65816_cpu_t* cpu);
void wdc65816_op_write_dp_sta_b(wdc65816_cpu_t* cpu);
void wdc65816_op_write_dp_sta_w(wdc65816_cpu_t* cpu);
void wdc65816_op_write_dp_stx_b(wdc65816_cpu_t* cpu);
void wdc65816_op_write_dp_stx_w(wdc65816_cpu_t* cpu);

void wdc65816_op_write_dpr_stz_b(wdc65816_cpu_t* cpu);
void wdc65816_op_write_dpr_stz_w(wdc65816_cpu_t* cpu);
void wdc65816_op_write_dpr_sty_b(wdc65816_cpu_t* cpu);
void wdc65816_op_write_dpr_sty_w(wdc65816_cpu_t* cpu);
void wdc65816_op_write_dpr_sta_b(wdc65816_cpu_t* cpu);
void wdc65816_op_write_dpr_sta_w(wdc65816_cpu_t* cpu);
void wdc65816_op_write_dpr_stx_b(wdc65816_cpu_t* cpu);
void wdc65816_op_write_dpr_stx_w(wdc65816_cpu_t* cpu);

void wdc65816_op_sta_idp_b(wdc65816_cpu_t* cpu);
void wdc65816_op_sta_idp_w(wdc65816_cpu_t* cpu);
void wdc65816_op_sta_ildp_b(wdc65816_cpu_t* cpu);
void wdc65816_op_sta_ildp_w(wdc65816_cpu_t* cpu);
void wdc65816_op_sta_idpx_b(wdc65816_cpu_t* cpu);
void wdc65816_op_sta_idpx_w(wdc65816_cpu_t* cpu);
void wdc65816_op_sta_idpy_b(wdc65816_cpu_t* cpu);
void wdc65816_op_sta_idpy_w(wdc65816_cpu_t* cpu);
void wdc65816_op_sta_ildpy_b(wdc65816_cpu_t* cpu);
void wdc65816_op_sta_ildpy_w(wdc65816_cpu_t* cpu);
void wdc65816_op_sta_sr_b(wdc65816_cpu_t* cpu);
void wdc65816_op_sta_sr_w(wdc65816_cpu_t* cpu);
void wdc65816_op_sta_isry_b(wdc65816_cpu_t* cpu);
void wdc65816_op_sta_isry_w(wdc65816_cpu_t* cpu);
#endif //MOCKUP_CPU_WRITE_H
