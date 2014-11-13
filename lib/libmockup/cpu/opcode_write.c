#include "cpu/memory.h"
#include "cpu/opcode_write.h"

template<int n> void op_write_addr_b(cpu_t* cpu) {
  cpu->aa.l = op_readpc(cpu);
  cpu->aa.h = op_readpc(cpu);
  op_writedbr(cpu, cpu->aa.w, cpu->regs.r[n]);
}

template<int n> void op_write_addr_w(cpu_t* cpu) {
  cpu->aa.l = op_readpc(cpu);
  cpu->aa.h = op_readpc(cpu);
  op_writedbr(cpu, cpu->aa.w + 0, cpu->regs.r[n] >> 0);
  op_writedbr(cpu, cpu->aa.w + 1, cpu->regs.r[n] >> 8);
}

template<int n, int i> void op_write_addrr_b(cpu_t* cpu) {
  cpu->aa.l = op_readpc(cpu);
  cpu->aa.h = op_readpc(cpu);
  op_writedbr(cpu, cpu->aa.w + cpu->regs.r[i], cpu->regs.r[n]);
}

template<int n, int i> void op_write_addrr_w(cpu_t* cpu) {
  cpu->aa.l = op_readpc(cpu);
  cpu->aa.h = op_readpc(cpu);
  op_writedbr(cpu, cpu->aa.w + cpu->regs.r[i] + 0, cpu->regs.r[n] >> 0);
  op_writedbr(cpu, cpu->aa.w + cpu->regs.r[i] + 1, cpu->regs.r[n] >> 8);
}

template<int i> void op_write_longr_b(cpu_t* cpu) {
  cpu->aa.l = op_readpc(cpu);
  cpu->aa.h = op_readpc(cpu);
  cpu->aa.b = op_readpc(cpu);
  op_writelong(cpu, cpu->aa.d + cpu->regs.r[i], cpu->regs.a.l);
}

template<int i> void op_write_longr_w(cpu_t* cpu) {
  cpu->aa.l = op_readpc(cpu);
  cpu->aa.h = op_readpc(cpu);
  cpu->aa.b = op_readpc(cpu);
  op_writelong(cpu, cpu->aa.d + cpu->regs.r[i] + 0, cpu->regs.a.l);
  op_writelong(cpu, cpu->aa.d + cpu->regs.r[i] + 1, cpu->regs.a.h);
}

template<int n> void op_write_dp_b(cpu_t* cpu) {
  cpu->dp = op_readpc(cpu);
  op_writedp(cpu->dp, cpu->regs.r[n]);
}

template<int n> void op_write_dp_w(cpu_t* cpu) {
  cpu->dp = op_readpc(cpu);
  op_writedp(cpu->dp + 0, cpu->regs.r[n] >> 0);
  op_writedp(cpu->dp + 1, cpu->regs.r[n] >> 8);
}

template<int n, int i> void op_write_dpr_b(cpu_t* cpu) {
  cpu->dp = op_readpc(cpu);
  op_writedp(cpu->dp + cpu->regs.r[i], cpu->regs.r[n]);
}

template<int n, int i> void op_write_dpr_w(cpu_t* cpu) {
  cpu->dp = op_readpc(cpu);
  op_writedp(cpu->dp + cpu->regs.r[i] + 0, cpu->regs.r[n] >> 0);
  op_writedp(cpu->dp + cpu->regs.r[i] + 1, cpu->regs.r[n] >> 8);
}

void op_sta_idp_b(cpu_t* cpu) {
  cpu->dp = op_readpc(cpu);
  cpu->aa.l = op_readdp(cpu, cpu->dp + 0);
  cpu->aa.h = op_readdp(cpu, cpu->dp + 1);
  op_writedbr(cpu, cpu->aa.w, cpu->regs.a.l);
}

void op_sta_idp_w(cpu_t* cpu) {
  cpu->dp = op_readpc(cpu);
  cpu->aa.l = op_readdp(cpu, cpu->dp + 0);
  cpu->aa.h = op_readdp(cpu, cpu->dp + 1);
  op_writedbr(cpu, cpu->aa.w + 0, cpu->regs.a.l);
  op_writedbr(cpu, cpu->aa.w + 1, cpu->regs.a.h);
}

void op_sta_ildp_b(cpu_t* cpu) {
  cpu->dp = op_readpc(cpu);
  cpu->aa.l = op_readdp(cpu, cpu->dp + 0);
  cpu->aa.h = op_readdp(cpu, cpu->dp + 1);
  cpu->aa.b = op_readdp(cpu, cpu->dp + 2);
  op_writelong(cpu, cpu->aa.d, cpu->regs.a.l);
}

