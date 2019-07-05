#ifndef BASE_GUARD
#define BASE_GUARD

//////////////////
// Set defaults //
//////////////////

#if !defined(LINUX) && !defined(WINDOWS) && !defined(WASM) 
#define LINUX
#endif

#if !defined(ARCH64) && !defined(ARCH32)
#define ARCH64
#endif

//////////////////
// 32 vs 64-bit //
//////////////////

#if defined(ARCH64)
#define type_addr long
#define type_i64  long
typedef int       wchar;
#elif defined(ARCH32)
#define type_addr int
#define type_i64  long long
typedef unsigned short wchar;
#endif

typedef signed   char     i8;
typedef unsigned char     u8;
typedef signed   short    i16;
typedef unsigned short    u16;
typedef signed   int      i32;
typedef unsigned int      u32;
typedef unsigned int      b32;
typedef signed   type_i64 i64;
typedef unsigned type_i64 u64;
typedef unsigned type_i64 b64;
typedef          float    f32;
typedef          double   f64;

typedef unsigned long     ulong;
typedef unsigned int      uint;
typedef unsigned short    ushort;
typedef unsigned char     uchar;

typedef          type_addr iptr;
typedef unsigned type_addr uptr;

#define SCHAR_MIN (-128)
#define SCHAR_MAX 127
#define UCHAR_MAX 255
#define SHRT_MIN  (-1-0x7fff)
#define SHRT_MAX  0x7fff
#define USHRT_MAX 0xffff
#define INT_MIN  (-1-0x7fffffff)
#define INT_MAX  0x7fffffff
#define UINT_MAX 0xffffffffU
#define LONG_MIN (-LONG_MAX-1)
#define ULONG_MAX (2UL*LONG_MAX+1)
#define LLONG_MIN (-LLONG_MAX-1)
#define ULLONG_MAX (2ULL*LLONG_MAX+1)

#define NULL ((void*)0)

#define STR_(x) #x
#define STR(x) STR_(x)
#define CODE_LOCATION STR(__FILE__) ": " STR(__LINE__) 

////////////////////////////////
// Platform dependent structs //
////////////////////////////////

#include "platform_linux.h"
#include "platform_windows.h"
#include "platform_wasm.h"



