#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_VSNPRINTF format_string
//#define NK_VSNPRINTF(s,n,f,a) nk_vsnprintf(s,n,f,a)
#include "nuklear/nuklear.h"
#include "nuklear/nuklear_glfw_gl3.h"

#include "mockup/smw.h"
#include "nuts/nuts.h"

typedef struct nk_context NkContext;
typedef struct nk_image   NkImage;
typedef struct nk_rect    NkRect;
typedef struct nk_color   NkColor;
typedef struct nk_vec2    NkVec2;

extern char* sprite_names[256];
GLFWwindow* win;
NkContext*  ctx;
struct nk_color background;

SMW smw;

AST ast;
Assembler assembler;
Wdc65816Mapper rom;
ErrorList error_list;

u8 level_105_data[] = { 0x33, 0x40, 0x08, 0x80, 0x27, 0xFF };
Buffer level_105_buffer = {
    .begin = level_105_data,
    .end   = level_105_data + sizeof(level_105_data)
};

/// Image /////////////////////////////////////////////////////////

typedef struct {
    GLuint texture_id;
    u32*   data;
    int size_x;
    int size_y;
} Image;

void image_init(Image* image) {
    *image = (Image) { };
    glGenTextures(1, &image->texture_id);
    glBindTexture(GL_TEXTURE_2D, image->texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void image_update(Image* image, int size_x, int size_y, u32* data) {
    if(data) {
        image->size_x = size_x;
        image->size_y = size_y;
        image->data   = data;
        glBindTexture(GL_TEXTURE_2D, image->texture_id);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size_x, size_y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }
}

/// Globals ////////////////////////////////////////////////////////


#include "assets.c"


void do_assemble(Arena* temp_arena) {
    assembler_init(&assembler, &error_list, &ast, &rom);
    
    Result result = RESULT_ERROR;
    while(result != RESULT_OK) {
        result = assemble(&assembler);
        if(result == RESULT_NEED_FILE) {
            Buffer file_buffer;
            String file_name = assembler_get_file_name(&assembler);
            NamedBuffer dummy = { .name = file_name };
            NamedBuffer* found = named_buffer_map_find(&global_buffer_map, dummy);
            if(found) {
                file_buffer = found->buffer;
            } else {
                Path file;
                path_init(&file, file_name);
                file_buffer  = path_read_file(&file, temp_arena);
            }
            
            assembler_give_buffer(&assembler, file_buffer);
        } else if(result == RESULT_ERROR) {
            break;
        }
    }
}

#include "viewers.c"


void update_and_render() {
    assets_clear();
    smw_clear(&smw);
    
    /* Input */
    int fb_width = 0;
    int fb_height = 0;
    
    glfwPollEvents();
    nk_glfw3_new_frame();

    glfwGetWindowSize(win, &fb_width, &fb_height);

    smw_viewer(ctx, &global_smw_viewer);
    
#if 0
    if(nk_begin(ctx, "Sprites", nk_rect(0, 0, 1200, 905),
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
            c_string_format_unsafe(sprite_name, "%s", sprite_names[level->sprites.data[i].number]);
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
    
    /* c_print_format("length: %i\n", smw.levels[current_level].layer1_objects.length); */
    
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

        enum {
            MAX_VERTEX_BUFFER  = 512 * 1024,
            MAX_ELEMENT_BUFFER = 128 * 1024
        };
        nk_glfw3_render(NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
        glfwSwapBuffers(win);
        //c_print_format("%.2fms\n", ctx->delta_time_seconds * 1000);
    }
}

void error_callback(int error, const char* description)
{
    c_error_format("Error %d: %s\n", error, description);
}

int main(int argc, char** argv)
{

    if(argc != 2 && argc != 3) {
        c_print_format("Usage: mockup FILE [FOLDER]\n"
               "\n"
               "FILE contains a path to the main asm file of a diassembly\n"
               "[FOLDER] is an optional path to a Cartridge folder\n"
            );
        return -1;
    }

    /* Setup win */
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
    {
        c_print_format("Could not create window. Test failed.\n");
        return -1;
    }
    glfwWindowHint(GLFW_RESIZABLE , 1);

    int width = 1280;
    int height = 1024;
    
    win = glfwCreateWindow(width, height, "Mockup", NULL, NULL);
    if (!win)
    {
        c_print_format("Could not create window.\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(win);

    /* Nuklear */
    ctx = nk_glfw3_init(win, NK_GLFW3_INSTALL_CALLBACKS);
    /* Load Fonts: if none of these are loaded a default font will be used  */
    struct nk_font_atlas *atlas;
    nk_glfw3_font_stash_begin(&atlas);
    nk_glfw3_font_stash_end();
    background = nk_rgb(190,205,255);


    Arena arena = arena_create(MB(1024));
    String cartridge_folder_path_name = (argc > 2)
        ? string_from_c_string(argv[2])
        : L("SuperMarioWorld.sfc");
    Path cartridge_folder_path;
    path_init(&cartridge_folder_path, cartridge_folder_path_name);
    Path manifest_path;
    path_init_from_c(&manifest_path, &cartridge_folder_path, "manifest.bml");
    Buffer manifest_buffer = path_read_file(&manifest_path, &arena);

    Wdc65816MapperBuilder rom_builder = { };
    
    char name[256];
    Buffer name_buffer = buffer(name, 256);
    Buffer rom_buffer;
    for(Wdc65816RomLoader loader = wdc65816_rom_loader_begin(&rom_builder,
                                                             string_from_buffer(manifest_buffer));
        wdc65816_rom_loader_end(&loader);
        wdc65816_rom_loader_next(&loader)) {
        Wdc65816MemoryBufferRequest request = wdc65816_rom_loader_get_buffer_request(&loader,
                                                                                     name_buffer);
        Buffer file_buffer = buffer(arena_alloc_array(&arena, request.size, u8), request.size);
        if(string_equal(request.name, L("program.rom")) &&
           request.type == WDC65816_MEMORY_ROM) {
            rom_buffer = file_buffer;
        }
        wdc65816_rom_loader_set_buffer(&loader, file_buffer);
    }

    uint mapper_buffer_size = wdc65816_mapper_get_buffer_size();
    u8* work_buffer = arena_alloc_array(&arena, mapper_buffer_size, u8);
    wdc65816_mapper_init(&rom, &rom_builder, (u8**)work_buffer);

    Path working_dir;
    path_init_working_directory(&working_dir);
    FreeList sentinel;
    free_list_init(&sentinel);
    error_list_init(&error_list, arena_subarena(&arena, MB(10)));

    nuts_global_init();
    ast_init(&ast, &arena);
    
    String file_name = string_from_c_string(argv[1]); 
    
    Parser parser;
    parser_init(&parser, &arena, &sentinel, &error_list, &ast);
    int error_num = 0;
    while(1) {
        Path file;
        path_init(&file, file_name);
        Buffer file_buffer = path_read_file(&file, &arena);
        TokenList token_list;
        //c_print_format("read %.*s\n", file_name.length, file_name.data);
        Text file_text = {
            .buffer = file_buffer,
            .name   = file_name
        };
        Result result = lex(file_text, &token_list, &arena, &error_list, &ast.identifier_map);
        if(result == RESULT_ERROR) {
            for(;error_num < error_list.length; error_num++) {
                describe_error(error_list.errors[error_num]);
            }
        }
        //token_list_print(token_list);
        result = parse(&parser, token_list);
        
        if(result == RESULT_NEED_TOKEN_STREAM) {
            file_name = parser.needed_token_stream_file_name;
            continue;
        } else if(result == RESULT_OK) {
            break;
        } else if(result == RESULT_ERROR) {
            for(;error_num < error_list.length; error_num++) {
                describe_error(error_list.errors[error_num]);
            }
            return 1;
        } else {
            invalid_code_path;
        }
    }

    named_buffer_map_init(&global_buffer_map, 128);
    do_assemble(&arena);
    for(int i = 0; i < error_list.length; i++) {
        describe_error(error_list.errors[i]);
    }

    global_init(&arena);
    smw_init(&smw, &rom_builder, &arena);

    
    while(!glfwWindowShouldClose(win)) {
        update_and_render();
    }


    parser_deinit(&parser);

    Path rom_path;
    path_create_from(&rom_path, &cartridge_folder_path, L("program.rom"));
    path_write_file(&rom_path, rom_buffer);
   
    nk_glfw3_shutdown();
    glfwTerminate();

    return 0;
}
