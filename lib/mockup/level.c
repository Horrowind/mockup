#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <r65816/cpu.h>
#include "addresses.h"
#include "misc.h"
#include "decode.h"
#include "level.h"

void level_init(level_t* l, r65816_rom_t* rom, gfx_store_t* gfx_store, int num_level) {
    l->rom = rom;

    level_header_init(&l->header, rom, num_level);
    palette_init(&l->palette, rom, num_level);
    tileset_init_level(&l->tileset, rom, num_level, gfx_store);
    map8_init(&l->map8, &l->tileset);
    map16_init_bg(&l->map16_bg, rom, num_level, &l->map8);
    map16_init_fg(&l->map16_fg, rom, num_level, &l->map8);
    //Todo
    int addr = 0;
    object_list_init_addr(l->layer1_objects, rom, addr);

    /* for(int i = 0; i < 8; i++) { */
    /*     level_animate(l, i); */
    /* } */
}

void level_deinit(level_t* l) {
    if(l->has_layer2_objects) object_list_deinit(l->layer2_objects);
    if(l->has_layer2_bg) layer16_deinit(l->layer2_background);
    object_list_deinit(l->layer1_objects);
}

/* void level_load_map16(level_t* l, r65816_cpu_t* cpu) { */
/*     r65816_cpu_t cpu; */
/*     r65816_cpu_init(&cpu, l->rom); */
    
/*     cpu.ram[0x65] = l->rom->data[0x02E000 + 3 * l->levelnum]; */
/*     cpu.ram[0x66] = l->rom->data[0x02E000 + 3 * l->levelnum + 1]; */
/*     cpu.ram[0x67] = l->rom->data[0x02E000 + 3 * l->levelnum + 2]; */

/*     // Run the palette loading routine. */
/*     r65816_cpu_add_exec_bp(&cpu, 0x0583B8); */
/*     r65816_cpu_run(&cpu, 0x0583AC); */
    
/*     int bgPalette = cpu.ram[0x1930]; */
              
/*     for(int i = 0; i < 512; i++) { */
/*         int map16lookupSNES = 0x0D0000 + cpu.ram[2 * i + 0x0FBF] * 256 + cpu.ram[2 * i + 0x0FBE]; */
/*         int map16lookupPC   = ((map16lookupSNES & 0x7f0000) >> 1) + (map16lookupSNES & 0x7fff); */
            
/*         tile8_t   tiles[4]; */
/*         uint8_t palettes[4]; */
/*         tile_flip_t flip; */
/*         int tile; */
/*         for(int j = 0; j < 4; j++) { */
/*             flip.XY     = (l->rom->data[map16lookupPC + j * 2 + 1] & 0b11000000) >> 6; */
/*             palettes[j] = (l->rom->data[map16lookupPC + j * 2 + 1] & 0b00011100) >> 2; */
/*             tile        =  l->rom->data[map16lookupPC + j * 2] | ((l->rom->data[map16lookupPC + j * 2 + 1] & 0b00000011) << 8); */
/*             tiles[j]    =  tile8_flip(l->map8[tile], flip); */
/*         } */

/*         l->map16_fg[i] = tile16_from_tile8(tiles, palettes); */
/*     } */

/*     for(int i = 0; i < 512; i++) { */
/*         int map16lookupSNES = 0x0D9100 + i*8; */
/*         int map16lookupPC   = ((map16lookupSNES & 0x7f0000) >> 1) + (map16lookupSNES & 0x7fff); */

/*         tile8_t   tiles[4]; */
/*         uint8_t palettes[4]; */
/*         tile_flip_t flip; */
/*         int tile; */
/*         for(int j = 0; j < 4; j++) { */
/*             flip.XY     = (l->rom->data[map16lookupPC + j * 2 + 1] & 0b11000000) >> 6; */
/*             palettes[j] = (l->rom->data[map16lookupPC + j * 2 + 1] & 0b00011100) >> 2; */
/*             tile        =  l->rom->data[map16lookupPC + j * 2] | ((l->rom->data[map16lookupPC + j * 2 + 1] & 0b00000011) << 8); */
/*             tiles[j]    =  tile8_flip(l->map8[tile], flip); */

