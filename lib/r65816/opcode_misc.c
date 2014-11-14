#include "memory.h"
#include "opcode_misc.h"
#include "table.h"

#define A 0
#define X 1
#define Y 2
#define Z 3
#define S 4
#define D 5

void op_nop(cpu_t* cpu) { }

void op_wdm(cpu_t* cpu) { }

void op_xba(cpu_t* cpu) {
    cpu->regs.a.l ^= cpu->regs.a.h;
    cpu->regs.a.h ^= cpu->regs.a.l;
    cpu->regs.a.l ^= cpu->regs.a.h;
    cpu->regs.p.n = (cpu->regs.a.l & 0x80);
    cpu->regs.p.z = (cpu->regs.a.l == 0);
}
#define op_move_gen(name, adjust)                                       \
    void op_move_##name##_b(cpu_t* cpu) {                               \
        cpu->dp = op_readpc(cpu);                                       \
        cpu->sp = op_readpc(cpu);                                       \
        cpu->regs.db = cpu->dp;                                         \
        cpu->rd.l = op_readlong(cpu,                                    \
                                (cpu->sp << 16) | cpu->regs.x.w);       \
        op_writelong(cpu,                                               \
                     (cpu->dp << 16) | cpu->regs.y.w, cpu->rd.l);       \
        cpu->regs.x.l += adjust;                                        \
        cpu->regs.y.l += adjust;                                        \
        if(cpu->regs.a.w--) cpu->regs.pc.w -= 3;                        \
    }                                                                   \
                                                                        \
    void op_move_##name##_w(cpu_t* cpu) {                               \
        cpu->dp = op_readpc(cpu);                                       \
        cpu->sp = op_readpc(cpu);                                       \
        cpu->regs.db = cpu->dp;                                         \
        cpu->rd.l = op_readlong(cpu, (cpu->sp << 16) | cpu->regs.x.w);  \
        op_writelong(cpu, (cpu->dp << 16) | cpu->regs.y.w, cpu->rd.l);  \
        cpu->regs.x.w += adjust;                                        \
        cpu->regs.y.w += adjust;                                        \
        if(cpu->regs.a.w--) cpu->regs.pc.w -= 3;                        \
    }                                                                   \
    
op_move_gen(mvp, (-1));
op_move_gen(mvn, (+1));
#undef op_move_gen

#define op_interrupt_gen(name, vectorE, vectorN)                   \
    void op_interrupt_##name##_e(cpu_t* cpu) {                     \
        op_readpc(cpu);                                            \
        op_writestack(cpu, cpu->regs.pc.h);                        \
        op_writestack(cpu, cpu->regs.pc.l);                        \
        op_writestack(cpu, cpu->regs.p.b);                         \
        cpu->rd.l = op_readlong(cpu, vectorE + 0);                 \
        cpu->regs.pc.b = 0;                                        \
        cpu->regs.p.i = 1;                                         \
        cpu->regs.p.d = 0;                                         \
        cpu->rd.h = op_readlong(cpu, vectorE + 1);                 \
        cpu->regs.pc.w = cpu->rd.w;                                \
    }                                                              \
                                                                   \
    void op_interrupt_##name##_n(cpu_t* cpu) {    \
        op_readpc(cpu);                                            \
        op_writestack(cpu, cpu->regs.pc.b);                        \
        op_writestack(cpu, cpu->regs.pc.h);                        \
        op_writestack(cpu, cpu->regs.pc.l);                        \
        op_writestack(cpu, cpu->regs.p.b);                         \
        cpu->rd.l = op_readlong(cpu, vectorN + 0);                 \
        cpu->regs.pc.b = 0x00;                                     \
        cpu->regs.p.i = 1;                                         \
        cpu->regs.p.d = 0;                                         \
        cpu->rd.h = op_readlong(cpu, vectorN + 1);                 \
        cpu->regs.pc.w = cpu->rd.w;                                \
    }                                                              \
    
op_interrupt_gen(brk, 0xfffe, 0xffe6);
op_interrupt_gen(cop, 0xfff4, 0xffe4);


void op_stp(cpu_t* cpu) {
    //while(cpu->regs.wai) { }
    //cpu->regs.wai = false;
}

void op_wai(cpu_t* cpu) {
    //cpu->regs.wai = true;
    //while(cpu->regs.wai) { }
    //cpu->regs.wai = false;
}

void op_xce(cpu_t* cpu) {
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

#define op_flag_gen(name, mask, value)                          \
void op_flag_##name(cpu_t* cpu) {                               \
    cpu->regs.p.b = (cpu->regs.p.b & ~mask) | value;            \
}                                                               \

