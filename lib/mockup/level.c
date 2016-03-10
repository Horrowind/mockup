#include <stdio.h>
#include <string.h>

#include <r65816.h>
#include "addresses.h"
#include "misc.h"
#include "decode.h"
#include "tileset.h"
#include "level.h"
#include "pool.h"

#define SPRITES
typedef struct {
    u16 x;
    u16 y;
    u16 tile;
    u16 is_top;
} object_tile_t;

void level_init(level_pc_t* l, r65816_rom_t* rom, int num_level, gfx_store_t* gfx_store) {
    u32 level_layer1_data_addr_sfc
        = (rom->data[level_layer1_data_table_pc + 2 + 3 * num_level] << 16)
        | (rom->data[level_layer1_data_table_pc + 1 + 3 * num_level] << 8)
        | (rom->data[level_layer1_data_table_pc + 0 + 3 * num_level] << 0);

    u32 level_layer2_data_addr_sfc
        = (rom->data[level_layer2_data_table_pc + 2 + 3 * num_level] << 16)
        | (rom->data[level_layer2_data_table_pc + 1 + 3 * num_level] << 8)
        | (rom->data[level_layer2_data_table_pc + 0 + 3 * num_level] << 0);

    u32 level_sprites_data_addr_sfc
        = 0x070000
        | (rom->data[level_layer2_data_table_pc + 0 + 2 * num_level] << 8)
        | (rom->data[level_layer2_data_table_pc + 1 + 2 * num_level] << 0);
    
    level_init_by_address(l, rom, num_level, gfx_store,
                          level_layer1_data_addr_sfc,
                          level_layer2_data_addr_sfc,
                          level_sprites_data_addr_sfc);
}

