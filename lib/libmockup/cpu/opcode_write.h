#ifndef MOCKUP_CPU_WRITE_H
#define MOCKUP_CPU_WRITE_H

#include "cpu.h"

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