op_flag_gen(CLC, 0x01, 0x00);
op_flag_gen(SEC, 0x01, 0x01);
op_flag_gen(CLI, 0x04, 0x00);
op_flag_gen(SLI, 0x04, 0x04);
op_flag_gen(CLV, 0x40, 0x00);
op_flag_gen(CLD, 0x08, 0x00);
op_flag_gen(SED, 0x08, 0x08);
#undef op_flag_gen

#define op_pflag_gen(name, mode)                                        \
    void op_pflag_##name##_e(cpu_t* cpu) {                              \
    cpu->rd.l = op_readpc(cpu);                                         \
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
    void op_pflag_##name##_n(cpu_t* cpu) {                              \
        cpu->rd.l = op_readpc(cpu);                                     \
        cpu->regs.p.b = (mode ? cpu->regs.p.b | cpu->rd.l               \
                         : cpu->regs.p.b & ~cpu->rd.l);                 \
        if(cpu->regs.p.x) {                                             \
            cpu->regs.x.h = 0x00;                                       \
            cpu->regs.y.h = 0x00;                                       \
        }                                                               \
        update_table(cpu);                                              \
    }                                                                   

op_pflag_gen(REP, 0);
op_pflag_gen(SEP, 1);
#undef op_pflag_gen

#define op_transfer_gen(name, from, to)                          \
    void op_transfer_##name##_b(cpu_t* cpu) {                    \
        cpu->regs.r[to].l = cpu->regs.r[from].l;                 \
        cpu->regs.p.n = (cpu->regs.r[to].l & 0x80);              \
        cpu->regs.p.z = (cpu->regs.r[to].l == 0);                \
    }                                                            \
                                                                 \
    void op_transfer_##name##_w(cpu_t* cpu) {                    \
        cpu->regs.r[to].w = cpu->regs.r[from].w;                 \
        cpu->regs.p.n = (cpu->regs.r[to].w & 0x8000);            \
        cpu->regs.p.z = (cpu->regs.r[to].w == 0);                \
    }



op_transfer_gen(tsc, S, A);
op_transfer_gen(tcd, A, D);
op_transfer_gen(tdc, D, A);

op_transfer_gen(txa, X, A);
op_transfer_gen(tya, Y, A);
op_transfer_gen(txy, X, Y);
op_transfer_gen(tay, A, Y);
op_transfer_gen(tax, A, X);
op_transfer_gen(tyx, Y, X);
#undef op_transfer_gen

void op_tcs_e(cpu_t* cpu) {
    cpu->regs.s.l = cpu->regs.a.l;
}

void op_tcs_n(cpu_t* cpu) {
    cpu->regs.s.w = cpu->regs.a.w;
}

void op_tsx_b(cpu_t* cpu) {
    cpu->regs.x.l = cpu->regs.s.l;
    cpu->regs.p.n = (cpu->regs.x.l & 0x80);
    cpu->regs.p.z = (cpu->regs.x.l == 0);
}

void op_tsx_w(cpu_t* cpu) {
    cpu->regs.x.w = cpu->regs.s.w;
    cpu->regs.p.n = (cpu->regs.x.w & 0x8000);
    cpu->regs.p.z = (cpu->regs.x.w == 0);
}

void op_txs_e(cpu_t* cpu) {
    cpu->regs.s.l = cpu->regs.x.l;
}

void op_txs_n(cpu_t* cpu) {
    cpu->regs.s.w = cpu->regs.x.w;
}
#define op_push_gen(name, n)                             \
    void op_push_##name##_b(cpu_t* cpu) {                \
        op_writestack(cpu, cpu->regs.r[n].l);            \
    }                                                    \
                                                         \
    void op_push_##name##_w(cpu_t* cpu) {                \
        op_writestack(cpu, cpu->regs.r[n].h);            \
        op_writestack(cpu, cpu->regs.r[n].l);            \
    }                                           

op_push_gen(PHA, A);
op_push_gen(PHX, X);
op_push_gen(PHY, Y);
#undef op_push_gen

void op_phd_e(cpu_t* cpu) {
    op_writestackn(cpu, cpu->regs.d.h);
    op_writestackn(cpu, cpu->regs.d.l);
    cpu->regs.s.h = 0x01;
}

void op_phd_n(cpu_t* cpu) {
    op_writestackn(cpu, cpu->regs.d.h);
    op_writestackn(cpu, cpu->regs.d.l);
}

void op_phb(cpu_t* cpu) {
    op_writestack(cpu, cpu->regs.db);
}

void op_phk(cpu_t* cpu) {
    op_writestack(cpu, cpu->regs.pc.b);
}

void op_php(cpu_t* cpu) {
    op_writestack(cpu, cpu->regs.p.b);
}

