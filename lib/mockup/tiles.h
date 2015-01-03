#ifndef MOCKUP_TILES_HPP
#define MOCKUP_TILES_HPP

#include <stdint.h>

typedef union {
    struct {
	uint8_t Y   : 1;
	uint8_t X   : 1;
	uint8_t P   : 1;
	uint8_t CCC : 3;
	uint8_t TT  : 2;
    };
    uint8_t YXPCCCTT;
} tile_properties_t;

const tile_properties_t tile_flip_None = { .X = 0, .Y = 0};
const tile_properties_t tile_flip_X    = { .X = 1, .Y = 0};
const tile_properties_t tile_flip_Y    = { .X = 0, .Y = 1};
const tile_properties_t tile_flip_XY   = { .X = 1, .Y = 1};

typedef struct {
    uint8_t pixels[64];
} tile8_t;

typedef struct {
    tile8_t* tiles;
    int length;
} map8_t;

typedef struct {
    tile8_t* tiles[4];
    tile_properties_t properties[4];
} tile16_t;

typedef struct {
    tile16_t* tiles;
    int length;
} map16_t;

typedef struct {
    uint32_t (*pixels)[8];
} map16_pc_t;

tile8_t tile8_from_3bpp(uint8_t* data);
tile8_t tile8_from_4bpp(uint8_t* data);

tile16_t tile16_from_tile8(tile8_t* tiles[4], tile_properties_t poperties[4]);

void map8_init(map8_t* map8, tileset_t* tileset);
void map8_deinit(map8_t* map8);

void map16_init(map16_t* map16, r65816_t* rom, int num_level);
void map16_to_pc(map16_t* map16, palette_t palette, map16_pc_t* map16pc);

#endif //MOCKUP_TILES_HPP
