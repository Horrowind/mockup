#ifndef WDC65816_CPU_H
#define WDC65816_CPU_H

#include "base/base.h"

#include "breakpoint.h"
#include "registers.h"
#include "rom.h"

typedef struct Wdc65816Cpu {
    Wdc65816Rom* rom;

    Wdc65816Regs regs;
    Wdc65816Reg24 aa, rd;
    u8 sp, dp;
    u8 breakpoint_data;
    u8 stop_execution;
    u32 breakpoint_address;
#ifdef DEBUG_PRINT_CPU_STATE
    u32 debug;
#endif
    void (**opcode_table)(struct Wdc65816Cpu*);
    
    Wdc65816BreakpointList breakpoints_exec;
    Wdc65816BreakpointList breakpoints_read;
    Wdc65816BreakpointList breakpoints_write;

    Wdc65816Mapper read_mapper;
    Wdc65816Mapper write_mapper;
    u8* ram;
    u8* sreg;
} Wdc65816Cpu;

void wdc65816_cpu_init(Wdc65816Cpu* cpu, Wdc65816Rom* rom, Arena* arena);
void wdc65816_cpu_init2(Wdc65816Cpu* cpu, Buffer manifest, Arena* arena);
void wdc65816_cpu_load(Wdc65816Cpu* cpu, const char* path);
void wdc65816_cpu_clear(Wdc65816Cpu* cpu);
void wdc65816_cpu_free(Wdc65816Cpu* cpu);

void wdc65816_cpu_write(Wdc65816Cpu* cpu, u32 addr, u8 data);
u8 wdc65816_cpu_read(Wdc65816Cpu* cpu, u32 addr);

void wdc65816_cpu_step(Wdc65816Cpu* cpu);
void wdc65816_cpu_run(Wdc65816Cpu* cpu);
void wdc65816_cpu_run_from(Wdc65816Cpu* cpu, u32 address);
void wdc65816_cpu_run_jsr(Wdc65816Cpu* cpu, u32 address);
void wdc65816_cpu_run_jsl(Wdc65816Cpu* cpu, u32 address);

void wdc65816_cpu_add_exec_bp(Wdc65816Cpu* cpu, u32 address);
void wdc65816_cpu_add_read_bp(Wdc65816Cpu* cpu, u32 address);
void wdc65816_cpu_add_write_bp(Wdc65816Cpu* cpu, u32 address);

void wdc65816_cpu_add_exec_bp_range(Wdc65816Cpu* cpu, u32 address_low, u32 address_high);
void wdc65816_cpu_add_read_bp_range(Wdc65816Cpu* cpu, u32 address_low, u32 address_high);
void wdc65816_cpu_add_write_bp_range(Wdc65816Cpu* cpu, u32 address_low, u32 address_high);

void wdc65816_cpu_clear_exec_bp(Wdc65816Cpu* cpu);
void wdc65816_cpu_clear_read_bp(Wdc65816Cpu* cpu);
void wdc65816_cpu_clear_write_bp(Wdc65816Cpu* cpu);

void wdc65816_cpu_show_state(Wdc65816Cpu* cpu, char ouput[256]);
void wdc65816_cpu_disassemble_opcode(Wdc65816Cpu* cpu, char* output, u32 addr);


#endif //WDC65816_CPU_H
