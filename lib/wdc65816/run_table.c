#include "cpu.h"
#include "algorithms.h"
#include "disassembler.h"
#include "memory.h"
#include "table.h"

#include "opcode_misc.h"
#include "opcode_pc.h"
#include "opcode_rmw.h"
#include "opcode_read.h"
#include "opcode_write.h"

void wdc65816_cpu_run_from(WDC65816Cpu* cpu, u32 address) {
    cpu->regs.pc.d = address;
    wdc65816_cpu_run(cpu);
}

void wdc65816_cpu_run(WDC65816Cpu* cpu) {
    cpu->stop_execution = 0;
    while(!cpu->stop_execution) {
        wdc65816_cpu_step(cpu);
        cpu->stop_execution |= wdc65816_breakpoint_list_is_hit(&cpu->breakpoints_exec, cpu->regs.pc.d);
    }
}

void wdc65816_cpu_run_jsr(WDC65816Cpu* cpu, u32 address) {
    cpu->stop_execution = 0;
    cpu->regs.pc.d = address;
    cpu->regs.s.w = 0x1FF;
    while((cpu->regs.s.w != 0x1FF || wdc65816_cpu_read(cpu, cpu->regs.pc.d) != 0x60) && !cpu->stop_execution) {
        wdc65816_cpu_step(cpu);
        cpu->stop_execution |= wdc65816_breakpoint_list_is_hit(&cpu->breakpoints_exec, cpu->regs.pc.d);
    }
}

void wdc65816_cpu_run_jsl(WDC65816Cpu* cpu, u32 address) {
    cpu->stop_execution = 0;
    cpu->regs.pc.d = address;
    cpu->regs.s.w = 0x1FF;
    while((cpu->regs.s.w != 0x1FF || wdc65816_cpu_read(cpu, cpu->regs.pc.d) != 0x6B) && !cpu->stop_execution) {
        wdc65816_cpu_step(cpu);
        cpu->stop_execution |= wdc65816_breakpoint_list_is_hit(&cpu->breakpoints_exec, cpu->regs.pc.d);
    }
}


enum {
    table_EM =    0,  // 8-bit accumulator,  8-bit index (emulation mode)
    table_MX =  256,  // 8-bit accumulator,  8-bit index
    table_Mx =  512,  // 8-bit accumulator, 16-bit index
    table_mX =  768,  //16-bit accumulator,  8-bit index
    table_mx = 1024,  //16-bit accumulator, 16-bit index
};

void (*op_table[])(struct WDC65816Cpu*) = {
#define opA(id, name)                           \
    [table_EM + id] = &wdc65816_op_##name,        \
    [table_MX + id] = &wdc65816_op_##name,        \
    [table_Mx + id] = &wdc65816_op_##name,        \
    [table_mX + id] = &wdc65816_op_##name,        \
    [table_mx + id] = &wdc65816_op_##name,        \

#define opE(id, name)                           \
    [table_EM + id] = &wdc65816_op_##name##_e,    \
    [table_MX + id] = &wdc65816_op_##name##_n,    \
    [table_Mx + id] = &wdc65816_op_##name##_n,    \
    [table_mX + id] = &wdc65816_op_##name##_n,    \
    [table_mx + id] = &wdc65816_op_##name##_n,    \

#define opM(id, name)                           \
    [table_EM + id] = &wdc65816_op_##name##_b,    \
    [table_MX + id] = &wdc65816_op_##name##_b,    \
    [table_Mx + id] = &wdc65816_op_##name##_b,    \
    [table_mX + id] = &wdc65816_op_##name##_w,    \
    [table_mx + id] = &wdc65816_op_##name##_w,    \

#define opX(id, name)                           \
    [table_EM + id] = &wdc65816_op_##name##_b,    \
    [table_MX + id] = &wdc65816_op_##name##_b,    \
    [table_Mx + id] = &wdc65816_op_##name##_w,    \
    [table_mX + id] = &wdc65816_op_##name##_b,    \
    [table_mx + id] = &wdc65816_op_##name##_w,    \

#include "opcode_table.c"

#undef opA
#undef opE
#undef opM
#undef opX

};

void wdc65816_update_table(WDC65816Cpu* cpu) {
    enum {
        table_EM =    0,  // 8-bit accumulator,  8-bit index (emulation mode)
        table_MX =  256,  // 8-bit accumulator,  8-bit index
        table_Mx =  512,  // 8-bit accumulator, 16-bit index
        table_mX =  768,  //16-bit accumulator,  8-bit index
        table_mx = 1024,  //16-bit accumulator, 16-bit index
    };
    if(cpu->regs.e) {
        cpu->opcode_table = &op_table[table_EM];
    } else if(cpu->regs.p.m) {
        if(cpu->regs.p.x) {
            cpu->opcode_table = &op_table[table_MX];
        } else {
            cpu->opcode_table = &op_table[table_Mx];
        }
    } else {
        if(cpu->regs.p.x) {
            cpu->opcode_table = &op_table[table_mX];
        } else {
            cpu->opcode_table = &op_table[table_mx];
        }
    }
}
