#include "cpu/memory.h"
#include "cpu/opcode_pc.h"
#include "cpu/table.h"

#define op_branch_gen(name, bit, val)                            \
    inline void op_branch_##name(cpu_t* cpu) {                   \
        /*if((bool)(cpu->regs.p & bit) != val) { */              \
        if((cpu->regs.p.b & bit) != val) {                       \
            cpu->rd.l = op_readpc(cpu);                          \
        } else {                                                 \
            cpu->rd.l = op_readpc(cpu);                          \
            cpu->regs.pc.w = cpu->regs.pc.d + (int8_t)cpu->rd.l; \
        }                                                        \
    }

op_branch_gen(bpl, 0x80, 0);
op_branch_gen(bmi, 0x80, 1);
op_branch_gen(bvc, 0x40, 0);
op_branch_gen(bvs, 0x40, 1);
op_branch_gen(bcc, 0x01, 0);
op_branch_gen(bcs, 0x01, 1);
op_branch_gen(bne, 0x02, 0);
op_branch_gen(beq, 0x02, 1);
#undef op_branch_gen


inline void op_bra(cpu_t* cpu) {
    cpu->rd.l = op_readpc(cpu);
    cpu->aa.w = cpu->regs.pc.d + (int8_t)cpu->rd.l;
    cpu->regs.pc.w = cpu->aa.w;
}

inline void op_brl(cpu_t* cpu) {
    cpu->rd.l = op_readpc(cpu);
    cpu->rd.h = op_readpc(cpu);
    cpu->regs.pc.w = cpu->regs.pc.d + (int16_t)cpu->rd.w;
}

inline void op_jmp_addr(cpu_t* cpu) {
    cpu->rd.l = op_readpc(cpu);
    cpu->rd.h = op_readpc(cpu);
    cpu->regs.pc.w = cpu->rd.w;
}

inline void op_jmp_long(cpu_t* cpu) {
    cpu->rd.l = op_readpc(cpu);
    cpu->rd.h = op_readpc(cpu);
    cpu->rd.b = op_readpc(cpu);
    cpu->regs.pc.d = cpu->rd.d & 0xffffff;
}

inline void op_jmp_iaddr(cpu_t* cpu) {
    cpu->aa.l = op_readpc(cpu);
    cpu->aa.h = op_readpc(cpu);
    cpu->rd.l = op_readaddr(cpu, cpu->aa.w + 0);
    cpu->rd.h = op_readaddr(cpu, cpu->aa.w + 1);
    cpu->regs.pc.w = cpu->rd.w;
}

inline void op_jmp_iaddrx(cpu_t* cpu) {
    cpu->aa.l = op_readpc(cpu);
    cpu->aa.h = op_readpc(cpu);
    cpu->rd.l = op_readpbr(cpu, cpu->aa.w + cpu->regs.x.w + 0);
    cpu->rd.h = op_readpbr(cpu, cpu->aa.w + cpu->regs.x.w + 1);
    cpu->regs.pc.w = cpu->rd.w;
}

inline void op_jmp_iladdr(cpu_t* cpu) {
    cpu->aa.l = op_readpc(cpu);
    cpu->aa.h = op_readpc(cpu);
    cpu->rd.l = op_readaddr(cpu, cpu->aa.w + 0);
    cpu->rd.h = op_readaddr(cpu, cpu->aa.w + 1);
    cpu->rd.b = op_readaddr(cpu, cpu->aa.w + 2);
    cpu->regs.pc.d = cpu->rd.d & 0xffffff;
}

inline void op_jsr_addr(cpu_t* cpu) {
    cpu->aa.l = op_readpc(cpu);
    cpu->aa.h = op_readpc(cpu);
    (cpu->regs.pc.w)--;
    op_writestack(cpu, cpu->regs.pc.h);
    op_writestack(cpu, cpu->regs.pc.l);
    cpu->regs.pc.w = cpu->aa.w;
}

