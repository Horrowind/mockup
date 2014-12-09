#include "memory.h"
#include "table.h"

#define A 0
#define X 1
#define Y 2
#define Z 3
#define S 4
#define D 5

void r65816_op_nop(r65816_cpu_t* cpu) { }

void r65816_op_wdm(r65816_cpu_t* cpu) { }

void r65816_op_xba(r65816_cpu_t* cpu) {
    cpu->regs.a.l ^= cpu->regs.a.h;
    cpu->regs.a.h ^= cpu->regs.a.l;
    cpu->regs.a.l ^= cpu->regs.a.h;
    cpu->regs.p.n = (cpu->regs.a.l & 0x80);
    cpu->regs.p.z = (cpu->regs.a.l == 0);
}
#define r65816_op_move_gen(name, adjust)                                       \
    void r65816_op_move_##name##_b(r65816_cpu_t* cpu) {                               \
        cpu->dp = r65816_op_readpc(cpu);                                       \
        cpu->sp = r65816_op_readpc(cpu);                                       \
        cpu->regs.db = cpu->dp;                                         \
        cpu->rd.l = r65816_op_readlong(cpu,                                    \
                                (cpu->sp << 16) | cpu->regs.x.w);       \
        r65816_op_writelong(cpu,                                               \
                     (cpu->dp << 16) | cpu->regs.y.w, cpu->rd.l);       \
        cpu->regs.x.l += adjust;                                        \
        cpu->regs.y.l += adjust;                                        \
        if(cpu->regs.a.w--) cpu->regs.pc.w -= 3;                        \
    }                                                                   \
                                                                        \
    void r65816_op_move_##name##_w(r65816_cpu_t* cpu) {                               \
        cpu->dp = r65816_op_readpc(cpu);                                       \
        cpu->sp = r65816_op_readpc(cpu);                                       \
        cpu->regs.db = cpu->dp;                                         \
        cpu->rd.l = r65816_op_readlong(cpu, (cpu->sp << 16) | cpu->regs.x.w);  \
        r65816_op_writelong(cpu, (cpu->dp << 16) | cpu->regs.y.w, cpu->rd.l);  \
        cpu->regs.x.w += adjust;                                        \
        cpu->regs.y.w += adjust;                                        \
        if(cpu->regs.a.w--) cpu->regs.pc.w -= 3;                        \
    }                                                                   \
    
r65816_op_move_gen(mvp, (-1));
r65816_op_move_gen(mvn, (+1));
#undef r65816_op_move_gen

#define r65816_op_interrupt_gen(name, vectorE, vectorN)                   \
    void r65816_op_interrupt_##name##_e(r65816_cpu_t* cpu) {                     \
        r65816_op_readpc(cpu);                                            \
        r65816_op_writestack(cpu, cpu->regs.pc.h);                        \
        r65816_op_writestack(cpu, cpu->regs.pc.l);                        \
        r65816_op_writestack(cpu, cpu->regs.p.b);                         \
        cpu->rd.l = r65816_op_readlong(cpu, vectorE + 0);                 \
        cpu->regs.pc.b = 0;                                        \
        cpu->regs.p.i = 1;                                         \
        cpu->regs.p.d = 0;                                         \
        cpu->rd.h = r65816_op_readlong(cpu, vectorE + 1);                 \
        cpu->regs.pc.w = cpu->rd.w;                                \
    }                                                              \
                                                                   \
    void r65816_op_interrupt_##name##_n(r65816_cpu_t* cpu) {    \
        r65816_op_readpc(cpu);                                            \
        r65816_op_writestack(cpu, cpu->regs.pc.b);                        \
        r65816_op_writestack(cpu, cpu->regs.pc.h);                        \
        r65816_op_writestack(cpu, cpu->regs.pc.l);                        \
        r65816_op_writestack(cpu, cpu->regs.p.b);                         \
        cpu->rd.l = r65816_op_readlong(cpu, vectorN + 0);                 \
        cpu->regs.pc.b = 0x00;                                     \
        cpu->regs.p.i = 1;                                         \
        cpu->regs.p.d = 0;                                         \
        cpu->rd.h = r65816_op_readlong(cpu, vectorN + 1);                 \
        cpu->regs.pc.w = cpu->rd.w;                                \
    }                                                              \
    
