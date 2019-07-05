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

Tile8 tile8_from_3bpp(u8* data) {
    Tile8 tile;
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

Tile8 tile8_from_4bpp(u8* data) {
    Tile8 tile;
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

/*     Tile8 tile_new; */
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

Tile16 tile16_from_tile8(Tile8* t[4], TileProperties properties[4]) {
    Tile16 tile;
    for(int i = 0; i < 4; i++) {
        tile.tile8s[i] = t[i];
        tile.properties[i] = properties[i];
    }
    return tile;
}

void tile16_pc_init(Tile16PC* tile16_pc, Tile16* tile16) {
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

void map16_pc_init(Map16PC* map16_pc, Map16* map16_snes, FreeList* free_list) {
    map16_pc->length = map16_snes->length;
    map16_pc->tiles = free_list_alloc(free_list, map16_pc->length * sizeof(Tile16PC));
    for(int i = 0; i < map16_pc->length; i++) {
        tile16_pc_init(&map16_pc->tiles[i], &map16_snes->tiles[i]);
    }
}

void map16_pc_deinit(Map16PC* map16_pc, FreeList* free_list) {
    free_list_dealloc(free_list, map16_pc->tiles, map16_pc->length * sizeof(Tile16PC));
}
