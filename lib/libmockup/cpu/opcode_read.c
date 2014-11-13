#include "cpu/algorithms.h"
#include "cpu/memory.h"
#include "cpu/opcode_read.h"

#define A 0
#define X 1
#define Y 2
#define Z 3
#define S 4
#define D 5

#define op_read_const_gen(op)                                           \
    void op_read_const_##op##_b(cpu_t* cpu) {                           \
        cpu->rd.l = op_readpc(cpu);                                     \
        op_##op##_b(cpu);                                               \
    }                                                                   \
                                                                        \
    void op_read_const_##op##_w(cpu_t* cpu) {                           \
        cpu->rd.l = op_readpc(cpu);                                     \
        cpu->rd.h = op_readpc(cpu);                                     \
        op_##op##_w(cpu);                                               \
    }

op_read_const_gen(ora);
op_read_const_gen(bit);
op_read_const_gen(and);
op_read_const_gen(eor);
op_read_const_gen(adc);
op_read_const_gen(ldy);
op_read_const_gen(lda);
op_read_const_gen(ldx);
op_read_const_gen(cpy);
op_read_const_gen(cmp);
op_read_const_gen(cpx);
op_read_const_gen(sbc);

#undef op_read_const_gen

void op_read_bit_const_b(cpu_t* cpu) {
    cpu->rd.l = op_readpc(cpu);
    cpu->regs.p.z = ((cpu->rd.l & cpu->regs.a.l) == 0);
}

void op_read_bit_const_w(cpu_t* cpu) {
    cpu->rd.l = op_readpc(cpu);
    cpu->rd.h = op_readpc(cpu);
    cpu->regs.p.z = ((cpu->rd.w & cpu->regs.a.w) == 0);
}

#define op_read_addr_gen(op)                                            \
    void op_read_addr_##op##_b(cpu_t* cpu) {                            \
        cpu->aa.l = op_readpc(cpu);                                     \
        cpu->aa.h = op_readpc(cpu);                                     \
        cpu->rd.l = op_readdbr(cpu, cpu->aa.w);                         \
        op_##op##_w(cpu);                                               \
    }                                                                   \
                                                                        \
    void op_read_addr_##op##_w(cpu_t* cpu) {                            \
        cpu->aa.l = op_readpc(cpu);                                     \
        cpu->aa.h = op_readpc(cpu);                                     \
        cpu->rd.l = op_readdbr(cpu, cpu->aa.w + 0);                     \
        cpu->rd.h = op_readdbr(cpu, cpu->aa.w + 1);                     \
        op_##op##_w(cpu);                                               \
    }

op_read_addr_gen(ora);
op_read_addr_gen(bit);
op_read_addr_gen(and);
op_read_addr_gen(eor);
op_read_addr_gen(adc);
op_read_addr_gen(ldy);
op_read_addr_gen(lda);
op_read_addr_gen(ldx);
op_read_addr_gen(cpy);
op_read_addr_gen(cmp);
op_read_addr_gen(cpx);
op_read_addr_gen(sbc);

#undef op_read_addr_gen

#define op_read_addrx_gen(op)                                           \
    void op_read_addrx_##op##_b(cpu_t* cpu) {                           \
        cpu->aa.l = op_readpc(cpu);                                     \
        cpu->aa.h = op_readpc(cpu);                                     \
        cpu->rd.l = op_readdbr(cpu, cpu->aa.w + cpu->regs.x.w);         \
        op_##op##_b(cpu);                                               \
    }                                                                   \
                                                                        \
    void op_read_addrx_##op##_w(cpu_t* cpu) {                           \
        cpu->aa.l = op_readpc(cpu);                                     \
        cpu->aa.h = op_readpc(cpu);                                     \
        cpu->rd.l = op_readdbr(cpu, cpu->aa.w + cpu->regs.x.w + 0);     \
        cpu->rd.h = op_readdbr(cpu, cpu->aa.w + cpu->regs.x.w + 1);     \
        op_##op##_w(cpu);                                               \
    }