inline void op_jsr_long_e(cpu_t* cpu) {
    cpu->aa.l = op_readpc(cpu);
    cpu->aa.h = op_readpc(cpu);
    op_writestackn(cpu, cpu->regs.pc.b);
    cpu->aa.b = op_readpc(cpu);
    (cpu->regs.pc.w)--;
    op_writestackn(cpu, cpu->regs.pc.h);
    op_writestackn(cpu, cpu->regs.pc.l);
    cpu->regs.pc.d = cpu->aa.d & 0xffffff;
    cpu->regs.s.h = 0x01;
}

inline void op_jsr_long_n(cpu_t* cpu) {
    cpu->aa.l = op_readpc(cpu);
    cpu->aa.h = op_readpc(cpu);
    op_writestackn(cpu, cpu->regs.pc.b);
    cpu->aa.b = op_readpc(cpu);
    (cpu->regs.pc.w)--;
    op_writestackn(cpu, cpu->regs.pc.h);
    op_writestackn(cpu, cpu->regs.pc.l);
    cpu->regs.pc.d = cpu->aa.d & 0xffffff;
}

inline void op_jsr_iaddrx_e(cpu_t* cpu) {
    cpu->aa.l = op_readpc(cpu);
    op_writestackn(cpu, cpu->regs.pc.h);
    op_writestackn(cpu, cpu->regs.pc.l);
    cpu->aa.h = op_readpc(cpu);
    cpu->rd.l = op_readpbr(cpu, cpu->aa.w + cpu->regs.x.w + 0);
    cpu->rd.h = op_readpbr(cpu, cpu->aa.w + cpu->regs.x.w + 1);
    cpu->regs.pc.w = cpu->rd.w;
    cpu->regs.s.h = 0x01;
}

inline void op_jsr_iaddrx_n(cpu_t* cpu) {
    cpu->aa.l = op_readpc(cpu);
    op_writestackn(cpu, cpu->regs.pc.h);
    op_writestackn(cpu, cpu->regs.pc.l);
    cpu->aa.h = op_readpc(cpu);
    cpu->rd.l = op_readpbr(cpu, cpu->aa.w + cpu->regs.x.w + 0);
    cpu->rd.h = op_readpbr(cpu, cpu->aa.w + cpu->regs.x.w + 1);
    cpu->regs.pc.w = cpu->rd.w;
}

inline void op_rti_e(cpu_t* cpu) {
    cpu->regs.p.b = op_readstack(cpu) | 0x30;
    cpu->rd.l = op_readstack(cpu);
    cpu->rd.h = op_readstack(cpu);
    cpu->regs.pc.w = cpu->rd.w;
}

inline void op_rti_n(cpu_t* cpu) {
    cpu->regs.p.b = op_readstack(cpu);
    if(cpu->regs.p.x) {
        cpu->regs.x.h = 0x00;
        cpu->regs.y.h = 0x00;
    }
    cpu->rd.l = op_readstack(cpu);
    cpu->rd.h = op_readstack(cpu);
    cpu->rd.b = op_readstack(cpu);
    cpu->regs.pc.d = cpu->rd.d & 0xffffff;
    update_table(cpu);
}

inline void op_rts(cpu_t* cpu) {
    cpu->rd.l = op_readstack(cpu);
    cpu->rd.h = op_readstack(cpu);
    cpu->regs.pc.w = ++(cpu->rd.w);
}

inline void op_rtl_e(cpu_t* cpu) {
    cpu->rd.l = op_readstackn(cpu);
    cpu->rd.h = op_readstackn(cpu);
    cpu->rd.b = op_readstackn(cpu);
    cpu->regs.pc.b = cpu->rd.b;
    cpu->regs.pc.w = ++(cpu->rd.w);
    cpu->regs.s.h = 0x01;
}

inline void op_rtl_n(cpu_t* cpu) {
    cpu->rd.l = op_readstackn(cpu);
    cpu->rd.h = op_readstackn(cpu);
    cpu->rd.b = op_readstackn(cpu);
    cpu->regs.pc.b = cpu->rd.b;
    cpu->regs.pc.w = ++(cpu->rd.w);
}
