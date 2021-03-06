#include "smw.h"
#include "addresses.h"
#include "misc.h"

void smw_init(SMW* smw, Wdc65816MapperBuilder* rom, Arena* arena, Arena* temp_arena) {
    smw->cpu = (Wdc65816Cpu){.regs.p.b = 0x24};

    u8* work_buffer = arena_alloc_array(arena, wdc65816_cpu_get_work_buffer_size(), u8);
    
    wdc65816_cpu_init(&smw->cpu, rom, work_buffer);
    gfx_store_init(&smw->gfx_pages, &smw->cpu.read_mapper, arena);

    smw->arena = arena_subarena(arena, MB(96));
    smw->temp_arena = temp_arena;
    free_list_init(&smw->free_list, arena_subarena(arena, MB(64)), 4096);
}

void smw_deinit(SMW* smw) {
    (void)smw;
}

void smw_init_all(SMW* smw, Wdc65816MapperBuilder* rom, Arena* arena, Arena* temp_arena) {
    smw_init(smw, rom, arena, temp_arena);
    for(int i = 0; i < 512; i++) {
        smw->levels[i] = arena_alloc_type(&smw->arena, LevelPC);
        smw_level_load(smw, i);
        c_print_format("Loaded level %03x\n", i);
    }
}

void smw_clear(SMW* smw) {
    arena_clear(&smw->arena);
    for(int i = 0; i < 512; i++) {
        smw->levels[i] = NULL;
    }
}

LevelPC* smw_get_level(SMW* smw, u16 level_num) {
    if(!smw->levels[level_num]) {
        smw->levels[level_num] = arena_alloc_type(&smw->arena, LevelPC);
        smw_level_load(smw, level_num);
    }
    return smw->levels[level_num];
}

void smw_deinit_all(SMW* smw) {
    smw_deinit(smw);
}

#ifdef SPRITES
SpriteTableEntries smw_get_sprite_table_entries(u8* ram, int sprite_index) {
    SpriteTableEntries result;
    result.entry_009E = ram[0x009E + sprite_index];
    result.entry_00AA = ram[0x00AA + sprite_index];
    result.entry_00B6 = ram[0x00B6 + sprite_index];
    result.entry_00C2 = ram[0x00C2 + sprite_index];
    result.entry_00D8 = ram[0x00D8 + sprite_index];
    result.entry_00E4 = ram[0x00E4 + sprite_index];
    result.entry_14C8 = ram[0x14C8 + sprite_index];
    result.entry_14D4 = ram[0x14D4 + sprite_index];
    result.entry_14E0 = ram[0x14E0 + sprite_index];
    result.entry_14EC = ram[0x14EC + sprite_index];
    result.entry_14F8 = ram[0x14F8 + sprite_index];
    result.entry_1504 = ram[0x1504 + sprite_index];
    result.entry_1510 = ram[0x1510 + sprite_index];
    result.entry_151C = ram[0x151C + sprite_index];
    result.entry_1528 = ram[0x1528 + sprite_index];
    result.entry_1534 = ram[0x1534 + sprite_index];
    result.entry_1540 = ram[0x1540 + sprite_index];
    result.entry_154C = ram[0x154C + sprite_index];
    result.entry_1558 = ram[0x1558 + sprite_index];
    result.entry_1564 = ram[0x1564 + sprite_index];
    result.entry_1570 = ram[0x1570 + sprite_index];
    result.entry_157C = ram[0x157C + sprite_index];
    result.entry_1588 = ram[0x1588 + sprite_index];
    result.entry_1594 = ram[0x1594 + sprite_index];
    result.entry_15A0 = ram[0x15A0 + sprite_index];
    result.entry_15AC = ram[0x15AC + sprite_index];
    result.entry_15B8 = ram[0x15B8 + sprite_index];
    result.entry_15C4 = ram[0x15C4 + sprite_index];
    result.entry_15D0 = ram[0x15D0 + sprite_index];
    result.entry_15DC = ram[0x15DC + sprite_index];
    result.entry_15EA = ram[0x15EA + sprite_index];
    result.entry_15F6 = ram[0x15F6 + sprite_index];
    result.entry_1602 = ram[0x1602 + sprite_index];
    result.entry_160E = ram[0x160E + sprite_index];
    result.entry_161A = ram[0x161A + sprite_index];
    result.entry_1626 = ram[0x1626 + sprite_index];
    result.entry_1632 = ram[0x1632 + sprite_index];
    result.entry_163E = ram[0x163E + sprite_index];
    result.entry_164A = ram[0x164A + sprite_index];
    result.entry_1656 = ram[0x1656 + sprite_index];
    result.entry_1662 = ram[0x1662 + sprite_index];
    result.entry_166E = ram[0x166E + sprite_index];
    result.entry_167A = ram[0x167A + sprite_index];
    result.entry_1686 = ram[0x1686 + sprite_index];
    result.entry_186C = ram[0x186C + sprite_index];
    result.entry_187B = ram[0x187B + sprite_index];
    result.entry_190F = ram[0x190F + sprite_index];
    result.entry_1FE2 = ram[0x1FE2 + sprite_index];
    return result;
}

