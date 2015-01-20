#include <cstdlib>
#include <ctime>

extern "C" {
#include "r65816/cpu.h"
}

#include "cpu.hpp"


int main() {

    CPU cpu_old("smw.smc", true);
    srand(time(0));
    r65816_cpu_t cpu_new;
    r65816_rom_t rom;
    r65816_rom_load(&rom, "smw.smc");
    r65816_cpu_init(&cpu_new, &rom);
    int addr = 0x0583AC;
    cpu_old.clear_ram();
    // for(int i = 0; i < 256; i++) {
    //     //printf("%x\n", cpu_new.opcode_table[i]);
    //     printf("%2x\n", i);
    // }
    cpu_old.set_cur_pos(addr);
    
    cpu_new.regs.pc.d = addr;
    char output[256];
    while(1) {
        r65816_cpu_show_state(&cpu_new, output);
        printf("%s\n", output);
        cpu_old.step();
        r65816_cpu_step(&cpu_new);
        getchar();
    }
    return 0;
}
