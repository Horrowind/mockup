static
int global_window_id;

typedef struct {
    NkContext* ctx;
    uint       window_id;
} PaletteViewer;

void palette_viewer(NkContext* ctx, PaletteViewer* palette_viewer, int level_num) {
    if(!palette_viewer->ctx) {
        palette_viewer->ctx = ctx;
        palette_viewer->window_id = global_window_id++;
    }
    ctx = palette_viewer->ctx;
    NkImage image = assets_get_palette_nk(level_num); 

    char window_title[64];
    c_string_format_unsafe(window_title, "Palette - Level %x", level_num);
    if(nk_begin_titled(ctx, (char*)&palette_viewer->window_id, window_title,
                       nk_rect(0, 620, 300, 310),
                       NK_WINDOW_MOVABLE|NK_WINDOW_SCALABLE|NK_WINDOW_BORDER|
                       NK_WINDOW_MINIMIZABLE|NK_WINDOW_TITLE)) {
        nk_layout_row_template_begin(ctx, 256);
        nk_layout_row_template_push_dynamic(ctx);
        nk_layout_row_template_push_static(ctx, 256);
        nk_layout_row_template_push_dynamic(ctx);
        nk_layout_row_template_end(ctx);
        nk_spacing(ctx, 1);
        nk_image(ctx, image);
    }
    nk_end(ctx);
}

typedef struct {
    NkContext* ctx;
    int        palette_num;
    uint       window_id;
} Map8Viewer;

void map8_viewer(NkContext* ctx, Map8Viewer* map8_viewer, int level_num) {
    if(!map8_viewer->ctx) {
        map8_viewer->ctx = ctx;
        map8_viewer->palette_num = 0;
        map8_viewer->window_id = global_window_id++;
    }
    ctx = map8_viewer->ctx;
    NkImage image = assets_get_map8_nk(level_num, map8_viewer->palette_num);

    char window_title[64];
    c_string_format_unsafe(window_title, "Map8 BG - Level %x", level_num);
    if(nk_begin_titled(ctx, (char*)&map8_viewer->window_id, window_title,
                       nk_rect(320, 620, 172, 310),
                       NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE | NK_WINDOW_BORDER |
                       NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE)) {
            
        nk_layout_row_static(ctx, 25, 25, 2);
        if(nk_button_label(ctx, "-") && map8_viewer->palette_num >  0) { map8_viewer->palette_num--; };
        if(nk_button_label(ctx, "+") && map8_viewer->palette_num < 15) { map8_viewer->palette_num++; };

        nk_layout_row_template_begin(ctx, 256);
        nk_layout_row_template_push_dynamic(ctx);
        nk_layout_row_template_push_static(ctx, 128);
        nk_layout_row_template_push_dynamic(ctx);
        nk_layout_row_template_end(ctx);
        nk_spacing(ctx, 1);
        nk_image(ctx, image);
    }
    nk_end(ctx);
}

void sprite_map8_viewer(NkContext* ctx, Map8Viewer* map8_viewer, int level_num) {
    if(!map8_viewer->ctx) {
        map8_viewer->ctx = ctx;
        map8_viewer->palette_num = 0;
        map8_viewer->window_id = global_window_id++;
    }

    NkImage image = assets_get_sprite_map8_nk(level_num, map8_viewer->palette_num);

    char window_title[64];
    c_string_format_unsafe(window_title, "Sprite Map8 - Level %x", level_num);
    if(nk_begin_titled(ctx, (char*)&map8_viewer->window_id, window_title,
                       nk_rect(520, 620, 172, 310),
                       NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE | NK_WINDOW_BORDER |
                       NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE)) {
            
        nk_layout_row_static(ctx, 25, 25, 2);
        if(nk_button_label(ctx, "-") && map8_viewer->palette_num >  0) { map8_viewer->palette_num--; };
        if(nk_button_label(ctx, "+") && map8_viewer->palette_num < 15) { map8_viewer->palette_num++; };

        nk_layout_row_template_begin(ctx, 256);
        nk_layout_row_template_push_dynamic(ctx);
        nk_layout_row_template_push_static(ctx, 128);
        nk_layout_row_template_push_dynamic(ctx);
        nk_layout_row_template_end(ctx);
        nk_spacing(ctx, 1);
        nk_image(ctx, image);
    }
    nk_end(ctx);
}