void smw_set_sprite_table_entries(u8* ram, SpriteTableEntries entries, int sprite_index) {
    ram[0x009E + sprite_index] = entries.entry_009E;
    ram[0x00AA + sprite_index] = entries.entry_00AA;
    ram[0x00B6 + sprite_index] = entries.entry_00B6;
    ram[0x00C2 + sprite_index] = entries.entry_00C2;
    ram[0x00D8 + sprite_index] = entries.entry_00D8;
    ram[0x00E4 + sprite_index] = entries.entry_00E4;
    ram[0x14C8 + sprite_index] = entries.entry_14C8;
    ram[0x14D4 + sprite_index] = entries.entry_14D4;
    ram[0x14E0 + sprite_index] = entries.entry_14E0;
    ram[0x14EC + sprite_index] = entries.entry_14EC;
    ram[0x14F8 + sprite_index] = entries.entry_14F8;
    ram[0x1504 + sprite_index] = entries.entry_1504;
    ram[0x1510 + sprite_index] = entries.entry_1510;
    ram[0x151C + sprite_index] = entries.entry_151C;
    ram[0x1528 + sprite_index] = entries.entry_1528;
    ram[0x1534 + sprite_index] = entries.entry_1534;
    ram[0x1540 + sprite_index] = entries.entry_1540;
    ram[0x154C + sprite_index] = entries.entry_154C;
    ram[0x1558 + sprite_index] = entries.entry_1558;
    ram[0x1564 + sprite_index] = entries.entry_1564;
    ram[0x1570 + sprite_index] = entries.entry_1570;
    ram[0x157C + sprite_index] = entries.entry_157C;
    ram[0x1588 + sprite_index] = entries.entry_1588;
    ram[0x1594 + sprite_index] = entries.entry_1594;
    ram[0x15A0 + sprite_index] = entries.entry_15A0;
    ram[0x15AC + sprite_index] = entries.entry_15AC;
    ram[0x15B8 + sprite_index] = entries.entry_15B8;
    ram[0x15C4 + sprite_index] = entries.entry_15C4;
    ram[0x15D0 + sprite_index] = entries.entry_15D0;
    ram[0x15DC + sprite_index] = entries.entry_15DC;
    ram[0x15EA + sprite_index] = entries.entry_15EA;
    ram[0x15F6 + sprite_index] = entries.entry_15F6;
    ram[0x1602 + sprite_index] = entries.entry_1602;
    ram[0x160E + sprite_index] = entries.entry_160E;
    ram[0x161A + sprite_index] = entries.entry_161A;
    ram[0x1626 + sprite_index] = entries.entry_1626;
    ram[0x1632 + sprite_index] = entries.entry_1632;
    ram[0x163E + sprite_index] = entries.entry_163E;
    ram[0x164A + sprite_index] = entries.entry_164A;
    ram[0x1656 + sprite_index] = entries.entry_1656;
    ram[0x1662 + sprite_index] = entries.entry_1662;
    ram[0x166E + sprite_index] = entries.entry_166E;
    ram[0x167A + sprite_index] = entries.entry_167A;
    ram[0x1686 + sprite_index] = entries.entry_1686;
    ram[0x186C + sprite_index] = entries.entry_186C;
    ram[0x187B + sprite_index] = entries.entry_187B;
    ram[0x190F + sprite_index] = entries.entry_190F;
    ram[0x1FE2 + sprite_index] = entries.entry_1FE2;
}
#endif

void smw_level_deinit(SMW* smw, u16 level_num) {
    LevelPC* l = smw->levels[level_num];
    map16_pc_deinit(&l->map16_fg, &smw->free_list);
    map16_pc_deinit(&l->map16_bg, &smw->free_list);
}

