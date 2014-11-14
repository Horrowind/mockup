#ifndef MOCKUP_LEVEL_H
#define MOCKUP_LEVEL_H

#include "r65816/cpu.h"
#include "tiles.h"

struct level {
    const char* path;
    int levelnum;
    
    
    uint16_t* layer1 = NULL;
    uint16_t* layer2 = NULL;
    
    int height;
    int width;    
    
    int      screens;
    uint8_t  has_layer_2_bg : 1;
    uint8_t  has_layer_2_object : 1;
    uint8_t  is_vertical_level : 1;
    uint32_t background_color;
	

    
    const int size_of_gfx_32 = 744;
    const int size_of_gfx_33 = 384;
    uint32_t  palette[256];
    tile8_t   map_8[512];
    tile8_t   gfx_32_33[sizeOfGFX32 + sizeOfGFX33];
    tile16_t  map_16_fg[512];
    tile16_t  map_16_bg[512];
    cpu_t     cpu;
};
typedef struct level level_t;

level_t*  level_init();
level_t*  level_load(const char* path, int levelnum);
void      level_free(level_t* l);

void      level_load_map16();
void      level_load_map8();
void      level_load_palette();
void      level_load_objects();
void      level_load_gfx3233();

void      level_render_line_fg(uint8_t* line, int linenum);
void      level_render_line_bg(uint8_t* line, int linenum);
uint8_t   level_render_pixel(int x, int y);
uint16_t  level_get_tile_16(int x, int y);
void      level_animate(unsigned char frame);

#endif //MOCKUP_LEVEL
