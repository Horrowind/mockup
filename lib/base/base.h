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

typedef unsigned int uint;
typedef unsigned short ushort;
typedef unsigned char uchar;

typedef uintptr_t uintptr;



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

typedef struct page_list {
    struct page_list* next;
    struct page_list* prev;
    int num_pages;
} page_list_t;

typedef struct {
    void* begin;
    void* end;
} buffer_t;

typedef struct {
    buffer_t buffer;
    void*    current;
} arena_t;

typedef struct {
    arena_t* arena;
    void*    old_current;
} temp_t;

void page_list_init(page_list_t* list);
void page_list_deinit(page_list_t* list);

arena_t arena_create(ulong size);
arena_t arena_create_buffer(buffer_t buffer);
void    arena_clear(arena_t* a);
arena_t arena_subarena(arena_t* a, ulong size);
void*   arena_alloc(arena_t* a, ulong size, ulong align);
#define arena_alloc_type(a,type) ((type*)arena_alloc((a), sizeof(type), alignof(type)))
#define arena_alloc_array(a,n,type) (type*)arena_alloc((a), (n)*sizeof(type), alignof(type))

temp_t temp_begin(arena_t* a);
void temp_end(temp_t tmp);

typedef struct {
    char* data;
    ushort length;
} string_t;

#define L(cstr) (string_t){.data = cstr, .length = array_length(cstr) - 1 }

b32 string_equal(string_t s1, string_t s2);
string_t string_from_c_string(char* buf);
u64 string_to_u64(string_t s);
u64 string_to_u64_base(string_t s, int base);

#define string_split_foreach(s, t, c)                                   \
    string_t t = { .data = (s).data };                                  \
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
} path_t;

typedef struct {
    path_t path;
    path_t* dir_path;
#ifdef LINUX
    DIR* dp_;
#endif
} dir_iter_t;

void       path_init(path_t* path, string_t name);
void       path_init_c(path_t* path, char* name);
void       path_init_working_directory(path_t* path);
void       path_init_from_c(path_t* path, path_t* dir, char* name);
void       path_init_from(path_t* path, path_t* dir, string_t name);
void       path_copy(path_t* orig, path_t* copy);
buffer_t   path_open_file(path_t* path, arena_t* arena);
buffer_t   path_open_file_aligned(path_t* path, arena_t* arena, int alignment);
void       path_close(path_t* path);
void       path_navigate(path_t* path, string_t name);
void       path_navigate_c(path_t* path, char* name);
ulong      path_get_file_size(path_t* path);
int        path_is_directory(path_t* path);
string_t   path_get_name(path_t* path, arena_t* arena); //TODO
string_t   path_get_base_name(path_t* path, arena_t* arena); //TODO
dir_iter_t dir_iter_begin(path_t* path);
int        dir_iter_next(dir_iter_t* dir_iter);
void       dir_iter_end(dir_iter_t* dir_iter);

typedef struct {
    page_list_t* free_list;
    void* data;
    ulong length;
    ulong fill;
} pool_t;

/* void  pool_init(pool_t* pool, page_list_t* free_list); */
/* void* pool_alloc(pool_t* pool,ulong bytes, ulong align); */
/* void  pool_empty(pool_t* pool); */
/* void  pool_deinit(pool_t* pool); */
/* #define pool_alloc_type(a,type) ((type*)pool_alloc(a, (int)sizeof(type), alignof(type))) */
/* #define pool_alloc_array(a,n,type) (type*)pool_alloc(a, (n)*sizeof(type), alignof(type)) */

typedef struct {
    string_t file_name;
    int      line_number;
    int      line_pos;
    char*    line_start;
} text_pos_t;



#ifdef NDEBUG
#define error_at_pos(text_pos) error_at_pos2(text_pos)
void error_at_pos2(text_pos_t text_pos);
#else
#define error_at_pos(text_pos) error_at_pos2(text_pos, __LINE__)
void error_at_pos2(text_pos_t text_pos, int line_num);
#endif

#define fixed_string_t(size)                                            \
    struct {                                                            \
        char data[size];                                                \
        u16 length;                                                     \
    } 

#define define_fixed_string(name, size)                                 \
    typedef fixed_string_t(size) name##_t;                              \
    void name##_append(name##_t s1, name##_t s2);                       \
    name##_t name##_cat(name##_t s1, name##_t s2);                      \
    string_t name##_to_string(name##_t* s);                             \
    void name##_from_c_string(name##_t* s, char* cstring);              \
    char* name##_find_char(name##_t* s, char c);