void smw_level_load(SMW* smw, u16 level_num) {
    Wdc65816Mapper* rom = &smw->cpu.read_mapper;
    GFXStore gfx_pages = smw->gfx_pages;
    LevelPC* l = smw->levels[level_num];
    Arena*    arena = &smw->arena; 
    Arena*    temp_arena = smw->temp_arena;

    Wdc65816Cpu* cpu = &smw->cpu;
    wdc65816_cpu_clear(cpu);

    u32 level_layer1_data_addr_sfc
        = (wdc65816_mapper_read(&cpu->read_mapper, level_layer1_data_table_sfc + 2 + 3 * level_num) << 16)
        | (wdc65816_mapper_read(&cpu->read_mapper, level_layer1_data_table_sfc + 1 + 3 * level_num) << 8)
        | (wdc65816_mapper_read(&cpu->read_mapper, level_layer1_data_table_sfc + 0 + 3 * level_num) << 0);

    u32 level_layer2_data_addr_sfc
        = (wdc65816_mapper_read(&cpu->read_mapper, level_layer2_data_table_sfc + 2 + 3 * level_num) << 16)
        | (wdc65816_mapper_read(&cpu->read_mapper, level_layer2_data_table_sfc + 1 + 3 * level_num) << 8)
        | (wdc65816_mapper_read(&cpu->read_mapper, level_layer2_data_table_sfc + 0 + 3 * level_num) << 0);

    u32 level_sprite_data_addr_sfc
        = 0x070000
        | (wdc65816_mapper_read(&cpu->read_mapper, level_sprite_data_table_sfc + 1 + 2 * level_num) << 8)
        | (wdc65816_mapper_read(&cpu->read_mapper, level_sprite_data_table_sfc + 0 + 2 * level_num) << 0);


    //u32 level_layer1_data_addr_pc = ((level_layer1_data_addr_sfc & 0x7f0000) >> 1) | (level_layer1_data_addr_sfc & 0x7fff);

    
    cpu->ram[0x10c] = level_num / 256;
    cpu->ram[0x10b] = level_num & 0xFF;
    
    cpu->ram[0x65] = (level_layer1_data_addr_sfc & 0x0000FF) >> 0;
    cpu->ram[0x66] = (level_layer1_data_addr_sfc & 0x00FF00) >> 8;
    cpu->ram[0x67] = (level_layer1_data_addr_sfc & 0xFF0000) >> 16;

    cpu->ram[0x68] = (level_layer2_data_addr_sfc & 0x0000FF) >> 0;
    cpu->ram[0x69] = (level_layer2_data_addr_sfc & 0x00FF00) >> 8;
    cpu->ram[0x6A] = (level_layer2_data_addr_sfc & 0xFF0000) >> 16;

    //TODO: memset
    for(int i = 0; i < 0x3800; i++) {
        cpu->ram[level_map16_low_ram + i] = 0x25;
        cpu->ram[level_map16_high_ram + i] = 0x25;
    }

    wdc65816_cpu_add_exec_bp(cpu, level_load_routine_end_sfc);
    wdc65816_cpu_run_from(cpu, level_load_routine_sfc);
    wdc65816_cpu_clear_exec_bp(cpu);

    // --- header init ------
    {
        wdc65816_mapper_read_range(rom, level_layer1_data_addr_sfc,
                                   level_layer1_data_addr_sfc + 5, (u8*)&l->header);
        l->is_vertical_level = cpu->ram[0x5B] & 0x01;
        switch(l->header.level_mode) {
        case 0x00: case 0x0C: case 0x0E: case 0x11:
        case 0x1E: case 0x0A: case 0x0D:
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
            break;
        default:
            l->has_layer2_objects = 1;
            l->has_layer2_bg = 0;
            l->is_boss_level = 0;
            break;
        }
    }


    {   // --- background color ----
        int bg_color_addr = level_bg_area_color_data_table_sfc + 2 * cpu->ram[level_header_bg_color_ram];
        u16 bg_color = wdc65816_mapper_read(&cpu->read_mapper, bg_color_addr) |
            (wdc65816_mapper_read(&cpu->read_mapper, bg_color_addr + 1) << 8);
        int r = ((bg_color & 0x001F) << 3); r |= (r >> 5);
        int g = ((bg_color & 0x03E0) >> 2); g |= (g >> 5);
        int b = ((bg_color & 0x7C00) >> 7); b |= (b >> 5);
        l->background_color = 0xFF000000 + (r) + (g << 8) + (b << 16);
    }
    

    {   // --- tileset init -----
        int num_tileset = cpu->ram[level_header_tileset_ram];
        int tileset_addr = 0xA92B + num_tileset * 4;
        u8 fg1 = wdc65816_mapper_read(&cpu->read_mapper, tileset_addr + 0);
        u8 fg2 = wdc65816_mapper_read(&cpu->read_mapper, tileset_addr + 1);
        u8 bg1 = wdc65816_mapper_read(&cpu->read_mapper, tileset_addr + 2);
        u8 fg3 = wdc65816_mapper_read(&cpu->read_mapper, tileset_addr + 3);
        if(fg1 >= gfx_pages.num_pages) return;
        if(fg2 >= gfx_pages.num_pages) return;
        if(bg1 >= gfx_pages.num_pages) return;
        if(fg3 >= gfx_pages.num_pages) return;

        l->tileset.fg1 = &gfx_pages.pages[fg1];
        l->tileset.fg2 = &gfx_pages.pages[fg2];
        l->tileset.bg1 = &gfx_pages.pages[bg1];
        l->tileset.fg3 = &gfx_pages.pages[fg3];
    
        l->map8.length = 512;
        l->map8.tiles = arena_alloc_array(arena, 512, Tile8);
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

#ifdef SPRITES

    {   // --- sprite map8
        int num_tileset = cpu->ram[level_header_sprite_tileset_ram];
        /* c_print_format("num_tileset: %i\n", num_tileset); */
        int tileset_addr = 0x00A8C3 + num_tileset * 4; //TODO: No magic numbers
        u8 fg1 = wdc65816_mapper_read(&cpu->read_mapper, tileset_addr + 0);
        u8 fg2 = wdc65816_mapper_read(&cpu->read_mapper, tileset_addr + 1);
        u8 bg1 = wdc65816_mapper_read(&cpu->read_mapper, tileset_addr + 2);
        u8 fg3 = wdc65816_mapper_read(&cpu->read_mapper, tileset_addr + 3);

        if(fg1 >= gfx_pages.num_pages) return;
        if(fg2 >= gfx_pages.num_pages) return;
        if(bg1 >= gfx_pages.num_pages) return;
        if(fg3 >= gfx_pages.num_pages) return;

        l->sprite_tileset.fg1 = &gfx_pages.pages[fg1];
        l->sprite_tileset.fg2 = &gfx_pages.pages[fg2];
        l->sprite_tileset.bg1 = &gfx_pages.pages[bg1];
        l->sprite_tileset.fg3 = &gfx_pages.pages[fg3];
    
        l->sprite_map8.length = 512;
        l->sprite_map8.tiles = arena_alloc_array(arena, 512, Tile8);
        for(int tile = 0; tile < 128; tile++) {
            l->sprite_map8.tiles[tile] = tile8_from_3bpp(l->sprite_tileset.fg1->data + 24 * (tile % 128));
        }
        for(int tile = 128; tile < 256; tile++) {
            l->sprite_map8.tiles[tile] = tile8_from_3bpp(l->sprite_tileset.fg2->data + 24 * (tile % 128));
        }
        for(int tile = 256; tile < 384; tile++) {
            l->sprite_map8.tiles[tile] = tile8_from_3bpp(l->sprite_tileset.bg1->data + 24 * (tile % 128));
        }
        for(int tile = 384; tile < 512; tile++) {
            l->sprite_map8.tiles[tile] = tile8_from_3bpp(l->sprite_tileset.fg3->data + 24 * (tile % 128));
        }
    }
#endif


    {   // --- map16 fg init ----
        Map16 map16_fg = {
            .length = 512,
            .tiles = arena_alloc_array(arena, 512, Tile16)
        };
        for(int i = 0; i < map16_fg.length; i++) {
            int map16_lookup_sfc = map16_fg_tiles_zero_offset_sfc + cpu->ram[2 * i + 0x0FBF] * 256 + cpu->ram[2 * i + 0x0FBE];
            for(int j = 0; j < 4; j++) {
                map16_fg.tiles[i].properties[j] =
                    (TileProperties)wdc65816_mapper_read(&cpu->read_mapper,
                                                         map16_lookup_sfc + j * 2 + 1);
                u16 num_tile =  wdc65816_mapper_read(&cpu->read_mapper,
                                                     map16_lookup_sfc + j * 2);
                num_tile    |= (wdc65816_mapper_read(&cpu->read_mapper,
                                                     map16_lookup_sfc + j * 2 + 1) & 0x03) << 8;
                if(num_tile == 0x3FF) num_tile = 0;
                map16_fg.tiles[i].tile8s[j] = &(l->map8.tiles[num_tile]);
            }
        }
        map16_pc_init(&l->map16_fg, &map16_fg, &smw->free_list);
    }
    


    {   // --- map16 bg init ----
        Map16 map16_bg = {
            .length = 512,
            .tiles = arena_alloc_array(arena, 512, Tile16)
        };
        for(int i = 0; i < map16_bg.length; i++) {
            int map16_addr_sfc = map16_bg_tiles_table_sfc + i*8;
            for(int j = 0; j < 4; j++) {
                map16_bg.tiles[i].properties[j] =
                    (TileProperties)(wdc65816_mapper_read(&cpu->read_mapper,
                                                          map16_addr_sfc + j * 2 + 1));
                u16 num_tile =  wdc65816_mapper_read(&cpu->read_mapper, map16_addr_sfc + j * 2);
                num_tile    |= (wdc65816_mapper_read(&cpu->read_mapper,
                                                     map16_addr_sfc + j * 2 + 1) & 0x03) << 8;
                if(num_tile >= 0x200) num_tile = 0;
                map16_bg.tiles[i].tile8s[j] = &(l->map8.tiles[num_tile]);
            }
        }
        map16_pc_init(&l->map16_bg, &map16_bg, &smw->free_list);
    }
    
    if(!l->is_boss_level) {
        if(l->is_vertical_level) {
            l->width = 32;
            l->height = (l->header.screens + 1) * 16;
        } else {
            l->width = (l->header.screens + 1) * 16;
            l->height = 27;
        }
        


        {   // --- layer1 object init ------
            /* wdc65816_cpu_add_exec_bp(cpu, level_load_objects_routine_end_sfc); */
            wdc65816_cpu_add_exec_bp(cpu, 0x0583D2);
            wdc65816_cpu_add_exec_bp(cpu, level_load_objects_routine_new_object_sfc);
            wdc65816_cpu_add_write_bp_range(cpu, level_objects_low_start_sfc, level_objects_low_end_sfc);
            wdc65816_cpu_add_write_bp_range(cpu, level_objects_high_start_sfc, level_objects_high_end_sfc);
    
            int xmax = 0, ymax = 0, xmin = 65535, ymin = 65535;
            l->layer1_objects.objects = arena_alloc_array(arena, 0, ObjectPC);
            int objects_length = 0;
            Temp temp = temp_begin(temp_arena);
            int tiles_length = 0;
            ObjectTile* tiles  = arena_alloc_array(temp_arena, 0, ObjectTile);
            
            ObjectPC*   object = NULL;
            ObjectTile* tile   = NULL;
            wdc65816_cpu_run(cpu); // start level loading routine
            while(cpu->regs.pc.d != 0x0583D2) {
                if(cpu->regs.pc.d == level_load_objects_routine_new_object_sfc) {
                    // Old object
                    if(objects_length > 0 && tiles_length > 0) {
                        object->bb_xmin = xmin;
                        object->bb_ymin = ymin;
                        object->bb_xmax = xmax;
                        object->bb_ymax = ymax;
                        object->object_tiles        = tiles;
                        object->object_tiles_length = tiles_length;
                                                
                        tiles = arena_alloc_array(temp_arena, 0, ObjectTile);
                        tiles_length = 0;
                    }

                    // New object
                    u8 num = cpu->ram[0x5A];
                    u8 settings = cpu->ram[0x59];
                    if(num != 0 || (settings != 0 && settings != 1)) {
                        object = arena_alloc_type(arena, ObjectPC);
                        *object = (ObjectPC){ 0 };
                        // TODO: which one is correct?
                        //object->x = (cpu->ram[level_current_screen_ram] << 4) | (cpu->ram[0x57] & 0x0F);
                        //object->y = ((cpu->ram[0x57] & 0xF0) >> 4) | (cpu->ram[0x0A] << 4);
                        object->x = (cpu->ram[0x0B] & 0x0F)  | (cpu->ram[0x1928] << 4);
                        object->y = cpu->ram[0x0A] & 0x1F;
                        object->num = num;
                        object->settings = settings;
                        object->tiles = NULL;

                        xmax = 0; ymax = 0; xmin = 65535; ymin = 65535;
                    
                        objects_length++;
                    }
                } else { // a new tile was written to the level data RAM location
                    //record access to this ram location
                    tile = arena_alloc_type(temp.arena, ObjectTile);
                    tiles_length++;
                    int pure_addr = (cpu->breakpoint_address & 0xFFFF) - 0xC800;
                    if(l->is_vertical_level) {
                        int x = pure_addr & 0x0F; int y = (pure_addr & 0xF0) >> 4;
                        int screen = pure_addr >> 4;  int left = screen & 0x10;
                        int h = (screen & 0x3E0) >> 1;
                        tile->x = left + x;
                        tile->y = h + y;
                    } else {
                        int screen_addr = pure_addr % 432;
                        int screen = pure_addr / 432;
                        tile->x = (screen_addr & 0x0F) | (screen * 16);
                        tile->y = (screen_addr & 0xFF0) >> 4;
                    }
                    if(tile->x < xmin) xmin = tile->x;
                    if(tile->x > xmax) xmax = tile->x;
                    if(tile->y < ymin) ymin = tile->y;
                    if(tile->y > ymax) ymax = tile->y;
                    tile->is_top = cpu->breakpoint_address >= level_objects_high_start_sfc;
                    tile->tile = tile->is_top
                        ? (cpu->breakpoint_data << 8)
                        : cpu->breakpoint_data;
                }
                cpu->debug = 0;
                wdc65816_cpu_run(cpu);
                cpu->debug = 0;
            }
            // Finalize the last object
            if(tiles_length > 0 && objects_length > 0) {
                object->bb_xmin = xmin;
                object->bb_ymin = ymin;
                object->bb_xmax = xmax;
                object->bb_ymax = ymax;
                object->object_tiles        = tiles;
                object->object_tiles_length = tiles_length;
            }

            l->layer1_objects.length  = objects_length;
            
            for(int i = 0; i < objects_length; i++) {
                ObjectPC* object = l->layer1_objects.objects + i;
                int width  = object->bb_xmax - object->bb_xmin + 1;
                int height = object->bb_ymax - object->bb_ymin + 1;
                ObjectTile* object_tiles = object->object_tiles;
                int object_tiles_length = object->object_tiles_length;
                object->tiles = arena_alloc_array(arena, width * height, u16);
                for(int j = 0; j < width * height; j++) object->tiles[j] = 0xFFFF;
                for(int j = 0; j < object_tiles_length; j++) {
                    ObjectTile* object_tile = object_tiles + j;
                    u16 x = object_tile->x - object->bb_xmin;
                    u16 y = object_tile->y - object->bb_ymin;
                    u16 tilenum = object_tile->tile;
                    u16 is_top = object_tile->is_top;
                    int index = y * width + x;
                    if(object->tiles[index] == 0xFFFF) object->tiles[index] = 0x0025;
                    if(is_top) {
                        object->tiles[index] = (object->tiles[index] & 0x00FF) | tilenum;
                    } else {
                        object->tiles[index] = (object->tiles[index] & 0xFF00) | tilenum;
                    }
                }

            }
            
            wdc65816_cpu_clear_write_bp(cpu);
            wdc65816_cpu_clear_exec_bp(cpu);            
            temp_end(temp);
            
	    
        }

        // --- bg init ----------

        if(l->has_layer2_bg) {
            l->layer2_background.data = arena_alloc_array(arena, 432 * 2, u16);
            int offset = 0;
            int addr = wdc65816_mapper_read(&cpu->read_mapper, level_layer2_data_table_sfc + 2 + 3 * level_num);
            if(addr == 0xFF) addr = 0x0C;
            addr = (addr << 8) | wdc65816_mapper_read(&cpu->read_mapper, level_layer2_data_table_sfc + 1 + 3 * level_num);
            addr = (addr << 8) | wdc65816_mapper_read(&cpu->read_mapper, level_layer2_data_table_sfc + 0 + 3 * level_num);
            if((addr & 0xFFFF) >= 0xE8FE) offset = 0x100; //See CODE_058046
            int pos = 0;
            u8 cmd, byte;
            int length = l->is_vertical_level ? 432 * 2 : 512 * 2;
            while(pos < length) {
                cmd  = wdc65816_cpu_read(cpu, addr);
                byte = wdc65816_cpu_read(cpu, addr + 1);
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
                        l->layer2_background.data[pos + i] = offset + wdc65816_cpu_read(cpu, addr + i + 1);
                    }
                    pos += length + 1;
                    addr += length + 2;
                }
            }
        } else if(l->has_layer2_objects) {
#if 0
            l->layer2_objects.objects = l->layer2_objects.objects;
            l->layer2_objects.length = l->layer2_objects.length;
#else
            wdc65816_cpu_add_exec_bp(cpu, level_load_objects_routine_end_sfc);
            wdc65816_cpu_add_exec_bp(cpu, level_load_objects_routine_end2_sfc);
            wdc65816_cpu_add_exec_bp(cpu, level_load_objects_routine_new_object_sfc);
            wdc65816_cpu_add_write_bp_range(cpu, level_objects_low_start_sfc, level_objects_low_end_sfc);
            wdc65816_cpu_add_write_bp_range(cpu, level_objects_high_start_sfc, level_objects_high_end_sfc);
    
            int xmax = 0, ymax = 0, xmin = 65535, ymin = 65535;
            l->layer2_objects.objects = arena_alloc_array(arena, 0, ObjectPC);
            int objects_length = 0;
            Temp temp = temp_begin(temp_arena);
            int tiles_length = 0;
            ObjectTile* tiles  = arena_alloc_array(temp_arena, 0, ObjectTile);
            
            ObjectPC*   object = NULL;
            ObjectTile* tile   = NULL;
            wdc65816_cpu_run(cpu); // start level loading routine
            while((cpu->regs.pc.d != level_load_objects_routine_end_sfc) &&
                  (cpu->regs.pc.d != level_load_objects_routine_end2_sfc)) {
                if(cpu->regs.pc.d == level_load_objects_routine_new_object_sfc) {
                    // Old object
                    if(objects_length > 0 && tiles_length > 0) {
                        object->bb_xmin = xmin;
                        object->bb_ymin = ymin;
                        object->bb_xmax = xmax;
                        object->bb_ymax = ymax;
                        object->object_tiles        = tiles;
                        object->object_tiles_length = tiles_length;
                                                
                        tiles = arena_alloc_array(temp_arena, 0, ObjectTile);
                        tiles_length = 0;
                    }

                    // New object
                    u8 num = cpu->ram[0x5A];
                    u8 settings = cpu->ram[0x59];
                    if(num != 0 || (settings != 0 && settings != 1)) {
                        object = arena_alloc_type(arena, ObjectPC);
                        *object = (ObjectPC){ 0 };
                        // TODO: which one is correct?
                        object->x = (cpu->ram[level_current_screen_ram] << 4) | (cpu->ram[0x57] & 0x0F);
                        object->y = ((cpu->ram[0x57] & 0xF0) >> 4) | (cpu->ram[0x0A] << 4);
                        //object->x = (cpu->ram[0x0B] & 0x0F)  | (cpu->ram[0x1928] << 4);
                        //object->y = cpu->ram[0x0A] & 0x1F;
                        object->num = num;
                        object->settings = settings;
                        object->tiles = NULL;

                        xmax = 0; ymax = 0; xmin = 65535; ymin = 65535;
                    
                        objects_length++;
                    }
                } else { // a new tile was written to the level data RAM location
                    //record access to this ram location
                    tile = arena_alloc_type(temp.arena, ObjectTile);
                    tiles_length++;
                    if(l->is_vertical_level) {
                        // TODO: Clean up
                        int pure_addr = (cpu->breakpoint_address & 0xFFFF) - 0xC800 - 0x0E * 32 * 16;
                        int x = pure_addr & 0x0F; int y = (pure_addr & 0xF0) >> 4;
                        int screen = pure_addr >> 5;  int left = screen & 0x10;
                        int h = (screen & 0x3E0) >> 1;
                        tile->x = left + x;
                        tile->y = h + y;
                    } else {
                        // TODO: Clean up
                        int pure_addr = (cpu->breakpoint_address & 0xFFFF) - 0xC800 - 0x10 * 27 * 16;
                        int screen_addr = pure_addr % 432;
                        int screen = pure_addr / 432;
                        tile->x = (screen_addr & 0x0F) | (screen * 16);
                        tile->y = (screen_addr & 0xFF0) >> 4;
                    }
                    if(tile->x < xmin) xmin = tile->x;
                    if(tile->x > xmax) xmax = tile->x;
                    if(tile->y < ymin) ymin = tile->y;
                    if(tile->y > ymax) ymax = tile->y;
                    tile->is_top = cpu->breakpoint_address >= level_objects_high_start_sfc;
                    tile->tile = tile->is_top
                        ? (cpu->breakpoint_data << 8)
                        : cpu->breakpoint_data;
                }
                wdc65816_cpu_run(cpu);
            }
            // Finalize the last object
            if(tiles_length > 0 && objects_length > 0) {
                object->bb_xmin = xmin;
                object->bb_ymin = ymin;
                object->bb_xmax = xmax;
                object->bb_ymax = ymax;
                object->object_tiles        = tiles;
                object->object_tiles_length = tiles_length;
            }

            l->layer2_objects.length  = objects_length;
            
            for(int i = 0; i < objects_length; i++) {
                ObjectPC* object = l->layer2_objects.objects + i;
                int width  = object->bb_xmax - object->bb_xmin + 1;
                int height = object->bb_ymax - object->bb_ymin + 1;
                ObjectTile* object_tiles = object->object_tiles;
                int object_tiles_length = object->object_tiles_length;
                object->tiles = arena_alloc_array(arena, width * height, u16);
                for(int j = 0; j < width * height; j++) object->tiles[j] = 0xFFFF;
                for(int j = 0; j < object_tiles_length; j++) {
                    ObjectTile* object_tile = object_tiles + j;
                    u16 x = object_tile->x - object->bb_xmin;
                    u16 y = object_tile->y - object->bb_ymin;
                    u16 tilenum = object_tile->tile;
                    u16 is_top = object_tile->is_top;
                    int index = y * width + x;
                    if(object->tiles[index] == 0xFFFF) object->tiles[index] = 0x0025;
                    if(is_top) {
                        object->tiles[index] = (object->tiles[index] & 0x00FF) | tilenum;
                    } else {
                        object->tiles[index] = (object->tiles[index] & 0xFF00) | tilenum;
                    }
                }

            }
            
            wdc65816_cpu_clear_write_bp(cpu);
            wdc65816_cpu_clear_exec_bp(cpu);            
            temp_end(temp);
#endif
        }

        
#ifdef SPRITES

        
        // --- sprites init -----
        {
            
            Temp temp = temp_begin(temp_arena);
            
            wdc65816_cpu_add_exec_bp(cpu, 0x2A751);            
            wdc65816_cpu_run(cpu);
            wdc65816_cpu_clear_exec_bp(cpu);
            u8* ram_copy = arena_alloc_array(temp_arena, 0x20000, u8);

            //TODO: memcpy
            for(int i = 0; i < 0x20000; i++) {
                ram_copy[i] = cpu->ram[i];
            }
            
            l->sprites.data = arena_alloc_array(arena, 0, SpritePC);
            int sprites_length = 0;
            
            while(wdc65816_mapper_read(&cpu->read_mapper, level_sprite_data_addr_sfc + 3*sprites_length + 1) != 0xFF) {

                //TODO: memcpy
                for(int i = 0; i < 0x20000; i++) {
                    cpu->ram[i] = ram_copy[i];
                }
                SpritePC* sprite = arena_alloc_type(arena, SpritePC);

                u8 sprite_data_byte1 = wdc65816_mapper_read(&cpu->read_mapper, level_sprite_data_addr_sfc + 3*sprites_length + 1);
                u8 sprite_data_byte2 = wdc65816_mapper_read(&cpu->read_mapper, level_sprite_data_addr_sfc + 3*sprites_length + 2);
                u8 sprite_data_byte3 = wdc65816_mapper_read(&cpu->read_mapper, level_sprite_data_addr_sfc + 3*sprites_length + 3);
                int screen = (sprite_data_byte1 & 0x2) << 3;
                screen |= sprite_data_byte2 & 0xF;

                if(l->is_vertical_level) {
                    //TODO: Check documentation!
                    sprite->y = (screen << 4) | ((sprite_data_byte2 & 0xF0) >> 4);
                    sprite->x = sprite_data_byte1 & 0xF1;
                    sprite->x = ((sprite->y & 0xF0) >> 4) | ((sprite->y & 0x01) << 4);
                } else {
                    sprite->x = (screen << 4) | ((sprite_data_byte2 & 0xF0) >> 4);
                    sprite->y = sprite_data_byte1 & 0xF1;
                    sprite->y = ((sprite->y & 0xF0) >> 4) | ((sprite->y & 0x01) << 4);
                }
                sprite->number = sprite_data_byte3;
                sprite->extra_bits = (sprite_data_byte1 & 0x0C) >> 2;
                sprite->tiles = arena_alloc_array(temp_arena, 0, SpriteTile);
                
                // Clear sprite tables
                SpriteTableEntries table_entries = { 0 };
                for(int i = 0; i < 12; i++) {
                    smw_set_sprite_table_entries(cpu->ram, table_entries, i);
                }
                // Camera
                cpu->ram[0x1a] = ((sprite->x - 8) << 4) & 0xFF;
                cpu->ram[0x1b] = ((sprite->x - 8) >> 4) & 0xFF;
                cpu->ram[0x1c] = ((sprite->y - 4) << 4) & 0xFF;
                cpu->ram[0x1d] = ((sprite->y - 4) >> 4) & 0xFF;

                // Mario
                cpu->ram[0xD1] = cpu->ram[0x94] = ((sprite->x - 4) << 4) & 0xFF; //0x00; // player x position
                cpu->ram[0xD2] = cpu->ram[0x95] = ((sprite->x - 4) >> 4) & 0xFF; //0x00;
                cpu->ram[0xD3] = cpu->ram[0x96] = ((sprite->y - 4) << 4) & 0xFF; //0x40; // player y position
                cpu->ram[0xD4] = cpu->ram[0x97] = ((sprite->y - 4) >> 4) & 0xFF; //0x01;
                /* cpu->ram[0xD1] = cpu->ram[0x94] = 0x00; // player x position */
                /* cpu->ram[0xD2] = cpu->ram[0x95] = 0x00; */
                /* cpu->ram[0xD3] = cpu->ram[0x96] = 0x40; // player y position */
                /* cpu->ram[0xD4] = cpu->ram[0x97] = 0x01; */
                cpu->ram[0x15EA] = 0x30;

                // set frame counter to something fancy.
                cpu->ram[0x14] = 0x00; 
                cpu->ram[0x13] = 0x00;
            
                //Create a new sprite list in scratch ram with one sprite in it.
                cpu->ram[0xCE] = 0x7b;
                cpu->ram[0xCF] = 0x9c;
                cpu->ram[0xD0] = 0x7f;

                
                //Copy sprite to scratch ram
                cpu->ram[0x19c7b] = wdc65816_mapper_read(&cpu->read_mapper, level_sprite_data_addr_sfc);
                cpu->ram[0x19c7c] = sprite_data_byte1;
                cpu->ram[0x19c7d] = sprite_data_byte2;
                cpu->ram[0x19c7e] = sprite_data_byte3;
                cpu->ram[0x19c7f] = 0xff;
                cpu->ram[0x19c80] = 0xff; //
                cpu->ram[0x19c81] = 0xff; // 
                cpu->ram[0x19c82] = 0xff; // SMW somehow loads more of the level in case
                cpu->ram[0x19c83] = 0xff; // of sprite C9.
                cpu->ram[0x19c84] = 0xff; // 
                cpu->ram[0x19c85] = 0xff; // 
                cpu->ram[0x19c86] = 0xff; // 

                //Sprite buoyancy; (level 30 needs this)
                cpu->ram[0x190E] = cpu->ram[0x19c7b] & 0xC0;
                cpu->regs.p.b = 0x30;

                int sprite_index = 0;

                wdc65816_cpu_run_jsl(cpu, 0x02A751);

                // Find sprite index and set it to register x and ram value 7E15E9.
                for(int i = 0x0; i < 12; i++) {
                    if(cpu->ram[i + 0x9E] != 0) {
                        cpu->ram[0x15E9] = cpu->regs.x.w = sprite_index = i;
                    }
                }
                wdc65816_cpu_clear_exec_bp(cpu);
                
                sprite->table_entries = smw_get_sprite_table_entries(cpu->ram, sprite_index);

                // Sprites are facing left. NOTE: Maybe add special cases for different sprites.
                cpu->ram[0x157C + sprite_index] = 0x1;
                // Rex needs this.
                cpu->ram[0x1FE2 + sprite_index] = 0x0;
                
                cpu->regs.db = 1;
                wdc65816_cpu_run_jsr(cpu, 0x018127);
                wdc65816_cpu_run_jsr(cpu, 0x018127);
                //wdc65816_cpu_run_jsr(cpu, 0x0185C3);
                
                cpu->regs.db = 0;
                int number_tiles = 0;

                for(int i = 0; i < 0x200; i += 4) {
                    if((cpu->ram[i + 0x200] | cpu->ram[i + 0x202] | cpu->ram[i + 0x203]) && (cpu->ram[i + 0x201] != 0xf0)) {
                        u8 extrabits = cpu->ram[0x420 + i/4];
                        //NOTE: Does not take $2101 into account
                        if(extrabits & 0x02) {
                            SpriteTile* tiles = arena_alloc_array(temp_arena, 4, SpriteTile);
                            u8 flipx = cpu->ram[i + 0x203] & 0x40;
                            u8 flipy = cpu->ram[i + 0x203] & 0x80;

                            for(int j = 0; j < 4; j++) {
                                tiles[j].flip_x = (cpu->ram[i + 0x203] & 0x40) && 1;
                                tiles[j].flip_y = (cpu->ram[i + 0x203] & 0x80) && 1;
                                tiles[j].palette = 8 + ((cpu->ram[i + 0x203] >> 1) & 0x7);
                                tiles[j].x = ((cpu->ram[0x1b] << 8) | cpu->ram[0x1a]) + cpu->ram[i + 0x200];
                                tiles[j].y = ((cpu->ram[0x1d] << 8) | cpu->ram[0x1c]) + cpu->ram[i + 0x201];
                            }
                            tiles[1].x += 8;
                            tiles[2].y += 8;
                            tiles[3].x += 8;
                            tiles[3].y += 8;

                            int number = ((cpu->ram[i + 0x203] << 8) |  cpu->ram[i + 0x202]) & 0x1FF;

                            if(flipx) {
                                if(flipy) {
                                    tiles[0].tile_num = (number + 17) & 0x1FF;
                                    tiles[1].tile_num = (number + 16) & 0x1FF;
                                    tiles[2].tile_num = (number +  1) & 0x1FF;
                                    tiles[3].tile_num = (number +  0) & 0x1FF;
                                } else {
                                    tiles[0].tile_num = (number +  1) & 0x1FF;
                                    tiles[1].tile_num = (number +  0) & 0x1FF;
                                    tiles[2].tile_num = (number + 17) & 0x1FF;
                                    tiles[3].tile_num = (number + 16) & 0x1FF;
                                }
                            } else {
                                if(flipy) {
                                    tiles[0].tile_num = (number + 16) & 0x1FF;
                                    tiles[1].tile_num = (number + 17) & 0x1FF;
                                    tiles[2].tile_num = (number +  0) & 0x1FF;
                                    tiles[3].tile_num = (number +  1) & 0x1FF;
                                } else {
                                    tiles[0].tile_num = (number +  0) & 0x1FF;
                                    tiles[1].tile_num = (number +  1) & 0x1FF;
                                    tiles[2].tile_num = (number + 16) & 0x1FF;
                                    tiles[3].tile_num = (number + 17) & 0x1FF;
                                }
                            }
                            number_tiles += 4;
                        } else {
                            SpriteTile* tile = arena_alloc_array(temp_arena, 1, SpriteTile);
                            tile->flip_x = (cpu->ram[i + 0x203] & 0x40) && 1;
                            tile->flip_y = (cpu->ram[i + 0x203] & 0x80) && 1;
                            tile->palette = 8 + ((cpu->ram[i + 0x203] >> 1) & 0x7);
                            tile->tile_num = ((cpu->ram[i + 0x203] << 8) |  cpu->ram[i + 0x202]) & 0x1FF;
                            tile->x = ((cpu->ram[0x1b] << 8) | cpu->ram[0x1a]) + cpu->ram[i + 0x200];
                            tile->y = ((cpu->ram[0x1d] << 8) | cpu->ram[0x1c]) + cpu->ram[i + 0x201];
                            number_tiles++;
                        }
                    }
                }

                sprite->num_tiles = number_tiles;
                sprites_length++;
            }
            for(int i = 0; i < sprites_length; i++) {
                // Move memory from temp_arena to arena
                SpritePC* sprite = l->sprites.data + i;
                SpriteTile* temp_tiles = sprite->tiles;
                sprite->tiles = arena_alloc_array(arena, sprite->num_tiles, SpriteTile);
                //assert(sprite->tiles);
                for(int i = 0; i < sprite->num_tiles; i++) {
                    sprite->tiles[i] = temp_tiles[i];
                }
            }
            
            wdc65816_cpu_clear_exec_bp(cpu);            
            temp_end(temp);
            l->sprites.length = sprites_length;
        }
#endif    
    }
    
    {
        // --- palette init -----
        cpu->regs.p.b = 0x24;
        wdc65816_cpu_add_exec_bp(cpu, 0x00ACEC);
        wdc65816_cpu_run_from(cpu, 0x00ABED);
        u8* palette_data_ptr = (u8*)l->palette.data;
        for(int i = 0; i < 512; i++) {
            palette_data_ptr[i] = cpu->ram[0x0703 + i];
        }
        
        for(int i = 0; i < 16; i += 16) {
            l->palette.data[i] = 0;
        }
    
        for(int i = 0; i < 8; i++) {
            smw_level_animate(smw, level_num, i);
        }
    }
        
    //wdc65816_cpu_free(cpu);
}

