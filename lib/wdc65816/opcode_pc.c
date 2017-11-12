#include "algorithms.h"
#include "memory.h"
#include "table.h"

#define wdc65816_op_branch_gen(name, bit, val)                        \
    void wdc65816_op_branch_##name(wdc65816_cpu_t* cpu) {      \
        /*if((bool)(cpu->regs.p & bit) != val) { */                 \
        if((1 && (cpu->regs.p.b & bit)) != val) {                   \
            cpu->rd.l = wdc65816_op_readpc(cpu);                      \
        } else {                                                    \
            cpu->rd.l = wdc65816_op_readpc(cpu);                      \
            cpu->regs.pc.w = cpu->regs.pc.d + (i8)cpu->rd.l;        \
        }                                                           \
    }

wdc65816_op_branch_gen(bpl, 0x80, 0);
wdc65816_op_branch_gen(bmi, 0x80, 1);
wdc65816_op_branch_gen(bvc, 0x40, 0);
wdc65816_op_branch_gen(bvs, 0x40, 1);
wdc65816_op_branch_gen(bcc, 0x01, 0);
wdc65816_op_branch_gen(bcs, 0x01, 1);
//wdc65816_op_branch_gen(bne, 0x02, 0);
wdc65816_op_branch_gen(beq, 0x02, 1);
#undef wdc65816_op_branch_gen
void wdc65816_op_branch_bne(wdc65816_cpu_t* cpu) {      \
    if((1 && (cpu->regs.p.b & 0x02)) != 0) {                        \
        cpu->rd.l = wdc65816_op_readpc(cpu);                          \
    } else {                                                        \
        cpu->rd.l = wdc65816_op_readpc(cpu);                          \
        cpu->regs.pc.w = cpu->regs.pc.d + (i8)cpu->rd.l;            \
    }                                                               \
}

/* void wdc65816_op_branch_beq(wdc65816_cpu_t* cpu) {               \ */
/*     if((1 && (cpu->regs.p.b & 0x02)) != 1) {                        \ */
/*         cpu->rd.l = wdc65816_op_readpc(cpu);                          \ */
/*     } else {                                                        \ */
/*         cpu->rd.l = wdc65816_op_readpc(cpu);                          \ */
/*         cpu->regs.pc.w = cpu->regs.pc.d + (i8)cpu->rd.l;        \ */
/*     }                                                               \ */
/* } */

void wdc65816_op_bra(wdc65816_cpu_t* cpu) {
    cpu->rd.l = wdc65816_op_readpc(cpu);
    cpu->aa.w = cpu->regs.pc.d + (i8)cpu->rd.l;
    cpu->regs.pc.w = cpu->aa.w;
}

void wdc65816_op_brl(wdc65816_cpu_t* cpu) {
    cpu->rd.l = wdc65816_op_readpc(cpu);
    cpu->rd.h = wdc65816_op_readpc(cpu);
    cpu->regs.pc.w = cpu->regs.pc.d + (i16)cpu->rd.w;
}

void wdc65816_op_jmp_addr(wdc65816_cpu_t* cpu) {
    cpu->rd.l = wdc65816_op_readpc(cpu);
    cpu->rd.h = wdc65816_op_readpc(cpu);
    cpu->regs.pc.w = cpu->rd.w;
}

void wdc65816_op_jmp_long(wdc65816_cpu_t* cpu) {
    cpu->rd.l = wdc65816_op_readpc(cpu);
    cpu->rd.h = wdc65816_op_readpc(cpu);
    cpu->rd.b = wdc65816_op_readpc(cpu);
    cpu->regs.pc.d = cpu->rd.d & 0xffffff;
}

void wdc65816_op_jmp_iaddr(wdc65816_cpu_t* cpu) {
    cpu->aa.l = wdc65816_op_readpc(cpu);
    cpu->aa.h = wdc65816_op_readpc(cpu);
    cpu->rd.l = wdc65816_op_readaddr(cpu, cpu->aa.w + 0);
    cpu->rd.h = wdc65816_op_readaddr(cpu, cpu->aa.w + 1);
    cpu->regs.pc.w = cpu->rd.w;
}

void wdc65816_op_jmp_iaddrx(wdc65816_cpu_t* cpu) {
    cpu->aa.l = wdc65816_op_readpc(cpu);
    cpu->aa.h = wdc65816_op_readpc(cpu);
    cpu->rd.l = wdc65816_op_readpbr(cpu, cpu->aa.w + cpu->regs.x.w + 0);
    cpu->rd.h = wdc65816_op_readpbr(cpu, cpu->aa.w + cpu->regs.x.w + 1);
    cpu->regs.pc.w = cpu->rd.w;
}

void wdc65816_op_jmp_iladdr(wdc65816_cpu_t* cpu) {
    cpu->aa.l = wdc65816_op_readpc(cpu);
    cpu->aa.h = wdc65816_op_readpc(cpu);
    cpu->rd.l = wdc65816_op_readaddr(cpu, cpu->aa.w + 0);
    cpu->rd.h = wdc65816_op_readaddr(cpu, cpu->aa.w + 1);
    cpu->rd.b = wdc65816_op_readaddr(cpu, cpu->aa.w + 2);
    cpu->regs.pc.d = cpu->rd.d & 0xffffff;
}