#ifndef NDEBUG
#define assert(x) do { if(!(x)) {                                               \
            error(L("Assertion (" #x ") at " CODE_LOCATION ".\n"));             \
            debug_break;} while(0)
#else
#define assert(x)
#endif
#define unused(x) ((void)x);
#define invalid_code_path assert(!"Invalid code path")
#define not_implemented assert(!"Not implemented")
#define invalid_default_case default: invalid_code_path; break

#define alignof(x) __alignof(x)
#if !defined(offsetof)
#define offsetof(st, m) ((uptr)&(((st*)0)->m))
#endif
//#define NULL ((void*)0)
#define true 1
#define false 0

#define KB(x) (x * 1024ull)
#define MB(x) (x * 1024ull * 1024ull)
#define GB(x) (x * 1024ull * 1024ull * 1024ull)

#define array_length(array) (sizeof(array)/sizeof(array[0]))
#undef max
#undef min
#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))
#define clamp(x, a, b) min(b, max(a, x))
#define ptr_add(p,n) ((void*)((char*)(p) + (n)))
#define ptr_sub(p,n) ((void*)((char*)(p) - (n)))
#define ptr_diff(p,q) ((uptr)((char*)(p) - (char*)(q)))

////////////////////
// Base interface //
////////////////////

#if !defined(WINDOWS)
void exit(int status);
#endif
int  raise_signal(int signal);

void* get_memory(ulong size);

ulong size_to_pages(ulong size);

typedef struct {
    void* begin;
    void* end;
} Buffer;

Buffer buffer(void* data, ulong length);
ulong  buffer_length(Buffer buffer);
void   zero_buffer(Buffer buffer);
void   copy_buffer(Buffer src, Buffer dest);

void* c_memset(void* s, int c, ulong n);
void* c_memcpy(void* dest, const void *src, ulong n);

typedef struct {
    Buffer   buffer;
    void*    current;
#if DEBUG
    char*    name_;
#endif
} Arena;

Arena   arena_create(ulong size);
Arena   arena_create_buffer(Buffer buffer);
void    arena_clear(Arena* a);
Arena   arena_subarena(Arena* a, ulong size);
void*   arena_alloc(Arena* a, ulong size, ulong align);
#define arena_alloc_type(a,type) ((type*)arena_alloc((a), sizeof(type), alignof(type)))
#define arena_alloc_array(a,n,type) (type*)arena_alloc((a), (n)*sizeof(type), alignof(type))
#define arena_array_size(a,type) (ptr_diff((a)->current, (a)->buffer.begin) / sizeof(type))
#define arena_array_capacity(a,type) (ptr_diff((a)->buffer.end, (a)->buffer.begin) / sizeof(type))
#define arena_array_last(a,type) ((a)->current != (a)->buffer.begin ? ((type*)(a)->current) - 1 : NULL)
Buffer  arena_alloc_buffer(Arena* a, ulong size, ulong align);
void*   arena_dealloc(Arena* a, ulong size, ulong align);
#define arena_dealloc_type(a,type) ((type*)arena_dealloc((a), sizeof(type), alignof(type)))

typedef struct {
    Arena* arena;
    void*  old_current;
} Temp;

Temp temp_begin(Arena* a);
void temp_end(Temp tmp);

typedef struct FreeListEntry_ {
    struct FreeListEntry_* next;
    struct FreeListEntry_* prev;
    void* ptr;
    ulong num_pages;
} FreeListEntry;

typedef struct {
    FreeListEntry sentinel;
    Arena arena;
    ulong page_size;
} FreeList;

void    free_list_init(FreeList* list, Arena arena, ulong page_size);
void*   free_list_alloc(FreeList* free_list_sentinel, ulong size);
Buffer  free_list_alloc_buffer(FreeList* free_list, ulong size);
#define free_list_alloc_array(f,n,type) (type*)free_list_alloc((f), (n)*sizeof(type))
void    free_list_dealloc(FreeList* free_list, void* ptr, ulong size);
void    free_list_dealloc_buffer(FreeList* free_list, Buffer buffer);
#define free_list_dealloc_array(f,p,n,type) free_list_dealloc((f), (p), (n)*sizeof(type))

typedef struct {
    char* data;
    ulong length;
} String;

#define L(cstr) (String){.data = cstr, .length = array_length(cstr) - 1 }

String string(char* data, ulong length);
String string_from_buffer(Buffer buffer);
Buffer buffer_from_string(String string);
b32 string_equal(String s1, String s2);
String string_from_c_string(char* buf);
String string_trim_spaces(String str);
u64 string_to_u64(String s);
u64 string_to_u64_base(String s, int base);
void upcase_string(String s);
void lowcase_string(String s);

String string_format(Buffer buffer, String format, ...);
int    string_format_list(Buffer buffer, String format, VariableArgumentList arg_list);
int    c_string_format(char* buffer, int length, char* format, ...);
int    c_string_format_list(char* buffer, int length, char* format, VariableArgumentList arg_list);
int    c_string_format_unsafe(char* buffer, char* format, ...);

typedef struct {
    Buffer buffer;
    int length;
} StringBuilder;

StringBuilder string_builder(Buffer buffer);
void string_builder_append(StringBuilder* builder, String string);
String string_from_builder(StringBuilder builder);

ulong c_string_length(const char* s);
int   c_string_equal(const char* restrict s1, const char* restrict s2);
void  c_string_copy(char* restrict dest, const char* restrict src, uint n);

#define string_split_foreach(s, t, c)                                           \
    String t = { .data = (s).data };                                            \
    for(char* macro_c_ = (s).data; macro_c_ <= (s).data + (s).length;           \
        t.data = (*macro_c_ == c) ? macro_c_ + 1 : t.data,                      \
            macro_c_++, t.length = macro_c_ - t.data)                           \
        if(*macro_c_ == c || macro_c_ == (s).data + (s).length)                 \

void    path_init(Path* path, String name);
void    path_init_c(Path* path, char* name);
void    path_init_from_c(Path* path, Path* dir, char* name);
void    path_init_from(Path* path, Path* dir, String name);
void    path_init_working_directory(Path* path);
void    path_create(Path* path, String name);
void    path_create_c(Path* path, char* name);
void    path_create_from_c(Path* path, Path* dir, char* name);
void    path_create_from(Path* path, Path* dir, String name);
void    path_copy(Path* orig, Path* copy);
b32     path_can_read(Path* path);
Buffer  path_read_file(Path* path, Arena* arena);
Buffer  path_read_file_aligned(Path* path, Arena* arena, int alignment);
b32     path_can_write(Path* path);
void    path_write_file(Path* path, Buffer buffer);
void    path_close(Path* path);
void    path_navigate(Path* path, String name);
void    path_navigate_c(Path* path, char* name);
ulong   path_get_file_size(Path* path);
int     path_is_directory(Path* path);
String  path_get_name(Path* path, Arena* arena); //TODO
String  path_get_base_name(Path* path, Arena* arena); //TODO
DirIter dir_iter_begin(Path* path);
int     dir_iter_next(DirIter* dir_iter);
void    dir_iter_end(DirIter* dir_iter);

void print(String s);
void c_print(char* c);
void c_print_format(char* format, ...);

void error(String s);
void c_error(char* c);
void c_error_format(char* format, ...);

/*******
 * BML *
 *******/

typedef enum {
    BML_NODE,
    BML_NODE_END,
    BML_NODE_EOF,
    BML_NODE_ERROR,
    BML_NODE_ERROR_INVALID_NODE_NAME,
} BmlNodeType;

typedef struct {
    BmlNodeType type;
    String name;
    String value;
} BmlNode;

typedef struct {
    Buffer  buffer;
    char*   pos;
    int     indent;
    int     indent_index;
    int     indent_levels[16];
    int     state;
} BmlParser;

void    bml_parser_init(BmlParser* parser, Buffer buffer);
BmlNode bml_parse(BmlParser* parser);
BmlNode bml_skip(BmlParser* parser);

typedef struct {
    u32  value;
    uint length;
} Hash;

Hash hash_murmur3_begin();
void hash_murmur3_add_buffer(Hash* hash, Buffer buffer);
void hash_murmur3_add(Hash* hash, void* data, uint data_length);
void hash_murmur3_add_u32(Hash* hash, u32 k);
u32  hash_murmur3_end(Hash* hash);
u32  hash_murmur3_c(void* data, uint data_length);
u32  hash_murmur3_string(String str);

/* Containers:

   Stack:
 * define_stack(name, prefix, type)
 * implement_stack(name, prefix, type)
 * generate_stack(name, prefix, type)
   * extern void prefix##_init(name* s, FreeList* sentinel, int size);
   * extern void prefix##_deinit(name* s, FreeList* sentinel);
   * extern type* prefix##_push(name* s, type data);
   * extern void prefix##_reserve(name* s);
   * extern type* prefix##_pop(name* s);
   * extern type* prefix##_top(name* s);
   * extern b32 prefix##_is_empty(name* s);

   TODO: Add other container
 */
#include "containers.h"

typedef struct {
    String name;
    Buffer buffer;
} VFSEntry;

u32 vfs_entry_hash(VFSEntry v);
u32 vfs_entry_equal(VFSEntry v1, VFSEntry v2);

define_hashmap(VFS, vfs, VFSEntry);

void vfs_add_dir(VFS* vfs, Path* path, Arena* arena);

/********
 * Math *
 ********/

int u32_log2(u32 a);

/*******
 * GUI *
 *******/
#ifdef BASE_GUI

#define GUI_COMMANDLIST_SIZE     (1024 * 256)
#define GUI_ROOTLIST_SIZE        32
#define GUI_CONTAINERSTACK_SIZE  32
#define GUI_CLIPSTACK_SIZE       32
#define GUI_IDSTACK_SIZE         32
#define GUI_LAYOUTSTACK_SIZE     16
#define GUI_MAX_WIDTHS           16
#define GUI_MAX_FMT              127

#define mu_stack(T, n)           struct { int idx; T items[n]; }

typedef enum {
    GUI_CLIP_NONE,
    GUI_CLIP_PART,
    GUI_CLIP_ALL
} GuiClipType;

typedef enum {
    GUI_COMMAND_JUMP = 1,
    GUI_COMMAND_CLIP,
    GUI_COMMAND_RECT,
    GUI_COMMAND_TEXT,
    GUI_COMMAND_ICON,
    GUI_COMMAND_IMAGE,
    GUI_COMMAND_MAX
} GuiCommandType;

typedef enum {
    GUI_COLOR_TEXT,
    GUI_COLOR_BORDER,
    GUI_COLOR_WINDOWBG,
    GUI_COLOR_TITLEBG,
    GUI_COLOR_TITLETEXT,
    GUI_COLOR_PANELBG,
    GUI_COLOR_BUTTON,
    GUI_COLOR_BUTTONHOVER,
    GUI_COLOR_BUTTONFOCUS,
    GUI_COLOR_BASE,
    GUI_COLOR_BASEHOVER,
    GUI_COLOR_BASEFOCUS,
    GUI_COLOR_SCROLLBASE,
    GUI_COLOR_SCROLLTHUMB,
    GUI_COLOR_MAX
} GuiColorType;

typedef enum {
    GUI_ICON_CLOSE       = 1,
    GUI_ICON_RESIZE,
    GUI_ICON_CHECK,
    GUI_ICON_COLLAPSED,
    GUI_ICON_EXPANDED,
    GUI_ICON_MAX
} GuiIconType;

typedef enum {
    GUI_RES_ACTIVE       = (1 << 0),
    GUI_RES_SUBMIT       = (1 << 1),
    GUI_RES_CHANGE       = (1 << 2)
} GuiResultType;

typedef enum {
    GUI_OPT_ALIGNCENTER  = (1 << 0),
    GUI_OPT_ALIGNRIGHT   = (1 << 1),
    GUI_OPT_NOINTERACT   = (1 << 2),
    GUI_OPT_NOFRAME      = (1 << 3),
    GUI_OPT_NORESIZE     = (1 << 4),
    GUI_OPT_NOSCROLL     = (1 << 5),
    GUI_OPT_NOCLOSE      = (1 << 6),
    GUI_OPT_NOTITLE      = (1 << 7),
    GUI_OPT_HOLDFOCUS    = (1 << 8),
    GUI_OPT_AUTOSIZE     = (1 << 9),
    GUI_OPT_POPUP        = (1 << 10),
    GUI_OPT_CLOSED       = (1 << 11)
} GuiOptionType;

typedef enum {
    GUI_MOUSE_BUTTON_LEFT   = (1 << 0),
    GUI_MOUSE_BUTTON_RIGHT  = (1 << 1),
    GUI_MOUSE_BUTTON_MIDDLE = (1 << 2)
} GuiMouseButton;

typedef enum {
    GUI_KEY_SHIFT        = (1 << 0),
    GUI_KEY_CTRL         = (1 << 1),
    GUI_KEY_ALT          = (1 << 2),
    GUI_KEY_BACKSPACE    = (1 << 3),
    GUI_KEY_RETURN       = (1 << 4)
} GuiKeyboard;

typedef uint  GuiId;
typedef f32   GuiReal;
typedef void* GuiFont;
typedef u32   GuiTexture;

typedef struct {
    int x, y;
} Vec2I;
typedef struct {
    u8 r, g, b, a;
} GuiColor;

typedef struct {
    f32 x, y, w, h;
} Rect;

typedef struct {
    int x, y, w, h;
} RectI;

Rect  rect(f32 x, f32 y, f32 w, f32 h);
RectI recti(int x, int y, int w, int h);
b32   rect_equal(Rect r1, Rect r2);
Vec2I vec2i(int x, int y);

typedef struct { int type, size; } GuiBaseCommand;
typedef struct { GuiBaseCommand base; void *dst; } GuiJumpCommand;
typedef struct { GuiBaseCommand base; RectI rect; } GuiClipCommand;
typedef struct { GuiBaseCommand base; RectI rect; GuiColor color; } GuiRectCommand;
typedef struct { GuiBaseCommand base; GuiFont font; Vec2I pos; GuiColor color; char str[1]; } GuiTextCommand;
typedef struct { GuiBaseCommand base; RectI rect; GuiIconType id; GuiColor color; } GuiIconCommand;
typedef struct { GuiBaseCommand base; RectI dest; Rect src; GuiTexture texture; } GuiImageCommand;

typedef union {
    GuiCommandType  type;
    GuiBaseCommand  base;
    GuiJumpCommand  jump;
    GuiClipCommand  clip;
    GuiRectCommand  rect;
    GuiTextCommand  text;
    GuiIconCommand  icon;
    GuiImageCommand image;
} GuiCommand;


typedef struct {
    RectI body;
    RectI next;
    Vec2I position;
    Vec2I size;
    Vec2I max;
    int widths[GUI_MAX_WIDTHS];
    int items;
    int row_index;
    int next_row;
    int next_type;
    int indent;
} GuiLayout;


typedef struct {
    GuiCommand *head, *tail;
    RectI rect;
    RectI body;
    Vec2I content_size;
    Vec2I scroll;
    int inited;
    int zindex;
    int open;
} GuiContainer;


typedef struct {
    GuiFont font;
    Vec2I size;
    int padding;
    int spacing;
    int indent;
    int title_height;
    int footer_height;
    int scrollbar_size;
    int thumb_size;
    GuiColor colors[GUI_COLOR_MAX];
} GuiStyle;

typedef struct {
    float position[2];
    float uv[2];
    u8    color[4];
} GuiVertex;

typedef struct {
    uint vbo_id, vao_id, ebo_id;
    uint program_id;
    uint vertex_shader_id;
    uint fragment_shader_id;
    int position_location;
    int uv_location;
    int color_location;
    int texture_location;
    int projection_location;

    GuiVertex* vertex_buffer;
    ushort* elements_buffer;
    int vertex_buffer_length;
    int elements_buffer_length;
} GuiOpenGLState;

b32  recti_equal(RectI r1, RectI r2);

typedef struct {
    uint       elem_count;
    Rect       clip_rect;
    GuiTexture texture;
} GuiDrawCommand;

typedef struct {
    Arena vertices_arena;
    Arena elements_arena;
    Arena draw_command_arena;
} GuiCommandBuffer;

typedef struct {
    ushort x0, y0, x1, y1;
    float  xoff, yoff, xadvance;
    float  xoff2, yoff2;
} GuiPackedChar;

typedef struct {
    GuiTexture texture;
    Rect       rect;
} GuiImage;

typedef struct {
    GuiTexture texture;
    u32* data;
    int  width;
    int  height;
} GuiImageData;

typedef struct GuiWindow_ {
    int width;
    int height;
    struct GuiWindow_* subwindows;
    float* sizes;
} GuiWindow;

typedef struct GuiContext_{
    PlatformWindow   window;
    GuiOpenGLState   gl_state;
    GuiCommandBuffer cmd_buffer;
    GuiTexture       font_texture;
    b32              running;
    
    int width;
    int height;
    int framebuffer_width;
    int framebuffer_height;
    float fb_scale_x;
    float fb_scale_y;

    /* core state */
    GuiStyle _style;
    GuiStyle *style;
    GuiId hover;
    GuiId focus;
    GuiId last_id;
    RectI last_rect;
    int last_zindex;
    int updated_focus;
    GuiContainer *hover_root;
    GuiContainer *last_hover_root;
    GuiContainer *scroll_target;
    char number_buf[GUI_MAX_FMT];
    GuiId number_editing;
    /* stacks */
    mu_stack(char, GUI_COMMANDLIST_SIZE) command_list;
    mu_stack(GuiContainer*, GUI_ROOTLIST_SIZE) root_list;
    mu_stack(GuiContainer*, GUI_CONTAINERSTACK_SIZE) container_stack;
    mu_stack(RectI, GUI_CLIPSTACK_SIZE) clip_stack;
    mu_stack(GuiId, GUI_IDSTACK_SIZE) id_stack;
    mu_stack(GuiLayout, GUI_LAYOUTSTACK_SIZE) layout_stack;
    /* input state */
    Vec2I mouse_pos;
    Vec2I last_mouse_pos;
    Vec2I mouse_delta;
    int mouse_wheel;
    GuiMouseButton mouse_down;
    GuiMouseButton mouse_pressed;
    GuiKeyboard key_down;
    GuiKeyboard key_pressed;
    char text_input[32];
} GuiContext;

GuiColor gui_color(int r, int g, int b, int a);

void mu_set_focus(GuiContext *ctx, GuiId id);
GuiId mu_get_id(GuiContext *ctx, const void *data, int size);
void mu_push_id(GuiContext *ctx, const void *data, int size);
void mu_pop_id(GuiContext *ctx);
void mu_push_clip_rect(GuiContext *ctx, RectI rect);
void mu_pop_clip_rect(GuiContext *ctx);
RectI mu_get_clip_rect(GuiContext *ctx);
int mu_check_clip(GuiContext *ctx, RectI r);
GuiContainer* mu_get_container(GuiContext *ctx);
void mu_init_window(GuiContext *ctx, GuiContainer *cnt, GuiOptionType opt);
void mu_bring_to_front(GuiContext *ctx, GuiContainer *cnt);

void mu_input_mousemove(GuiContext *ctx, int x, int y);
void mu_input_mousedown(GuiContext *ctx, int x, int y, int btn);
void mu_input_mouseup(GuiContext *ctx, int x, int y, int btn);
void mu_input_mousewheel(GuiContext *ctx, int y);
void mu_input_keydown(GuiContext *ctx, GuiKeyboard key);
void mu_input_key(GuiContext *ctx, GuiKeyboard key, int down);
void mu_input_keyup(GuiContext *ctx, GuiKeyboard key);
void mu_input_text(GuiContext *ctx, const char *text);

GuiCommand* mu_push_command(GuiContext *ctx, int type, int size);
int mu_next_command(GuiContext *ctx, GuiCommand **cmd);
void mu_set_clip(GuiContext *ctx, RectI rect);
void mu_draw_rect(GuiContext *ctx, RectI rect, GuiColor color);
void mu_draw_box(GuiContext *ctx, RectI rect, GuiColor color);
void mu_draw_text(GuiContext *ctx, GuiFont font, const char *str, int len, Vec2I pos, GuiColor color);
void mu_draw_icon(GuiContext *ctx, GuiIconType id, RectI rect, GuiColor color);

void mu_layout_row(GuiContext *ctx, int items, const int *widths, int height);
void mu_layout_width(GuiContext *ctx, int width);
void mu_layout_height(GuiContext *ctx, int height);
void mu_layout_begin_column(GuiContext *ctx);
void mu_layout_end_column(GuiContext *ctx);
void mu_layout_set_next(GuiContext *ctx, RectI r, int relative);
RectI mu_layout_next(GuiContext *ctx);

void mu_draw_control_frame(GuiContext *ctx, GuiId id, RectI rect, int colorid, GuiOptionType opt);
void mu_draw_control_text(GuiContext *ctx, const char *str, RectI rect, int colorid, GuiOptionType opt);
int mu_mouse_over(GuiContext *ctx, RectI rect);
void mu_update_control(GuiContext *ctx, GuiId id, RectI rect, GuiOptionType opt);

void mu_image(GuiContext *ctx, GuiImage image);
void mu_text(GuiContext *ctx, const char *text);
void mu_label(GuiContext *ctx, const char *text);
int mu_button_ex(GuiContext *ctx, const char *label, int icon, GuiOptionType opt);
int mu_button(GuiContext *ctx, const char *label);
int mu_checkbox(GuiContext *ctx, int *state, const char *label);
int mu_textbox_raw(GuiContext *ctx, char *buf, int bufsz, GuiId id, RectI r, GuiOptionType opt);
int mu_textbox_ex(GuiContext *ctx, char *buf, int bufsz, GuiOptionType opt);
int mu_textbox(GuiContext *ctx, char *buf, int bufsz);
int mu_slider_ex(GuiContext *ctx, GuiReal *value, GuiReal low, GuiReal high, GuiReal step, const char *fmt, GuiOptionType opt);
int mu_slider(GuiContext *ctx, GuiReal *value, GuiReal low, GuiReal high);
int mu_number_ex(GuiContext *ctx, GuiReal *value, GuiReal step, const char *fmt, GuiOptionType opt);
int mu_number(GuiContext *ctx, GuiReal *value, GuiReal step);
int mu_header(GuiContext *ctx, int *state, const char *label);
int mu_begin_treenode(GuiContext *ctx, int *state, const char *label);
void mu_end_treenode(GuiContext *ctx);
int mu_begin_window_ex(GuiContext *ctx, GuiContainer *cnt, const char *title, GuiOptionType opt);
int mu_begin_window(GuiContext *ctx, GuiContainer *cnt, const char *title);
void mu_end_window(GuiContext *ctx);
void mu_open_popup(GuiContext *ctx, GuiContainer *cnt);
int mu_begin_popup(GuiContext *ctx, GuiContainer *cnt);
void mu_end_popup(GuiContext *ctx);
void mu_begin_panel_ex(GuiContext *ctx, GuiContainer *cnt, GuiOptionType opt);
void mu_begin_panel(GuiContext *ctx, GuiContainer *cnt);
void mu_end_panel(GuiContext *ctx);

void gui_init(GuiContext* context, Arena* arena);
void gui_deinit(GuiContext* context);

void gui_texture_init_nearest(GuiTexture* texture);
void gui_texture_init_linear(GuiTexture* texture);
void gui_texture_update(GuiTexture* texture, void* data, int width, int height);

float gui_text_width(String s);
void gui_draw_text(GuiContext* context, float x, float y, String s, GuiColor color);
void gui_draw_image(GuiContext* context, Rect dest, Rect src, GuiTexture texture);
void gui_add_draw_command(GuiContext* context,
                          GuiVertex* verts, int verts_length,
                          ushort* elems, int elems_length,
                          Rect clip_rect,
                          GuiTexture texture);
void gui_draw_quad_color(GuiContext* context, Rect src, Rect dest, GuiTexture tex, GuiColor color);
void gui_draw_rect_color(GuiContext* context, Rect dest, GuiColor color);
void gui_draw_icon(GuiContext* context, unsigned int icon, Rect dest, GuiColor color);
void gui_add_clip_rect(GuiContext* context, Rect clip);
void gui_begin_frame(GuiContext* context);
void gui_end_frame(GuiContext* context);
void gui_begin_window(GuiContext* context, String name);
void gui_begin_end(GuiContext* context);
void gui_set_window_name(GuiContext* context, String window_name);
void gui_get_window_size(GuiContext* context, int* width, int* height);
void gui_get_framebuffer_size(GuiContext* context, int* width, int* height);
b32  gui_should_continue(GuiContext* context);
void gui_image_data_init(GuiImageData* image);
void gui_image_data_update(GuiImageData* image, int size_x, int size_y, u32* data);
GuiImage gui_image(GuiImageData image_data);
GuiImage gui_sub_image(GuiImageData image_data, RectI rect);


//void gui_get_cursor_position(GuiContext* context, int* x, int* y);
//KeyState gui_get_mouse_button(GuiContext* context, int button);
//KeyState gui_get_key(GuiContext* context, KeyboardKey key);

#endif //BASE_GUI

#endif //BASE_GUARD

#ifdef BASE_IMPLEMENTATION
#ifndef BASE_IMPLEMENTATION_GUARD
#define BASE_IMPLEMENTATION_GUARD

///////////////////////////////////////
// Platform dependent implementation //
///////////////////////////////////////

#ifdef BASE_GUI
#include "font.c"
#include "gui.c"
#endif

#include "platform_linux.c"
#include "platform_windows.c"
#include "platform_wasm.c"

#include "memory.c"
#include "string.c"


#include "bml.c"

implement_hashmap(StringMap, string_map, String, hash_murmur3_string, string_equal);

Hash hash_murmur3_begin() {
    return (Hash) { };
}

void hash_murmur3_add(Hash* hash, void* data, uint data_length) {
    const u32 c1 = 0xcc9e2d51;
    const u32 c2 = 0x1b873593;

    const int  blocks_length = data_length / 4;
    const u32* blocks = (const u32*)(data);
    const u8*  tail = (const u8*)(data + (blocks_length * 4));

    int i;
    u32 k;
    for (i = 0; i < blocks_length; i++) {
        k = blocks[i];

        k *= c1;
        k = (k << 15) | (k >> (32 - 15));
        k *= c2;

        hash->value ^= k;
        hash->value = (hash->value << 13) | (hash->value >> (32 - 13));
        hash->value = (hash->value * 5) + 0xe6546b64;
    }

    k = 0;
    switch (data_length & 3) {
    case 3:
        k ^= tail[2] << 16;
        fallthrough;
        
    case 2:
        k ^= tail[1] << 8;
        fallthrough;

    case 1:
        k ^= tail[0];
        k *= c1;
        k = (k << 15) | (k >> (32 - 15));
        k *= c2;
        hash->value ^= k;
    };
}

void hash_murmur3_add_u32(Hash* hash, u32 k) {
    const u32 c1 = 0xcc9e2d51;
    const u32 c2 = 0x1b873593;

    k *= c1;
    k = (k << 15) | (k >> (32 - 15));
    k *= c2;

    hash->value ^= k;
    hash->value = (hash->value << 13) | (hash->value >> (32 - 13));
    hash->value = (hash->value * 5) + 0xe6546b64;

    hash->length += 4;
}


u32 hash_murmur3_end(Hash* hash) {
    hash->value ^= hash->length;

    hash->value ^= hash->value >> 16;
    hash->value *= 0x85ebca6b;
    hash->value ^= hash->value >> 13;
    hash->value *= 0xc2b2ae35;
    hash->value ^= hash->value >> 16;

    return hash->value;
}

u32 hash_murmur3(void* data, uint data_length) {
    if(data == NULL || data_length == 0)
        return 0;
    Hash hash = hash_murmur3_begin();
    hash_murmur3_add(&hash, data, data_length);
    return hash_murmur3_end(&hash);
}

u32 hash_murmur3_string(String str) {
    if(str.data == NULL || str.length == 0)
        return 0;
    Hash hash = hash_murmur3_begin();
    hash_murmur3_add(&hash, str.data, str.length);
    return hash_murmur3_end(&hash);
}

u32 vfs_entry_hash(VFSEntry v) {
    return hash_murmur3_string(v.name);
}
u32 vfs_entry_equal(VFSEntry v1, VFSEntry v2) {
    return string_equal(v1.name, v2.name);
}

implement_hashmap(VFS, vfs, VFSEntry, vfs_entry_hash, vfs_entry_equal);


void vfs_add_dir(VFS* vfs, Path* path, Arena* arena) {
    DirIter dir_iter = dir_iter_begin(path);
    while(dir_iter_next(&dir_iter)) {
        VFSEntry entry = { };
        entry.name   = path_get_base_name(&dir_iter.path, arena);
        entry.buffer = path_read_file(&dir_iter.path, arena);
        vfs_insert(vfs, entry);
    }
    dir_iter_end(&dir_iter);
}

void c_print_format(char* format, ...) {
    char raw_buffer[4096];
    Buffer buf = buffer(raw_buffer, sizeof(raw_buffer));
    VariableArgumentList arg_list;
    variable_argument_begin(arg_list, format);
    int length = string_format_list(buf, string_from_c_string(format), arg_list);
    variable_argument_end(arg_list);
    
    print(string(raw_buffer, length));
}

void c_error_format(char* format, ...) {
    char raw_buffer[4096];
    Buffer buf = buffer(raw_buffer, sizeof(raw_buffer));
    
    VariableArgumentList arg_list;
    variable_argument_begin(arg_list, format);
    int length = string_format_list(buf, string_from_c_string(format), arg_list);
    variable_argument_end(arg_list);
    
    error(string(raw_buffer, length));
}

int u32_log2(u32 n) {
    int i = 0;
    if (n >= (1 << 16)) { i += 16; n >>= 16; }
    if (n >= (1 <<  8)) { i +=  8; n >>=  8; }
    if (n >= (1 <<  4)) { i +=  4; n >>=  4; }
    if (n >= (1 <<  2)) { i +=  2; n >>=  2; }
    if (n >= (1 <<  1)) { i +=  1; n >>=  1; }
    return i;
}


#endif //BASE_IMPLEMENTATION_GUARD
#endif //BASE_IMPLEMENTATION