void smw_level_animate(SMW* smw, u16 level_num, u8 frame) {

    Wdc65816Mapper* rom = &smw->cpu.read_mapper;
    GFXStore gfx_pages = smw->gfx_pages;
    LevelPC* l = smw->levels[level_num];

    if(l->is_boss_level) return;
    
    Wdc65816Cpu* cpu = &smw->cpu;
    wdc65816_cpu_clear(cpu);

    cpu->ram[0x14] = frame;
    wdc65816_cpu_add_exec_bp(cpu, 0x00A42F);
    wdc65816_cpu_run_from(cpu, 0x00A418);
    int pos = cpu->sreg[0x0121];
    u32 tmp_color = cpu->sreg[0x0122];
    wdc65816_cpu_step(cpu);
    wdc65816_cpu_step(cpu);
    l->palette.data[pos] = tmp_color | (cpu->sreg[0x0122] << 8);

    wdc65816_cpu_clear(cpu);
    cpu->ram[0x65] = wdc65816_mapper_read(rom, 0x05E000 + 3 * level_num);
    cpu->ram[0x66] = wdc65816_mapper_read(rom, 0x05E001 + 3 * level_num);
    cpu->ram[0x67] = wdc65816_mapper_read(rom, 0x05E002 + 3 * level_num);
    wdc65816_cpu_add_exec_bp(cpu, 0x0583B8);
    wdc65816_cpu_run_from(cpu, 0x0583AC);
    cpu->ram[0x0014] = frame;
    cpu->ram[0x14AD] = 0;
    cpu->ram[0x14AE] = 0;
    cpu->ram[0x14AF] = 0;
    cpu->regs.p.b = 0;
    cpu->regs.p.x = 1;
    cpu->regs.p.m = 1;
    //cpu->regs.d = 0x3000;

    wdc65816_cpu_add_exec_bp(cpu, 0x00A601);
    wdc65816_cpu_run_from(cpu, 0x00A5FD);

    u16 source[3], dest[3];
    // Get the dma source addresses. It works... note that 1000 is decimal.
    source[0] = ((cpu->ram[0x0D77] << 8) + cpu->ram[0x0D76]) / 32 - 1000; 
    source[1] = ((cpu->ram[0x0D79] << 8) + cpu->ram[0x0D78]) / 32 - 1000;
    source[2] = ((cpu->ram[0x0D7B] << 8) + cpu->ram[0x0D7A]) / 32 - 1000;

    dest[0] = ((cpu->ram[0x0D7D] << 8) | cpu->ram[0x0D7C]) / 16;
    dest[1] = ((cpu->ram[0x0D7F] << 8) | cpu->ram[0x0D7E]) / 16;
    dest[2] = ((cpu->ram[0x0D81] << 8) | cpu->ram[0x0D80]) / 16;
	       
    for(int i = 0; i < 3; i++) {
        if(dest[i] == 0 || ((source[i] & 0xFF00) == 0xFF00)) continue;
        if(dest[i] == 0x800) {                                              //See CODE_00A3DA
            l->map8.tiles[dest[i]     ] = tile8_from_3bpp(gfx_pages.pages[0x33].data + 24 * (source[i] + 0));
            l->map8.tiles[dest[i] +  1] = tile8_from_3bpp(gfx_pages.pages[0x33].data + 24 * (source[i] + 1));
            l->map8.tiles[dest[i] + 16] = tile8_from_3bpp(gfx_pages.pages[0x33].data + 24 * (source[i] + 2));
            l->map8.tiles[dest[i] + 17] = tile8_from_3bpp(gfx_pages.pages[0x33].data + 24 * (source[i] + 3));
        } else {
            for(int j = 0; j < 4; j++) {
                l->map8.tiles[dest[i] + j] = tile8_from_3bpp(gfx_pages.pages[0x33].data + 24 * (source[i] + j));
            }
        }
    }
}

