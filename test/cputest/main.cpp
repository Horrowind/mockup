#include <cstdlib>
#include <ctime>

extern "C" {
#include "r65816/cpu.h"
}

#include "cpu.hpp"


int main() {
    for(int i = 0; i < 512; i++) {
        CPU cpu_old("smw.sfc");
        srand(time(0));

    
        int num_level = i;
        int addr = 0x583AC;
        r65816_cpu_t cpu_new;
        r65816_rom_t rom;
        r65816_rom_load(&rom, "smw.sfc");
        r65816_cpu_init(&cpu_new, &rom);
        cpu_new.ram[0x65] = cpu_new.rom->banks[5][0xE000 + 3 * num_level]; 
        cpu_new.ram[0x66] = cpu_new.rom->banks[5][0xE001 + 3 * num_level];
        cpu_new.ram[0x67] = cpu_new.rom->banks[5][0xE002 + 3 * num_level];
        cpu_new.regs.pc.d = addr;

        cpu_old.clear_ram();
        cpu_old.m_ram[0x65] = cpu_new.rom->banks[5][0xE000 + 3 * num_level];
        cpu_old.m_ram[0x66] = cpu_new.rom->banks[5][0xE001 + 3 * num_level];
        cpu_old.m_ram[0x67] = cpu_new.rom->banks[5][0xE002 + 3 * num_level];
        // for(int i = 0; i < 256; i++) {
        //     //printf("%x\n", cpu_new.opcode_table[i]);
        //     printf("%2x\n", i);
        // }
        cpu_old.set_cur_pos(addr);
    
        char output[256], output_old[256];
        while(cpu_new.regs.pc.d != 0x0583BB) {
            r65816_cpu_show_state(&cpu_new, output);
            cpu_old.disassemble_opcode(output_old, cpu_old.regs.pc);
            cpu_old.step();
            r65816_cpu_step(&cpu_new);
            if(strcmp(output, output_old)) {
                printf("%x %x %x\n", r65816_cpu_read(&cpu_new, cpu_new.regs.pc.d), r65816_cpu_read(&cpu_new, cpu_new.regs.pc.d+1), r65816_cpu_read(&cpu_new, cpu_new.regs.pc.d+2));
                printf("%s\n", output);
                printf("%s\n", output_old);
                getchar();
            } else {
                // printf("%s\n", output);
            }
        }
    }
    return 0;
}
