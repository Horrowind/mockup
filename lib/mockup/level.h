#ifndef MOCKUP_LEVEL_H
#define MOCKUP_LEVEL_H

#include "r65816/cpu.h"
#include "gfx_store.h"
#include "object.h"
#include "overworld.h"
#include "palette.h"
#include "tiles.h"
#include "tileset.h"
#include "sprites.h"

#define SPRITES


typedef struct {
    uint8_t screens         : 5;
    uint8_t bg_palette      : 3;

    uint8_t level_mode      : 5;
    uint8_t bg_color        : 3;
    
    uint8_t sprite_set      : 4;
    uint8_t music           : 3;
    uint8_t layer3_priority : 1;

    uint8_t fg_palette      : 3;
    uint8_t sprite_palette  : 3;
    uint8_t time            : 2;
    
    uint8_t tile_set        : 4;
    uint8_t vert_scroll     : 2;
    uint8_t item_memory     : 2;
} level_header_t;


typedef struct {
    level_header_t header;
    object_list_pc_t layer1_objects;
#ifdef SPRITES
    sprite_list_pc_t sprites;
#endif
    union {
        object_list_pc_t layer2_objects;
        layer16_t        layer2_background;
    };
    uint8_t has_layer2_bg      : 1;
    uint8_t has_layer2_objects : 1;
    uint8_t is_vertical_level  : 1;
    uint8_t is_boss_level      : 1;

    uint32_t  background_color;
    
    palette_t palette;
    tileset_t tileset;
    map8_t    map8;
    map16_t   map16_fg;
    map16_t   map16_bg;
    int       num_level;
    int       height;
    int       width;


    r65816_rom_t* rom;

} level_pc_t;

void level_init(level_pc_t* l, r65816_rom_t* rom, int num_level, gfx_store_t* gfx_store);
void level_deinit(level_pc_t* l);

void level_animate(level_pc_t* l, uint8_t frame, gfx_store_t* gfx_store);

void level_serialize_objects(level_pc_t* l, uint8_t* data);

#endif //MOCKUP_LEVEL
