#include <string.h>
#include <stdlib.h>

extern "C" {
#include "cpu.h"
}

#include "cpu_old.hpp"

void r65816_cpu_init(r65816_cpu_t* cpu, r65816_rom_t* rom) {
    CPU* cpu_old = new CPU(rom, false);
    cpu->rom = rom;
    cpu->ram = cpu_old->m_ram;
    cpu->sreg = (uint8_t*) cpu_old; 
    cpu->breakpoints_exec = NULL;
    cpu->breakpoints_read = NULL;
    cpu->breakpoints_write = NULL;
}

void r65816_cpu_free(r65816_cpu_t* cpu) {
    //delete (CPU*)cpu->sreg;
    free(cpu->ram);
    free(cpu->sreg);
    r65816_breakpoint_clear(&cpu->breakpoints_exec);
    r65816_breakpoint_clear(&cpu->breakpoints_read);
    r65816_breakpoint_clear(&cpu->breakpoints_write);
}

void r65816_cpu_clear_ram(r65816_cpu_t* cpu) {
    ((CPU*)cpu->sreg)->clear_ram();
}

void r65816_cpu_step(r65816_cpu_t* cpu) {
    ((CPU*)cpu->sreg)->step();
}

void r65816_cpu_run(r65816_cpu_t* cpu, uint32_t address) {
    ((CPU*)cpu->sreg)->set_cur_pos(address);
    cpu->stop_execution = 0;
    while(!cpu->stop_execution) {
        ((CPU*)cpu->sreg)->step();
        cpu->stop_execution |= r65816_breakpoint_is_hit(
            cpu->breakpoints_exec,
            ((CPU*)cpu->sreg)->get_cur_pos());
    }
}

void r65816_cpu_add_exec_bp(r65816_cpu_t* cpu, uint32_t addr) {
    r65816_breakpoint_add(&cpu->breakpoints_exec, addr);
}

void r65816_cpu_add_read_bp(r65816_cpu_t* cpu, uint32_t addr) {
    r65816_breakpoint_add(&cpu->breakpoints_read, addr);
}

void r65816_cpu_add_write_bp(r65816_cpu_t* cpu, uint32_t addr) {
    r65816_breakpoint_add(&cpu->breakpoints_write, addr);
}

void r65816_cpu_show_state(r65816_cpu_t* cpu, char* output) {

}

void r65816_cpu_disassemble_opcode(r65816_cpu_t* cpu, char* output, uint32_t addr) {

}
