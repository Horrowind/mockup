#include "smw.h"
#include "tiles.h"

/* tile8_t tile8_flip(tile8_t tile, tile_properties_t props) { */
/*     tile8_t tile_new; */
/*     int itx = flip.X ? -1 : 1; */
/*     int ity = flip.Y ? -1 : 1; */
        
/*     int sy  = flip.Y ?  7 : 0; */
/*     for(int i = 0; i < 8; i++) { */
/*         int sx  = flip.X ?  7 : 0; */
/*         for(int j = 0; j < 8; j++) { */
/*             tile_new.pixels[i * 8 + j] = tile.pixels[sy * 8 + sx]; */
/*             sx += itx; */
                
/*         } */
/*         sy += ity; */
/*     } */
/*     return tile_new; */
/* } */

tile8_t tile8_from_3bpp(u8* data) {
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

tile8_t tile8_from_4bpp(u8* data) {
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

/*     tile8_t tile_new; */
/*     int itx = flip.X ? -1 : 1; */
/*     int ity = flip.Y ? -1 : 1; */
        
/*     int sy  = flip.Y ?  7 : 0; */
/*     for(int i = 0; i < 8; i++) { */
/*         int sx  = flip.X ?  7 : 0; */
/*         for(int j = 0; j < 8; j++) { */
/*             tile_new.pixels[i * 8 + j] = tile.pixels[sy * 8 + sx]; */
/*             sx += itx; */
                
/*         } */
/*         sy += ity; */
/*     } */
/*     return tile_new; */

tile16_t tile16_from_tile8(tile8_t* t[4], tile_properties_t properties[4]) {
    tile16_t tile;
    for(int i = 0; i < 4; i++) {
        tile.tile8s[i] = t[i];
        tile.properties[i] = properties[i];
    }
    return tile;
}

void tile16_pc_init(tile16_pc_t* tile16_pc, tile16_t* tile16) {
    for(int k = 0; k < 2; k++) {
        for(int l = 0; l < 2; l++) {
            int pos = k*2 +l;
            int itx = tile16->properties[pos].X ? -1 : 1;
            int ity = tile16->properties[pos].Y ? -1 : 1; 
            int sy  = tile16->properties[pos].Y ?  7 : 0;
            for(int i = 0; i < 8; i++) {
                int sx  = tile16->properties[pos].X ?  7 : 0;
                for(int j = 0; j < 8; j++) {
                    tile16_pc->data[128 * l + (k + 2*i) * 8 + j] =
                        16 * tile16->properties[pos].CCC + tile16->tile8s[pos]->pixels[sy * 8 + sx];
                    sx += itx;
                }
                sy += ity;
            }
        }
    }
}

void map16_pc_init(map16_pc_t* map16_pc, map16_t* map16_snes) {
    map16_pc->length = map16_snes->length;
    map16_pc->tiles = malloc(map16_pc->length * 256 * sizeof(u32));
    for(int i = 0; i < map16_pc->length; i++) {
        tile16_pc_init(&map16_pc->tiles[i], &map16_snes->tiles[i]);
    }
}

void map16_pc_deinit(map16_pc_t* map16_pc) {
    free(map16_pc->tiles);
}
