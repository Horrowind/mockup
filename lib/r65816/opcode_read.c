#include "algorithms.h"
#include "memory.h"

#include "opcode_read.h"

#define A 0
#define X 1
#define Y 2
#define Z 3
#define S 4
#define D 5

#define r65816_op_read_const_gen(op)                        \
    void r65816_op_read_const_##op##_b(r65816_cpu_t* cpu) { \
        cpu->rd.l = r65816_op_readpc(cpu);                  \
        r65816_op_##op##_b(cpu);                            \
    }                                                       \
                                                            \
    void r65816_op_read_const_##op##_w(r65816_cpu_t* cpu) { \
        cpu->rd.l = r65816_op_readpc(cpu);                  \
        cpu->rd.h = r65816_op_readpc(cpu);                  \
        r65816_op_##op##_w(cpu);                            \
    }

r65816_op_read_const_gen(ora);
r65816_op_read_const_gen(bit);
r65816_op_read_const_gen(and);
r65816_op_read_const_gen(eor);
r65816_op_read_const_gen(adc);
r65816_op_read_const_gen(ldy);
r65816_op_read_const_gen(lda);
r65816_op_read_const_gen(ldx);
r65816_op_read_const_gen(cpy);
r65816_op_read_const_gen(cmp);
r65816_op_read_const_gen(cpx);
r65816_op_read_const_gen(sbc);

#undef r65816_op_read_const_gen

void r65816_op_read_bit_const_b(r65816_cpu_t* cpu) {
    cpu->rd.l = r65816_op_readpc(cpu);
    cpu->regs.p.z = ((cpu->rd.l & cpu->regs.a.l) == 0);
}

void r65816_op_read_bit_const_w(r65816_cpu_t* cpu) {
    cpu->rd.l = r65816_op_readpc(cpu);
    cpu->rd.h = r65816_op_readpc(cpu);
    cpu->regs.p.z = ((cpu->rd.w & cpu->regs.a.w) == 0);
}

#define r65816_op_read_addr_gen(op)                         \
    void r65816_op_read_addr_##op##_b(r65816_cpu_t* cpu) {  \
        cpu->aa.l = r65816_op_readpc(cpu);                  \
        cpu->aa.h = r65816_op_readpc(cpu);                  \
        cpu->rd.l = r65816_op_readdbr(cpu, cpu->aa.w);      \
        r65816_op_##op##_b(cpu);                            \
    }                                                       \
                                                            \
    void r65816_op_read_addr_##op##_w(r65816_cpu_t* cpu) {  \
        cpu->aa.l = r65816_op_readpc(cpu);                  \
        cpu->aa.h = r65816_op_readpc(cpu);                  \
        cpu->rd.l = r65816_op_readdbr(cpu, cpu->aa.w + 0);  \
        cpu->rd.h = r65816_op_readdbr(cpu, cpu->aa.w + 1);  \
        r65816_op_##op##_w(cpu);                            \
    }

r65816_op_read_addr_gen(ora);
r65816_op_read_addr_gen(bit);
r65816_op_read_addr_gen(and);
r65816_op_read_addr_gen(eor);
r65816_op_read_addr_gen(adc);
r65816_op_read_addr_gen(ldy);
r65816_op_read_addr_gen(lda);
r65816_op_read_addr_gen(ldx);
r65816_op_read_addr_gen(cpy);
r65816_op_read_addr_gen(cmp);
r65816_op_read_addr_gen(cpx);
r65816_op_read_addr_gen(sbc);

#undef r65816_op_read_addr_gen

