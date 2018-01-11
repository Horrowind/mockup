#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_VSNPRINTF(s,n,f,a) nk_vsnprintf(s,n,f,a)
#include "nuklear/nuklear.h"
#include "nuklear/nuklear_glfw_gl3.h"

#define MAX_VERTEX_BUFFER 512 * 1024
#define MAX_ELEMENT_BUFFER 128 * 1024
#define UNUSED(a) (void)a
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MAX(a,b) ((a) < (b) ? (b) : (a))
#define LEN(a) (sizeof(a)/sizeof(a)[0])

#include "mockup/smw.h"

extern char* sprite_names[256];
GLFWwindow* win;
struct nk_context *ctx;
struct nk_color background;

int inFullscreen = 0;
int wasFullscreen = 0;

void render();
void error_callback(int error, const char* description);

int g_done = 0;

int show_palette_window = 1,
    show_window = 1,
    show_map16fg_window = 1,
    show_map16bg_window = 1,
    show_map8_window = 1,
    show_app_metrics = 1;

int next_level;
int prev_level;

SMW         smw;
Wdc65816Rom rom;
PalettePC   palette;
Map16PC     map16_fg, map16_bg;

u32       map8_pixel_data[128*256*16];
u32       sprite_map8_pixel_data[128*256*16];
u32       map16_fg_pixel_data[256*512];
u32       map16_bg_pixel_data[256*512];
u16*      level_layer1_object_data = NULL;
u16*      level_layer2_object_data = NULL;

u16 selected_index_x;
u16 selected_index_y;

int hot_index = INT_MAX;
int selected_index = INT_MAX;

uint8_t frame_num = 0;
int current_level = 0x105;
int sprite_map8_palette = 0;
int map8_palette = 0;



typedef struct {
    struct nk_image nk_image_handle;
    uchar* data;
    int size_x;
    int size_y;
} Image;

Image palette_img;
Image map8_img[16];
Image sprite_map8_img[16];
Image map16_fg_img;
Image map16_bg_img;

Image image_init(int size_x, int size_y, uchar* data) {
    Image result;
    GLuint texture;
    result.size_x = size_x;
    result.size_y = size_y;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size_x, size_y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    result.nk_image_handle = nk_image_id((int)texture);
    result.data = data;
    return result;
}

void update_image(Image image) {
    glBindTexture(GL_TEXTURE_2D, image.nk_image_handle.handle.id);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, image.size_x, image.size_y, GL_RGBA, GL_UNSIGNED_BYTE, image.data);
}

void gl_init_textures() {
    palette_img = image_init(16, 16, (uchar*) palette.data);
    for(int p = 0; p < 16; p++) {
        map8_img[p]  = image_init(128, 256, (uchar*) &map8_pixel_data[p * 128 * 256]);
        sprite_map8_img[p]  = image_init(128, 256, (uchar*) &sprite_map8_pixel_data[p * 128 * 256]);
    }
    map16_fg_img  = image_init(256, 512, (uchar*) map16_fg_pixel_data);
    map16_bg_img  = image_init(256, 512, (uchar*) map16_bg_pixel_data);
}


void gl_update_textures() {
    update_image(palette_img);
    for(int p = 0; p < 16; p++) {
        update_image(map8_img[p]);
        update_image(sprite_map8_img[p]);
    }
    update_image(map16_fg_img);
    update_image(map16_bg_img);
}