op_read_addrx_gen(ora);
op_read_addrx_gen(bit);
op_read_addrx_gen(and);
op_read_addrx_gen(eor);
op_read_addrx_gen(adc);
op_read_addrx_gen(ldy);
op_read_addrx_gen(lda);
op_read_addrx_gen(cmp);
op_read_addrx_gen(sbc);

#undef op_read_addrx

#define op_read_addry_gen(op)                                           \
    void op_read_addry_##op##_b(cpu_t* cpu) {                           \
        cpu->aa.l = op_readpc(cpu);                                     \
        cpu->aa.h = op_readpc(cpu);                                     \
        cpu->rd.l = op_readdbr(cpu, cpu->aa.w + cpu->regs.y.w);         \
        op_##op##_b(cpu);                                               \
    }                                                                   \
                                                                        \
    void op_read_addry_##op##_w(cpu_t* cpu) {                           \
        cpu->aa.l = op_readpc(cpu);                                     \
        cpu->aa.h = op_readpc(cpu);                                     \
        cpu->rd.l = op_readdbr(cpu, cpu->aa.w + cpu->regs.y.w + 0);     \
        cpu->rd.h = op_readdbr(cpu, cpu->aa.w + cpu->regs.y.w + 1);     \
        op_##op##_w(cpu);                                               \
    }

op_read_addry_gen(ora);
op_read_addry_gen(and);
op_read_addry_gen(eor);
op_read_addry_gen(adc);
op_read_addry_gen(ldx);
op_read_addry_gen(lda);
op_read_addry_gen(cmp);
op_read_addry_gen(sbc);

#undef op_read_addry_gen

#define op_read_long_gen(op)                                            \
    void op_read_long_##op##_b(cpu_t* cpu) {                            \
        cpu->aa.l = op_readpc(cpu);                                     \
        cpu->aa.h = op_readpc(cpu);                                     \
        cpu->aa.b = op_readpc(cpu);                                     \
        cpu->rd.l = op_readlong(cpu, cpu->aa.d);                        \
        op_##op##_b(cpu);                                               \
    }                                                                   \
                                                                        \
    void op_read_long_##op##_w(cpu_t* cpu) {                            \
        cpu->aa.l = op_readpc(cpu);                                     \
        cpu->aa.h = op_readpc(cpu);                                     \
        cpu->aa.b = op_readpc(cpu);                                     \
        cpu->rd.l = op_readlong(cpu, cpu->aa.d + 0);                    \
        cpu->rd.h = op_readlong(cpu, cpu->aa.d + 1);                    \
        op_##op##_w(cpu);                                               \
    }

op_read_long_gen(ora);
op_read_long_gen(and);
op_read_long_gen(eor);
op_read_long_gen(adc);
op_read_long_gen(lda);
op_read_long_gen(cmp);
op_read_long_gen(sbc);

#undef op_read_long_b_gen

#define op_read_longx_gen(op)                                           \
    void op_read_longx_##op##_b(cpu_t* cpu) {                           \
        cpu->aa.l = op_readpc(cpu);                                     \
        cpu->aa.h = op_readpc(cpu);                                     \
        cpu->aa.b = op_readpc(cpu);                                     \
        cpu->rd.l = op_readlong(cpu, cpu->aa.d + cpu->regs.x.w);        \
        op_##op##_b(cpu);                                               \
    }                                                                   \
                                                                        \
    void op_read_longx_##op##_w(cpu_t* cpu) {                           \
        cpu->aa.l = op_readpc(cpu);                                     \
        cpu->aa.h = op_readpc(cpu);                                     \
        cpu->aa.b = op_readpc(cpu);                                     \
        cpu->rd.l = op_readlong(cpu, cpu->aa.d + cpu->regs.x.w + 0);    \
        cpu->rd.h = op_readlong(cpu, cpu->aa.d + cpu->regs.x.w + 1);    \
        op_##op##_w(cpu);                                               \
    }

op_read_longx_gen(ora);
op_read_longx_gen(and);
op_read_longx_gen(eor);
op_read_longx_gen(adc);
op_read_longx_gen(lda);
op_read_longx_gen(cmp);
op_read_longx_gen(sbc);

#undef op_read_longx_gen

