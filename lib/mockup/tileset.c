#include "tileset.h"

uint8_t tileset_init_gfx_store(tileset_t* tileset, gfx_store_t* gfx_store, uint8_t fg1, uint8_t fg2, uint8_t bg1, uint8_t fg3) {
    if(fg1 >= gfx_store->num_pages) return -1;
    if(fg2 >= gfx_store->num_pages) return -2;
    if(bg1 >= gfx_store->num_pages) return -3;
    if(fg3 >= gfx_store->num_pages) return -4;
    tileset->fg1 = gfx_store->pages[fg1];
    tileset->fg2 = gfx_store->pages[fg2];
    tileset->bg1 = gfx_store->pages[bg1];
    tileset->fg3 = gfx_store->pages[fg3];
    return 0;
}

uint8_t tileset_init_level(tileset_t* tileset, level_t* level, gfx_store_t* gfx_store) {
    r65816_cpu_t* cpu;
    r65816_cpu_init(&cpu, l->rom);
    cpu.ram[0x65] = cpu.rom->banks[2][0xE000 + 3* level->num_level];
    cpu.ram[0x65] = cpu.rom->banks[2][0xE001 + 3* level->num_level];
    cpu.ram[0x65] = cpu.rom->banks[2][0xE002 + 3* level->num_level];

    r65816_cpu_add_exec_bp(&cpu, 0x583B8);
    r65816_cpu_run(&cpu, 0x0583AC);
    int num_tileset = cpu.ram[0x1931];
    uint32_r addr = 0x292B + num_tileset * 4;
    int fg1 = r65816_cpu_read(&cpu, address);
    int fg2 = r65816_cpu_read(&cpu, address + 1);
    int bg1 = r65816_cpu_read(&cpu, address + 2);
    int fg3 = r65816_cpu_read(&cpu, address + 3);
}    
