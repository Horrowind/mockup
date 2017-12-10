#ifndef BASE_GUARD
#define BASE_GUARD

#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <stdalign.h>
#include <stdio.h>
#include <stdint.h>

#include <dirent.h>
#include <fcntl.h>
#include <libgen.h>
#include <errno.h>
#include <signal.h>


#if defined(__EMSCRIPTEN__)
#define WASM
#elif defined(__MINGW32__)
#define WINDOWS
#else
#define LINUX
#endif

typedef int8_t i8;
typedef uint8_t u8;
typedef int16_t i16;
typedef uint16_t u16;
typedef uint32_t b32;
typedef int32_t i32;
typedef uint32_t u32;
typedef int64_t i64;
typedef uint64_t u64;
typedef float f32;
typedef double f64;

typedef unsigned long  ulong;
typedef unsigned int   uint;
typedef unsigned short ushort;
typedef unsigned char  uchar;

typedef intptr_t       iptr;
typedef uintptr_t      uptr;



#ifndef NDEBUG
#define assert(x) if(!(x)) {*(int *)0 = 0;}
#else
#define assert(x)
#endif

#define STR_(x) #x
#define STR(x) STR_(x)
#define CODE_LOCATION STR(__FILE__) ": " STR(__LINE__) " in " STR(__PRETTY_FUNCTION__)
#define invalid_code_path assert(!"Invalid code path at "   \
                                 CODE_LOCATION)
#define invalid_default_case default: {invalid_code_path;} break


#define KB(x) (x * 1024ull)
#define MB(x) (x * 1024ull * 1024ull)
#define GB(x) (x * 1024ull * 1024ull * 1024ull)

#define array_length(array) (sizeof(array)/sizeof(array[0]))
#undef max
#undef min
#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))
#define debug_break raise(SIGINT)
#define PAGE_SIZE 4096

struct {
    uint page_size;
    uint is_initialized : 1;
} global;

void global_init();

typedef struct {
    void* begin;
    void* end;
} Buffer;

typedef struct {
    Buffer buffer;
    void*  current;
} Arena;

Arena   arena_create(ulong size);
Arena   arena_create_buffer(Buffer buffer);
void    arena_clear(Arena* a);
Arena   arena_subarena(Arena* a, ulong size);
void*   arena_alloc(Arena* a, ulong size, ulong align);
#define arena_alloc_type(a,type) ((type*)arena_alloc((a), sizeof(type), alignof(type)))
#define arena_alloc_array(a,n,type) (type*)arena_alloc((a), (n)*sizeof(type), alignof(type))

typedef struct {
    Arena* arena;
    void*  old_current;
} Temp;

Temp temp_begin(Arena* a);
void temp_end(Temp tmp);

typedef struct {
    char* data;
    ushort length;
} String;

#define L(cstr) (String){.data = cstr, .length = array_length(cstr) - 1 }

b32 string_equal(String s1, String s2);
String string_from_c_string(char* buf);
u64 string_to_u64(String s);
u64 string_to_u64_base(String s, int base);

#define string_split_foreach(s, t, c)                                   \
    String t = { .data = (s).data };                                  \
    for(char* macro_c_ = (s).data; macro_c_ <= (s).data + (s).length;   \
        t.data = (*macro_c_ == c) ? macro_c_ + 1 : t.data,              \
            macro_c_++, t.length = macro_c_ - t.data)                   \
        if(*macro_c_ == c || macro_c_ == (s).data + (s).length)         \


typedef struct {
    uint has_stats : 1;
    uint invalid   : 1;
#ifdef LINUX
    int fd_;
    struct stat st_buf_;
#endif
} Path;

typedef struct {
    Path path;
    Path* dir_path;
#ifdef LINUX
    DIR* dp_;
#endif
} DirIter;

void       path_init(Path* path, String name);
void       path_init_c(Path* path, char* name);
void       path_init_working_directory(Path* path);
void       path_init_from_c(Path* path, Path* dir, char* name);
void       path_init_from(Path* path, Path* dir, String name);
void       path_copy(Path* orig, Path* copy);
Buffer     path_open_file(Path* path, Arena* arena);
Buffer     path_open_file_aligned(Path* path, Arena* arena, int alignment);
void       path_close(Path* path);
void       path_navigate(Path* path, String name);
void       path_navigate_c(Path* path, char* name);
ulong      path_get_file_size(Path* path);
int        path_is_directory(Path* path);
String   path_get_name(Path* path, Arena* arena); //TODO
String   path_get_base_name(Path* path, Arena* arena); //TODO
DirIter dir_iter_begin(Path* path);
int        dir_iter_next(DirIter* dir_iter);
void       dir_iter_end(DirIter* dir_iter);

