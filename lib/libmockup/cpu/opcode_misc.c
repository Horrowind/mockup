#include "opcode_misc.h"

void op_nop(cpu_t* cpu) { }

void op_wdm(cpu_t* cpu) { }

void op_xba(cpu_t* cpu) {
    cpu->regs.a.l ^= cpu->regs.a.h;
    cpu->regs.a.h ^= cpu->regs.a.l;
    cpu->regs.a.l ^= cpu->regs.a.h;
    cpu->regs.p.flags.n = (cpu->regs.a.l & 0x80);
    cpu->regs.p.flags.z = (cpu->regs.a.l == 0);
}

void op_move_b(cpu_t* cpu, int adjust) {
    cpu->dp = op_readpc();
    sp = op_readpc();
    cpu->regs.db = cpu->dp;
    rd.l = op_readlong((sp << 16) | cpu->regs.x.w);
    op_writelong((cpu->dp << 16) | cpu->regs.y.w, rd.l);
    cpu->regs.x.l += adjust;
    cpu->regs.y.l += adjust;
    if(cpu->regs.a.w--) cpu->regs.pc.w -= 3;
}

void op_move_w(cpu_t* cpu, int adjust) {
    cpu->dp = op_readpc();
    sp = op_readpc();
    cpu->regs.db = cpu->dp;
    rd.l = op_readlong((sp << 16) | cpu->regs.x.w);
    op_writelong((cpu->dp << 16) | cpu->regs.y.w, rd.l);
    cpu->regs.x.w += adjust;
    cpu->regs.y.w += adjust;
    if(cpu->regs.a.w--) cpu->regs.pc.w -= 3;
}

void op_interrupt_e(cpu_t* cpu, int vectorE, int vectorN) {
    op_readpc();
    op_writestack(cpu->regs.pc.h);
    op_writestack(cpu->regs.pc.l);
    op_writestack(cpu->regs.p);
    rd.l = op_readlong(vectorE + 0);
    cpu->regs.pc.b = 0;
    cpu->regs.p.i = 1;
    cpu->regs.p.d = 0;
    rd.h = op_readlong(vectorE + 1);
    cpu->regs.pc.w = rd.w;
}

void op_interrupt_n(cpu_t* cpu, int vectorE, int vectorN) {
    op_readpc();
    op_writestack(cpu->regs.pc.b);
    op_writestack(cpu->regs.pc.h);
    op_writestack(cpu->regs.pc.l);
    op_writestack(cpu->regs.p);
    rd.l = op_readlong(vectorN + 0);
    cpu->regs.pc.b = 0x00;
    cpu->regs.p.i = 1;
    cpu->regs.p.d = 0;
    rd.h = op_readlong(vectorN + 1);
    cpu->regs.pc.w = rd.w;
}

void op_stp(cpu_t* cpu) {
    while(cpu->regs.wai = true) { }
    cpu->regs.wai = false;
}

void op_wai(cpu_t* cpu) {
    //cpu->regs.wai = true;
    //while(cpu->regs.wai) { }
    cpu->regs.wai = false;
}

void op_xce(cpu_t* cpu) {
    bool carry = cpu->regs.p.c;
    cpu->regs.p.c = cpu->regs.e;
    cpu->regs.e = carry;
    if(cpu->regs.e) {
        cpu->regs.p |= 0x30;
        cpu->regs.s.h = 0x01;
    }
    if(cpu->regs.p.x) {
        cpu->regs.x.h = 0x00;
        cpu->regs.y.h = 0x00;
    }
    update_table(cpu_t* cpu);
}

void op_flag(cpu_t* cpu, int mask, int value) {
    cpu->regs.p = (cpu->regs.p & ~mask) | value;
}

void op_pflag_e(cpu_t* cpu, int mode) {
    rd.l = op_readpc();
    cpu->regs.p = (mode ? cpu->regs.p | rd.l : cpu->regs.p & ~rd.l);
    cpu->regs.p |= 0x30;
    if(cpu->regs.p.x) {
        cpu->regs.x.h = 0x00;
        cpu->regs.y.h = 0x00;
    }
    update_table(cpu_t* cpu);
}

void op_pflag_n(cpu_t* cpu, int mode) {
    rd.l = op_readpc();
    cpu->regs.p = (mode ? cpu->regs.p | rd.l : cpu->regs.p & ~rd.l);
    if(cpu->regs.p.x) {
        cpu->regs.x.h = 0x00;
        cpu->regs.y.h = 0x00;
    }
    update_table(cpu_t* cpu);
}

void op_transfer_b(cpu_t* cpu, int from, int to) {
    cpu->regs.r[to].l = cpu->regs.r[from].l;
    cpu->regs.p.flags.n = (cpu->regs.r[to].l & 0x80);
    cpu->regs.p.flags.z = (cpu->regs.r[to].l == 0);
}

void op_transfer_w(cpu_t* cpu, int from, int to) {
    cpu->regs.r[to].w = cpu->regs.r[from].w;
    cpu->regs.p.flags.n = (cpu->regs.r[to].w & 0x8000);
    cpu->regs.p.flags.z = (cpu->regs.r[to].w == 0);
}

void op_tcs_e(cpu_t* cpu) {
    cpu->regs.s.l = cpu->regs.a.l;
}

void op_tcs_n(cpu_t* cpu) {
    cpu->regs.s.w = cpu->regs.a.w;
}

void op_tsx_b(cpu_t* cpu) {
    cpu->regs.x.l = cpu->regs.s.l;
    cpu->regs.p.flags.n = (cpu->regs.x.l & 0x80);
    cpu->regs.p.flags.z = (cpu->regs.x.l == 0);
}

