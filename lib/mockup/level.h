#ifndef MOCKUP_LEVEL_H
#define MOCKUP_LEVEL_H

#include "r65816/cpu.h"
#include "tiles.h"

typedef struct object {
    uint32_t x;
    uint32_t y;
    uint8_t number;
    uint8_t settings;
    uint8_t extended : 1;
    int     z_index;
    struct object* next;
} object_t;

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
    level_header_t* header;
    object_t* layer_1_objects;

    union {
	object_t* layer_2_objects;
	uint16_t* layer_2_background;
    };
    uint8_t  has_layer_2_bg : 1;
    uint8_t  has_layer_2_object : 1;
    uint8_t  is_vertical_level : 1;
    
    uint32_t      palette[256];
    tile8_t       map8[512];
    tile8_t       gfx32_33[SIZE_OF_GFX_32 + SIZE_OF_GFX_33];
    tile16_t      map16_fg[512];
    tile16_t      map16_bg[512];
    r65816_rom_t* rom;
} level_t;

level_t*  level_init();
void      level_load(level_t* l, int levelnum);
void      level_free(level_t* l);

uint16_t  level_get_tile_16(level_t* l, int x, int y);
void      level_animate(level_t* l, unsigned char frame);

#endif //MOCKUP_LEVEL
