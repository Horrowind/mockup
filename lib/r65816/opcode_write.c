#include "memory.h"

#define A 0
#define X 1
#define Y 2
#define Z 3
#define S 4
#define D 5

#define r65816_op_write_addr_gen(name, num)                             \
    void r65816_op_write_addr_##name##_b(r65816_cpu_t* cpu) {           \
        cpu->aa.l = r65816_op_readpc(cpu);                              \
        cpu->aa.h = r65816_op_readpc(cpu);                              \
        r65816_op_writedbr(cpu, cpu->aa.w, cpu->regs.r[num].w);         \
    }                                                                   \
                                                                        \
    void r65816_op_write_addr_##name##_w(r65816_cpu_t* cpu) {            \
        cpu->aa.l = r65816_op_readpc(cpu);                              \
        cpu->aa.h = r65816_op_readpc(cpu);                              \
        r65816_op_writedbr(cpu, cpu->aa.w + 0, cpu->regs.r[num].w >> 0); \
        r65816_op_writedbr(cpu, cpu->aa.w + 1, cpu->regs.r[num].w >> 8); \
    }

r65816_op_write_addr_gen(sty, Y);
r65816_op_write_addr_gen(sta, A);
r65816_op_write_addr_gen(stx, X);
r65816_op_write_addr_gen(stz, Z);

#undef r65816_op_write_addr_gen

#define r65816_op_write_addrr_gen(name, num, i)                         \
    void r65816_op_write_addrr_##name##_b(r65816_cpu_t* cpu) {			\
        cpu->aa.l = r65816_op_readpc(cpu);                              \
        cpu->aa.h = r65816_op_readpc(cpu);                              \
        r65816_op_writedbr(cpu, cpu->aa.w + cpu->regs.r[i].w, cpu->regs.r[num].w); \
    }                                                                   \
                                                                        \
    void r65816_op_write_addrr_##name##_w(r65816_cpu_t* cpu) {			\
        cpu->aa.l = r65816_op_readpc(cpu);                              \
        cpu->aa.h = r65816_op_readpc(cpu);                              \
        r65816_op_writedbr(cpu, cpu->aa.w + cpu->regs.r[i].w + 0, cpu->regs.r[num].w >> 0); \
        r65816_op_writedbr(cpu, cpu->aa.w + cpu->regs.r[i].w + 1, cpu->regs.r[num].w >> 8); \
    }

r65816_op_write_addrr_gen(stay, A, Y);
r65816_op_write_addrr_gen(stax, A, X);
r65816_op_write_addrr_gen(stz, Z, X);

#undef r65816_op_write_addrr_gen

#define r65816_op_write_longr_gen(name, i)                              \
    void r65816_op_write_longr_##name##_b(r65816_cpu_t* cpu) {			\
        cpu->aa.l = r65816_op_readpc(cpu);                              \
        cpu->aa.h = r65816_op_readpc(cpu);                              \
        cpu->aa.b = r65816_op_readpc(cpu);                              \
        r65816_op_writelong(cpu, cpu->aa.d + cpu->regs.r[i].w, cpu->regs.a.l); \
    }                                                                   \
                                                                        \
    void r65816_op_write_longr_##name##_w(r65816_cpu_t* cpu) {			\
        cpu->aa.l = r65816_op_readpc(cpu);                              \
        cpu->aa.h = r65816_op_readpc(cpu);                              \
        cpu->aa.b = r65816_op_readpc(cpu);                              \
        r65816_op_writelong(cpu, cpu->aa.d + cpu->regs.r[i].w + 0, cpu->regs.a.l); \
        r65816_op_writelong(cpu, cpu->aa.d + cpu->regs.r[i].w + 1, cpu->regs.a.h); \
    }

r65816_op_write_longr_gen(sta, Z);
r65816_op_write_longr_gen(stax, X);

#undef r65816_op_write_longr_gen	 


#define r65816_op_write_dp_gen(name, num)                               \
    void r65816_op_write_dp_##name##_b(r65816_cpu_t* cpu) {             \
        cpu->dp = r65816_op_readpc(cpu);                                \
        r65816_op_writedp(cpu, cpu->dp, cpu->regs.r[num].w);            \
    }                                                                   \
                                                                        \
    void r65816_op_write_dp_##name##_w(r65816_cpu_t* cpu) {             \
        cpu->dp = r65816_op_readpc(cpu);                                \
        r65816_op_writedp(cpu, cpu->dp + 0, cpu->regs.r[num].w >> 0);	\
        r65816_op_writedp(cpu, cpu->dp + 1, cpu->regs.r[num].w >> 8);	\
    }

