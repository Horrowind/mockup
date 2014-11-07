#include <stdlib.h>

#include "cpu.h"
#include "memory.h"

void cpu_step(cpu_t* cpu) {
    
}

void cpu_clear_ram(cpu_t* cpu) {

}

void cpu_init(cpu_t* cpu, rom_t* rom) {
    cpu->rom = rom;
    cpu->ram = malloc(0x20000);
    cpu->sreg = malloc(0x2500);
    cpu_clear_ram(cpu);
}



void cpu_show_state(cpu_t* cpu, char ouput[256]) {

}

