#include "algorithms.h"
#include "memory.h"

#include "opcode_read.h"

#define A 0
#define X 1
#define Y 2
#define Z 3
#define S 4
#define D 5

#define wdc65816_op_read_const_gen(op)                          \
    void wdc65816_op_read_const_##op##_b(Wdc65816Cpu* cpu) {    \
        cpu->rd.l = wdc65816_op_readpc(cpu);                    \
        wdc65816_op_##op##_b(cpu);                              \
    }                                                           \
                                                                \
    void wdc65816_op_read_const_##op##_w(Wdc65816Cpu* cpu) {    \
        cpu->rd.l = wdc65816_op_readpc(cpu);                    \
        cpu->rd.h = wdc65816_op_readpc(cpu);                    \
        wdc65816_op_##op##_w(cpu);                              \
    }

wdc65816_op_read_const_gen(ora);
wdc65816_op_read_const_gen(bit);
wdc65816_op_read_const_gen(and);
wdc65816_op_read_const_gen(eor);
wdc65816_op_read_const_gen(adc);
wdc65816_op_read_const_gen(ldy);
wdc65816_op_read_const_gen(lda);
wdc65816_op_read_const_gen(ldx);
wdc65816_op_read_const_gen(cpy);
wdc65816_op_read_const_gen(cmp);
wdc65816_op_read_const_gen(cpx);
wdc65816_op_read_const_gen(sbc);

#undef wdc65816_op_read_const_gen

void wdc65816_op_read_bit_const_b(Wdc65816Cpu* cpu) {
    cpu->rd.l = wdc65816_op_readpc(cpu);
    cpu->regs.p.z = ((cpu->rd.l & cpu->regs.a.l) == 0);
}

void wdc65816_op_read_bit_const_w(Wdc65816Cpu* cpu) {
    cpu->rd.l = wdc65816_op_readpc(cpu);
    cpu->rd.h = wdc65816_op_readpc(cpu);
    cpu->regs.p.z = ((cpu->rd.w & cpu->regs.a.w) == 0);
}

#define wdc65816_op_read_addr_gen(op)                           \
    void wdc65816_op_read_addr_##op##_b(Wdc65816Cpu* cpu) {     \
        cpu->aa.l = wdc65816_op_readpc(cpu);                    \
        cpu->aa.h = wdc65816_op_readpc(cpu);                    \
        cpu->rd.l = wdc65816_op_readdbr(cpu, cpu->aa.w);        \
        wdc65816_op_##op##_b(cpu);                              \
    }                                                           \
                                                                \
    void wdc65816_op_read_addr_##op##_w(Wdc65816Cpu* cpu) {     \
        cpu->aa.l = wdc65816_op_readpc(cpu);                    \
        cpu->aa.h = wdc65816_op_readpc(cpu);                    \
        cpu->rd.l = wdc65816_op_readdbr(cpu, cpu->aa.w + 0);    \
        cpu->rd.h = wdc65816_op_readdbr(cpu, cpu->aa.w + 1);    \
        wdc65816_op_##op##_w(cpu);                              \
    }

wdc65816_op_read_addr_gen(ora);
wdc65816_op_read_addr_gen(bit);
wdc65816_op_read_addr_gen(and);
wdc65816_op_read_addr_gen(eor);
wdc65816_op_read_addr_gen(adc);
wdc65816_op_read_addr_gen(ldy);
wdc65816_op_read_addr_gen(lda);
wdc65816_op_read_addr_gen(ldx);
wdc65816_op_read_addr_gen(cpy);
wdc65816_op_read_addr_gen(cmp);
wdc65816_op_read_addr_gen(cpx);
wdc65816_op_read_addr_gen(sbc);

#undef wdc65816_op_read_addr_gen