r65816_op_interrupt_gen(brk, 0xfffe, 0xffe6);
r65816_op_interrupt_gen(cop, 0xfff4, 0xffe4);


void r65816_op_stp(r65816_cpu_t* cpu) {
    //while(cpu->regs.wai) { }
    //cpu->regs.wai = false;
}

void r65816_op_wai(r65816_cpu_t* cpu) {
    //cpu->regs.wai = true;
    //while(cpu->regs.wai) { }
    //cpu->regs.wai = false;
}

void r65816_op_xce(r65816_cpu_t* cpu) {
    uint8_t carry = cpu->regs.p.c;
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
    update_table(cpu);
}

#define r65816_op_flag_gen(name, mask, value)                          \
void r65816_op_flag_##name(r65816_cpu_t* cpu) {                               \
    cpu->regs.p.b = (cpu->regs.p.b & ~mask) | value;            \
}                                                               \

r65816_op_flag_gen(clc, 0x01, 0x00);
r65816_op_flag_gen(sec, 0x01, 0x01);
r65816_op_flag_gen(cli, 0x04, 0x00);
r65816_op_flag_gen(sli, 0x04, 0x04);
r65816_op_flag_gen(clv, 0x40, 0x00);
r65816_op_flag_gen(cld, 0x08, 0x00);
r65816_op_flag_gen(sed, 0x08, 0x08);
#undef r65816_op_flag_gen

#define r65816_op_pflag_gen(name, mode)                                        \
    void r65816_op_pflag_##name##_e(r65816_cpu_t* cpu) {                              \
    cpu->rd.l = r65816_op_readpc(cpu);                                         \
    cpu->regs.p.b = (mode ? cpu->regs.p.b | cpu->rd.l                   \
                     : cpu->regs.p.b & ~cpu->rd.l);                     \
    cpu->regs.p.b |= 0x30;                                              \
    if(cpu->regs.p.x) {                                                 \
        cpu->regs.x.h = 0x00;                                           \
        cpu->regs.y.h = 0x00;                                           \
    }                                                                   \
    update_table(cpu);                                                  \
    }                                                                   \
                                                                        \
    void r65816_op_pflag_##name##_n(r65816_cpu_t* cpu) {                              \
        cpu->rd.l = r65816_op_readpc(cpu);                                     \
        cpu->regs.p.b = (mode ? cpu->regs.p.b | cpu->rd.l               \
                         : cpu->regs.p.b & ~cpu->rd.l);                 \
        if(cpu->regs.p.x) {                                             \
            cpu->regs.x.h = 0x00;                                       \
            cpu->regs.y.h = 0x00;                                       \
        }                                                               \
        update_table(cpu);                                              \
    }                                                                   

r65816_op_pflag_gen(rep, 0);
r65816_op_pflag_gen(sep, 1);
#undef r65816_op_pflag_gen

#define r65816_op_transfer_gen(name, from, to)                          \
    void r65816_op_transfer_##name##_b(r65816_cpu_t* cpu) {                    \
        cpu->regs.r[to].l = cpu->regs.r[from].l;                 \
        cpu->regs.p.n = (cpu->regs.r[to].l & 0x80);              \
        cpu->regs.p.z = (cpu->regs.r[to].l == 0);                \
    }                                                            \
                                                                 \
    void r65816_op_transfer_##name##_w(r65816_cpu_t* cpu) {                    \
        cpu->regs.r[to].w = cpu->regs.r[from].w;                 \
        cpu->regs.p.n = (cpu->regs.r[to].w & 0x8000);            \
        cpu->regs.p.z = (cpu->regs.r[to].w == 0);                \
    }



r65816_op_transfer_gen(tsc, S, A);
r65816_op_transfer_gen(tcd, A, D);
r65816_op_transfer_gen(tdc, D, A);

