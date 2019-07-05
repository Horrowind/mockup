Rect rect(float x, float y, float w, float h) {
    return (Rect){ .x = x, .y = y, .w = w, .h = h };
}

b32 rect_equal(Rect r1, Rect r2) {
    return r1.x == r2.x && r1.y == r2.y && r1.w == r2.w && r1.h == r2.h;
}

typedef int   GLInt;
typedef uint  GLUInt;
typedef int   GLSizeI;
typedef int   GLEnum;
typedef iptr  GLIntPtr;
typedef iptr  GLSizeIPtr;
typedef uchar GLBoolean;
typedef void  GLVoid;
typedef float GLFloat;
typedef char  GLChar;
typedef float GLClampFloat;
typedef uint  GLBitfield;
typedef uchar GLUByte;
typedef char  GLChar;

/* GL_ARB_vertex_buffer_object */
OPENGL_DEF(gl_gen_buffers, glGenBuffers, void, GLSizeI, GLUInt*);
OPENGL_DEF(gl_bind_buffer, glBindBuffer, void, GLEnum, GLUInt);
OPENGL_DEF(gl_buffer_data, glBufferData, void, GLEnum, GLSizeIPtr, const GLVoid*, GLEnum);
OPENGL_DEF(gl_buffer_sub_data, glBufferSubData, void, GLEnum, GLIntPtr, GLSizeIPtr, const GLVoid*);
OPENGL_DEF(gl_map_buffer, glMapBuffer, void*, GLEnum, GLEnum);
OPENGL_DEF(gl_unmap_buffer, glUnmapBuffer, GLBoolean, GLEnum);
OPENGL_DEF(gl_delete_buffers, glDeleteBuffers, void, GLSizeI, GLUInt*);
/* GL_ARB_vertex_array_object */
OPENGL_DEF(gl_gen_vertex_arrays, glGenVertexArrays, void, GLSizeI, GLUInt*);
OPENGL_DEF(gl_bind_vertex_array, glBindVertexArray, void, GLUInt);
OPENGL_DEF(gl_delete_vertex_arrays, glDeleteVertexArrays, void, GLSizeI, const GLUInt*);
/* GL_ARB_vertex_program / GL_ARB_fragment_program */
OPENGL_DEF(gl_vertex_attrib_pointer, glVertexAttribPointer, void, GLUInt, GLInt, GLEnum, GLBoolean, GLSizeI, const GLVoid*);
OPENGL_DEF(gl_enable_vertex_attrib_array, glEnableVertexAttribArray, void, GLUInt);
OPENGL_DEF(gl_disable_vertex_attrib_array, glDisablleVertexAttribArray, void, GLUInt);
/* GLSL/OpenGL 2.0 core */
OPENGL_DEF(gl_create_shader, glCreateShader, GLUInt, GLEnum);
OPENGL_DEF(gl_shader_source, glShaderSource, void, GLUInt, GLSizeI, const GLChar**, const GLInt*);
OPENGL_DEF(gl_compile_shader, glCompileShader, void, GLUInt);
OPENGL_DEF(gl_get_shader_int_vector, glGetShaderiv, void, GLUInt, GLEnum, GLInt*);
OPENGL_DEF(gl_get_shader_info_log, glGetShaderInfoLog, void, GLUInt, GLSizeI, GLSizeI*, GLChar*);
OPENGL_DEF(gl_delete_shader, glDeleteShader, void, GLUInt);
OPENGL_DEF(gl_create_program, glCreateProgram, GLUInt, void);
OPENGL_DEF(gl_attach_shader, glAttachShader, void, GLUInt, GLUInt);
OPENGL_DEF(gl_detach_shader, glDetachShader, void, GLUInt, GLUInt);
OPENGL_DEF(gl_link_program, glLinkProgram, void, GLUInt);
OPENGL_DEF(gl_use_program, glUseProgram, void, GLUInt);
OPENGL_DEF(gl_get_program_int_vector, glGetProgramiv, void, GLUInt, GLEnum, GLInt*);
OPENGL_DEF(gl_get_program_info_log, glGetProgramInfoLog, void, GLUInt, GLSizeI, GLSizeI*, GLChar*);
OPENGL_DEF(gl_delete_program, glDeleteProgram, void, GLUInt);
OPENGL_DEF(gl_get_uniform_location, glGetUniformLocation, GLInt, GLUInt, const GLChar*);
OPENGL_DEF(gl_get_attrib_location, glGetAttribLocation, GLInt, GLUInt, const GLChar*);
OPENGL_DEF(gl_uniform1_int, glUniform1i, void, GLInt, GLInt);
//OPENGL_DEF(gl_uniform1f, void, GLInt, GLFloat);
//OPENGL_DEF(gl_uniform_matrix_3fv, void, GLInt, GLSizeI, GLBoolean, const GLFloat*);
OPENGL_DEF(gl_uniform_matrix_4fv, glUniformMatrix4fv, void, GLInt, GLSizeI, GLBoolean, const GLFloat*);
//OPENGL_DEF(gl_get_proc_address, void*, const GLUByte*);                

OPENGL_CORE_DEF(gl_gen_textures, glGenTextures, void, GLSizeI, GLUInt*);
OPENGL_CORE_DEF(gl_delete_textures, glDeleteTextures, void, GLSizeI, const GLUInt*);
OPENGL_CORE_DEF(gl_bind_texture, glBindTexture, void, GLEnum, GLUInt);
OPENGL_CORE_DEF(gl_enable, glEnable, void, GLEnum);
OPENGL_CORE_DEF(gl_tex_parameter_int, glTexParameteri, void, GLEnum, GLEnum, GLInt);                
OPENGL_CORE_DEF(gl_tex_image_2d, glTexImage2D, void, GLEnum, GLInt, GLInt, GLSizeI, GLSizeI, GLInt, GLEnum, GLEnum, const GLVoid*);
OPENGL_CORE_DEF(gl_blend_equation, glBlendEquation, void, GLEnum);
OPENGL_CORE_DEF(gl_blend_func, glBlendFunc, void, GLEnum, GLEnum);
OPENGL_CORE_DEF(gl_disable, glDisable, void,  GLEnum);
OPENGL_CORE_DEF(gl_active_texture, glActiveTexture, void, GLEnum);
OPENGL_CORE_DEF(gl_viewport, glViewport, void, GLInt, GLInt, GLSizeI, GLSizeI);
OPENGL_CORE_DEF(gl_scissor, glScissor, void, GLInt, GLInt, GLSizeI, GLSizeI);
OPENGL_CORE_DEF(gl_clear_color, glClearColor, void, GLClampFloat, GLClampFloat, GLClampFloat, GLClampFloat);
OPENGL_CORE_DEF(gl_clear, glClear, void, GLBitfield);
OPENGL_CORE_DEF(gl_draw_elements, glDrawElements, void, GLEnum, GLSizeI, GLEnum, const GLVoid*);
OPENGL_CORE_DEF(gl_get_string, glGetString, const GLUByte*, GLEnum);
//OPENGL_CORE_DEF(gl_get_string_int, glGetStringi, const GLUByte*, GLEnum, GLUInt);
OPENGL_CORE_DEF(gl_get_integer_vector, glGetIntegerv, void, GLEnum, GLInt*);                


typedef enum  {
    VENDOR_UNKNOWN,
    VENDOR_NVIDIA,
    VENDOR_AMD,
    VENDOR_INTEL
} GraphicsCardVendor;

typedef struct  {
    /* info */
    String vendor_str;
    String version_str;
    String extensions_str;
    String renderer_str;
    String glsl_version_str;
    GraphicsCardVendor vendor;
    /* version */
    int major_version;
    int minor_version;
    /* extensions */
    int glsl_available;
    int vertex_buffer_obj_available;
    int vertex_array_obj_available;
    int fragment_program_available;
    int frame_buffer_object_available;
} OpenGLInfo;

#define GL_TRUE                1
#define GL_FALSE               0

#define GL_TRIANGLES      0x0004
#define GL_BYTE           0x1400
#define GL_UNSIGNED_BYTE  0x1401
#define GL_SHORT          0x1402
#define GL_UNSIGNED_SHORT 0x1403
#define GL_INT            0x1404
#define GL_UNSIGNED_INT   0x1405
#define GL_DEPTH          0x1801
#define GL_COLOR          0x1800
#define GL_ALPHA          0x1906
#define GL_TRIANGLES      0x0004
#define GL_QUADS          0x0007
#define GL_FRAGMENT_SHADER 0x8B30
#define GL_VERTEX_SHADER 0x8B31
#define GL_LINK_STATUS 0x8B82
#define GL_COMPILE_STATUS 0x8B81
#define GL_ARRAY_BUFFER 0x8892
#define GL_ELEMENT_ARRAY_BUFFER 0x8893
#define GL_STATIC_DRAW 0x88E4
#define GL_DYNAMIC_DRAW 0x88E8
#define GL_FLOAT 0x1406
#define GL_CULL_FACE 0x0B44
#define GL_BLEND 0x0BE2
#define GL_DEPTH_TEST 0x0B71
#define GL_FRONT 0x0404
#define GL_BACK 0x0405
#define GL_TEXTURE_2D 0x0DE1
#define GL_TEXTURE0 0x84C0
#define GL_RGB8 0x8051
#define GL_RGBA8 0x8058
#define GL_SRGB8 0x8C41
#define GL_SRGB8_ALPHA8 0x8C43
#define GL_RGB 0x1907
#define GL_RGBA 0x1908
#define GL_SRGB 0x8C40
#define GL_TEXTURE_MAG_FILTER 0x2800
#define GL_TEXTURE_MIN_FILTER 0x2801
#define GL_TEXTURE_WRAP_S 0x2802
#define GL_TEXTURE_WRAP_T 0x2803
#define GL_LINEAR 0x2601
#define GL_CLAMP_TO_EDGE 0x812F
#define GL_LINEAR_MIPMAP_LINEAR 0x2703
#define GL_TEXTURE_MAX_ANISOTROPY_EXT 0x84FE
#define GL_SCISSOR_TEST 0x0C11
#define GL_FUNC_ADD	0x8006
#define GL_SRC_ALPHA 0x0302
#define GL_ONE_MINUS_SRC_ALPHA	0x0303
#define GL_WRITE_ONLY 0x88B9
#define GL_NEAREST 0x2600
#define GL_INFO_LOG_LENGTH 0x8B84

