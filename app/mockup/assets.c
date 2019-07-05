#include "mockup.h"

u32 named_buffer_hash(NamedBuffer b) {
    return hash_murmur3_string(b.name);
}

u32 named_buffer_equal(NamedBuffer b1, NamedBuffer b2) {
    return string_equal(b1.name, b2.name);
}

implement_hashmap(NamedBufferMap, named_buffer_map, NamedBuffer,
                  named_buffer_hash, named_buffer_equal);

void assets_clear(Mockup* mockup) {
    arena_clear(&mockup->temp_arena);
    for(int i = 0; i < 512; i++) {
        mockup->assets.palette[i].data     = NULL;
        mockup->assets.map8[i].data        = NULL;
        mockup->assets.sprite_map8[i].data = NULL;
        mockup->assets.map16_fg[i].data    = NULL;
        mockup->assets.map16_bg[i].data    = NULL;
        mockup->assets.level[i].data       = NULL;
    }
}

void assets_load_palette(Mockup* mockup, int level_num) {
    LevelPC*      level      = smw_get_level(&mockup->smw, level_num);
    Palette*      palette    = &level->palette;
    PalettePC*    palette_pc = arena_alloc_type(&mockup->temp_arena, PalettePC);
    GuiImageData* image      = &mockup->assets.palette[level_num];
    palette_to_pc(palette, palette_pc);
    gui_image_data_update(image, 16, 16, palette_pc->data);
}

GuiImageData assets_get_palette(Mockup* mockup, int level_num) {
    if(!mockup->assets.palette[level_num].data) {
        assets_load_palette(mockup, level_num);
    }
    return mockup->assets.palette[level_num];
}

void assets_load_map8(Mockup* mockup, int level_num) {
    LevelPC*      level      = smw_get_level(&mockup->smw, level_num);
    GuiImageData  palette    = assets_get_palette(mockup, level_num);
    u32*          image_data = arena_alloc_array(&mockup->temp_arena, 128*256*16, u32);
    GuiImageData* image      = &mockup->assets.map8[level_num];

    for(int p = 0; p < 16; p++) {
        for(int i = 0; i < 512; i++) {
            for(int j = 0; j < 64; j++) {
                image_data[p * 128 * 256 + (i % 16) * 8 + (j % 8) + ((i / 16) * 8 + (j / 8)) * 128]
                    = palette.data[16 * p + level->map8.tiles[i].pixels[j]];
            }
        }
    }
    gui_image_data_update(image, 128, 256*16, image_data);
}

GuiImage assets_get_map8(Mockup* mockup, int level_num, int palette_num) {
    if(!mockup->assets.map8[level_num].data) {
        assets_load_map8(mockup, level_num);
    }
    return (GuiImage){
        .texture = mockup->assets.map8[level_num].texture,
        .rect = rect(0.0f, palette_num / 16.0f, 1.0f, 1.0f/16.0f)
    };
}

GuiImageData assets_get_map8_data(Mockup* mockup, int level_num, int palette_num) {
    if(!mockup->assets.map8[level_num].data) {
        assets_load_map8(mockup, level_num);
    }
    GuiImageData image = {
        .width   = 128,
        .height  = 256,
        .data    = mockup->assets.map8[level_num].data + 128 * 256 * palette_num,
        .texture = mockup->assets.map8[level_num].texture,
    };
    return image;
}


void assets_load_sprite_map8(Mockup* mockup, int level_num) {
    LevelPC*      level      = smw_get_level(&mockup->smw, level_num);
    GuiImageData  palette    = assets_get_palette(mockup, level_num);
    u32*          image_data = arena_alloc_array(&mockup->temp_arena, 128*256*16, u32);
    GuiImageData* image      = &mockup->assets.sprite_map8[level_num];

    for(int p = 0; p < 16; p++) {
        for(int i = 0; i < 512; i++) {
            for(int j = 0; j < 64; j++) {
                image_data[p * 128 * 256 + (i % 16) * 8 + (j % 8) + ((i / 16) * 8 + (j / 8)) * 128]
                    = palette.data[16 * p + level->sprite_map8.tiles[i].pixels[j]];
            }
        }
    }
    gui_image_data_update(image, 128, 256*16, image_data);
}

GuiImage assets_get_sprite_map8(Mockup* mockup, int level_num, int palette_num) {
    if(!mockup->assets.sprite_map8[level_num].data) {
        assets_load_sprite_map8(mockup, level_num);
    }
    return (GuiImage){
        .texture = mockup->assets.sprite_map8[level_num].texture,
        .rect = rect(0.0f, palette_num / 16.0f, 1.0f, 1.0f/16.0f)
    };
}

void assets_load_map16_fg(Mockup* mockup, int level_num) {
    LevelPC*      level      = smw_get_level(&mockup->smw, level_num);
    GuiImageData  palette    = assets_get_palette(mockup, level_num);
    GuiImageData* image      = &mockup->assets.map16_fg[level_num];
    u32*          image_data = arena_alloc_array(&mockup->temp_arena, 256*512, u32);
    for(int k = 0; k < 512; k++) {
        for(int j = 0; j < 256; j++) {
            image_data[(k % 16) * 16 + (j % 16) + ((k / 16) * 16 + (j / 16)) * 256]
                = palette.data[level->map16_fg.tiles[k].data[j]];
        }
    }
    gui_image_data_update(image, 256, 512, image_data);
}

GuiImage assets_get_map16_fg(Mockup* mockup, int level_num) {
    if(!mockup->assets.map16_fg[level_num].data) {
        assets_load_map16_fg(mockup, level_num);
    }
    return (GuiImage){
        .texture = mockup->assets.map16_fg[level_num].texture,
        .rect = rect(0,0,1,1)
    };
}

void assets_load_map16_bg(Mockup* mockup, int level_num) {
    LevelPC*      level      = smw_get_level(&mockup->smw, level_num);
    GuiImageData  palette    = assets_get_palette(mockup, level_num);
    GuiImageData* image      = &mockup->assets.map16_bg[level_num];
    u32*          image_data = arena_alloc_array(&mockup->temp_arena, 16*16*256, u32);
    for(int i = 0; i < 2; i++) {
        for(int k = 0; k < 512; k++) {
            for(int j = 0; j < 256; j++) {
                image_data[(k % 16) * 16 + (j % 16) + ((k / 16) * 16 + (j / 16)) * 256]
                    = palette.data[level->map16_bg.tiles[k].data[j]];
            }
        }
    }
    
    gui_image_data_update(image, 256, 512, image_data);
}


GuiImage assets_get_map16_bg(Mockup* mockup, int level_num) {
    if(!mockup->assets.map16_bg[level_num].data) {
        assets_load_map16_bg(mockup, level_num);
    }
    return (GuiImage){
        .texture = mockup->assets.map16_bg[level_num].texture,
        .rect = rect(0,0,1,1)
    };
}