void op_tsx_w(cpu_t* cpu) {
    cpu->regs.x.w = cpu->regs.s.w;
    cpu->regs.p.flags.n = (cpu->regs.x.w & 0x8000);
    cpu->regs.p.flags.z = (cpu->regs.x.w == 0);
}

void op_txs_e(cpu_t* cpu) {
    cpu->regs.s.l = cpu->regs.x.l;
}

void op_txs_n(cpu_t* cpu) {
    cpu->regs.s.w = cpu->regs.x.w;
}

void op_push_b(cpu_t* cpu, int n) {
    op_writestack(cpu->regs.r[n].l);
}

void op_push_w(cpu_t* cpu, int n) {
    op_writestack(cpu->regs.r[n].h);
    op_writestack(cpu->regs.r[n].l);
}

void op_phd_e(cpu_t* cpu) {
    op_writestackn(cpu->regs.d.h);
    op_writestackn(cpu->regs.d.l);
    cpu->regs.s.h = 0x01;
}

void op_phd_n(cpu_t* cpu) {
    op_writestackn(cpu->regs.d.h);
    op_writestackn(cpu->regs.d.l);
}

void op_phb(cpu_t* cpu) {
    op_writestack(cpu->regs.db);
}

void op_phk(cpu_t* cpu) {
    op_writestack(cpu->regs.pc.b);
}

void op_php(cpu_t* cpu) {
    op_writestack(cpu->regs.p);
}

void op_pull_b(cpu_t* cpu, int n) {
    cpu->regs.r[n].l = op_readstack(cpu_t* cpu);
    cpu->regs.p.flags.n = (cpu->regs.r[n].l & 0x80);
    cpu->regs.p.flags.z = (cpu->regs.r[n].l == 0);
}

void op_pull_w(cpu_t* cpu, int n) {
    cpu->regs.r[n].l = op_readstack(cpu_t* cpu);
    cpu->regs.r[n].h = op_readstack(cpu_t* cpu);
    cpu->regs.p.flags.n = (cpu->regs.r[n].w & 0x8000);
    cpu->regs.p.flags.z = (cpu->regs.r[n].w == 0);
}

void op_pld_e(cpu_t* cpu) {
    cpu->regs.d.l = op_readstackn(cpu_t* cpu);
    cpu->regs.d.h = op_readstackn(cpu_t* cpu);
    cpu->regs.p.flags.n = (cpu->regs.d.w & 0x8000);
    cpu->regs.p.flags.z = (cpu->regs.d.w == 0);
    cpu->regs.s.h = 0x01;
}

void op_pld_n(cpu_t* cpu) {
    cpu->regs.d.l = op_readstackn(cpu_t* cpu);
    cpu->regs.d.h = op_readstackn(cpu_t* cpu);
    cpu->regs.p.flags.n = (cpu->regs.d.w & 0x8000);
    cpu->regs.p.flags.z = (cpu->regs.d.w == 0);
}

void op_plb(cpu_t* cpu) {
    cpu->regs.db = op_readstack(cpu_t* cpu);
    cpu->regs.p.flags.n = (cpu->regs.db & 0x80);
    cpu->regs.p.flags.z = (cpu->regs.db == 0);
}

void op_plp_e(cpu_t* cpu) {
    cpu->regs.p = op_readstack(cpu_t* cpu) | 0x30;
    if(cpu->regs.p.x) {
        cpu->regs.x.h = 0x00;
        cpu->regs.y.h = 0x00;
    }
    update_table(cpu_t* cpu);
}

void op_plp_n(cpu_t* cpu) {
    cpu->regs.p = op_readstack(cpu_t* cpu);
    if(cpu->regs.p.x) {
        cpu->regs.x.h = 0x00;
        cpu->regs.y.h = 0x00;
    }
    update_table(cpu_t* cpu);
}

void op_pea_e(cpu_t* cpu) {
    aa.l = op_readpc();
    aa.h = op_readpc();
    op_writestackn(aa.h);
    op_writestackn(aa.l);
    cpu->regs.s.h = 0x01;
}

void op_pea_n(cpu_t* cpu) {
    aa.l = op_readpc();
    aa.h = op_readpc();
    op_writestackn(aa.h);
    op_writestackn(aa.l);
}

void op_pei_e(cpu_t* cpu) {
    cpu->dp = op_readpc();
    aa.l = op_readdp(cpu->dp + 0);
    aa.h = op_readdp(cpu->dp + 1);
    op_writestackn(aa.h);
    op_writestackn(aa.l);
    cpu->regs.s.h = 0x01;
}

void op_pei_n(cpu_t* cpu) {
    cpu->dp = op_readpc();
    aa.l = op_readdp(cpu->dp + 0);
    aa.h = op_readdp(cpu->dp + 1);
    op_writestackn(aa.h);
    op_writestackn(aa.l);
}

void op_per_e(cpu_t* cpu) {
    aa.l = op_readpc();
    aa.h = op_readpc();
    rd.w = cpu->regs.pc.d + (int16)aa.w;
    op_writestackn(rd.h);
    op_writestackn(rd.l);
    cpu->regs.s.h = 0x01;
}

void op_per_n(cpu_t* cpu) {
    aa.l = op_readpc();
    aa.h = op_readpc();
    rd.w = cpu->regs.pc.d + (int16)aa.w;
    op_writestackn(rd.h);
    op_writestackn(rd.l);
}
