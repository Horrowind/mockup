
#ifndef MOCKUP_TILESET_H
#define MOCKUP_TILESET_H

typedef struct {
    gfx_page_t* fg1;
    gfx_page_t* bg1;
    gfx_page_t* fg2;
    gfx_page_t* fg3;
} tileset_t;

void tileset_load(tileset_t* tileset, rom_t* rom, int num_level);
#endif //MOCKUP_TILESET_H