/*         } */

/*        	l->map16_bg[i] = tile16_from_tile8(tiles, palettes); */
/*     } */

/*     r65816_cpu_free(&cpu); */
/* } */

/* void level_load_objects(level_t* l, r65816_cpu_t* cpu) { */
/*     r65816_cpu_t cpu; */
/*     r65816_cpu_init(&cpu, l->rom); */
    
/*     cpu.ram[0x65] = l->rom->data[0x02E000 + 3 * l->levelnum]; */
/*     cpu.ram[0x66] = l->rom->data[0x02E000 + 3 * l->levelnum + 1]; */
/*     cpu.ram[0x67] = l->rom->data[0x02E000 + 3 * l->levelnum + 2]; */

/*     // Run the palette loading routine. */
/*     r65816_cpu_add_exec_bp(&cpu, 0x0583B8); */
/*     r65816_cpu_run(&cpu, 0x0583AC); */
        
/*     for(int i = 0; i < 0x14 * 27 * 16; i++) { */
/*         cpu.ram[0x0C800 + i] = 0x25; */
/*         cpu.ram[0x1C800 + i] = 0; */
/*     } */
    
/*     int levelmode = cpu.ram[0x1925]; */
/*     int bgColorAddr = 0x30A0 + 2 * cpu.ram[0x192F]; */
/*     l->background_color = convertColor(l->rom->data[bgColorAddr] | (l->rom->data[bgColorAddr + 1] << 8)); */
/*     switch(levelmode) { */
/*     case 0x00: */
/*     case 0x0C: */
/*     case 0x0E: */
/*     case 0x11: */
/*     case 0x1E: */
/*     case 0x0A: */
/*     case 0x0D: */
/*         l->hasLayer2Objects = true; */
/*         break; */
	    
/*     case 0x09: // \ */
/*     case 0x0B: // | Boss level */
/*     case 0x10: // / */
/*         l->layer1    = new uint16_t[1]; */
/*         l->layer1[0] = 0x25; */
/*         l->width     = 1; */
/*         l->height    = 1; */
/*         l->hasLayer2Objects = false; */
/*         return; */
/*     default: */
/*         l->hasLayer2Objects = false; */
/*         break; */
/*     } */

/*     l->cpu.run(0x0583CF, 0x0583D2); */
/*     int screens = cpu.ram[0x5D]; */
/*     bool isVerticalLevel = cpu.ram[0x5B] & 0x01; */

/*     l->cpu.run(0x0583AC, 0x0583B8); */
/*     l->hasLayer2BG = (l->rom->data[0x02E600 + 3 * l->levelnum + 2] == 0xFF); */
/*     // cpu.ram[0x1933] = 1; //Object */
/*     // l->cpu.run(0x0583CF, 0x0583D2); */
    
        
        
    
/*     if(l->layer1) { */
/*         delete[] l->layer1; */
/*         l->layer1 = NULL; */
/*     } */
/*     if(l->layer2) { */
/*         delete[] l->layer2; */
/*         l->layer2 = NULL; */
/*     } */

/*     // Layer 1 */
/*     if(isVerticalLevel) { */
/*         l->layer1 = new uint16_t[512 * screens]; */
/*         l->width = 32; */
/*         l->height = screens * 16; */
/*         for(int i = 0; i < 512 * screens; i++) { */
/*             int xy = i % 256; int x = xy % 16; int y = xy >> 4; */
/*             int sc = i >> 8;  int left = sc&1; int h = sc >> 1; */
            
/*             int cx = left * 16 + x; */
/*             int cy = h * 16 + y; */
            
/*             l->layer1[cy * 32 + cx] = cpu.ram[0x1C800 + i] * 256 + cpu.ram[0x0C800 + i]; */
/*         } */
/*     } else { */
/*         l->layer1 = new uint16_t[432 * screens]; */
/*         l->width = screens * 16; */
/*         l->height = 27; */
/*         for(int i = 0; i < 432 * screens; i++) { */
/*             int xy = i % 432; */
/*             int sc = i / 432; */
/*             int cx = (xy % 16); */
/*             int cy = xy >> 4; */
/*             l->layer1[(cy * screens + sc) * 16 + cx] = cpu.ram[0x1C800 + i] * 256 + cpu.ram[0x0C800 + i]; */
/*         } */
/*     } */

