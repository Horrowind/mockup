
#ifndef MOCKUP_TILESET_H
#define MOCKUP_TILESET_H

#include "r65816/rom.h"
#include "gfx_store.h"

typedef struct {
    gfx_page_t* fg1;
    gfx_page_t* bg1;
    gfx_page_t* fg2;
    gfx_page_t* fg3;
} tileset_t;

void tileset_init(tileset_t* tileset, gfx_store_t* gfx_store, uint8_t fg1, uint8_t fg2, uint8_t bg1, uint8_t fg3);
void tileset_init_level(tileset_t* tileset, r65816_rom_t* rom, int num_level, gfx_store_t* gfx_store);
#endif //MOCKUP_TILESET_H