#define wdc65816_op_read_addrx_gen(op)                                  \
    void wdc65816_op_read_addrx_##op##_b(Wdc65816Cpu* cpu) {            \
        cpu->aa.l = wdc65816_op_readpc(cpu);                            \
        cpu->aa.h = wdc65816_op_readpc(cpu);                            \
        cpu->rd.l = wdc65816_op_readdbr(cpu, cpu->aa.w + cpu->regs.x.w); \
        wdc65816_op_##op##_b(cpu);                                      \
    }                                                                   \
                                                                        \
    void wdc65816_op_read_addrx_##op##_w(Wdc65816Cpu* cpu) {            \
        cpu->aa.l = wdc65816_op_readpc(cpu);                            \
        cpu->aa.h = wdc65816_op_readpc(cpu);                            \
        cpu->rd.l = wdc65816_op_readdbr(cpu, cpu->aa.w + cpu->regs.x.w + 0); \
        cpu->rd.h = wdc65816_op_readdbr(cpu, cpu->aa.w + cpu->regs.x.w + 1); \
        wdc65816_op_##op##_w(cpu);                                      \
    }

wdc65816_op_read_addrx_gen(ora);
wdc65816_op_read_addrx_gen(bit);
wdc65816_op_read_addrx_gen(and);
wdc65816_op_read_addrx_gen(eor);
wdc65816_op_read_addrx_gen(adc);
wdc65816_op_read_addrx_gen(ldy);
wdc65816_op_read_addrx_gen(lda);
wdc65816_op_read_addrx_gen(cmp);
wdc65816_op_read_addrx_gen(sbc);

#undef wdc65816_op_read_addrx

#define wdc65816_op_read_addry_gen(op)                                  \
    void wdc65816_op_read_addry_##op##_b(Wdc65816Cpu* cpu) {            \
        cpu->aa.l = wdc65816_op_readpc(cpu);                            \
        cpu->aa.h = wdc65816_op_readpc(cpu);                            \
        cpu->rd.l = wdc65816_op_readdbr(cpu, cpu->aa.w + cpu->regs.y.w); \
        wdc65816_op_##op##_b(cpu);                                      \
    }                                                                   \
                                                                        \
    void wdc65816_op_read_addry_##op##_w(Wdc65816Cpu* cpu) {            \
        cpu->aa.l = wdc65816_op_readpc(cpu);                            \
        cpu->aa.h = wdc65816_op_readpc(cpu);                            \
        cpu->rd.l = wdc65816_op_readdbr(cpu, cpu->aa.w + cpu->regs.y.w + 0); \
        cpu->rd.h = wdc65816_op_readdbr(cpu, cpu->aa.w + cpu->regs.y.w + 1); \
        wdc65816_op_##op##_w(cpu);                                      \
    }

wdc65816_op_read_addry_gen(ora);
wdc65816_op_read_addry_gen(and);
wdc65816_op_read_addry_gen(eor);
wdc65816_op_read_addry_gen(adc);
wdc65816_op_read_addry_gen(ldx);
wdc65816_op_read_addry_gen(lda);
wdc65816_op_read_addry_gen(cmp);
wdc65816_op_read_addry_gen(sbc);

#undef wdc65816_op_read_addry_gen

#define wdc65816_op_read_long_gen(op)                           \
    void wdc65816_op_read_long_##op##_b(Wdc65816Cpu* cpu) {     \
        cpu->aa.l = wdc65816_op_readpc(cpu);                    \
        cpu->aa.h = wdc65816_op_readpc(cpu);                    \
        cpu->aa.b = wdc65816_op_readpc(cpu);                    \
        cpu->rd.l = wdc65816_op_readlong(cpu, cpu->aa.d);       \
        wdc65816_op_##op##_b(cpu);                              \
    }                                                           \
                                                                \
    void wdc65816_op_read_long_##op##_w(Wdc65816Cpu* cpu) {     \
        cpu->aa.l = wdc65816_op_readpc(cpu);                    \
        cpu->aa.h = wdc65816_op_readpc(cpu);                    \
        cpu->aa.b = wdc65816_op_readpc(cpu);                    \
        cpu->rd.l = wdc65816_op_readlong(cpu, cpu->aa.d + 0);   \
        cpu->rd.h = wdc65816_op_readlong(cpu, cpu->aa.d + 1);   \
        wdc65816_op_##op##_w(cpu);                              \
    }

wdc65816_op_read_long_gen(ora);
wdc65816_op_read_long_gen(and);
wdc65816_op_read_long_gen(eor);
wdc65816_op_read_long_gen(adc);
wdc65816_op_read_long_gen(lda);
wdc65816_op_read_long_gen(cmp);
wdc65816_op_read_long_gen(sbc);

#undef wdc65816_op_read_long_b_gen

