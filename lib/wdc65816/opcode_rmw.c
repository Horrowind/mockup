#include "algorithms.h"
#include "memory.h"

#define A 0
#define X 1
#define Y 2
#define Z 3
#define S 4
#define D 5

#define wdc65816_op_adjust_imm_gen(name, num, adjust)                     \
    void wdc65816_op_adjust_imm_##name##_b(WDC65816Cpu* cpu) {           \
        cpu->regs.r[num].l += adjust;                                   \
        cpu->regs.p.n = 1 && (cpu->regs.r[num].l & 0x80);               \
        cpu->regs.p.z = (cpu->regs.r[num].l == 0);                      \
    }                                                                   \
                                                                        \
    void wdc65816_op_adjust_imm_##name##_w(WDC65816Cpu* cpu) {           \
        cpu->regs.r[num].w += adjust;                                   \
        cpu->regs.p.n = 1 && (cpu->regs.r[num].w & 0x8000);             \
        cpu->regs.p.z = (cpu->regs.r[num].w == 0);                      \
    }

wdc65816_op_adjust_imm_gen(inca, A, 1);
wdc65816_op_adjust_imm_gen(deca, A,-1);
wdc65816_op_adjust_imm_gen(incx, X, 1);
wdc65816_op_adjust_imm_gen(decx, X,-1);
wdc65816_op_adjust_imm_gen(incy, Y, 1);
wdc65816_op_adjust_imm_gen(decy, Y,-1);
#undef wdc65816_op_adjust_imm_gen

void wdc65816_op_asl_imm_b(WDC65816Cpu* cpu) {
    cpu->regs.p.c = 1 && (cpu->regs.a.l & 0x80);
    cpu->regs.a.l <<= 1;
    cpu->regs.p.n = 1 && (cpu->regs.a.l & 0x80);
    cpu->regs.p.z = (cpu->regs.a.l == 0);
}

void wdc65816_op_asl_imm_w(WDC65816Cpu* cpu) {
    cpu->regs.p.c = 1 && (cpu->regs.a.w & 0x8000);
    cpu->regs.a.w <<= 1;
    cpu->regs.p.n = 1 && (cpu->regs.a.w & 0x8000);
    cpu->regs.p.z = (cpu->regs.a.w == 0);
}

void wdc65816_op_lsr_imm_b(WDC65816Cpu* cpu) {
    cpu->regs.p.c = 1 && (cpu->regs.a.l & 0x01);
    cpu->regs.a.l >>= 1;
    cpu->regs.p.n = 1 && (cpu->regs.a.l & 0x80);
    cpu->regs.p.z = (cpu->regs.a.l == 0);
}

void wdc65816_op_lsr_imm_w(WDC65816Cpu* cpu) {
    cpu->regs.p.c = 1 && (cpu->regs.a.w & 0x0001);
    cpu->regs.a.w >>= 1;
    cpu->regs.p.n = 1 && (cpu->regs.a.w & 0x8000);
    cpu->regs.p.z = (cpu->regs.a.w == 0);
}

void wdc65816_op_rol_imm_b(WDC65816Cpu* cpu) {
    u8 carry = cpu->regs.p.c;
    cpu->regs.p.c = 1 && (cpu->regs.a.l & 0x80);
    cpu->regs.a.l = (cpu->regs.a.l << 1) | carry;
    cpu->regs.p.n = 1 && (cpu->regs.a.l & 0x80);
    cpu->regs.p.z = (cpu->regs.a.l == 0);
}

void wdc65816_op_rol_imm_w(WDC65816Cpu* cpu) {
    u8 carry = cpu->regs.p.c;
    cpu->regs.p.c = 1 && (cpu->regs.a.w & 0x8000);
    cpu->regs.a.w = (cpu->regs.a.w << 1) | carry;
    cpu->regs.p.n = 1 && (cpu->regs.a.w & 0x8000);
    cpu->regs.p.z = (cpu->regs.a.w == 0);
}