/*     // Layer 2 */
/*     if(l->hasLayer2BG) { */
/*         l->layer2 = new uint16_t[432*2]; */
/*         int addr = l->rom->data[0x02E600 + 3 * l->levelnum + 2]; */
/*         int offset = 0; */
/*         if(addr == 0xFF) addr = 0x0C; */
/*         addr = (addr << 8) | l->rom->data[0x02E600 + 3 * l->levelnum + 1]; */
/*         addr = (addr << 8) | l->rom->data[0x02E600 + 3 * l->levelnum + 0]; */
/*         if((addr & 0xFFFF) >= 0xE8FE) offset = 0x100;                              //See CODE_058046 */
/*         int pos = 0; */
/*         while((l->cpu.op_read(addr) != 0xFF || l->cpu.op_read(addr + 1) != 0xFF) && pos < 432 * 2) { */
/*             uint8_t cmd = l->cpu.op_read(addr); */
/*             uint8_t length = cmd & 0b01111111; */
/*             if(cmd & 0x80) { */
/*                 for(int i = 0; i <= length; i++) l->layer2[pos + i] = offset + l->cpu.op_read(addr + 1); */
/*                 pos += length + 1; */
/*                 addr += 2; */
/*             } else { */
/*                 for(int i = 0; i <= length; i++) l->layer2[pos + i] = offset + l->cpu.op_read(addr + i + 1); */
/*                 pos += length + 1; */
/*                 addr += length + 2; */
/*             } */
/*         } */
/*     } else if(l->hasLayer2Objects) { */
/*         int addrLayer2LowTableEntryPC = l->rom->data[0x003DA8 + 2 * levelmode]; */
/*         int addrLayer2Low = l->rom->data[addrLayer2LowTableEntryPC] + (l->rom->data[addrLayer2LowTableEntryPC + 1] << 8); */
/*         int addrLayer2HighTableEntryPC = l->rom->data[0x003DA8 + 2 * levelmode]; */
/*         int addrLayer2High = l->rom->data[addrLayer2LowTableEntryPC] + (l->rom->data[addrLayer2LowTableEntryPC + 1] << 8); */
/*         if(isVerticalLevel) { */
/*             l->layer2 = new uint16_t[512 * screens]; */
/*             for(int i = 0; i < 512 * screens; i++) { */
/*                 int xy = i % 256; int x = xy % 16; int y = xy >> 4; */
/*                 int sc = i >> 8;  int left = sc&1; int h = sc >> 1; */
            
/*                 int cx = left * 16 + x; */
/*                 int cy = h * 16 + y; */
            
/*                 l->layer2[cy * 32 + cx] = cpu.ram[addrLayer2Low + i] * 256 + cpu.ram[addrLayer2High + i]; */
                    
/*             } */
/*         } else { */
/*             l->layer2 = new uint16_t[432 * screens]; */
/*             for(int i = 0; i < 432 * screens; i++) { */
/*                 int xy = i % 432; */
/*                 int sc = i / 432; */
/*                 int cx = (xy % 16); */
/*                 int cy = xy >> 4; */
/*                 l->layer2[(cy * screens + sc) * 16 + cx] = cpu.ram[addrLayer2Low + i] * 256 + cpu.ram[addrLayer2High + i]; */
/*             } */
/*         } */
/*     } */

/*     r65816_cpu_free(&cpu); */
/* } */

/* void level_get_layer1(r65816_cpu* cpu, uint8_t frame, layer16_t* layer) { */
        
/* } */


/* int Level::getHeight() { */
/*     return l->height; */
/* } */

/* int Level::getWidth() { */
/*     return l->width; */
/* } */

/* bool Level::hasLayer2BG() { */
/*     return l->hasLayer2BG; */
/* } */

/* bool Level::hasLayer2Objects() { */
/*     return l->hasLayer2Objects; */
/* } */

/* bool Level::isVerticalLevel() { */
/*     return l->isVerticalLevel; */
/* } */