#define wdc65816_op_read_longx_gen(op)                                  \
    void wdc65816_op_read_longx_##op##_b(Wdc65816Cpu* cpu) {            \
        cpu->aa.l = wdc65816_op_readpc(cpu);                            \
        cpu->aa.h = wdc65816_op_readpc(cpu);                            \
        cpu->aa.b = wdc65816_op_readpc(cpu);                            \
        cpu->rd.l = wdc65816_op_readlong(cpu, cpu->aa.d + cpu->regs.x.w); \
        wdc65816_op_##op##_b(cpu);                                      \
    }                                                                   \
                                                                        \
    void wdc65816_op_read_longx_##op##_w(Wdc65816Cpu* cpu) {            \
        cpu->aa.l = wdc65816_op_readpc(cpu);                            \
        cpu->aa.h = wdc65816_op_readpc(cpu);                            \
        cpu->aa.b = wdc65816_op_readpc(cpu);                            \
        cpu->rd.l = wdc65816_op_readlong(cpu, cpu->aa.d + cpu->regs.x.w + 0); \
        cpu->rd.h = wdc65816_op_readlong(cpu, cpu->aa.d + cpu->regs.x.w + 1); \
        wdc65816_op_##op##_w(cpu);                                      \
    }

wdc65816_op_read_longx_gen(ora);
wdc65816_op_read_longx_gen(and);
wdc65816_op_read_longx_gen(eor);
wdc65816_op_read_longx_gen(adc);
wdc65816_op_read_longx_gen(lda);
wdc65816_op_read_longx_gen(cmp);
wdc65816_op_read_longx_gen(sbc);

#undef wdc65816_op_read_longx_gen

#define wdc65816_op_read_dp_gen(op)                         \
    void wdc65816_op_read_dp_##op##_b(Wdc65816Cpu* cpu) {   \
        cpu->dp = wdc65816_op_readpc(cpu);                  \
        cpu->rd.l = wdc65816_op_readdp(cpu, cpu->dp);       \
        wdc65816_op_##op##_b(cpu);                          \
    }                                                       \
                                                            \
    void wdc65816_op_read_dp_##op##_w(Wdc65816Cpu* cpu) {   \
        cpu->dp = wdc65816_op_readpc(cpu);                  \
        cpu->rd.l = wdc65816_op_readdp(cpu, cpu->dp + 0);   \
        cpu->rd.h = wdc65816_op_readdp(cpu, cpu->dp + 1);   \
        wdc65816_op_##op##_w(cpu);                          \
    }

wdc65816_op_read_dp_gen(ora);
wdc65816_op_read_dp_gen(bit);
wdc65816_op_read_dp_gen(and);
wdc65816_op_read_dp_gen(eor);
wdc65816_op_read_dp_gen(adc);
wdc65816_op_read_dp_gen(ldy);
wdc65816_op_read_dp_gen(lda);
wdc65816_op_read_dp_gen(ldx);
wdc65816_op_read_dp_gen(cpy);
wdc65816_op_read_dp_gen(cmp);
wdc65816_op_read_dp_gen(cpx);
wdc65816_op_read_dp_gen(sbc);

#undef wdc65816_op_read_dp_gen

#define wdc65816_op_read_dpr_gen(op, num)                               \
    void wdc65816_op_read_dpr_##op##_b(Wdc65816Cpu* cpu) {              \
        cpu->dp = wdc65816_op_readpc(cpu);                              \
        cpu->rd.l = wdc65816_op_readdp(cpu, cpu->dp + cpu->regs.r[num].w); \
        wdc65816_op_##op##_b(cpu);                                      \
    }                                                                   \
                                                                        \
    void wdc65816_op_read_dpr_##op##_w(Wdc65816Cpu* cpu) {              \
        cpu->dp = wdc65816_op_readpc(cpu);                              \
        cpu->rd.l = wdc65816_op_readdp(cpu, cpu->dp + cpu->regs.r[num].w + 0); \
        cpu->rd.h = wdc65816_op_readdp(cpu, cpu->dp + cpu->regs.r[num].w + 1); \
        wdc65816_op_##op##_w(cpu);                                      \
    }

