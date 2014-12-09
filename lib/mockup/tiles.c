#include "tiles.h"

tile8_t tile8_flip(tile8_t tile, tile_flip_t flip) {
    tile8_t tile_new;
    int itx = flip.X ? -1 : 1;
    int ity = flip.Y ? -1 : 1;
        
    int sy  = flip.Y ?  7 : 0;
    for(int i = 0; i < 8; i++) {
        int sx  = flip.X ?  7 : 0;
        for(int j = 0; j < 8; j++) {
            tile_new.pixels[i * 8 + j] = tile.pixels[sy * 8 + sx];
            sx += itx;
                
        }
        sy += ity;
    }
    return tile_new;
}

tile8_t tile8_from_3bpp(uint8_t* data) {
    tile8_t tile;
    for(int k = 0; k < 8; k++) {
        unsigned char bp1, bp2, bp3;
        bp1 = data[2 * k];
        bp2 = data[2 * k + 1];
        bp3 = data[16 + k];

        for(int l = 0; l < 8; l++) {
            int index = ((bp1 & (1 << (7 - l))) ? 1 : 0)
                + ((bp2 & (1 << (7 - l))) ? 2 : 0)
                + ((bp3 & (1 << (7 - l))) ? 4 : 0);
                    
            tile.pixels[k * 8 + l] = index;
        }
    }
    return tile;
}

tile8_t tile8_from_4bpp(uint8_t* data) {
    tile8_t tile;
    for(int k = 0; k < 8; k++) {
        unsigned char bp1, bp2, bp3, bp4;
        bp1 = data[2 * k];
        bp2 = data[2 * k + 1];
        bp3 = data[16 + 2 * k];
        bp4 = data[16 + 2 * k + 1];

        for(int l = 0; l < 8; l++) {
            int index = ((bp1 & (1 << (7 - l))) ? 1 : 0)
                + ((bp2 & (1 << (7 - l))) ? 2 : 0)
                + ((bp3 & (1 << (7 - l))) ? 4 : 0)
                + ((bp4 & (1 << (7 - l))) ? 8 : 0);

            tile.pixels[k * 8 + l] = index;
        }
    }
    return tile;
}

tile16_t tile16_from_tile8(tile8_t t[4], const uint8_t* pal) {
    tile16_t tile;
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 16; j++) {
            if(j < 8) {
                tile.pixels[i * 16 + j] = 16 * pal[0] + t[0].pixels[i * 8 + j];
            } else {
                tile.pixels[i * 16 + j] = 16 * pal[2] + t[2].pixels[i * 8 + j - 8];
            }
        }
    }
    for(int i = 8; i < 16; i++) {
        for(int j = 0; j < 16; j++) {
            if(j < 8) {
                tile.pixels[i * 16 + j] = 16 * pal[1] + t[1].pixels[i * 8 + j - 64];
            } else {
                tile.pixels[i * 16 + j] = 16 * pal[3] + t[3].pixels[i * 8 + j - 72];
            }                
        }
    }
    return tile;
}