#define op_read_dp_gen(op)                                              \
    void op_read_dp_##op##_b(cpu_t* cpu) {                              \
        cpu->dp = op_readpc(cpu);                                       \
        cpu->rd.l = op_readdp(cpu, cpu->dp);                            \
        op_##op##_b(cpu);                                               \
    }                                                                   \
                                                                        \
    void op_read_dp_##op##_w(cpu_t* cpu) {                              \
        cpu->dp = op_readpc(cpu);                                       \
        cpu->rd.l = op_readdp(cpu, cpu->dp + 0);                        \
        cpu->rd.h = op_readdp(cpu, cpu->dp + 1);                        \
        op_##op##_w(cpu);                                               \
    }

op_read_dp_gen(ora);
op_read_dp_gen(bit);
op_read_dp_gen(and);
op_read_dp_gen(eor);
op_read_dp_gen(adc);
op_read_dp_gen(ldy);
op_read_dp_gen(lda);
op_read_dp_gen(ldx);
op_read_dp_gen(cpy);
op_read_dp_gen(cmp);
op_read_dp_gen(cpx);
op_read_dp_gen(sbc);

#undef op_read_dp_gen

#define op_read_dpr_gen(op, num)                                        \
    void op_read_dpr_##op##_b(cpu_t* cpu) {                             \
        cpu->dp = op_readpc(cpu);                                       \
        cpu->rd.l = op_readdp(cpu, cpu->dp + cpu->regs.r[num].w);       \
        op_##op##_b(cpu);                                               \
    }                                                                   \
                                                                        \
    void op_read_dpr_##op##_w(cpu_t* cpu) {                             \
        cpu->dp = op_readpc(cpu);                                       \
        cpu->rd.l = op_readdp(cpu, cpu->dp + cpu->regs.r[num].w + 0);   \
        cpu->rd.h = op_readdp(cpu, cpu->dp + cpu->regs.r[num].w + 1);   \
        op_##op##_w(cpu);                                               \
    }

op_read_dpr_gen(ora, X);
op_read_dpr_gen(bit, X);
op_read_dpr_gen(and, X);
op_read_dpr_gen(eor, X);
op_read_dpr_gen(adc, X);
op_read_dpr_gen(ldy, X);
op_read_dpr_gen(lda, X);
op_read_dpr_gen(ldx, Y);
op_read_dpr_gen(cmp, X);
op_read_dpr_gen(sbc, X);

#undef op_read_dpr_gen

#define op_read_idp_gen(op)                                             \
    void op_read_idp_##op##_b(cpu_t* cpu) {                             \
        cpu->dp = op_readpc(cpu);                                       \
        cpu->aa.l = op_readdp(cpu, cpu->dp + 0);                        \
        cpu->aa.h = op_readdp(cpu, cpu->dp + 1);                        \
        cpu->rd.l = op_readdbr(cpu, cpu->aa.w);                         \
        op_##op##_b(cpu);                                               \
    }                                                                   \
                                                                        \
    void op_read_idp_##op##_w(cpu_t* cpu) {                             \
        cpu->dp = op_readpc(cpu);                                       \
        cpu->aa.l = op_readdp(cpu, cpu->dp + 0);                        \
        cpu->aa.h = op_readdp(cpu, cpu->dp + 1);                        \
        cpu->rd.l = op_readdbr(cpu, cpu->aa.w + 0);                     \
        cpu->rd.h = op_readdbr(cpu, cpu->aa.w + 1);                     \
        op_##op##_w(cpu);                                               \
    }

op_read_idp_gen(ora);
op_read_idp_gen(and);
op_read_idp_gen(eor);
op_read_idp_gen(adc);
op_read_idp_gen(lda);
op_read_idp_gen(cmp);
op_read_idp_gen(sbc);
#undef op_read_idp_gen

