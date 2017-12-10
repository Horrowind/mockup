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
} TileProperties;

typedef struct {
    u8 pixels[64];
} Tile8;

typedef struct {
    int length;
    Tile8* tiles;
} Map8;

typedef struct {
    Tile8* tile8s[4];
    TileProperties properties[4];
} Tile16;

typedef struct {
    u32 data[256];
} Tile16PC;

typedef struct {
    int length;
    Tile16* tiles;
} Map16;

typedef struct {
    int length;
    Tile16PC* tiles;
} Map16PC;

Tile8 tile8_from_3bpp(u8* data);
Tile8 tile8_from_4bpp(u8* data);

Tile16 tile16_from_tile8(Tile8* tiles[4], TileProperties poperties[4]);

void map16_pc_init(Map16PC* map16_pc, Map16* map16);
void map16_pc_deinit(Map16PC* map16);
#endif //MOCKUP_TILES_HPP
