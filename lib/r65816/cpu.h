#ifndef R65816_CPU_H
#define R65816_CPU_H

#define DEBUG_PRINT_CPU_STATE



#include <stdio.h>
#include <string.h>

#include "base/base.h"

#include "breakpoint.h"
#include "registers.h"
#include "rom.h"

typedef struct r65816_cpu {
    r65816_rom_t* rom;

    r65816_regs_t regs;
    r65816_reg24_t aa, rd;
    u8 sp, dp;
    u8 breakpoint_data;
    u8 stop_execution;
    u32 breakpoint_address;
#ifdef DEBUG_PRINT_CPU_STATE
    u32 debug;
#endif
    void (**opcode_table)(struct r65816_cpu*);
    
    r65816_breakpoint_list_t breakpoints_exec;
    r65816_breakpoint_list_t breakpoints_read;
    r65816_breakpoint_list_t breakpoints_write;

    r65816_read_function_t  read;
    r65816_write_function_t write;
    r65816_ptr_function_t   ptr;

    r65816_mapper_t read_mapper;
    r65816_mapper_t write_mapper;
    u8 ram[0x20000];
    u8 sreg[0x2500];
} r65816_cpu_t;

void r65816_cpu_init(r65816_cpu_t* cpu, r65816_rom_t* rom);
void r65816_cpu_init2(r65816_cpu_t* cpu, buffer_t manifest);
void r65816_cpu_load(r65816_cpu_t* cpu, const char* path);
void r65816_cpu_clear(r65816_cpu_t* cpu);
void r65816_cpu_free(r65816_cpu_t* cpu);

void r65816_cpu_write(r65816_cpu_t* cpu, u32 addr, u8 data);
u8 r65816_cpu_read(r65816_cpu_t* cpu, u32 addr);

void r65816_cpu_step(r65816_cpu_t* cpu);
void r65816_cpu_run(r65816_cpu_t* cpu);
void r65816_cpu_run_from(r65816_cpu_t* cpu, u32 address);
void r65816_cpu_run_jsr(r65816_cpu_t* cpu, u32 address);
void r65816_cpu_run_jsl(r65816_cpu_t* cpu, u32 address);

void r65816_cpu_add_exec_bp(r65816_cpu_t* cpu, u32 address);
void r65816_cpu_add_read_bp(r65816_cpu_t* cpu, u32 address);
void r65816_cpu_add_write_bp(r65816_cpu_t* cpu, u32 address);

void r65816_cpu_add_exec_bp_range(r65816_cpu_t* cpu, u32 address_low, u32 address_high);
void r65816_cpu_add_read_bp_range(r65816_cpu_t* cpu, u32 address_low, u32 address_high);
void r65816_cpu_add_write_bp_range(r65816_cpu_t* cpu, u32 address_low, u32 address_high);

void r65816_cpu_show_state(r65816_cpu_t* cpu, char ouput[256]);
void r65816_cpu_disassemble_opcode(r65816_cpu_t* cpu, char* output, u32 addr);


#endif //R65816_CPU_H
