#ifndef R65816_CPU_H
#define R65816_CPU_H

#include <stdio.h>
#include <string.h>

#include "breakpoint.h"
#include "registers.h"
#include "rom.h"


typedef struct cpu {
    r65816_rom_t* rom;
    uint8_t* ram;
    uint8_t* sreg;


    r65816_regs_t regs;
    r65816_reg24_t aa, rd;
    uint8_t sp, dp;

    void (**opcode_table)(struct cpu*);
    void (*op_table[5 * 256])(struct cpu*);

    r65816_breakpoint_t* first_breakpoint_exec;
    r65816_breakpoint_t* first_breakpoint_read;
    r65816_breakpoint_t* first_breakpoint_write;
    uint8_t stop_execution;
} r65816_cpu_t;

void r65816_cpu_step(r65816_cpu_t* cpu);
void r65816_cpu_run(r65816_cpu_t* cpu, uint32_t address);
void r65816_cpu_add_exec_bp(r65816_cpu_t* cpu, uint32_t address);
void r65816_cpu_add_read_bp(r65816_cpu_t* cpu, uint32_t address);
void r65816_cpu_add_write_bp(r65816_cpu_t* cpu, uint32_t address);
void r65816_cpu_init(r65816_cpu_t* cpu, r65816_rom_t* rom);
void r65816_cpu_show_state(r65816_cpu_t* cpu, char ouput[256]);
void r65816_cpu_disassemble_opcode(r65816_cpu_t* cpu, char* output, uint32_t addr);
void r65816_cpu_write(r65816_cpu_t* cpu, uint32_t addr, uint8_t data);
uint8_t r65816_cpu_read(r65816_cpu_t* cpu, uint32_t addr);

#endif //R65816_CPU_H