/* void  pool_init(pool_t* pool, page_list_t* free_list); */
/* void* pool_alloc(pool_t* pool,ulong bytes, ulong align); */
/* void  pool_empty(pool_t* pool); */
/* void  pool_deinit(pool_t* pool); */
/* #define pool_alloc_type(a,type) ((type*)pool_alloc(a, (int)sizeof(type), alignof(type))) */
/* #define pool_alloc_array(a,n,type) (type*)pool_alloc(a, (n)*sizeof(type), alignof(type)) */

typedef struct {
    String file_name;
    int      line_number;
    int      line_pos;
    char*    line_start;
} TextPos;



#ifdef NDEBUG
#define error_at_pos(text_pos) error_at_pos2(text_pos)
void error_at_pos2(TextPos text_pos);
#else
#define error_at_pos(text_pos) error_at_pos2(text_pos, __LINE__)
void error_at_pos2(TextPos text_pos, int line_num);
#endif

#define FixedString(size)                                               \
    struct {                                                            \
        char data[size];                                                \
        u16 length;                                                     \
    } 

#define define_fixed_string(name, prefix, size)                         \
    typedef FixedString(size) name;                                     \
    void prefix##_append(name s1, name s2);                             \
    name prefix##_cat(name s1, name s2);                                \
    String prefix##_to_string(name* s);                                 \
    void prefix##_from_c_string(name* s, char* cstring);                \
    char* prefix##_find_char(name* s, char c);

#define implement_fixed_string(name, prefix, size)                      \
    inline void prefix##_append(name s1, name s2) {                     \
        u16 size_to_copy;                                               \
        if(s1.length + s2.length > size) {                              \
            size_to_copy = size - s1.length;                            \
        } else {                                                        \
            size_to_copy = s2.length;                                   \
        }                                                               \
        for(int i = 0; i < size_to_copy; i++) {                         \
            s1.data[s1.length + i] = s2.data[i];                        \
        }                                                               \
    }                                                                   \
                                                                        \
    inline name prefix##_cat(name s1, name s2) {                        \
        name result = s1;                                               \
        u16 size_to_copy;                                               \
        if(s1.length + s2.length > size) {                              \
            size_to_copy = size - s1.length;                            \
        } else {                                                        \
            size_to_copy = s2.length;                                   \
        }                                                               \
        for(int i = 0; i < size_to_copy; i++) {                         \
            result.data[s1.length + i] = s2.data[i];                    \
        }                                                               \
        return result;                                                  \
    }                                                                   \
                                                                        \
    inline char* prefix##_find_char(name* s, char c) {                  \
        char* iter = s->data;                                           \
        for(int i = 0; i < s->length; i++) {                            \
            if(*iter == c) return iter;                                 \
            iter++;                                                     \
        }                                                               \
    }                                                                   \
                                                                        \
    inline void prefix##_from_c_string(name* s, char* cstring) {        \
        u16 length = 0;                                                 \
        while(*cstring != '\0' || length == size) {                     \
            s->data[length] = *cstring;                                 \
            cstring++;                                                  \
            length++;                                                   \
        }                                                               \
        s->length = length;                                             \
    }                                                                   \
                                                                        \
    inline String prefix##_to_string(name* s) {                         \
        String result = {                                               \
            .data = (char*)&s->data,                                    \
            .length = s->length                                         \
        };                                                              \
        return result;                                                  \
    }

#define generate_fixed_string(name, prefix, size)                       \
    define_fixed_string(name, prefix, size);                            \
    implement_fixed_string(name, prefix, size);                         \


typedef struct bml_node {
    String name;
    String value;
    struct bml_node* child;
    struct bml_node* next;
} BMLNode;

BMLNode* bml_parse(Buffer buffer, Arena* arena);
void bml_print_node(BMLNode* node, int indent);



#define define_stack(name, prefix, type)                                \
    typedef struct {                                                    \
        type* data;                                                     \
        u16 fill;                                                       \
        u16 capacity;                                                   \
    } name;                                                             \
    extern void prefix##_init(name* s, int size);                       \
    extern void prefix##_deinit(name* s);                               \
    extern type* prefix##_push(name* s, type data);                     \
    extern void prefix##_reserve(name* s);                              \
    extern type* prefix##_pop(name* s);                                 \
    extern type* prefix##_top(name* s);                                 \
    extern b32 prefix##_is_empty(name* s);