#define implement_fixed_string(name, size)                              \
    inline void name##_append(name##_t s1, name##_t s2) {               \
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
    inline name##_t name##_cat(name##_t s1, name##_t s2) {              \
        name##_t result = s1;                                           \
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
    inline char* name##_find_char(name##_t* s, char c) {                \
        char* iter = s->data;                                           \
        for(int i = 0; i < s->length; i++) {                            \
            if(*iter == c) return iter;                                 \
            iter++;                                                     \
        }                                                               \
    }                                                                   \
                                                                        \
    inline void name##_from_c_string(name##_t* s, char* cstring) {      \
        u16 length = 0;                                                 \
        while(*cstring != '\0' || length == size) {                     \
            s->data[length] = *cstring;                                 \
            cstring++;                                                  \
            length++;                                                   \
        }                                                               \
        s->length = length;                                             \
    }                                                                   \
                                                                        \
    inline string_t name##_to_string(name##_t* s) {                     \
        string_t result = {                                             \
            .data = (char*)&s->data,                                    \
            .length = s->length                                         \
        };                                                              \
        return result;                                                  \
    }

#define generate_fixed_string(name, size)                               \
    define_fixed_string(name, size);                                    \
    implement_fixed_string(name, size);                                 \


typedef struct bml_node {
    string_t name;
    string_t value;
    struct bml_node* child;
    struct bml_node* next;
} bml_node_t;

bml_node_t* bml_parse(buffer_t buffer, arena_t* arena);
void bml_print_node(bml_node_t* node, int indent);



#define define_stack(name, type)                                        \
    typedef struct {                                                    \
        type* data;                                                     \
        u16 fill;                                                       \
        u16 capacity;                                                   \
    } name##_t;                                                         \
    extern void name##_init(name##_t* s, int size);                     \
    extern void name##_deinit(name##_t* s);                             \
    extern type* name##_push(name##_t* s, type data);                   \
    extern void name##_reserve(name##_t* s);                            \
    extern type* name##_pop(name##_t* s);                               \
    extern type* name##_top(name##_t* s);                               \
    extern b32 name##_is_empty(name##_t* s);

#define implement_stack(name, type)                                     \
    inline void name##_init(name##_t* s, int size) {                    \
        s->data = malloc(size * sizeof(type));                          \
        s->fill = 0;                                                    \
        s->capacity = size;                                             \
    }                                                                   \
                                                                        \
    inline void name##_deinit(name##_t* s) {                            \
        free(s->data);                                                  \
    }                                                                   \
                                                                        \
    inline type* name##_push(name##_t* s, type data) {                  \
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
    inline void name##_reserve(name##_t* s) {                           \
        if(s->fill == s->capacity) {                                    \
            s->capacity *= 2;                                           \
            /*TODO: One should check for null here...        */         \
            s->data = realloc(s->data, s->capacity * sizeof(type));     \
        }                                                               \
        s->fill++;                                                      \
    }                                                                   \
                                                                        \
    inline type* name##_top(name##_t* s) {                              \
        return s->data + s->fill - 1;                                   \
    }                                                                   \
                                                                        \
    inline type* name##_pop(name##_t* s) {                              \
        s->fill--;                                                      \
        return s->data + s->fill - 1;                                   \
    }                                                                   \
                                                                        \
    inline void name##_do_empty(name##_t* s) {                          \
        s->fill = 0;                                                    \
        return;                                                         \
    }                                                                   \
                                                                        \
    inline b32 name##_is_empty(name##_t* s) {                           \
        return s->fill == 0;                                            \
    }


#define generate_stack(name, type)                                      \
    define_stack(name, type);                                           \
    implement_stack(name, type);



#define get16bits(d) (*((const u16*) (d)))
#define get32bits(d) (*((const u32*) (d)))
#define MAX_FILL_NOMINATOR 9
#define MAX_FILL_DENOMINATOR 10

u32 SuperFastHash (string_t str);

#define hashmap_t(type)                                                 \
    struct {                                                            \
        struct {                                                        \
            type data;                                                  \
            u32  hash;                                                  \
        }* entries;                                                     \
        uint size;                                                      \
        uint fill;                                                      \
    }

