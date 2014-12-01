#include "algorithms.h"
#include "memory.h"

#define A 0
#define X 1
#define Y 2
#define Z 3
#define S 4
#define D 5

#define op_adjust_imm_gen(name, num, adjust)                            \
    void op_adjust_imm_##name##_b(cpu_t* cpu) {                         \
        cpu->regs.r[num].l += adjust;                                   \
        cpu->regs.p.n = (cpu->regs.r[num].l & 0x80);                    \
        cpu->regs.p.z = (cpu->regs.r[num].l == 0);                      \
    }                                                                   \
                                                                        \
    void op_adjust_imm_##name##_w(cpu_t* cpu) {                         \
        cpu->regs.r[num].w += adjust;                                   \
        cpu->regs.p.n = (cpu->regs.r[num].w & 0x8000);                  \
        cpu->regs.p.z = (cpu->regs.r[num].w == 0);                      \
    }

op_adjust_imm_gen(inca, A, 1);
op_adjust_imm_gen(deca, A,-1);
op_adjust_imm_gen(incx, X, 1);
op_adjust_imm_gen(decx, X,-1);
op_adjust_imm_gen(incy, Y, 1);
op_adjust_imm_gen(decy, Y,-1);
#undef op_adjust_imm_gen

void op_asl_imm_b(cpu_t* cpu) {
    cpu->regs.p.c = (cpu->regs.a.l & 0x80);
    cpu->regs.a.l <<= 1;
    cpu->regs.p.n = (cpu->regs.a.l & 0x80);
    cpu->regs.p.z = (cpu->regs.a.l == 0);
}

void op_asl_imm_w(cpu_t* cpu) {
    cpu->regs.p.c = (cpu->regs.a.w & 0x8000);
    cpu->regs.a.w <<= 1;
    cpu->regs.p.n = (cpu->regs.a.w & 0x8000);
    cpu->regs.p.z = (cpu->regs.a.w == 0);
}

void op_lsr_imm_b(cpu_t* cpu) {
    cpu->regs.p.c = (cpu->regs.a.l & 0x01);
    cpu->regs.a.l >>= 1;
    cpu->regs.p.n = (cpu->regs.a.l & 0x80);
    cpu->regs.p.z = (cpu->regs.a.l == 0);
}

void op_lsr_imm_w(cpu_t* cpu) {
    cpu->regs.p.c = (cpu->regs.a.w & 0x0001);
    cpu->regs.a.w >>= 1;
    cpu->regs.p.n = (cpu->regs.a.w & 0x8000);
    cpu->regs.p.z = (cpu->regs.a.w == 0);
}

void op_rol_imm_b(cpu_t* cpu) {
    uint8_t carry = cpu->regs.p.c;
    cpu->regs.p.c = (cpu->regs.a.l & 0x80);
    cpu->regs.a.l = (cpu->regs.a.l << 1) | carry;
    cpu->regs.p.n = (cpu->regs.a.l & 0x80);
    cpu->regs.p.z = (cpu->regs.a.l == 0);
}

void op_rol_imm_w(cpu_t* cpu) {
    uint8_t carry = cpu->regs.p.c;
    cpu->regs.p.c = (cpu->regs.a.w & 0x8000);
    cpu->regs.a.w = (cpu->regs.a.w << 1) | carry;
    cpu->regs.p.n = (cpu->regs.a.w & 0x8000);
    cpu->regs.p.z = (cpu->regs.a.w == 0);
}

void op_ror_imm_b(cpu_t* cpu) {
    uint8_t carry = cpu->regs.p.c;
    cpu->regs.p.c = (cpu->regs.a.l & 0x01);
    cpu->regs.a.l = (carry << 7) | (cpu->regs.a.l >> 1);
    cpu->regs.p.n = (cpu->regs.a.l & 0x80);
    cpu->regs.p.z = (cpu->regs.a.l == 0);
}

void op_ror_imm_w(cpu_t* cpu) {
    uint8_t carry = cpu->regs.p.c;
    cpu->regs.p.c = (cpu->regs.a.w & 0x0001);
    cpu->regs.a.w = (carry << 15) | (cpu->regs.a.w >> 1);
    cpu->regs.p.n = (cpu->regs.a.w & 0x8000);
    cpu->regs.p.z = (cpu->regs.a.w == 0);
}

#define op_adjust_addr_gen(op)                                          \
    void op_adjust_addr_##op##_b(cpu_t* cpu) {                          \
        cpu->aa.l = op_readpc(cpu);                                     \
        cpu->aa.h = op_readpc(cpu);                                     \
        cpu->rd.l = op_readdbr(cpu, cpu->aa.w);                         \
        op_##op##_w(cpu);                                               \
        op_writedbr(cpu, cpu->aa.w, cpu->rd.l);                         \
    }                                                                   \
                                                                        \
    void op_adjust_addr_##op##_w(cpu_t* cpu) {                          \
        cpu->aa.l = op_readpc(cpu);                                     \
        cpu->aa.h = op_readpc(cpu);                                     \
        cpu->rd.l = op_readdbr(cpu, cpu->aa.w + 0);                     \
        cpu->rd.h = op_readdbr(cpu, cpu->aa.w + 1);                     \
        op_##op##_w(cpu);                                               \
        op_writedbr(cpu, cpu->aa.w + 1, cpu->rd.h);                     \
        op_writedbr(cpu, cpu->aa.w + 0, cpu->rd.l);                     \
    }

