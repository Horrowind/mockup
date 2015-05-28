#include "palette.h"

void palette_init(palette_t* palette, r65816_rom_t* rom, int num_level) {
    r65816_cpu_t cpu = {.regs.p.b = 0x24};
    r65816_cpu_init(&cpu, rom);

    cpu.ram[0x65] = cpu.rom->banks[5][0xE000 + 3 * num_level]; 
    cpu.ram[0x66] = cpu.rom->banks[5][0xE001 + 3 * num_level];
    cpu.ram[0x67] = cpu.rom->banks[5][0xE002 + 3 * num_level];

    r65816_cpu_add_exec_bp(&cpu, 0x0583B8);
    r65816_cpu_run_from(&cpu, 0x0583AC);
    memcpy(palette->data, cpu.ram + 0x0703, 512);
    
    for(int i = 0; i < 16; i++) {
        palette->data[i << 4] = 0;
    }
    r65816_cpu_free(&cpu);
}

uint32_t palette_to_pc(palette_t* palette, palette_pc_t* palette_pc) {
    for(int i = 0; i < 256; i++) {
        uint16_t snes = palette->data[i];
        int r = ((snes & 0x001F) << 3); r |= (r >> 5);
        int g = ((snes & 0x03E0) >> 2); g |= (g >> 5);
        int b = ((snes & 0x7C00) >> 7); b |= (b >> 5);
        if(i & 0x0F) {
            palette_pc->data[i] = 0xFF000000 + (r << 16) + (g << 8) + b;
        } else {
            palette_pc->data[i] = 0;
        }
    }
}