void level_init_by_address(level_pc_t* l, r65816_rom_t* rom, int num_level, gfx_store_t* gfx_store,
                           u32 level_layer1_data_addr_sfc,
                           u32 level_layer2_data_addr_sfc,
                           u32 level_sprite_data_addr_sfc) {

    l->rom = rom;
    l->num_level = num_level;
    //printf("Layer 1: %x\n", level_layer1_data_addr_sfc);
    u32 level_layer1_data_addr_pc = ((level_layer1_data_addr_sfc & 0x7f0000) >> 1) | (level_layer1_data_addr_sfc & 0x7fff);
    
    r65816_cpu_t cpu = {.regs.p.b = 0x24};
    r65816_cpu_init(&cpu, rom);

    cpu.ram[0x10c] = num_level / 256;
    cpu.ram[0x10b] = num_level & 0xFF;
    
    cpu.ram[0x65] = (level_layer1_data_addr_sfc & 0x0000FF) >> 0;
    cpu.ram[0x66] = (level_layer1_data_addr_sfc & 0x00FF00) >> 8;
    cpu.ram[0x67] = (level_layer1_data_addr_sfc & 0xFF0000) >> 16;

    cpu.ram[0x68] = (level_layer2_data_addr_sfc & 0x0000FF) >> 0;
    cpu.ram[0x69] = (level_layer2_data_addr_sfc & 0x00FF00) >> 8;
    cpu.ram[0x6A] = (level_layer2_data_addr_sfc & 0xFF0000) >> 16;
   
    memset(cpu.ram + level_map16_low_ram, 0x25, 0x3800);
    memset(cpu.ram + level_map16_high_ram, 0x00, 0x3800);

    r65816_cpu_add_exec_bp(&cpu, level_load_routine_end_sfc);
    r65816_cpu_run_from(&cpu, level_load_routine_sfc);
    // --- header init ------
    {
        memcpy(&(l->header), rom->data + level_layer1_data_addr_pc, 5);
        l->is_vertical_level = cpu.ram[0x5B] & 0x01;
        switch(l->header.level_mode) {
        case 0x00:
        case 0x0C:
        case 0x0E:
        case 0x11:
        case 0x1E:
        case 0x0A:
        case 0x0D:
            l->has_layer2_objects = 0;
            l->has_layer2_bg = 1;
            l->is_boss_level = 0;
            break;
            
        case 0x09: // > 
        case 0x0B: // >Boss level
        case 0x10: // >
            l->has_layer2_objects = 0;
            l->has_layer2_bg = 0;
            l->is_boss_level = 1;
            l->layer1_objects.length = 0;
#ifdef SPRITES
            l->sprites.length = 0;
#endif
        default:
            l->has_layer2_objects = 1;
            l->has_layer2_bg = 0;
            l->is_boss_level = 0;
            break;
        }


        int bg_color_addr = level_bg_area_color_data_table_pc + 2 * cpu.ram[level_header_bg_color_ram];
        // TODO: Maybe implement as macro/function
        u16 bg_color = rom->data[bg_color_addr] | (rom->data[bg_color_addr + 1] << 8);
        int r = ((bg_color & 0x001F) << 3); r |= (r >> 5);
        int g = ((bg_color & 0x03E0) >> 2); g |= (g >> 5);
        int b = ((bg_color & 0x7C00) >> 7); b |= (b >> 5);
        l->background_color = 0xFF000000 + (r) + (g << 8) + (b << 16);
    }
    
    
    // --- tileset init -----
    {
        int num_tileset = cpu.ram[level_header_tileset_ram];
        int tileset_addr = 0x292B + num_tileset * 4;
        u8 fg1 = rom->data[tileset_addr];
        u8 fg2 = rom->data[tileset_addr + 1];
        u8 bg1 = rom->data[tileset_addr + 2];
        u8 fg3 = rom->data[tileset_addr + 3];
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
    }

    // --- map16 fg init ----
    {
        l->map16_fg.length = 512;
        l->map16_fg.tiles = malloc(l->map16_fg.length * sizeof(tile16_t));
        for(int i = 0; i < l->map16_fg.length; i++) {
            int map16lookupSNES = map16_fg_tiles_zero_offset_sfc + cpu.ram[2 * i + 0x0FBF] * 256 + cpu.ram[2 * i + 0x0FBE];
            int map16lookupPC   = ((map16lookupSNES & 0x7f0000) >> 1) + (map16lookupSNES & 0x7fff);

            for(int j = 0; j < 4; j++) {
                l->map16_fg.tiles[i].properties[j] = (tile_properties_t)rom->data[map16lookupPC + j * 2 + 1];
                u16 num_tile =  rom->data[map16lookupPC + j * 2];
                num_tile         |= (rom->data[map16lookupPC + j * 2 + 1] & 0x03) << 8;
                if(num_tile == 0x3FF) num_tile = 0;
                l->map16_fg.tiles[i].tile8s[j] = &(l->map8.tiles[num_tile]);
            }
        }
    }
    
    // --- map16 bg init ----

    {
        l->map16_bg.length = 512;
        l->map16_bg.tiles = malloc(l->map16_bg.length * sizeof(tile16_t));
        for(int i = 0; i < l->map16_bg.length; i++) {
            int map16_addr_sfc = map16_bg_tiles_table_sfc + i*8;
            int map16_addr_pc  = ((map16_addr_sfc & 0x7f0000) >> 1) | (map16_addr_sfc & 0x7fff);        
            for(int j = 0; j < 4; j++) {
                l->map16_bg.tiles[i].properties[j] = (tile_properties_t)(rom->data[map16_addr_pc + j * 2 + 1]);
                u16 num_tile =  rom->data[map16_addr_pc + j * 2];
                num_tile         |= (rom->data[map16_addr_pc + j * 2 + 1] & 0x03) << 8;
                if(num_tile >= 0x200) num_tile = 0;
                l->map16_bg.tiles[i].tile8s[j] = &(l->map8.tiles[num_tile]);
            }
        }
    }

    
    if(!l->is_boss_level) {

        if(l->is_vertical_level) {
            l->width = 32;
            l->height = (l->header.screens + 1) * 16;
        } else {
            l->width = (l->header.screens + 1) * 16;
            l->height = 27;
        }
        
        // --- object init ------

        {
            r65816_cpu_add_exec_bp(&cpu, level_load_tiles_routine_end_sfc);
            r65816_cpu_add_exec_bp(&cpu, level_load_tiles_routine_end2_sfc);
            r65816_cpu_add_exec_bp(&cpu, level_load_tiles_routine_new_object_sfc);
            r65816_cpu_add_write_bp_range(&cpu, level_tiles_low_start_sfc, level_tiles_low_end_sfc);
            r65816_cpu_add_write_bp_range(&cpu, level_tiles_high_start_sfc, level_tiles_high_end_sfc);

            pool_t object_pool;
            pool_t tiles_pool;
            pool_init(&object_pool);
            pool_init(&tiles_pool);
    
            int xmax = 0, ymax = 0, xmin = 65535, ymin = 65535;
            int tile_count = 0;
            int obj_count = 0;
            object_pc_t* object = NULL;
            
            r65816_cpu_run(&cpu); // start level loading routine
            while((cpu.regs.pc.d != level_load_tiles_routine_end_sfc) &&
                  (cpu.regs.pc.d != level_load_tiles_routine_end2_sfc)) {
                if(cpu.regs.pc.d == level_load_tiles_routine_new_object_sfc) {
                    // we have a new object
                    if(obj_count == 0) {
                        object = (object_pc_t*)pool_alloc(&object_pool, sizeof(object_pc_t));
                        object->tiles = NULL;
                        xmax = 0; ymax = 0; xmin = 65535; ymin = 65535;
                        obj_count = 1;
                    }
                    if(tile_count > 0) {
                        object->bb_xmin = xmin;
                        object->bb_ymin = ymin;
                        object->bb_xmax = xmax;
                        object->bb_ymax = ymax;
                        object->x = (cpu.ram[0x57] & 0x0F) | (cpu.ram[0x1928] << 4);
                        object->y = ((cpu.ram[0x57] & 0xF0) >> 4) | (cpu.ram[0x0A] << 4);
                        object->num = cpu.ram[0x5A];
                        object->settings = cpu.ram[0x59];
                        int width  = xmax - xmin + 1;
                        int height = ymax - ymin + 1;
                        object->tiles = malloc(sizeof(u16) * width * height);
                        for(int i = 0; i < width * height; i++) object->tiles[i] = 0xFFFF;
                        for(int i = 0; i < tile_count; i++) {
                            u16 x = ((object_tile_t*)tiles_pool.data)[i].x - xmin;
                            u16 y = ((object_tile_t*)tiles_pool.data)[i].y - ymin;
                            u16 tilenum = ((object_tile_t*)tiles_pool.data)[i].tile;
                            u16 is_top = ((object_tile_t*)tiles_pool.data)[i].is_top;
                            int index = y * width + x;
                            if(object->tiles[index] == 0xFFFF) object->tiles[index] = 0x0025;
                            if(is_top) {
                                object->tiles[index] = (object->tiles[index] & 0x00FF) | tilenum;
                            } else {
                                object->tiles[index] = (object->tiles[index] & 0xFF00) | tilenum;
                            }
                        }
                        pool_empty(&tiles_pool);
                        tile_count = 0;
                    
                        object = (object_pc_t*)pool_alloc(&object_pool, sizeof(object_pc_t));
                        object->tiles = NULL;
                        xmax = 0; ymax = 0; xmin = 65535; ymin = 65535;
                        obj_count++;
                    }
                } else { // a new tile was written to the level data RAM location
                    //record access to this ram location
                    object_tile_t* object_tile = (object_tile_t*)pool_alloc(&tiles_pool, sizeof(object_tile_t));
                    tile_count++;
                    int pure_addr = (cpu.breakpoint_address & 0xFFFF) - 0xC800;
                    if(l->is_vertical_level) {
                        int x = pure_addr & 0x0F; int y = (pure_addr & 0xF0) >> 4;
                        int screen = pure_addr >> 4;  int left = screen & 0x10; int h = (screen & 0x3E0) >> 1;
                        object_tile->x = left + x;
                        object_tile->y = h + y;
                    } else {
                        int screen_addr = pure_addr % 432;
                        int screen = pure_addr / 432;
                        object_tile->x = (screen_addr & 0x0F) | (screen * 16);
                        object_tile->y = (screen_addr & 0xFF0) >> 4;
                    }
                    if(object_tile->x < xmin) xmin = object_tile->x;
                    if(object_tile->x > xmax) xmax = object_tile->x;
                    if(object_tile->y < ymin) ymin = object_tile->y;
                    if(object_tile->y > ymax) ymax = object_tile->y;
                    object_tile->is_top = cpu.breakpoint_address >= level_tiles_high_start_sfc;
                    object_tile->tile = object_tile->is_top ? (cpu.breakpoint_data << 8) : cpu.breakpoint_data;
                }
                r65816_cpu_run(&cpu);
            }
            if(tile_count > 0 && obj_count > 0) {
                object->bb_xmin = xmin;
                object->bb_ymin = ymin;
                object->bb_xmax = xmax;
                object->bb_ymax = ymax;
                object->x = (cpu.ram[level_current_screen_ram] << 4) | (cpu.ram[0x57] & 0x0F);
                object->y = ((cpu.ram[0x57] & 0xF0) >> 4) | (cpu.ram[0x0A] << 4);
                //TODO: set z_index
                object->num = cpu.ram[0x5A];
                object->settings = cpu.ram[0x59];
                int width  = xmax - xmin + 1;
                int height = ymax - ymin + 1;
                object->tiles = malloc(sizeof(u16) * width * height);
                // Mark each tile as unset (= 0xFFFF)
                for(int i = 0; i < width * height; i++) object->tiles[i] = 0xFFFF;
                for(int i = 0; i < tile_count; i++) {
                    u16 x = ((object_tile_t*)tiles_pool.data)[i].x - xmin;
                    u16 y = ((object_tile_t*)tiles_pool.data)[i].y - ymin;
                    u16 tilenum = ((object_tile_t*)tiles_pool.data)[i].tile;
                    u16 is_top = ((object_tile_t*)tiles_pool.data)[i].is_top;
                    int index = y * width + x;
                    // if tile is unset, initialize it to its default value.
                    if(object->tiles[index] == 0xFFFF) object->tiles[index] = 0x0025;
                    if(is_top) {
                        object->tiles[index] = (object->tiles[index] & 0x00FF) | tilenum;
                    } else {
                        object->tiles[index] = (object->tiles[index] & 0xFF00) | tilenum;
                    }
                }
                pool_empty(&tiles_pool);
                tile_count = 0;
            }

            pool_deinit(&tiles_pool);
    
            l->layer1_objects.length  = obj_count;
            l->layer1_objects.objects = (object_pc_t*)object_pool.data;
	    
        }

        // --- bg init ----------

        if(l->has_layer2_bg) {
            l->layer2_background.data = malloc(432 * 2 * sizeof(u16) + 300);
            int offset = 0;
            int addr = rom->data[level_layer2_data_table_pc + 2 + 3 * num_level];
            if(addr == 0xFF) addr = 0x0C;
            addr = (addr << 8) | rom->data[level_layer2_data_table_pc + 1 + 3 * num_level];
            addr = (addr << 8) | rom->data[level_layer2_data_table_pc + 0 + 3 * num_level];
            if((addr & 0xFFFF) >= 0xE8FE) offset = 0x100; //See CODE_058046
            int pos = 0;
            u8 cmd, byte;
            while(pos < 432 * 2) {
                cmd  = r65816_cpu_read(&cpu, addr);
                byte = r65816_cpu_read(&cpu, addr + 1);
                if(cmd == 0xFF && byte == 0xFF) break;
                u8 length = cmd & 0b01111111;
                if(cmd & 0x80) {
                    for(int i = 0; i <= length; i++) {
                        l->layer2_background.data[pos + i] = offset + byte;
                    }
                    pos += length + 1;
                    addr += 2;
                } else {
                    for(int i = 0; i <= length; i++) {
                        l->layer2_background.data[pos + i] = offset + r65816_cpu_read(&cpu, addr + i + 1);
                    }
                    pos += length + 1;
                    addr += length + 2;
                }
            }
        }
    
        // --- palette init -----
        {
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

        }

#ifdef SPRITES
        // --- sprites init -----
        {
            int number_sprites = 0;
            u8 low  = rom->data[0x02EC00 + 2 * num_level]; 
            u8 high = rom->data[0x02EC01 + 2 * num_level];
            u8 bank = 0x07;
            int level_sprite_data_addr_sfc  = (bank << 16) | (high << 8) | low;
            int level_sprite_data_addr_pc = ((level_sprite_data_addr_sfc & 0x7f0000) >> 1) + (level_sprite_data_addr_sfc & 0x7fff);

            pool_t sprite_pool;
            pool_t sprite_tile_pool;
            pool_init(&sprite_pool);
        
            while(rom->data[level_sprite_data_addr_pc + 3*number_sprites + 1] != 0xFF) {
                sprite_pc_t* sprite = (sprite_pc_t*)pool_alloc(&sprite_pool, sizeof(sprite_pc_t));
                r65816_cpu_clear(&cpu);
                cpu.ram[0x65] = rom->data[0x02E000 + 3 * num_level]; 
                cpu.ram[0x66] = rom->data[0x02E001 + 3 * num_level];
                cpu.ram[0x67] = rom->data[0x02E002 + 3 * num_level];

                cpu.ram[0xD1] = cpu.ram[0x94] = 0x00; // player x position
                cpu.ram[0xD2] = cpu.ram[0x95] = 0x00;
                cpu.ram[0xD3] = cpu.ram[0x96] = 0x40; // player y position
                cpu.ram[0xD4] = cpu.ram[0x97] = 0x01;
                cpu.ram[0x15EA] = 0x30;

                //Sets RAM 0x01 to the screen number of the sprite.
                int screen = (rom->data[level_sprite_data_addr_pc + 3*number_sprites + 1] & 0x2) << 3;
                screen |= rom->data[level_sprite_data_addr_pc + 3*number_sprites + 2] & 0xF;
                sprite->x = (screen << 4) | ((rom->data[level_sprite_data_addr_pc + 3*number_sprites + 2] & 0xF0) >> 4);
                sprite->y = rom->data[level_sprite_data_addr_pc + 3*number_sprites + 2] & 0xF1;
                sprite->y = ((sprite->y & 0xF0) >> 4) | ((sprite->y & 0x01) << 4);
                sprite->enemy = rom->data[level_sprite_data_addr_pc + 3*number_sprites + 3];
                sprite->extra_bits = (rom->data[level_sprite_data_addr_pc + 3*number_sprites + 1] & 0x0C) >> 2;
                sprite->tiles = 0;
                cpu.ram[0x1a] = 0;//(sprite->x << 4) & 0xFF;
                cpu.ram[0x1b] = 0;//((sprite->x) & 0xFF0) >> 4;
                cpu.ram[0x1c] = 0xc0;
                cpu.ram[0x1d] = 0x00;

                cpu.ram[0x14] = 0x01; // set frame counter to something fancy.
                cpu.ram[0x13] = 0x01;
            
                //Create a new sprite list in scratch ram with one sprite in it.
                cpu.ram[0xCE] = 0x7b;
                cpu.ram[0xCF] = 0x9c;
                cpu.ram[0xD0] = 0x7f;



                //Copy sprite to scratch ram
                cpu.ram[0x19c7b] = rom->data[level_sprite_data_addr_pc];
                cpu.ram[0x19c7c] = rom->data[level_sprite_data_addr_pc + 3*number_sprites + 1];
                cpu.ram[0x19c7d] = rom->data[level_sprite_data_addr_pc + 3*number_sprites + 2] & 0xF0;
                cpu.ram[0x19c7e] = rom->data[level_sprite_data_addr_pc + 3*number_sprites + 3];
                cpu.ram[0x19c7f] = 0xff;

                r65816_cpu_run_jsr(&cpu, 0x2a751);
                int spriteindex;                        
                for(int i = 0x0; i < 0x12; i++) {
                    if(cpu.ram[i + 0x9E] != 0) cpu.regs.x.w = spriteindex = i;
                    //cpu.ram[0x163E + i] = 0x1;
                    cpu.ram[0x1FE2 + i] = 0;
                }

                cpu.regs.db = 1;
                r65816_cpu_run_jsr(&cpu, 0x0185C3);
                cpu.ram[0x1a] = (sprite->x << 4) & 0xFF;
                cpu.ram[0x1b] = screen;
                cpu.regs.db = 0;
                pool_init(&sprite_tile_pool);
                int number_tiles = 0;
                for(int i = 0; i < 0x200; i+=4) {
                    if((cpu.ram[i + 0x200] | cpu.ram[i + 0x202] | cpu.ram[i + 0x203]) && (cpu.ram[i + 0x201] != 0xf0)) {
                    
                        u8 extrabits = (cpu.ram[(i / 4) + 0x400] >> (i % 4)) & 0x03;
                        //Does not take $2101 into account
                        if(extrabits & 0x02) {
                            sprite_tile_t* tile = (sprite_tile_t*)pool_alloc(&sprite_tile_pool, sizeof(sprite_tile_t));
                            tile->flip_x = (cpu.ram[i + 0x203] & 0x40) && 1;
                            tile->flip_y = (cpu.ram[i + 0x203] & 0x80) && 1;
                            tile->palette = 8 + ((cpu.ram[i + 0x203] >> 1) & 0x7);
                            tile->tile_num = ((cpu.ram[i + 0x203] << 8) |  cpu.ram[i + 0x202]) & 0x1FF;
                            tile->x = ((cpu.ram[0x1b] << 8) | cpu.ram[0x1a]) + cpu.ram[i + 0x200] - 9;
                            tile->y = ((cpu.ram[0x1d] << 8) | cpu.ram[0x1c]) + cpu.ram[i + 0x201];
                        } else {
                            sprite_tile_t* tiles = (sprite_tile_t*)pool_alloc(&sprite_tile_pool, 4 * sizeof(sprite_tile_t));
                            u8 flipx = cpu.ram[i + 0x203] & 0x40;
                            u8 flipy = cpu.ram[i + 0x203] & 0x80;

                            for(int j = 0; j < 4; j++) {
                                tiles[i].flip_x = (cpu.ram[i + 0x203] & 0x40) && 1;
                                tiles[i].flip_y = (cpu.ram[i + 0x203] & 0x80) && 1;
                                tiles[i].palette = 8 + ((cpu.ram[i + 0x203] >> 1) & 0x7);
                                tiles[i].x = ((cpu.ram[0x1b] << 8) | cpu.ram[0x1a]) + cpu.ram[i + 0x200] - 9;
                                tiles[i].y = ((cpu.ram[0x1d] << 8) | cpu.ram[0x1c]) + cpu.ram[i + 0x201];
                            }
                            tiles[1].x += 8;
                            tiles[2].y += 8;
                            tiles[3].x += 8;
                            tiles[3].y += 8;

                            int number = ((cpu.ram[i + 0x203] << 8) |  cpu.ram[i + 0x202]) & 0x1FF;

                            if(flipx) {
                                if(flipy) {
                                    tiles[0].tile_num = number +17;
                                    tiles[1].tile_num = number + 1;
                                    tiles[2].tile_num = number +16;
                                    tiles[3].tile_num = number + 0;
                                } else {
                                    tiles[0].tile_num = number + 1;
                                    tiles[1].tile_num = number +17;
                                    tiles[2].tile_num = number + 0;
                                    tiles[3].tile_num = number +16;
                                }
                            } else {
                                if(flipy) {
                                    tiles[0].tile_num = number +16;
                                    tiles[1].tile_num = number + 0;
                                    tiles[2].tile_num = number +17;
                                    tiles[3].tile_num = number + 1;
                                } else {
                                    tiles[0].tile_num = number + 0;
                                    tiles[1].tile_num = number +16;
                                    tiles[2].tile_num = number + 1;
                                    tiles[3].tile_num = number +17;
                                }
                            }
                        }
                        number_tiles++;
                    }
                }
                sprite->tiles = malloc(sizeof(sprite_tile_t) * number_tiles);
                memcpy(sprite->tiles, sprite_tile_pool.data, sizeof(sprite_tile_t) * number_tiles);
                pool_deinit(&sprite_tile_pool);

                number_sprites++;
            }
            l->sprites.length = number_sprites;
            l->sprites.data = (sprite_pc_t*)sprite_pool.data;
        }
#endif    
    }
    r65816_cpu_free(&cpu);

}




