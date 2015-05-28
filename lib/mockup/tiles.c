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

tile16_t tile16_from_tile8(tile8_t* t[4], tile_properties_t properties[4]) {
    tile16_t tile;
    for(int i = 0; i < 4; i++) {
        tile.tile8s[i] = t[i];
        tile.properties[i] = properties[i];
    }
    return tile;
}


void map16_init_fg(map16_t* map16, r65816_rom_t* rom, int num_level, map8_t* map8) {
    r65816_cpu_t cpu;
    r65816_cpu_init(&cpu, rom);

    cpu.ram[0x65] = cpu.rom->banks[5][0xE000 + 3 * num_level]; 
    cpu.ram[0x66] = cpu.rom->banks[5][0xE001 + 3 * num_level];
    cpu.ram[0x67] = cpu.rom->banks[5][0xE002 + 3 * num_level];

    r65816_cpu_add_exec_bp(&cpu, 0x0583B8);
    r65816_cpu_run_from(&cpu, 0x0583AC);
    map16->length = 512;
    map16->tiles = malloc(512 * sizeof(tile16_t));
    for(int i = 0; i < map16->length; i++) {
        int map16lookupSNES = 0x0D0000 + cpu.ram[2 * i + 0x0FBF] * 256 + cpu.ram[2 * i + 0x0FBE];
        int map16lookupPC   = ((map16lookupSNES & 0x7f0000) >> 1) + (map16lookupSNES & 0x7fff);

        for(int j = 0; j < 4; j++) {
            map16->tiles[i].properties[j] = (tile_properties_t)rom->data[map16lookupPC + j * 2 + 1];
            uint16_t num_tile             = rom->data[map16lookupPC + j * 2] | ((rom->data[map16lookupPC + j * 2 + 1] & 0x03) << 8);
            map16->tiles[i].tile8s[j]     = &map8->tiles[num_tile];
        }
    }

    r65816_cpu_free(&cpu);
}

void map16_init_bg(map16_t* map16, r65816_rom_t* rom, int num_level, map8_t* map8) {
    r65816_cpu_t cpu;
    r65816_cpu_init(&cpu, rom);

    cpu.ram[0x65] = cpu.rom->banks[5][0xE000 + 3 * num_level]; 
    cpu.ram[0x66] = cpu.rom->banks[5][0xE001 + 3 * num_level];
    cpu.ram[0x67] = cpu.rom->banks[5][0xE002 + 3 * num_level];

    r65816_cpu_add_exec_bp(&cpu, 0x0583B8);
    r65816_cpu_run_from(&cpu, 0x0583AC);

    map16->length = 512;
    map16->tiles = malloc(512 * sizeof(tile16_t));
    for(int i = 0; i < 512; i++) {
        int map16lookupSNES = 0x0D9100 + i*8;
        int map16lookupPC   = ((map16lookupSNES & 0x7f0000) >> 1) + (map16lookupSNES & 0x7fff);

        for(int j = 0; j < 4; j++) {
            map16->tiles[i].properties[j] = (tile_properties_t)rom->banks[0xD][9100 + i * 8 + j * 2 + 1];
            uint16_t num_tile             = rom->banks[0xD][9100 + i * 8 + j * 2];
            num_tile                     |= (rom->banks[0xD][9100 + i * 8 + j * 2 + 1] & 0b00000011) << 8;
            map16->tiles[i].tile8s[j]     = &map8->tiles[num_tile];
        }
    }

    r65816_cpu_free(&cpu);
}

void map16_deinit(map16_t* map16) {


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
                        16 * tile16->properties[0].CCC + tile16->tile8s[pos]->pixels[sy * 8 + sx];
                    sx += itx;
                }
                sy += ity;
            }
        }
    }
}

void map16_pc_init(map16_pc_t* map16_pc, map16_t* map16_snes) {
    map16_pc->length = map16_snes->length;
    map16_pc->tiles = malloc(map16_pc->length * 256 * sizeof(uint32_t));
    for(int i = 0; i < map16_pc->length; i++) {
        tile16_pc_init(&map16_pc->tiles[i], &map16_snes->tiles[i]);
    }
}

void map16_pc_deinit(map16_pc_t* map16_pc) {
    free(map16_pc->tiles);
}
