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

void tile16_to_pc(tile16_t* tile16, tile16_pc_t* tile16_pc) {
    for(int k = 0; k < 1; k++) {
        for(int l = 0; l < 1; l++) {
            int pos = k*2 +l;
            int itx = tile16->properties[pos].X ? -1 : 1;
            int ity = tile16->properties[pos].Y ? -1 : 1; 
            int sy  = tile16->properties[pos].Y ?  7 : 0;
            for(int i = 0; i < 8; i++) {
                int sx  = tile16->properties[pos].X ?  7 : 0;
                for(int j = 0; j < 8; j++) {
                    tile16_pc->data[16* l + (2 * k + i) * 8 + j] = tile16->tiles[pos]->pixels[sy * 8 + sx];
                    sx += itx;
                }
                sy += ity;
            }
        }
    }
/*     tile8_t tile_new;
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
}

tile16_t tile16_from_tile8(tile8_t* t[4], tile_properties_t properties[4]) {
    tile16_t tile;
    for(int i = 0; i < 4; i++) {
        tile.tiles[i] = t[i];
        tile.properties[i] = properties[i];
    }
    return tile;
}

void map16_init_bg(map16_t* map16, r65816_rom_t* rom, int num_level, map8_t* map8) {

}
void map16_init_fg(map16_t* map16, r65816_rom_t* rom, int num_level, map8_t* map8) {

}

void map8_init(map8_t* map8, tileset_t* tileset) {
    map8->length = 512;
    map8->tiles = malloc(512 * sizeof(tile8_t));
    for(int tile = 0; tile < 512; tile++) {
        gfx_page_t* used_chr;
        switch(tile >> 7) {
        case 0:
            used_chr = tileset->fg1;
            break;
        case 1:
            used_chr = tileset->fg2;
            break;
        case 2:
            used_chr = tileset->bg1;
            break;
        case 3:
            used_chr = tileset->fg3;
            break;
        }
        map8->tiles[tile] = tile8_from_3bpp(used_chr->data + 24 * (tile % 128));
    }
}

void map8_deinit(map8_t* map8) {
    free(map8->tiles);
}
