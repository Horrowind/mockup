#ifndef R65816_CPU_H
#define R65816_CPU_H

#define DEBUG_PRINT_CPU_STATE



#include <stdio.h>
#include <string.h>

#include "base/base.h"

#include "breakpoint.h"
#include "registers.h"
#include "rom.h"

typedef struct wdc65816_cpu {
    wdc65816_rom_t* rom;

    wdc65816_regs_t regs;
    wdc65816_reg24_t aa, rd;
    u8 sp, dp;
    u8 breakpoint_data;
    u8 stop_execution;
    u32 breakpoint_address;
#ifdef DEBUG_PRINT_CPU_STATE
    u32 debug;
#endif
    void (**opcode_table)(struct wdc65816_cpu*);
    
    wdc65816_breakpoint_list_t breakpoints_exec;
    wdc65816_breakpoint_list_t breakpoints_read;
    wdc65816_breakpoint_list_t breakpoints_write;

    wdc65816_read_function_t  read;
    wdc65816_write_function_t write;
    wdc65816_ptr_function_t   ptr;

    wdc65816_mapper_t read_mapper;
    wdc65816_mapper_t write_mapper;
    u8* ram;
    u8* sreg;
} wdc65816_cpu_t;

void wdc65816_cpu_init(wdc65816_cpu_t* cpu, wdc65816_rom_t* rom, arena_t* arena);
void wdc65816_cpu_init2(wdc65816_cpu_t* cpu, buffer_t manifest, arena_t* arena);
void wdc65816_cpu_load(wdc65816_cpu_t* cpu, const char* path);
void wdc65816_cpu_clear(wdc65816_cpu_t* cpu);
void wdc65816_cpu_free(wdc65816_cpu_t* cpu);

void wdc65816_cpu_write(wdc65816_cpu_t* cpu, u32 addr, u8 data);
u8 wdc65816_cpu_read(wdc65816_cpu_t* cpu, u32 addr);

void wdc65816_cpu_step(wdc65816_cpu_t* cpu);
void wdc65816_cpu_run(wdc65816_cpu_t* cpu);
void wdc65816_cpu_run_from(wdc65816_cpu_t* cpu, u32 address);
void wdc65816_cpu_run_jsr(wdc65816_cpu_t* cpu, u32 address);
void wdc65816_cpu_run_jsl(wdc65816_cpu_t* cpu, u32 address);

void wdc65816_cpu_add_exec_bp(wdc65816_cpu_t* cpu, u32 address);
void wdc65816_cpu_add_read_bp(wdc65816_cpu_t* cpu, u32 address);
void wdc65816_cpu_add_write_bp(wdc65816_cpu_t* cpu, u32 address);

void wdc65816_cpu_add_exec_bp_range(wdc65816_cpu_t* cpu, u32 address_low, u32 address_high);
void wdc65816_cpu_add_read_bp_range(wdc65816_cpu_t* cpu, u32 address_low, u32 address_high);
void wdc65816_cpu_add_write_bp_range(wdc65816_cpu_t* cpu, u32 address_low, u32 address_high);

void wdc65816_cpu_clear_exec_bp(wdc65816_cpu_t* cpu);
void wdc65816_cpu_clear_read_bp(wdc65816_cpu_t* cpu);
void wdc65816_cpu_clear_write_bp(wdc65816_cpu_t* cpu);

void wdc65816_cpu_show_state(wdc65816_cpu_t* cpu, char ouput[256]);
void wdc65816_cpu_disassemble_opcode(wdc65816_cpu_t* cpu, char* output, u32 addr);


#endif //R65816_CPU_H