wdc65816_op_read_dpr_gen(ora, X);
wdc65816_op_read_dpr_gen(bit, X);
wdc65816_op_read_dpr_gen(and, X);
wdc65816_op_read_dpr_gen(eor, X);
wdc65816_op_read_dpr_gen(adc, X);
wdc65816_op_read_dpr_gen(ldy, X);
wdc65816_op_read_dpr_gen(lda, X);
wdc65816_op_read_dpr_gen(ldx, Y);
wdc65816_op_read_dpr_gen(cmp, X);
wdc65816_op_read_dpr_gen(sbc, X);

#undef wdc65816_op_read_dpr_gen

#define wdc65816_op_read_idp_gen(op)                            \
    void wdc65816_op_read_idp_##op##_b(Wdc65816Cpu* cpu) {      \
        cpu->dp = wdc65816_op_readpc(cpu);                      \
        cpu->aa.l = wdc65816_op_readdp(cpu, cpu->dp + 0);       \
        cpu->aa.h = wdc65816_op_readdp(cpu, cpu->dp + 1);       \
        cpu->rd.l = wdc65816_op_readdbr(cpu, cpu->aa.w);        \
        wdc65816_op_##op##_b(cpu);                              \
    }                                                           \
                                                                \
    void wdc65816_op_read_idp_##op##_w(Wdc65816Cpu* cpu) {      \
        cpu->dp = wdc65816_op_readpc(cpu);                      \
        cpu->aa.l = wdc65816_op_readdp(cpu, cpu->dp + 0);       \
        cpu->aa.h = wdc65816_op_readdp(cpu, cpu->dp + 1);       \
        cpu->rd.l = wdc65816_op_readdbr(cpu, cpu->aa.w + 0);    \
        cpu->rd.h = wdc65816_op_readdbr(cpu, cpu->aa.w + 1);    \
        wdc65816_op_##op##_w(cpu);                              \
    }

wdc65816_op_read_idp_gen(ora);
wdc65816_op_read_idp_gen(and);
wdc65816_op_read_idp_gen(eor);
wdc65816_op_read_idp_gen(adc);
wdc65816_op_read_idp_gen(lda);
wdc65816_op_read_idp_gen(cmp);
wdc65816_op_read_idp_gen(sbc);
#undef wdc65816_op_read_idp_gen

#define wdc65816_op_read_idpx_gen(op)                                   \
    void wdc65816_op_read_idpx_##op##_b(Wdc65816Cpu* cpu) {             \
        cpu->dp = wdc65816_op_readpc(cpu);                              \
        cpu->aa.l = wdc65816_op_readdp(cpu, cpu->dp + cpu->regs.x.w + 0); \
        cpu->aa.h = wdc65816_op_readdp(cpu, cpu->dp + cpu->regs.x.w + 1); \
        cpu->rd.l = wdc65816_op_readdbr(cpu, cpu->aa.w);                \
        wdc65816_op_##op##_b(cpu);                                      \
    }                                                                   \
                                                                        \
    void wdc65816_op_read_idpx_##op##_w(Wdc65816Cpu* cpu) {             \
        cpu->dp = wdc65816_op_readpc(cpu);                              \
        cpu->aa.l = wdc65816_op_readdp(cpu, cpu->dp + cpu->regs.x.w + 0); \
        cpu->aa.h = wdc65816_op_readdp(cpu, cpu->dp + cpu->regs.x.w + 1); \
        cpu->rd.l = wdc65816_op_readdbr(cpu, cpu->aa.w + 0);            \
        cpu->rd.h = wdc65816_op_readdbr(cpu, cpu->aa.w + 1);            \
        wdc65816_op_##op##_w(cpu);                                      \
    }

wdc65816_op_read_idpx_gen(ora);
wdc65816_op_read_idpx_gen(and);
wdc65816_op_read_idpx_gen(eor);
wdc65816_op_read_idpx_gen(adc);
wdc65816_op_read_idpx_gen(lda);
wdc65816_op_read_idpx_gen(cmp);
wdc65816_op_read_idpx_gen(sbc);

#undef wdc65816_op_read_idpx_gen