#define GL_STREAM_DRAW 0x88E0
#define GL_DEPTH_BUFFER_BIT 0x00000100
#define GL_COLOR_BUFFER_BIT 0x00004000
static
void shader_init(GuiOpenGLState* device) {
    const char* vertex_shader_source = 
#if defined(WASM)
        "#version 100\n"
#else
        "#version 300 es\n"
#endif
        "uniform mat4 uniform_projection_matrix;\n"
#if defined(WASM)
        "attribute vec2 vertex_position;\n"
        "attribute vec2 vertex_tex_coord;\n"
        "attribute vec4 vertex_color;\n"
        "varying vec2 fragment_tex_coord;\n"
        "varying vec4 fragment_color;\n"
#else
        "in vec2 vertex_position;\n"
        "in vec2 vertex_tex_coord;\n"
        "in vec4 vertex_color;\n"
        "out vec4 fragment_color;\n"
        "out vec2 fragment_tex_coord;\n"
#endif
        "void main(void) {\n"
        "    gl_Position = uniform_projection_matrix * vec4(vertex_position.xy, 0, 1);\n"
        "    fragment_color = vertex_color;\n"
        "    fragment_tex_coord = vertex_tex_coord;\n"
        "}\n";
    
    const char* fragment_shader_source = 
#if defined(WASM)
        "#version 100\n"
#else
        "#version 300 es\n"
#endif
        "precision mediump float;\n"
        "uniform sampler2D uniform_texture;\n"
#if defined(WASM)
        "varying vec2 fragment_tex_coord;\n"
        "varying vec4 fragment_color;\n"
#else
        "in vec2 fragment_tex_coord;\n"
        "in vec4 fragment_color;\n"
        "out vec4 out_color;\n"
#endif
        "void main(){\n"
#ifdef WASM
        "   gl_FragColor = fragment_color * texture2D(uniform_texture, fragment_tex_coord);\n"
#else
        "   out_color = fragment_color * texture(uniform_texture, fragment_tex_coord.st);\n"
#endif
        "}\n";

    int status = 1;
    int info_log_length = 0;

    device->vertex_shader_id = gl_create_shader(GL_VERTEX_SHADER);
    gl_shader_source(device->vertex_shader_id, 1, &vertex_shader_source, NULL);
    gl_compile_shader(device->vertex_shader_id);

    gl_get_shader_int_vector(device->vertex_shader_id, GL_COMPILE_STATUS, &status);
    gl_get_shader_int_vector(device->vertex_shader_id, GL_INFO_LOG_LENGTH, &info_log_length);
    if(!status || info_log_length > 0) {
        GLChar buffer[256];
        GLInt length = sizeof(buffer);
        gl_get_shader_info_log(device->vertex_shader_id, length, &length, buffer);
        c_print_format("[GL]: failed to compile shader: %.*s\n", length, buffer);
        //exit(1);
    }
    device->fragment_shader_id = gl_create_shader(GL_FRAGMENT_SHADER);
    gl_shader_source(device->fragment_shader_id, 1, &fragment_shader_source, NULL);
    gl_compile_shader(device->fragment_shader_id);

    gl_get_shader_int_vector(device->fragment_shader_id, GL_COMPILE_STATUS, &status);
    gl_get_shader_int_vector(device->fragment_shader_id, GL_INFO_LOG_LENGTH, &info_log_length);
    if(!status || info_log_length > 0) {
        GLChar buffer[256];
        GLInt length = sizeof(buffer);
        gl_get_shader_info_log(device->program_id, length, &length, buffer);
        c_print_format("[GL]: failed to compile shader: %s\n", buffer);
    }
    
    device->program_id = gl_create_program();
    gl_attach_shader(device->program_id, device->vertex_shader_id);
    gl_attach_shader(device->program_id, device->fragment_shader_id);
    gl_link_program(device->program_id);
    gl_get_program_int_vector(device->program_id, GL_LINK_STATUS, &status);
    gl_get_program_int_vector(device->program_id, GL_INFO_LOG_LENGTH, &info_log_length);
    if(!status || info_log_length > 0) {
        GLChar buffer[256];
        GLInt length = sizeof(buffer);
        gl_get_program_info_log(device->program_id, length, NULL, buffer);
        c_print_format("Error:  %s\n", 256, buffer);
        exit(1);
    }
    gl_use_program(device->program_id);
       
    device->projection_location = gl_get_uniform_location(device->program_id, "uniform_projection_matrix");
    device->texture_location    = gl_get_uniform_location(device->program_id, "uniform_texture");
}

