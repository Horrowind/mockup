#include "algorithms.h"
#include "memory.h"
#include "table.h"

#include "opcode_pc.h"

#define r65816_op_branch_gen(name, bit, val)                            \
    inline void r65816_op_branch_##name(r65816_cpu_t* cpu) {                   \
        /*if((bool)(cpu->regs.p & bit) != val) { */              \
        if((cpu->regs.p.b & bit) != val) {                       \
            cpu->rd.l = r65816_op_readpc(cpu);                          \
        } else {                                                 \
            cpu->rd.l = r65816_op_readpc(cpu);                          \
            cpu->regs.pc.w = cpu->regs.pc.d + (int8_t)cpu->rd.l; \
        }                                                        \
    }

r65816_op_branch_gen(bpl, 0x80, 0);
r65816_op_branch_gen(bmi, 0x80, 1);
r65816_op_branch_gen(bvc, 0x40, 0);
r65816_op_branch_gen(bvs, 0x40, 1);
r65816_op_branch_gen(bcc, 0x01, 0);
r65816_op_branch_gen(bcs, 0x01, 1);
r65816_op_branch_gen(bne, 0x02, 0);
r65816_op_branch_gen(beq, 0x02, 1);
#undef r65816_op_branch_gen


inline void r65816_op_bra(r65816_cpu_t* cpu) {
    cpu->rd.l = r65816_op_readpc(cpu);
    cpu->aa.w = cpu->regs.pc.d + (int8_t)cpu->rd.l;
    cpu->regs.pc.w = cpu->aa.w;
}

inline void r65816_op_brl(r65816_cpu_t* cpu) {
    cpu->rd.l = r65816_op_readpc(cpu);
    cpu->rd.h = r65816_op_readpc(cpu);
    cpu->regs.pc.w = cpu->regs.pc.d + (int16_t)cpu->rd.w;
}

inline void r65816_op_jmp_addr(r65816_cpu_t* cpu) {
    cpu->rd.l = r65816_op_readpc(cpu);
    cpu->rd.h = r65816_op_readpc(cpu);
    cpu->regs.pc.w = cpu->rd.w;
}

inline void r65816_op_jmp_long(r65816_cpu_t* cpu) {
    cpu->rd.l = r65816_op_readpc(cpu);
    cpu->rd.h = r65816_op_readpc(cpu);
    cpu->rd.b = r65816_op_readpc(cpu);
    cpu->regs.pc.d = cpu->rd.d & 0xffffff;
}

inline void r65816_op_jmp_iaddr(r65816_cpu_t* cpu) {
    cpu->aa.l = r65816_op_readpc(cpu);
    cpu->aa.h = r65816_op_readpc(cpu);
    cpu->rd.l = r65816_op_readaddr(cpu, cpu->aa.w + 0);
    cpu->rd.h = r65816_op_readaddr(cpu, cpu->aa.w + 1);
    cpu->regs.pc.w = cpu->rd.w;
}

inline void r65816_op_jmp_iaddrx(r65816_cpu_t* cpu) {
    cpu->aa.l = r65816_op_readpc(cpu);
    cpu->aa.h = r65816_op_readpc(cpu);
    cpu->rd.l = r65816_op_readpbr(cpu, cpu->aa.w + cpu->regs.x.w + 0);
    cpu->rd.h = r65816_op_readpbr(cpu, cpu->aa.w + cpu->regs.x.w + 1);
    cpu->regs.pc.w = cpu->rd.w;
}

inline void r65816_op_jmp_iladdr(r65816_cpu_t* cpu) {
    cpu->aa.l = r65816_op_readpc(cpu);
    cpu->aa.h = r65816_op_readpc(cpu);
    cpu->rd.l = r65816_op_readaddr(cpu, cpu->aa.w + 0);
    cpu->rd.h = r65816_op_readaddr(cpu, cpu->aa.w + 1);
    cpu->rd.b = r65816_op_readaddr(cpu, cpu->aa.w + 2);
    cpu->regs.pc.d = cpu->rd.d & 0xffffff;
}

inline void r65816_op_jsr_addr(r65816_cpu_t* cpu) {
    cpu->aa.l = r65816_op_readpc(cpu);
    cpu->aa.h = r65816_op_readpc(cpu);
    (cpu->regs.pc.w)--;
    r65816_op_writestack(cpu, cpu->regs.pc.h);
    r65816_op_writestack(cpu, cpu->regs.pc.l);
    cpu->regs.pc.w = cpu->aa.w;
}