#define op_read_idpx_gen(op)                                            \
    void op_read_idpx_##op##_b(cpu_t* cpu) {                            \
        cpu->dp = op_readpc(cpu);                                       \
        cpu->aa.l = op_readdp(cpu, cpu->dp + cpu->regs.x.w + 0);        \
        cpu->aa.h = op_readdp(cpu, cpu->dp + cpu->regs.x.w + 1);        \
        cpu->rd.l = op_readdbr(cpu, cpu->aa.w);                         \
        op_##op##_b(cpu);                                               \
    }                                                                   \
                                                                        \
    void op_read_idpx_##op##_w(cpu_t* cpu) {                            \
        cpu->dp = op_readpc(cpu);                                       \
        cpu->aa.l = op_readdp(cpu, cpu->dp + cpu->regs.x.w + 0);        \
        cpu->aa.h = op_readdp(cpu, cpu->dp + cpu->regs.x.w + 1);        \
        cpu->rd.l = op_readdbr(cpu, cpu->aa.w + 0);                     \
        cpu->rd.h = op_readdbr(cpu, cpu->aa.w + 1);                     \
        op_##op##_w(cpu);                                               \
    }

op_read_idpx_gen(ora);
op_read_idpx_gen(and);
op_read_idpx_gen(eor);
op_read_idpx_gen(adc);
op_read_idpx_gen(lda);
op_read_idpx_gen(cmp);
op_read_idpx_gen(sbc);

#undef op_read_idpx_gen

#define op_read_idpy_gen(op)                                            \
    void op_read_idpy_##op##_b(cpu_t* cpu) {                            \
        cpu->dp = op_readpc(cpu);                                       \
        cpu->aa.l = op_readdp(cpu, cpu->dp + 0);                        \
        cpu->aa.h = op_readdp(cpu, cpu->dp + 1);                        \
        cpu->rd.l = op_readdbr(cpu, cpu->aa.w + cpu->regs.y.w);         \
        op_##op##_b(cpu);                                               \
    }                                                                   \
                                                                        \
    void op_read_idpy_##op##_w(cpu_t* cpu) {                            \
        cpu->dp = op_readpc(cpu);                                       \
        cpu->aa.l = op_readdp(cpu, cpu->dp + 0);                        \
        cpu->aa.h = op_readdp(cpu, cpu->dp + 1);                        \
        cpu->rd.l = op_readdbr(cpu, cpu->aa.w + cpu->regs.y.w + 0);     \
        cpu->rd.h = op_readdbr(cpu, cpu->aa.w + cpu->regs.y.w + 1);     \
        op_##op##_w(cpu);                                               \
    }

op_read_idpy_gen(ora);
op_read_idpy_gen(and);
op_read_idpy_gen(eor);
op_read_idpy_gen(adc);
op_read_idpy_gen(lda);
op_read_idpy_gen(cmp);
op_read_idpy_gen(sbc);

#undef op_read_idpy_gen

#define op_read_ildp_gen(op)                                            \
    void op_read_ildp_##op##_b(cpu_t* cpu) {                            \
        cpu->dp = op_readpc(cpu);                                       \
        cpu->aa.l = op_readdp(cpu, cpu->dp + 0);                        \
        cpu->aa.h = op_readdp(cpu, cpu->dp + 1);                        \
        cpu->aa.b = op_readdp(cpu, cpu->dp + 2);                        \
        cpu->rd.l = op_readlong(cpu, cpu->aa.d);                        \
        op_##op##_b(cpu);                                               \
    }                                                                   \
                                                                        \
    void op_read_ildp_##op##_w(cpu_t* cpu) {                            \
        cpu->dp = op_readpc(cpu);                                       \
        cpu->aa.l = op_readdp(cpu, cpu->dp + 0);                        \
        cpu->aa.h = op_readdp(cpu, cpu->dp + 1);                        \
        cpu->aa.b = op_readdp(cpu, cpu->dp + 2);                        \
        cpu->rd.l = op_readlong(cpu, cpu->aa.d + 0);                    \
        cpu->rd.h = op_readlong(cpu, cpu->aa.d + 1);                    \
        op_##op##_w(cpu);                                               \
    }


op_read_ildp_gen(ora);
op_read_ildp_gen(and);
op_read_ildp_gen(eor);
op_read_ildp_gen(adc);
op_read_ildp_gen(lda);
op_read_ildp_gen(cmp);
op_read_ildp_gen(sbc);
#undef op_read_ildp_gen

