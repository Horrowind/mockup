#ifndef MOCKUP_PALETTE_H
#define MOCKUP_PALETTE_H

#include "r65816/rom.h"
#include "r65816/cpu.h"

typedef struct {
    uint16_t data[256];
} palette_t;

typedef struct {
    uint32_t data[256];
} palette_pc_t;

void palette_init(palette_t* palette, r65816_rom_t* rom, int num_level);
uint32_t palette_to_pc(palette_t* palette, uint8_t index);

#endif //MOCKUP_PALETTE_H
