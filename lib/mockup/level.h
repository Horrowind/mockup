#ifndef MOCKUP_LEVEL_H
#define MOCKUP_LEVEL_H

#include "r65816/cpu.h"
#include "tiles.h"

typedef struct {
    const char* path;
    int levelnum;
    
    uint16_t* layer1;
    uint16_t* layer2;
    
    int height;
    int width;    
    
    int      screens;
    uint8_t  has_layer_2_bg : 1;
    uint8_t  has_layer_2_object : 1;
    uint8_t  is_vertical_level : 1;
    uint32_t background_color;
    
    uint32_t      palette[256];
    tile8_t       map_8[512];
    tile8_t       gfx_32_33[SIZE_OF_GFX_32 + SIZE_OF_GFX_33];
    tile16_t      map_16_fg[512];
    tile16_t      map_16_bg[512];
    r65816_rom_t* rom;
} level_t;

level_t*  level_init();
void      level_load(level_t* l, int levelnum);
void      level_free(level_t* l);

void      level_load_palette();
void      level_load_map16();
void      level_load_map8();
void      level_load_objects();
void      level_load_gfx3233();

void      level_render_line_fg(level_t* l, uint8_t* line, int linenum);
void      level_render_line_bg(level_t* l, uint8_t* line, int linenum);
uint8_t   level_render_pixel(level_t* l, int x, int y);
uint16_t  level_get_tile_16(level_t* l, int x, int y);
void      level_animate(level_t* l, unsigned char frame);

#endif //MOCKUP_LEVEL
