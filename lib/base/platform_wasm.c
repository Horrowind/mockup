#if defined(WASM)

void path_init(Path* path, String name) { }
void path_init_c(Path* path, char* name) { }
void path_init_from_c(Path* path, Path* dir, char* name) { }
void path_init_from(Path* path, Path* dir, String name) { }
void path_init_working_directory(Path* path) { }
void path_create(Path* path, String name) { }
void path_create_c(Path* path, char* name) { }
void path_create_from_c(Path* path, Path* dir, char* name) { }
void path_create_from(Path* path, Path* dir, String name) { }
void path_copy(Path* orig, Path* copy) { }
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

void base_global_init() { }

void exit(int status) { while(1) { } }
int  raise_signal(int signal) { return 0; }

void* page_alloc(ulong num_pages) { return NULL; }
void* page_realloc(void* pages, ulong old_num_pages, ulong new_num_pages) { return NULL; }
void  page_dealloc(void* pages, ulong num_pages) { }

void  c_print_format(char* format, ...) { }
void  c_error_format(char* format, ...) { }

void c_write(char* c) { }
void write(String s) { }

void c_error(char* c) { }
void error(String s) { }

void* memset(void *s, int c, ulong n) { return NULL; }
void* memcpy(void *dest, const void *src, ulong n) { return NULL; }
void* realloc(void *ptr, ulong size) { return NULL; }
void* malloc(ulong size) { return NULL; }
void  free(void *ptr) { }
#endif //defined(WASM)