/* uint32_t* Level::getPalette() { */
/*     return l->palette; */
/* } */

/* uint32_t Level::getBackgroundColor() { */
/*     return l->backgroundColor; */
/* } */


/* Tile8 Level::getMap8(int i) { */
/*     return l->map8[i]; */
/* } */

/* Tile8 Level::getGFX3233(int i) { */
/*     return l->gfx3233[i]; */
/* } */


/* Tile16 Level::getMap16fg(int i) { */
/*     return l->map16fg[i]; */
/* } */

/* Tile16 Level::getMap16bg(int i) { */
/*     return l->map16bg[i]; */
/* } */

/* uint16_t Level::getTile16(int x, int y) { */
/*     return l->layer1[y * l->width + x]; */
/* } */


/* void Level::animate(uint8_t frame) { */
/*     //std::cout<<"Frame: " << (int)frame << std::endl; */
/*     l->cpu.clear_ram(); */
/*     cpu.ram[0x14] = frame; */
/*     l->cpu.run(0x00A418, 0x00A42F); */
/*     int pos = l->cpu.op_read(0x2121); */
/*     uint32_t snesColor = l->cpu.op_read(0x2122); */
/*     l->cpu.step(); l->cpu.step(); */
/*     snesColor = snesColor | (l->cpu.op_read(0x2122) << 8); */
/*     l->palette[pos] = convertColor(snesColor); */

/*     l->cpu.clear_ram(); */
/*     cpu.ram[0x65] = l->rom->data[0x02E000 + 3 * l->levelnum];  */
/*     cpu.ram[0x66] = l->rom->data[0x02E000 + 3 * l->levelnum + 1]; */
/*     cpu.ram[0x67] = l->rom->data[0x02E000 + 3 * l->levelnum + 2]; */
/*     l->cpu.run(0x0583AC, 0x0583B8); */
/*     cpu.ram[0x0014] = frame; */
/*     cpu.ram[0x14AD] = 0; */
/*     cpu.ram[0x14AE] = 0; */
/*     cpu.ram[0x14AF] = 0; */
/*     l->cpu.regs.p = 0; */
/*     l->cpu.regs.p.x = true; */
/*     l->cpu.regs.p.m = true; */
/*     //l->cpu.regs.d = 0x3000; */

/*     //        l->cpu.setDebug(true); */
/*     l->cpu.run(0x00A5FD, 0x00A601); */
/*     //        l->cpu.setDebug(false); */

/*     uint16_t source[3], dest[3]; */
/*     source[0] = ((cpu.ram[0x0D77] << 8) + cpu.ram[0x0D76] - 0x2000) / 32;  */
/*     source[1] = ((cpu.ram[0x0D79] << 8) + cpu.ram[0x0D78] - 0x2000) / 32;  */
/*     source[2] = ((cpu.ram[0x0D7B] << 8) + cpu.ram[0x0D7A] - 0x2000) / 32;  */

/*     dest[0] = ((cpu.ram[0x0D7D] << 8) | cpu.ram[0x0D7C]) / 16; */
/*     dest[1] = ((cpu.ram[0x0D7F] << 8) | cpu.ram[0x0D7E]) / 16;  */
/*     dest[2] = ((cpu.ram[0x0D81] << 8) | cpu.ram[0x0D80]) / 16; */
	       
/*     for(int i = 0; i < 3; i++) { */
/* 	if(dest[i] == 0 || ((source[i] & 0xFF00) == 0xFF00)) continue; */
/* 	if(dest[i] == 0x80) {                                              //See CODE_00A3DA */
/* 	    l->map8[dest[i]] = l->gfx3233[source[i]]; */
/* 	    l->map8[dest[i] + 1] = l->gfx3233[source[i] + 1]; */
/* 	    l->map8[dest[i] + 16] = l->gfx3233[source[i] + 2]; */
/* 	    l->map8[dest[i] + 17] = l->gfx3233[source[i] + 3]; */
/* 	} else { */
/* 	    for(int j = 0; j < 4; j++) { */
/* 		l->map8[dest[i] + j] = l->gfx3233[source[i] +j]; */
/* 	    } */
/* 	} */
/*     } */
/*     load_map16(); */
	
/* } */