void op_sta_ildp_w(cpu_t* cpu) {
  cpu->dp = op_readpc(cpu);
  cpu->aa.l = op_readdp(cpu, cpu->dp + 0);
  cpu->aa.h = op_readdp(cpu, cpu->dp + 1);
  cpu->aa.b = op_readdp(cpu, cpu->dp + 2);
  op_writelong(cpu, cpu->aa.d + 0, cpu->regs.a.l);
  op_writelong(cpu, cpu->aa.d + 1, cpu->regs.a.h);
}

void op_sta_idpx_b(cpu_t* cpu) {
  cpu->dp = op_readpc(cpu);
  cpu->aa.l = op_readdp(cpu, cpu->dp + cpu->regs.x.w + 0);
  cpu->aa.h = op_readdp(cpu, cpu->dp + cpu->regs.x.w + 1);
  op_writedbr(cpu, cpu->aa.w, cpu->regs.a.l);
}

void op_sta_idpx_w(cpu_t* cpu) {
  cpu->dp = op_readpc(cpu);
  cpu->aa.l = op_readdp(cpu, cpu->dp + cpu->regs.x.w + 0);
  cpu->aa.h = op_readdp(cpu, cpu->dp + cpu->regs.x.w + 1);
  op_writedbr(cpu, cpu->aa.w + 0, cpu->regs.a.l);
  op_writedbr(cpu, cpu->aa.w + 1, cpu->regs.a.h);
}

void op_sta_idpy_b(cpu_t* cpu) {
  cpu->dp = op_readpc(cpu);
  cpu->aa.l = op_readdp(cpu, cpu->dp + 0);
  cpu->aa.h = op_readdp(cpu, cpu->dp + 1);
  op_writedbr(cpu, cpu->aa.w + cpu->regs.y.w, cpu->regs.a.l);
}

void op_sta_idpy_w(cpu_t* cpu) {
  cpu->dp = op_readpc(cpu);
  cpu->aa.l = op_readdp(cpu, cpu->dp + 0);
  cpu->aa.h = op_readdp(cpu, cpu->dp + 1);
  op_writedbr(cpu, cpu->aa.w + cpu->regs.y.w + 0, cpu->regs.a.l);
  op_writedbr(cpu, cpu->aa.w + cpu->regs.y.w + 1, cpu->regs.a.h);
}

void op_sta_ildpy_b(cpu_t* cpu) {
  cpu->dp = op_readpc(cpu);
  cpu->aa.l = op_readdp(cpu, cpu->dp + 0);
  cpu->aa.h = op_readdp(cpu, cpu->dp + 1);
  cpu->aa.b = op_readdp(cpu, cpu->dp + 2);
  op_writelong(cpu, cpu->aa.d + cpu->regs.y.w, cpu->regs.a.l);
}

void op_sta_ildpy_w(cpu_t* cpu) {
  cpu->dp = op_readpc(cpu);
  cpu->aa.l = op_readdp(cpu, cpu->dp + 0);
  cpu->aa.h = op_readdp(cpu, cpu->dp + 1);
  cpu->aa.b = op_readdp(cpu, cpu->dp + 2);
  op_writelong(cpu, cpu->aa.d + cpu->regs.y.w + 0, cpu->regs.a.l);
  op_writelong(cpu, cpu->aa.d + cpu->regs.y.w + 1, cpu->regs.a.h);
}

void op_sta_sr_b(cpu_t* cpu) {
  cpu->sp = op_readpc(cpu);
  op_writesp(cpu, cpu->sp, cpu->regs.a.l);
}

void op_sta_sr_w(cpu_t* cpu) {
  cpu->sp = op_readpc(cpu);
  op_writesp(cpu, cpu->sp + 0, cpu->regs.a.l);
  op_writesp(cpu, cpu->sp + 1, cpu->regs.a.h);
}

void op_sta_isry_b(cpu_t* cpu) {
  cpu->sp = op_readpc(cpu);
  cpu->aa.l = op_readsp(cpu, cpu->sp + 0);
  cpu->aa.h = op_readsp(cpu, cpu->sp + 1);
  op_writedbr(cpu, cpu->aa.w + cpu->regs.y.w, cpu->regs.a.l);
}

void op_sta_isry_w(cpu_t* cpu) {
  cpu->sp = op_readpc(cpu);
  cpu->aa.l = op_readsp(cpu, cpu->sp + 0);
  cpu->aa.h = op_readsp(cpu, cpu->sp + 1);
  op_writedbr(cpu, cpu->aa.w + cpu->regs.y.w + 0, cpu->regs.a.l);
  op_writedbr(cpu, cpu->aa.w + cpu->regs.y.w + 1, cpu->regs.a.h);
}
