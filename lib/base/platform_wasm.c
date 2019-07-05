#if defined(WASM)

void    path_init(Path* path, String name) { }
void    path_init_c(Path* path, char* name) { }
void    path_init_from_c(Path* path, Path* dir, char* name) { }
void    path_init_from(Path* path, Path* dir, String name) { }
void    path_init_working_directory(Path* path) { }
void    path_create(Path* path, String name) { }
void    path_create_c(Path* path, char* name) { }
void    path_create_from_c(Path* path, Path* dir, char* name) { }
void    path_create_from(Path* path, Path* dir, String name) { }
void    path_copy(Path* orig, Path* copy) { }
b32     path_can_read(Path* path) { return 0; }
Buffer  path_read_file(Path* path, Arena* arena) { return (Buffer){ }; }
Buffer  path_read_file_aligned(Path* path, Arena* arena, int alignment) { return (Buffer){ }; }
b32     path_can_write(Path* path) { return 0; }
void    path_write_file(Path* path, Buffer buffer) { }
void    path_close(Path* path) { }
void    path_navigate(Path* path, String name) { }
void    path_navigate_c(Path* path, char* name) { }
ulong   path_get_file_size(Path* path) { return 0; }
int     path_is_directory(Path* path) { return 0; }
String  path_get_name(Path* path, Arena* arena) { return (String) { }; }
String  path_get_base_name(Path* path, Arena* arena) { return (String) { }; }
DirIter dir_iter_begin(Path* path) { return (DirIter) { }; }
int     dir_iter_next(DirIter* dir_iter) { return 0; }
void    dir_iter_end(DirIter* dir_iter) { }

void exit(int status) { while(1); }
int  raise_signal(int signal) { return 0; }

void* page_alloc(ulong num_pages) { return NULL; }
void* page_realloc(void* pages, ulong old_num_pages, ulong new_num_pages) { return NULL; }
void  page_dealloc(void* pages, ulong num_pages) { }

void console_log(char name[static 1], int length);
void console_error(char name[static 1], int length);

void print(String s) {
    console_log(s.data, s.length);
}
void c_print(char* c) { print(string(c, c_string_length(c))); }

void error(String s) {
    console_error(s.data, s.length);
}
void c_error(char* c) { error(string(c, c_string_length(c))); }

// NOTE: See https://dassur.ma/things/c-to-webassembly/ for explanation
extern unsigned char __heap_base;

void* get_memory(ulong size) {
    u32 current  = __builtin_wasm_memory_grow(0, (size + 65535) / 65536);
    c_print_format("Huhu: %u %u\n", current * 65536, size);
    return (void*)(current * 65536);
}

void* memcpy(void* dest, const void* src, ulong size) {
    return c_memcpy(dest, src, size);
}

void* memset(void* dest, int c, ulong size) {
    return c_memset(dest, c, size);
}


#ifdef BASE_GUI
void gui_init(GuiContext* context, Arena* arena) {
    gui_context_init(context, arena);
}

void gui_deinit(GuiContext* context) {
    gui_context_deinit(context);
}

void gui_get_window_size(GuiContext* context, int* width, int* height) {
    unused(context);
    *width = 1918;
    *height = 985;
}

void gui_get_framebuffer_size(GuiContext* context, int* width, int* height) {
    unused(context);
    *width = 1918;
    *height = 985;
}


void gui_begin_frame(GuiContext* context) {
    gui_begin_frame_opengl(context);
}


void gui_end_frame(GuiContext* context) {
    gui_end_frame_opengl(context);
}

b32 gui_should_continue(GuiContext* context){
    unused(context);
    return 0;
}

PlatformWindow create_window() {
    return (PlatformWindow){};
}

void get_cursor_position(PlatformWindow window, int* x, int* y) {
    unused(window);
    *x = *y = 0;
}

void gui_set_window_name(GuiContext* context, String window_name) {
    unused(context);
    unused(window_name);
}

void glActiveTexture(i32 slot);
void glAttachShader(i32 shader, i32 program);
void glBindBuffer(i32 target, i32 buffer);
void glBindTexture(i32 type, i32 texture);
void glBindVertexArray(i32 vao);
void glBlendEquation(i32);
void glBlendFunc(i32, i32);
void glBufferData(i32 target, i32 length, const void* data, i32 usage);
void glBufferSubData(i32 target, i32 offset, i32 size, const void* data);
void glClear(i32);
void glClearColor(f32 r, f32 g, f32 b, f32 a);
void glCompileShader(i32 shader);
i32  glCreateProgram(void);
i32  glCreateShader(i32 type);
void glDeleteBuffer(i32 BO);
void glDeleteProgram(i32 program);
void glDeleteShader(i32 shader);
void glDeleteTexture(i32 texture);
void glDetachShader(i32 program, i32 shader);
void glDisable(i32 cap);
void glDrawElements(i32 mode, i32 count, i32 type, i32 offset);
void glEnable(i32 cap);
void glEnableVertexAttribArray(i32 index);
i32  glGenBuffer();
i32  glGenTexture();
i32  glGenVertexArray();
i32  glGetAttribLocation(i32 program, const char name[static 1], i32 name_len);
i32  glGetUniformLocation(i32 program, const char name[static 1], i32 name_len);
i32  glGetShaderParameter(i32, i32);
i32  glGetProgramParameter(i32, i32);
void glLinkProgram(i32 program);
void glScissor(i32, i32, i32, i32);
void glShaderSource(i32 shader, const char source[static 1], i32 source_len);
void glTexImage2D(i32 target, i32 level, i32 internalFormat, i32 width, i32 height, i32 border, i32 format, i32 type, void* data);
void glTexParameteri(i32 target, i32 pname, i32 value);
void glUniform1i(i32 location, i32 value);
void glUniformMatrix4fv(i32 location, i32 transpose, const f32 data[static 16]);
void glUseProgram(i32 program);
void glVertexAttribPointer(i32 index, i32 size, i32 type, i32 normalized,
                           i32 stride, i32 offset);