r65816_op_transfer_gen(txa, X, A);
r65816_op_transfer_gen(tya, Y, A);
r65816_op_transfer_gen(txy, X, Y);
r65816_op_transfer_gen(tay, A, Y);
r65816_op_transfer_gen(tax, A, X);
r65816_op_transfer_gen(tyx, Y, X);
#undef r65816_op_transfer_gen

void r65816_op_tcs_e(r65816_cpu_t* cpu) {
    cpu->regs.s.l = cpu->regs.a.l;
}

void r65816_op_tcs_n(r65816_cpu_t* cpu) {
    cpu->regs.s.w = cpu->regs.a.w;
}

void r65816_op_tsx_b(r65816_cpu_t* cpu) {
    cpu->regs.x.l = cpu->regs.s.l;
    cpu->regs.p.n = (cpu->regs.x.l & 0x80);
    cpu->regs.p.z = (cpu->regs.x.l == 0);
}

void r65816_op_tsx_w(r65816_cpu_t* cpu) {
    cpu->regs.x.w = cpu->regs.s.w;
    cpu->regs.p.n = (cpu->regs.x.w & 0x8000);
    cpu->regs.p.z = (cpu->regs.x.w == 0);
}

void r65816_op_txs_e(r65816_cpu_t* cpu) {
    cpu->regs.s.l = cpu->regs.x.l;
}

void r65816_op_txs_n(r65816_cpu_t* cpu) {
    cpu->regs.s.w = cpu->regs.x.w;
}
#define r65816_op_push_gen(name, n)                             \
    void r65816_op_push_##name##_b(r65816_cpu_t* cpu) {                \
        r65816_op_writestack(cpu, cpu->regs.r[n].l);            \
    }                                                    \
                                                         \
    void r65816_op_push_##name##_w(r65816_cpu_t* cpu) {                \
        r65816_op_writestack(cpu, cpu->regs.r[n].h);            \
        r65816_op_writestack(cpu, cpu->regs.r[n].l);            \
    }                                           

r65816_op_push_gen(pha, A);
r65816_op_push_gen(phx, X);
r65816_op_push_gen(phy, Y);
#undef r65816_op_push_gen

void r65816_op_phd_e(r65816_cpu_t* cpu) {
    r65816_op_writestackn(cpu, cpu->regs.d.h);
    r65816_op_writestackn(cpu, cpu->regs.d.l);
    cpu->regs.s.h = 0x01;
}

void r65816_op_phd_n(r65816_cpu_t* cpu) {
    r65816_op_writestackn(cpu, cpu->regs.d.h);
    r65816_op_writestackn(cpu, cpu->regs.d.l);
}

void r65816_op_phb(r65816_cpu_t* cpu) {
    r65816_op_writestack(cpu, cpu->regs.db);
}

void r65816_op_phk(r65816_cpu_t* cpu) {
    r65816_op_writestack(cpu, cpu->regs.pc.b);
}

void r65816_op_php(r65816_cpu_t* cpu) {
    r65816_op_writestack(cpu, cpu->regs.p.b);
}

#define r65816_op_pull_gen(name, num)                          \
    void r65816_op_pull_##name##_b(r65816_cpu_t* cpu) {               \
        cpu->regs.r[num].l = r65816_op_readstack(cpu);         \
        cpu->regs.p.n = (cpu->regs.r[num].l & 0x80);    \
        cpu->regs.p.z = (cpu->regs.r[num].l == 0);      \
    }                                                   \
                                                        \
    void r65816_op_pull_##name##_w(r65816_cpu_t* cpu) {               \
        cpu->regs.r[num].l = r65816_op_readstack(cpu);         \
        cpu->regs.r[num].h = r65816_op_readstack(cpu);         \
        cpu->regs.p.n = (cpu->regs.r[num].w & 0x8000);  \
        cpu->regs.p.z = (cpu->regs.r[num].w == 0);      \
    }

r65816_op_pull_gen(pla, A);
r65816_op_pull_gen(plx, X);
r65816_op_pull_gen(ply, Y);

#undef r65816_op_push_gen

