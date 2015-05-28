#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <r65816/cpu.h>
#include "addresses.h"
#include "misc.h"
#include "decode.h"
#include "tileset.h"
#include "level.h"
#include "pool.h"

typedef struct {
    uint16_t x;
    uint16_t y;
    uint16_t tile;
} level_tile_t;

void level_init(level_t* l, r65816_rom_t* rom, int num_level, gfx_store_t* gfx_store) {
    l->rom = rom;
    l->num_level = num_level;

    r65816_cpu_t cpu = {.regs.p.b = 0x24};
    r65816_cpu_init(&cpu, rom);

    cpu.ram[0x10c] = num_level / 256;
    cpu.ram[0x10b] = num_level & 0xFF;
    
    cpu.ram[0x65] = rom->banks[5][0xE000 + 3 * num_level]; 
    cpu.ram[0x66] = rom->banks[5][0xE001 + 3 * num_level];
    cpu.ram[0x67] = rom->banks[5][0xE002 + 3 * num_level];

    cpu.ram[0x68] = rom->banks[5][0xE600 + 3 * num_level]; 
    cpu.ram[0x69] = rom->banks[5][0xE601 + 3 * num_level];
    cpu.ram[0x60] = rom->banks[5][0xE602 + 3 * num_level];

    r65816_cpu_add_exec_bp(&cpu, 0x0583B8);
    r65816_cpu_run_from(&cpu, 0x05801E);

    uint32_t level_address_snes = (cpu.ram[0x67] << 16) | (cpu.ram[0x66] << 8) | cpu.ram[0x65];
    uint32_t level_address_pc = ((level_address_snes & 0x7f0000) >> 1) + (level_address_snes & 0x7fff);
    
    memcpy(&(l->header), rom->data + level_address_pc, 5);

    // --- tileset init -----
    
    int num_tileset = cpu.ram[0x1931];
    int tileset_addr = 0x292B + num_tileset * 4;
    uint8_t fg1 = rom->data[tileset_addr];
    uint8_t fg2 = rom->data[tileset_addr + 1];
    uint8_t bg1 = rom->data[tileset_addr + 2];
    uint8_t fg3 = rom->data[tileset_addr + 3];
    if(fg1 >= gfx_store->num_pages) return;
    if(fg2 >= gfx_store->num_pages) return;
    if(bg1 >= gfx_store->num_pages) return;
    if(fg3 >= gfx_store->num_pages) return;

    l->tileset.fg1 = &gfx_store->pages[fg1];
    l->tileset.fg2 = &gfx_store->pages[fg2];
    l->tileset.bg1 = &gfx_store->pages[bg1];
    l->tileset.fg3 = &gfx_store->pages[fg3];
    
    l->map8.length = 512;
    l->map8.tiles = malloc(512 * sizeof(tile8_t));
    for(int tile = 0; tile < 128; tile++) {
        l->map8.tiles[tile] = tile8_from_3bpp(l->tileset.fg1->data + 24 * (tile % 128));
    }
    for(int tile = 128; tile < 256; tile++) {
        l->map8.tiles[tile] = tile8_from_3bpp(l->tileset.fg2->data + 24 * (tile % 128));
    }
    for(int tile = 256; tile < 384; tile++) {
        l->map8.tiles[tile] = tile8_from_3bpp(l->tileset.bg1->data + 24 * (tile % 128));
    }
    for(int tile = 384; tile < 512; tile++) {
        l->map8.tiles[tile] = tile8_from_3bpp(l->tileset.fg3->data + 24 * (tile % 128));
    }
    
    // --- map16 fg init ----

    l->map16_fg.length = 512;
    l->map16_fg.tiles = malloc(l->map16_fg.length * sizeof(tile16_t));
    for(int i = 0; i < l->map16_fg.length; i++) {
        int map16lookupSNES = 0x0D0000 + cpu.ram[2 * i + 0x0FBF] * 256 + cpu.ram[2 * i + 0x0FBE];
        int map16lookupPC   = ((map16lookupSNES & 0x7f0000) >> 1) + (map16lookupSNES & 0x7fff);

        for(int j = 0; j < 4; j++) {
            l->map16_fg.tiles[i].properties[j] = (tile_properties_t)rom->data[map16lookupPC + j * 2 + 1];
            uint16_t num_tile =  rom->data[map16lookupPC + j * 2];
            num_tile         |= (rom->data[map16lookupPC + j * 2 + 1] & 0x03) << 8;
            if(num_tile == 0x3FF) num_tile = 0;
            l->map16_fg.tiles[i].tile8s[j] = &(l->map8.tiles[num_tile]);
        }
    }
    // --- map16 bg init ----

    l->map16_bg.length = 512;
    l->map16_bg.tiles = malloc(l->map16_bg.length * sizeof(tile16_t));
    for(int i = 0; i < l->map16_bg.length; i++) {
        int map16lookupSNES = 0x0D9100 + i*8;
        int map16lookupPC   = ((map16lookupSNES & 0x7f0000) >> 1) + (map16lookupSNES & 0x7fff);        
        for(int j = 0; j < 4; j++) {
            l->map16_bg.tiles[i].properties[j] = (tile_properties_t)(rom->data[map16lookupPC + j * 2 + 1]);
            uint16_t num_tile =  rom->data[map16lookupPC + j * 2];
            num_tile         |= (rom->data[map16lookupPC + j * 2 + 1] & 0x03) << 8;
            if(num_tile >= 0x200) num_tile = 0;
            l->map16_bg.tiles[i].tile8s[j] = &(l->map8.tiles[num_tile]);
        }
    }

    // --- object init ------

    const uint32_t load_level_data_routine_end  = 0x05809D; 
    const uint32_t load_level_data_routine_end2 = 0x0586E2; 
    
    r65816_cpu_add_exec_bp(&cpu, load_level_data_routine_end);
    r65816_cpu_add_exec_bp(&cpu, load_level_data_routine_end2);
    r65816_cpu_add_exec_bp(&cpu, 0x0586C5);
    r65816_cpu_add_write_bp_range(&cpu, 0x7EC800, 0x7EFFFF);
    r65816_cpu_add_write_bp_range(&cpu, 0x7FC800, 0x7FFFFF);

    pool_t object_pool;
    pool_t tiles_pool;
    pool_init(&object_pool);
    pool_init(&tiles_pool);
    
    int xmax = 0, ymax = 0, xmin = 65535, ymin = 65535;
    int tile_count = 0;
    int obj_count = 0;
    object_t* object = NULL;

    r65816_cpu_run(&cpu); // start level loading routine
    if(cpu.ram[0x5B] & 0x01) { // if the level is a vertical level
        l->width = 32;
        l->height = l->header.screens * 16;
        /* while((cpu.regs.pc.d != load_level_data_routine_end) && (cpu.regs.pc.d != load_level_data_routine_end2)) { */
        /*     if(cpu.regs.pc.d == 0x586C5) { // we have a new object */
        /*         if(cpu.ram[0x5A] == 0) { // it is an extended object */
        /*             printf("Extended object: %02x\n", cpu.ram[0x59]); */
        /*         } else { */
        /*             printf("Normal object: %02x\n", cpu.ram[0x5A]); */
        /*         } */
        /*     } else { // a new tile was written to the level data RAM location */
        /*         printf("Tile %02x at %06x\n", cpu.breakpoint_data, cpu.breakpoint_address); */
            
        /*     } */
        /*     r65816_cpu_run(&cpu); */
        /* } */
    } else { // the level is not a vertical level
        l->width = l->header.screens * 16;
        l->height = 27;
        while((cpu.regs.pc.d != load_level_data_routine_end) && (cpu.regs.pc.d != load_level_data_routine_end2)) {
            if(cpu.regs.pc.d == 0x586C5) { // we have a new object
                if(tile_count > 0) {
                    object->bb_xmin = xmin; 
                    object->bb_ymin = ymin; 
                    object->bb_xmax = xmax; 
                    object->bb_ymax = ymax;
                    object->x = (cpu.ram[0x1928] << 4) | (cpu.ram[0x57] & 0x0F);
                    object->y = ((cpu.ram[0x57] & 0xF0) >> 4) | (cpu.ram[0x0A] << 4);
                    //TODO: set z_index
                    object->num = cpu.ram[0x5A];
                    object->settings = cpu.ram[0x59];
                    int width  = xmax - xmin + 1;
                    int height = ymax - ymin + 1;
                    object->tiles = malloc(sizeof(uint16_t) * width * height);
                    for(int i = 0; i < tile_count; i++) {
                        uint16_t x = ((level_tile_t*)tiles_pool.data)[i].x - xmin;
                        uint16_t y = ((level_tile_t*)tiles_pool.data)[i].y - ymin;
                        uint16_t tilenum = ((level_tile_t*)tiles_pool.data)[i].tile;
                        int index = y * width + x;
                        if(tilenum & 0xFF) {
                            object->tiles[index] = (object->tiles[index] & 0xFF00) | tilenum;
                        } else {
                            object->tiles[index] = (object->tiles[index] & 0x00FF) | (tilenum << 8);
                        }
                    }
                    pool_empty(&tiles_pool);
                    tile_count = 0;
                }
                object = (object_t*)pool_alloc(&object_pool, sizeof(object_t));
                xmax = 0; ymax = 0; xmin = 65535; ymin = 65535;
                obj_count++;
            } else { // a new tile was written to the level data RAM location
                //record access to this ram location
                level_tile_t* level_tile = (level_tile_t*)pool_alloc(&tiles_pool, sizeof(level_tile_t));
                tile_count++;
                int pure_addr = (cpu.breakpoint_address & 0xFFFF) - 0xC800;
                int screen_addr = pure_addr % 432;
                int screen = pure_addr / 432;
                level_tile->x = (screen_addr & 0x0F) | (screen << 16);
                level_tile->y = (screen_addr & 0xFF0) >> 4;
                if(level_tile->x < xmin) xmin = level_tile->x;
                if(level_tile->x > xmax) xmax = level_tile->x;
                if(level_tile->y < ymin) ymin = level_tile->y;
                if(level_tile->y > ymax) ymax = level_tile->y;
                level_tile->tile = cpu.breakpoint_data | (pure_addr > 0x7F0000 ? 0x100 : 0); 
            }
            r65816_cpu_run(&cpu);
        }
    }
    pool_deinit(&tiles_pool);
    l->layer1_objects.length  = obj_count;
    l->layer1_objects.objects = (object_t*)object_pool.data;

    // --- palette init -----

    cpu.regs.p.b = 0x24;
    r65816_cpu_add_exec_bp(&cpu, 0x00ACEC);
    r65816_cpu_run_from(&cpu, 0x00ABED);
    memcpy(l->palette.data, cpu.ram + 0x0703, 512);

    for(int i = 0; i < 256; i += 16) {
        l->palette.data[i] = 0;
    }

    for(int i = 0; i < 8; i++) {
        level_animate(l, i, gfx_store);
    }
    
    r65816_cpu_free(&cpu);
}