void wdc65816_op_ror_imm_b(WDC65816Cpu* cpu) {
    u8 carry = cpu->regs.p.c;
    cpu->regs.p.c = 1 && (cpu->regs.a.l & 0x01);
    cpu->regs.a.l = (carry << 7) | (cpu->regs.a.l >> 1);
    cpu->regs.p.n = 1 && (cpu->regs.a.l & 0x80);
    cpu->regs.p.z = (cpu->regs.a.l == 0);
}

void wdc65816_op_ror_imm_w(WDC65816Cpu* cpu) {
    u8 carry = cpu->regs.p.c;
    cpu->regs.p.c = 1 && (cpu->regs.a.w & 0x0001);
    cpu->regs.a.w = (carry << 15) | (cpu->regs.a.w >> 1);
    cpu->regs.p.n = 1 && (cpu->regs.a.w & 0x8000);
    cpu->regs.p.z = (cpu->regs.a.w == 0);
}

#define wdc65816_op_adjust_addr_gen(op)                                   \
    void wdc65816_op_adjust_addr_##op##_b(WDC65816Cpu* cpu) {            \
        cpu->aa.l = wdc65816_op_readpc(cpu);                              \
        cpu->aa.h = wdc65816_op_readpc(cpu);                              \
        cpu->rd.l = wdc65816_op_readdbr(cpu, cpu->aa.w);                  \
        wdc65816_op_##op##_b(cpu);                                        \
        wdc65816_op_writedbr(cpu, cpu->aa.w, cpu->rd.l);                  \
    }                                                                   \
                                                                        \
    void wdc65816_op_adjust_addr_##op##_w(WDC65816Cpu* cpu) {            \
        cpu->aa.l = wdc65816_op_readpc(cpu);                              \
        cpu->aa.h = wdc65816_op_readpc(cpu);                              \
        cpu->rd.l = wdc65816_op_readdbr(cpu, cpu->aa.w + 0);              \
        cpu->rd.h = wdc65816_op_readdbr(cpu, cpu->aa.w + 1);              \
        wdc65816_op_##op##_w(cpu);                                        \
        wdc65816_op_writedbr(cpu, cpu->aa.w + 1, cpu->rd.h);              \
        wdc65816_op_writedbr(cpu, cpu->aa.w + 0, cpu->rd.l);              \
    }

wdc65816_op_adjust_addr_gen(tsb);
wdc65816_op_adjust_addr_gen(asl);
wdc65816_op_adjust_addr_gen(trb);
wdc65816_op_adjust_addr_gen(rol);
wdc65816_op_adjust_addr_gen(lsr);
wdc65816_op_adjust_addr_gen(ror);
wdc65816_op_adjust_addr_gen(dec);
wdc65816_op_adjust_addr_gen(inc);

#undef wdc65816_op_adjust_addr_gen

#define wdc65816_op_adjust_addrx_gen(op)                                  \
    void wdc65816_op_adjust_addrx_##op##_b(WDC65816Cpu* cpu) {           \
        cpu->aa.l = wdc65816_op_readpc(cpu);                              \
        cpu->aa.h = wdc65816_op_readpc(cpu);                              \
        cpu->rd.l = wdc65816_op_readdbr(cpu, cpu->aa.w + cpu->regs.x.w);  \
        wdc65816_op_##op##_b(cpu);                                        \
        wdc65816_op_writedbr(cpu, cpu->aa.w + cpu->regs.x.w, cpu->rd.l);  \
    }                                                                   \
                                                                        \
    void wdc65816_op_adjust_addrx_##op##_w(WDC65816Cpu* cpu) {           \
        cpu->aa.l = wdc65816_op_readpc(cpu);                              \
        cpu->aa.h = wdc65816_op_readpc(cpu);                              \
        cpu->rd.l = wdc65816_op_readdbr(cpu, cpu->aa.w + cpu->regs.x.w + 0); \
        cpu->rd.h = wdc65816_op_readdbr(cpu, cpu->aa.w + cpu->regs.x.w + 1); \
        wdc65816_op_##op##_w(cpu);                                        \
        wdc65816_op_writedbr(cpu, cpu->aa.w + cpu->regs.x.w + 1, cpu->rd.h); \
        wdc65816_op_writedbr(cpu, cpu->aa.w + cpu->regs.x.w + 0, cpu->rd.l); \
    }