#define r65816_op_read_addrx_gen(op)                                    \
    void r65816_op_read_addrx_##op##_b(r65816_cpu_t* cpu) {             \
        cpu->aa.l = r65816_op_readpc(cpu);                              \
        cpu->aa.h = r65816_op_readpc(cpu);                              \
        cpu->rd.l = r65816_op_readdbr(cpu, cpu->aa.w + cpu->regs.x.w);  \
        r65816_op_##op##_b(cpu);                                        \
    }                                                                   \
                                                                        \
    void r65816_op_read_addrx_##op##_w(r65816_cpu_t* cpu) {             \
        cpu->aa.l = r65816_op_readpc(cpu);                              \
        cpu->aa.h = r65816_op_readpc(cpu);                              \
        cpu->rd.l = r65816_op_readdbr(cpu, cpu->aa.w + cpu->regs.x.w + 0); \
        cpu->rd.h = r65816_op_readdbr(cpu, cpu->aa.w + cpu->regs.x.w + 1); \
        r65816_op_##op##_w(cpu);                                        \
    }

r65816_op_read_addrx_gen(ora);
r65816_op_read_addrx_gen(bit);
r65816_op_read_addrx_gen(and);
r65816_op_read_addrx_gen(eor);
r65816_op_read_addrx_gen(adc);
r65816_op_read_addrx_gen(ldy);
r65816_op_read_addrx_gen(lda);
r65816_op_read_addrx_gen(cmp);
r65816_op_read_addrx_gen(sbc);

#undef r65816_op_read_addrx

#define r65816_op_read_addry_gen(op)                                    \
    void r65816_op_read_addry_##op##_b(r65816_cpu_t* cpu) {             \
        cpu->aa.l = r65816_op_readpc(cpu);                              \
        cpu->aa.h = r65816_op_readpc(cpu);                              \
        cpu->rd.l = r65816_op_readdbr(cpu, cpu->aa.w + cpu->regs.y.w);  \
        r65816_op_##op##_b(cpu);                                        \
    }                                                                   \
                                                                        \
    void r65816_op_read_addry_##op##_w(r65816_cpu_t* cpu) {             \
        cpu->aa.l = r65816_op_readpc(cpu);                              \
        cpu->aa.h = r65816_op_readpc(cpu);                              \
        cpu->rd.l = r65816_op_readdbr(cpu, cpu->aa.w + cpu->regs.y.w + 0); \
        cpu->rd.h = r65816_op_readdbr(cpu, cpu->aa.w + cpu->regs.y.w + 1); \
        r65816_op_##op##_w(cpu);                                        \
    }

r65816_op_read_addry_gen(ora);
r65816_op_read_addry_gen(and);
r65816_op_read_addry_gen(eor);
r65816_op_read_addry_gen(adc);
r65816_op_read_addry_gen(ldx);
r65816_op_read_addry_gen(lda);
r65816_op_read_addry_gen(cmp);
r65816_op_read_addry_gen(sbc);

#undef r65816_op_read_addry_gen

#define r65816_op_read_long_gen(op)                         \
    void r65816_op_read_long_##op##_b(r65816_cpu_t* cpu) {  \
        cpu->aa.l = r65816_op_readpc(cpu);                  \
        cpu->aa.h = r65816_op_readpc(cpu);                  \
        cpu->aa.b = r65816_op_readpc(cpu);                  \
        cpu->rd.l = r65816_op_readlong(cpu, cpu->aa.d);     \
        r65816_op_##op##_b(cpu);                            \
    }                                                       \
                                                            \
    void r65816_op_read_long_##op##_w(r65816_cpu_t* cpu) {  \
        cpu->aa.l = r65816_op_readpc(cpu);                  \
        cpu->aa.h = r65816_op_readpc(cpu);                  \
        cpu->aa.b = r65816_op_readpc(cpu);                  \
        cpu->rd.l = r65816_op_readlong(cpu, cpu->aa.d + 0); \
        cpu->rd.h = r65816_op_readlong(cpu, cpu->aa.d + 1); \
        r65816_op_##op##_w(cpu);                            \
    }

r65816_op_read_long_gen(ora);
r65816_op_read_long_gen(and);
r65816_op_read_long_gen(eor);
r65816_op_read_long_gen(adc);
r65816_op_read_long_gen(lda);
r65816_op_read_long_gen(cmp);
r65816_op_read_long_gen(sbc);

#undef r65816_op_read_long_b_gen

