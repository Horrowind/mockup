#include "algorithms.h"
#include "memory.h"
#include "table.h"

#include "opcode_misc.h"

#define A 0
#define X 1
#define Y 2
#define Z 3
#define S 4
#define D 5

void wdc65816_op_nop(Wdc65816Cpu* cpu) { (void)(cpu); }

void wdc65816_op_wdm(Wdc65816Cpu* cpu) {
    wdc65816_op_readpc(cpu);
}

void wdc65816_op_xba(Wdc65816Cpu* cpu) {
    cpu->regs.a.l ^= cpu->regs.a.h;
    cpu->regs.a.h ^= cpu->regs.a.l;
    cpu->regs.a.l ^= cpu->regs.a.h;
    cpu->regs.p.n = 1 && (cpu->regs.a.l & 0x80);
    cpu->regs.p.z = (cpu->regs.a.l == 0);
}
#define wdc65816_op_move_gen(name, adjust)                                \
    void wdc65816_op_move_##name##_b(Wdc65816Cpu* cpu) {                 \
        cpu->dp = wdc65816_op_readpc(cpu);                                \
        cpu->sp = wdc65816_op_readpc(cpu);                                \
        cpu->regs.db = cpu->dp;                                         \
        cpu->rd.l = wdc65816_op_readlong(cpu, (cpu->sp << 16) | cpu->regs.x.w); \
        wdc65816_op_writelong(cpu, (cpu->dp << 16) | cpu->regs.y.w, cpu->rd.l); \
        cpu->regs.x.l += adjust;                                        \
        cpu->regs.y.l += adjust;                                        \
        if(cpu->regs.a.w--) cpu->regs.pc.w -= 3;                        \
    }                                                                   \
                                                                        \
    void wdc65816_op_move_##name##_w(Wdc65816Cpu* cpu) {                 \
        cpu->dp = wdc65816_op_readpc(cpu);                                \
        cpu->sp = wdc65816_op_readpc(cpu);                                \
        cpu->regs.db = cpu->dp;                                         \
        cpu->rd.l = wdc65816_op_readlong(cpu, (cpu->sp << 16) | cpu->regs.x.w); \
        wdc65816_op_writelong(cpu, (cpu->dp << 16) | cpu->regs.y.w, cpu->rd.l); \
        cpu->regs.x.w += adjust;                                        \
        cpu->regs.y.w += adjust;                                        \
        if(cpu->regs.a.w--) cpu->regs.pc.w -= 3;                        \
    }                                                                   \
    
wdc65816_op_move_gen(mvp, (-1));
wdc65816_op_move_gen(mvn, (+1));
#undef wdc65816_op_move_gen

#define wdc65816_op_interrupt_gen(name, vectorE, vectorN)         \
    void wdc65816_op_interrupt_##name##_e(Wdc65816Cpu* cpu) {    \
        wdc65816_op_readpc(cpu);                                  \
        wdc65816_op_writestack(cpu, cpu->regs.pc.h);              \
        wdc65816_op_writestack(cpu, cpu->regs.pc.l);              \
        wdc65816_op_writestack(cpu, cpu->regs.p.b);               \
        cpu->rd.l = wdc65816_op_readlong(cpu, vectorE + 0);       \
        cpu->regs.pc.b = 0;                                     \
        cpu->regs.p.i = 1;                                      \
        cpu->regs.p.d = 0;                                      \
        cpu->rd.h = wdc65816_op_readlong(cpu, vectorE + 1);       \
        cpu->regs.pc.w = cpu->rd.w;                             \
    }                                                           \
                                                                \
    void wdc65816_op_interrupt_##name##_n(Wdc65816Cpu* cpu) {    \
        wdc65816_op_readpc(cpu);                                  \
        wdc65816_op_writestack(cpu, cpu->regs.pc.b);              \
        wdc65816_op_writestack(cpu, cpu->regs.pc.h);              \
        wdc65816_op_writestack(cpu, cpu->regs.pc.l);              \
        wdc65816_op_writestack(cpu, cpu->regs.p.b);               \
        cpu->rd.l = wdc65816_op_readlong(cpu, vectorN + 0);       \
        cpu->regs.pc.b = 0x00;                                  \
        cpu->regs.p.i = 1;                                      \
        cpu->regs.p.d = 0;                                      \
        cpu->rd.h = wdc65816_op_readlong(cpu, vectorN + 1);       \
        cpu->regs.pc.w = cpu->rd.w;                             \
    }                                                           \
    