#define implement_stack(name, prefix, type)                             \
    inline void prefix##_init(name* s, int size) {                      \
        s->data = malloc(size * sizeof(type));                          \
        s->fill = 0;                                                    \
        s->capacity = size;                                             \
    }                                                                   \
                                                                        \
    inline void prefix##_deinit(name* s) {                              \
        free(s->data);                                                  \
    }                                                                   \
                                                                        \
    inline type* prefix##_push(name* s, type data) {                    \
        if(s->fill == s->capacity) {                                    \
            s->capacity *= 2;                                           \
            /*TODO: One should check for null here...        */         \
            s->data = realloc(s->data, s->capacity * sizeof(type));     \
        }                                                               \
        s->data[s->fill] = data;                                        \
        s->fill++;                                                      \
        return s->data + s->fill - 1;                                   \
    }                                                                   \
                                                                        \
    inline void prefix##_reserve(name* s) {                             \
        if(s->fill == s->capacity) {                                    \
            s->capacity *= 2;                                           \
            /*TODO: One should check for null here...        */         \
            s->data = realloc(s->data, s->capacity * sizeof(type));     \
        }                                                               \
        s->fill++;                                                      \
    }                                                                   \
                                                                        \
    inline type* prefix##_top(name* s) {                                \
        return s->data + s->fill - 1;                                   \
    }                                                                   \
                                                                        \
    inline type* prefix##_pop(name* s) {                                \
        s->fill--;                                                      \
        return s->data + s->fill - 1;                                   \
    }                                                                   \
                                                                        \
    inline void prefix##_do_empty(name* s) {                            \
        s->fill = 0;                                                    \
        return;                                                         \
    }                                                                   \
                                                                        \
    inline b32 prefix##_is_empty(name* s) {                             \
        return s->fill == 0;                                            \
    }


#define generate_stack(name, prefix, type)                               \
    define_stack(name, prefix, type);                                    \
    implement_stack(name, prefix, type);



#define get16bits(d) (*((const u16*) (d)))
#define get32bits(d) (*((const u32*) (d)))
#define MAX_FILL_NOMINATOR 9
#define MAX_FILL_DENOMINATOR 10

u32 SuperFastHash(String str);

#define Hashmap(type)                                                   \
    struct {                                                            \
        struct {                                                        \
            type data;                                                  \
            u32  hash;                                                  \
        }* entries;                                                     \
        uint size;                                                      \
        uint fill;                                                      \
    }

#define define_hashmap(name, prefix, type, fun, cmp)                    \
    typedef Hashmap(type) name;                                         \
    void prefix##_init(name* map, int size);                            \
    void prefix##_deinit(name* map);                                    \
    void prefix##_resize(name* map);                                    \
    type* prefix##_find(name* map, type data);                          \
    type* prefix##_find_hash(name* map, type data, u32 hash);           \
    void prefix##_insert(name* map, type data);                         \
    void prefix##_insert2(name* map, type data, u32 hash);


#define implement_hashmap(name, prefix, type, fun, cmp)                 \
    void prefix##_init(name* map, int size) {                           \
        map->size = size;                                               \
        map->fill = 0;                                                  \
        map->entries = calloc(size, sizeof(*map->entries));             \
    }                                                                   \
                                                                        \
    void prefix##_deinit(name* map) {                                   \
        free(map->entries);                                             \
    }                                                                   \
                                                                        \
    void prefix##_insert2(name* map, type data, u32 hash);              \
    void prefix##_resize(name* map) {                                   \
        name new_map;                                                   \
        prefix##_init(&new_map, 2 * map->size);                         \
        for(int i = 0; i < map->size; i++) {                            \
            type data = map->entries[i].data;                           \
            u32 hash = map->entries[i].hash;                            \
            if(hash != 0x00000000) {                                    \
                prefix##_insert2(&new_map, data, hash);                 \
            }                                                           \
        }                                                               \
        free(map->entries);                                             \
        *map = new_map;                                                 \
    }                                                                   \
                                                                        \
    inline                                                              \
    void prefix##_insert2(name* map, type data, u32 hash) {             \
        u32 mask = (map->size - 1);                                     \
        u32 pos = hash & mask;                                          \
        u32 dist = 0;                                                   \
        for(;;) {                                                       \
            if(map->entries[pos].hash == 0x00000000) {                  \
                map->entries[pos].data = data;                          \
                map->entries[pos].hash = hash;                          \
                break;                                                  \
            } else {                                                    \
                u32 probed_hash = map->entries[pos].hash;               \
                u32 probed_dist = (probed_hash - pos) & mask;           \
                if(probed_dist < dist) {                                \
                    type tmp_data = data;                               \
                    data = map->entries[pos].data;                      \
                    map->entries[pos].data = tmp_data;                  \
                                                                        \
                    u32 tmp_hash = hash;                                \
                    hash = probed_hash;                                 \
                    map->entries[pos].hash = tmp_hash;                  \
                }                                                       \
            }                                                           \
            pos = (pos + 1) & mask;                                     \
            dist++;                                                     \
        }                                                               \
        map->fill++;                                                    \
        if(map->fill * MAX_FILL_DENOMINATOR > map->size * MAX_FILL_NOMINATOR) \
            prefix##_resize(map);                                       \
    }                                                                   \
                                                                        \
                                                                        \
    void prefix##_insert(name* map, type data) {                        \
        u32 hash = fun(data);                                           \
        prefix##_insert2(map, data, hash);                              \
    }                                                                   \
                                                                        \
    type* prefix##_find(name* map, type data) {                         \
        u32 hash = fun(data);                                           \
        return prefix##_find_hash(map, data, hash);                     \
    }                                                                   \
                                                                        \
    type* prefix##_find_hash(name* map, type data, u32 hash) {          \
        u32 mask = (map->size - 1);                                     \
        u32 dist = 0;                                                   \
        u32 pos = hash & mask;                                          \
        for(;;) {                                                       \
            if(map->entries[pos].hash == 0x00000000) {                  \
                return NULL;                                            \
            } else  if(cmp(map->entries[pos].data, data)) {             \
                return &map->entries[pos].data;                         \
            } else {                                                    \
                u32 probed_hash = map->entries[pos].hash;               \
                u32 probed_dist = (probed_hash - pos) & mask;           \
                if(probed_dist < dist) {                                \
                    return NULL;                                        \
                }                                                       \
                pos = (pos + 1) & mask;                                 \
                dist++;                                                 \
            }                                                           \
        }                                                               \
    }

