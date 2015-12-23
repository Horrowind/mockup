#ifndef MOCKUP_LEVEL_H
#define MOCKUP_LEVEL_H

#include "r65816/cpu.h"
#include "gfx_store.h"
#include "layer.h"
#include "overworld.h"
#include "palette.h"
#include "tiles.h"
#include "tileset.h"

#define SPRITES

typedef struct {
    uint16_t x;
    uint16_t y;
    uint8_t num : 6;
    uint8_t settings;

    uint16_t  bb_xmin;
    uint16_t  bb_ymin;
    uint16_t  bb_xmax;
    uint16_t  bb_ymax;
    uint16_t  zindex;
    uint16_t* tiles;
} object_pc_t;

typedef struct {
    object_pc_t* objects;
    uint16_t length;
} object_list_pc_t;


#ifdef SPRITES
typedef struct {
    uint8_t y1         : 4;
    uint8_t extra_bits : 2;
    uint8_t screen1    : 1;
    uint8_t y2         : 1;
    uint8_t x          : 4;
    uint8_t screen2    : 4;
    uint8_t enemy;
} sprite_t;

typedef struct {
    int x;
    int y;
    uint8_t flip_x : 1;
    uint8_t flip_y : 1;
    
    uint8_t palette;
    uint16_t tile_num;
} sprite_tile_t;

typedef struct {
    int x, y;
    uint8_t enemy;
    uint8_t extra_bits;
    int num_tiles;
    sprite_tile_t* tiles;   
} sprite_pc_t;

typedef struct {
    int length;
    sprite_pc_t* data;   
} sprite_list_pc_t;
#endif

typedef struct {
    uint8_t* layer1_objects_data;
    int layer1_objects_length;
} level_save_locations_t;


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
void level_init_by_address(level_pc_t* l, r65816_rom_t* rom, int num_level, gfx_store_t* gfx_store,
                           uint32_t level_layer1_data_addr_sfc,
                           uint32_t level_layer2_data_addr_sfc,
                           uint32_t level_sprite_data_addr_sfc);

void level_deinit(level_pc_t* l);

int level_object_list_pc_to_level_data(object_list_pc_t* object_list, uint8_t* output_data, int max_length);
void level_move_object(level_pc_t* l, int index, uint16_t x, uint16_t y, gfx_store_t* gfx_store);

void level_animate(level_pc_t* l, uint8_t frame, gfx_store_t* gfx_store);


#endif //MOCKUP_LEVEL