wdc65816_op_interrupt_gen(brk, 0xfffe, 0xffe6);
wdc65816_op_interrupt_gen(cop, 0xfff4, 0xffe4);


void wdc65816_op_stp(Wdc65816Cpu* cpu) {
    (void)(cpu);
}

void wdc65816_op_wai(Wdc65816Cpu* cpu) {
    (void)(cpu);
}

void wdc65816_op_xce(Wdc65816Cpu* cpu) {
    u8 carry = cpu->regs.p.c;
    cpu->regs.p.c = cpu->regs.e;
    cpu->regs.e = carry;
    if(cpu->regs.e) {
        cpu->regs.p.b |= 0x30;
        cpu->regs.s.h = 0x01;
    }
    if(cpu->regs.p.x) {
        cpu->regs.x.h = 0x00;
        cpu->regs.y.h = 0x00;
    }
}

#define wdc65816_op_flag_gen(name, mask, value)               \
    void wdc65816_op_flag_##name(Wdc65816Cpu* cpu) {         \
        cpu->regs.p.b = (cpu->regs.p.b & ~mask) | value;    \
    }

wdc65816_op_flag_gen(clc, 0x01, 0x00);
wdc65816_op_flag_gen(sec, 0x01, 0x01);
wdc65816_op_flag_gen(cli, 0x04, 0x00);
wdc65816_op_flag_gen(sli, 0x04, 0x04);
wdc65816_op_flag_gen(clv, 0x40, 0x00);
wdc65816_op_flag_gen(cld, 0x08, 0x00);
wdc65816_op_flag_gen(sed, 0x08, 0x08);
#undef wdc65816_op_flag_gen

#define wdc65816_op_pflag_gen(name, mode)                     \
    void wdc65816_op_pflag_##name##_e(Wdc65816Cpu* cpu) {    \
        cpu->rd.l = wdc65816_op_readpc(cpu);                  \
        cpu->regs.p.b = mode;                               \
        cpu->regs.p.b |= 0x30;                              \
        if(cpu->regs.p.x) {                                 \
            cpu->regs.x.h = 0x00;                           \
            cpu->regs.y.h = 0x00;                           \
        }                                                   \
    }                                                       \
                                                            \
    void wdc65816_op_pflag_##name##_n(Wdc65816Cpu* cpu) {    \
        cpu->rd.l = wdc65816_op_readpc(cpu);                  \
        cpu->regs.p.b = mode;                               \
        if(cpu->regs.p.x) {                                 \
            cpu->regs.x.h = 0x00;                           \
            cpu->regs.y.h = 0x00;                           \
        }                                                   \
    }

wdc65816_op_pflag_gen(rep, cpu->regs.p.b & ~cpu->rd.l);
wdc65816_op_pflag_gen(sep, cpu->regs.p.b | cpu->rd.l);
#undef wdc65816_op_pflag_gen

#define wdc65816_op_transfer_gen(name, from, to)              \
    void wdc65816_op_transfer_##name##_b(Wdc65816Cpu* cpu) { \
        cpu->regs.r[to].l = cpu->regs.r[from].l;            \
        cpu->regs.p.n = 1 && (cpu->regs.r[to].l & 0x80);    \
        cpu->regs.p.z = (cpu->regs.r[to].l == 0);           \
    }                                                       \
                                                            \
    void wdc65816_op_transfer_##name##_w(Wdc65816Cpu* cpu) { \
        cpu->regs.r[to].w = cpu->regs.r[from].w;            \
        cpu->regs.p.n = 1 && (cpu->regs.r[to].w & 0x8000);  \
        cpu->regs.p.z = (cpu->regs.r[to].w == 0);           \
    }



wdc65816_op_transfer_gen(tsc, S, A);
wdc65816_op_transfer_gen(tcd, A, D);
wdc65816_op_transfer_gen(tdc, D, A);

wdc65816_op_transfer_gen(txa, X, A);
wdc65816_op_transfer_gen(tya, Y, A);
wdc65816_op_transfer_gen(txy, X, Y);
wdc65816_op_transfer_gen(tay, A, Y);
wdc65816_op_transfer_gen(tax, A, X);
wdc65816_op_transfer_gen(tyx, Y, X);
#undef wdc65816_op_transfer_gen