#define wdc65816_op_read_idpy_gen(op)                                   \
    void wdc65816_op_read_idpy_##op##_b(Wdc65816Cpu* cpu) {             \
        cpu->dp = wdc65816_op_readpc(cpu);                              \
        cpu->aa.l = wdc65816_op_readdp(cpu, cpu->dp + 0);               \
        cpu->aa.h = wdc65816_op_readdp(cpu, cpu->dp + 1);               \
        cpu->rd.l = wdc65816_op_readdbr(cpu, cpu->aa.w + cpu->regs.y.w); \
        wdc65816_op_##op##_b(cpu);                                      \
    }                                                                   \
                                                                        \
    void wdc65816_op_read_idpy_##op##_w(Wdc65816Cpu* cpu) {             \
        cpu->dp = wdc65816_op_readpc(cpu);                              \
        cpu->aa.l = wdc65816_op_readdp(cpu, cpu->dp + 0);               \
        cpu->aa.h = wdc65816_op_readdp(cpu, cpu->dp + 1);               \
        cpu->rd.l = wdc65816_op_readdbr(cpu, cpu->aa.w + cpu->regs.y.w + 0); \
        cpu->rd.h = wdc65816_op_readdbr(cpu, cpu->aa.w + cpu->regs.y.w + 1); \
        wdc65816_op_##op##_w(cpu);                                      \
    }

wdc65816_op_read_idpy_gen(ora);
wdc65816_op_read_idpy_gen(and);
wdc65816_op_read_idpy_gen(eor);
wdc65816_op_read_idpy_gen(adc);
wdc65816_op_read_idpy_gen(lda);
wdc65816_op_read_idpy_gen(cmp);
wdc65816_op_read_idpy_gen(sbc);

#undef wdc65816_op_read_idpy_gen

#define wdc65816_op_read_ildp_gen(op)                           \
    void wdc65816_op_read_ildp_##op##_b(Wdc65816Cpu* cpu) {     \
        cpu->dp = wdc65816_op_readpc(cpu);                      \
        cpu->aa.l = wdc65816_op_readdp(cpu, cpu->dp + 0);       \
        cpu->aa.h = wdc65816_op_readdp(cpu, cpu->dp + 1);       \
        cpu->aa.b = wdc65816_op_readdp(cpu, cpu->dp + 2);       \
        cpu->rd.l = wdc65816_op_readlong(cpu, cpu->aa.d);       \
        wdc65816_op_##op##_b(cpu);                              \
    }                                                           \
                                                                \
    void wdc65816_op_read_ildp_##op##_w(Wdc65816Cpu* cpu) {     \
        cpu->dp = wdc65816_op_readpc(cpu);                      \
        cpu->aa.l = wdc65816_op_readdp(cpu, cpu->dp + 0);       \
        cpu->aa.h = wdc65816_op_readdp(cpu, cpu->dp + 1);       \
        cpu->aa.b = wdc65816_op_readdp(cpu, cpu->dp + 2);       \
        cpu->rd.l = wdc65816_op_readlong(cpu, cpu->aa.d + 0);   \
        cpu->rd.h = wdc65816_op_readlong(cpu, cpu->aa.d + 1);   \
        wdc65816_op_##op##_w(cpu);                              \
    }


wdc65816_op_read_ildp_gen(ora);
wdc65816_op_read_ildp_gen(and);
wdc65816_op_read_ildp_gen(eor);
wdc65816_op_read_ildp_gen(adc);
wdc65816_op_read_ildp_gen(lda);
wdc65816_op_read_ildp_gen(cmp);
wdc65816_op_read_ildp_gen(sbc);
#undef wdc65816_op_read_ildp_gen

#define wdc65816_op_read_ildpy_gen(op)                                  \
    void wdc65816_op_read_ildpy_##op##_b(Wdc65816Cpu* cpu) {            \
        cpu->dp = wdc65816_op_readpc(cpu);                              \
        cpu->aa.l = wdc65816_op_readdp(cpu, cpu->dp + 0);               \
        cpu->aa.h = wdc65816_op_readdp(cpu, cpu->dp + 1);               \
        cpu->aa.b = wdc65816_op_readdp(cpu, cpu->dp + 2);               \
        cpu->rd.l = wdc65816_op_readlong(cpu, cpu->aa.d + cpu->regs.y.w); \
        wdc65816_op_##op##_b(cpu);                                      \
    }                                                                   \
                                                                        \
    void wdc65816_op_read_ildpy_##op##_w(Wdc65816Cpu* cpu) {            \
        cpu->dp = wdc65816_op_readpc(cpu);                              \
        cpu->aa.l = wdc65816_op_readdp(cpu, cpu->dp + 0);               \
        cpu->aa.h = wdc65816_op_readdp(cpu, cpu->dp + 1);               \
        cpu->aa.b = wdc65816_op_readdp(cpu, cpu->dp + 2);               \
        cpu->rd.l = wdc65816_op_readlong(cpu, cpu->aa.d + cpu->regs.y.w + 0); \
        cpu->rd.h = wdc65816_op_readlong(cpu, cpu->aa.d + cpu->regs.y.w + 1); \
        wdc65816_op_##op##_w(cpu);                                      \
    }


