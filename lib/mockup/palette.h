#ifndef MOCKUP_PALETTE_H
#define MOCKUP_PALETTE_H

#include "wdc65816/wdc65816.h"

typedef struct {
    u16 data[256];
} Palette;

typedef struct {
    u32 data[256];
} PalettePC;

void palette_to_pc(Palette* palette, PalettePC* palette_pc);

#endif //MOCKUP_PALETTE_H