Buffer smw_level_serialize_fast(SMW* smw, u16 level_num) {
    uint length = smw->levels[level_num]->layer1_objects.length;
    u8* level_data = arena_alloc(&smw->arena, 0, 0);
    LevelHeader* level_header = arena_alloc_type(&smw->arena, LevelHeader);
    *level_header = smw->levels[level_num]->header;
    int prev_screen = 0;
    for(uint i = 0; i < length; i++) {
        ObjectPC* object = &smw->levels[level_num]->layer1_objects.objects[i];
        /* c_print_format("Object: { x: %4i, y: %4i, num: %02x, settings: %02x}\n", */
        /*        object->x, object->y, object->num, object->settings); */
        assert(!(object->y & (0xFFE0))); 
        assert(!(object->num & (0xFFC0))); 

        u8* data;
        int screen = object->x / 16;
        data = arena_alloc_array(&smw->arena, 3, u8);
        if(prev_screen == screen) {
            data[0] =  (object->num >> 4) << 5;
        } else if(prev_screen + 1 == screen) {
            data[0] = ((object->num >> 4) << 5) | 0x80;
        } else { // We need to insert a screen jump
            data[0] = screen;
            data[1] = 0; // We can savely set this to zero. 
            data[2] = 1;
            data = arena_alloc_array(&smw->arena, 3, u8);
            data[0] = (object->num >> 4) << 5;
        }
        data[0] |= object->y;
        assert(data[0] != 0xFF);
        data[1] = (object->num << 4);
        data[1] |= object->x & 0x0F;
        data[2] = object->settings;
        prev_screen = screen;
    }
    
    *(u8*)arena_alloc_array(&smw->arena, 1, u8) = 0xFF;
    int level_data_length = arena_alloc_array(&smw->arena, 0, u8) - level_data;
    return buffer(level_data, level_data_length);
}

