#ifndef MOCKUP_TILES_HPP
#define MOCKUP_TILES_HPP

#include "palette.h"
#include "tileset.h"

struct smw;

typedef union {
    struct {
        u8 TT  : 2;
        u8 CCC : 3;
        u8 P   : 1;
        u8 X   : 1;
        u8 Y   : 1;
    };
    u8 YXPCCCTT;
} tile_properties_t;

typedef tile_properties_t tile_flip_t;

/* const tile_flip_t tile_flip_None = { .X = 0, .Y = 0}; */
/* const tile_flip_t tile_flip_X    = { .X = 1, .Y = 0}; */
/* const tile_flip_t tile_flip_Y    = { .X = 0, .Y = 1}; */
/* const tile_flip_t tile_flip_XY   = { .X = 1, .Y = 1}; */

typedef struct {
    u8 pixels[64];
} tile8_t;

typedef struct {
    int length;
    tile8_t* tiles;
} map8_t;

typedef struct {
    tile8_t* tile8s[4];
    tile_properties_t properties[4];
} tile16_t;

typedef struct {
    u32 data[256];
} tile16_pc_t;

typedef struct {
    int length;
    tile16_t* tiles;
} map16_t;

typedef struct {
    int length;
    tile16_pc_t* tiles;
} map16_pc_t;

tile8_t tile8_from_3bpp(u8* data);
tile8_t tile8_from_4bpp(u8* data);

tile16_t tile16_from_tile8(tile8_t* tiles[4], tile_properties_t poperties[4]);

void map16_pc_init(map16_pc_t* map16_pc, map16_t* map16);
void map16_pc_deinit(map16_pc_t* map16);
#endif //MOCKUP_TILES_HPP
