#ifndef WDC65186_CPU_WRITE_H
#define WDC65816_CPU_WRITE_H
#include "cpu.h"

void wdc65816_op_write_addr_sty_b(Wdc65816Cpu* cpu);
void wdc65816_op_write_addr_sty_w(Wdc65816Cpu* cpu);
void wdc65816_op_write_addr_sta_b(Wdc65816Cpu* cpu);
void wdc65816_op_write_addr_sta_w(Wdc65816Cpu* cpu);
void wdc65816_op_write_addr_stx_b(Wdc65816Cpu* cpu);
void wdc65816_op_write_addr_stx_w(Wdc65816Cpu* cpu);
void wdc65816_op_write_addr_stz_b(Wdc65816Cpu* cpu);
void wdc65816_op_write_addr_stz_w(Wdc65816Cpu* cpu);

void wdc65816_op_write_addrr_stay_b(Wdc65816Cpu* cpu);
void wdc65816_op_write_addrr_stay_w(Wdc65816Cpu* cpu);
void wdc65816_op_write_addrr_stax_b(Wdc65816Cpu* cpu);
void wdc65816_op_write_addrr_stax_w(Wdc65816Cpu* cpu);
void wdc65816_op_write_addrr_stz_b(Wdc65816Cpu* cpu);
void wdc65816_op_write_addrr_stz_w(Wdc65816Cpu* cpu);

void wdc65816_op_write_longr_sta_b(Wdc65816Cpu* cpu);
void wdc65816_op_write_longr_sta_w(Wdc65816Cpu* cpu);
void wdc65816_op_write_longr_stax_b(Wdc65816Cpu* cpu);
void wdc65816_op_write_longr_stax_w(Wdc65816Cpu* cpu);

void wdc65816_op_write_dp_stz_b(Wdc65816Cpu* cpu);
void wdc65816_op_write_dp_stz_w(Wdc65816Cpu* cpu);
void wdc65816_op_write_dp_sty_b(Wdc65816Cpu* cpu);
void wdc65816_op_write_dp_sty_w(Wdc65816Cpu* cpu);
void wdc65816_op_write_dp_sta_b(Wdc65816Cpu* cpu);
void wdc65816_op_write_dp_sta_w(Wdc65816Cpu* cpu);
void wdc65816_op_write_dp_stx_b(Wdc65816Cpu* cpu);
void wdc65816_op_write_dp_stx_w(Wdc65816Cpu* cpu);

void wdc65816_op_write_dpr_stz_b(Wdc65816Cpu* cpu);
void wdc65816_op_write_dpr_stz_w(Wdc65816Cpu* cpu);
void wdc65816_op_write_dpr_sty_b(Wdc65816Cpu* cpu);
void wdc65816_op_write_dpr_sty_w(Wdc65816Cpu* cpu);
void wdc65816_op_write_dpr_sta_b(Wdc65816Cpu* cpu);
void wdc65816_op_write_dpr_sta_w(Wdc65816Cpu* cpu);
void wdc65816_op_write_dpr_stx_b(Wdc65816Cpu* cpu);
void wdc65816_op_write_dpr_stx_w(Wdc65816Cpu* cpu);

void wdc65816_op_sta_idp_b(Wdc65816Cpu* cpu);
void wdc65816_op_sta_idp_w(Wdc65816Cpu* cpu);
void wdc65816_op_sta_ildp_b(Wdc65816Cpu* cpu);
void wdc65816_op_sta_ildp_w(Wdc65816Cpu* cpu);
void wdc65816_op_sta_idpx_b(Wdc65816Cpu* cpu);
void wdc65816_op_sta_idpx_w(Wdc65816Cpu* cpu);
void wdc65816_op_sta_idpy_b(Wdc65816Cpu* cpu);
void wdc65816_op_sta_idpy_w(Wdc65816Cpu* cpu);
void wdc65816_op_sta_ildpy_b(Wdc65816Cpu* cpu);
void wdc65816_op_sta_ildpy_w(Wdc65816Cpu* cpu);
void wdc65816_op_sta_sr_b(Wdc65816Cpu* cpu);
void wdc65816_op_sta_sr_w(Wdc65816Cpu* cpu);
void wdc65816_op_sta_isry_b(Wdc65816Cpu* cpu);
void wdc65816_op_sta_isry_w(Wdc65816Cpu* cpu);
#endif //WDC65816_CPU_WRITE_H