void glViewport(i32 x, i32 y, i32 w, i32 h);

void glActiveTexture_(i32 slot) {
    glActiveTexture(slot);
}
void glAttachShader_(uint shader, uint program) {
    glAttachShader(shader, program);
}
void glBindBuffer_(int target, uint buffer) {
    glBindBuffer(target, buffer);
}
void glBindTexture_(int type, uint texture) {
    glBindTexture(type, texture);
}
void glBindVertexArray_(uint vao) {
    glBindVertexArray(vao);
}
void glBlendEquation_(i32 mode) {
    glBlendEquation(mode);
}
void glBlendFunc_(i32 sfactor, i32 dfactor) {
    glBlendFunc(sfactor, dfactor);
}
void glBufferData_(int target, iptr length, const void * data, i32 usage) {
    glBufferData(target, length, data, usage);
}
void glCompileShader_(i32 shader) {
    glCompileShader(shader);
}
i32 glCreateProgram_(void) {
    return glCreateProgram();
}
i32 glCreateShader_(i32 type) {
    return glCreateShader(type);
}
void glDeleteProgram_(i32 program) {
    glDeleteProgram(program);
}
void glDeleteShader_(i32 shader) {
    glDeleteShader(shader);
}
void glDetachShader_(i32 program, i32 shader) {
    glDetachShader(program, shader);
}
void glDisable_(i32 cap) {
    glDisable(cap);
}
void glDrawElements_(i32 mode, i32 count, i32 type, void* offset) {
    glDrawElements(mode, count, type, (i32)offset);
}
void glEnable_(i32 cap) {
    glEnable(cap);
}
void glEnableVertexAttribArray_(i32 index) {
    glEnableVertexAttribArray(index);
}
i32 glGetUniformLocation_(i32 program, const char* name) {
    return glGetUniformLocation(program, name, c_string_length(name));
}
void glLinkProgram_(i32 program) {
    glLinkProgram(program);
}
void glShaderSource_(i32 shader, i32 count, const char** source, int* length) {
    if(length != 0) {
        for(int i = 0; i < count; i++) {
            glShaderSource(shader, source[i], length[i]);
        }
    } else {
        for(int i = 0; i < count; i++) {
            glShaderSource(shader, source[i], c_string_length(source[i]));
        }
    }
}
void glTexParameteri_(i32 target, i32 pname, i32 value) {
    glTexParameteri(target, pname, value);
}
void glUniform1i_(i32 location, i32 value) {
    glUniform1i(location, value);
}
void glUseProgram_(i32 program) {
    glUseProgram(program);
}
void glVertexAttribPointer_(i32 index, i32 size, i32 type, i32 normalized, i32 stride, const void* pointer) {
    glVertexAttribPointer(index, size, type, normalized, stride, (i32)pointer);
}
void glViewport_(i32 x, i32 y, i32 w, i32 h) {
    glViewport(x, y, w, h);
}
i32  glGetAttribLocation_(i32 program, const char* name) {
    return glGetAttribLocation(program, name, c_string_length(name));
}
void glScissor_(i32 x, i32 y, i32 width, i32 height) {
    glScissor(x, y, width, height);
}
void glClear_(i32 mask) {
    glClear(mask);
}
void glClearColor_(f32 r, f32 g, f32 b, f32 a) {
    glClearColor(r, g, b, a);
}
void glTexImage2D_(i32 target, i32 level, i32 internalFormat, i32 width, i32 height, i32 border, i32 format, i32 type, void* data) {
    glTexImage2D(target, level, internalFormat, width, height, border, format, type, data);
}
void glDeleteBuffers_(int n, uint* buffers) {
    for(int i = 0; i < n; i++) {
        glDeleteBuffer(buffers[i]);
    }
}
void glDeleteTextures_(int n, uint* textures) {
    for(int i = 0; i < n; i++) {
        glDeleteTexture(textures[i]);
    }
}
void glGenBuffers_(int n, uint* buffers) {
    for(int i = 0; i < n; i++) {
        buffers[i] = glGenBuffer();
    }
}
void glGenTextures_(int n, uint* textures) {
    for(int i = 0; i < n; i++) {
        textures[i] = glGenTexture();
    }
}
void glGenVertexArrays_(int n, uint* arrays) {
    for(int i = 0; i < n; i++) {
        arrays[i] = glGenVertexArray();
    }
}
void glGetShaderiv_(uint shader, uint pname, int* params) {
    unused(shader);
    unused(pname);
    unused(params);
}
void glGetProgramiv_(uint program, uint pname, int* params) {
    unused(program);
    unused(pname);
    unused(params);
}
void glUniformMatrix4fv_(int location, int count, uchar transpose, float* value) {
    for(int i = 0; i < count; i++) {
        glUniformMatrix4fv(location, transpose, value + sizeof(float) * 16 * i);
    }
}
i32 glGetShaderParameter_(i32 shader, i32 pname) {
    return glGetShaderParameter(shader, pname);
}
i32 glGetProgramParameter_(i32 program, i32 pname) {
    return glGetProgramParameter(program, pname);
}
void glBufferSubData_(i32 target, iptr offset, iptr size, const void* data) {
    glBufferSubData(target, offset, size, data);
}

void glGetShaderInfoLog_(i32 a, i32 b , iptr c, const void* d) { }
void glGetProgramInfoLog_(i32 a, i32 b , iptr c, const void* d) { }


#endif //BASE_GUI
#endif //defined(WASM)