void gui_texture_init_nearest(GuiTexture* texture) {
    gl_gen_textures(1, texture);
    gl_bind_texture(GL_TEXTURE_2D, *texture);
    gl_tex_parameter_int(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    gl_tex_parameter_int(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void gui_texture_init_linear(GuiTexture* texture) {
    gl_gen_textures(1, texture);
    gl_bind_texture(GL_TEXTURE_2D, *texture);
    gl_tex_parameter_int(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    gl_tex_parameter_int(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void gui_texture_update(GuiTexture* texture, void* data, int width, int height) {
    gl_bind_texture(GL_TEXTURE_2D, *texture);
    gl_tex_image_2d(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
}


static
void vbo_init(GuiOpenGLState* gl_state) {
    gl_gen_vertex_arrays(1, &gl_state->vao_id);
    gl_bind_vertex_array(gl_state->vao_id);

    gl_state->vertex_buffer_length = MB(1);
    gl_state->elements_buffer_length = MB(1);

    gl_state->position_location = gl_get_attrib_location(gl_state->program_id, "vertex_position");
    gl_state->uv_location       = gl_get_attrib_location(gl_state->program_id, "vertex_tex_coord");
    gl_state->color_location    = gl_get_attrib_location(gl_state->program_id, "vertex_color");
    gl_uniform1_int(gl_state->texture_location, 0);

    gl_gen_buffers(1, &gl_state->vbo_id);
    gl_gen_buffers(1, &gl_state->ebo_id);
}


static int text_width(GuiFont font, const char *text, int len) {
    unused(font);
    if (len == -1) { len = c_string_length(text); }
    return gui_text_width(string((char*)text, len));
}

static int text_height(GuiFont font) {
    unused(font);
    return 12;
}


static void draw_frame(GuiContext *ctx, RectI rect, int colorid);
static RectI unclipped_rect = { 0, 0, 0x1000000, 0x1000000 };

static GuiStyle default_style = {
    NULL,       /* font */
    { 68, 10 }, /* size */
    6, 4, 24,   /* padding, spacing, indent */
    26, 20,     /* title_height, footer_height */
    12, 8,      /* scrollbar_size, thumb_size */
    {
        { 230, 230, 230, 255 }, /* MU_COLOR_TEXT */
        { 25,  25,  25,  255 }, /* MU_COLOR_BORDER */
        { 50,  50,  50,  255 }, /* MU_COLOR_WINDOWBG */
        { 20,  20,  20,  255 }, /* MU_COLOR_TITLEBG */
        { 240, 240, 240, 255 }, /* MU_COLOR_TITLETEXT */
        { 0,   0,   0,   0   }, /* MU_COLOR_PANELBG */
        { 75,  75,  75,  255 }, /* MU_COLOR_BUTTON */
        { 95,  95,  95,  255 }, /* MU_COLOR_BUTTONHOVER */
        { 115, 115, 115, 255 }, /* MU_COLOR_BUTTONFOCUS */
        { 30,  30,  30,  255 }, /* MU_COLOR_BASE */
        { 35,  35,  35,  255 }, /* MU_COLOR_BASEHOVER */
        { 40,  40,  40,  255 }, /* MU_COLOR_BASEFOCUS */
        { 43,  43,  43,  255 }, /* MU_COLOR_SCROLLBASE */
        { 30,  30,  30,  255 }  /* MU_COLOR_SCROLLTHUMB */
    }
};



void gui_context_init(GuiContext* context, Arena* arena) {
    gui_texture_init_nearest(&context->font_texture);
    gui_texture_update(&context->font_texture, 
                       liberation_sans_regular,
                       liberation_sans_regular_width,
                       liberation_sans_regular_height);
    shader_init(&context->gl_state);
    vbo_init(&context->gl_state);
    context->_style = default_style;
    context->style = &context->_style;

#if defined(WASM)
    
#endif
    
    gl_enable(GL_BLEND);
    gl_blend_equation(GL_FUNC_ADD);
    gl_blend_func(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    gl_disable(GL_CULL_FACE);
    gl_disable(GL_DEPTH_TEST);
    gl_enable(GL_SCISSOR_TEST);

    context->cmd_buffer.draw_command_arena = arena_subarena(arena, MB(1));
}

static
void gui_context_deinit(GuiContext* context) {
    gl_detach_shader(context->gl_state.program_id, context->gl_state.vertex_shader_id);
    gl_detach_shader(context->gl_state.program_id, context->gl_state.fragment_shader_id);
    gl_delete_shader(context->gl_state.vertex_shader_id);
    gl_delete_shader(context->gl_state.fragment_shader_id);
    gl_delete_program(context->gl_state.program_id);
    gl_delete_textures(1, &context->font_texture);
    gl_delete_buffers(1, &context->gl_state.vbo_id);
    gl_delete_buffers(1, &context->gl_state.ebo_id);
}


void gui_begin_frame_opengl(GuiContext* context) {
    GuiOpenGLState* gl_state = &context->gl_state;
    int vbo_size = gl_state->vertex_buffer_length;
    int ebo_size = gl_state->elements_buffer_length;

    gl_bind_buffer(GL_ARRAY_BUFFER, gl_state->vbo_id);
    gl_bind_buffer(GL_ELEMENT_ARRAY_BUFFER, gl_state->ebo_id);
#if defined(WASM)
    Buffer vertices = buffer(gl_state->vertex_buffer, vbo_size);
    Buffer elements = buffer(gl_state->elements_buffer, ebo_size);
#else
    gl_buffer_data(GL_ARRAY_BUFFER, vbo_size, NULL, GL_STREAM_DRAW);
    gl_buffer_data(GL_ELEMENT_ARRAY_BUFFER, ebo_size, NULL, GL_STREAM_DRAW);
    GuiVertex* verts_buffer = (GuiVertex*)gl_map_buffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY); 
    ushort* elems_buffer = (ushort*)gl_map_buffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);    
    Buffer vertices = buffer(verts_buffer, vbo_size);
    Buffer elements = buffer(elems_buffer, ebo_size);
#endif
    context->cmd_buffer.vertices_arena = arena_create_buffer(vertices);
    context->cmd_buffer.elements_arena = arena_create_buffer(elements);

    gl_scissor( -32768, -32768, 65536, 65536 );
    context->command_list.idx = 0;
    context->root_list.idx = 0;
    context->scroll_target = NULL;
    context->last_hover_root = context->hover_root;
    context->hover_root = NULL;
    context->mouse_delta.x = context->mouse_pos.x - context->last_mouse_pos.x;
    context->mouse_delta.y = context->mouse_pos.y - context->last_mouse_pos.y;

}

void gui_end_frame_opengl(GuiContext* context) {
        int i, j, n;
    /* check stacks */
    assert(context->container_stack.idx == 0);
    assert(context->clip_stack.idx      == 0);
    assert(context->id_stack.idx        == 0);
    assert(context->layout_stack.idx    == 0);
    
    /* handle mouse wheel scrolling */
    if (context->scroll_target && context->mouse_wheel) {
        context->scroll_target->scroll.y -= context->mouse_wheel * 30;
    }
    
    /* unset focus if focus id was not touched this frame */
    if (!context->updated_focus) { context->focus = 0; }
    context->updated_focus = 0;
    
    /* bring hover root to front if mouse was pressed */
    if(context->mouse_pressed && context->hover_root &&
       context->hover_root->zindex < context->last_zindex) {
        mu_bring_to_front(context, context->hover_root);
    }
    
    /* reset input state */
    context->key_pressed = 0;
    context->text_input[0] = '\0';
    context->mouse_pressed = 0;
    context->mouse_wheel = 0;
    context->last_mouse_pos = context->mouse_pos;
    
    /* sort root containers by zindex */
    //qsort(context->root_list.items, n, sizeof(mu_Container*), compare_zindex);
    n = context->root_list.idx;
    for(i = 0; i < n - 1; i++) {
        for(j = i + 1; j < n; j++) {
            GuiContainer* a = context->root_list.items[i];
            GuiContainer* b = context->root_list.items[j];
            if(a->zindex > b->zindex) {
                GuiContainer* tmp = a;
                a = b;
                b = tmp;
            }
        }
    }
    
    /* set root container jump commands */
    for (i = 0; i < n; i++) {
        GuiContainer *cnt = context->root_list.items[i];
        /* if this is the first container then make the first command jump to it.
         ** otherwise set the previous container's tail to jump to this one */
        if (i == 0) {
            GuiCommand *cmd = (GuiCommand*) context->command_list.items;
            cmd->jump.dst = (char*) cnt->head + sizeof(GuiJumpCommand);
        } else {
            GuiContainer *prev = context->root_list.items[i - 1];
            prev->tail->jump.dst = (char*) cnt->head + sizeof(GuiJumpCommand);
        }
        /* make the last container's tail jump to the end of command list */
        if (i == n - 1) {
            cnt->tail->jump.dst = context->command_list.items + context->command_list.idx;
        }
    }
    GuiCommand *cmd = NULL;
    while (mu_next_command(context, &cmd)) {
        switch (cmd->type) {
        case GUI_COMMAND_TEXT: {
            String s = string_from_c_string(cmd->text.str);
            gui_draw_text(context, cmd->text.pos.x, cmd->text.pos.y + 10, s, cmd->text.color);
            break;
        }
        case GUI_COMMAND_RECT: {
            Rect dest = rect(cmd->rect.rect.x, cmd->rect.rect.y, cmd->rect.rect.w, cmd->rect.rect.h);
            gui_draw_rect_color(context, dest, cmd->rect.color);
            break;
        }
        case GUI_COMMAND_ICON: {
            Rect dest = rect(cmd->icon.rect.x, cmd->icon.rect.y, cmd->icon.rect.w, cmd->icon.rect.h);
            gui_draw_icon(context, cmd->icon.id, dest, cmd->icon.color);
            break;
        }
        case GUI_COMMAND_IMAGE: {
            Rect dest = rect(cmd->image.dest.x, cmd->image.dest.y, cmd->image.dest.w, cmd->image.dest.h);
            gui_draw_quad_color(context, cmd->image.src, dest, cmd->image.texture, gui_color(255, 255, 255, 255));
            break;
        }
        case GUI_COMMAND_CLIP: {
            Rect dest = rect(cmd->clip.rect.x, cmd->clip.rect.y, cmd->clip.rect.w, cmd->clip.rect.h);
            gui_add_clip_rect(context, dest);
            break;
        }
        invalid_default_case;
        }
    }
    
    GuiOpenGLState* gl_state = &context->gl_state;

#if defined(WASM)
    gl_buffer_sub_data(GL_ARRAY_BUFFER, 0, gl_state->vertex_buffer_length, gl_state->vertex_buffer);
    gl_buffer_sub_data(GL_ELEMENT_ARRAY_BUFFER, 0, gl_state->elements_buffer_length, gl_state->elements_buffer);
#else
    gl_unmap_buffer(GL_ARRAY_BUFFER);
    gl_unmap_buffer(GL_ELEMENT_ARRAY_BUFFER);
#endif
    
    int  vertex_size     = sizeof(GuiVertex);
    long position_offset = offsetof(GuiVertex, position);
    long uv_offset       = offsetof(GuiVertex, uv);
    long color_offset    = offsetof(GuiVertex, color);

    gl_vertex_attrib_pointer(gl_state->position_location, 2, GL_FLOAT, GL_FALSE, vertex_size, (void*)position_offset);
    gl_vertex_attrib_pointer(gl_state->uv_location,       2, GL_FLOAT, GL_FALSE, vertex_size, (void*)uv_offset);
    gl_vertex_attrib_pointer(gl_state->color_location,    4, GL_UNSIGNED_BYTE, GL_TRUE, vertex_size, (void*)color_offset);
    
    gl_enable_vertex_attrib_array(gl_state->position_location);
    gl_enable_vertex_attrib_array(gl_state->uv_location);
    gl_enable_vertex_attrib_array(gl_state->color_location);

    gl_active_texture(GL_TEXTURE0);
    
    gui_get_window_size(context, &context->width, &context->height);
    gui_get_framebuffer_size(context, &context->framebuffer_width, &context->framebuffer_height);
    context->fb_scale_x = (float)context->framebuffer_width  / (float)context->width;
    context->fb_scale_y = (float)context->framebuffer_height / (float)context->height;

    float ortho[4][4] = {
        {2.0f, 0.0f, 0.0f, 0.0f},
        {0.0f,-2.0f, 0.0f, 0.0f},
        {0.0f, 0.0f,-1.0f, 0.0f},
        {-1.0f,1.0f, 0.0f, 1.0f},
    };

    ortho[0][0] /= (float)context->width;
    ortho[1][1] /= (float)context->height;

    gl_uniform_matrix_4fv(gl_state->projection_location, 1, false, &ortho[0][0]);
    gl_viewport(0, 0, context->framebuffer_width, context->framebuffer_height);
    gl_scissor(-8192, -8192, 16384, 16384);
    gl_clear_color(0.2, 0.2, 0.2, 1);
    gl_clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    int num_draw_commands = arena_array_size(&context->cmd_buffer.draw_command_arena, GuiDrawCommand);
    void* offset = 0;
    for(int i = 0; i < num_draw_commands; i++) {
        GuiDrawCommand* cmd = &((GuiDrawCommand*)context->cmd_buffer.draw_command_arena.buffer.begin)[i];
        gl_bind_texture(GL_TEXTURE_2D, cmd->texture);
        //c_print_format("(%f, %f, %f, %f)\n", cmd->clip_rect.x, cmd->clip_rect.y, cmd->clip_rect.w, cmd->clip_rect.h);
        gl_scissor((int)(cmd->clip_rect.x * context->fb_scale_x),
                   (int)((context->height - (int)(cmd->clip_rect.y + cmd->clip_rect.h)) * context->fb_scale_y),
                   (int)(cmd->clip_rect.w * context->fb_scale_x),
                   (int)(cmd->clip_rect.h * context->fb_scale_y));
        gl_draw_elements(GL_TRIANGLES, (int)cmd->elem_count, GL_UNSIGNED_SHORT, offset);
        offset += 2 * cmd->elem_count;
    }
    arena_clear(&context->cmd_buffer.draw_command_arena);
}



static const GuiTexture texture_reuse   = -1;
static const Rect       clip_rect_reuse = { -32768, -32768, 65536, 65536 };

void gui_add_draw_command(GuiContext* context,
                          GuiVertex* new_vertices, int new_vertices_length,
                          ushort* new_elements, int new_elements_length,
                          Rect clip_rect,
                          GuiTexture texture) {
    GuiCommandBuffer* cmd_buffer = &context->cmd_buffer;

    GuiDrawCommand* draw_cmd;//  = arena_alloc_type(&cmd_buffer->draw_command_arena, GuiDrawCommand);
    //*draw_cmd = (GuiDrawCommand) { 0 };
    
#if 1
    //Reuse last draw call:
    GuiDrawCommand* last_draw_cmd = arena_array_last(&cmd_buffer->draw_command_arena, GuiDrawCommand);
    if(last_draw_cmd == NULL) {
        draw_cmd = arena_alloc_type(&cmd_buffer->draw_command_arena, GuiDrawCommand);
        *draw_cmd = (GuiDrawCommand) { 0 };
    } else {
        draw_cmd = last_draw_cmd;
        if(texture == texture_reuse) {
            texture = last_draw_cmd->texture;
        } else if(last_draw_cmd->texture == texture_reuse) {
            last_draw_cmd->texture = texture;
        }
#if 0
        if(rect_equal(last_draw_cmd->clip_rect, clip_rect_reuse)) {
            last_draw_cmd->clip_rect = clip_rect;
        } else
#endif
            if(rect_equal(clip_rect, clip_rect_reuse)) {
            clip_rect = last_draw_cmd->clip_rect;
        }
        if(!rect_equal(last_draw_cmd->clip_rect, clip_rect) ||
           last_draw_cmd->texture != texture) {
            draw_cmd = arena_alloc_type(&cmd_buffer->draw_command_arena, GuiDrawCommand);
            *draw_cmd = (GuiDrawCommand) { 0 };
        }
    }
#endif
    
    int old_vertices_length  = arena_array_size(&cmd_buffer->vertices_arena, GuiVertex);
    GuiVertex* vertices      = arena_alloc_array(&cmd_buffer->vertices_arena, new_vertices_length, GuiVertex);
    ushort* elements         = arena_alloc_array(&cmd_buffer->elements_arena, new_elements_length, ushort);

    for(int i = 0; i < new_vertices_length; i++) {
        vertices[i] = new_vertices[i];
    }
    for(int i = 0; i < new_elements_length; i++) {
        elements[i] = new_elements[i] + old_vertices_length;
    }
    draw_cmd->elem_count += new_elements_length;
    draw_cmd->clip_rect  = clip_rect;
    draw_cmd->texture    = texture;
}

void gui_draw_quad_color(GuiContext* context, Rect src, Rect dest, GuiTexture tex, GuiColor color) {
    float u0 = src.x;  float v0 = src.y;  float u1 = src.x  + src.w;  float v1 = src.y  + src.h;
    float x0 = dest.x; float y0 = dest.y; float x1 = dest.x + dest.w; float y1 = dest.y + dest.h;

    GuiVertex vertices[] = {
        { { x0,  y0  }, { u0, v0 }, { color.r, color.g, color.b, color.a } },
        { { x0,  y1  }, { u0, v1 }, { color.r, color.g, color.b, color.a } },
        { { x1,  y1  }, { u1, v1 }, { color.r, color.g, color.b, color.a } },
        { { x1,  y0  }, { u1, v0 }, { color.r, color.g, color.b, color.a } },
    };

#if DEBUG
    for(ulong i = 0; i < array_length(vertices); i++) {
        GuiVertex* v = &vertices[i];
        c_print_format("%i: (%5f %5f) (%5f %5f) (%02x %02x %02x %02x)\n",
                       i, v->position[0], v->position[1], v->uv[0], v->uv[1],
                       v->color[0], v->color[1], v->color[2], v->color[3]);
    }
#endif
    
    ushort elements[] = { 0, 1, 2, 0, 2, 3 };
    gui_add_draw_command(context,
                         vertices, array_length(vertices),
                         elements, array_length(elements),
                         clip_rect_reuse, tex);
}



void gui_add_clip_rect(GuiContext* context, Rect clip) {
    gui_add_draw_command(context,
                         NULL, 0,
                         NULL, 0,
                         clip, texture_reuse);
}

void gui_draw_rect_color(GuiContext* context, Rect dest, GuiColor color) {
    GuiTexture tex = context->font_texture;
    GuiPackedChar info = packed_icons[0];
    float x0 = info.x0 / 128.0f;
    float y0 = info.y0 / 128.0f;
    float x1 = info.x1 / 128.0f;
    float y1 = info.y1 / 128.0f;
    Rect src = rect(x0, y0, x1 - x0, y1 - y0);
    gui_draw_quad_color(context, src, dest, tex, color);
}

void gui_draw_icon(GuiContext* context, unsigned int icon, Rect dest, GuiColor color) {
    assert(icon < GUI_ICON_MAX);
    GuiTexture tex = context->font_texture;
    GuiPackedChar info = packed_icons[icon];
    float x0 = info.x0 / 128.0f;
    float y0 = info.y0 / 128.0f;
    float x1 = info.x1 / 128.0f;
    float y1 = info.y1 / 128.0f;
    Rect src = rect(x0, y0, x1 - x0, y1 - y0);
    gui_draw_quad_color(context, src, dest, tex, color);
}

void gui_draw_text(GuiContext* context, float x, float y, String s, GuiColor color) {
    GuiTexture tex = context->font_texture;
    float start_x = x;
    for(uint i = 0; i < s.length; i++) {
        char c = s.data[i];
        if(c >= ' ' && c < 127) {
            float xoff  = liberation_sans_regular_packed_chars[(int)c].xoff;
            float yoff  = liberation_sans_regular_packed_chars[(int)c].yoff;
            float xoff2 = liberation_sans_regular_packed_chars[(int)c].xoff2;
            float yoff2 = liberation_sans_regular_packed_chars[(int)c].yoff2;
            float xadvance = liberation_sans_regular_packed_chars[(int)c].xadvance;

            float x0  = liberation_sans_regular_packed_chars[(int)c].x0 / 128.0f;
            float y0  = liberation_sans_regular_packed_chars[(int)c].y0 / 128.0f;
            float x1  = liberation_sans_regular_packed_chars[(int)c].x1 / 128.0f;
            float y1  = liberation_sans_regular_packed_chars[(int)c].y1 / 128.0f;

            Rect src  = rect(x0, y0, x1 - x0, y1 - y0); 
            Rect dest = rect(x + xoff, y + yoff, xoff2 - xoff, yoff2 - yoff); 
            gui_draw_quad_color(context, src, dest, tex, color);
            x += xadvance;
        } else if (c == '\n') {
            x = start_x;
            y += 12;
        }
    }
}

float gui_text_width(String s) {
    float x = 0.0f;
    float start_x = x;
    float max_x = 0.0f;
    for(uint i = 0; i < s.length; i++) {
        char c = s.data[i];
        if(c >= ' ' && c < 127) {
            float xadvance = liberation_sans_regular_packed_chars[(int)c].xadvance;
            x += xadvance;
            max_x = x > max_x ? x : max_x;
        } else if (c == '\n') {
            x = start_x;
        }
    }
    return max_x;
}


void gui_image_data_init(GuiImageData* image) {
    *image =  (GuiImageData){ 0 };
    gui_texture_init_nearest(&image->texture);
}

void gui_image_data_update(GuiImageData* image, int width, int height, u32* data) {
    image->data   = data;
    image->width  = width;
    image->height = height;
    gui_texture_update(&image->texture, data, width, height);
}

GuiImage gui_image(GuiImageData image_data) {
    return (GuiImage) {
        .texture = image_data.texture,
        .rect = rect(0.0, 0.0, 1.0, 1.0),
    };
}
GuiImage gui_sub_image(GuiImageData image_data, RectI rect) {
    return (GuiImage) {
        .texture = image_data.texture,
        .rect = {
            (float)rect.x / image_data.width,
            (float)rect.y / image_data.height,
            (float)rect.w / image_data.width,
            (float)rect.h / image_data.height,
        }
    };
}



/*
** Copyright (c) 2018 rxi
**
** Permission is hereby granted, free of charge, to any person obtaining a copy
** of this software and associated documentation files (the "Software"), to
** deal in the Software without restriction, including without limitation the
** rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
** sell copies of the Software, and to permit persons to whom the Software is
** furnished to do so, subject to the following conditions:
**
** The above copyright notice and this permission notice shall be included in
** all copies or substantial portions of the Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
** FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
** IN THE SOFTWARE.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define GUI_REAL_FMT             "%.3g"
#define GUI_SLIDER_FMT           "%.02f"

#define push(stk, val) do {                                                     \
    assert((stk).idx < (int) (sizeof((stk).items) / sizeof(*(stk).items)));     \
    (stk).items[ (stk).idx ] = (val);                                           \
    (stk).idx++;                                                                \
    } while (0)
        

#define pop(stk) do {                                                           \
    assert((stk).idx > 0);                                                      \
    (stk).idx--;                                                                \
    } while (0)


Vec2I vec2i(int x, int y) {
    Vec2I res;
    res.x = x, res.y = y;
    return res;
}


RectI recti(int x, int y, int w, int h) {
    RectI res;
    res.x = x, res.y = y, res.w = w, res.h = h;
    return res;
}


GuiColor gui_color(int r, int g, int b, int a) {
    GuiColor res;
    res.r = r, res.g = g, res.b = b, res.a = a;
    return res;
}


static RectI expand_rect(RectI rect, int n) {
    return recti(rect.x - n, rect.y - n, rect.w + n * 2, rect.h + n * 2);
}


static RectI clip_rect(RectI r1, RectI r2) {
    int x1 = max(r1.x, r2.x);
    int y1 = max(r1.y, r2.y);
    int x2 = min(r1.x + r1.w, r2.x + r2.w);
    int y2 = min(r1.y + r1.h, r2.y + r2.h);
    if (x2 < x1) { x2 = x1; }
    if (y2 < y1) { y2 = y1; }
    return recti(x1, y1, x2 - x1, y2 - y1);
}


static int rect_overlaps_vec2(RectI r, Vec2I p) {
    return p.x >= r.x && p.x < r.x + r.w && p.y >= r.y && p.y < r.y + r.h;
}


static void draw_frame(GuiContext *ctx, RectI rect, int colorid) {
    mu_draw_rect(ctx, rect, ctx->style->colors[colorid]);
    if (colorid == GUI_COLOR_SCROLLBASE  ||
        colorid == GUI_COLOR_SCROLLTHUMB ||
        colorid == GUI_COLOR_TITLEBG) { return; }
    /* draw border */
    if (ctx->style->colors[GUI_COLOR_BORDER].a) {
        mu_draw_box(ctx, expand_rect(rect, 1), ctx->style->colors[GUI_COLOR_BORDER]);
    }
}

void mu_set_focus(GuiContext *ctx, GuiId id) {
    ctx->focus = id;
    ctx->updated_focus = 1;
}


/* 32bit fnv-1a hash */
#define HASH_INITIAL 2166136261

static void hash(GuiId *hash, const void *data, int size) {
    const unsigned char *p = data;
    while (size--) {
        *hash = (*hash ^ *p++) * 16777619;
    }
}


GuiId mu_get_id(GuiContext *ctx, const void *data, int size) {
    int idx = ctx->id_stack.idx;
    GuiId res = (idx > 0) ? ctx->id_stack.items[idx - 1] : HASH_INITIAL;
    hash(&res, data, size);
    ctx->last_id = res;
    return res;
}


void mu_push_id(GuiContext *ctx, const void *data, int size) {
    push(ctx->id_stack, mu_get_id(ctx, data, size));
}


void mu_pop_id(GuiContext *ctx) {
    pop(ctx->id_stack);
}


void mu_push_clip_rect(GuiContext *ctx, RectI rect) {
    RectI last = mu_get_clip_rect(ctx);
    push(ctx->clip_stack, clip_rect(rect, last));
}


void mu_pop_clip_rect(GuiContext *ctx) {
    pop(ctx->clip_stack);
}


RectI mu_get_clip_rect(GuiContext *ctx) {
    assert(ctx->clip_stack.idx > 0);
    return ctx->clip_stack.items[ ctx->clip_stack.idx - 1 ];
}


int mu_check_clip(GuiContext *ctx, RectI r) {
    RectI cr = mu_get_clip_rect(ctx);
    if (r.x > cr.x + cr.w || r.x + r.w < cr.x ||
        r.y > cr.y + cr.h || r.y + r.h < cr.y    ) { return GUI_CLIP_ALL;  }
    if (r.x >= cr.x && r.x + r.w <= cr.x + cr.w &&
        r.y >= cr.y && r.y + r.h <= cr.y + cr.h  ) { return GUI_CLIP_NONE; }
    return GUI_CLIP_PART;
}


static void push_layout(GuiContext *ctx, RectI body, Vec2I scroll) {
    GuiLayout layout;
    int width = 0;
    memset(&layout, 0, sizeof(GuiLayout));
    layout.body = recti(body.x - scroll.x, body.y - scroll.y, body.w, body.h);
    layout.max = vec2i(-0x1000000, -0x1000000);
    push(ctx->layout_stack, layout);
    mu_layout_row(ctx, 1, &width, 0);
}


static GuiLayout* get_layout(GuiContext *ctx) {
    return &ctx->layout_stack.items[ctx->layout_stack.idx - 1];
}


static void push_container(GuiContext *ctx, GuiContainer *cnt) {
    push(ctx->container_stack, cnt);
    mu_push_id(ctx, &cnt, sizeof(GuiContainer*));
}


static void pop_container(GuiContext *ctx) {
    GuiContainer *cnt = mu_get_container(ctx);
    GuiLayout *layout = get_layout(ctx);
    cnt->content_size.x = layout->max.x - layout->body.x;
    cnt->content_size.y = layout->max.y - layout->body.y;
    /* pop container, layout and id */
    pop(ctx->container_stack);
    pop(ctx->layout_stack);
    mu_pop_id(ctx);
}


GuiContainer* mu_get_container(GuiContext *ctx) {
    assert(ctx->container_stack.idx > 0);
    return ctx->container_stack.items[ ctx->container_stack.idx - 1 ];
}




void mu_init_window(GuiContext *ctx, GuiContainer *cnt, GuiOptionType opt) {
    memset(cnt, 0, sizeof(*cnt));
    cnt->inited = 1;
    cnt->open = opt & GUI_OPT_CLOSED ? 0 : 1;
    cnt->rect = recti(100, 100, 300, 300);
    mu_bring_to_front(ctx, cnt);
}


void mu_bring_to_front(GuiContext *ctx, GuiContainer *cnt) {
    cnt->zindex = ++ctx->last_zindex;
}


/*============================================================================
 * input handlers
 *============================================================================*/

void mu_input_mousemove(GuiContext *ctx, int x, int y) {
    ctx->mouse_pos = vec2i(x, y);
}


void mu_input_mousedown(GuiContext *ctx, int x, int y, int btn) {
    mu_input_mousemove(ctx, x, y);
    ctx->mouse_down |= btn;
    ctx->mouse_pressed |= btn;
}


void mu_input_mouseup(GuiContext *ctx, int x, int y, int btn) {
    mu_input_mousemove(ctx, x, y);
    ctx->mouse_down &= ~btn;
}


void mu_input_mousewheel(GuiContext *ctx, int y) {
    ctx->mouse_wheel += y;
}


void mu_input_key(GuiContext *ctx, GuiKeyboard key, int down) {
    if(down) {
        mu_input_keydown(ctx, key);
    } else {
        mu_input_keyup(ctx, key);
    }
}


void mu_input_keydown(GuiContext *ctx, GuiKeyboard key) {
    ctx->key_pressed |= key;
    ctx->key_down |= key;
}


void mu_input_keyup(GuiContext *ctx, GuiKeyboard key) {
    ctx->key_down &= ~key;
}


void mu_input_text(GuiContext *ctx, const char *text) {
    int len = c_string_length(ctx->text_input);
    int size = c_string_length(text) + 1;
    assert(len + size <= (int) sizeof(ctx->text_input));
    memcpy(ctx->text_input + len, text, size);
}


/*============================================================================
 * commandlist
 *============================================================================*/

GuiCommand* mu_push_command(GuiContext *ctx, int type, int size) {
    GuiCommand *cmd = (GuiCommand*) (ctx->command_list.items + ctx->command_list.idx);
    assert(ctx->command_list.idx + size < GUI_COMMANDLIST_SIZE);
    cmd->base.type = type;
    cmd->base.size = size;
    ctx->command_list.idx += size;
    return cmd;
}


int mu_next_command(GuiContext *ctx, GuiCommand **cmd) {
    if (*cmd) {
        *cmd = (GuiCommand*) (((char*) *cmd) + (*cmd)->base.size);
    } else {
        *cmd = (GuiCommand*) ctx->command_list.items;
    }
    while ((char*) *cmd != ctx->command_list.items + ctx->command_list.idx) {
        if ((*cmd)->type != GUI_COMMAND_JUMP) { return 1; }
        *cmd = (*cmd)->jump.dst;
    }
    return 0;
}


static GuiCommand* push_jump(GuiContext *ctx, GuiCommand *dst) {
    GuiCommand *cmd;
    cmd = mu_push_command(ctx, GUI_COMMAND_JUMP, sizeof(GuiJumpCommand));
    cmd->jump.dst = dst;
    return cmd;
}


void mu_set_clip(GuiContext *ctx, RectI rect) {
    GuiCommand *cmd;
    cmd = mu_push_command(ctx, GUI_COMMAND_CLIP, sizeof(GuiClipCommand));
    cmd->clip.rect = rect;
}


void mu_draw_rect(GuiContext *ctx, RectI rect, GuiColor color) {
    GuiCommand *cmd;
    rect = clip_rect(rect, mu_get_clip_rect(ctx));
    if (rect.w > 0 && rect.h > 0) {
        cmd = mu_push_command(ctx, GUI_COMMAND_RECT, sizeof(GuiRectCommand));
        cmd->rect.rect = rect;
        cmd->rect.color = color;
    }
}


void mu_draw_box(GuiContext *ctx, RectI rect, GuiColor color) {
    mu_draw_rect(ctx, recti(rect.x + 1, rect.y, rect.w - 2, 1), color);
    mu_draw_rect(ctx, recti(rect.x+1, rect.y + rect.h-1, rect.w-2, 1), color);
    mu_draw_rect(ctx, recti(rect.x, rect.y, 1, rect.h), color);
    mu_draw_rect(ctx, recti(rect.x + rect.w - 1, rect.y, 1, rect.h), color);
}


void mu_draw_text(GuiContext *ctx, GuiFont font, const char *str, int len,
                  Vec2I pos, GuiColor color) {
    GuiCommand *cmd;
    RectI rect = recti(pos.x, pos.y, text_width(font, str, len), text_height(font));
    int clipped = mu_check_clip(ctx, rect);
    if (clipped == GUI_CLIP_ALL ) { return; }
    if (clipped == GUI_CLIP_PART) { mu_set_clip(ctx, mu_get_clip_rect(ctx)); }
    /* add command */
    if (len < 0) { len = c_string_length(str); }
    cmd = mu_push_command(ctx, GUI_COMMAND_TEXT, sizeof(GuiTextCommand) + len);
    memcpy(cmd->text.str, str, len);
    cmd->text.str[len] = '\0';
    cmd->text.pos = pos;
    cmd->text.color = color;
    cmd->text.font = font;
    /* reset clipping if it was set */
    if (clipped) { mu_set_clip(ctx, unclipped_rect); }
}


void mu_draw_icon(GuiContext *ctx, GuiIconType id, RectI rect, GuiColor color) {
    GuiCommand *cmd;
    /* do clip command if the rect isn't fully contained within the cliprect */
    int clipped = mu_check_clip(ctx, rect);
    if (clipped == GUI_CLIP_ALL ) { return; }
    if (clipped == GUI_CLIP_PART) { mu_set_clip(ctx, mu_get_clip_rect(ctx)); }
    /* do icon command */
    cmd = mu_push_command(ctx, GUI_COMMAND_ICON, sizeof(GuiIconCommand));
    cmd->icon.id = id;
    cmd->icon.rect = rect;
    cmd->icon.color = color;
    /* reset clipping if it was set */
    if (clipped) { mu_set_clip(ctx, unclipped_rect); }
}

void mu_draw_image(GuiContext *ctx, GuiImage image, RectI rect) {
    GuiCommand *cmd;
    /* do clip command if the rect isn't fully contained within the cliprect */
    int clipped = mu_check_clip(ctx, rect);
    if (clipped == GUI_CLIP_ALL ) { return; }
    if (clipped == GUI_CLIP_PART) { mu_set_clip(ctx, mu_get_clip_rect(ctx)); }
    /* do icon command */
    cmd = mu_push_command(ctx, GUI_COMMAND_IMAGE, sizeof(GuiImageCommand));
    cmd->image.texture = image.texture;
    cmd->image.src     = image.rect;
    cmd->image.dest    = rect;
    /* reset clipping if it was set */
    if (clipped) { mu_set_clip(ctx, unclipped_rect); }
}


/*============================================================================
 * layout
 *============================================================================*/

enum { RELATIVE = 1, ABSOLUTE = 2 };


void mu_layout_begin_column(GuiContext *ctx) {
    push_layout(ctx, mu_layout_next(ctx), vec2i(0, 0));
}


void mu_layout_end_column(GuiContext *ctx) {
    GuiLayout *a, *b;
    b = get_layout(ctx);
    pop(ctx->layout_stack);
    /* inherit position/next_row/max from child layout if they are greater */
    a = get_layout(ctx);
    a->position.x = max(a->position.x, b->position.x + b->body.x - a->body.x);
    a->next_row = max(a->next_row, b->next_row + b->body.y - a->body.y);
    a->max.x = max(a->max.x, b->max.x);
    a->max.y = max(a->max.y, b->max.y);
}


void mu_layout_row(GuiContext *ctx, int items, const int *widths, int height) {
    GuiLayout *layout = get_layout(ctx);
    if (widths) {
        assert(items <= GUI_MAX_WIDTHS);
        memcpy(layout->widths, widths, items * sizeof(widths[0]));
    }
    layout->items = items;
    layout->position = vec2i(layout->indent, layout->next_row);
    layout->size.y = height;
    layout->row_index = 0;
}


void mu_layout_width(GuiContext *ctx, int width) {
    get_layout(ctx)->size.x = width;
}


void mu_layout_height(GuiContext *ctx, int height) {
    get_layout(ctx)->size.y = height;
}


void mu_layout_set_next(GuiContext *ctx, RectI r, int relative) {
    GuiLayout *layout = get_layout(ctx);
    layout->next = r;
    layout->next_type = relative ? RELATIVE : ABSOLUTE;
}


RectI mu_layout_next(GuiContext *ctx) {
    GuiLayout *layout = get_layout(ctx);
    GuiStyle *style = ctx->style;
    RectI res;
    
    if (layout->next_type) {
        /* handle rect set by `mu_layout_set_next` */
        int type = layout->next_type;
        layout->next_type = 0;
        res = layout->next;
        if (type == ABSOLUTE) { return res; }
        
    } else {
        /* handle next row */
        if (layout->row_index == layout->items) {
            mu_layout_row(ctx, layout->items, NULL, layout->size.y);
        }
        
        /* position */
        res.x = layout->position.x;
        res.y = layout->position.y;
        
        /* size */
        res.w = layout->items > -1 ? layout->widths[layout->row_index] : layout->size.x;
        res.h = layout->size.y;
        if (res.w == 0) { res.w = style->size.x + style->padding * 2; }
        if (res.h == 0) { res.h = style->size.y + style->padding * 2; }
        if (res.w <  0) { res.w += layout->body.w - res.x + 1; }
        if (res.h <  0) { res.h += layout->body.h - res.y + 1; }
        
        layout->row_index++;
    }
    
    /* update position */
    layout->position.x += res.w + style->spacing;
    layout->next_row = max(layout->next_row, res.y + res.h + style->spacing);
    
    /* apply body offset */
    res.x += layout->body.x;
    res.y += layout->body.y;
    
    /* update max position */
    layout->max.x = max(layout->max.x, res.x + res.w);
    layout->max.y = max(layout->max.y, res.y + res.h);
    
    return (ctx->last_rect = res);
}


/*============================================================================
 * controls
 *============================================================================*/

static int in_hover_root(GuiContext *ctx) {
    int i = ctx->container_stack.idx;
    while (i--) {
        if (ctx->container_stack.items[i] == ctx->last_hover_root) { return 1; }
        /* only root containers have their `head` field set; stop searching if we've
         ** reached the current root container */
        if (ctx->container_stack.items[i]->head) { break; }
    }
    return 0;
}


void mu_draw_control_frame(GuiContext *ctx, GuiId id, RectI rect,
                           int colorid, GuiOptionType opt) {
    if (opt & GUI_OPT_NOFRAME) { return; }
    colorid += (ctx->focus == id) ? 2 : (ctx->hover == id) ? 1 : 0;
    draw_frame(ctx, rect, colorid);
}


void mu_draw_control_text(GuiContext *ctx, const char *str, RectI rect,
                          int colorid, GuiOptionType opt) {
    Vec2I pos;
    GuiFont font = ctx->style->font;
    int tw = text_width(font, str, -1);
    mu_push_clip_rect(ctx, rect);
    pos.y = rect.y + (rect.h - text_height(font)) / 2;
    if (opt & GUI_OPT_ALIGNCENTER) {
        pos.x = rect.x + (rect.w - tw) / 2;
    } else if (opt & GUI_OPT_ALIGNRIGHT) {
        pos.x = rect.x + rect.w - tw - ctx->style->padding;
    } else {
        pos.x = rect.x + ctx->style->padding;
    }
    mu_draw_text(ctx, font, str, -1, pos, ctx->style->colors[colorid]);
    mu_pop_clip_rect(ctx);
}


int mu_mouse_over(GuiContext *ctx, RectI rect) {
    return rect_overlaps_vec2(rect, ctx->mouse_pos) &&
        rect_overlaps_vec2(mu_get_clip_rect(ctx), ctx->mouse_pos) &&
        in_hover_root(ctx);
}


void mu_update_control(GuiContext *ctx, GuiId id, RectI rect, GuiOptionType opt) {
    int mouseover = mu_mouse_over(ctx, rect);
    
    if (ctx->focus == id) { ctx->updated_focus = 1; }
    if (opt & GUI_OPT_NOINTERACT) { return; }
    if (mouseover && !ctx->mouse_down) { ctx->hover = id; }
    
    if (ctx->focus == id) {
        if (ctx->mouse_pressed && !mouseover) { mu_set_focus(ctx, 0); }
        if (!ctx->mouse_down && ~opt & GUI_OPT_HOLDFOCUS) { mu_set_focus(ctx, 0); }
    }
    
    if (ctx->hover == id) {
        if (!mouseover) {
            ctx->hover = 0;
        } else if (ctx->mouse_pressed) {
            mu_set_focus(ctx, id);
        }
    }
}

void mu_image(GuiContext* ctx, GuiImage image) {
    RectI r = mu_layout_next(ctx);
    mu_draw_image(ctx, image, r);
}

void mu_text(GuiContext *ctx, const char *text) {
    const char *start, *end, *p = text;
    int width = -1;
    GuiFont font = ctx->style->font;
    GuiColor color = ctx->style->colors[GUI_COLOR_TEXT];
    mu_layout_begin_column(ctx);
    mu_layout_row(ctx, 1, &width, text_height(font));
    do {
        RectI r = mu_layout_next(ctx);
        int w = 0;
        start = end = p;
        do {
            const char* word = p;
            while (*p && *p != ' ' && *p != '\n') { p++; }
            w += text_width(font, word, p - word);
            if (w > r.w && end != start) { break; }
            w += text_width(font, p, 1);
            end = p++;
        } while (*end && *end != '\n');
        mu_draw_text(ctx, font, start, end - start, vec2i(r.x, r.y), color);
        p = end + 1;
    } while (*end);
    mu_layout_end_column(ctx);
}


void mu_label(GuiContext *ctx, const char *text) {
    mu_draw_control_text(ctx, text, mu_layout_next(ctx), GUI_COLOR_TEXT, 0);
}


int mu_button_ex(GuiContext *ctx, const char *label, int icon, GuiOptionType opt) {
    int res = 0;
    GuiId id = label ? mu_get_id(ctx, label, c_string_length(label))
        : mu_get_id(ctx, &icon, sizeof(icon));
    RectI r = mu_layout_next(ctx);
    mu_update_control(ctx, id, r, opt);
    /* handle click */
    if (ctx->mouse_pressed == GUI_MOUSE_BUTTON_LEFT && ctx->focus == id) {
        res |= GUI_RES_SUBMIT;
    }
    /* draw */
    mu_draw_control_frame(ctx, id, r, GUI_COLOR_BUTTON, opt);
    if (label) { mu_draw_control_text(ctx, label, r, GUI_COLOR_TEXT, opt); }
    if (icon) { mu_draw_icon(ctx, icon, r, ctx->style->colors[GUI_COLOR_TEXT]); }
    return res;
}


int mu_button(GuiContext *ctx, const char *label) {
    return mu_button_ex(ctx, label, 0, GUI_OPT_ALIGNCENTER);
}


int mu_checkbox(GuiContext *ctx, int *state, const char *label) {
    int res = 0;
    GuiId id = mu_get_id(ctx, &state, sizeof(state));
    RectI r = mu_layout_next(ctx);
    RectI box = recti(r.x, r.y, r.h, r.h);
    mu_update_control(ctx, id, r, 0);
    /* handle click */
    if (ctx->mouse_pressed == GUI_MOUSE_BUTTON_LEFT && ctx->focus == id) {
        res |= GUI_RES_CHANGE;
        *state = !*state;
    }
    /* draw */
    mu_draw_control_frame(ctx, id, box, GUI_COLOR_BASE, 0);
    if (*state) {
        mu_draw_icon(ctx, GUI_ICON_CHECK, box, ctx->style->colors[GUI_COLOR_TEXT]);
    }
    r = recti(r.x + box.w, r.y, r.w - box.w, r.h);
    mu_draw_control_text(ctx, label, r, GUI_COLOR_TEXT, 0);
    return res;
}


#if !defined(WASM)
int mu_textbox_raw(GuiContext *ctx, char *buf, int bufsz, GuiId id, RectI r,
                   GuiOptionType opt)
{
    int res = 0;
    mu_update_control(ctx, id, r, opt | GUI_OPT_HOLDFOCUS);
    
    if (ctx->focus == id) {
        /* handle text input */
        int len = c_string_length(buf);
        int n = min(bufsz - len - 1, (int) c_string_length(ctx->text_input));
        if (n > 0) {
            memcpy(buf + len, ctx->text_input, n);
            len += n;
            buf[len] = '\0';
            res |= GUI_RES_CHANGE;
        }
        /* handle backspace */
        if (ctx->key_pressed & GUI_KEY_BACKSPACE && len > 0) {
            /* skip utf-8 continuation bytes */
            while ((buf[--len] & 0xc0) == 0x80 && len > 0);
            buf[len] = '\0';
            res |= GUI_RES_CHANGE;
        }
        /* handle return */
        if (ctx->key_pressed & GUI_KEY_RETURN) {
            mu_set_focus(ctx, 0);
            res |= GUI_RES_SUBMIT;
        }
    }
    
    /* draw */
    mu_draw_control_frame(ctx, id, r, GUI_COLOR_BASE, opt);
    if (ctx->focus == id) {
        GuiColor color = ctx->style->colors[GUI_COLOR_TEXT];
        GuiFont font = ctx->style->font;
        int textw = text_width(font, buf, -1);
        int texth = text_height(font);
        int ofx = r.w - ctx->style->padding - textw - 1;
        int textx = r.x + min(ofx, ctx->style->padding);
        int texty = r.y + (r.h - texth) / 2;
        mu_push_clip_rect(ctx, r);
        mu_draw_text(ctx, font, buf, -1, vec2i(textx, texty), color);
        mu_draw_rect(ctx, recti(textx + textw, texty, 1, texth), color);
        mu_pop_clip_rect(ctx);
    } else {
        mu_draw_control_text(ctx, buf, r, GUI_COLOR_TEXT, opt);
    }
    
    return res;
}


static int number_textbox(GuiContext *ctx, GuiReal *value, RectI r, GuiId id) {
    if (ctx->mouse_pressed == GUI_MOUSE_BUTTON_LEFT &&
        ctx->key_down & GUI_KEY_SHIFT &&
        ctx->hover == id)
        {
            ctx->number_editing = id;
            sprintf(ctx->number_buf, GUI_REAL_FMT, *value);
        }
    if (ctx->number_editing == id) {
        int res = mu_textbox_raw(
                                 ctx, ctx->number_buf, sizeof(ctx->number_buf), id, r, 0);
        if (res & GUI_RES_SUBMIT || ctx->focus != id) {
            *value = strtod(ctx->number_buf, NULL);
            ctx->number_editing = 0;
        } else {
            return 1;
        }
    }
    return 0;
}


int mu_textbox_ex(GuiContext *ctx, char *buf, int bufsz, GuiOptionType opt) {
    GuiId id = mu_get_id(ctx, &buf, sizeof(buf));
    RectI r = mu_layout_next(ctx);
    return mu_textbox_raw(ctx, buf, bufsz, id, r, opt);
}


int mu_textbox(GuiContext *ctx, char *buf, int bufsz) {
    return mu_textbox_ex(ctx, buf, bufsz, 0);
}

int mu_slider_ex(GuiContext *ctx, GuiReal *value, GuiReal low, GuiReal high,
                 GuiReal step, const char *fmt, GuiOptionType opt)
{
    char buf[GUI_MAX_FMT + 1];
    RectI thumb;
    int w, res = 0;
    GuiReal normalized, last = *value, v = last;
    GuiId id = mu_get_id(ctx, &value, sizeof(value));
    RectI base = mu_layout_next(ctx);
    
    /* handle text input mode */
    if (number_textbox(ctx, &v, base, id)) { return res; }
    
    /* handle normal mode */
    mu_update_control(ctx, id, base, opt);
    
    /* handle input */
    if (ctx->focus == id && ctx->mouse_down == GUI_MOUSE_BUTTON_LEFT) {
        v = low + ((GuiReal) (ctx->mouse_pos.x - base.x) / base.w) * (high - low);
        if (step) { v = ((long) ((v + step/2) / step)) * step; }
    }
    /* clamp and store value, update res */
    *value = v = clamp(v, low, high);
    if (last != v) { res |= GUI_RES_CHANGE; }
    
    /* draw base */
    mu_draw_control_frame(ctx, id, base, GUI_COLOR_BASE, opt);
    /* draw thumb */
    w = ctx->style->thumb_size;
    normalized = (v - low) / (high - low);
    thumb = recti(base.x + normalized * (base.w - w), base.y, w, base.h);
    mu_draw_control_frame(ctx, id, thumb, GUI_COLOR_BUTTON, opt);
    /* draw text  */
    sprintf(buf, fmt, v);
    mu_draw_control_text(ctx, buf, base, GUI_COLOR_TEXT, opt);
    
    return res;
}


int mu_slider(GuiContext *ctx, GuiReal *value, GuiReal low, GuiReal high) {
    return mu_slider_ex(ctx, value, low, high, 0, GUI_SLIDER_FMT, GUI_OPT_ALIGNCENTER);
}

int mu_number_ex(GuiContext *ctx, GuiReal *value, GuiReal step,
                 const char *fmt,GuiOptionType opt)
{
    char buf[GUI_MAX_FMT + 1];
    int res = 0;
    GuiId id = mu_get_id(ctx, &value, sizeof(value));
    RectI base = mu_layout_next(ctx);
    GuiReal last = *value;
    
    /* handle text input mode */
    if (number_textbox(ctx, value, base, id)) { return res; }
    
    /* handle normal mode */
    mu_update_control(ctx, id, base, opt);
    
    /* handle input */
    if (ctx->focus == id && ctx->mouse_down == GUI_MOUSE_BUTTON_LEFT) {
        *value += ctx->mouse_delta.x * step;
    }
    /* set flag if value changed */
    if (*value != last) { res |= GUI_RES_CHANGE; }
    
    /* draw base */
    mu_draw_control_frame(ctx, id, base, GUI_COLOR_BASE, opt);
    /* draw text  */
    sprintf(buf, fmt, *value);
    mu_draw_control_text(ctx, buf, base, GUI_COLOR_TEXT, opt);
    
    return res;
}


int mu_number(GuiContext *ctx, GuiReal *value, GuiReal step) {
    return mu_number_ex(ctx, value, step, "%.03f", GUI_OPT_ALIGNCENTER);
}
#endif

static int header(GuiContext *ctx, int *state, const char *label,
                  int istreenode)
{
    RectI r;
    GuiId id;
    int width = -1;
    mu_layout_row(ctx, 1, &width, 0);
    r = mu_layout_next(ctx);
    id = mu_get_id(ctx, &state, sizeof(state));
    mu_update_control(ctx, id, r, 0);
    /* handle click */
    if (ctx->mouse_pressed == GUI_MOUSE_BUTTON_LEFT && ctx->focus == id) {
        *state = !(*state);
    }
    /* draw */
    if (istreenode) {
        if (ctx->hover == id) { draw_frame(ctx, r, GUI_COLOR_BUTTONHOVER); }
    } else {
        mu_draw_control_frame(ctx, id, r, GUI_COLOR_BUTTON, 0);
    }
    mu_draw_icon(
                 ctx, *state ? GUI_ICON_EXPANDED : GUI_ICON_COLLAPSED,
                 recti(r.x, r.y, r.h, r.h), ctx->style->colors[GUI_COLOR_TEXT]);
    r.x += r.h - ctx->style->padding;
    r.w -= r.h - ctx->style->padding;
    mu_draw_control_text(ctx, label, r, GUI_COLOR_TEXT, 0);
    return *state ? GUI_RES_ACTIVE : 0;
}


int mu_header(GuiContext *ctx, int *state, const char *label) {
    return header(ctx, state, label, 0);
}


int mu_begin_treenode(GuiContext *ctx, int *state, const char *label) {
    int res = header(ctx, state, label, 1);
    if (res & GUI_RES_ACTIVE) {
        get_layout(ctx)->indent += ctx->style->indent;
        mu_push_id(ctx, &state, sizeof(void*));
    }
    return res;
}


void mu_end_treenode(GuiContext *ctx) {
    get_layout(ctx)->indent -= ctx->style->indent;
    mu_pop_id(ctx);
}


#define scrollbar(ctx, cnt, b, cs, x, y, w, h)                                  \
    do {                                                                        \
        /* only add scrollbar if content size is larger than body */            \
        int maxscroll = cs.y - b->h;                                            \
                                                                                \
        if(maxscroll > 0 && b->h > 0) {                                         \
            RectI base, thumb;                                                  \
            GuiId id = mu_get_id(ctx, "!scrollbar" #y, 11);                     \
                                                                                \
            /* get sizing / positioning */                                      \
            base = *b;                                                          \
            base.x = b->x + b->w;                                               \
            base.w = ctx->style->scrollbar_size;                                \
                                                                                \
            /* handle input */                                                  \
            mu_update_control(ctx, id, base, 0);                                \
            if(ctx->focus == id && ctx->mouse_down == GUI_MOUSE_BUTTON_LEFT) {  \
                cnt->scroll.y += ctx->mouse_delta.y * cs.y / base.h;            \
            }                                                                   \
            /* clamp scroll to limits */                                        \
            cnt->scroll.y = clamp(cnt->scroll.y, 0, maxscroll);                 \
                                                                                \
            /* draw base and thumb */                                           \
            draw_frame(ctx, base, GUI_COLOR_SCROLLBASE);                        \
            thumb = base;                                                       \
            thumb.h = max(16, base.h * b->h / cs.y);                            \
            thumb.y += cnt->scroll.y * (base.h - thumb.h) / maxscroll;          \
            draw_frame(ctx, thumb, GUI_COLOR_SCROLLTHUMB);                      \
                                                                                \
            /* set this as the scroll_target (will get scrolled on */           \
            /* mousewheel) if the mouse is over it */                           \
            if (mu_mouse_over(ctx, *b)) { ctx->scroll_target = cnt; }           \
        } else {                                                                \
            cnt->scroll.y = 0;                                                  \
        }                                                                       \
        } while (0)

#if 0
        }}}
#endif
        
static void scrollbars(GuiContext *ctx, GuiContainer *cnt, RectI *body) {
    int sz = ctx->style->scrollbar_size;
    Vec2I cs = cnt->content_size;
    cs.x += ctx->style->padding * 2;
    cs.y += ctx->style->padding * 2;
    mu_push_clip_rect(ctx, *body);
    /* resize body to make room for scrollbars */
    if (cs.y > cnt->body.h) { body->w -= sz; }
    if (cs.x > cnt->body.w) { body->h -= sz; }
    /* to create a horizontal or vertical scrollbar almost-identical code is
     * used; only the references to `x|y` `w|h` need to be switched */
    scrollbar(ctx, cnt, body, cs, x, y, w, h);
    scrollbar(ctx, cnt, body, cs, y, x, h, w);
    mu_pop_clip_rect(ctx);
}
    
    
static void push_container_body(GuiContext *ctx, GuiContainer *cnt, RectI body,
                                GuiOptionType opt) {
    if (~opt & GUI_OPT_NOSCROLL) { scrollbars(ctx, cnt, &body); }
    mu_push_clip_rect(ctx, body);
    push_layout(ctx, expand_rect(body, -ctx->style->padding), cnt->scroll);
    cnt->body = body;
}


static void begin_root_container(GuiContext *ctx, GuiContainer *cnt) {
    push_container(ctx, cnt);
    
    /* push container to roots list and push head command */
    push(ctx->root_list, cnt);
    cnt->head = push_jump(ctx, NULL);
    
    /* set as hover root if the mouse is overlapping this container and it has a
     * higher zindex than the current hover root */
    if (rect_overlaps_vec2(cnt->rect, ctx->mouse_pos) &&
        (!ctx->hover_root || cnt->zindex > ctx->hover_root->zindex)) {
            ctx->hover_root = cnt;
        }
    /* clipping is reset here in case a root-container is made within
     * another root-containers's begin/end block; this prevents the inner
     * root-container being clipped to the outer */
    push(ctx->clip_stack, unclipped_rect);
}

static void end_root_container(GuiContext *ctx) {
    /* push tail 'goto' jump command and set head 'skip' command. the final steps
     ** on initing these are done in mu_end() */
    GuiContainer *cnt = mu_get_container(ctx);
    cnt->tail = push_jump(ctx, NULL);
    cnt->head->jump.dst = ctx->command_list.items + ctx->command_list.idx;
    /* pop base clip rect and container */
    mu_pop_clip_rect(ctx);
    pop_container(ctx);
}


int mu_begin_window_ex(GuiContext *ctx, GuiContainer *cnt, const char *title,
                       GuiOptionType opt) {
    RectI rect, body, titlerect;
    
    if (!cnt->inited) { mu_init_window(ctx, cnt, opt); }
    if (!cnt->open) { return 0; }
    
    begin_root_container(ctx, cnt);
    rect = cnt->rect;
    body = rect;
    
    /* draw frame */
    if (~opt & GUI_OPT_NOFRAME) {
        draw_frame(ctx, rect, GUI_COLOR_WINDOWBG);
    }
    
    /* do title bar */
    titlerect = rect;
    titlerect.h = ctx->style->title_height;
    if (~opt & GUI_OPT_NOTITLE) {
        draw_frame(ctx, titlerect, GUI_COLOR_TITLEBG);
        
        /* do title text */
        if (~opt & GUI_OPT_NOTITLE) {
            GuiId id = mu_get_id(ctx, "!title", 6);
            mu_update_control(ctx, id, titlerect, opt);
            mu_draw_control_text(ctx, title, titlerect, GUI_COLOR_TITLETEXT, opt);
            if (id == ctx->focus && ctx->mouse_down == GUI_MOUSE_BUTTON_LEFT) {
                cnt->rect.x += ctx->mouse_delta.x;
                cnt->rect.y += ctx->mouse_delta.y;
            }
            body.y += titlerect.h;
            body.h -= titlerect.h;
        }
        
        /* do `close` button */
        if (~opt & GUI_OPT_NOCLOSE) {
            GuiId id = mu_get_id(ctx, "!close", 6);
            RectI r = recti(
                                titlerect.x + titlerect.w - titlerect.h,
                                titlerect.y, titlerect.h, titlerect.h);
            titlerect.w -= r.w;
            mu_draw_icon(ctx, GUI_ICON_CLOSE, r, ctx->style->colors[GUI_COLOR_TITLETEXT]);
            mu_update_control(ctx, id, r, opt);
            if (ctx->mouse_pressed == GUI_MOUSE_BUTTON_LEFT && id == ctx->focus) {
                cnt->open = 0;
            }
        }
    }
    
    /* do `resize` notch */
    if (~opt & GUI_OPT_NORESIZE) {
        int sz = ctx->style->footer_height;
        GuiId id = mu_get_id(ctx, "!resize", 7);
        RectI r = recti(rect.x + rect.w - sz, rect.y + rect.h - sz, sz, sz);
        mu_update_control(ctx, id, r, opt);
        mu_draw_icon(ctx, GUI_ICON_RESIZE, r, ctx->style->colors[GUI_COLOR_TEXT]);
        if (id == ctx->focus && ctx->mouse_down == GUI_MOUSE_BUTTON_LEFT) {
            cnt->rect.w += ctx->mouse_delta.x;
            cnt->rect.h += ctx->mouse_delta.y;
            cnt->rect.w = max(96, cnt->rect.w);
            cnt->rect.h = max(64, cnt->rect.h);
        }
        body.h -= sz;
    }
    
    /* do scrollbars and init clipping */
    push_container_body(ctx, cnt, body, opt);
    
    /* resize to content size */
    if (opt & GUI_OPT_AUTOSIZE) {
        RectI r = get_layout(ctx)->body;
        cnt->rect.w = cnt->content_size.x + (cnt->rect.w - r.w);
        cnt->rect.h = cnt->content_size.y + (cnt->rect.h - r.h);
    }
    
    /* close if this is a popup window and elsewhere was clicked */
    if (opt & GUI_OPT_POPUP && ctx->mouse_pressed && ctx->last_hover_root != cnt) {
        cnt->open = 0;
    }
    
    return GUI_RES_ACTIVE;
}


int mu_begin_window(GuiContext *ctx, GuiContainer *cnt, const char *title) {
    return mu_begin_window_ex(ctx, cnt, title, 0);
}


void mu_end_window(GuiContext *ctx) {
    mu_pop_clip_rect(ctx);
    end_root_container(ctx);
}


void mu_open_popup(GuiContext *ctx, GuiContainer *cnt) {
    /* set as hover root so popup isn't closed in begin_window_ex()  */
    ctx->last_hover_root = ctx->hover_root = cnt;
    /* init container if not inited */
    if (!cnt->inited) { mu_init_window(ctx, cnt, 0); }
    /* position at mouse cursor, open and bring-to-front */
    cnt->rect = recti(ctx->mouse_pos.x, ctx->mouse_pos.y, 0, 0);
    cnt->open = 1;
    mu_bring_to_front(ctx, cnt);
}


int mu_begin_popup(GuiContext *ctx, GuiContainer *cnt) {
    GuiOptionType opt = GUI_OPT_POPUP | GUI_OPT_AUTOSIZE | GUI_OPT_NORESIZE |
        GUI_OPT_NOSCROLL | GUI_OPT_NOTITLE | GUI_OPT_CLOSED;
    return mu_begin_window_ex(ctx, cnt, "", opt);
}


void mu_end_popup(GuiContext *ctx) {
    mu_end_window(ctx);
}


void mu_begin_panel_ex(GuiContext *ctx, GuiContainer *cnt, GuiOptionType opt) {
    cnt->rect = mu_layout_next(ctx);
    if (~opt & GUI_OPT_NOFRAME) {
        draw_frame(ctx, cnt->rect, GUI_COLOR_PANELBG);
    }
    push_container(ctx, cnt);
    push_container_body(ctx, cnt, cnt->rect, opt);
}


void mu_begin_panel(GuiContext *ctx, GuiContainer *cnt) {
    mu_begin_panel_ex(ctx, cnt, 0);
}


void mu_end_panel(GuiContext *ctx) {
    mu_pop_clip_rect(ctx);
    pop_container(ctx);
}