#define r65816_op_read_longx_gen(op)                                    \
    void r65816_op_read_longx_##op##_b(r65816_cpu_t* cpu) {             \
        cpu->aa.l = r65816_op_readpc(cpu);                              \
        cpu->aa.h = r65816_op_readpc(cpu);                              \
        cpu->aa.b = r65816_op_readpc(cpu);                              \
        cpu->rd.l = r65816_op_readlong(cpu, cpu->aa.d + cpu->regs.x.w); \
        r65816_op_##op##_b(cpu);                                        \
    }                                                                   \
                                                                        \
    void r65816_op_read_longx_##op##_w(r65816_cpu_t* cpu) {             \
        cpu->aa.l = r65816_op_readpc(cpu);                              \
        cpu->aa.h = r65816_op_readpc(cpu);                              \
        cpu->aa.b = r65816_op_readpc(cpu);                              \
        cpu->rd.l = r65816_op_readlong(cpu, cpu->aa.d + cpu->regs.x.w + 0); \
        cpu->rd.h = r65816_op_readlong(cpu, cpu->aa.d + cpu->regs.x.w + 1); \
        r65816_op_##op##_w(cpu);                                        \
    }

r65816_op_read_longx_gen(ora);
r65816_op_read_longx_gen(and);
r65816_op_read_longx_gen(eor);
r65816_op_read_longx_gen(adc);
r65816_op_read_longx_gen(lda);
r65816_op_read_longx_gen(cmp);
r65816_op_read_longx_gen(sbc);

#undef r65816_op_read_longx_gen

#define r65816_op_read_dp_gen(op)                                       \
    void r65816_op_read_dp_##op##_b(r65816_cpu_t* cpu) {                \
        cpu->dp = r65816_op_readpc(cpu);                                \
        cpu->rd.l = r65816_op_readdp(cpu, cpu->dp);                     \
        r65816_op_##op##_b(cpu);                                        \
    }                                                                   \
                                                                        \
    void r65816_op_read_dp_##op##_w(r65816_cpu_t* cpu) {                \
        cpu->dp = r65816_op_readpc(cpu);                                \
        cpu->rd.l = r65816_op_readdp(cpu, cpu->dp + 0);                 \
        cpu->rd.h = r65816_op_readdp(cpu, cpu->dp + 1);                 \
        r65816_op_##op##_w(cpu);                                        \
    }

r65816_op_read_dp_gen(ora);
r65816_op_read_dp_gen(bit);
r65816_op_read_dp_gen(and);
r65816_op_read_dp_gen(eor);
r65816_op_read_dp_gen(adc);
r65816_op_read_dp_gen(ldy);
r65816_op_read_dp_gen(lda);
r65816_op_read_dp_gen(ldx);
r65816_op_read_dp_gen(cpy);
r65816_op_read_dp_gen(cmp);
r65816_op_read_dp_gen(cpx);
r65816_op_read_dp_gen(sbc);

#undef r65816_op_read_dp_gen

#define r65816_op_read_dpr_gen(op, num)                                 \
    void r65816_op_read_dpr_##op##_b(r65816_cpu_t* cpu) {               \
        cpu->dp = r65816_op_readpc(cpu);                                \
        cpu->rd.l = r65816_op_readdp(cpu, cpu->dp + cpu->regs.r[num].w); \
        r65816_op_##op##_b(cpu);                                        \
    }                                                                   \
                                                                        \
    void r65816_op_read_dpr_##op##_w(r65816_cpu_t* cpu) {               \
        cpu->dp = r65816_op_readpc(cpu);                                \
        cpu->rd.l = r65816_op_readdp(cpu, cpu->dp + cpu->regs.r[num].w + 0); \
        cpu->rd.h = r65816_op_readdp(cpu, cpu->dp + cpu->regs.r[num].w + 1); \
        r65816_op_##op##_w(cpu);                                        \
    }