void level_deinit(level_pc_t* l) {
    if(l->is_boss_level) return;
    for(int i = 0; i < l->layer1_objects.length; i++) {
        if(l->layer1_objects.objects[i].tiles) free(l->layer1_objects.objects[i].tiles);
    }
#ifdef SPRITES
    for(int i = 0; i < l->sprites.length; i++) {
        if(l->sprites.data[i].tiles) free(l->sprites.data[i].tiles);
    }
    free(l->sprites.data);
#endif
    
    if(l->has_layer2_bg) free(l->layer2_background.data);
    free(l->layer1_objects.objects);
    free(l->map16_bg.tiles);
    free(l->map16_fg.tiles);
    free(l->map8.tiles);
}


void level_animate(level_pc_t* l, u8 frame, gfx_store_t* gfx_store) {
    if(l->is_boss_level) return;
    
    r65816_cpu_t cpu;
    r65816_cpu_init(&cpu, l->rom);

    cpu.ram[0x14] = frame;
    r65816_cpu_add_exec_bp(&cpu, 0x00A42F);
    r65816_cpu_run_from(&cpu, 0x00A418);
    int pos = cpu.sreg[0x0121];
    u32 tmp_color = cpu.sreg[0x0122];
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

    u16 source[3], dest[3];
    // Get the dma source addresses. It works... note that 1000 is decimal.
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

    r65816_cpu_free(&cpu);
}

void level_serialize(level_pc_t* level, level_save_locations_t locations) {
    
}
