typedef struct {
    String name;
    Buffer buffer;
} NamedBuffer;

u32 named_buffer_hash(NamedBuffer b) {
    return hash_murmur3_string(b.name);
}

u32 named_buffer_equal(NamedBuffer b1, NamedBuffer b2) {
    return string_equal(b1.name, b2.name);
}

generate_hashmap(NamedBufferMap, named_buffer_map, NamedBuffer,
                  named_buffer_hash, named_buffer_equal);

struct {
    Image palette[512];
    Image map8[512];
    Image sprite_map8[512];
    Image map16_fg[512];
    Image map16_bg[512];
    Image level[512];

} global_assets = { };

NamedBufferMap global_buffer_map;

Arena global_temp_arena = { };


void global_init(Arena* arena) {
    global_temp_arena = arena_subarena(arena, MB(256));
    
    for(int i = 0; i < 512; i++) {
        image_init(&global_assets.palette[i]);
        image_init(&global_assets.map8[i]);
        image_init(&global_assets.sprite_map8[i]);
        image_init(&global_assets.map16_fg[i]);
        image_init(&global_assets.map16_bg[i]);
        image_init(&global_assets.level[i]);
    }
}

void assets_clear() {
    arena_clear(&global_temp_arena);
    for(int i = 0; i < 512; i++) {
        global_assets.palette[i].data     = NULL;
        global_assets.map8[i].data        = NULL;
        global_assets.sprite_map8[i].data = NULL;
        global_assets.map16_fg[i].data    = NULL;
        global_assets.map16_bg[i].data    = NULL;
        global_assets.level[i].data       = NULL;
    }
}

void assets_load_palette(int level_num) {
    LevelPC*   level      = smw_get_level(&smw, level_num);
    Palette*   palette    = &level->palette;
    PalettePC* palette_pc = arena_alloc_type(&global_temp_arena, PalettePC);
    Image*     image      = &global_assets.palette[level_num];
    palette_to_pc(palette, palette_pc);
    image_update(image, 16, 16, palette_pc->data);
}

Image assets_get_palette(int level_num) {
    if(!global_assets.palette[level_num].data) {
        assets_load_palette(level_num);
    }
    return global_assets.palette[level_num];
}

NkImage assets_get_palette_nk(int level_num) {
    Image image = assets_get_palette(level_num);
    return nk_image_id(image.texture_id);
}


void assets_load_map8(int level_num) {
    LevelPC* level      = smw_get_level(&smw, level_num);
    Image    palette    = assets_get_palette(level_num);
    u32*     image_data = arena_alloc_array(&global_temp_arena, 128*256*16, u32);
    Image*   image      = &global_assets.map8[level_num];

    for(int p = 0; p < 16; p++) {
        for(int i = 0; i < 512; i++) {
            for(int j = 0; j < 64; j++) {
                image_data[p * 128 * 256 + (i % 16) * 8 + (j % 8) + ((i / 16) * 8 + (j / 8)) * 128]
                    = palette.data[16 * p + level->map8.tiles[i].pixels[j]];
            }
        }
    }
    image_update(image, 128, 256*16, image_data);
}

Image assets_get_map8(int level_num, int palette_num) {
    if(!global_assets.map8[level_num].data) {
        assets_load_map8(level_num);
    }
    Image image = {
        .size_x     = 128,
        .size_y     = 256,
        .data       = global_assets.map8[level_num].data + 128 * 256 * palette_num,
        .texture_id = global_assets.map8[level_num].texture_id,
    };
    return image;
}

NkImage assets_get_map8_nk(int level_num, int palette_num) {
    if(!global_assets.map8[level_num].data) {
        assets_load_map8(level_num);
    }
    GLuint texture_id = global_assets.map8[level_num].texture_id;
    NkRect rect       = nk_rect(0, 256 * palette_num, 128, 256);
    return nk_subimage_id(texture_id, 128, 256 * 16, rect);
}

void assets_load_sprite_map8(int level_num) {
    LevelPC* level      = smw_get_level(&smw, level_num);
    Image    palette    = assets_get_palette(level_num);
    u32*     image_data = arena_alloc_array(&global_temp_arena, 128*256*16, u32);
    Image*   image      = &global_assets.sprite_map8[level_num];

    for(int p = 0; p < 16; p++) {
        for(int i = 0; i < 512; i++) {
            for(int j = 0; j < 64; j++) {
                image_data[p * 128 * 256 + (i % 16) * 8 + (j % 8) + ((i / 16) * 8 + (j / 8)) * 128]
                    = palette.data[16 * p + level->sprite_map8.tiles[i].pixels[j]];
            }
        }
    }
    image_update(image, 128, 256*16, image_data);
}