wdc65816_op_read_ildpy_gen(ora);
wdc65816_op_read_ildpy_gen(and);
wdc65816_op_read_ildpy_gen(eor);
wdc65816_op_read_ildpy_gen(adc);
wdc65816_op_read_ildpy_gen(lda);
wdc65816_op_read_ildpy_gen(cmp);
wdc65816_op_read_ildpy_gen(sbc);
#undef wdc65816_op_read_ildpy_gen

#define wdc65816_op_read_sr_gen(op)                         \
    void wdc65816_op_read_sr_##op##_b(Wdc65816Cpu* cpu) {   \
        cpu->sp = wdc65816_op_readpc(cpu);                  \
        cpu->rd.l = wdc65816_op_readsp(cpu, cpu->sp);       \
        wdc65816_op_##op##_b(cpu);                          \
    }                                                       \
                                                            \
    void wdc65816_op_read_sr_##op##_w(Wdc65816Cpu* cpu) {   \
        cpu->sp = wdc65816_op_readpc(cpu);                  \
        cpu->rd.l = wdc65816_op_readsp(cpu, cpu->sp + 0);   \
        cpu->rd.h = wdc65816_op_readsp(cpu, cpu->sp + 1);   \
        wdc65816_op_##op##_w(cpu);                          \
    }

wdc65816_op_read_sr_gen(ora);
wdc65816_op_read_sr_gen(and);
wdc65816_op_read_sr_gen(eor);
wdc65816_op_read_sr_gen(adc);
wdc65816_op_read_sr_gen(lda);
wdc65816_op_read_sr_gen(cmp);
wdc65816_op_read_sr_gen(sbc);
#undef wdc65816_op_read_sr_gen


#define wdc65816_op_read_isry_gen(op)                                   \
    void wdc65816_op_read_isry_##op##_b(Wdc65816Cpu* cpu) {             \
        cpu->sp = wdc65816_op_readpc(cpu);                              \
        cpu->aa.l = wdc65816_op_readsp(cpu, cpu->sp + 0);               \
        cpu->aa.h = wdc65816_op_readsp(cpu, cpu->sp + 1);               \
        cpu->rd.l = wdc65816_op_readdbr(cpu, cpu->aa.w + cpu->regs.y.w); \
        wdc65816_op_##op##_b(cpu);                                      \
    }                                                                   \
                                                                        \
    void wdc65816_op_read_isry_##op##_w(Wdc65816Cpu* cpu) {             \
        cpu->sp = wdc65816_op_readpc(cpu);                              \
        cpu->aa.l = wdc65816_op_readsp(cpu, cpu->sp + 0);               \
        cpu->aa.h = wdc65816_op_readsp(cpu, cpu->sp + 1);               \
        cpu->rd.l = wdc65816_op_readdbr(cpu, cpu->aa.w + cpu->regs.y.w + 0); \
        cpu->rd.h = wdc65816_op_readdbr(cpu, cpu->aa.w + cpu->regs.y.w + 1); \
        wdc65816_op_##op##_w(cpu);                                      \
    }

wdc65816_op_read_isry_gen(ora);
wdc65816_op_read_isry_gen(and);
wdc65816_op_read_isry_gen(eor);
wdc65816_op_read_isry_gen(adc);
wdc65816_op_read_isry_gen(lda);
wdc65816_op_read_isry_gen(cmp);
wdc65816_op_read_isry_gen(sbc);
#undef wdc65816_op_read_isry_gen


#undef A
#undef X
#undef Y
#undef Z
#undef S
#undef D
