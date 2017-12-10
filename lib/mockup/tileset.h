
#ifndef MOCKUP_TILESET_H
#define MOCKUP_TILESET_H

#include "wdc65816/wdc65816.h"
#include "gfx_store.h"

typedef struct {
    GFXPage* fg1;
    GFXPage* bg1;
    GFXPage* fg2;
    GFXPage* fg3;
} Tileset;

void tileset_init(Tileset* tileset, GFXStore* gfx_store, u8 fg1, u8 fg2, u8 bg1, u8 fg3);
void tileset_init_level(Tileset* tileset, WDC65816Rom* rom, int num_level, GFXStore* gfx_store);
#endif //MOCKUP_TILESET_H