void wdc65816_op_jsr_addr(wdc65816_cpu_t* cpu) {
    cpu->aa.l = wdc65816_op_readpc(cpu);
    cpu->aa.h = wdc65816_op_readpc(cpu);
    (cpu->regs.pc.w)--;
    wdc65816_op_writestack(cpu, cpu->regs.pc.h);
    wdc65816_op_writestack(cpu, cpu->regs.pc.l);
    cpu->regs.pc.w = cpu->aa.w;
}

void wdc65816_op_jsr_long_e(wdc65816_cpu_t* cpu) {
    cpu->aa.l = wdc65816_op_readpc(cpu);
    cpu->aa.h = wdc65816_op_readpc(cpu);
    wdc65816_op_writestackn(cpu, cpu->regs.pc.b);
    cpu->aa.b = wdc65816_op_readpc(cpu);
    (cpu->regs.pc.w)--;
    wdc65816_op_writestackn(cpu, cpu->regs.pc.h);
    wdc65816_op_writestackn(cpu, cpu->regs.pc.l);
    cpu->regs.pc.d = cpu->aa.d & 0xffffff;
    cpu->regs.s.h = 0x01;
}

void wdc65816_op_jsr_long_n(wdc65816_cpu_t* cpu) {
    cpu->aa.l = wdc65816_op_readpc(cpu);
    cpu->aa.h = wdc65816_op_readpc(cpu);
    wdc65816_op_writestackn(cpu, cpu->regs.pc.b);
    cpu->aa.b = wdc65816_op_readpc(cpu);
    (cpu->regs.pc.w)--;
    wdc65816_op_writestackn(cpu, cpu->regs.pc.h);
    wdc65816_op_writestackn(cpu, cpu->regs.pc.l);
    cpu->regs.pc.d = cpu->aa.d & 0xffffff;
}

void wdc65816_op_jsr_iaddrx_e(wdc65816_cpu_t* cpu) {
    cpu->aa.l = wdc65816_op_readpc(cpu);
    wdc65816_op_writestackn(cpu, cpu->regs.pc.h);
    wdc65816_op_writestackn(cpu, cpu->regs.pc.l);
    cpu->aa.h = wdc65816_op_readpc(cpu);
    cpu->rd.l = wdc65816_op_readpbr(cpu, cpu->aa.w + cpu->regs.x.w + 0);
    cpu->rd.h = wdc65816_op_readpbr(cpu, cpu->aa.w + cpu->regs.x.w + 1);
    cpu->regs.pc.w = cpu->rd.w;
    cpu->regs.s.h = 0x01;
}

void wdc65816_op_jsr_iaddrx_n(wdc65816_cpu_t* cpu) {
    cpu->aa.l = wdc65816_op_readpc(cpu);
    wdc65816_op_writestackn(cpu, cpu->regs.pc.h);
    wdc65816_op_writestackn(cpu, cpu->regs.pc.l);
    cpu->aa.h = wdc65816_op_readpc(cpu);
    cpu->rd.l = wdc65816_op_readpbr(cpu, cpu->aa.w + cpu->regs.x.w + 0);
    cpu->rd.h = wdc65816_op_readpbr(cpu, cpu->aa.w + cpu->regs.x.w + 1);
    cpu->regs.pc.w = cpu->rd.w;
}

void wdc65816_op_rti_e(wdc65816_cpu_t* cpu) {
    cpu->regs.p.b = wdc65816_op_readstack(cpu) | 0x30;
    cpu->rd.l = wdc65816_op_readstack(cpu);
    cpu->rd.h = wdc65816_op_readstack(cpu);
    cpu->regs.pc.w = cpu->rd.w;
}

void wdc65816_op_rti_n(wdc65816_cpu_t* cpu) {
    cpu->regs.p.b = wdc65816_op_readstack(cpu);
    if(cpu->regs.p.x) {
        cpu->regs.x.h = 0x00;
        cpu->regs.y.h = 0x00;
    }
    cpu->rd.l = wdc65816_op_readstack(cpu);
    cpu->rd.h = wdc65816_op_readstack(cpu);
    cpu->rd.b = wdc65816_op_readstack(cpu);
    cpu->regs.pc.d = cpu->rd.d & 0xffffff;
}

void wdc65816_op_rts(wdc65816_cpu_t* cpu) {
    cpu->rd.l = wdc65816_op_readstack(cpu);
    cpu->rd.h = wdc65816_op_readstack(cpu);
    cpu->regs.pc.w = ++(cpu->rd.w);
}

void wdc65816_op_rtl_e(wdc65816_cpu_t* cpu) {
    cpu->rd.l = wdc65816_op_readstackn(cpu);
    cpu->rd.h = wdc65816_op_readstackn(cpu);
    cpu->rd.b = wdc65816_op_readstackn(cpu);
    cpu->regs.pc.b = cpu->rd.b;
    cpu->regs.pc.w = ++(cpu->rd.w);
    cpu->regs.s.h = 0x01;
}

void wdc65816_op_rtl_n(wdc65816_cpu_t* cpu) {
    cpu->rd.l = wdc65816_op_readstackn(cpu);
    cpu->rd.h = wdc65816_op_readstackn(cpu);
    cpu->rd.b = wdc65816_op_readstackn(cpu);
    cpu->regs.pc.b = cpu->rd.b;
    cpu->regs.pc.w = ++(cpu->rd.w);
}