#define generate_hashmap(name, type, fun, cmp)                          \
    define_hashmap(name, type, fun, cmp)                                \
    implement_hashmap(name, type, fun, cmp)

define_hashmap(StringMap, string_map, String, SuperFastHash, string_equal);

typedef struct {
    String name;
    Buffer buffer;
} VFSEntry;

u32 vfs_entry_hash(VFSEntry v);
u32 vfs_entry_equal(VFSEntry v1, VFSEntry v2);

define_hashmap(VFS, vfs, VFSEntry, vfs_entry_hash, vfs_entry_equal);

void vfs_add_dir(VFS* vfs, Path* path, Arena* arena);

#endif //BASE_GUARD

#ifdef BASE_IMPLEMENTATION
#ifndef BASE_IMPLEMENTATION_GUARD
#define BASE_IMPLEMENTATION_GUARD

#ifdef LINUX
#include <sys/mman.h>
void* page_alloc(uint num_pages) {
    return mmap(NULL, PAGE_SIZE * num_pages, PROT_READ | PROT_WRITE,
                MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
}

void page_free(void* page, uint num_pages) {
    munmap(page, num_pages * PAGE_SIZE);
}
#elif defined(WINDOWS)
#include <windows.h>
void* page_alloc(uint num_pages) {
    return VirtualAlloc(0, PAGE_SIZE * num_pages,
                        MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
}

void page_free(void* page, uint num_pages) {
    VirtualFree(page, num_pages * PAGE_SIZE, MEM_RELEASE);
}
#endif



Arena arena_create(ulong size) {
    Buffer buffer;
    buffer.begin = page_alloc((size + PAGE_SIZE - 1) / PAGE_SIZE);
    buffer.end   = buffer.begin + size;
    return (Arena){
        .buffer = buffer,
        .current = buffer.begin
    };
}

Arena arena_create_buffer(Buffer buffer) {
    return (Arena){
        .buffer = buffer,
        .current = buffer.begin
    };
}

void arena_clear(Arena* a) {
    a->current = a->buffer.begin;
}

void* arena_alloc(Arena* a, ulong size, ulong align) {
    assert(a && a->current >= a->buffer.begin && a->current <= a->buffer.end);
    if (!a) return 0;
    align = align == 0 ? 0 : align - 1;
    void* result = ((void*)(((iptr)(a->current + align)&~align)));
    assert(result + size < a->buffer.end);
    if(result + size >= a->buffer.end) return NULL;
    /* { */
    /*     long num_pages = (sizeof(page_list_t) + size + align + PAGE_SIZE - 1) / PAGE_SIZE; */
    /*     void* page = (page_list_t*)page_list_alloc(a->free_list, num_pages); */
    /*     page_list_add(&a->used_list_sentinel, page, num_pages); */
    /*     result = page + sizeof(page_list_t); */
    /*     a->buffer.end = result + num_pages * PAGE_SIZE; */
    /* } */
    a->current = result + size;
    return result;
}

Arena arena_subarena(Arena* a, ulong size) {
    void* data = arena_alloc(a, size, alignof(void*));
    Arena result = {
        .buffer.begin = data,
        .buffer.end   = data + size,
        .current      = data
    };
    return result;
}

Temp temp_begin(Arena* a) {
    return (Temp) {
        .arena = a,
        .old_current = a->current
    };
}

void temp_end(Temp temp) {
    temp.arena->current = temp.old_current;
}

b32 string_equal(String s1, String s2) {
    if(s1.length != s2.length) return 0;
    for(int i = 0; i < s1.length; i++) {
        if(s1.data[i] != s2.data[i]) return 0;
    }
    return 1;
}

u64 string_to_u64_base(String s, int base) {
    u64 num = 0;
    for(int i = 0; i < s.length; i++) {
        int digit = 0;
        if(s.data[i] >= '0' && s.data[i] <= '9') {
            digit = s.data[i] - '0';
        } else if(s.data[i] >= 'A' && s.data[i] <= 'Z') {
            digit = s.data[i] - 'A' + 10;
        } else if(s.data[i] >= 'a' && s.data[i] <= 'z') {
            digit = s.data[i] - 'a' + 10;
        } else {
            break;
        }
        if(digit >= base) break;
        num = num * base + digit;
    }
    return num;
}

u64 string_to_u64(String s) {
    int base = 10;
    if(s.length >= 3 && s.data[0] == '0' && (s.data[1] == 'x' || s.data[1] == 'X')) {
        base = 16; s.data += 2; s.length -= 2;
    } else if(s.length >= 3 && s.data[0] == '0' && (s.data[1] == 'b' || s.data[1] == 'B')) {
        base =  2; s.data += 2; s.length -= 2;
    } else if(s.length >= 3 && s.data[0] == '0' && (s.data[1] == 'o' || s.data[1] == 'O')) {
        base =  8; s.data += 2; s.length -= 2;
    } else if(s.length >= 2 && s.data[0] == '%') {
        base =  2; s.data += 1; s.length -= 1;
    } else if(s.length >= 2 && s.data[0] == '$') {
        base = 16; s.data += 1; s.length -= 1;
    }
    return string_to_u64_base(s, base);
}



String string_from_c_string(char* buf) {
    int length = 0;
    String s = {
        .data = buf
    };
    while(*buf != '\0') {
        buf++;
        length++;
    }
    
    s.length = length;
    //s.params = STRING_PARAMETER_ZERO_TERMINATED;
    return s;
}

#ifdef LINUX
#include <unistd.h>
#include <sys/stat.h>

void path_init_c(Path* path, char* name) {
    path->fd_ = open(name, O_RDONLY);
    path->has_stats = 0;
    path->invalid = 0;
}

void path_init(Path* path, String name) {
    char name_c[PATH_MAX];
    if(name.length >= PATH_MAX - 1) {
        fprintf(stderr, "File name \"%.*s\" is to long.\n", name.length, name.data);
    }
    strncpy(name_c, name.data, name.length);
    name_c[name.length] = '\0';
    path_init_c(path, name_c);
}

void path_init_from_c(Path* path, Path* dir, char* name) {
    path->fd_ = openat(dir->fd_, name, O_RDONLY);
    path->has_stats = 0;
    path->invalid = 0;
}


void path_init_from(Path* path, Path* dir, String name) {
    char name_c[PATH_MAX];
    if(name.length >= PATH_MAX - 1) {
        fprintf(stderr, "File name \"%.*s\" is to long.\n", name.length, name.data);
    }
    strncpy(name_c, name.data, name.length);
    name_c[name.length] = '\0';
    path_init_c(path, name_c);
}


void path_init_working_directory(Path* path) {
    path_init_c(path, ".");
}

void path_load_stats(Path* path) {
    if(!path->has_stats) {
        fstat(path->fd_, &path->st_buf_);        
        path->has_stats = 1;
    }
}

void path_copy(Path* orig, Path* copy) {
    *copy = (Path){ 0 };
    copy->fd_ = dup(orig->fd_);
    if(orig->has_stats) path_load_stats(copy);
}


Buffer path_open_file_aligned(Path* path, Arena* arena, int alignment) {
    path_load_stats(path);
    size_t file_size = path->st_buf_.st_size;
    Buffer result = { 0 };
    result.begin = arena_alloc(arena, file_size, alignment);
    result.end = result.begin + file_size;
    read(path->fd_, result.begin, file_size);
    return result;
}

Buffer path_open_file(Path* path, Arena* arena) {
    return path_open_file_aligned(path, arena, 1);
}

void path_close(Path* path) {
    if(!path->invalid) {
        close(path->fd_);
        path->invalid = 1;
    }
}

void path_navigate_c(Path* path, char* name) {
    int new_fd = openat(path->fd_, name, O_RDONLY);
    path_close(path);
    path->fd_ = new_fd;
    path->has_stats = 0;
    path->invalid = 0;
}

void path_navigate(Path* path, String name) {
    char name_c[PATH_MAX];
    if(name.length >= PATH_MAX - 1) {
        fprintf(stderr, "File name \"%.*s\" is to long.\n", name.length, name.data);
    }
    strncpy(name_c, name.data, name.length);
    name_c[name.length] = '\0';
    path_navigate_c(path, name_c);
}

int path_is_directory(Path* path) {
    path_load_stats(path);
    return path->st_buf_.st_mode == S_IFDIR;
}

ulong path_get_file_size(Path* path) {
    path_load_stats(path);
    return path->st_buf_.st_size;
}

String path_get_name(Path* path, Arena* arena) {
    char proc_path_name[256];
    sprintf(proc_path_name, "/proc/self/fd/%i", path->fd_);
    struct stat sb;
    if(lstat(proc_path_name, &sb) == -1) {
        perror("lstat");
        exit(1);
    }
    int supposed_file_name_length = sb.st_size + 1;
    if(sb.st_size == 0) supposed_file_name_length = PATH_MAX;
    String result;
    result.data   = arena_alloc_array(arena, supposed_file_name_length, char);
    result.length = readlink(proc_path_name, result.data,
                             supposed_file_name_length);
    if(result.length == -1) {
        perror("readlink");
        exit(1);
    }
    return result;
}


String path_get_base_name(Path* path, Arena* arena) {
    String result;
    result.data = arena_alloc_array(arena, 256, char);
    Temp temp = temp_begin(arena);
    String path_name = path_get_name(path, arena);
    char* last_char = path_name.data + path_name.length - 1;
    char* last_slash = last_char;
    // If path_name == '/':
    if(*last_char == '/' && path_name.length == 1) {
        result.data[0] = '/';
        result.length  = 1;
        return result;
    }
    
    // If path_name ends in '/'s, do not include these '/':
    while(*last_char == '/') {
        last_char--;
        last_slash--;
    }

    //Search for the last slash:
    for(; last_slash >= path_name.data; last_slash--) {
        if(*last_slash == '/') {
            break;
        }
    }

    if(last_slash >= path_name.data - 1) {
        result.length = last_char - last_slash;
        strncpy(result.data, last_slash + 1, result.length);
    } else {
        assert(0 && "This should not happen.");
        exit(1);
    }
    
    temp_end(temp);
    return result;
}

int dir_iter_next(DirIter* dir_iter) {
    struct dirent* de;
    do {
        de = readdir(dir_iter->dp_);
        if(!de) return 0;
        if(!strcmp(de->d_name, "."))
            continue;
        if(!strcmp(de->d_name, ".."))
            continue;
        break;
    } while(1);
    path_close(&dir_iter->path);
    dir_iter->path.fd_ = openat(dir_iter->dir_path->fd_, de->d_name, O_RDONLY);
    dir_iter->path.has_stats = 0;
    dir_iter->path.invalid   = 0;
    return 1;
}

DirIter dir_iter_begin(Path* path) {
    DirIter result = { 0 };
    result.dp_ = fdopendir(path->fd_);

    if(!result.dp_) {
        fprintf(stderr, strerror(errno));
        exit(1);
    }
    result.dir_path = path;
    return result;
}

void dir_iter_end(DirIter* dir_iter) {
    path_close(&dir_iter->path);
    closedir(dir_iter->dp_);
}
#endif

implement_hashmap(StringMap, string_map, String, SuperFastHash, string_equal);

u32 SuperFastHash(String str) {
    u32 hash = str.length, tmp;
    uint length = str.length;
    char rem;

    if (length <= 0 || str.data == NULL) return 0;

    rem = length & 3;
    length >>= 2;

    /* Main loop */
    for (;length > 0; length--) {
        hash += get16bits(str.data);
        tmp   = (get16bits(str.data+2) << 11) ^ hash;
        hash  = (hash << 16) ^ tmp;
        str.data += 2*sizeof(u16);
        hash += hash >> 11;
    }

    /* Handle end cases */
    switch (rem) {
    case 3: hash += get16bits(str.data);
        hash ^= hash << 16;
        hash ^= ((i8)str.data[sizeof(u16)]) << 18;
        hash += hash >> 11;
        break;
    case 2: hash += get16bits(str.data);
        hash ^= hash << 11;
        hash += hash >> 17;
        break;
    case 1: hash += (i8)*str.data;
        hash ^= hash << 10;
        hash += hash >> 1;
    }

    /* Force "avalanching" of final 127 bits */
    hash ^= hash << 3;
    hash += hash >> 5;
    hash ^= hash << 4;
    hash += hash >> 17;
    hash ^= hash << 25;
    hash += hash >> 6;

    return hash;
}

u32 vfs_entry_hash(VFSEntry v) {
    return SuperFastHash(v.name);
}
u32 vfs_entry_equal(VFSEntry v1, VFSEntry v2) {
    return string_equal(v1.name, v2.name);
}

implement_hashmap(VFS, vfs, VFSEntry, vfs_entry_hash, vfs_entry_equal);


#ifdef NDEBUG
void error_at_pos2(TextPos text_pos) {
    fprintf(stderr, "%.*s:%i:%i: error\n", text_pos.file_name.length, text_pos.file_name.data,
        text_pos.line_number, text_pos.line_pos);
    char* line_end = text_pos.line_start;
    int length = 0;
    while(*line_end && *line_end != '\n') {
        if(line_end - text_pos.line_start < text_pos.line_pos)
        if(*line_end == '\t') {
            length += 8;
        } else {
            length += 1;
        }            
        line_end++;
    }
    fprintf(stderr, "%.*s\n", line_end - text_pos.line_start, text_pos.line_start);
    for(int i = 0; i < length; i++) fprintf(stderr, "~");
    fprintf(stderr, "^\n");
}
#else

void error_at_pos2(TextPos text_pos, int line_num) {
    fprintf(stderr, "%.*s:%i:%i: error from line %i\n", text_pos.file_name.length, text_pos.file_name.data,
            text_pos.line_number, text_pos.line_pos, line_num);
    char* line_end = text_pos.line_start;
    int length = 0;
    while(*line_end && *line_end != '\n') {
        if(line_end - text_pos.line_start < text_pos.line_pos) {
            if(*line_end == '\t') {
                length += 8;
            } else {
                length += 1;
            }
        }
        line_end++;
    }
    fprintf(stderr, "%.*s\n", (int)(line_end - text_pos.line_start), text_pos.line_start);
    for(int i = 0; i < length; i++) fprintf(stderr, "~");
    fprintf(stderr, "^\n");
}
#endif

typedef struct {
    Buffer  buffer;
    char*   pos;
    TextPos text_pos;
    Arena*  arena;
    int     had_new_line;
} bml_parser_state_t;


void bml_parser_advance(bml_parser_state_t* parser) {
    parser->pos++;
    parser->text_pos.line_pos++;
    if(parser->had_new_line) {
        parser->text_pos.line_pos = 0;
        parser->text_pos.line_number++;
    }
}

int bml_is_valid_char(char p)  {  //A-Z, a-z, 0-9, -.
    return p - 'A' < 26u || p - 'a' < 26u || p - '0' < 10u || p - '-' < 2u;
}


void bml_parse_name(BMLNode* node, bml_parser_state_t* parser) {
    char* begin = parser->pos;
    while(bml_is_valid_char(parser->pos[0])) bml_parser_advance(parser);
    if(parser->pos == begin) {
        error_at_pos(parser->text_pos);
        fprintf(stderr, "Invalid node name.\n");
        exit(1);
    }
    node->name = (String) {
        .data = begin,
        .length = parser->pos - begin
    };
}

void bml_parse_data(BMLNode* node, bml_parser_state_t* parser) {
    char* begin = NULL, *end = NULL;
    if(parser->pos[0] == '=' && parser->pos[1] == '"') {
        bml_parser_advance(parser);
        bml_parser_advance(parser);
        begin = parser->pos;
        int cont = 1;
        while(cont) {
            switch(parser->pos[0]) {
            case '"':
                cont = 0;
                break;
            case '\0':
            case '\n':
                error_at_pos(parser->text_pos);
                fprintf(stderr, "Unescaped value.\n");
                exit(1);
            case '\r':
                if(parser->pos[1] == '\n') {
                    error_at_pos(parser->text_pos);
                    fprintf(stderr, "Unescaped value.\n");
                    exit(1);
                }
            default:
                bml_parser_advance(parser);
            }
        }
        end = parser->pos;
        bml_parser_advance(parser);
    } else if(parser->pos[0] == '=') {
        bml_parser_advance(parser);
        begin = parser->pos;
        int cont = 1;
        while(cont) {
            switch(parser->pos[0]) {
            case '\0':
            case '\n':
            case ' ':
                cont = 0;
                break;
            case '"':
                error_at_pos(parser->text_pos);
                fprintf(stderr, "Illegal character in value.\n");
                exit(1);
            case '\r':
                if(parser->pos[1] == '\n') {
                    cont = 0;
                }
                bml_parser_advance(parser);
                break;
            default:
                bml_parser_advance(parser);
            }
        }
        end = parser->pos;
    } else if(parser->pos[0] == ':') {
        bml_parser_advance(parser);
        begin = parser->pos;
        int cont = 1;
        while(cont) {
            switch(parser->pos[0]) {
            case '\0':
            case '\n':
                cont = 0;
                break;
            default:
                bml_parser_advance(parser);
            }
        }
        end = parser->pos;
    }
    node->value = (String) {
        .data = begin,
        .length = end - begin
    };
}

void bml_parse_attributes(BMLNode* node, bml_parser_state_t* parser) {
    BMLNode** next_node_ptr = &node->child;
    while(1) {
        if(!parser->pos[0]) break;
        if(parser->pos[0] == '\n') {
            bml_parser_advance(parser);
            break;
        }
        if(parser->pos[0] != ' ') {
            error_at_pos(parser->text_pos);
            fprintf(stderr, "Invalid node name.\n");
            exit(1);
        }
        while(parser->pos[0] == ' ') bml_parser_advance(parser); //skip excess spaces
        if(parser->pos[0] == '/' && parser->pos[1] == '/') {     //skip comments
            while(parser->pos[0] && parser->pos[0] != '\n' &&
                  !(parser->pos[0] == '\r' && parser->pos[1] == '\n')) {
                bml_parser_advance(parser);
            }
            break;
        }

        BMLNode* new_node = (BMLNode*)arena_alloc_type(parser->arena, BMLNode);
        *new_node = (BMLNode){ 0 };
        char* begin = parser->pos;
        while(bml_is_valid_char(parser->pos[0])) bml_parser_advance(parser);
        if(parser->pos == begin) {
            error_at_pos(parser->text_pos);
            fprintf(stderr, "Invalid attribute name.\n");
            exit(1);
        }
        new_node->name = (String) {
            .data = begin,
            .length = parser->pos - begin
        };
        bml_parse_data(new_node, parser);
        *next_node_ptr = new_node;
        next_node_ptr = &new_node->next;
    }
}

int bml_parse_indent(bml_parser_state_t* parser) {
    int result = 0;
    while(parser->pos[0] == ' ' || parser->pos[0] == '\t') {
        result++;
        bml_parser_advance(parser);
    }
    return result;
}

BMLNode* bml_parse_node(bml_parser_state_t* parser, int* indent) {
    if(parser->pos[0] == '\0') {
        return NULL;
    }
    if(parser->pos[0] == '\n') {
        if(*indent == 0) {
            bml_parser_advance(parser);
            *indent = bml_parse_indent(parser);
        }
    }
    BMLNode* node = arena_alloc_type(parser->arena, BMLNode);
    *node = (BMLNode) { 0 };
    bml_parse_name(node, parser);
    bml_parse_data(node, parser);

    BMLNode** next_node_ptr = &node->child;

    // Parse attributes
    while(1) {
        if(!parser->pos[0]) break;
        if(parser->pos[0] == '\n') {
            bml_parser_advance(parser);
            break;
        }
        if(parser->pos[0] != ' ') {
            error_at_pos(parser->text_pos);
            fprintf(stderr, "Invalid node name.\n");
            exit(1);
        }
        while(parser->pos[0] == ' ') bml_parser_advance(parser); //skip excess spaces
        if(parser->pos[0] == '/' && parser->pos[1] == '/') {     //skip comments
            while(parser->pos[0] && parser->pos[0] != '\n' &&
                  !(parser->pos[0] == '\r' && parser->pos[1] == '\n')) {
                bml_parser_advance(parser);
            }
            break;
        }

        BMLNode* new_node = arena_alloc_type(parser->arena, BMLNode);
        *new_node = (BMLNode){ 0 };
        char* begin = parser->pos;
        while(bml_is_valid_char(parser->pos[0])) bml_parser_advance(parser);
        if(parser->pos == begin) {
            error_at_pos(parser->text_pos);
            fprintf(stderr, "Invalid attribute name.\n");
            exit(1);
        }
        new_node->name = (String) {
            .data = begin,
            .length = parser->pos - begin
        };
        bml_parse_data(new_node, parser);
        *next_node_ptr = new_node;
        next_node_ptr = &new_node->next;
    }
    
    
    // Parse child nodes
    if(parser->pos[0] == '\0') {
        return node;
    }

    int child_indent = bml_parse_indent(parser);
    int new_indent = child_indent;
    while(1) {
        if(parser->pos[0] == '\0') {
            return node;
        }
        if(new_indent <= *indent) {
            *indent = new_indent;
            return node;
        } else if(new_indent != child_indent) {
            error_at_pos(parser->text_pos);
            fprintf(stderr, "Indentation error.\n");
            exit(1);
        } else {
            BMLNode* new_node = bml_parse_node(parser, &new_indent);
            *next_node_ptr = new_node;
            next_node_ptr = &new_node->next;
            if(new_indent <= *indent) {
                *indent = new_indent;
                return node;
            } else if(new_indent != child_indent) {
                error_at_pos(parser->text_pos);
                fprintf(stderr, "Indentation error.\n");
                exit(1);
            }
        }
    }
}

BMLNode* bml_parse(Buffer buffer, Arena* arena) {
    bml_parser_state_t parser = { 0 };
    parser.buffer = buffer;
    parser.pos    = (char*)buffer.begin;
    parser.text_pos.line_number = 1;
    parser.arena = arena;
    int indent = 0;
    BMLNode* root;
    BMLNode** next_node_ptr = &root;
    while(parser.pos[0] != '\0') {
        *next_node_ptr =  bml_parse_node(&parser, &indent);
        next_node_ptr = &((*next_node_ptr)->next);
    }
    return root;
}

void bml_print_node(BMLNode* node, int indent) {
    if(node) {
        printf("%*s%.*s: %.*s\n", indent, "", node->name.length, node->name.data, node->value.length, node->value.data);
        bml_print_node(node->child, indent + 2);
        bml_print_node(node->next, indent);
    }
}



void vfs_add_dir(VFS* vfs, Path* path, Arena* arena) {
    DirIter dir_iter = dir_iter_begin(path);
    while(dir_iter_next(&dir_iter)) {
        VFSEntry entry = { 0 };
        entry.name   = path_get_base_name(&dir_iter.path, arena);
        entry.buffer = path_open_file(&dir_iter.path, arena);
        vfs_insert(vfs, entry);
    }
    dir_iter_end(&dir_iter);
}

#endif //BASE_IMPLEMENTATION_GUARD
#endif //BASE_IMPLEMENTATION
