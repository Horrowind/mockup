#include "algorithms.h"
#include "memory.h"

#include "opcode_write.h"

#define A 0
#define X 1
#define Y 2
#define Z 3
#define S 4
#define D 5

#define wdc65816_op_write_addr_gen(name, num)                             \
    void wdc65816_op_write_addr_##name##_b(wdc65816_cpu_t* cpu) {			\
        cpu->aa.l = wdc65816_op_readpc(cpu);                              \
        cpu->aa.h = wdc65816_op_readpc(cpu);                              \
        wdc65816_op_writedbr(cpu, cpu->aa.w, cpu->regs.r[num].w);         \
    }                                                                   \
                                                                        \
    void wdc65816_op_write_addr_##name##_w(wdc65816_cpu_t* cpu) {           \
        cpu->aa.l = wdc65816_op_readpc(cpu);                              \
        cpu->aa.h = wdc65816_op_readpc(cpu);                              \
        wdc65816_op_writedbr(cpu, cpu->aa.w + 0, cpu->regs.r[num].w >> 0); \
        wdc65816_op_writedbr(cpu, cpu->aa.w + 1, cpu->regs.r[num].w >> 8); \
    }

wdc65816_op_write_addr_gen(sty, Y);
wdc65816_op_write_addr_gen(sta, A);
wdc65816_op_write_addr_gen(stx, X);
wdc65816_op_write_addr_gen(stz, Z);

#undef wdc65816_op_write_addr_gen

#define wdc65816_op_write_addrr_gen(name, num, i)                         \
    void wdc65816_op_write_addrr_##name##_b(wdc65816_cpu_t* cpu) {			\
        cpu->aa.l = wdc65816_op_readpc(cpu);                              \
        cpu->aa.h = wdc65816_op_readpc(cpu);                              \
        wdc65816_op_writedbr(cpu, cpu->aa.w + cpu->regs.r[i].w, cpu->regs.r[num].w); \
    }                                                                   \
                                                                        \
    void wdc65816_op_write_addrr_##name##_w(wdc65816_cpu_t* cpu) {			\
        cpu->aa.l = wdc65816_op_readpc(cpu);                              \
        cpu->aa.h = wdc65816_op_readpc(cpu);                              \
        wdc65816_op_writedbr(cpu, cpu->aa.w + cpu->regs.r[i].w + 0, cpu->regs.r[num].w >> 0); \
        wdc65816_op_writedbr(cpu, cpu->aa.w + cpu->regs.r[i].w + 1, cpu->regs.r[num].w >> 8); \
    }

wdc65816_op_write_addrr_gen(stay, A, Y);
wdc65816_op_write_addrr_gen(stax, A, X);
wdc65816_op_write_addrr_gen(stz, Z, X);

#undef wdc65816_op_write_addrr_gen

#define wdc65816_op_write_longr_gen(name, i)                              \
    void wdc65816_op_write_longr_##name##_b(wdc65816_cpu_t* cpu) {			\
        cpu->aa.l = wdc65816_op_readpc(cpu);                              \
        cpu->aa.h = wdc65816_op_readpc(cpu);                              \
        cpu->aa.b = wdc65816_op_readpc(cpu);                              \
        wdc65816_op_writelong(cpu, cpu->aa.d + cpu->regs.r[i].w, cpu->regs.a.l); \
    }                                                                   \
                                                                        \
    void wdc65816_op_write_longr_##name##_w(wdc65816_cpu_t* cpu) {			\
        cpu->aa.l = wdc65816_op_readpc(cpu);                              \
        cpu->aa.h = wdc65816_op_readpc(cpu);                              \
        cpu->aa.b = wdc65816_op_readpc(cpu);                              \
        wdc65816_op_writelong(cpu, cpu->aa.d + cpu->regs.r[i].w + 0, cpu->regs.a.l); \
        wdc65816_op_writelong(cpu, cpu->aa.d + cpu->regs.r[i].w + 1, cpu->regs.a.h); \
    }

wdc65816_op_write_longr_gen(sta, Z);
wdc65816_op_write_longr_gen(stax, X);

#undef wdc65816_op_write_longr_gen	 


#define wdc65816_op_write_dp_gen(name, num)                               \
    void wdc65816_op_write_dp_##name##_b(wdc65816_cpu_t* cpu) {             \
        cpu->dp = wdc65816_op_readpc(cpu);                                \
        wdc65816_op_writedp(cpu, cpu->dp, cpu->regs.r[num].w);            \
    }                                                                   \
                                                                        \
    void wdc65816_op_write_dp_##name##_w(wdc65816_cpu_t* cpu) {             \
        cpu->dp = wdc65816_op_readpc(cpu);                                \
        wdc65816_op_writedp(cpu, cpu->dp + 0, cpu->regs.r[num].w >> 0);	\
        wdc65816_op_writedp(cpu, cpu->dp + 1, cpu->regs.r[num].w >> 8);	\
    }

