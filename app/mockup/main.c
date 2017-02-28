#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <math.h>
#include <limits.h>
#include <time.h>

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#include "nuklear.h"
#include "nuklear_glfw_gl3.h"

#define GLFW_INCLUDE_ES2
#include <GLFW/glfw3.h>


#define MAX_VERTEX_BUFFER 512 * 1024
#define MAX_ELEMENT_BUFFER 128 * 1024
#define UNUSED(a) (void)a
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MAX(a,b) ((a) < (b) ? (b) : (a))
#define LEN(a) (sizeof(a)/sizeof(a)[0])

#include "smw.h"

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
    show_level_window = 1,
    show_map16fg_window = 1,
    show_map16bg_window = 1,
    show_map8_window = 1,
    show_app_metrics = 1;

int next_level;
int prev_level;

smw_t          smw;
r65816_rom_t   rom;
palette_pc_t   palette;
map16_pc_t     map16_fg, map16_bg;

uint32_t       map8_pixel_data[128*256*16];
uint32_t       sprite_map8_pixel_data[128*256*16];
uint32_t       map16_fg_pixel_data[256*512];
uint32_t       map16_bg_pixel_data[256*512];
uint16_t*      level_object_data = NULL;
uint16_t*      level_tile_flags = NULL;

uint16_t selected_index_x;
uint16_t selected_index_y;

int hot_index = INT_MAX;
int selected_index = INT_MAX;

uint8_t frame_num = 0;
int current_level = 0x105;
int sprite_map8_palette = 0;
int map8_palette = 0;



typedef struct {
    struct nk_image nk_image_handle;
    unsigned char* data;
    int size_x;
    int size_y;
} image_t;

image_t palette_img;
image_t map8_img[16];
image_t sprite_map8_img[16];
image_t map16_fg_img;
image_t map16_bg_img;

image_t image_init(int size_x, int size_y, unsigned char* data) {
    image_t result;
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

void update_image(image_t image) {
    glBindTexture(GL_TEXTURE_2D, image.nk_image_handle.handle.id);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, image.size_x, image.size_y, GL_RGBA, GL_UNSIGNED_BYTE, image.data);
}