#define op_pull_gen(name, num)                          \
    void op_pull_##name##_b(cpu_t* cpu) {               \
        cpu->regs.r[num].l = op_readstack(cpu);         \
        cpu->regs.p.n = (cpu->regs.r[num].l & 0x80);    \
        cpu->regs.p.z = (cpu->regs.r[num].l == 0);      \
    }                                                   \
                                                        \
    void op_pull_##name##_w(cpu_t* cpu) {               \
        cpu->regs.r[num].l = op_readstack(cpu);         \
        cpu->regs.r[num].h = op_readstack(cpu);         \
        cpu->regs.p.n = (cpu->regs.r[num].w & 0x8000);  \
        cpu->regs.p.z = (cpu->regs.r[num].w == 0);      \
    }

op_pull_gen(PLA, A);
op_pull_gen(PLX, X);
op_pull_gen(PLY, Y);

#undef op_push_gen

void op_pld_e(cpu_t* cpu) {
    cpu->regs.d.l = op_readstackn(cpu);
    cpu->regs.d.h = op_readstackn(cpu);
    cpu->regs.p.n = (cpu->regs.d.w & 0x8000);
    cpu->regs.p.z = (cpu->regs.d.w == 0);
    cpu->regs.s.h = 0x01;
}

void op_pld_n(cpu_t* cpu) {
    cpu->regs.d.l = op_readstackn(cpu);
    cpu->regs.d.h = op_readstackn(cpu);
    cpu->regs.p.n = (cpu->regs.d.w & 0x8000);
    cpu->regs.p.z = (cpu->regs.d.w == 0);
}

void op_plb(cpu_t* cpu) {
    cpu->regs.db = op_readstack(cpu);
    cpu->regs.p.n = (cpu->regs.db & 0x80);
    cpu->regs.p.z = (cpu->regs.db == 0);
}

void op_plp_e(cpu_t* cpu) {
    cpu->regs.p.b = op_readstack(cpu) | 0x30;
    if(cpu->regs.p.x) {
        cpu->regs.x.h = 0x00;
        cpu->regs.y.h = 0x00;
    }
    update_table(cpu);
}

void op_plp_n(cpu_t* cpu) {
    cpu->regs.p.b = op_readstack(cpu);
    if(cpu->regs.p.x) {
        cpu->regs.x.h = 0x00;
        cpu->regs.y.h = 0x00;
    }
    update_table(cpu);
}

void op_pea_e(cpu_t* cpu) {
    cpu->aa.l = op_readpc(cpu);
    cpu->aa.h = op_readpc(cpu);
    op_writestackn(cpu, cpu->aa.h);
    op_writestackn(cpu, cpu->aa.l);
    cpu->regs.s.h = 0x01;
}

void op_pea_n(cpu_t* cpu) {
    cpu->aa.l = op_readpc(cpu);
    cpu->aa.h = op_readpc(cpu);
    op_writestackn(cpu, cpu->aa.h);
    op_writestackn(cpu, cpu->aa.l);
}

void op_pei_e(cpu_t* cpu) {
    cpu->dp = op_readpc(cpu);
    cpu->aa.l = op_readdp(cpu, cpu->dp + 0);
    cpu->aa.h = op_readdp(cpu, cpu->dp + 1);
    op_writestackn(cpu, cpu->aa.h);
    op_writestackn(cpu, cpu->aa.l);
    cpu->regs.s.h = 0x01;
}

void op_pei_n(cpu_t* cpu) {
    cpu->dp = op_readpc(cpu);
    cpu->aa.l = op_readdp(cpu, cpu->dp + 0);
    cpu->aa.h = op_readdp(cpu, cpu->dp + 1);
    op_writestackn(cpu, cpu->aa.h);
    op_writestackn(cpu, cpu->aa.l);
}

void op_per_e(cpu_t* cpu) {
    cpu->aa.l = op_readpc(cpu);
    cpu->aa.h = op_readpc(cpu);
    cpu->rd.w = cpu->regs.pc.d + (int16_t)cpu->aa.w;
    op_writestackn(cpu, cpu->rd.h);
    op_writestackn(cpu, cpu->rd.l);
    cpu->regs.s.h = 0x01;
}

void op_per_n(cpu_t* cpu) {
    cpu->aa.l = op_readpc(cpu);
    cpu->aa.h = op_readpc(cpu);
    cpu->rd.w = cpu->regs.pc.d + (int16_t)cpu->aa.w;
    op_writestackn(cpu, cpu->rd.h);
    op_writestackn(cpu, cpu->rd.l);
}

#undef A
#undef X
#undef Y
#undef Z
#undef S
#undef D