void level_deinit(level_t* l) {
    // The commented should work but segfaults
    /* for(int i = 0; i < l->layer1_objects.length; i++) { */
    /*     free(l->layer1_objects.objects[i].tiles); */
    /* } */
    free(l->layer1_objects.objects);
    free(l->map16_bg.tiles);
    free(l->map16_fg.tiles);
    free(l->map8.tiles);
}


void level_animate(level_t* l, uint8_t frame, gfx_store_t* gfx_store) {
    //std::cout<<"Frame: " << (int)frame << std::endl;
    r65816_cpu_t cpu;
    r65816_cpu_init(&cpu, l->rom);

    cpu.ram[0x14] = frame;
    r65816_cpu_add_exec_bp(&cpu, 0x00A42F);
    r65816_cpu_run_from(&cpu, 0x00A418);
    int pos = cpu.sreg[0x0121];
    uint32_t tmp_color = cpu.sreg[0x0122];
    r65816_cpu_step(&cpu);
    r65816_cpu_step(&cpu);
    l->palette.data[pos] = tmp_color | (cpu.sreg[0x0122] << 8);

    r65816_cpu_clear(&cpu);
    cpu.ram[0x65] = l->rom->banks[5][0xE000 + 3 * l->num_level];
    cpu.ram[0x66] = l->rom->banks[5][0xE001 + 3 * l->num_level];
    cpu.ram[0x67] = l->rom->banks[5][0xE002 + 3 * l->num_level];
    r65816_cpu_add_exec_bp(&cpu, 0x0583B8);
    r65816_cpu_run_from(&cpu, 0x0583AC);
    cpu.ram[0x0014] = frame;
    cpu.ram[0x14AD] = 0;
    cpu.ram[0x14AE] = 0;
    cpu.ram[0x14AF] = 0;
    cpu.regs.p.b = 0;
    cpu.regs.p.x = 1;
    cpu.regs.p.m = 1;
    //cpu.regs.d = 0x3000;

    r65816_cpu_add_exec_bp(&cpu, 0x00A601);
    r65816_cpu_run_from(&cpu, 0x00A5FD);

    uint16_t source[3], dest[3];
    // get the dma source addresses. It works... note that 1000 is decimal.
    source[0] = ((cpu.ram[0x0D77] << 8) + cpu.ram[0x0D76]) / 32 - 1000; 
    source[1] = ((cpu.ram[0x0D79] << 8) + cpu.ram[0x0D78]) / 32 - 1000;
    source[2] = ((cpu.ram[0x0D7B] << 8) + cpu.ram[0x0D7A]) / 32 - 1000;

    dest[0] = ((cpu.ram[0x0D7D] << 8) | cpu.ram[0x0D7C]) / 16;
    dest[1] = ((cpu.ram[0x0D7F] << 8) | cpu.ram[0x0D7E]) / 16;
    dest[2] = ((cpu.ram[0x0D81] << 8) | cpu.ram[0x0D80]) / 16;
	       
    for(int i = 0; i < 3; i++) {
        if(dest[i] == 0 || ((source[i] & 0xFF00) == 0xFF00)) continue;
        if(dest[i] == 0x80) {                                              //See CODE_00A3DA
            l->map8.tiles[dest[i]     ] = tile8_from_3bpp(gfx_store->pages[0x33].data + 24 * (source[i] + 0));
            l->map8.tiles[dest[i] +  1] = tile8_from_3bpp(gfx_store->pages[0x33].data + 24 * (source[i] + 1));
            l->map8.tiles[dest[i] + 16] = tile8_from_3bpp(gfx_store->pages[0x33].data + 24 * (source[i] + 2));
            l->map8.tiles[dest[i] + 17] = tile8_from_3bpp(gfx_store->pages[0x33].data + 24 * (source[i] + 3));
        } else {
            for(int j = 0; j < 4; j++) {
                l->map8.tiles[dest[i] + j] = tile8_from_3bpp(gfx_store->pages[0x33].data + 24 * (source[i] + j));
            }
        }
    }
}



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