void render() {
    /* Input */
    int fb_width = 0;
    int fb_height = 0;
    
    glfwPollEvents();
    nk_glfw3_new_frame();

    glfwGetWindowSize(win, &fb_width, &fb_height);

    LevelPC* level = smw.levels + current_level;
    
    /* Update */
    for(int i = 0; i < 1; i++) {
        smw_level_animate(&smw, current_level, frame_num & 0x1F);
        frame_num++;
    }

    
    
    palette_to_pc(&level->palette, &palette);
    
    // Map8
    for(int p = 0; p < 16; p++) {
        for(int i = 0; i < 512; i++) {
            for(int j = 0; j < 64; j++) {
                map8_pixel_data[p * 128 * 256 + (i % 16) * 8 + (j % 8) + ((i / 16) * 8 + (j / 8)) * 128]
                    = palette.data[16 * p + level->map8.tiles[i].pixels[j]];
            }
        }
    }

    #ifdef SPRITES
    // Sprite Map8
    for(int p = 0; p < 16; p++) {
        for(int i = 0; i < 512; i++) {
            for(int j = 0; j < 64; j++) {
                sprite_map8_pixel_data[p * 128 * 256 + (i % 16) * 8 + (j % 8) + ((i / 16) * 8 + (j / 8)) * 128]
                    = palette.data[16 * p + level->sprite_map8.tiles[i].pixels[j]];
            }
        }
    }
    #endif
    
    // Map16
    map16_pc_init(&map16_fg, &level->map16_fg);
    map16_pc_init(&map16_bg, &level->map16_bg);
    for(int i = 0; i < 2; i++) {
        for(int k = 0; k < 512; k++) {
            for(int j = 0; j < 256; j++) {
                map16_fg_pixel_data[(k % 16) * 16 + (j % 16) + ((k / 16) * 16 + (j / 16)) * 256]
                    = palette.data[map16_fg.tiles[k].data[j]];
                map16_bg_pixel_data[(k % 16) * 16 + (j % 16) + ((k / 16) * 16 + (j / 16)) * 256]
                    = palette.data[map16_bg.tiles[k].data[j]];
            }
        }
    }

    // Level objects
    level_layer1_object_data = (u16*)realloc(level_layer1_object_data,
                                                  level->width * level->height * sizeof(u16));
    level_layer2_object_data = (u16*)realloc(level_layer2_object_data,
                                                  level->width * level->height * sizeof(u16));

    for(int i = 0; i < level->width * level->height; i++) {
        level_layer1_object_data[i] = 0x25;
        level_layer2_object_data[i] = 0x25;
    }
    
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
                    level_layer1_object_data[(j + obj->bb_ymin) * level->width + k + obj->bb_xmin]
                        = obj->tiles[j * obj_width + k];
                }
            }
        }
    }

    if(level->has_layer2_objects) {
        for(int i = 0; i < level->layer2_objects.length; i++) {
            ObjectPC* obj = level->layer2_objects.objects + i;
            if(!obj->tiles) continue;
            int obj_width = obj->bb_xmax - obj->bb_xmin + 1;
            int obj_height = obj->bb_ymax - obj->bb_ymin + 1;
            for(int j = 0; j < obj_height; j++) {
                for(int k = 0; k < obj_width; k++) {
                    if(j + obj->bb_ymin >= 0 && k + obj->bb_xmin >= 0 &&
                       j + obj->bb_ymin < level->height &&
                       k + obj->bb_xmin < level->width &&
                       obj->tiles[j * obj_width + k] != 0xFFFF) {
                        level_layer2_object_data[(j + obj->bb_ymin) * level->width + k + obj->bb_xmin]
                            = obj->tiles[j * obj_width + k];
                    }
                }
            }
        }
    }

    gl_update_textures();
    map16_pc_deinit(&map16_fg);
    map16_pc_deinit(&map16_bg);
   
    { /* GUI */
        /* nk_style_push_color(ctx, &ctx->style.window.background, nk_rgba_u32(0xFF000000)); */
        /* if(nk_begin(ctx, "Main", nk_rect(0, 0, 50, 50), NK_WINDOW_BACKGROUND | NK_WINDOW_SCALABLE)) { */
        /*     nk_window_set_size(ctx, nk_vec2(fb_width, fb_height)); */
        /*     nk_menubar_begin(ctx); */
        /*     nk_layout_row_begin(ctx, NK_STATIC, 25, 4); */
        /*     nk_layout_row_push(ctx, 45); */
        /*     if(nk_menu_begin_label(ctx, "MENU", NK_TEXT_LEFT, nk_vec2(120, 200))) { */
        /*         static size_t prog = 40; */
        /*         static int slider = 10; */
        /*         static int check = nk_true; */
        /*         nk_layout_row_dynamic(ctx, 25, 1); */
        /*         nk_progress(ctx, &prog, 100, NK_MODIFIABLE); */
        /*         nk_slider_int(ctx, 0, &slider, 16, 1); */
        /*         nk_checkbox_label(ctx, "check", &check); */
        /*         nk_menu_end(ctx); */
        /*     } */
        /*     nk_menubar_end(ctx); */
        /* } */
        /* nk_end(ctx); */
        /* nk_style_pop_color(ctx); */

        
       
        if(nk_begin(ctx, "Map8", nk_rect(320, 620, 172, 310),
                    NK_WINDOW_MOVABLE|NK_WINDOW_SCALABLE|NK_WINDOW_BORDER|
                    NK_WINDOW_MINIMIZABLE|NK_WINDOW_TITLE)) {
            
            nk_layout_row_static(ctx, 25, 25, 2);
            if(nk_button_label(ctx, "-") && map8_palette > 0) { map8_palette--; };
            if(nk_button_label(ctx, "+") && map8_palette < 15) { map8_palette++; };

            nk_layout_row_template_begin(ctx, 256);
            nk_layout_row_template_push_dynamic(ctx);
            nk_layout_row_template_push_static(ctx, 128);
            nk_layout_row_template_push_dynamic(ctx);
            nk_layout_row_template_end(ctx);
            nk_spacing(ctx, 1);
            nk_image(ctx, map8_img[map8_palette].nk_image_handle);
        }
        nk_end(ctx);

#ifdef SPRITES
        if(nk_begin(ctx, "Sprite Map8", nk_rect(620, 620, 172, 310),
                    NK_WINDOW_MOVABLE|NK_WINDOW_SCALABLE|NK_WINDOW_BORDER|
                    NK_WINDOW_MINIMIZABLE|NK_WINDOW_TITLE)) {
            nk_layout_row_static(ctx, 25, 25, 2);
            if(nk_button_label(ctx, "-") && sprite_map8_palette > 0) { sprite_map8_palette--; };
            if(nk_button_label(ctx, "+") && sprite_map8_palette < 15) { sprite_map8_palette++; };
                
            nk_layout_row_template_begin(ctx, 256);
            nk_layout_row_template_push_dynamic(ctx);
            nk_layout_row_template_push_static(ctx, 128);
            nk_layout_row_template_push_dynamic(ctx);
            nk_layout_row_template_end(ctx);
            nk_spacing(ctx, 1);
            nk_image(ctx, sprite_map8_img[sprite_map8_palette].nk_image_handle);
        }
        nk_end(ctx);
#endif        
        if(nk_begin(ctx, "Palette", nk_rect(0, 620, 300, 310),
                    NK_WINDOW_MOVABLE|NK_WINDOW_SCALABLE|NK_WINDOW_BORDER|
                    NK_WINDOW_MINIMIZABLE|NK_WINDOW_TITLE)) {
            nk_layout_row_template_begin(ctx, 256);
            nk_layout_row_template_push_dynamic(ctx);
            nk_layout_row_template_push_static(ctx, 256);
            nk_layout_row_template_push_dynamic(ctx);
            nk_layout_row_template_end(ctx);
            nk_spacing(ctx, 1);
            nk_image(ctx, palette_img.nk_image_handle);
        }
        nk_end(ctx);

        if(nk_begin(ctx, "Map16 FG", nk_rect(0, 30, 300, 566),
                    NK_WINDOW_MOVABLE|NK_WINDOW_SCALABLE|NK_WINDOW_BORDER|
                    NK_WINDOW_MINIMIZABLE|NK_WINDOW_TITLE)) {
            nk_layout_row_template_begin(ctx, 512);
            nk_layout_row_template_push_dynamic(ctx);
            nk_layout_row_template_push_static(ctx, 256);
            nk_layout_row_template_push_dynamic(ctx);
            nk_layout_row_template_end(ctx);
            nk_spacing(ctx, 1);
            nk_image(ctx, map16_fg_img.nk_image_handle);
        }
        nk_end(ctx);

        if(nk_begin(ctx, "Map16 BG", nk_rect(320, 30, 300, 566),
                    NK_WINDOW_MOVABLE|NK_WINDOW_SCALABLE|NK_WINDOW_BORDER|
                    NK_WINDOW_MINIMIZABLE|NK_WINDOW_TITLE)) {
            nk_layout_row_template_begin(ctx, 512);
            nk_layout_row_template_push_dynamic(ctx);
            nk_layout_row_template_push_static(ctx, 256);
            nk_layout_row_template_push_dynamic(ctx);
            nk_layout_row_template_end(ctx);
            nk_spacing(ctx, 1);
            nk_image(ctx, map16_bg_img.nk_image_handle);
        }
        nk_end(ctx);
        

        char window_title[64];
        sprintf(window_title, "Level %x", current_level);
        if(nk_begin_titled(ctx, "Level", window_title, nk_rect(640, 30, 500, 566),
                           NK_WINDOW_MOVABLE|NK_WINDOW_SCALABLE|NK_WINDOW_BORDER|
                           NK_WINDOW_MINIMIZABLE|NK_WINDOW_TITLE)) {

            static int show_layer1 = 1;
            static int show_layer2 = 1;
            static int show_layer3 = 1;

            nk_layout_row_static(ctx, 30, 70, 4);
            nk_checkbox_label(ctx, "Layer 1", &show_layer1);
            nk_checkbox_label(ctx, "Layer 2", &show_layer2);
            nk_checkbox_label(ctx, "Layer 3", &show_layer3);
#ifdef SPRITES
            static int show_sprites = 1;
            nk_checkbox_label(ctx, "Sprites", &show_sprites);
#endif
            nk_layout_row_static(ctx, 30, 120, 3);

            int current_level_old = current_level;
            nk_property_int(ctx, "Level:", 0, &current_level, 511, 1, 1);

            if(nk_button_label(ctx, "Prev") && current_level > 0)     {
                current_level--;
            }
            if(nk_button_label(ctx, "Next") && current_level < 0x1FF) {
                current_level++;
            }
            if(current_level_old != current_level)
                smw_level_load(&smw, current_level);
                                
            int width = level->width;
            int height = level->height;
                
            struct nk_command_buffer *canvas;
            //struct nk_input *input = &ctx->input;
            canvas = nk_window_get_canvas(ctx);

                
            struct nk_rect space;
            enum nk_widget_layout_states state;
            nk_layout_row_template_begin(ctx, height * 16);
            nk_layout_row_template_push_dynamic(ctx);
            nk_layout_row_template_push_static(ctx, width * 16);
            nk_layout_row_template_push_dynamic(ctx);
            nk_layout_row_template_end(ctx);
            nk_spacing(ctx, 1);

                
            state = nk_widget(&space, ctx);
            if (state) {
                if (state != NK_WIDGET_ROM) {
                    // @TODO: Handle input
                }

                // Layer 3
                if(show_layer3) {
                    nk_fill_rect(canvas, space, 0, nk_rgba_u32(level->background_color));
                }
                    
                // Layer 2
                if(show_layer2) {
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
                                struct nk_image tile_img = nk_subimage_handle(
                                    map16_bg_img.nk_image_handle.handle,
                                    256, 512,
                                    nk_rect(16 * tile_x, 16 * tile_y, 16, 16));
                                nk_draw_image(canvas, nk_rect(space.x + 16 * i, space.y + 16 * j, 16, 16),
                                              &tile_img, nk_rgba_u32(0xFFFFFFFF));
                            }
                        }
                    } else if(level->has_layer2_objects) {
                        for(int j = 0; j < height; j++) {
                            for(int i = 0; i < width; i++) {
                                u16 tile_num = level_layer2_object_data[j * width + i];
                                int tile_x = tile_num & 0x00F;
                                int tile_y = (tile_num & 0xFF0) >> 4;
                                struct nk_image tile_img = nk_subimage_handle(
                                    map16_fg_img.nk_image_handle.handle,
                                    256, 512,
                                    nk_rect(16 * tile_x, 16 * tile_y, 16, 16));
                                nk_draw_image(canvas, nk_rect(space.x + 16 * i, space.y + 16 * j, 16, 16),
                                              &tile_img, nk_rgba_u32(0xFFFFFFFF));
                            }
                        }
                    }
                }
                // Layer 1
                if(show_layer1) {
                    for(int j = 0; j < height; j++) {
                        for(int i = 0; i < width; i++) {
                            u16 tile_num = level_layer1_object_data[j * width + i];
                            int tile_x = tile_num & 0x00F;
                            int tile_y = (tile_num & 0xFF0) >> 4;
                            struct nk_image tile_img = nk_subimage_handle(
                                map16_fg_img.nk_image_handle.handle,
                                256, 512,
                                nk_rect(16 * tile_x, 16 * tile_y, 16, 16));
                            nk_draw_image(canvas, nk_rect(space.x + 16 * i, space.y + 16 * j, 16, 16),
                                          &tile_img, nk_rgba_u32(0xFFFFFFFF));
                        }
                    }
                }