r65816_op_read_dpr_gen(ora, X);
r65816_op_read_dpr_gen(bit, X);
r65816_op_read_dpr_gen(and, X);
r65816_op_read_dpr_gen(eor, X);
r65816_op_read_dpr_gen(adc, X);
r65816_op_read_dpr_gen(ldy, X);
r65816_op_read_dpr_gen(lda, X);
r65816_op_read_dpr_gen(ldx, Y);
r65816_op_read_dpr_gen(cmp, X);
r65816_op_read_dpr_gen(sbc, X);

#undef r65816_op_read_dpr_gen

#define r65816_op_read_idp_gen(op)                                      \
    void r65816_op_read_idp_##op##_b(r65816_cpu_t* cpu) {               \
        cpu->dp = r65816_op_readpc(cpu);                                \
        cpu->aa.l = r65816_op_readdp(cpu, cpu->dp + 0);                 \
        cpu->aa.h = r65816_op_readdp(cpu, cpu->dp + 1);                 \
        cpu->rd.l = r65816_op_readdbr(cpu, cpu->aa.w);                  \
        r65816_op_##op##_b(cpu);                                        \
    }                                                                   \
                                                                        \
    void r65816_op_read_idp_##op##_w(r65816_cpu_t* cpu) {               \
        cpu->dp = r65816_op_readpc(cpu);                                \
        cpu->aa.l = r65816_op_readdp(cpu, cpu->dp + 0);                 \
        cpu->aa.h = r65816_op_readdp(cpu, cpu->dp + 1);                 \
        cpu->rd.l = r65816_op_readdbr(cpu, cpu->aa.w + 0);              \
        cpu->rd.h = r65816_op_readdbr(cpu, cpu->aa.w + 1);              \
        r65816_op_##op##_w(cpu);                                        \
    }

r65816_op_read_idp_gen(ora);
r65816_op_read_idp_gen(and);
r65816_op_read_idp_gen(eor);
r65816_op_read_idp_gen(adc);
r65816_op_read_idp_gen(lda);
r65816_op_read_idp_gen(cmp);
r65816_op_read_idp_gen(sbc);
#undef r65816_op_read_idp_gen

#define r65816_op_read_idpx_gen(op)                                     \
    void r65816_op_read_idpx_##op##_b(r65816_cpu_t* cpu) {              \
        cpu->dp = r65816_op_readpc(cpu);                                \
        cpu->aa.l = r65816_op_readdp(cpu, cpu->dp + cpu->regs.x.w + 0); \
        cpu->aa.h = r65816_op_readdp(cpu, cpu->dp + cpu->regs.x.w + 1); \
        cpu->rd.l = r65816_op_readdbr(cpu, cpu->aa.w);                  \
        r65816_op_##op##_b(cpu);                                        \
    }                                                                   \
                                                                        \
    void r65816_op_read_idpx_##op##_w(r65816_cpu_t* cpu) {              \
        cpu->dp = r65816_op_readpc(cpu);                                \
        cpu->aa.l = r65816_op_readdp(cpu, cpu->dp + cpu->regs.x.w + 0); \
        cpu->aa.h = r65816_op_readdp(cpu, cpu->dp + cpu->regs.x.w + 1); \
        cpu->rd.l = r65816_op_readdbr(cpu, cpu->aa.w + 0);              \
        cpu->rd.h = r65816_op_readdbr(cpu, cpu->aa.w + 1);              \
        r65816_op_##op##_w(cpu);                                        \
    }

r65816_op_read_idpx_gen(ora);
r65816_op_read_idpx_gen(and);
r65816_op_read_idpx_gen(eor);
r65816_op_read_idpx_gen(adc);
r65816_op_read_idpx_gen(lda);
r65816_op_read_idpx_gen(cmp);
r65816_op_read_idpx_gen(sbc);

#undef r65816_op_read_idpx_gen

