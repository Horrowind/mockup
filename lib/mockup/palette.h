#ifndef MOCKUP_PALETTE_H
#define MOCKUP_PALETTE_H

typedef struct {
    uint16_t[256] data;

} palette_t;

int convertColor(int snes) {
}


void palette_load(palette_t* palette) {
    r65816_cpu_t cpu;
    r65816_cpu_init(&cpu, l->rom);

    cpu.ram[0x65] = cpu.rom->banks[2][0xE000 + 3 * l->levelnum]; 
    cpu.ram[0x66] = cpu.rom->banks[2][0xE000 + 3 * l->levelnum + 1];
    cpu.ram[0x67] = cpu.rom->banks[2][0xE000 + 3 * l->levelnum + 2];

    r65816_cpu_add_exec_bp(&cpu, 0x0583AC);
    r65816_cpu_run(&cpu, 0x0583B8);
    memcpy(palette->data, cpu.ram + 0x0703, 512);
    
    for(int i = 0; i < 16; i++) {
        palette->data[i << 4] = 0;
    }
    
    r65816_cpu_free(&cpu);
}

uint32_t palette_to_pc(palette_t* palette, uint8_t index) {
    uint16_t snes = palette->data[index];
    int r = ((snes & 0x1F) << 3);   r |= (r >> 5);
    int g = ((snes & 0x3E0) >> 2);  g |= (g >> 5);
    int b = ((snes & 0x7C00) >> 7); b |= (b >> 5);
    return 0xFF000000 + (r << 16) + (g << 8) + b;
}


#endif //MOCKUP_PALETTE_H
