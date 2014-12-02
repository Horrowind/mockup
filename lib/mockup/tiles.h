#ifndef MOCKUP_TILES_HPP
#define MOCKUP_TILES_HPP

#include <stdint.h>

typedef struct {
    uint8_t X :1;
    uint8_t Y :1;
} tile_flip_t;

const tile_flip_t tile_flip_None = { .X = 0, .Y = 0};
const tile_flip_t tile_flip_X = { .X = 1, .Y = 0};
const tile_flip_t tile_flip_Y = { .X = 0, .Y = 1};
const tile_flip_t tile_flip_XY = { .X = 1, .Y = 1};

typedef struct {
    uint8_t pixels[64];
} tile8_t;
    
typedef struct {
    uint32_t pixels[256];
} tile16_t;


tile8_t tile8_flip(tile8_t tile, tile_flip_t flip);
tile8_t tile8_from3bpp(uint8_t* data);
tile8_t tile8_from4bpp(uint8_t* data);

tile16_t tile16_fromTile8(tile8_t t[4], const uint8_t* palette);
    
#endif //MOCKUP_TILES_HPP