typedef struct {
    NkContext* ctx;
    uint       window_id;
} Map16Viewer;

void map16_fg_viewer(NkContext* ctx, Map16Viewer* map16_viewer, int level_num) {
    if(!map16_viewer->ctx) {
        map16_viewer->ctx       = ctx;
        map16_viewer->window_id = global_window_id++;
    }
    ctx = map16_viewer->ctx;
    NkImage image = assets_get_map16_fg_nk(level_num);
    char window_title[64];
    c_string_format_unsafe(window_title, "Map16 FG - Level %x", level_num);
    if(nk_begin_titled(ctx, (char*)&map16_viewer->window_id, window_title,
                       nk_rect(0, 30, 300, 566),
                       NK_WINDOW_MOVABLE|NK_WINDOW_SCALABLE|NK_WINDOW_BORDER|
                       NK_WINDOW_MINIMIZABLE|NK_WINDOW_TITLE)) {
        nk_layout_row_template_begin(ctx, 512);
        nk_layout_row_template_push_dynamic(ctx);
        nk_layout_row_template_push_static(ctx, 256);
        nk_layout_row_template_push_dynamic(ctx);
        nk_layout_row_template_end(ctx);
        nk_spacing(ctx, 1);
        nk_image(ctx, image);
    }
    nk_end(ctx);

}

void map16_bg_viewer(NkContext* ctx, Map16Viewer* map16_viewer, int level_num) {
    if(!map16_viewer->ctx) {
        map16_viewer->ctx       = ctx;
        map16_viewer->window_id = global_window_id++;
    }

    ctx = map16_viewer->ctx;
    NkImage image = assets_get_map16_bg_nk(level_num);

    char window_title[64];
    c_string_format_unsafe(window_title, "Map16 BG - Level %x", level_num);
    if(nk_begin_titled(ctx, (char*)&map16_viewer->window_id, window_title,
                       nk_rect(320, 30, 300, 566),
                       NK_WINDOW_MOVABLE|NK_WINDOW_SCALABLE|NK_WINDOW_BORDER|
                       NK_WINDOW_MINIMIZABLE|NK_WINDOW_TITLE)) {
        nk_layout_row_template_begin(ctx, 512);
        nk_layout_row_template_push_dynamic(ctx);
        nk_layout_row_template_push_static(ctx, 256);
        nk_layout_row_template_push_dynamic(ctx);
        nk_layout_row_template_end(ctx);
        nk_spacing(ctx, 1);
        nk_image(ctx, image);
    }
    nk_end(ctx);
}


typedef struct {
    NkContext* ctx;
    uint       window_id;
    LevelPC*   level;
    u16 selected_index_x;
    u16 selected_index_y;

    int hot_index;
    int active_index;

    int level_num;
    int show_layer1;
    int show_layer2;
    int show_layer3;
    int show_sprites;

    int show_palette;
    int show_map8;
    int show_sprite_map8;
    int show_map16_fg;
    int show_map16_bg;
    int show_object_editor;

    int do_drag;
    int drag_origin_x;
    int drag_origin_y;
    int drag_obj_x;
    int drag_obj_y;
    
    PaletteViewer palette_viewer;
    Map8Viewer    map8_viewer;
    Map8Viewer    sprite_map8_viewer;
    Map16Viewer   map16_fg_viewer;
    Map16Viewer   map16_bg_viewer;

    u8 frame_num;
} LevelViewer;

enum {
    UNSELECTED_INDEX = 999999
};