#define r65816_op_read_idpy_gen(op)                                     \
    void r65816_op_read_idpy_##op##_b(r65816_cpu_t* cpu) {              \
        cpu->dp = r65816_op_readpc(cpu);                                \
        cpu->aa.l = r65816_op_readdp(cpu, cpu->dp + 0);                 \
        cpu->aa.h = r65816_op_readdp(cpu, cpu->dp + 1);                 \
        cpu->rd.l = r65816_op_readdbr(cpu, cpu->aa.w + cpu->regs.y.w);  \
        r65816_op_##op##_b(cpu);                                        \
    }                                                                   \
                                                                        \
    void r65816_op_read_idpy_##op##_w(r65816_cpu_t* cpu) {              \
        cpu->dp = r65816_op_readpc(cpu);                                \
        cpu->aa.l = r65816_op_readdp(cpu, cpu->dp + 0);                 \
        cpu->aa.h = r65816_op_readdp(cpu, cpu->dp + 1);                 \
        cpu->rd.l = r65816_op_readdbr(cpu, cpu->aa.w + cpu->regs.y.w + 0); \
        cpu->rd.h = r65816_op_readdbr(cpu, cpu->aa.w + cpu->regs.y.w + 1); \
        r65816_op_##op##_w(cpu);                                        \
    }

r65816_op_read_idpy_gen(ora);
r65816_op_read_idpy_gen(and);
r65816_op_read_idpy_gen(eor);
r65816_op_read_idpy_gen(adc);
r65816_op_read_idpy_gen(lda);
r65816_op_read_idpy_gen(cmp);
r65816_op_read_idpy_gen(sbc);

#undef r65816_op_read_idpy_gen

#define r65816_op_read_ildp_gen(op)                         \
    void r65816_op_read_ildp_##op##_b(r65816_cpu_t* cpu) {  \
        cpu->dp = r65816_op_readpc(cpu);                    \
        cpu->aa.l = r65816_op_readdp(cpu, cpu->dp + 0);     \
        cpu->aa.h = r65816_op_readdp(cpu, cpu->dp + 1);     \
        cpu->aa.b = r65816_op_readdp(cpu, cpu->dp + 2);     \
        cpu->rd.l = r65816_op_readlong(cpu, cpu->aa.d);     \
        r65816_op_##op##_b(cpu);                            \
    }                                                       \
                                                            \
    void r65816_op_read_ildp_##op##_w(r65816_cpu_t* cpu) {  \
        cpu->dp = r65816_op_readpc(cpu);                    \
        cpu->aa.l = r65816_op_readdp(cpu, cpu->dp + 0);     \
        cpu->aa.h = r65816_op_readdp(cpu, cpu->dp + 1);     \
        cpu->aa.b = r65816_op_readdp(cpu, cpu->dp + 2);     \
        cpu->rd.l = r65816_op_readlong(cpu, cpu->aa.d + 0); \
        cpu->rd.h = r65816_op_readlong(cpu, cpu->aa.d + 1); \
        r65816_op_##op##_w(cpu);                            \
    }


r65816_op_read_ildp_gen(ora);
r65816_op_read_ildp_gen(and);
r65816_op_read_ildp_gen(eor);
r65816_op_read_ildp_gen(adc);
r65816_op_read_ildp_gen(lda);
r65816_op_read_ildp_gen(cmp);
r65816_op_read_ildp_gen(sbc);
#undef r65816_op_read_ildp_gen

#define r65816_op_read_ildpy_gen(op)                                    \
    void r65816_op_read_ildpy_##op##_b(r65816_cpu_t* cpu) {             \
        cpu->dp = r65816_op_readpc(cpu);                                \
        cpu->aa.l = r65816_op_readdp(cpu, cpu->dp + 0);                 \
        cpu->aa.h = r65816_op_readdp(cpu, cpu->dp + 1);                 \
        cpu->aa.b = r65816_op_readdp(cpu, cpu->dp + 2);                 \
        cpu->rd.l = r65816_op_readlong(cpu, cpu->aa.d + cpu->regs.y.w); \
        r65816_op_##op##_b(cpu);                                        \
    }                                                                   \
                                                                        \
    void r65816_op_read_ildpy_##op##_w(r65816_cpu_t* cpu) {             \
        cpu->dp = r65816_op_readpc(cpu);                                \
        cpu->aa.l = r65816_op_readdp(cpu, cpu->dp + 0);                 \
        cpu->aa.h = r65816_op_readdp(cpu, cpu->dp + 1);                 \
        cpu->aa.b = r65816_op_readdp(cpu, cpu->dp + 2);                 \
        cpu->rd.l = r65816_op_readlong(cpu, cpu->aa.d + cpu->regs.y.w + 0); \
        cpu->rd.h = r65816_op_readlong(cpu, cpu->aa.d + cpu->regs.y.w + 1); \
        r65816_op_##op##_w(cpu);                                        \
    }


