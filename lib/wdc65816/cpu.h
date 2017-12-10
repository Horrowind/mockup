#ifndef R65816_CPU_H
#define R65816_CPU_H

#define DEBUG_PRINT_CPU_STATE

#include "base/base.h"

#include "breakpoint.h"
#include "registers.h"
#include "rom.h"
#ifdef LLVM
#include "dynarec.h"
#endif
typedef struct WDC65816Cpu {
    WDC65816Rom* rom;

    WDC65816Regs regs;
    WDC65816Reg24 aa, rd;
    u8 sp, dp;
    u8 breakpoint_data;
    u8 stop_execution;
    u32 breakpoint_address;
#ifdef DEBUG_PRINT_CPU_STATE
    u32 debug;
#endif
    void (**opcode_table)(struct WDC65816Cpu*);
    
    WDC65816BreakpointList breakpoints_exec;
    WDC65816BreakpointList breakpoints_read;
    WDC65816BreakpointList breakpoints_write;

    WDC65816ReadFunction  read;
    WDC65816WriteFunction write;
    WDC65816PtrFunction   ptr;

    WDC65816Mapper read_mapper;
    WDC65816Mapper write_mapper;
    u8* ram;
    u8* sreg;
#ifdef LLVM
    WDC65816BBMap bb_map;
#endif
    
} WDC65816Cpu;

void wdc65816_cpu_init(WDC65816Cpu* cpu, WDC65816Rom* rom, Arena* arena);
void wdc65816_cpu_init2(WDC65816Cpu* cpu, Buffer manifest, Arena* arena);
void wdc65816_cpu_load(WDC65816Cpu* cpu, const char* path);
void wdc65816_cpu_clear(WDC65816Cpu* cpu);
void wdc65816_cpu_free(WDC65816Cpu* cpu);

void wdc65816_cpu_write(WDC65816Cpu* cpu, u32 addr, u8 data);
u8 wdc65816_cpu_read(WDC65816Cpu* cpu, u32 addr);

void wdc65816_cpu_step(WDC65816Cpu* cpu);
void wdc65816_cpu_run(WDC65816Cpu* cpu);
void wdc65816_cpu_run_from(WDC65816Cpu* cpu, u32 address);
void wdc65816_cpu_run_jsr(WDC65816Cpu* cpu, u32 address);
void wdc65816_cpu_run_jsl(WDC65816Cpu* cpu, u32 address);

void wdc65816_cpu_add_exec_bp(WDC65816Cpu* cpu, u32 address);
void wdc65816_cpu_add_read_bp(WDC65816Cpu* cpu, u32 address);
void wdc65816_cpu_add_write_bp(WDC65816Cpu* cpu, u32 address);

void wdc65816_cpu_add_exec_bp_range(WDC65816Cpu* cpu, u32 address_low, u32 address_high);
void wdc65816_cpu_add_read_bp_range(WDC65816Cpu* cpu, u32 address_low, u32 address_high);
void wdc65816_cpu_add_write_bp_range(WDC65816Cpu* cpu, u32 address_low, u32 address_high);

void wdc65816_cpu_clear_exec_bp(WDC65816Cpu* cpu);
void wdc65816_cpu_clear_read_bp(WDC65816Cpu* cpu);
void wdc65816_cpu_clear_write_bp(WDC65816Cpu* cpu);

void wdc65816_cpu_show_state(WDC65816Cpu* cpu, char ouput[256]);
void wdc65816_cpu_disassemble_opcode(WDC65816Cpu* cpu, char* output, u32 addr);


#endif //R65816_CPU_H
