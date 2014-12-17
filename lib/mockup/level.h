#ifndef MOCKUP_LEVEL_H
#define MOCKUP_LEVEL_H

#include "r65816/cpu.h"

<<<<<<< HEAD
=======
#include "gfx_store.h"
#include "objects.h"
#include "overworld.h"
#include "palette.h"
#include "tiles.h"
>>>>>>> ed9da0e54072b184c46264beb797d49a1c91ddf2

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
<<<<<<< HEAD
    level_header_t* header;
    object_list_t*  layer_1_objects;
    sprite_list_t*  sprites;

    union {
	object_t*  layer_2_objects;
        layer16_t* layer_2_background;
    };
    uint8_t  has_layer_2_bg     : 1;
    uint8_t  has_layer_2_object : 1;
    uint8_t  is_vertical_level  : 1;
    
    palette_t palette;
    map8_t    map8;
    map8_t    gfx32_33;
    map16_t   map16_fg;
    map16_t   map16_bg;
    
    r65816_rom_t* rom;
} level_t;

level_t*  level_init();
void      level_load(level_t* l, int levelnum);
void      level_deinit(level_t* l);

uint16_t  level_get_tile_16(level_t* l, int x, int y);
void      level_animate_map8(level_t* l, uint8_t frame, map8_t* map8);
=======
    level_header_t header;
    object_list_t*  layer1_objects;

    union {
        object_list_t* layer2_objects;
        layer16_t*  layer2_background;
    };

    sprite_list_t*  sprites;
    
    uint8_t  has_layer2_bg : 1;
    uint8_t  has_layer2_object : 1;
    uint8_t  is_vertical_level : 1;
    
    palette_t* palette;
    map8_t*    map8;
    tileset_t* tileset;
    map16_t*   map16_fg;
    map16_t*   map16_bg;

    r65816_rom_t* rom;
} level_t;

void level_init(level_t* l);
void level_load(level_t* l, int num_level);
void level_free(level_t* l);

void level_draw_layer1(level_t* l, uint8_t num_frame, layer16 layer1);
void level_draw_layer2(level_t* l, uint8_t num_frame, layer16 layer1);
>>>>>>> ed9da0e54072b184c46264beb797d49a1c91ddf2

#endif //MOCKUP_LEVEL
