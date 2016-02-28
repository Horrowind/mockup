#ifndef MOCKUP_PALETTE_H
#define MOCKUP_PALETTE_H

#include "r65816.h"

typedef struct {
    u16 data[256];
} palette_t;

typedef struct {
    u32 data[256];
} palette_pc_t;

void palette_init(palette_t* palette, r65816_rom_t* rom, int num_level);
void palette_to_pc(palette_t* palette, palette_pc_t* palette_pc);

#endif //MOCKUP_PALETTE_H