r65816_op_read_ildpy_gen(ora);
r65816_op_read_ildpy_gen(and);
r65816_op_read_ildpy_gen(eor);
r65816_op_read_ildpy_gen(adc);
r65816_op_read_ildpy_gen(lda);
r65816_op_read_ildpy_gen(cmp);
r65816_op_read_ildpy_gen(sbc);
#undef r65816_op_read_ildpy_gen

#define r65816_op_read_sr_gen(op)                           \
    void r65816_op_read_sr_##op##_b(r65816_cpu_t* cpu) {    \
        cpu->sp = r65816_op_readpc(cpu);                    \
        cpu->rd.l = r65816_op_readsp(cpu, cpu->sp);         \
        r65816_op_##op##_b(cpu);                            \
    }                                                       \
                                                            \
    void r65816_op_read_sr_##op##_w(r65816_cpu_t* cpu) {    \
        cpu->sp = r65816_op_readpc(cpu);                    \
        cpu->rd.l = r65816_op_readsp(cpu, cpu->sp + 0);     \
        cpu->rd.h = r65816_op_readsp(cpu, cpu->sp + 1);     \
        r65816_op_##op##_w(cpu);                            \
    }

r65816_op_read_sr_gen(ora);
r65816_op_read_sr_gen(and);
r65816_op_read_sr_gen(eor);
r65816_op_read_sr_gen(adc);
r65816_op_read_sr_gen(lda);
r65816_op_read_sr_gen(cmp);
r65816_op_read_sr_gen(sbc);
#undef r65816_op_read_sr_gen


#define r65816_op_read_isry_gen(op)                                     \
    void r65816_op_read_isry_##op##_b(r65816_cpu_t* cpu) {              \
        cpu->sp = r65816_op_readpc(cpu);                                \
        cpu->aa.l = r65816_op_readsp(cpu, cpu->sp + 0);                 \
        cpu->aa.h = r65816_op_readsp(cpu, cpu->sp + 1);                 \
        cpu->rd.l = r65816_op_readdbr(cpu, cpu->aa.w + cpu->regs.y.w);  \
        r65816_op_##op##_b(cpu);                                        \
    }                                                                   \
                                                                        \
    void r65816_op_read_isry_##op##_w(r65816_cpu_t* cpu) {              \
        cpu->sp = r65816_op_readpc(cpu);                                \
        cpu->aa.l = r65816_op_readsp(cpu, cpu->sp + 0);                 \
        cpu->aa.h = r65816_op_readsp(cpu, cpu->sp + 1);                 \
        cpu->rd.l = r65816_op_readdbr(cpu, cpu->aa.w + cpu->regs.y.w + 0); \
        cpu->rd.h = r65816_op_readdbr(cpu, cpu->aa.w + cpu->regs.y.w + 1); \
        r65816_op_##op##_w(cpu);                                        \
    }

r65816_op_read_isry_gen(ora);
r65816_op_read_isry_gen(and);
r65816_op_read_isry_gen(eor);
r65816_op_read_isry_gen(adc);
r65816_op_read_isry_gen(lda);
r65816_op_read_isry_gen(cmp);
r65816_op_read_isry_gen(sbc);
#undef r65816_op_read_isry_gen


#undef A
#undef X
#undef Y
#undef Z
#undef S
#undef D
