#ifndef MOCKUP_LEVEL_H
#define MOCKUP_LEVEL_H

#include "r65816/cpu.h"
#include "gfx_store.h"
#include "object.h"
#include "overworld.h"
#include "palette.h"
#include "tiles.h"

typedef struct {
    uint8_t bg_palette      : 3;
    uint8_t length          : 5;
    uint8_t bg_color        : 3;
    uint8_t level_mode      : 5;
    uint8_t layer3_priority : 1;
    uint8_t music           : 3;
    uint8_t sprite_set      : 4;
    uint8_t time            : 2;
    uint8_t sprite_palette  : 3;
    uint8_t fg_palette      : 3;
    uint8_t item_memory     : 2;
    uint8_t vert_scroll     : 2;
    uint8_t tile_set        : 4;
} level_header_t;


typedef struct {
    level_header_t header;
    object_list_t* layer_1_objects;
    //sprite_list_t* sprites;

    union {
        object_t*  layer_2_objects;
        layer16_t* layer_2_background;
    };
    uint8_t has_layer_2_bg     : 1;
    uint8_t has_layer_2_object : 1;
    uint8_t is_vertical_level  : 1;

    palette_t palette;
    map8_t    map8;
    map16_t   map16_fg;
    map16_t   map16_bg;

    r65816_rom_t* rom;
} level_t;

void level_init(level_t* l, r65816_rom_t* rom, gfx_store* gfx_store, int levelnum);
void level_deinit(level_t* l);

void level_init_path(level_t* l, const char* path, int levelnum);
void level_deinit_path(level_t* l);


void level_load_palette(level_t* l, palette_t* palette);
void level_load_palette(level_t* l, object_t* objects);

void level_animate_map8(level_t* l, uint8_t frame, map8_t* map8);
#endif //MOCKUP_LEVEL