void wdc65816_op_tcs_e(Wdc65816Cpu* cpu) {
    cpu->regs.s.l = cpu->regs.a.l;
}

void wdc65816_op_tcs_n(Wdc65816Cpu* cpu) {
    cpu->regs.s.w = cpu->regs.a.w;
}

void wdc65816_op_tsx_b(Wdc65816Cpu* cpu) {
    cpu->regs.x.l = cpu->regs.s.l;
    cpu->regs.p.n = 1 && (cpu->regs.x.l & 0x80);
    cpu->regs.p.z = (cpu->regs.x.l == 0);
}

void wdc65816_op_tsx_w(Wdc65816Cpu* cpu) {
    cpu->regs.x.w = cpu->regs.s.w;
    cpu->regs.p.n = 1 && (cpu->regs.x.w & 0x8000);
    cpu->regs.p.z = (cpu->regs.x.w == 0);
}

void wdc65816_op_txs_e(Wdc65816Cpu* cpu) {
    cpu->regs.s.l = cpu->regs.x.l;
}

void wdc65816_op_txs_n(Wdc65816Cpu* cpu) {
    cpu->regs.s.w = cpu->regs.x.w;
}

#define wdc65816_op_push_gen(name, n)                                     \
    void wdc65816_op_push_##name##_b(Wdc65816Cpu* cpu) {                 \
        wdc65816_op_writestack(cpu, cpu->regs.r[n].l);                    \
    }                                                                   \
                                                                        \
    void wdc65816_op_push_##name##_w(Wdc65816Cpu* cpu) {                 \
        wdc65816_op_writestack(cpu, cpu->regs.r[n].h);                    \
        wdc65816_op_writestack(cpu, cpu->regs.r[n].l);                    \
    }                                           

wdc65816_op_push_gen(pha, A);
wdc65816_op_push_gen(phx, X);
wdc65816_op_push_gen(phy, Y);
#undef wdc65816_op_push_gen

void wdc65816_op_phd_e(Wdc65816Cpu* cpu) {
    wdc65816_op_writestackn(cpu, cpu->regs.d.h);
    wdc65816_op_writestackn(cpu, cpu->regs.d.l);
    cpu->regs.s.h = 0x01;
}

void wdc65816_op_phd_n(Wdc65816Cpu* cpu) {
    wdc65816_op_writestackn(cpu, cpu->regs.d.h);
    wdc65816_op_writestackn(cpu, cpu->regs.d.l);
}

void wdc65816_op_phb(Wdc65816Cpu* cpu) {
    wdc65816_op_writestack(cpu, cpu->regs.db);
}

void wdc65816_op_phk(Wdc65816Cpu* cpu) {
    wdc65816_op_writestack(cpu, cpu->regs.pc.b);
}

void wdc65816_op_php(Wdc65816Cpu* cpu) {
    wdc65816_op_writestack(cpu, cpu->regs.p.b);
}

#define wdc65816_op_pull_gen(name, num)                       \
    void wdc65816_op_pull_##name##_b(Wdc65816Cpu* cpu) {     \
        cpu->regs.r[num].l = wdc65816_op_readstack(cpu);      \
        cpu->regs.p.n = 1 && (cpu->regs.r[num].l & 0x80);   \
        cpu->regs.p.z = (cpu->regs.r[num].l == 0);          \
    }                                                       \
                                                            \
    void wdc65816_op_pull_##name##_w(Wdc65816Cpu* cpu) {     \
        cpu->regs.r[num].l = wdc65816_op_readstack(cpu);      \
        cpu->regs.r[num].h = wdc65816_op_readstack(cpu);      \
        cpu->regs.p.n = 1 && (cpu->regs.r[num].w & 0x8000); \
        cpu->regs.p.z = (cpu->regs.r[num].w == 0);          \
    }

wdc65816_op_pull_gen(pla, A);
wdc65816_op_pull_gen(plx, X);
wdc65816_op_pull_gen(ply, Y);

#undef wdc65816_op_push_gen