#ifdef SPRITES
                if(show_sprites) {
                    for(int i = 0; i < level->sprites.length; i++) {
                        int num_tiles = level->sprites.data[i].num_tiles;
                        for(int j = 0; j < num_tiles; j++) {
                            SpriteTile* tile = &level->sprites.data[i].tiles[j];
                            u16 tile_num = tile->tile_num;
                            int tile_x = tile_num & 0x00F;
                            int tile_y = (tile_num & 0xFF0) >> 4;
                            struct nk_rect tile_rect;
                            if(tile->flip_x) {
                                if(tile->flip_y) {
                                    tile_rect = nk_rect(8 * tile->x + 8, 8 * tile_y + 8, -8, -8);
                                } else {
                                    tile_rect = nk_rect(8 * tile_x + 8, 8 * tile_y + 0, -8,  8);
                                }
                            } else {
                                if(tile->flip_y) {
                                    tile_rect = nk_rect(8 * tile_x + 0, 8 * tile_y + 8,  8, -8);
                                } else {
                                    tile_rect = nk_rect(8 * tile_x + 0, 8 * tile_y + 0,  8,  8);
                                }
                            }


                                
                            struct nk_image tile_img = nk_subimage_handle(
                                sprite_map8_img[tile->palette].nk_image_handle.handle,
                                128, 256, tile_rect);
                            nk_draw_image(canvas, nk_rect(space.x + tile->x, space.y + tile->y, 8, 8),
                                          &tile_img, nk_rgba_u32(0xFFFFFFFF));
                        }

                        int sprite_x = space.x + 16 * level->sprites.data[i].x;
                        int sprite_y = space.y + 16 * level->sprites.data[i].y;
                        char* sprite_name = sprite_names[level->sprites.data[i].number];
                        int   sprite_name_length = strlen(sprite_name);
                        float sprite_name_width = ctx->style.font->width(ctx->style.font->userdata,
                                                                         ctx->style.font->height,
                                                                         sprite_name,
                                                                         sprite_name_length);
                        struct nk_rect sprite_rect =  nk_rect(sprite_x, sprite_y, sprite_name_width, 16);
                        nk_fill_rect(canvas, sprite_rect, 0, nk_rgba_u32(0x80FF0000));
                        nk_draw_text(canvas, sprite_rect, sprite_name, sprite_name_length, ctx->style.font,
                                     nk_rgba_u32(0x80FF0000), nk_rgba_u32(0x80FFFFFF));

                    }
                }