void gl_init_textures() {
    palette_img = image_init(16, 16, (unsigned char*) palette.data);
    for(int p = 0; p < 16; p++) {
        map8_img[p]  = image_init(128, 256, (unsigned char*) &map8_pixel_data[p * 128 * 256]);
        sprite_map8_img[p]  = image_init(128, 256, (unsigned char*) &sprite_map8_pixel_data[p * 128 * 256]);
    }
    map16_fg_img  = image_init(256, 512, (unsigned char*) map16_fg_pixel_data);
    map16_bg_img  = image_init(256, 512, (unsigned char*) map16_bg_pixel_data);
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

    /* Update */
    for(int i = 0; i < 1; i++) {
        smw_level_animate(&smw, current_level, frame_num & 0x1F);
        frame_num++;
    }

    palette_to_pc(&smw.levels[current_level].palette, &palette);
    
    // Map8
    for(int p = 0; p < 16; p++) {
        for(int i = 0; i < 512; i++) {
            for(int j = 0; j < 64; j++) {
                map8_pixel_data[p * 128 * 256 + (i % 16) * 8 + (j % 8) + ((i / 16) * 8 + (j / 8)) * 128]
                    = palette.data[16 * p + smw.levels[current_level].map8.tiles[i].pixels[j]];
            }
        }
    }
    
    // Sprite Map8
    for(int p = 0; p < 16; p++) {
        for(int i = 0; i < 512; i++) {
            for(int j = 0; j < 64; j++) {
                sprite_map8_pixel_data[p * 128 * 256 + (i % 16) * 8 + (j % 8) + ((i / 16) * 8 + (j / 8)) * 128]
                    = palette.data[16 * p + smw.levels[current_level].sprite_map8.tiles[i].pixels[j]];
            }
        }
    }

    
    // Map16
    map16_pc_init(&map16_fg, &smw.levels[current_level].map16_fg);
    map16_pc_init(&map16_bg, &smw.levels[current_level].map16_bg);
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
    level_object_data = (uint16_t*)realloc(level_object_data,
                                           smw.levels[current_level].width * smw.levels[current_level].height * sizeof(uint16_t));
    level_tile_flags = (uint16_t*)realloc(level_tile_flags,
                                          smw.levels[current_level].width * smw.levels[current_level].height * sizeof(uint16_t));

    for(int i = 0; i < smw.levels[current_level].width * smw.levels[current_level].height; i++) {
        level_object_data[i] = 0x25;
        level_tile_flags[i] = 0;
    }
    
    for(int i = 0; i < smw.levels[current_level].layer1_objects.length; i++) {
        object_pc_t* obj = &smw.levels[current_level].layer1_objects.objects[i];
        if(!obj->tiles) continue;
        int obj_width = obj->bb_xmax - obj->bb_xmin + 1;
        int obj_height = obj->bb_ymax - obj->bb_ymin + 1;
        for(int j = 0; j < obj_height; j++) {
            for(int k = 0; k < obj_width; k++) {
                if(j + obj->bb_ymin >= 0 && k + obj->bb_xmin >= 0 &&
                   j + obj->bb_ymin < smw.levels[current_level].height &&
                   k + obj->bb_xmin < smw.levels[current_level].width &&
                   obj->tiles[j * obj_width + k] != 0xFFFF) {
                    level_object_data[(j + obj->bb_ymin) * smw.levels[current_level].width + k + obj->bb_xmin]
                        = obj->tiles[j * obj_width + k];
                    if(hot_index == i) {
                        level_tile_flags[(j + obj->bb_ymin) * smw.levels[current_level].width + k + obj->bb_xmin] |= 0x1;
                    }
                    if(selected_index == i) {
                        level_tile_flags[(j + obj->bb_ymin) * smw.levels[current_level].width + k + obj->bb_xmin] |= 0x2;
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
        if(smw.levels[current_level].is_boss_level) {
            sprintf(window_title, "Level %x - Boss Level", current_level);
            nk_begin_titled(ctx, "Level", window_title, nk_rect(640, 30, 500, 566),
                            NK_WINDOW_MOVABLE|NK_WINDOW_SCALABLE|NK_WINDOW_BORDER|
                            NK_WINDOW_MINIMIZABLE|NK_WINDOW_TITLE);
            nk_end(ctx);
        } else {
            sprintf(window_title, "Level %x", current_level);
            if(nk_begin_titled(ctx, "Level", window_title, nk_rect(640, 30, 500, 566),
                               NK_WINDOW_MOVABLE|NK_WINDOW_SCALABLE|NK_WINDOW_BORDER|
                               NK_WINDOW_MINIMIZABLE|NK_WINDOW_TITLE)) {

                static int show_layer1 = 1;
                static int show_layer2 = 1;
                static int show_layer3 = 1;
                static int show_sprites = 1;

                nk_layout_row_static(ctx, 30, 70, 4);
                nk_checkbox_label(ctx, "Layer 1", &show_layer1);
                nk_checkbox_label(ctx, "Layer 2", &show_layer2);
                nk_checkbox_label(ctx, "Layer 3", &show_layer3);
                nk_checkbox_label(ctx, "Sprites", &show_sprites);

                nk_layout_row_static(ctx, 30, 120, 3);

                nk_property_int(ctx, "Level:", 0, &current_level, 511, 1, 1);

                if(nk_button_label(ctx, "Prev") && current_level > 0)     {
                    current_level--;
                }
                if(nk_button_label(ctx, "Next") && current_level < 0x1FF) {
                    current_level++;
                }

                smw_level_load(&smw, current_level);
                                
                int width = smw.levels[current_level].width;
                int height = smw.levels[current_level].height;
                
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
                        nk_fill_rect(canvas, space, 0, nk_rgba_u32(smw.levels[current_level].background_color));
                    }
                    
                    // Layer 2
                    if(show_layer2) {
                        for(int j = 0; j < height; j++) {
                            for(int i = 0; i < width; i++) {
                                uint16_t tile_num = smw.levels[current_level].layer2_background
                                    .data[(i & 0x10) * 27 + j * 16 + (i & 0x0F)];
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
                    }

                    // Layer 1
                    if(show_layer1) {
                        for(int j = 0; j < height; j++) {
                            for(int i = 0; i < width; i++) {
                                uint16_t tile_num = level_object_data[j * width + i];
                                //uint16_t tile_flags = level_object_flags[j * width + i];
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

                    if(show_sprites) {
                        for(int i = 0; i < smw.levels[current_level].sprites.length; i++) {
                            int num_tiles = smw.levels[current_level].sprites.data[i].num_tiles;
                            for(int j = 0; j < num_tiles; j++) {
                                sprite_tile_t* tile = &smw.levels[current_level].sprites.data[i].tiles[j];
                                uint16_t tile_num = tile->tile_num;
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

                            int sprite_x = space.x + 16 * smw.levels[current_level].sprites.data[i].x;
                            int sprite_y = space.y + 16 * smw.levels[current_level].sprites.data[i].y;
                            char* sprite_name = sprite_names[smw.levels[current_level].sprites.data[i].number];
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
                }                
            }
            nk_end(ctx);


            if(nk_begin(ctx, "Main", nk_rect(0, 0, 200, 400),
                        NK_WINDOW_MOVABLE|NK_WINDOW_SCALABLE|NK_WINDOW_BORDER|
                        NK_WINDOW_MINIMIZABLE|NK_WINDOW_TITLE)) {

                nk_layout_row_static(ctx, 25, 70, 3);
                nk_button_label(ctx, "Open");
            }
            nk_end(ctx);
        }
    }

    
    smw_level_refresh(&smw, current_level);

   
    /* Draw */
    {float bg[4];
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
        glfwSwapBuffers(win);}
}

void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error %d: %s\n", error, description);
}



int main()
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
        printf("Could not create window. Test failed.\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(win);

    /* Nuklear */
    ctx = nk_glfw3_init(win, NK_GLFW3_INSTALL_CALLBACKS);
    /* Load Fonts: if none of these are loaded a default font will be used  */
    {struct nk_font_atlas *atlas;
        nk_glfw3_font_stash_begin(&atlas);
        /*struct nk_font *droid = nk_font_atlas_add_from_file(atlas, "../../../extra_font/DroidSans.ttf", 14, 0);*/
        /*struct nk_font *roboto = nk_font_atlas_add_from_file(atlas, "../../../extra_font/Roboto-Regular.ttf", 14, 0);*/
        /*struct nk_font *future = nk_font_atlas_add_from_file(atlas, "../../../extra_font/kenvector_future_thin.ttf", 13, 0);*/
        /*struct nk_font *clean = nk_font_atlas_add_from_file(atlas, "../../../extra_font/ProggyClean.ttf", 12, 0);*/
        /*struct nk_font *tiny = nk_font_atlas_add_from_file(atlas, "../../../extra_font/ProggyTiny.ttf", 10, 0);*/
        /*struct nk_font *cousine = nk_font_atlas_add_from_file(atlas, "../../../extra_font/Cousine-Regular.ttf", 13, 0);*/
        nk_glfw3_font_stash_end();
        /*nk_style_set_font(ctx, &droid->handle);*/}
    background = nk_rgb(190,205,255);


    r65816_rom_load(&rom, (char*)"smw.sfc");
    smw_init(&smw, &rom);
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