void wdc65816_op_pld_e(Wdc65816Cpu* cpu) {
    cpu->regs.d.l = wdc65816_op_readstackn(cpu);
    cpu->regs.d.h = wdc65816_op_readstackn(cpu);
    cpu->regs.p.n = 1 && (cpu->regs.d.w & 0x8000);
    cpu->regs.p.z = (cpu->regs.d.w == 0);
    cpu->regs.s.h = 0x01;
}

void wdc65816_op_pld_n(Wdc65816Cpu* cpu) {
    cpu->regs.d.l = wdc65816_op_readstackn(cpu);
    cpu->regs.d.h = wdc65816_op_readstackn(cpu);
    cpu->regs.p.n = 1 && (cpu->regs.d.w & 0x8000);
    cpu->regs.p.z = (cpu->regs.d.w == 0);
}

void wdc65816_op_plb(Wdc65816Cpu* cpu) {
    cpu->regs.db = wdc65816_op_readstack(cpu);
    cpu->regs.p.n = 1 && (cpu->regs.db & 0x80);
    cpu->regs.p.z = (cpu->regs.db == 0);
}

void wdc65816_op_plp_e(Wdc65816Cpu* cpu) {
    cpu->regs.p.b = wdc65816_op_readstack(cpu) | 0x30;
    if(cpu->regs.p.x) {
        cpu->regs.x.h = 0x00;
        cpu->regs.y.h = 0x00;
    }
}

void wdc65816_op_plp_n(Wdc65816Cpu* cpu) {
    cpu->regs.p.b = wdc65816_op_readstack(cpu);
    if(cpu->regs.p.x) {
        cpu->regs.x.h = 0x00;
        cpu->regs.y.h = 0x00;
    }
}

void wdc65816_op_pea_e(Wdc65816Cpu* cpu) {
    cpu->aa.l = wdc65816_op_readpc(cpu);
    cpu->aa.h = wdc65816_op_readpc(cpu);
    wdc65816_op_writestackn(cpu, cpu->aa.h);
    wdc65816_op_writestackn(cpu, cpu->aa.l);
    cpu->regs.s.h = 0x01;
}

void wdc65816_op_pea_n(Wdc65816Cpu* cpu) {
    cpu->aa.l = wdc65816_op_readpc(cpu);
    cpu->aa.h = wdc65816_op_readpc(cpu);
    wdc65816_op_writestackn(cpu, cpu->aa.h);
    wdc65816_op_writestackn(cpu, cpu->aa.l);
}

void wdc65816_op_pei_e(Wdc65816Cpu* cpu) {
    cpu->dp = wdc65816_op_readpc(cpu);
    cpu->aa.l = wdc65816_op_readdp(cpu, cpu->dp + 0);
    cpu->aa.h = wdc65816_op_readdp(cpu, cpu->dp + 1);
    wdc65816_op_writestackn(cpu, cpu->aa.h);
    wdc65816_op_writestackn(cpu, cpu->aa.l);
    cpu->regs.s.h = 0x01;
}

void wdc65816_op_pei_n(Wdc65816Cpu* cpu) {
    cpu->dp = wdc65816_op_readpc(cpu);
    cpu->aa.l = wdc65816_op_readdp(cpu, cpu->dp + 0);
    cpu->aa.h = wdc65816_op_readdp(cpu, cpu->dp + 1);
    wdc65816_op_writestackn(cpu, cpu->aa.h);
    wdc65816_op_writestackn(cpu, cpu->aa.l);
}

void wdc65816_op_per_e(Wdc65816Cpu* cpu) {
    cpu->aa.l = wdc65816_op_readpc(cpu);
    cpu->aa.h = wdc65816_op_readpc(cpu);
    cpu->rd.w = cpu->regs.pc.d + (i16)cpu->aa.w;
    wdc65816_op_writestackn(cpu, cpu->rd.h);
    wdc65816_op_writestackn(cpu, cpu->rd.l);
    cpu->regs.s.h = 0x01;
}

void wdc65816_op_per_n(Wdc65816Cpu* cpu) {
    cpu->aa.l = wdc65816_op_readpc(cpu);
    cpu->aa.h = wdc65816_op_readpc(cpu);
    cpu->rd.w = cpu->regs.pc.d + (i16)cpu->aa.w;
    wdc65816_op_writestackn(cpu, cpu->rd.h);
    wdc65816_op_writestackn(cpu, cpu->rd.l);
}

#undef A
#undef X
#undef Y
#undef Z
#undef S
#undef D