#define op_read_ildpy_gen(op)                                           \
    void op_read_ildpy_##op##_b(cpu_t* cpu) {                           \
        cpu->dp = op_readpc(cpu);                                       \
        cpu->aa.l = op_readdp(cpu, cpu->dp + 0);                        \
        cpu->aa.h = op_readdp(cpu, cpu->dp + 1);                        \
        cpu->aa.b = op_readdp(cpu, cpu->dp + 2);                        \
        cpu->rd.l = op_readlong(cpu, cpu->aa.d + cpu->regs.y.w);        \
        op_##op##_b(cpu);                                               \
    }                                                                   \
                                                                        \
    void op_read_ildpy_##op##_w(cpu_t* cpu) {                           \
        cpu->dp = op_readpc(cpu);                                       \
        cpu->aa.l = op_readdp(cpu, cpu->dp + 0);                        \
        cpu->aa.h = op_readdp(cpu, cpu->dp + 1);                        \
        cpu->aa.b = op_readdp(cpu, cpu->dp + 2);                        \
        cpu->rd.l = op_readlong(cpu, cpu->aa.d + cpu->regs.y.w + 0);    \
        cpu->rd.h = op_readlong(cpu, cpu->aa.d + cpu->regs.y.w + 1);    \
        op_##op##_w(cpu);                                               \
    }


op_read_ildpy_gen(ora);
op_read_ildpy_gen(and);
op_read_ildpy_gen(eor);
op_read_ildpy_gen(adc);
op_read_ildpy_gen(lda);
op_read_ildpy_gen(cmp);
op_read_ildpy_gen(sbc);
#undef op_read_ildpy_gen

#define op_read_sr_gen(op)                                              \
    void op_read_sr_##op##_b(cpu_t* cpu) {                              \
        cpu->sp = op_readpc(cpu);                                       \
        cpu->rd.l = op_readsp(cpu, cpu->sp);                            \
        op_##op##_b(cpu);                                               \
    }                                                                   \
                                                                        \
    void op_read_sr_##op##_w(cpu_t* cpu) {                              \
        cpu->sp = op_readpc(cpu);                                       \
        cpu->rd.l = op_readsp(cpu, cpu->sp + 0);                        \
        cpu->rd.h = op_readsp(cpu, cpu->sp + 1);                        \
        op_##op##_w(cpu);                                               \
    }

op_read_sr_gen(ora);
op_read_sr_gen(and);
op_read_sr_gen(eor);
op_read_sr_gen(adc);
op_read_sr_gen(lda);
op_read_sr_gen(cmp);
op_read_sr_gen(sbc);
#undef op_read_sr_gen


#define op_read_isry_gen(op)                                            \
    void op_read_isry_##op##_b(cpu_t* cpu) {                            \
        cpu->sp = op_readpc(cpu);                                       \
        cpu->aa.l = op_readsp(cpu, cpu->sp + 0);                        \
        cpu->aa.h = op_readsp(cpu, cpu->sp + 1);                        \
        cpu->rd.l = op_readdbr(cpu, cpu->aa.w + cpu->regs.y.w);         \
        op_##op##_b(cpu);                                               \
    }                                                                   \
                                                                        \
    void op_read_isry_##op##_w(cpu_t* cpu) {                            \
        cpu->sp = op_readpc(cpu);                                       \
        cpu->aa.l = op_readsp(cpu, cpu->sp + 0);                        \
        cpu->aa.h = op_readsp(cpu, cpu->sp + 1);                        \
        cpu->rd.l = op_readdbr(cpu, cpu->aa.w + cpu->regs.y.w + 0);     \
        cpu->rd.h = op_readdbr(cpu, cpu->aa.w + cpu->regs.y.w + 1);     \
        op_##op##_w(cpu);                                               \
    }

op_read_isry_gen(ora);
op_read_isry_gen(and);
op_read_isry_gen(eor);
op_read_isry_gen(adc);
op_read_isry_gen(lda);
op_read_isry_gen(cmp);
op_read_isry_gen(sbc);
#undef op_read_isry_gen


#undef A
#undef X
#undef Y
#undef Z
#undef S
#undef D