#define define_hashmap(name, type, fun, cmp)                            \
    typedef hashmap_t(type) name##_t;                                   \
    void name##_init(name##_t* map, int size);                          \
    void name##_deinit(name##_t* map);                                  \
    void name##_resize(name##_t* map);                                  \
    type* name##_find(name##_t* map, type data);                        \
    type* name##_find_hash(name##_t* map, type data, u32 hash);         \
    void name##_insert(name##_t* map, type data);                       \
    void name##_insert2(name##_t* map, type data, u32 hash);


#define implement_hashmap(name, type, fun, cmp)                         \
    void name##_init(name##_t* map, int size) {                         \
        map->size = size;                                               \
        map->fill = 0;                                                  \
        map->entries = calloc(size, sizeof(*map->entries));             \
    }                                                                   \
                                                                        \
    void name##_deinit(name##_t* map) {                                 \
        free(map->entries);                                             \
    }                                                                   \
                                                                        \
    void name##_insert2(name##_t* map, type data, u32 hash);            \
    void name##_resize(name##_t* map) {                                 \
        name##_t new_map;                                               \
        name##_init(&new_map, 2 * map->size);                           \
        for(int i = 0; i < map->size; i++) {                            \
            type data = map->entries[i].data;                           \
            u32 hash = map->entries[i].hash;                            \
            if(hash != 0x00000000) {                                    \
                name##_insert2(&new_map, data, hash);                   \
            }                                                           \
        }                                                               \
        free(map->entries);                                             \
        *map = new_map;                                                 \
    }                                                                   \
                                                                        \
    inline                                                              \
    void name##_insert2(name##_t* map, type data, u32 hash) {           \
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
            name##_resize(map);                                         \
    }                                                                   \
                                                                        \
                                                                        \
    void name##_insert(name##_t* map, type data) {                      \
        u32 hash = fun(data);                                           \
        name##_insert2(map, data, hash);                                \
    }                                                                   \
                                                                        \
    type* name##_find(name##_t* map, type data) {                       \
        u32 hash = fun(data);                                           \
        return name##_find_hash(map, data, hash);                       \
    }                                                                   \
                                                                        \
    type* name##_find_hash(name##_t* map, type data, u32 hash) {        \
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

define_hashmap(string_map, string_t, SuperFastHash, string_equal);

typedef struct {
    string_t name;
    buffer_t buffer;
} vfs_entry_t;

u32 vfs_entry_hash(vfs_entry_t v);
u32 vfs_entry_equal(vfs_entry_t v1, vfs_entry_t v2);

define_hashmap(vfs, vfs_entry_t, vfs_entry_hash, vfs_entry_equal);

