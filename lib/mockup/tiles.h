#ifndef MOCKUP_TILES_HPP
#define MOCKUP_TILES_HPP

#include <stdint.h>
#include "r65816/rom.h"
#include "r65816/cpu.h"
#include "palette.h"
#include "tileset.h"

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

typedef tile_properties_t tile_flip_t;

const tile_flip_t tile_flip_None = { .X = 0, .Y = 0};
const tile_flip_t tile_flip_X    = { .X = 1, .Y = 0};
const tile_flip_t tile_flip_Y    = { .X = 0, .Y = 1};
const tile_flip_t tile_flip_XY   = { .X = 1, .Y = 1};

typedef struct {
    uint8_t pixels[64];
} tile8_t;

typedef struct {
    int length;
    tile8_t* tiles;
} map8_t;

typedef struct {
    tile8_t* tiles[4];
    tile_properties_t properties[4];
} tile16_t;

typedef uint32_t tile16_pc_t[64];

typedef struct {
    int length;
    tile16_t* tiles;
} map16_t;

typedef struct {
    int length;
    tile16_pc_t* tiles;
} map16_pc_t;

tile8_t tile8_from_3bpp(uint8_t* data);
tile8_t tile8_from_4bpp(uint8_t* data);

tile16_t tile16_from_tile8(tile8_t* tiles[4], tile_properties_t poperties[4]);

void map8_init(map8_t* map8, tileset_t* tileset);
void map8_deinit(map8_t* map8);

void map16_init_bg(map16_t* map16, r65816_rom_t* rom, int num_level, map8_t* map8);
void map16_init_fg(map16_t* map16, r65816_rom_t* rom, int num_level, map8_t* map8);

void map16_to_pc(map16_t* map16, map16_pc_t* map16_pc);

#endif //MOCKUP_TILES_HPP