op_adjust_addr_gen(tsb);
op_adjust_addr_gen(asl);
op_adjust_addr_gen(trb);
op_adjust_addr_gen(rol);
op_adjust_addr_gen(lsr);
op_adjust_addr_gen(ror);
op_adjust_addr_gen(dec);
op_adjust_addr_gen(inc);

#undef op_adjust_addr_gen

#define op_adjust_addrx_gen(op)                                         \
    void op_adjust_addrx_##op##_b(cpu_t* cpu) {                         \
        cpu->aa.l = op_readpc(cpu);                                     \
        cpu->aa.h = op_readpc(cpu);                                     \
        cpu->rd.l = op_readdbr(cpu, cpu->aa.w + cpu->regs.x.w);         \
        op_##op##_w(cpu);                                               \
        op_writedbr(cpu, cpu->aa.w + cpu->regs.x.w, cpu->rd.l);         \
    }                                                                   \
                                                                        \
    void op_adjust_addrx_##op##_w(cpu_t* cpu) {                         \
        cpu->aa.l = op_readpc(cpu);                                     \
        cpu->aa.h = op_readpc(cpu);                                     \
        cpu->rd.l = op_readdbr(cpu, cpu->aa.w + cpu->regs.x.w + 0);     \
        cpu->rd.h = op_readdbr(cpu, cpu->aa.w + cpu->regs.x.w + 1);     \
        op_##op##_w(cpu);                                               \
        op_writedbr(cpu, cpu->aa.w + cpu->regs.x.w + 1, cpu->rd.h);     \
        op_writedbr(cpu, cpu->aa.w + cpu->regs.x.w + 0, cpu->rd.l);     \
    }

op_adjust_addrx_gen(asl);
op_adjust_addrx_gen(rol);
op_adjust_addrx_gen(lsr);
op_adjust_addrx_gen(ror);
op_adjust_addrx_gen(dec);
op_adjust_addrx_gen(inc);
#undef op_adjust_addrx_gen    

#define op_adjust_dp_gen(op)                                            \
    void op_adjust_dp_##op##_b(cpu_t* cpu) {                            \
        cpu->dp = op_readpc(cpu);                                       \
        cpu->rd.l = op_readdp(cpu, cpu->dp);                            \
        op_##op##_w(cpu);                                               \
        op_writedp(cpu, cpu->dp, cpu->rd.l);                            \
    }                                                                   \
                                                                        \
    void op_adjust_dp_##op##_w(cpu_t* cpu) {                            \
        cpu->dp = op_readpc(cpu);                                       \
        cpu->rd.l = op_readdp(cpu, cpu->dp + 0);                        \
        cpu->rd.h = op_readdp(cpu, cpu->dp + 1);                        \
        op_##op##_w(cpu);                                               \
        op_writedp(cpu, cpu->dp + 1, cpu->rd.h);                        \
        op_writedp(cpu, cpu->dp + 0, cpu->rd.l);                        \
    }


op_adjust_dp_gen(tsb);
op_adjust_dp_gen(asl);
op_adjust_dp_gen(trb);
op_adjust_dp_gen(rol);
op_adjust_dp_gen(lsr);
op_adjust_dp_gen(ror);
op_adjust_dp_gen(dec);
op_adjust_dp_gen(inc);
#undef op_adjust_dp_gen


#define op_adjust_dpx_gen(op)                                           \
    void op_adjust_dpx_##op##_b(cpu_t* cpu) {                           \
        cpu->dp = op_readpc(cpu);                                       \
        cpu->rd.l = op_readdp(cpu, cpu->dp + cpu->regs.x.w);            \
        op_##op##_w(cpu);                                               \
        op_writedp(cpu, cpu->dp + cpu->regs.x.w, cpu->rd.l);            \
    }                                                                   \
                                                                        \
    void op_adjust_dpx_##op##_w(cpu_t* cpu) {                           \
        cpu->dp = op_readpc(cpu);                                       \
        cpu->rd.l = op_readdp(cpu, cpu->dp + cpu->regs.x.w + 0);        \
        cpu->rd.h = op_readdp(cpu, cpu->dp + cpu->regs.x.w + 1);        \
        op_##op##_w(cpu);                                               \
        op_writedp(cpu, cpu->dp + cpu->regs.x.w + 1, cpu->rd.h);        \
        op_writedp(cpu, cpu->dp + cpu->regs.x.w + 0, cpu->rd.l);        \
    }

op_adjust_dpx_gen(asl);
op_adjust_dpx_gen(rol);
op_adjust_dpx_gen(lsr);
op_adjust_dpx_gen(ror);
op_adjust_dpx_gen(dec);
op_adjust_dpx_gen(inc);
#undef op_adjust_dpx_gen


#undef A
#undef X
#undef Y
#undef Z
#undef S
#undef D