#endif
            }                
            nk_end(ctx);


#ifdef SPRITES
            if(nk_begin(ctx, "Sprites", nk_rect(0, 0, 1200, 00),
                        NK_WINDOW_MOVABLE|NK_WINDOW_SCALABLE|NK_WINDOW_BORDER|
                        NK_WINDOW_MINIMIZABLE|NK_WINDOW_TITLE)) {
                
                nk_layout_row_begin(ctx, NK_STATIC, 10, 50);
                nk_layout_row_push(ctx, 20);
                nk_label(ctx, " ", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 200);
                nk_label(ctx, " ", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "00", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "00", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "00", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "00", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "00", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "00", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "14", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "14", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "14", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "14", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "14", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "15", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "15", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "15", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "15", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "15", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "15", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "15", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "15", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "15", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "15", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "15", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "15", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "15", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "15", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "15", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "15", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "15", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "15", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "15", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "15", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "15", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "16", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "16", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "16", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "16", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "16", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "16", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "16", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "16", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "16", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "16", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "16", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "16", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "18", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "18", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "19", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "1F", NK_TEXT_ALIGN_RIGHT);

                nk_layout_row_begin(ctx, NK_STATIC, 20, 50);
                nk_layout_row_push(ctx, 20);
                nk_label(ctx, " ", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 200);
                nk_label(ctx, " ", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "9E", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "AA", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "B6", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "C2", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "D8", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "E4", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "C8", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "D4", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "E0", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "EC", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "F8", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "04", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "10", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "1C", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "28", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "34", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "40", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "4C", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "58", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "64", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "70", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "7C", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "88", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "94", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "A0", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "AC", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "B8", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "C4", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "D0", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "DC", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "EA", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "F6", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "02", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "0E", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "1A", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "26", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "32", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "3E", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "4A", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "56", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "62", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "6E", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "7A", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "86", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "6C", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "7B", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "0F", NK_TEXT_ALIGN_RIGHT);
                nk_layout_row_push(ctx, 15); nk_label(ctx, "E2", NK_TEXT_ALIGN_RIGHT);
                
                
                for(int i = 0; i < level->sprites.length; i++) {
                    SpriteTableEntries* entries = &level->sprites.data[i].table_entries;
                    nk_layout_row_begin(ctx, NK_STATIC, 20, 50);
                    nk_layout_row_push(ctx, 20);
                    nk_labelf(ctx, NK_TEXT_ALIGN_RIGHT, "%i", i);
                    char sprite_name[64];
                    sprintf(sprite_name, "%s", sprite_names[level->sprites.data[i].number]);
                    nk_layout_row_push(ctx, 200);
                    nk_label(ctx, sprite_name, NK_TEXT_ALIGN_LEFT);
                    nk_layout_row_push(ctx, 15); nk_labelf(ctx, NK_TEXT_ALIGN_RIGHT, "%02x", entries->entry_009E);
                    nk_layout_row_push(ctx, 15); nk_labelf(ctx, NK_TEXT_ALIGN_RIGHT, "%02x", entries->entry_00AA);
                    nk_layout_row_push(ctx, 15); nk_labelf(ctx, NK_TEXT_ALIGN_RIGHT, "%02x", entries->entry_00B6);
                    nk_layout_row_push(ctx, 15); nk_labelf(ctx, NK_TEXT_ALIGN_RIGHT, "%02x", entries->entry_00C2);
                    nk_layout_row_push(ctx, 15); nk_labelf(ctx, NK_TEXT_ALIGN_RIGHT, "%02x", entries->entry_00D8);
                    nk_layout_row_push(ctx, 15); nk_labelf(ctx, NK_TEXT_ALIGN_RIGHT, "%02x", entries->entry_00E4);
                    nk_layout_row_push(ctx, 15); nk_labelf(ctx, NK_TEXT_ALIGN_RIGHT, "%02x", entries->entry_14C8);
                    nk_layout_row_push(ctx, 15); nk_labelf(ctx, NK_TEXT_ALIGN_RIGHT, "%02x", entries->entry_14D4);
                    nk_layout_row_push(ctx, 15); nk_labelf(ctx, NK_TEXT_ALIGN_RIGHT, "%02x", entries->entry_14E0);
                    nk_layout_row_push(ctx, 15); nk_labelf(ctx, NK_TEXT_ALIGN_RIGHT, "%02x", entries->entry_14EC);
                    nk_layout_row_push(ctx, 15); nk_labelf(ctx, NK_TEXT_ALIGN_RIGHT, "%02x", entries->entry_14F8);
                    nk_layout_row_push(ctx, 15); nk_labelf(ctx, NK_TEXT_ALIGN_RIGHT, "%02x", entries->entry_1504);
                    nk_layout_row_push(ctx, 15); nk_labelf(ctx, NK_TEXT_ALIGN_RIGHT, "%02x", entries->entry_1510);
                    nk_layout_row_push(ctx, 15); nk_labelf(ctx, NK_TEXT_ALIGN_RIGHT, "%02x", entries->entry_151C);
                    nk_layout_row_push(ctx, 15); nk_labelf(ctx, NK_TEXT_ALIGN_RIGHT, "%02x", entries->entry_1528);
                    nk_layout_row_push(ctx, 15); nk_labelf(ctx, NK_TEXT_ALIGN_RIGHT, "%02x", entries->entry_1534);
                    nk_layout_row_push(ctx, 15); nk_labelf(ctx, NK_TEXT_ALIGN_RIGHT, "%02x", entries->entry_1540);
                    nk_layout_row_push(ctx, 15); nk_labelf(ctx, NK_TEXT_ALIGN_RIGHT, "%02x", entries->entry_154C);
                    nk_layout_row_push(ctx, 15); nk_labelf(ctx, NK_TEXT_ALIGN_RIGHT, "%02x", entries->entry_1558);
                    nk_layout_row_push(ctx, 15); nk_labelf(ctx, NK_TEXT_ALIGN_RIGHT, "%02x", entries->entry_1564);
                    nk_layout_row_push(ctx, 15); nk_labelf(ctx, NK_TEXT_ALIGN_RIGHT, "%02x", entries->entry_1570);
                    nk_layout_row_push(ctx, 15); nk_labelf(ctx, NK_TEXT_ALIGN_RIGHT, "%02x", entries->entry_157C);
                    nk_layout_row_push(ctx, 15); nk_labelf(ctx, NK_TEXT_ALIGN_RIGHT, "%02x", entries->entry_1588);
                    nk_layout_row_push(ctx, 15); nk_labelf(ctx, NK_TEXT_ALIGN_RIGHT, "%02x", entries->entry_1594);
                    nk_layout_row_push(ctx, 15); nk_labelf(ctx, NK_TEXT_ALIGN_RIGHT, "%02x", entries->entry_15A0);
                    nk_layout_row_push(ctx, 15); nk_labelf(ctx, NK_TEXT_ALIGN_RIGHT, "%02x", entries->entry_15AC);
                    nk_layout_row_push(ctx, 15); nk_labelf(ctx, NK_TEXT_ALIGN_RIGHT, "%02x", entries->entry_15B8);
                    nk_layout_row_push(ctx, 15); nk_labelf(ctx, NK_TEXT_ALIGN_RIGHT, "%02x", entries->entry_15C4);
                    nk_layout_row_push(ctx, 15); nk_labelf(ctx, NK_TEXT_ALIGN_RIGHT, "%02x", entries->entry_15D0);
                    nk_layout_row_push(ctx, 15); nk_labelf(ctx, NK_TEXT_ALIGN_RIGHT, "%02x", entries->entry_15DC);
                    nk_layout_row_push(ctx, 15); nk_labelf(ctx, NK_TEXT_ALIGN_RIGHT, "%02x", entries->entry_15EA);
                    nk_layout_row_push(ctx, 15); nk_labelf(ctx, NK_TEXT_ALIGN_RIGHT, "%02x", entries->entry_15F6);
                    nk_layout_row_push(ctx, 15); nk_labelf(ctx, NK_TEXT_ALIGN_RIGHT, "%02x", entries->entry_1602);
                    nk_layout_row_push(ctx, 15); nk_labelf(ctx, NK_TEXT_ALIGN_RIGHT, "%02x", entries->entry_160E);
                    nk_layout_row_push(ctx, 15); nk_labelf(ctx, NK_TEXT_ALIGN_RIGHT, "%02x", entries->entry_161A);
                    nk_layout_row_push(ctx, 15); nk_labelf(ctx, NK_TEXT_ALIGN_RIGHT, "%02x", entries->entry_1626);
                    nk_layout_row_push(ctx, 15); nk_labelf(ctx, NK_TEXT_ALIGN_RIGHT, "%02x", entries->entry_1632);
                    nk_layout_row_push(ctx, 15); nk_labelf(ctx, NK_TEXT_ALIGN_RIGHT, "%02x", entries->entry_163E);
                    nk_layout_row_push(ctx, 15); nk_labelf(ctx, NK_TEXT_ALIGN_RIGHT, "%02x", entries->entry_164A);
                    nk_layout_row_push(ctx, 15); nk_labelf(ctx, NK_TEXT_ALIGN_RIGHT, "%02x", entries->entry_1656);
                    nk_layout_row_push(ctx, 15); nk_labelf(ctx, NK_TEXT_ALIGN_RIGHT, "%02x", entries->entry_1662);
                    nk_layout_row_push(ctx, 15); nk_labelf(ctx, NK_TEXT_ALIGN_RIGHT, "%02x", entries->entry_166E);
                    nk_layout_row_push(ctx, 15); nk_labelf(ctx, NK_TEXT_ALIGN_RIGHT, "%02x", entries->entry_167A);
                    nk_layout_row_push(ctx, 15); nk_labelf(ctx, NK_TEXT_ALIGN_RIGHT, "%02x", entries->entry_1686);
                    nk_layout_row_push(ctx, 15); nk_labelf(ctx, NK_TEXT_ALIGN_RIGHT, "%02x", entries->entry_186C);
                    nk_layout_row_push(ctx, 15); nk_labelf(ctx, NK_TEXT_ALIGN_RIGHT, "%02x", entries->entry_187B);
                    nk_layout_row_push(ctx, 15); nk_labelf(ctx, NK_TEXT_ALIGN_RIGHT, "%02x", entries->entry_190F);
                    nk_layout_row_push(ctx, 15); nk_labelf(ctx, NK_TEXT_ALIGN_RIGHT, "%02x", entries->entry_1FE2);

                }
            }                
            nk_end(ctx);
            
            static int do_once = 1;
            if(do_once) {
                nk_window_collapse(ctx, "Sprites", NK_MINIMIZED);
                do_once = 0;
            }