r65816_op_write_dp_gen(stz, Z);
r65816_op_write_dp_gen(sty, Y);
r65816_op_write_dp_gen(sta, A);
r65816_op_write_dp_gen(stx, X);

#undef r65816_op_write_dp_gen

#define r65816_op_write_dpr_gen(name, num, i)                           \
    void r65816_op_write_dpr_##name##_b(r65816_cpu_t* cpu) {            \
        cpu->dp = r65816_op_readpc(cpu);                                \
        r65816_op_writedp(cpu, cpu->dp + cpu->regs.r[i].w, cpu->regs.r[num].w); \
    }                                                                   \
                                                                        \
    void r65816_op_write_dpr_##name##_w(r65816_cpu_t* cpu) {            \
        cpu->dp = r65816_op_readpc(cpu);                                \
        r65816_op_writedp(cpu, cpu->dp + cpu->regs.r[i].w + 0, cpu->regs.r[num].w >> 0); \
        r65816_op_writedp(cpu, cpu->dp + cpu->regs.r[i].w + 1, cpu->regs.r[num].w >> 8); \
    }

r65816_op_write_dpr_gen(stz, Z, X);
r65816_op_write_dpr_gen(sty, Y, X);
r65816_op_write_dpr_gen(sta, A, X);
r65816_op_write_dpr_gen(stx, X, Y);

#undef r65816_op_write_dpr_gen

void r65816_op_sta_idp_b(r65816_cpu_t* cpu) {
    cpu->dp = r65816_op_readpc(cpu);
    cpu->aa.l = r65816_op_readdp(cpu, cpu->dp + 0);
    cpu->aa.h = r65816_op_readdp(cpu, cpu->dp + 1);
    r65816_op_writedbr(cpu, cpu->aa.w, cpu->regs.a.l);
}

void r65816_op_sta_idp_w(r65816_cpu_t* cpu) {
    cpu->dp = r65816_op_readpc(cpu);
    cpu->aa.l = r65816_op_readdp(cpu, cpu->dp + 0);
    cpu->aa.h = r65816_op_readdp(cpu, cpu->dp + 1);
    r65816_op_writedbr(cpu, cpu->aa.w + 0, cpu->regs.a.l);
    r65816_op_writedbr(cpu, cpu->aa.w + 1, cpu->regs.a.h);
}

void r65816_op_sta_ildp_b(r65816_cpu_t* cpu) {
    cpu->dp = r65816_op_readpc(cpu);
    cpu->aa.l = r65816_op_readdp(cpu, cpu->dp + 0);
    cpu->aa.h = r65816_op_readdp(cpu, cpu->dp + 1);
    cpu->aa.b = r65816_op_readdp(cpu, cpu->dp + 2);
    r65816_op_writelong(cpu, cpu->aa.d, cpu->regs.a.l);
}

void r65816_op_sta_ildp_w(r65816_cpu_t* cpu) {
    cpu->dp = r65816_op_readpc(cpu);
    cpu->aa.l = r65816_op_readdp(cpu, cpu->dp + 0);
    cpu->aa.h = r65816_op_readdp(cpu, cpu->dp + 1);
    cpu->aa.b = r65816_op_readdp(cpu, cpu->dp + 2);
    r65816_op_writelong(cpu, cpu->aa.d + 0, cpu->regs.a.l);
    r65816_op_writelong(cpu, cpu->aa.d + 1, cpu->regs.a.h);
}

void r65816_op_sta_idpx_b(r65816_cpu_t* cpu) {
    cpu->dp = r65816_op_readpc(cpu);
    cpu->aa.l = r65816_op_readdp(cpu, cpu->dp + cpu->regs.x.w + 0);
    cpu->aa.h = r65816_op_readdp(cpu, cpu->dp + cpu->regs.x.w + 1);
    r65816_op_writedbr(cpu, cpu->aa.w, cpu->regs.a.l);
}