inline void r65816_op_jsr_long_e(r65816_cpu_t* cpu) {
    cpu->aa.l = r65816_op_readpc(cpu);
    cpu->aa.h = r65816_op_readpc(cpu);
    r65816_op_writestackn(cpu, cpu->regs.pc.b);
    cpu->aa.b = r65816_op_readpc(cpu);
    (cpu->regs.pc.w)--;
    r65816_op_writestackn(cpu, cpu->regs.pc.h);
    r65816_op_writestackn(cpu, cpu->regs.pc.l);
    cpu->regs.pc.d = cpu->aa.d & 0xffffff;
    cpu->regs.s.h = 0x01;
}

inline void r65816_op_jsr_long_n(r65816_cpu_t* cpu) {
    cpu->aa.l = r65816_op_readpc(cpu);
    cpu->aa.h = r65816_op_readpc(cpu);
    r65816_op_writestackn(cpu, cpu->regs.pc.b);
    cpu->aa.b = r65816_op_readpc(cpu);
    (cpu->regs.pc.w)--;
    r65816_op_writestackn(cpu, cpu->regs.pc.h);
    r65816_op_writestackn(cpu, cpu->regs.pc.l);
    cpu->regs.pc.d = cpu->aa.d & 0xffffff;
}

inline void r65816_op_jsr_iaddrx_e(r65816_cpu_t* cpu) {
    cpu->aa.l = r65816_op_readpc(cpu);
    r65816_op_writestackn(cpu, cpu->regs.pc.h);
    r65816_op_writestackn(cpu, cpu->regs.pc.l);
    cpu->aa.h = r65816_op_readpc(cpu);
    cpu->rd.l = r65816_op_readpbr(cpu, cpu->aa.w + cpu->regs.x.w + 0);
    cpu->rd.h = r65816_op_readpbr(cpu, cpu->aa.w + cpu->regs.x.w + 1);
    cpu->regs.pc.w = cpu->rd.w;
    cpu->regs.s.h = 0x01;
}

inline void r65816_op_jsr_iaddrx_n(r65816_cpu_t* cpu) {
    cpu->aa.l = r65816_op_readpc(cpu);
    r65816_op_writestackn(cpu, cpu->regs.pc.h);
    r65816_op_writestackn(cpu, cpu->regs.pc.l);
    cpu->aa.h = r65816_op_readpc(cpu);
    cpu->rd.l = r65816_op_readpbr(cpu, cpu->aa.w + cpu->regs.x.w + 0);
    cpu->rd.h = r65816_op_readpbr(cpu, cpu->aa.w + cpu->regs.x.w + 1);
    cpu->regs.pc.w = cpu->rd.w;
}

inline void r65816_op_rti_e(r65816_cpu_t* cpu) {
    cpu->regs.p.b = r65816_op_readstack(cpu) | 0x30;
    cpu->rd.l = r65816_op_readstack(cpu);
    cpu->rd.h = r65816_op_readstack(cpu);
    cpu->regs.pc.w = cpu->rd.w;
}

inline void r65816_op_rti_n(r65816_cpu_t* cpu) {
    cpu->regs.p.b = r65816_op_readstack(cpu);
    if(cpu->regs.p.x) {
        cpu->regs.x.h = 0x00;
        cpu->regs.y.h = 0x00;
    }
    cpu->rd.l = r65816_op_readstack(cpu);
    cpu->rd.h = r65816_op_readstack(cpu);
    cpu->rd.b = r65816_op_readstack(cpu);
    cpu->regs.pc.d = cpu->rd.d & 0xffffff;
    update_table(cpu);
}

inline void r65816_op_rts(r65816_cpu_t* cpu) {
    cpu->rd.l = r65816_op_readstack(cpu);
    cpu->rd.h = r65816_op_readstack(cpu);
    cpu->regs.pc.w = ++(cpu->rd.w);
}

inline void r65816_op_rtl_e(r65816_cpu_t* cpu) {
    cpu->rd.l = r65816_op_readstackn(cpu);
    cpu->rd.h = r65816_op_readstackn(cpu);
    cpu->rd.b = r65816_op_readstackn(cpu);
    cpu->regs.pc.b = cpu->rd.b;
    cpu->regs.pc.w = ++(cpu->rd.w);
    cpu->regs.s.h = 0x01;
}

inline void r65816_op_rtl_n(r65816_cpu_t* cpu) {
    cpu->rd.l = r65816_op_readstackn(cpu);
    cpu->rd.h = r65816_op_readstackn(cpu);
    cpu->rd.b = r65816_op_readstackn(cpu);
    cpu->regs.pc.b = cpu->rd.b;
    cpu->regs.pc.w = ++(cpu->rd.w);
}