#endif

                
            /* if(nk_begin(ctx, "Main", nk_rect(0, 0, 200, 400), */
            /*             NK_WINDOW_MOVABLE|NK_WINDOW_SCALABLE|NK_WINDOW_BORDER| */
            /*             NK_WINDOW_MINIMIZABLE|NK_WINDOW_TITLE)) { */

            /*     nk_layout_row_static(ctx, 25, 70, 3); */
            /*     nk_button_label(ctx, "Open"); */
            /* } */
            /* nk_end(ctx); */
        }
    }

    
    //smw_level_refresh(&smw, current_level);

   
    /* Draw */
    {
        float bg[4];
        nk_color_fv(bg, background);

        glViewport(0, 0, fb_width, fb_height);
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(bg[0], bg[1], bg[2], bg[3]);
        /* IMPORTANT: `nk_glfw3_render` modifies some global OpenGL state
         * with blending, scissor, face culling, depth test and viewport and
         * defaults everything back into a default state.
         * Make sure to either a.) save and restore or b.) reset your own state after
         * rendering the UI. */
        nk_glfw3_render(NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
        glfwSwapBuffers(win);
    }
}

void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error %d: %s\n", error, description);
}



int main(int argc, char** argv)
{
    /* Setup win */
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
    {
        printf("Could not create window. Test failed.\n");
        return -1;
    }
    glfwWindowHint(GLFW_RESIZABLE , 1);

    int width = 1280;
    int height = 1024;
#ifdef __EMSCRIPTEN__
    int is_fullscreen = 1;
    emscripten_get_canvas_size(&width, &height, &is_fullscreen);
#endif
    
    win = glfwCreateWindow(width, height, "Mockup", NULL, NULL);
    if (!win)
    {
        printf("Could not create window.\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(win);

    /* Nuklear */
    ctx = nk_glfw3_init(win, NK_GLFW3_INSTALL_CALLBACKS);
    /* Load Fonts: if none of these are loaded a default font will be used  */
    struct nk_font_atlas *atlas;
    nk_glfw3_font_stash_begin(&atlas);
    /*struct nk_font *droid = nk_font_atlas_add_from_file(atlas, "../../../extra_font/DroidSans.ttf", 14, 0);*/
    /*struct nk_font *roboto = nk_font_atlas_add_from_file(atlas, "../../../extra_font/Roboto-Regular.ttf", 14, 0);*/
    /*struct nk_font *future = nk_font_atlas_add_from_file(atlas, "../../../extra_font/kenvector_future_thin.ttf", 13, 0);*/
    /*struct nk_font *clean = nk_font_atlas_add_from_file(atlas, "../../../extra_font/ProggyClean.ttf", 12, 0);*/
    /*struct nk_font *tiny = nk_font_atlas_add_from_file(atlas, "../../../extra_font/ProggyTiny.ttf", 10, 0);*/
    /*struct nk_font *cousine = nk_font_atlas_add_from_file(atlas, "../../../extra_font/Cousine-Regular.ttf", 13, 0);*/
    nk_glfw3_font_stash_end();
    /*nk_style_set_font(ctx, &droid->handle);*/
    background = nk_rgb(190,205,255);


    Arena arena = arena_create(MB(1024));
    String path_name = (argc > 2) ? string_from_c_string(argv[2]) : L("SuperMarioWorld.sfc");
    Path path;
    path_init(&path, path_name);
    VFS vfs;
    vfs_init(&vfs, 4);
    vfs_add_dir(&vfs, &path, &arena);
    
    Wdc65816Rom rom;
    wdc65816_rom_init(&rom, &vfs, &arena);

    smw_init(&smw, &rom, &arena);
    smw_level_load(&smw, current_level);

    gl_init_textures();

   
#ifdef __EMSCRIPTEN__
    /* Main loop */
    emscripten_set_main_loop(render, 0, 1);

#else
    while (!glfwWindowShouldClose(win))
    {
        render();
    }
#endif
   
   
    nk_glfw3_shutdown();
    glfwTerminate();

    return 0;
}