wdc65816_op_write_dp_gen(stz, Z);
wdc65816_op_write_dp_gen(sty, Y);
wdc65816_op_write_dp_gen(sta, A);
wdc65816_op_write_dp_gen(stx, X);

#undef wdc65816_op_write_dp_gen

#define wdc65816_op_write_dpr_gen(name, num, i)                           \
    void wdc65816_op_write_dpr_##name##_b(wdc65816_cpu_t* cpu) {            \
        cpu->dp = wdc65816_op_readpc(cpu);                                \
        wdc65816_op_writedp(cpu, cpu->dp + cpu->regs.r[i].w, cpu->regs.r[num].w); \
    }                                                                   \
                                                                        \
    void wdc65816_op_write_dpr_##name##_w(wdc65816_cpu_t* cpu) {            \
        cpu->dp = wdc65816_op_readpc(cpu);                                \
        wdc65816_op_writedp(cpu, cpu->dp + cpu->regs.r[i].w + 0, cpu->regs.r[num].w >> 0); \
        wdc65816_op_writedp(cpu, cpu->dp + cpu->regs.r[i].w + 1, cpu->regs.r[num].w >> 8); \
    }

wdc65816_op_write_dpr_gen(stz, Z, X);
wdc65816_op_write_dpr_gen(sty, Y, X);
wdc65816_op_write_dpr_gen(sta, A, X);
wdc65816_op_write_dpr_gen(stx, X, Y);

#undef wdc65816_op_write_dpr_gen

void wdc65816_op_sta_idp_b(wdc65816_cpu_t* cpu) {
    cpu->dp = wdc65816_op_readpc(cpu);
    cpu->aa.l = wdc65816_op_readdp(cpu, cpu->dp + 0);
    cpu->aa.h = wdc65816_op_readdp(cpu, cpu->dp + 1);
    wdc65816_op_writedbr(cpu, cpu->aa.w, cpu->regs.a.l);
}

void wdc65816_op_sta_idp_w(wdc65816_cpu_t* cpu) {
    cpu->dp = wdc65816_op_readpc(cpu);
    cpu->aa.l = wdc65816_op_readdp(cpu, cpu->dp + 0);
    cpu->aa.h = wdc65816_op_readdp(cpu, cpu->dp + 1);
    wdc65816_op_writedbr(cpu, cpu->aa.w + 0, cpu->regs.a.l);
    wdc65816_op_writedbr(cpu, cpu->aa.w + 1, cpu->regs.a.h);
}

void wdc65816_op_sta_ildp_b(wdc65816_cpu_t* cpu) {
    cpu->dp = wdc65816_op_readpc(cpu);
    cpu->aa.l = wdc65816_op_readdp(cpu, cpu->dp + 0);
    cpu->aa.h = wdc65816_op_readdp(cpu, cpu->dp + 1);
    cpu->aa.b = wdc65816_op_readdp(cpu, cpu->dp + 2);
    wdc65816_op_writelong(cpu, cpu->aa.d, cpu->regs.a.l);
}

void wdc65816_op_sta_ildp_w(wdc65816_cpu_t* cpu) {
    cpu->dp = wdc65816_op_readpc(cpu);
    cpu->aa.l = wdc65816_op_readdp(cpu, cpu->dp + 0);
    cpu->aa.h = wdc65816_op_readdp(cpu, cpu->dp + 1);
    cpu->aa.b = wdc65816_op_readdp(cpu, cpu->dp + 2);
    wdc65816_op_writelong(cpu, cpu->aa.d + 0, cpu->regs.a.l);
    wdc65816_op_writelong(cpu, cpu->aa.d + 1, cpu->regs.a.h);
}

void wdc65816_op_sta_idpx_b(wdc65816_cpu_t* cpu) {
    cpu->dp = wdc65816_op_readpc(cpu);
    cpu->aa.l = wdc65816_op_readdp(cpu, cpu->dp + cpu->regs.x.w + 0);
    cpu->aa.h = wdc65816_op_readdp(cpu, cpu->dp + cpu->regs.x.w + 1);
    wdc65816_op_writedbr(cpu, cpu->aa.w, cpu->regs.a.l);
}