#if 0
    /* u8* data = pool.data; */
    /* u8 output, output2, output3; */
    /* uint screen = 0; */
    /* while((output = *data++) != 0xFF) { */
    /*     output2 = *data++; */
    /*     output3 = *data++; */
    /*     if(output & 0x80) { */
    /*         screen++; */
    /*     } */
    /*     u8 obj = ((output & 0x60) >> 1) | (output2 >> 4); */
    /*     u8 x = (output2 & 0x0F) + 16 * screen; */
    /*     u8 y = output & 0x1F; */
    /*     if(obj == 0) { */
    /*         if(output3 != 0) { */
    /*             if(output3 == 1) { */
    /*                 screen = output; */
    /*             } else { */
    /*                 c_print_format("(%2i,%2i) Extended object: %2x\n", x, y, output3); */
    /*             } */
    /*         } else { */
    /*             data++; */
    /*             //c_print_format("Screen Exit (%i)\n", output4); */
    /*         } */
    /*     } else { */
    /*         c_print_format("(%2i,%2i) Normal object: %2x\n", x, y, obj); */
    /*     } */
    /* } */
    /* c_print_format("\n"); */

    int level_data_length = arena_alloc(temp_arena, 0, 0) - level_data;

    u32 freespace_addr_pc = freespace_manager_request(&smw->freespace_manager,
                                                      level_data_length);
    int result = 0;
    if(freespace_addr_pc) {
        u8* rom_data = smw->rom->data;
        memcpy(rom_data + freespace_addr_pc, level_data, level_data_length);
        u32 freespace_addr_sfc = ((freespace_addr_pc << 1) & 0x7F0000) | (freespace_addr_pc & 0x7FFF) | 0x8000;
        rom_data[level_layer1_data_table_pc + 2 + 3 * level_num] = (freespace_addr_sfc & 0xFF0000) >> 16;
        rom_data[level_layer1_data_table_pc + 1 + 3 * level_num] = (freespace_addr_sfc & 0x00FF00) >>  8;
        rom_data[level_layer1_data_table_pc + 0 + 3 * level_num] = (freespace_addr_sfc & 0x0000FF) >>  0;
        result = 1;
    }
}
#endif

void smw_level_remove_layer1_object(SMW* smw, u16 level_num, uint object_index) {
    LevelPC* l = smw->levels[level_num];
    for(int i = object_index + 1; i < l->layer1_objects.length; i++) {
        l->layer1_objects.objects[i - 1] = l->layer1_objects.objects[i];
    }
    l->layer1_objects.length--;
}

void smw_level_add_layer1_object(SMW* smw, u16 level_num, uint object_index, ObjectPC object) {
    ObjectListPC* object_list = &smw->levels[level_num]->layer1_objects;
    ObjectPC* old_objects = object_list->objects;
    ObjectPC* new_objects = arena_alloc_array(&smw->arena, object_list->length + 1, ObjectPC);
    for(uint i = 0; i < object_list->length - object_index; i++) {
        new_objects[object_index + 1 + i] =  old_objects[i];
    }

    //free(old_objects);
    object_list->objects = new_objects;
    object_list->objects[object_index] = object;
    object_list->length++;
}

#if 0
void smw_level_refresh(SMW* smw, u16 level_num) {
    freespace_manager_empty(&smw->freespace_manager);
    if(smw_level_serialize_fast(smw, level_num)) {
        smw_level_load(smw, level_num);
    }
}
#endif