Image assets_get_sprite_map8(int level_num, int palette_num) {
    if(!global_assets.sprite_map8[level_num].data) {
        assets_load_sprite_map8(level_num);
    }
    Image image = {
        .size_x     = 128,
        .size_y     = 256,
        .data       = global_assets.sprite_map8[level_num].data + 128 * 256 * palette_num,
        .texture_id = global_assets.sprite_map8[level_num].texture_id,
    };
    return image;
}

NkImage assets_get_sprite_map8_nk(int level_num, int palette_num) {
    if(!global_assets.map8[level_num].data) {
        assets_load_sprite_map8(level_num);
    }
    GLuint texture_id = global_assets.sprite_map8[level_num].texture_id;
    NkRect rect       = nk_rect(0, 256 * palette_num, 128, 256);
    return nk_subimage_id(texture_id, 128, 256 * 16, rect);
}


NkImage assets_get_sprite_map8_tile_nk(int level_num, int tile_num, int flip_x, int flip_y, int palette_num) {
    if(!global_assets.sprite_map8[level_num].data) {
        assets_load_sprite_map8(level_num);
    }
    GLuint texture_id = global_assets.sprite_map8[level_num].texture_id;
    int tile_x = tile_num & 0x00F;
    int tile_y = (tile_num & 0xFF0) >> 4;
    NkRect tile_rect;
    if(flip_x) {
        if(flip_y) {
            tile_rect = nk_rect(8 * tile_x + 8, 256 * palette_num + 8 * tile_y + 8, -8, -8);
        } else {
            tile_rect = nk_rect(8 * tile_x + 8, 256 * palette_num + 8 * tile_y + 0, -8,  8);
        }
    } else {
        if(flip_y) {
            tile_rect = nk_rect(8 * tile_x + 0, 256 * palette_num + 8 * tile_y + 8,  8, -8);
        } else {
            tile_rect = nk_rect(8 * tile_x + 0, 256 * palette_num + 8 * tile_y + 0,  8,  8);
        }
    }
    return nk_subimage_id(texture_id, 128, 256 * 16, tile_rect);
}



void assets_load_map16_fg(int level_num) {
    LevelPC* level      = smw_get_level(&smw, level_num);
    Image    palette    = assets_get_palette(level_num);
    Image*   image = &global_assets.map16_fg[level_num];
    u32*     image_data = arena_alloc_array(&global_temp_arena, 256*512, u32);
    for(int k = 0; k < 512; k++) {
        for(int j = 0; j < 256; j++) {
            image_data[(k % 16) * 16 + (j % 16) + ((k / 16) * 16 + (j / 16)) * 256]
                = palette.data[level->map16_fg.tiles[k].data[j]];
        }
    }
    image_update(image, 256, 512, image_data);
}

Image assets_get_map16_fg(int level_num) {
    if(!global_assets.map16_fg[level_num].data) {
        assets_load_map16_fg(level_num);
    }
    Image image = {
        .size_x     = 256,
        .size_y     = 512,
        .data       = global_assets.map16_fg[level_num].data,
        .texture_id = global_assets.map16_fg[level_num].texture_id,
    };
    return image;
}

NkImage assets_get_map16_fg_nk(int level_num) {
    Image image = assets_get_map16_fg(level_num);
    return nk_image_id(image.texture_id);
}


NkImage assets_get_map16_fg_tile_nk(int level_num, int tile_x, int tile_y) {
    Image image = assets_get_map16_fg(level_num);
    return nk_subimage_id(image.texture_id,
                          256, 512,
                          nk_rect(16 * tile_x, 16 * tile_y, 16, 16));
}

void assets_load_map16_bg(int level_num) {
    LevelPC* level = smw_get_level(&smw, level_num);
    Image    palette    = assets_get_palette(level_num);
    Image*   image = &global_assets.map16_bg[level_num];
    u32*     image_data = arena_alloc_array(&global_temp_arena, 16*16*256, u32);
    for(int i = 0; i < 2; i++) {
        for(int k = 0; k < 512; k++) {
            for(int j = 0; j < 256; j++) {
                image_data[(k % 16) * 16 + (j % 16) + ((k / 16) * 16 + (j / 16)) * 256]
                    = palette.data[level->map16_bg.tiles[k].data[j]];
            }
        }
    }
    
    image_update(image, 256, 512, image_data);
}



Image assets_get_map16_bg(int level_num) {
    if(!global_assets.map16_bg[level_num].data) {
        assets_load_map16_bg(level_num);
    }
    return global_assets.map16_bg[level_num];
}

NkImage assets_get_map16_bg_nk(int level_num) {
    Image image = assets_get_map16_bg(level_num);
    return nk_image_id(image.texture_id);
}

NkImage assets_get_map16_bg_tile_nk(int level_num, int tile_x, int tile_y) {
    Image image = assets_get_map16_bg(level_num);
    return nk_subimage_id(image.texture_id,
                          256, 512,
                          nk_rect(16 * tile_x, 16 * tile_y, 16, 16));
}