void wdc65816_op_sta_idpx_w(wdc65816_cpu_t* cpu) {
    cpu->dp = wdc65816_op_readpc(cpu);
    cpu->aa.l = wdc65816_op_readdp(cpu, cpu->dp + cpu->regs.x.w + 0);
    cpu->aa.h = wdc65816_op_readdp(cpu, cpu->dp + cpu->regs.x.w + 1);
    wdc65816_op_writedbr(cpu, cpu->aa.w + 0, cpu->regs.a.l);
    wdc65816_op_writedbr(cpu, cpu->aa.w + 1, cpu->regs.a.h);
}

void wdc65816_op_sta_idpy_b(wdc65816_cpu_t* cpu) {
    cpu->dp = wdc65816_op_readpc(cpu);
    cpu->aa.l = wdc65816_op_readdp(cpu, cpu->dp + 0);
    cpu->aa.h = wdc65816_op_readdp(cpu, cpu->dp + 1);
    wdc65816_op_writedbr(cpu, cpu->aa.w + cpu->regs.y.w, cpu->regs.a.l);
}

void wdc65816_op_sta_idpy_w(wdc65816_cpu_t* cpu) {
    cpu->dp = wdc65816_op_readpc(cpu);
    cpu->aa.l = wdc65816_op_readdp(cpu, cpu->dp + 0);
    cpu->aa.h = wdc65816_op_readdp(cpu, cpu->dp + 1);
    wdc65816_op_writedbr(cpu, cpu->aa.w + cpu->regs.y.w + 0, cpu->regs.a.l);
    wdc65816_op_writedbr(cpu, cpu->aa.w + cpu->regs.y.w + 1, cpu->regs.a.h);
}

void wdc65816_op_sta_ildpy_b(wdc65816_cpu_t* cpu) {
    cpu->dp = wdc65816_op_readpc(cpu);
    cpu->aa.l = wdc65816_op_readdp(cpu, cpu->dp + 0);
    cpu->aa.h = wdc65816_op_readdp(cpu, cpu->dp + 1);
    cpu->aa.b = wdc65816_op_readdp(cpu, cpu->dp + 2);
    wdc65816_op_writelong(cpu, cpu->aa.d + cpu->regs.y.w, cpu->regs.a.l);
}

void wdc65816_op_sta_ildpy_w(wdc65816_cpu_t* cpu) {
    cpu->dp = wdc65816_op_readpc(cpu);
    cpu->aa.l = wdc65816_op_readdp(cpu, cpu->dp + 0);
    cpu->aa.h = wdc65816_op_readdp(cpu, cpu->dp + 1);
    cpu->aa.b = wdc65816_op_readdp(cpu, cpu->dp + 2);
    wdc65816_op_writelong(cpu, cpu->aa.d + cpu->regs.y.w + 0, cpu->regs.a.l);
    wdc65816_op_writelong(cpu, cpu->aa.d + cpu->regs.y.w + 1, cpu->regs.a.h);
}

void wdc65816_op_sta_sr_b(wdc65816_cpu_t* cpu) {
    cpu->sp = wdc65816_op_readpc(cpu);
    wdc65816_op_writesp(cpu, cpu->sp, cpu->regs.a.l);
}

void wdc65816_op_sta_sr_w(wdc65816_cpu_t* cpu) {
    cpu->sp = wdc65816_op_readpc(cpu);
    wdc65816_op_writesp(cpu, cpu->sp + 0, cpu->regs.a.l);
    wdc65816_op_writesp(cpu, cpu->sp + 1, cpu->regs.a.h);
}

void wdc65816_op_sta_isry_b(wdc65816_cpu_t* cpu) {
    cpu->sp = wdc65816_op_readpc(cpu);
    cpu->aa.l = wdc65816_op_readsp(cpu, cpu->sp + 0);
    cpu->aa.h = wdc65816_op_readsp(cpu, cpu->sp + 1);
    wdc65816_op_writedbr(cpu, cpu->aa.w + cpu->regs.y.w, cpu->regs.a.l);
}

void wdc65816_op_sta_isry_w(wdc65816_cpu_t* cpu) {
    cpu->sp = wdc65816_op_readpc(cpu);
    cpu->aa.l = wdc65816_op_readsp(cpu, cpu->sp + 0);
    cpu->aa.h = wdc65816_op_readsp(cpu, cpu->sp + 1);
    wdc65816_op_writedbr(cpu, cpu->aa.w + cpu->regs.y.w + 0, cpu->regs.a.l);
    wdc65816_op_writedbr(cpu, cpu->aa.w + cpu->regs.y.w + 1, cpu->regs.a.h);
}

#undef A
#undef X
#undef Y
#undef Z
#undef S
#undef D