void vfs_add_dir(vfs_t* vfs, path_t* path, arena_t* arena);

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
#elif WINDOWS
#include <windows.h>
void* page_alloc(uint num_pages) {
    return VirtualAlloc(0, PAGE_SIZE * num_pages,
                        MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
}

void page_free(void* page, uint num_pages) {
    VirtualFree(page, num_pages * PAGE_SIZE, MEM_RELEASE);
}
#endif


void page_list_init(page_list_t* l) {
    l->next = l;
    l->prev = l;
    l->num_pages = 0;
}

void page_list_add(page_list_t* l, void* page, int num_pages) {
    page_list_t* page_list = (page_list_t*)page;
    page_list->num_pages = num_pages;
    page_list->prev = l;
    page_list->next = l->next;
    l->next->prev = page_list;
    l->next = page_list;
}

void page_list_add_list(page_list_t* l1, page_list_t* l2) {
    l1->prev->next = l2->next;
    l2->next->prev = l1->prev;
    l1->prev       = l2->prev;
    l2->prev->next = l1;
}

void* page_list_alloc(page_list_t* sentinel, int num_pages) {
    void* result = NULL;
    page_list_t* l = sentinel->next;
    while(l != sentinel && l->num_pages > num_pages) {
        l = l->next;
    }
    if(l != sentinel) {
        result = l;
        if(l->num_pages == num_pages) {
            l->prev->next = l->next;
            l->next->prev = l->prev;
        } else {
            int new_num_pages = l->num_pages - num_pages;
            page_list_t* new_page = (page_list_t*)(((void*)l) + num_pages * PAGE_SIZE);
            new_page->num_pages = new_num_pages;
            new_page->next = l->next;
            new_page->prev = l->prev;
            l->prev->next = l->next;
            l->next->prev = l->prev;
        }
    } else {
        result = page_alloc(num_pages);
    }
    return result;
}

void page_list_deinit(page_list_t* l) {
    assert(l);
    while(l) {
        page_list_t* next = l->next;
        page_free(l, l->num_pages);
        l = next;
    }
}

arena_t arena_create(ulong size) {
    buffer_t buffer;
    buffer.begin = page_alloc((size + PAGE_SIZE - 1) / PAGE_SIZE);
    buffer.end   = buffer.begin + size;
    return (arena_t){
        .buffer = buffer,
        .current = buffer.begin
    };
}

arena_t arena_create_buffer(buffer_t buffer) {
    return (arena_t){
        .buffer = buffer,
        .current = buffer.begin
    };
}

void arena_clear(arena_t* a) {
    a->current = a->buffer.begin;
}

void* arena_alloc(arena_t* a, ulong size, ulong align) {
    assert(a && a->current >= a->buffer.begin && a->current <= a->buffer.end);
    if (!a) return 0;
    align = align == 0 ? 0 : align - 1;
    void* result = ((void*)(((intptr_t)(a->current + align)&~align)));
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

arena_t arena_subarena(arena_t* a, ulong size) {
    void* data = arena_alloc(a, size, alignof(void*));
    arena_t result = {
        .buffer.begin = data,
        .buffer.end   = data + size,
        .current      = data
    };
    return result;
}

temp_t temp_begin(arena_t* a) {
    return (temp_t) {
        .arena = a,
        .old_current = a->current
    };
}

void temp_end(temp_t temp) {
    temp.arena->current = temp.old_current;
}



/* temp_memory_t temp_memory_begin(arena_t* a) */
/* { */
/*     temp_memory_t res; */
/*     res.used = a->blk ? a->blk->used: 0; */
/*     res.blk = a->blk; */
/*     res.arena = a; */
/*     return res; */
/* } */
/* void temp_memory_end(temp_memory_t tmp) */
/* { */
/*    arena_t* a = tmp.arena; */
/*     while (a->blk != tmp.blk) */
/*         arena_free_last_blk(a); */
/*     if (a->blk) a->blk->used = tmp.used; */
/* } */

b32 string_equal(string_t s1, string_t s2) {
    if(s1.length != s2.length) return 0;
    for(int i = 0; i < s1.length; i++) {
        if(s1.data[i] != s2.data[i]) return 0;
    }
    return 1;
}

u64 string_to_u64_base(string_t s, int base) {
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

u64 string_to_u64(string_t s) {
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



string_t string_from_c_string(char* buf) {
    int length = 0;
    string_t s = {
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

void path_init_c(path_t* path, char* name) {
    path->fd_ = open(name, O_RDONLY);
    path->has_stats = 0;
    path->invalid = 0;
}

void path_init(path_t* path, string_t name) {
    char name_c[PATH_MAX];
    if(name.length >= PATH_MAX - 1) {
        fprintf(stderr, "File name \"%.*s\" is to long.\n", name.length, name.data);
    }
    strncpy(name_c, name.data, name.length);
    name_c[name.length] = '\0';
    path_init_c(path, name_c);
}

void path_init_from_c(path_t* path, path_t* dir, char* name) {
    path->fd_ = openat(dir->fd_, name, O_RDONLY);
    path->has_stats = 0;
    path->invalid = 0;
}


void path_init_from(path_t* path, path_t* dir, string_t name) {
    char name_c[PATH_MAX];
    if(name.length >= PATH_MAX - 1) {
        fprintf(stderr, "File name \"%.*s\" is to long.\n", name.length, name.data);
    }
    strncpy(name_c, name.data, name.length);
    name_c[name.length] = '\0';
    path_init_c(path, name_c);
}


void path_init_working_directory(path_t* path) {
    path_init_c(path, ".");
}

void path_load_stats(path_t* path) {
    if(!path->has_stats) {
        fstat(path->fd_, &path->st_buf_);        
        path->has_stats = 1;
    }
}

void path_copy(path_t* orig, path_t* copy) {
    *copy = (path_t){ 0 };
    copy->fd_ = dup(orig->fd_);
    if(orig->has_stats) path_load_stats(copy);
}


buffer_t path_open_file_aligned(path_t* path, arena_t* arena, int alignment) {
    path_load_stats(path);
    size_t file_size = path->st_buf_.st_size;
    buffer_t result = { 0 };
    result.begin = arena_alloc(arena, file_size, alignment);
    result.end = result.begin + file_size;
    read(path->fd_, result.begin, file_size);
    return result;
}

buffer_t path_open_file(path_t* path, arena_t* arena) {
    return path_open_file_aligned(path, arena, 1);
}

void path_close(path_t* path) {
    if(!path->invalid) {
        close(path->fd_);
        path->invalid = 1;
    }
}

void path_navigate_c(path_t* path, char* name) {
    int new_fd = openat(path->fd_, name, O_RDONLY);
    path_close(path);
    path->fd_ = new_fd;
    path->has_stats = 0;
    path->invalid = 0;
}

void path_navigate(path_t* path, string_t name) {
    char name_c[PATH_MAX];
    if(name.length >= PATH_MAX - 1) {
        fprintf(stderr, "File name \"%.*s\" is to long.\n", name.length, name.data);
    }
    strncpy(name_c, name.data, name.length);
    name_c[name.length] = '\0';
    path_navigate_c(path, name_c);
}

int path_is_directory(path_t* path) {
    path_load_stats(path);
    return path->st_buf_.st_mode == S_IFDIR;
}

ulong path_get_file_size(path_t* path) {
    path_load_stats(path);
    return path->st_buf_.st_size;
}

string_t path_get_name(path_t* path, arena_t* arena) {
    char proc_path_name[256];
    sprintf(proc_path_name, "/proc/self/fd/%i", path->fd_);
    struct stat sb;
    if(lstat(proc_path_name, &sb) == -1) {
        perror("lstat");
        exit(1);
    }
    int supposed_file_name_length = sb.st_size + 1;
    if(sb.st_size == 0) supposed_file_name_length = PATH_MAX;
    string_t result;
    result.data   = arena_alloc_array(arena, supposed_file_name_length, char);
    result.length = readlink(proc_path_name, result.data,
                             supposed_file_name_length);
    if(result.length == -1) {
        perror("readlink");
        exit(1);
    }
    return result;
}


string_t path_get_base_name(path_t* path, arena_t* arena) {
    string_t result;
    result.data = arena_alloc_array(arena, 256, char);
    temp_t temp = temp_begin(arena);
    string_t path_name = path_get_name(path, arena);
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

int dir_iter_next(dir_iter_t* dir_iter) {
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

dir_iter_t dir_iter_begin(path_t* path) {
    dir_iter_t result = { 0 };
    result.dp_ = fdopendir(path->fd_);

    if(!result.dp_) {
        fprintf(stderr, strerror(errno));
        exit(1);
    }
    result.dir_path = path;
    return result;
}

void dir_iter_end(dir_iter_t* dir_iter) {
    path_close(&dir_iter->path);
    closedir(dir_iter->dp_);
}
#endif

implement_hashmap(string_map, string_t, SuperFastHash, string_equal);

u32 SuperFastHash(string_t str) {
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

u32 vfs_entry_hash(vfs_entry_t v) {
    return SuperFastHash(v.name);
}
u32 vfs_entry_equal(vfs_entry_t v1, vfs_entry_t v2) {
    return string_equal(v1.name, v2.name);
}

implement_hashmap(vfs, vfs_entry_t, vfs_entry_hash, vfs_entry_equal);

void pool_init(pool_t* pool, page_list_t* free_list) {
    pool->free_list = free_list;
    pool->data      = page_alloc(1);
    assert(pool->data);
    pool->length    = PAGE_SIZE;
    pool->fill      = 0;
}

void* pool_alloc(pool_t* pool, ulong bytes, ulong align) {
    ulong old_fill = pool->fill;
    void* result = NULL;
    if(pool->fill > pool->length) {
        long old_num_pages = pool->length / PAGE_SIZE;
        while(pool->fill > pool->length) {
            pool->length <<= 2;
        }
        long new_num_pages = pool->length / PAGE_SIZE;
        void* new_data = page_list_alloc(pool->free_list, new_num_pages);
        assert(new_data);
        memcpy(new_data, pool->data, old_fill);
        page_list_add(pool->free_list, pool->data, old_num_pages);

        result = ((void*)(((intptr_t)(new_data + pool->fill + (align-1))&~(align-1))));
        pool->fill = result + bytes - new_data;
        pool->data = new_data;
    } else {
        result = ((void*)(((intptr_t)(pool->data + pool->fill + (align-1))&~(align-1))));
        pool->fill = result + bytes - pool->data;
    }
    
    return result;
}

void pool_empty(pool_t* pool) {
    pool->fill = 0;
}

void pool_deinit(pool_t* pool) {
    page_list_add(pool->free_list, pool->data, pool->length / PAGE_SIZE);
}

#ifdef NDEBUG
void error_at_pos2(text_pos_t text_pos) {
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

void error_at_pos2(text_pos_t text_pos, int line_num) {
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
    buffer_t   buffer;
    char*      pos;
    text_pos_t text_pos;
    arena_t*   arena;
    int        had_new_line;
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


void bml_parse_name(bml_node_t* node, bml_parser_state_t* parser) {
    char* begin = parser->pos;
    while(bml_is_valid_char(parser->pos[0])) bml_parser_advance(parser);
    if(parser->pos == begin) {
        error_at_pos(parser->text_pos);
        fprintf(stderr, "Invalid node name.\n");
        exit(1);
    }
    node->name = (string_t) {
        .data = begin,
        .length = parser->pos - begin
    };
}

void bml_parse_data(bml_node_t* node, bml_parser_state_t* parser) {
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
    node->value = (string_t) {
        .data = begin,
        .length = end - begin
    };
}

void bml_parse_attributes(bml_node_t* node, bml_parser_state_t* parser) {
    bml_node_t** next_node_ptr = &node->child;
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

        bml_node_t* new_node = (bml_node_t*)arena_alloc_type(parser->arena, bml_node_t);
        *new_node = (bml_node_t){ 0 };
        char* begin = parser->pos;
        while(bml_is_valid_char(parser->pos[0])) bml_parser_advance(parser);
        if(parser->pos == begin) {
            error_at_pos(parser->text_pos);
            fprintf(stderr, "Invalid attribute name.\n");
            exit(1);
        }
        new_node->name = (string_t) {
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

bml_node_t* bml_parse_node(bml_parser_state_t* parser, int* indent) {
    if(parser->pos[0] == '\0') {
        return NULL;
    }
    if(parser->pos[0] == '\n') {
        if(*indent == 0) {
            bml_parser_advance(parser);
            *indent = bml_parse_indent(parser);
        }
    }
    bml_node_t* node = arena_alloc_type(parser->arena, bml_node_t);
    *node = (bml_node_t) { 0 };
    bml_parse_name(node, parser);
    bml_parse_data(node, parser);

    bml_node_t** next_node_ptr = &node->child;

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

        bml_node_t* new_node = arena_alloc_type(parser->arena, bml_node_t);
        *new_node = (bml_node_t){ 0 };
        char* begin = parser->pos;
        while(bml_is_valid_char(parser->pos[0])) bml_parser_advance(parser);
        if(parser->pos == begin) {
            error_at_pos(parser->text_pos);
            fprintf(stderr, "Invalid attribute name.\n");
            exit(1);
        }
        new_node->name = (string_t) {
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
            bml_node_t* new_node = bml_parse_node(parser, &new_indent);
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

bml_node_t* bml_parse(buffer_t buffer, arena_t* arena) {
    bml_parser_state_t parser = { 0 };
    parser.buffer = buffer;
    parser.pos    = (char*)buffer.begin;
    parser.text_pos.line_number = 1;
    parser.arena = arena;
    int indent = 0;
    bml_node_t* root;
    bml_node_t** next_node_ptr = &root;
    while(parser.pos[0] != '\0') {
        *next_node_ptr =  bml_parse_node(&parser, &indent);
        next_node_ptr = &((*next_node_ptr)->next);
    }
    return root;
}

void bml_print_node(bml_node_t* node, int indent) {
    if(node) {
        printf("%*s%.*s: %.*s\n", indent, "", node->name.length, node->name.data, node->value.length, node->value.data);
        bml_print_node(node->child, indent + 2);
        bml_print_node(node->next, indent);
    }
}



void vfs_add_dir(vfs_t* vfs, path_t* path, arena_t* arena) {
    dir_iter_t dir_iter = dir_iter_begin(path);
    while(dir_iter_next(&dir_iter)) {
        vfs_entry_t entry = { 0 };
        entry.name   = path_get_base_name(&dir_iter.path, arena);
        entry.buffer = path_open_file(&dir_iter.path, arena);
        vfs_insert(vfs, entry);
    }
    dir_iter_end(&dir_iter);
}

#endif //BASE_IMPLEMENTATION_GUARD
#endif //BASE_IMPLEMENTATION