void r65816_op_pld_e(r65816_cpu_t* cpu) {
    cpu->regs.d.l = r65816_op_readstackn(cpu);
    cpu->regs.d.h = r65816_op_readstackn(cpu);
    cpu->regs.p.n = (cpu->regs.d.w & 0x8000);
    cpu->regs.p.z = (cpu->regs.d.w == 0);
    cpu->regs.s.h = 0x01;
}

void r65816_op_pld_n(r65816_cpu_t* cpu) {
    cpu->regs.d.l = r65816_op_readstackn(cpu);
    cpu->regs.d.h = r65816_op_readstackn(cpu);
    cpu->regs.p.n = (cpu->regs.d.w & 0x8000);
    cpu->regs.p.z = (cpu->regs.d.w == 0);
}

void r65816_op_plb(r65816_cpu_t* cpu) {
    cpu->regs.db = r65816_op_readstack(cpu);
    cpu->regs.p.n = (cpu->regs.db & 0x80);
    cpu->regs.p.z = (cpu->regs.db == 0);
}

void r65816_op_plp_e(r65816_cpu_t* cpu) {
    cpu->regs.p.b = r65816_op_readstack(cpu) | 0x30;
    if(cpu->regs.p.x) {
        cpu->regs.x.h = 0x00;
        cpu->regs.y.h = 0x00;
    }
    update_table(cpu);
}

void r65816_op_plp_n(r65816_cpu_t* cpu) {
    cpu->regs.p.b = r65816_op_readstack(cpu);
    if(cpu->regs.p.x) {
        cpu->regs.x.h = 0x00;
        cpu->regs.y.h = 0x00;
    }
    update_table(cpu);
}

void r65816_op_pea_e(r65816_cpu_t* cpu) {
    cpu->aa.l = r65816_op_readpc(cpu);
    cpu->aa.h = r65816_op_readpc(cpu);
    r65816_op_writestackn(cpu, cpu->aa.h);
    r65816_op_writestackn(cpu, cpu->aa.l);
    cpu->regs.s.h = 0x01;
}

void r65816_op_pea_n(r65816_cpu_t* cpu) {
    cpu->aa.l = r65816_op_readpc(cpu);
    cpu->aa.h = r65816_op_readpc(cpu);
    r65816_op_writestackn(cpu, cpu->aa.h);
    r65816_op_writestackn(cpu, cpu->aa.l);
}

void r65816_op_pei_e(r65816_cpu_t* cpu) {
    cpu->dp = r65816_op_readpc(cpu);
    cpu->aa.l = r65816_op_readdp(cpu, cpu->dp + 0);
    cpu->aa.h = r65816_op_readdp(cpu, cpu->dp + 1);
    r65816_op_writestackn(cpu, cpu->aa.h);
    r65816_op_writestackn(cpu, cpu->aa.l);
    cpu->regs.s.h = 0x01;
}

void r65816_op_pei_n(r65816_cpu_t* cpu) {
    cpu->dp = r65816_op_readpc(cpu);
    cpu->aa.l = r65816_op_readdp(cpu, cpu->dp + 0);
    cpu->aa.h = r65816_op_readdp(cpu, cpu->dp + 1);
    r65816_op_writestackn(cpu, cpu->aa.h);
    r65816_op_writestackn(cpu, cpu->aa.l);
}

void r65816_op_per_e(r65816_cpu_t* cpu) {
    cpu->aa.l = r65816_op_readpc(cpu);
    cpu->aa.h = r65816_op_readpc(cpu);
    cpu->rd.w = cpu->regs.pc.d + (int16_t)cpu->aa.w;
    r65816_op_writestackn(cpu, cpu->rd.h);
    r65816_op_writestackn(cpu, cpu->rd.l);
    cpu->regs.s.h = 0x01;
}

void r65816_op_per_n(r65816_cpu_t* cpu) {
    cpu->aa.l = r65816_op_readpc(cpu);
    cpu->aa.h = r65816_op_readpc(cpu);
    cpu->rd.w = cpu->regs.pc.d + (int16_t)cpu->aa.w;
    r65816_op_writestackn(cpu, cpu->rd.h);
    r65816_op_writestackn(cpu, cpu->rd.l);
}

#undef A
#undef X
#undef Y
#undef Z
#undef S
#undef D