wdc65816_op_adjust_addrx_gen(asl);
wdc65816_op_adjust_addrx_gen(rol);
wdc65816_op_adjust_addrx_gen(lsr);
wdc65816_op_adjust_addrx_gen(ror);
wdc65816_op_adjust_addrx_gen(dec);
wdc65816_op_adjust_addrx_gen(inc);
#undef wdc65816_op_adjust_addrx_gen    

#define wdc65816_op_adjust_dp_gen(op)                                     \
    void wdc65816_op_adjust_dp_##op##_b(WDC65816Cpu* cpu) {              \
        cpu->dp = wdc65816_op_readpc(cpu);                                \
        cpu->rd.l = wdc65816_op_readdp(cpu, cpu->dp);                     \
        wdc65816_op_##op##_b(cpu);                                        \
        wdc65816_op_writedp(cpu, cpu->dp, cpu->rd.l);                     \
    }                                                                   \
                                                                        \
    void wdc65816_op_adjust_dp_##op##_w(WDC65816Cpu* cpu) {              \
        cpu->dp = wdc65816_op_readpc(cpu);                                \
        cpu->rd.l = wdc65816_op_readdp(cpu, cpu->dp + 0);                 \
        cpu->rd.h = wdc65816_op_readdp(cpu, cpu->dp + 1);                 \
        wdc65816_op_##op##_w(cpu);                                        \
        wdc65816_op_writedp(cpu, cpu->dp + 1, cpu->rd.h);                 \
        wdc65816_op_writedp(cpu, cpu->dp + 0, cpu->rd.l);                 \
    }


wdc65816_op_adjust_dp_gen(tsb);
wdc65816_op_adjust_dp_gen(asl);
wdc65816_op_adjust_dp_gen(trb);
wdc65816_op_adjust_dp_gen(rol);
wdc65816_op_adjust_dp_gen(lsr);
wdc65816_op_adjust_dp_gen(ror);
wdc65816_op_adjust_dp_gen(dec);
wdc65816_op_adjust_dp_gen(inc);
#undef wdc65816_op_adjust_dp_gen


#define wdc65816_op_adjust_dpx_gen(op)                                    \
    void wdc65816_op_adjust_dpx_##op##_b(WDC65816Cpu* cpu) {             \
        cpu->dp = wdc65816_op_readpc(cpu);                                \
        cpu->rd.l = wdc65816_op_readdp(cpu, cpu->dp + cpu->regs.x.w);     \
        wdc65816_op_##op##_b(cpu);                                        \
        wdc65816_op_writedp(cpu, cpu->dp + cpu->regs.x.w, cpu->rd.l);     \
    }                                                                   \
                                                                        \
    void wdc65816_op_adjust_dpx_##op##_w(WDC65816Cpu* cpu) {             \
        cpu->dp = wdc65816_op_readpc(cpu);                                \
        cpu->rd.l = wdc65816_op_readdp(cpu, cpu->dp + cpu->regs.x.w + 0); \
        cpu->rd.h = wdc65816_op_readdp(cpu, cpu->dp + cpu->regs.x.w + 1); \
        wdc65816_op_##op##_w(cpu);                                        \
        wdc65816_op_writedp(cpu, cpu->dp + cpu->regs.x.w + 1, cpu->rd.h); \
        wdc65816_op_writedp(cpu, cpu->dp + cpu->regs.x.w + 0, cpu->rd.l); \
    }

wdc65816_op_adjust_dpx_gen(asl);
wdc65816_op_adjust_dpx_gen(rol);
wdc65816_op_adjust_dpx_gen(lsr);
wdc65816_op_adjust_dpx_gen(ror);
wdc65816_op_adjust_dpx_gen(dec);
wdc65816_op_adjust_dpx_gen(inc);
#undef wdc65816_op_adjust_dpx_gen


#undef A
#undef X
#undef Y
#undef Z
#undef S
#undef D