void level_viewer(NkContext* ctx, LevelViewer* level_viewer, int level_num) {
    if(!level_viewer->ctx) {
        level_viewer->ctx                = ctx;
        level_viewer->window_id          = global_window_id++;
        level_viewer->hot_index          = UNSELECTED_INDEX;
        level_viewer->active_index       = UNSELECTED_INDEX;
        level_viewer->level_num          = level_num;
        level_viewer->show_layer1        = 1;
        level_viewer->show_layer2        = 1;
        level_viewer->show_layer3        = 1;
        level_viewer->show_sprites       = 1;

        level_viewer->show_palette       = 0;
        level_viewer->show_map8          = 0;
        level_viewer->show_sprite_map8   = 0;
        level_viewer->show_map16_fg      = 0;
        level_viewer->show_map16_bg      = 0;
        level_viewer->show_object_editor = 0;

        level_viewer->palette_viewer     = (PaletteViewer) { };
        level_viewer->map8_viewer        = (Map8Viewer)    { };
        level_viewer->sprite_map8_viewer = (Map8Viewer)    { };
        level_viewer->map16_fg_viewer    = (Map16Viewer)   { };
        level_viewer->map16_bg_viewer    = (Map16Viewer)   { };

        level_viewer->frame_num          = 0;
        level_viewer->drag_origin_x      = 0;
        level_viewer->drag_origin_y      = 0;
        level_viewer->drag_obj_x         = 0;
        level_viewer->drag_obj_y         = 0;
        level_viewer->do_drag            = 0;
    }
    
    ctx = level_viewer->ctx;
    level_num = level_viewer->level_num;
    
    LevelPC* level = level_viewer->level = smw_get_level(&smw, level_num);

    /* for(int i = 0; i < (level_viewer->frame_num & 0x1F); i++) { */
    /*     smw_level_animate(&smw, level_num, level_viewer->frame_num & 0x1F); */
    /* } */
    /* level_viewer->frame_num++; */
    
    char window_title[64];
    c_string_format_unsafe(window_title, "Level %03x", level_num);
    if(nk_begin_titled(ctx, (char*)&level_viewer->window_id, window_title,
                       nk_rect(640, 30, 500, 566),
                       NK_WINDOW_MOVABLE|NK_WINDOW_SCALABLE|NK_WINDOW_BORDER|
                       NK_WINDOW_MINIMIZABLE|NK_WINDOW_TITLE)) {
        nk_layout_row_static(ctx, 30, 70, 9);
        nk_checkbox_label(ctx, "Layer 1",        &level_viewer->show_layer1);
        nk_checkbox_label(ctx, "Layer 2",        &level_viewer->show_layer2);
        nk_checkbox_label(ctx, "Layer 3",        &level_viewer->show_layer3);
        nk_checkbox_label(ctx, "Sprites",        &level_viewer->show_sprites);
        nk_checkbox_label(ctx, "Objects editor", &level_viewer->show_object_editor);
        nk_checkbox_label(ctx, "Palette",        &level_viewer->show_palette);
        nk_checkbox_label(ctx, "Map 8",          &level_viewer->show_map8);
        nk_checkbox_label(ctx, "Sprite Map8",    &level_viewer->show_sprite_map8);
        nk_checkbox_label(ctx, "Map 16 FG",      &level_viewer->show_map16_fg);
        nk_checkbox_label(ctx, "Map 16 BG",      &level_viewer->show_map16_bg);
        nk_layout_row_static(ctx, 30, 120, 3);

        nk_property_int(ctx, "Level:", 0, &level_viewer->level_num, 511, 1, 1);

        if(nk_button_label(ctx, "Prev") && level_num > 0)     {
            level_viewer->level_num--;
        }
        if(nk_button_label(ctx, "Next") && level_num < 0x1FF) {
            level_viewer->level_num++;
        }
                                
        int width = level->width;
        int height = level->height;
                
        struct nk_command_buffer *canvas;
        //struct nk_input *input = &ctx->input;
        canvas = nk_window_get_canvas(ctx);

                
        nk_layout_row_template_begin(ctx, height * 16);
        nk_layout_row_template_push_dynamic(ctx);
        nk_layout_row_template_push_static(ctx, width * 16);
        nk_layout_row_template_push_dynamic(ctx);
        nk_layout_row_template_end(ctx);
        nk_spacing(ctx, 1);

        
        NkRect space;
        enum nk_widget_layout_states state = nk_widget(&space, ctx);
        struct nk_input* input = &ctx->input;
        if (state) {
            level_viewer->hot_index = UNSELECTED_INDEX;
            if(nk_window_is_hovered(ctx)) {
                for(int i = 0; i < level->layer1_objects.length; i++) {
                    ObjectPC* obj = &level->layer1_objects.objects[i];
                    int bb_x = obj->bb_xmin;
                    int bb_y = obj->bb_ymin;
                    int bb_w = obj->bb_xmax - obj->bb_xmin + 1;
                    int bb_h = obj->bb_ymax - obj->bb_ymin + 1;
                    int mouse_tile_x = (input->mouse.pos.x - space.x) / 16;
                    int mouse_tile_y = (input->mouse.pos.y - space.y) / 16;
                    if(NK_INBOX(mouse_tile_x, mouse_tile_y, bb_x, bb_y, bb_w, bb_h)) {
                        if(obj->tiles[bb_w * mouse_tile_y + mouse_tile_x] != 0xFFFF) {
                            level_viewer->hot_index = i;
                            if(nk_input_is_mouse_down(input, NK_BUTTON_LEFT)
                               && !nk_input_has_mouse_click(input, NK_BUTTON_LEFT)) {
                                level_viewer->active_index = i;
                                level_viewer->do_drag = 1;
                                NkVec2 relative_mouse_pos = nk_vec2_sub(input->mouse.pos, nk_rect_pos(space));
                                level_viewer->drag_origin_x = relative_mouse_pos.x / 16.0f;
                                level_viewer->drag_origin_y = relative_mouse_pos.y / 16.0f;
                                level_viewer->drag_obj_x = obj->x;
                                level_viewer->drag_obj_y = obj->y;
                            }
                        }
                    }
                }
                if(level_viewer->do_drag) {
                    NkVec2 relative_mouse_pos = nk_vec2_sub(input->mouse.pos, nk_rect_pos(space));
                    int x = relative_mouse_pos.x / 16.0f;
                    int y = relative_mouse_pos.y / 16.0f;
                    int delta_x = x - level_viewer->drag_origin_x;
                    int delta_y = y - level_viewer->drag_origin_y;
                    int active_index = level_viewer->active_index;
                    level->layer1_objects.objects[active_index].x = level_viewer->drag_obj_x + delta_x;
                    level->layer1_objects.objects[active_index].y = level_viewer->drag_obj_y + delta_y;
                }
            }

            if(!nk_input_is_mouse_down(input, NK_BUTTON_LEFT)) {
                level_viewer->do_drag = 0;
            }
            
            if (state != NK_WIDGET_ROM) {
                // @TODO: Handle input
            }

            // Layer 3
            if(level_viewer->show_layer3) {
                nk_fill_rect(canvas, space, 0, nk_rgba_u32(level->background_color));
            }
                    
            // Layer 2
            if(level_viewer->show_layer2) {
                if(level->has_layer2_bg) {
                    for(int j = 0; j < height; j++) {
                        for(int i = 0; i < width; i++) {
                            u16 tile_num;
                            if(level->is_vertical_level) {
                                tile_num = level->layer2_background.data[((i & 0x10) * 27) + ((j % 27) * 16) + (i & 0x0F)];
                            } else {
                                tile_num = level->layer2_background.data[((i & 0x10) * 27) + (j << 4) + (i & 0x0F)];
                            }
                            int tile_x = tile_num & 0x00F;
                            int tile_y = (tile_num & 0xFF0) >> 4;
                            NkImage tile_img = assets_get_map16_bg_tile_nk(level_num, tile_x, tile_y);
                            nk_draw_image(canvas, nk_rect(space.x + 16 * i, space.y + 16 * j, 16, 16),
                                          &tile_img, nk_rgba_u32(0xFFFFFFFF));
                        }
                    }
                } else if(level->has_layer2_objects) {
                    for(int i = 0; i < level->layer2_objects.length; i++) {
                        ObjectPC* obj = level->layer2_objects.objects + i;
                        if(!obj->tiles) continue;
                        int obj_width = obj->bb_xmax - obj->bb_xmin + 1;
                        int obj_height = obj->bb_ymax - obj->bb_ymin + 1;
                        for(int j = 0; j < obj_height; j++) {
                            for(int k = 0; k < obj_width; k++) {
                                if(j + obj->bb_ymin >= 0 && k + obj->bb_xmin >= 0 &&
                                   j + obj->bb_ymax < level->height &&
                                   k + obj->bb_xmax < level->width &&
                                   obj->tiles[j * obj_width + k] != 0xFFFF) {
                                    u16 tile_num = obj->tiles[j * obj_width + k];
                                    int tile_x = tile_num & 0x00F;
                                    int tile_y = (tile_num & 0xFF0) >> 4;
                                    NkImage tile_img = assets_get_map16_fg_tile_nk(level_num, tile_x, tile_y);
                                    int pos_x  = k + obj->bb_xmin;
                                    int pos_y  = j + obj->bb_ymin;
                                    nk_draw_image(canvas, nk_rect(space.x + 16 * pos_x, space.y + 16 * pos_y, 16, 16),
                                                  &tile_img, nk_rgba_u32(0xFFFFFFFF));
                                }
                            }
                        }
                    }
                }
            }
            // Layer 1
            if(level_viewer->show_layer1) {
                for(int i = 0; i < level->layer1_objects.length; i++) {
                    ObjectPC* obj = level->layer1_objects.objects + i;
                    if(!obj->tiles) continue;
                    int obj_width = obj->bb_xmax - obj->bb_xmin + 1;
                    int obj_height = obj->bb_ymax - obj->bb_ymin + 1;
                    for(int j = 0; j < obj_height; j++) {
                        for(int k = 0; k < obj_width; k++) {
                            if(j + obj->bb_ymin >= 0 && k + obj->bb_xmin >= 0 &&
                               j + obj->bb_ymin < level->height &&
                               k + obj->bb_xmin < level->width &&
                               obj->tiles[j * obj_width + k] != 0xFFFF) {

                                u16 tile_num = obj->tiles[j * obj_width + k];
                                int tile_x = tile_num & 0x00F;
                                int tile_y = (tile_num & 0xFF0) >> 4;
                                NkImage tile_img = assets_get_map16_fg_tile_nk(level_num, tile_x, tile_y);
                                int pos_x  = k + obj->bb_xmin;
                                int pos_y  = j + obj->bb_ymin;
                                
                                NkColor color = nk_rgba_u32(0xFFFFFFFF);
                                if(i == level_viewer->active_index) {
                                    color = nk_rgba_u32(0xFFFF7777);
                                } else if(i == level_viewer->hot_index) {
                                    color = nk_rgba_u32(0xFF7777FF);
                                } 
                                nk_draw_image(canvas, nk_rect(space.x + 16 * pos_x, space.y + 16 * pos_y, 16, 16),
                                              &tile_img, color);
                            }
                        }
                    }
                }
            }

            if(level_viewer->show_sprites) {
                for(int i = 0; i < level->sprites.length; i++) {
                    int num_tiles = level->sprites.data[i].num_tiles;
                    for(int j = 0; j < num_tiles; j++) {
                        SpriteTile* tile = &level->sprites.data[i].tiles[j];
                        u16 tile_num = tile->tile_num;
                        NkImage tile_img = assets_get_sprite_map8_tile_nk(level_num, tile_num,
                                                                          tile->flip_x, tile->flip_y, tile->palette);
                        nk_draw_image(canvas, nk_rect(space.x + tile->x, space.y + tile->y, 8, 8),
                                      &tile_img, nk_rgba_u32(0xFFFFFFFF));
                    }

                    int sprite_x = space.x + 16 * level->sprites.data[i].x;
                    int sprite_y = space.y + 16 * level->sprites.data[i].y;
                    char* sprite_name = sprite_names[level->sprites.data[i].number];
                    int   sprite_name_length = c_string_length(sprite_name);
                    float sprite_name_width = ctx->style.font->width(ctx->style.font->userdata,
                                                                     ctx->style.font->height,
                                                                     sprite_name,
                                                                     sprite_name_length);
                    NkRect sprite_rect =  nk_rect(sprite_x, sprite_y, sprite_name_width, 16);
                    nk_fill_rect(canvas, sprite_rect, 0, nk_rgba_u32(0x80FF0000));
                    nk_draw_text(canvas, sprite_rect, sprite_name, sprite_name_length, ctx->style.font,
                                 nk_rgba_u32(0x80FF0000), nk_rgba_u32(0x80FFFFFF));

                }
            }
        }                
    }
    nk_end(ctx);
    
    if(level_viewer->show_palette) {
        palette_viewer(ctx, &level_viewer->palette_viewer, level_num);
    }
    if(level_viewer->show_map8) {
        map8_viewer(ctx, &level_viewer->map8_viewer, level_num);
    }
    if(level_viewer->show_sprite_map8) {
        sprite_map8_viewer(ctx, &level_viewer->sprite_map8_viewer, level_num);
    }
    if(level_viewer->show_map16_fg) {
        map16_fg_viewer(ctx, &level_viewer->map16_fg_viewer, level_num);
    }
    if(level_viewer->show_map16_bg) {
        map16_bg_viewer(ctx, &level_viewer->map16_bg_viewer, level_num);
    }
    if(level_viewer->show_object_editor) {
        c_string_format_unsafe(window_title, "Object editor %03x", level_num);
        if(nk_begin(ctx, window_title, nk_rect(30, 630, 400, 400),
                    NK_WINDOW_MOVABLE|NK_WINDOW_SCALABLE|NK_WINDOW_BORDER|
                    NK_WINDOW_MINIMIZABLE|NK_WINDOW_TITLE)) {
            nk_layout_row_static(ctx, 30, 120, 3);
            if(nk_button_label(ctx, "New Object")) {
                ObjectPC new_object = {
                    .x = 5,
                    .y = 5,
                    .num = 1,
                    .settings = 0,
                    .tiles = NULL
                };
                smw_level_add_layer1_object(&smw, level_num, 0, new_object);
                level_viewer->active_index = UNSELECTED_INDEX;
                //debug_break;
            }
            if(level_viewer->active_index != UNSELECTED_INDEX) {
                int i    = level_viewer->active_index;
                int x    = smw.levels[level_num]->layer1_objects.objects[i].x;
                int y    = smw.levels[level_num]->layer1_objects.objects[i].y;
                int num  = smw.levels[level_num]->layer1_objects.objects[i].num;
                int type = smw.levels[level_num]->layer1_objects.objects[i].settings;
                //if(num != 0 || (type != 0 && type != 1)) {
                nk_property_int(ctx, "x",    0, &x, smw.levels[level_num]->width, 1, 0.4);
                smw.levels[level_num]->layer1_objects.objects[i].x = x;
                nk_property_int(ctx, "y",    0, &y, smw.levels[level_num]->height, 1, 0.4);
                smw.levels[level_num]->layer1_objects.objects[i].y = y;
                nk_property_int(ctx, "num",  0, &num,  0x3F, 1, 0.4);
                smw.levels[level_num]->layer1_objects.objects[i].num = num;
                nk_property_int(ctx, "type", 0, &type, 255, 1, 0.4);
                smw.levels[level_num]->layer1_objects.objects[i].settings = type;

            }
            level_105_buffer = smw_level_serialize_fast(&smw, 0x105);
            do_assemble(&smw.temp_arena);
        }
        nk_end(ctx);
    }
}

typedef struct {
    LevelViewer level_viewers[512];
    int         level_viewers_active[512];
    NkContext* ctx;
} SMWViewer;

SMWViewer global_smw_viewer = { };

void smw_viewer(NkContext* ctx, SMWViewer* smw_viewer) {
    if(!smw_viewer->ctx) {
        smw_viewer->ctx = ctx;
    }
    ctx = smw_viewer->ctx;

    if(nk_begin(ctx, "SMW",
                nk_rect(30, 30, 130, 566),
                NK_WINDOW_MOVABLE|NK_WINDOW_SCALABLE|NK_WINDOW_BORDER|
                NK_WINDOW_MINIMIZABLE|NK_WINDOW_TITLE)) {
        nk_layout_row_static(ctx, 25, 100, 1);
        char buffer[256];
        for(int i = 0x100; i <0x110; i++) {
            c_string_format_unsafe(buffer, "Level %03x", i);
            nk_checkbox_label(ctx, buffer, &smw_viewer->level_viewers_active[i]);
        }
    }
    nk_end(ctx);

    for(int i = 0; i < 512; i++) {
        if(smw_viewer->level_viewers_active[i]) {
            level_viewer(ctx, &smw_viewer->level_viewers[i], i);
        }
    }
};