void r65816_op_sta_idpx_w(r65816_cpu_t* cpu) {
    cpu->dp = r65816_op_readpc(cpu);
    cpu->aa.l = r65816_op_readdp(cpu, cpu->dp + cpu->regs.x.w + 0);
    cpu->aa.h = r65816_op_readdp(cpu, cpu->dp + cpu->regs.x.w + 1);
    r65816_op_writedbr(cpu, cpu->aa.w + 0, cpu->regs.a.l);
    r65816_op_writedbr(cpu, cpu->aa.w + 1, cpu->regs.a.h);
}

void r65816_op_sta_idpy_b(r65816_cpu_t* cpu) {
    cpu->dp = r65816_op_readpc(cpu);
    cpu->aa.l = r65816_op_readdp(cpu, cpu->dp + 0);
    cpu->aa.h = r65816_op_readdp(cpu, cpu->dp + 1);
    r65816_op_writedbr(cpu, cpu->aa.w + cpu->regs.y.w, cpu->regs.a.l);
}

void r65816_op_sta_idpy_w(r65816_cpu_t* cpu) {
    cpu->dp = r65816_op_readpc(cpu);
    cpu->aa.l = r65816_op_readdp(cpu, cpu->dp + 0);
    cpu->aa.h = r65816_op_readdp(cpu, cpu->dp + 1);
    r65816_op_writedbr(cpu, cpu->aa.w + cpu->regs.y.w + 0, cpu->regs.a.l);
    r65816_op_writedbr(cpu, cpu->aa.w + cpu->regs.y.w + 1, cpu->regs.a.h);
}

void r65816_op_sta_ildpy_b(r65816_cpu_t* cpu) {
    cpu->dp = r65816_op_readpc(cpu);
    cpu->aa.l = r65816_op_readdp(cpu, cpu->dp + 0);
    cpu->aa.h = r65816_op_readdp(cpu, cpu->dp + 1);
    cpu->aa.b = r65816_op_readdp(cpu, cpu->dp + 2);
    r65816_op_writelong(cpu, cpu->aa.d + cpu->regs.y.w, cpu->regs.a.l);
}

void r65816_op_sta_ildpy_w(r65816_cpu_t* cpu) {
    cpu->dp = r65816_op_readpc(cpu);
    cpu->aa.l = r65816_op_readdp(cpu, cpu->dp + 0);
    cpu->aa.h = r65816_op_readdp(cpu, cpu->dp + 1);
    cpu->aa.b = r65816_op_readdp(cpu, cpu->dp + 2);
    r65816_op_writelong(cpu, cpu->aa.d + cpu->regs.y.w + 0, cpu->regs.a.l);
    r65816_op_writelong(cpu, cpu->aa.d + cpu->regs.y.w + 1, cpu->regs.a.h);
}

void r65816_op_sta_sr_b(r65816_cpu_t* cpu) {
    cpu->sp = r65816_op_readpc(cpu);
    r65816_op_writesp(cpu, cpu->sp, cpu->regs.a.l);
}

void r65816_op_sta_sr_w(r65816_cpu_t* cpu) {
    cpu->sp = r65816_op_readpc(cpu);
    r65816_op_writesp(cpu, cpu->sp + 0, cpu->regs.a.l);
    r65816_op_writesp(cpu, cpu->sp + 1, cpu->regs.a.h);
}

void r65816_op_sta_isry_b(r65816_cpu_t* cpu) {
    cpu->sp = r65816_op_readpc(cpu);
    cpu->aa.l = r65816_op_readsp(cpu, cpu->sp + 0);
    cpu->aa.h = r65816_op_readsp(cpu, cpu->sp + 1);
    r65816_op_writedbr(cpu, cpu->aa.w + cpu->regs.y.w, cpu->regs.a.l);
}

void r65816_op_sta_isry_w(r65816_cpu_t* cpu) {
    cpu->sp = r65816_op_readpc(cpu);
    cpu->aa.l = r65816_op_readsp(cpu, cpu->sp + 0);
    cpu->aa.h = r65816_op_readsp(cpu, cpu->sp + 1);
    r65816_op_writedbr(cpu, cpu->aa.w + cpu->regs.y.w + 0, cpu->regs.a.l);
    r65816_op_writedbr(cpu, cpu->aa.w + cpu->regs.y.w + 1, cpu->regs.a.h);
}

#undef A
#undef X
#undef Y
#undef Z
#undef S
#undef D
