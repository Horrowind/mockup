#ifndef BASE_GUARD
#define BASE_GUARD

#if defined(LINUX)
#define _GNU_SOURCE
#include <dirent.h>    // readdir, fdopendir, closedir
#include <stdlib.h>    // calloc, free
#include <string.h>    // memset, memcpy
#include <stdio.h>     // fprintf, sprintf
#elif defined(WINDOWS)
#define UNICODE
#define _UNICODE
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#elif defined(WASM)
#endif

#if defined(ARCH64)
#define TYPE_ADDR long
#define TYPE_I64  long
typedef int       wchar;
#elif defined(ARCH32)
#define TYPE_ADDR int
#define TYPE_I64  long long
typedef long      wchar;
#endif

typedef signed   char     i8;
typedef unsigned char     u8;
typedef          short    i16;
typedef unsigned short    u16;
typedef signed   int      i32;
typedef unsigned int      u32;
typedef unsigned int      b32;
typedef unsigned TYPE_I64 u64;
typedef unsigned TYPE_I64 b64;
typedef          float    f32;
typedef          double   f64;

typedef unsigned long     ulong;
typedef unsigned int      uint;
typedef unsigned short    ushort;
typedef unsigned char     uchar;

typedef          TYPE_ADDR iptr;
typedef unsigned TYPE_ADDR uptr;

#define STR_(x) #x
#define STR(x) STR_(x)
#define CODE_LOCATION STR(__FILE__) ": " STR(__LINE__) 

#if defined(WINDOWS)
#define fallthrough
#else
#define fallthrough __attribute__ ((fallthrough));
#endif

#define invalid_code_path assert(!"Invalid code path")
#define not_implemented assert(!"Not implemented")
#define invalid_default_case default: invalid_code_path; break

#define alignof(x) __alignof(x)

typedef __builtin_va_list VariableArgumentList;
#define variable_argument_begin(v,l) __builtin_va_start(v,l)
#define variable_argument_end(v)     __builtin_va_end(v)
#define variable_argument_get(v,l)   __builtin_va_arg(v,l)
#define variable_argument_copy(d,s)  __builtin_va_copy(d,s)


#define KB(x) (x * 1024ull)
#define MB(x) (x * 1024ull * 1024ull)
#define GB(x) (x * 1024ull * 1024ull * 1024ull)

#define array_length(array) (sizeof(array)/sizeof(array[0]))
#undef max
#undef min
#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))

#if defined(LINUX)
#define debug_break raise_signal(2 /*SIGINT*/)
#elif defined(WINDOWS)
#define debug_break DebugBreak()
#endif

////////////////////////////////
// Platform dependent structs //
////////////////////////////////

#if defined(LINUX)

typedef struct {
    long seconds;
    long nano_seconds;
} TimeSpec;

typedef struct {
    ulong st_dev;
    u64   st_ino;
    long  st_nlink;

    uint  st_mode;
    uint  st_uid;
    uint  st_gid;
    uint  __pad0;
    u64   st_rdev;
    long  st_size;
    long  st_blksize;
    u64   st_blocks;

    TimeSpec st_atim;
    TimeSpec st_mtim;
    TimeSpec st_ctim;
    long __unused[3];
} Stat;

typedef struct {
    int fd_;
    int fd_read_;
    int fd_write_;
    Stat stat_;
} Path;

typedef struct {
    Path path;
    Path* dir_path;
    DIR* dp_;
} DirIter;

#elif defined(WINDOWS)

typedef struct  {
    ulong low_date_time;
    ulong high_date_time;
} Win32FileTime;

typedef struct {
    ulong dwFileAttributes;
    Win32FileTime ftCreationTime;
    Win32FileTime ftLastAccessTime;
    Win32FileTime ftLastWriteTime;
    ulong nFileSizeHigh;
    ulong nFileSizeLow;
} Win32FileAttributeData;

typedef struct {
    ulong dwFileAttributes;
    Win32FileTime ftCreationTime;
    Win32FileTime ftLastAccessTime;
    Win32FileTime ftLastWriteTime;
    ulong nFileSizeHigh;
    ulong nFileSizeLow;
    ulong dwReserved0;
    ulong dwReserved1;
    wchar cFileName[260];
    wchar cAlternateFileName[14];
} Win32FindDataW;

typedef struct {
    union {
        ulong dwOemId;
        struct {
            ushort wProcessorArchitecture;
            ushort wReserved;
        };
    };
    ulong  dwPageSize;
    void*  lpMinimumApplicationAddress;
    void*  lpMaximumApplicationAddress;
    ulong* dwActiveProcessorMask;
    ulong  dwNumberOfProcessors;
    ulong  dwProcessorType;
    ulong  dwAllocationGranularity;
    ushort wProcessorLevel;
    ushort wProcessorRevision;
} Win32SystemInfo;

typedef struct {
    void*  handle_;
    wchar* path_name_;
    Win32FileAttributeData data_;
} Path;

typedef struct {
    Path path;
    Path* dir_path;
    void* handle_;
    Win32FindDataW find_file_data_;
    int first_iteration : 1;
    int last_iteration  : 1;
} DirIter;

#define WIN32API __declspec(dllimport) __stdcall
WIN32API
void DebugBreak();

#elif defined(WASM)

typedef struct FILE FILE;

void* malloc(uptr size);
void  free(void *ptr);
void* calloc(uptr nmemb, uptr size);
void* memcpy(void *dest, const void *src, uptr n);
int   memcmp(const void *s1, const void *s2, uptr n);
void* memset(void *s, int c, uptr n);

void exit(int status);

FILE* stderr;
int   printf(const char *format, ...);
int   fprintf(FILE* fp, const char *format, ...);
int   sprintf(char *str, const char *format, ...);
#endif

////////////////////
// Base interface //
////////////////////

typedef struct  {
    uint page_size;
    uint is_initialized : 1;
} Global;
extern Global global;

void global_init();

void exit(int status);
int  raise_signal(int signal);

void* page_alloc(ulong num_pages);
void* page_realloc(void* pages, ulong old_num_pages, ulong new_num_pages);
void  page_dealloc(void* pages, ulong num_pages);

ulong size_to_pages(ulong size);

typedef struct FreeList_ {
    struct FreeList_* next;
    struct FreeList_* prev;
    void* ptr;
    ulong num_pages;
} FreeList;

void  free_list_init(FreeList* free_list_sentinel);
void* free_list_alloc(FreeList* free_list_sentinel, ulong size);
void  free_list_add(FreeList* free_list_sentinel, void* ptr, ulong num_pages);

typedef struct {
    void* begin;
    void* end;
} Buffer;

Buffer buffer(void* data, ulong length);
ulong  buffer_length(Buffer buffer);
void   zero_buffer(Buffer buffer);

typedef struct {
    Buffer   buffer;
    void*    current;
} Arena;

Arena   arena_create(ulong size);
Arena   arena_create_buffer(Buffer buffer);
void    arena_clear(Arena* a);
Arena   arena_subarena(Arena* a, ulong size);
void*   arena_alloc(Arena* a, ulong size, ulong align);
#define arena_alloc_type(a,type) ((type*)arena_alloc((a), sizeof(type), alignof(type)))
#define arena_alloc_array(a,n,type) (type*)arena_alloc((a), (n)*sizeof(type), alignof(type))
void*   arena_dealloc(Arena* a);

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

String string_from_buffer(Buffer buffer);
Buffer buffer_from_string(String string);
b32 string_equal(String s1, String s2);
String string_from_c_string(char* buf);
String string_trim_spaces(String str);
u64 string_to_u64(String s);
u64 string_to_u64_base(String s, int base);
void upcase_string(String s);
void lowcase_string(String s);

typedef struct {
    Buffer buffer;
    int length;
} StringBuilder;

StringBuilder string_builder(Buffer buffer);
void string_builder_append(StringBuilder* builder, String string);
String string_from_builder(StringBuilder builder);

#define string_split_foreach(s, t, c)                                   \
    String t = { .data = (s).data };                                    \
    for(char* macro_c_ = (s).data; macro_c_ <= (s).data + (s).length;   \
        t.data = (*macro_c_ == c) ? macro_c_ + 1 : t.data,              \
            macro_c_++, t.length = macro_c_ - t.data)                   \
        if(*macro_c_ == c || macro_c_ == (s).data + (s).length)         \

#ifndef NDEBUG
#define print_error_fixed_cstring(s)  print_error(L(s))
#define assert(x) if(!(x)) {                                            \
        print_error_fixed_cstring("Assertion (" #x ") at " CODE_LOCATION ".\n"); \
        __builtin_trap();}
#else
#define assert(x)
#endif

void print(String s);
void print_error(String s);

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
    
#define define_stack(name, prefix, type)                                \
    typedef struct {                                                    \
        type* data;                                                     \
        u16 fill;                                                       \
        u16 capacity;                                                   \
    } name;                                                             \
    extern void prefix##_init(name* s, FreeList* sentinel, int size);   \
    extern void prefix##_deinit(name* s, FreeList* sentinel);           \
    extern type* prefix##_push(name* s, type data);                     \
    extern void prefix##_reserve(name* s);                              \
    extern type* prefix##_pop(name* s);                                 \
    extern type* prefix##_top(name* s);                                 \
    extern b32 prefix##_is_empty(name* s);

#define implement_stack(name, prefix, type)                             \
    inline void prefix##_init(name* s, FreeList* sentinel, int size) {  \
        if(!global.is_initialized) global_init();                       \
        ulong num_pages = (size * sizeof(type) + global.page_size - 1) / \
            global.page_size;                                           \
        s->data = free_list_alloc(sentinel, num_pages);                 \
        s->fill = 0;                                                    \
        s->capacity = size;                                             \
    }                                                                   \
                                                                        \
    inline void prefix##_deinit(name* s, FreeList* sentinel) {          \
        ulong num_pages = (s->capacity * sizeof(type) +                 \
                           global.page_size - 1) /                      \
                           global.page_size;                            \
        free_list_add(sentinel, s->data, num_pages);                    \
    }                                                                   \
                                                                        \
    inline type* prefix##_push(name* s, type data) {                    \
        if(s->fill == s->capacity) {                                    \
            ulong old_num_pages = (s->capacity * sizeof(type) +         \
                                   global.page_size - 1) /              \
                                   global.page_size;                    \
            s->capacity *= 2;                                           \
            ulong new_num_pages = (s->capacity * sizeof(type) +         \
                                   global.page_size - 1) /              \
                                   global.page_size;                    \
            /*TODO: One should check for null here...        */         \
            s->data = page_realloc(s->data, old_num_pages, new_num_pages); \
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


#define generate_stack(name, prefix, type)                              \
    define_stack(name, prefix, type);                                   \
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
            u32  hash : 31;                                             \
            u32  used :  1;                                             \
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
        map->entries = page_alloc(size_to_pages(size * sizeof(map->entries[0])));  \
    }                                                                   \
                                                                        \
    void prefix##_deinit(name* map) {                                   \
        page_dealloc(map->entries, size_to_pages(map->size * sizeof(map->entries[0]))); \
    }                                                                   \
                                                                        \
    void prefix##_insert2(name* map, type data, u32 hash);              \
    void prefix##_resize(name* map) {                                   \
        name new_map;                                                   \
        prefix##_init(&new_map, 2 * map->size);                         \
        for(int i = 0; i < map->size; i++) {                            \
            type data = map->entries[i].data;                           \
            u32 hash = map->entries[i].hash;                            \
            u32 used = map->entries[i].used;                            \
            if(used) {                                                  \
                prefix##_insert2(&new_map, data, hash);                 \
            }                                                           \
        }                                                               \
        page_dealloc(map->entries, size_to_pages(map->size * sizeof(map->entries[0]))); \
        *map = new_map;                                                 \
    }                                                                   \
                                                                        \
    inline                                                              \
    void prefix##_insert2(name* map, type data, u32 hash) {             \
        u32 mask = (map->size - 1);                                     \
        u32 pos = hash & mask;                                          \
        u32 dist = 0;                                                   \
        for(;;) {                                                       \
            if(!map->entries[pos].used) {                               \
                map->entries[pos].data = data;                          \
                map->entries[pos].hash = hash;                          \
                map->entries[pos].used = 1;                             \
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
                    map->entries[pos].used = 1;                         \
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
            if(!map->entries[pos].used) {                               \
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

#define generate_hashmap(Name, name, type, fun, cmp)                    \
    define_hashmap(Name, name, type, fun, cmp)                          \
    implement_hashmap(Name, name, type, fun, cmp)

define_hashmap(StringMap, string_map, String, SuperFastHash, string_equal);

#define InternHashmap(type)                                             \
    struct {                                                            \
        Arena arena;                                                    \
        struct {                                                        \
            type* data;                                                 \
            u32  hash : 31;                                             \
            u32  used :  1;                                             \
        }* entries;                                                     \
        uint size;                                                      \
        uint fill;                                                      \
    }

#define define_intern_hashmap(name, prefix, type)                       \
    typedef InternHashmap(type) name;                                   \
    void  prefix##_init(name* map, int size);                           \
    void  prefix##_deinit(name* map);                                   \
    void  prefix##_resize(name* map);                                   \
    type* prefix##_find(name* map, type data);                          \
    type* prefix##_find_hash(name* map, type data, u32 hash);           \
    void  prefix##_insert(name* map, type data);                        \
    type* prefix##_find_or_insert(name* map, type data);                \
    void  prefix##_insert2(name* map, type* data, u32 hash);


#define implement_intern_hashmap(name, prefix, type, fun, cmp, cpy)     \
    void prefix##_init(name* map, int size) {                           \
        map->arena = arena_create(1024 * 1024 * 128);                   \
        map->size = size;                                               \
        map->fill = 0;                                                  \
        map->entries = page_alloc(size_to_pages(size * sizeof(map->entries[0]))); \
    }                                                                   \
                                                                        \
    void prefix##_deinit(name* map) {                                   \
        page_dealloc(map->entries, size_to_pages(map->size * sizeof(map->entries[0]))); \
    }                                                                   \
                                                                        \
    void prefix##_insert2(name* map, type* data, u32 hash);             \
    void prefix##_resize(name* map) {                                   \
        name new_map;                                                   \
        new_map.size = 2 * map->size;                                   \
        new_map.fill = 0;                                               \
        new_map.entries = page_alloc(size_to_pages(new_map.size * sizeof(map->entries[0]))); \
        new_map.arena = map->arena;                                     \
        for(int i = 0; i < map->size; i++) {                            \
            type* data = map->entries[i].data;                          \
            u32 hash = map->entries[i].hash;                            \
            u32 used = map->entries[i].used;                            \
            if(used) {                                                  \
                prefix##_insert2(&new_map, data, hash);                 \
            }                                                           \
        }                                                               \
        page_dealloc(map->entries, size_to_pages(map->size * sizeof(map->entries[0]))); \
        *map = new_map;                                                 \
    }                                                                   \
                                                                        \
    inline                                                              \
    void prefix##_insert2(name* map, type* data, u32 hash) {            \
        u32 mask = (map->size - 1);                                     \
        u32 pos = hash & mask;                                          \
        u32 dist = 0;                                                   \
        for(;;) {                                                       \
            if(!map->entries[pos].used) {                               \
                map->entries[pos].data = data;                          \
                map->entries[pos].hash = hash;                          \
                map->entries[pos].used = 1;                             \
                break;                                                  \
            } else {                                                    \
                u32 probed_hash = map->entries[pos].hash;               \
                u32 probed_dist = (probed_hash - pos) & mask;           \
                if(probed_dist < dist) {                                \
                    type* tmp_data = data;                              \
                    data = map->entries[pos].data;                      \
                    map->entries[pos].data = tmp_data;                  \
                                                                        \
                    u32 tmp_hash = hash;                                \
                    hash = probed_hash;                                 \
                    map->entries[pos].hash = tmp_hash;                  \
                    map->entries[pos].used = 1;                         \
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
            if(!map->entries[pos].used) {                               \
                return NULL;                                            \
            } else  if(cmp(*map->entries[pos].data, data)) {            \
                return map->entries[pos].data;                          \
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
    }                                                                   \
                                                                        \
    type* prefix##_find_or_insert(name* map, type data) {               \
        u32 hash = fun(data);                                           \
        type* found = prefix##_find_hash(map, data, hash);              \
        if(found) return found;                                         \
        type* new_data = cpy(&map->arena, data);                        \
        prefix##_insert2(map, new_data, hash);                          \
        return new_data;                                                \
    }                                                                   \
                                                                        \
    void prefix##_insert(name* map, type data) {                        \
        u32 hash = fun(data);                                           \
        type* new_data = cpy(&map->arena, data);                        \
        prefix##_insert2(map, new_data, hash);                          \
    }

#define generate_intern_hashmap(Name, name, type, fun, cmp)             \
    define_intern_hashmap(Name, name, type, fun, cmp)                   \
    implement_intern_hashmap(Name, name, type, fun, cmp)



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

Global global = { 0 };

///////////////////////////////////////
// Platform dependent implementation //
///////////////////////////////////////

#if defined(LINUX)

#define INVALID_FD (-1)

static __inline long linux_syscall0(long n) {
	unsigned long ret;
	__asm__ __volatile__ ("syscall" : "=a"(ret) : "a"(n) : "rcx", "r11", "memory");
	return ret;
}

static __inline long linux_syscall1(long n, long a1) {
	unsigned long ret;
	__asm__ __volatile__ ("syscall" : "=a"(ret) : "a"(n), "D"(a1) : "rcx", "r11", "memory");
	return ret;
}

static __inline long linux_syscall2(long n, long a1, long a2) {
	unsigned long ret;
	__asm__ __volatile__ ("syscall" : "=a"(ret) : "a"(n), "D"(a1), "S"(a2)
						  : "rcx", "r11", "memory");
	return ret;
}

static __inline long linux_syscall3(long n, long a1, long a2, long a3) {
	unsigned long ret;
	__asm__ __volatile__ ("syscall" : "=a"(ret) : "a"(n), "D"(a1), "S"(a2),
						  "d"(a3) : "rcx", "r11", "memory");
	return ret;
}

static __inline long linux_syscall4(long n, long a1, long a2, long a3, long a4) {
	unsigned long ret;
	register long r10 __asm__("r10") = a4;
	__asm__ __volatile__ ("syscall" : "=a"(ret) : "a"(n), "D"(a1), "S"(a2),
						  "d"(a3), "r"(r10): "rcx", "r11", "memory");
	return ret;
}

static __inline long linux_syscall5(long n, long a1, long a2, long a3, long a4, long a5) {
	unsigned long ret;
	register long r10 __asm__("r10") = a4;
	register long r8 __asm__("r8") = a5;
	__asm__ __volatile__ ("syscall" : "=a"(ret) : "a"(n), "D"(a1), "S"(a2),
						  "d"(a3), "r"(r10), "r"(r8) : "rcx", "r11", "memory");
	return ret;
}

static __inline long linux_syscall6(long n, long a1, long a2, long a3, long a4, long a5, long a6) {
	unsigned long ret;
	register long r10 __asm__("r10") = a4;
	register long r8 __asm__("r8") = a5;
	register long r9 __asm__("r9") = a6;
	__asm__ __volatile__ ("syscall" : "=a"(ret) : "a"(n), "D"(a1), "S"(a2),
						  "d"(a3), "r"(r10), "r"(r8), "r"(r9) : "rcx", "r11", "memory");
	return ret;
}


#define LINUX_SYSCALL_NARGS_X_(a,b,c,d,e,f,g,h,n,...) n
#define LINUX_SYSCALL_NARGS_(...) LINUX_SYSCALL_NARGS_X_(__VA_ARGS__,7,6,5,4,3,2,1,0,)
#define LINUX_SYSCALL_CONCAT_X_(a,b) a##b
#define LINUX_SYSCALL_CONCAT_(a,b) LINUX_SYSCALL_CONCAT_X_(a,b)
#define LINUX_SYSCALL_DISP_(b,...) LINUX_SYSCALL_CONCAT_(b, LINUX_SYSCALL_NARGS_(__VA_ARGS__))(__VA_ARGS__)

#define linux_syscall(...) LINUX_SYSCALL_DISP_(linux_syscall,__VA_ARGS__)

enum {
    LINUX_SYSCALL_READ           =   0,
    LINUX_SYSCALL_WRITE          =   1,
    LINUX_SYSCALL_OPEN           =   2,
    LINUX_SYSCALL_CLOSE          =   3,
    LINUX_SYSCALL_FSTAT          =   5,
    LINUX_SYSCALL_LSTAT          =   6,
    LINUX_SYSCALL_MMAP           =   9,
    LINUX_SYSCALL_MUNMAP         =  11,
    LINUX_SYSCALL_RT_SIGPROCMASK =  14,
    LINUX_SYSCALL_MREMAP         =  25,
    LINUX_SYSCALL_DUP            =  32,
    LINUX_SYSCALL_READLINK       =  89,
    LINUX_SYSCALL_GETTID         = 186,
    LINUX_SYSCALL_TKILL          = 200,
    LINUX_SYSCALL_EXIT_GROUP     = 231,
    LINUX_SYSCALL_OPENAT         = 257,
} LinuxSyscall;

enum {
    OPEN_FLAGS_READ           =       0,
    OPEN_FLAGS_WRITE          =       1,
    OPEN_FLAGS_READ_WRITE     =       2,

    OPEN_FLAGS_LARGE_FILE     =       0,
    OPEN_FLAGS_CREATE         =    0x40,
    OPEN_FLAGS_CLOSE_ON_EXEC  = 0x80000,

    OPEN_FLAGS_COMMON         = OPEN_FLAGS_LARGE_FILE | OPEN_FLAGS_CLOSE_ON_EXEC,
} LinuxOpenFlags;

enum {
    MISC_FLAGS_CURRENT_DIR_FD =    -100,
} LinuxMiscFlags;

enum {
    FILE_MODE_DIRECTORY       =  0x4000,
} LinuxFileMode;

enum {
    MEMORY_FLAGS_READ         =  1,
    MEMORY_FLAGS_WRITE        =  2,
    MEMORY_FLAGS_PRIVATE      =  2,
    MEMORY_FLAGS_ANONYMOUS    = 32,
    MEMORY_FLAGS_MAYMOVE      =  1,
} LinuxMemoryFlags;

long linux_syscall_read(int file_descriptor, void* buf, unsigned long count) {
    return linux_syscall(LINUX_SYSCALL_READ, (long)file_descriptor, (long)buf, (long)count);
}

long linux_syscall_write(int file_descriptor, const void* buf, unsigned long count) {
    return linux_syscall(LINUX_SYSCALL_WRITE, (long)file_descriptor, (long)buf, (long)count);
}

long linux_syscall_open(const char* file_path, int flags, int mode) {
    return linux_syscall(LINUX_SYSCALL_OPEN, (long)file_path, (long)flags, (long)mode);
}

int linux_syscall_close(int file_descriptor) {
    return (int)linux_syscall(LINUX_SYSCALL_CLOSE, (long)file_descriptor);
}

long linux_syscall_fstat(int file_descriptor, Stat* stat) {
    return linux_syscall(LINUX_SYSCALL_FSTAT, (long)file_descriptor, (long)stat);
}

long linux_syscall_lstat(const char* path_name, Stat* stat) {
    return linux_syscall(LINUX_SYSCALL_LSTAT, (long)path_name, (long)stat);
}

void* linux_syscall_mmap(void* addr, unsigned long length, int prot,
                   int flags, int file_descriptor, off_t offset) {
    return (void*)linux_syscall(LINUX_SYSCALL_MMAP, (long)addr, (long)length, (long)prot, (long)flags,
                   (long)file_descriptor, (long)offset);
}

int linux_syscall_munmap(void* start, size_t len) {
    return (int)linux_syscall(LINUX_SYSCALL_MUNMAP, (long)start, (long)len);
}

int linux_syscall_rt_sigprocmask(int how, const ulong* signal_set,
                           ulong* old_signal_set, ulong signal_set_size) {
    return (int)linux_syscall(LINUX_SYSCALL_RT_SIGPROCMASK, (long)how, (long)signal_set,
                        (long)old_signal_set, (long)signal_set_size);
}

void* linux_syscall_mremap(void* old_addr, unsigned long old_size,
                     unsigned long new_size, int flags, void* new_addr) {
    return (void*)linux_syscall(LINUX_SYSCALL_MREMAP, (long)old_addr, (long)old_size, (long)new_size, (long)flags,
                   (long)new_addr);
}

int linux_syscall_dup(int file_descriptor) {
    return (int)linux_syscall(LINUX_SYSCALL_DUP, (long)file_descriptor);
}

int linux_syscall_gettid() {
    return (int)linux_syscall(LINUX_SYSCALL_GETTID);
}

int linux_syscall_tkill(int thread_id, int signal) {
    return (int)linux_syscall(LINUX_SYSCALL_TKILL, (long)thread_id, (long)signal);
}

long linux_syscall_readlink(const char* restrict path_name, char* restrict buffer, unsigned long buffer_size) {
    return linux_syscall(LINUX_SYSCALL_READLINK, (long)path_name, (long)buffer, (long)buffer_size);
}

void linux_syscall_exit_group(int status) {
    linux_syscall(LINUX_SYSCALL_EXIT_GROUP, (long)status);
}

long linux_syscall_openat(const char* file_path, int file_descriptor, int flags, int mode) {
    return linux_syscall(LINUX_SYSCALL_OPENAT, (long)file_descriptor, (long)file_path, (long)flags, (long)mode);
}


void print(String s) {
    linux_syscall_write(1, s.data, s.length);
}

void print_error(String s) {
    linux_syscall_write(2, s.data, s.length);
}

void print_format(const char* s, ulong length) {
    linux_syscall_write(1, s, length);
}

void error_print_format(const char* s, ulong length) {
    linux_syscall_write(2, s, length);
}


_Noreturn
void exit(int status) {
    linux_syscall_exit_group(status);
    for(;;) {}
}


int raise_signal(int signal) {
    int thread_id;
    int result;
    ulong signal_set[8];
    static const unsigned long app_mask[] = {
        0x7fffffff, 0xfffffffc
    };
    linux_syscall_rt_sigprocmask(0 /*SIG_BLOCK*/, app_mask, signal_set, 8);
    thread_id = linux_syscall_gettid();
    result    = linux_syscall_tkill(thread_id, signal);
    linux_syscall_rt_sigprocmask(2 /*SIG_SETMASK*/, signal_set, 0, 8);
    return result;
}


void global_init() {
    global.page_size      = 4096;
    global.is_initialized = 1;
}

void* page_alloc(ulong num_pages) {
    if(!global.is_initialized) global_init();
    return linux_syscall_mmap(NULL, global.page_size * num_pages, MEMORY_FLAGS_READ | MEMORY_FLAGS_WRITE,
                              MEMORY_FLAGS_PRIVATE | MEMORY_FLAGS_ANONYMOUS, 0, 0);
}

void* page_realloc(void* pages, ulong old_num_pages, ulong new_num_pages) {
    if(!global.is_initialized) global_init();
    return linux_syscall_mremap(pages, global.page_size * old_num_pages,
                          global.page_size * new_num_pages, MEMORY_FLAGS_MAYMOVE, 0);
}


void page_dealloc(void* page, ulong num_pages) {
    if(!global.is_initialized) global_init();
    linux_syscall_munmap(page, num_pages * global.page_size);
}


#elif defined(WINDOWS)


typedef enum {
    WIN32_GET_FILEEX_INFO_STANDARD,
    WIN32_GET_FILEEX_MAX_INFO_LEVEL
} Win32GetFileExInfoLevels;

typedef enum {
    WIN32_LMEM_FIXED          = 0x0000,
    WIN32_LMEM_MOVEABLE       = 0x0002,
    WIN32_LMEM_NOCOMPACT      = 0x0010,
    WIN32_LMEM_NODISCARD      = 0x0020,
    WIN32_LMEM_ZEROINIT       = 0x0040,
    WIN32_LMEM_MODIFY         = 0x0080,
    WIN32_LMEM_DISCARDABLE    = 0x0f00,
    WIN32_LMEM_VALID_FLAGS    = 0x0f72,
    WIN32_LMEM_INVALID_HANDLE = 0x8000
} Win32LocalAllocFlags;

WIN32API
b32 GetFileAttributesExW(const wchar* lpFileName,
                         Win32GetFileExInfoLevels fInfoLevelId,
                         void* lpFileInformation);
WIN32API
void GetSystemInfo(Win32SystemInfo* system_info);

WIN32API
void* LocalAlloc(uint uFlags, size_t uBytes);

WIN32API
void* LocalFree(void* hMem);

WIN32API
ulong GetFullPathNameW(const wchar* lpFileName,
                                 ulong   nBufferLength,
                                 wchar*  lpBuffer,
                                 wchar** lpFilePart);

WIN32API
void DebugBreak(void);

typedef struct {
    ulong nLength;
    void* lpSecurityDescriptor;
    b32   bInheritHandle;
} Win32SecurityAttributes;


typedef enum {
    WIN32_GENERIC_ALL     = 0x10000000L,
    WIN32_GENERIC_EXECUTE = 0x20000000L,
    WIN32_GENERIC_WRITE   = 0x40000000L,
    WIN32_GENERIC_READ    = 0x80000000L,
} Win32AccessRights;

typedef enum {
    WIN32_FILE_SHARE_READ   = 0x00000001,
    WIN32_FILE_SHARE_WRITE  = 0x00000002,
    WIN32_FILE_SHARE_DELETE = 0x00000004,
} Win32ShareMode;

typedef enum {
    WIN32_CREATE_NEW        = 1,
    WIN32_CREATE_ALWAYS     = 2,
    WIN32_OPEN_EXISTING     = 3,
    WIN32_OPEN_ALWAYS       = 4,
    WIN32_TRUNCATE_EXISTING = 5,
} Win32CreationDisposition;

WIN32API
void* CreateFileW(const wchar* lpFileName,
                  ulong /*Win32AccessRights*/ dwDesiredAccess,
                  ulong /*Win32ShareMode*/ dwShareMode,
                  Win32SecurityAttributes* lpSecurityAttributes,
                  ulong /*Win32CreationDisposition*/ dwCreationDisposition,
                  ulong dwFlagsAndAttributes,
                  void* hTemplateFile);

typedef struct {
    ulong* Internal;
    ulong* InternalHigh;
    union {
        struct {
            ulong Offset;
            ulong OffsetHigh;
        };
        void* Pointer;
    };
    void* hEvent;
} Win32Overlapped;


typedef union {
    struct {
        ulong low_part;
        long high_part;
    };
    i64 quad_part;
} Win32LargeInteger;

typedef enum {
    WIN32_FILE_ATTRIBUTE_HIDDEN        = 0x2,
    WIN32_FILE_ATTRIBUTE_NORMAL        = 0x80,
    WIN32_FILE_ATTRIBUTE_DIRECTORY     = 0x10,
    WIN32_FILE_ATTRIBUTE_TEMPORARY     = 0x100,
    WIN32_FILE_ATTRIBUTE_REPARSE_POINT = 0x400,
    WIN32_INVALID_FILE_ATTRIBUTES      = 0xffffffff
} Win32FileAttribute;

WIN32API
b32 ReadFile(void* file, void* buffer, ulong number_of_bytes_to_read,
                    ulong* number_of_bytes_read, Win32Overlapped* overlapped);
WIN32API
b32 WriteFile(void* file,
              const void* buffer,
              ulong number_of_bytes_to_read,
              ulong* number_of_bytes_read,
              Win32Overlapped* lpOverlapped);

WIN32API
ulong GetCurrentDirectoryW(ulong buffer_length,
                                    wchar* buffer);
WIN32API
b32 GetFileSizeEx(void* hFile, Win32LargeInteger* lpFileSize);

WIN32API
b32 PathIsRelativeW(const wchar* path);

WIN32API
b32 CloseHandle(void*);

WIN32API
void* FindFirstFileW(const wchar* lpFileName,
                              Win32FindDataW* lpFindFileData);
WIN32API
b32 FindNextFileW(void* hFindFile,
                           Win32FindDataW* lpFindFileData);
WIN32API
b32 FindClose(void* hFindFile);


#define WIN32_CP_UTF8 65001
#define WIN32_WC_NO_BEST_FIT_CHARS 0x00000400

WIN32API
int WideCharToMultiByte(uint CodePage,
                        ulong dwFlags,
                        const wchar* lpWideCharStr,
                        int cchWideChar,
                        char* lpMultiByteStr,
                        int cbMultiByte,
                        const char* lpDefaultChar,
                        b32* lpUsedDefaultChar);

WIN32API
int lstrlenW(const wchar* lpString);
WIN32API
wchar* lstrcpyW(wchar* lpString1, const wchar* lpString2);
WIN32API
wchar* lstrcatW(wchar* lpString1, const wchar* lpString2);


typedef enum {
    WIN32_ERROR_SUCCESS             = 0L,
    WIN32_ERROR_NO_MORE_FILES       = 18L,
    WIN32_ERROR_MORE_DATA           = 234L,
    WIN32_ERROR_INSUFFICIENT_BUFFER = 122L
} Win32Error;


WIN32API
ulong /* Win32Error */ GetLastError();

#endif




Buffer buffer(void* data, ulong length) {
    return (Buffer){
        .begin = data,
        .end   = data + length
    };
}

ulong buffer_length(Buffer buffer) {
    return buffer.end - buffer.begin;
}

void zero_buffer(Buffer buffer) {
    __builtin_memset(buffer.begin, 0, buffer_length(buffer));
}

void copy_buffer(Buffer src, Buffer dest) {
    ulong length_src = buffer_length(src);
    ulong length_dest = buffer_length(dest);
    ulong length = min(length_src, length_dest);

    __builtin_memcpy(dest.begin, src.begin, length);    
}

Buffer buffer_from_string(String string) {
    return (Buffer){
        .begin = string.data,
        .end   = string.data + string.length
    };
}

String string_from_buffer(Buffer buffer) {
    return (String){
        .data   = buffer.begin,
        .length = buffer.end - buffer.begin
    };
}


void lowcase_string(String s) {
    for(int i = 0; i < s.length; i++) {
        if((uint)s.data[i] - 'A' < 26) s.data[i] |= 0x20;
    }
}

void upcase_string(String s) {
    for(int i = 0; i < s.length; i++) {
        if((uint)s.data[i] - 'a' < 26) s.data[i] &= 0x5f;
    }
}


StringBuilder string_builder(Buffer buffer) {
    return (StringBuilder) {
        .buffer = buffer,
        .length = 0
    };
}

void string_builder_append(StringBuilder* builder, String string) {
    Buffer dest = {
        .begin = builder->buffer.begin + builder->length,
        .end   = builder->buffer.end
    };
    copy_buffer(buffer_from_string(string), dest);
    builder->length += string.length;
}

String string_from_builder(StringBuilder builder) {
    return (String) {
        .data = builder.buffer.begin,
        .length = builder.length
    };
}



ulong size_to_pages(ulong size) {
    if(!global.is_initialized) global_init();
    return (size + global.page_size - 1) / global.page_size;
}


Arena arena_create(ulong size) {
    if(!global.is_initialized) global_init();
    Buffer buffer = {
        .begin = page_alloc(size_to_pages(size)),
        .end   = (void*)((char*)buffer.begin + size)
    };
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
    void* result = ((void*)(((iptr)((char*)a->current + align)&~align)));
    void* end = (void*)((char*)result + size); 
    assert(end < a->buffer.end);
    if(end >= a->buffer.end) return NULL;
    a->current = end;
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

void free_list_init(FreeList* free_list_sentinel) {
    free_list_sentinel->next = free_list_sentinel;
    free_list_sentinel->prev = free_list_sentinel;
}

void* free_list_alloc(FreeList* free_list_sentinel, ulong num_pages) {
    if(!global.is_initialized) global_init();
    void* result = NULL;
    for(FreeList* entry = free_list_sentinel->next;
        entry != free_list_sentinel;
        entry = entry->next) {
        if(num_pages < entry->num_pages) {
            result = entry->ptr + num_pages * global.page_size;
            entry->num_pages -= num_pages;
            break;
        } else if(num_pages == entry->num_pages) {
            result = entry->ptr;
            entry->prev->next = entry->next;
            entry->next->prev = entry->prev;
            break;
        }
    }
    if(!result) result = page_alloc(num_pages);
    return result;
}

void free_list_add(FreeList* free_list_sentinel, void* ptr, ulong num_pages) {
    FreeList* entry = ptr;
    entry->ptr = ptr;
    entry->num_pages = num_pages;
    entry->prev = free_list_sentinel;
    entry->next = free_list_sentinel->next;
    entry->prev->next = entry;
    entry->next->prev = entry;
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

String string_trim_spaces(String str) {
    while(str.length > 0 && str.data[0] == ' ') {
        str.length--;
        str.data++;
    }
    return str;
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

#if 0
typedef enum {
    FORMAT_ARGUMENT_TYPE_NULL,
    FORMAT_ARGUMENT_TYPE_CHAR,
    FORMAT_ARGUMENT_TYPE_UCHAR,
    FORMAT_ARGUMENT_TYPE_SHORT,
    FORMAT_ARGUMENT_TYPE_USHORT,
    FORMAT_ARGUMENT_TYPE_INT,
    FORMAT_ARGUMENT_TYPE_UINT,
    FORMAT_ARGUMENT_TYPE_LONG,
    FORMAT_ARGUMENT_TYPE_ULONG,
    FORMAT_ARGUMENT_TYPE_U8,
    FORMAT_ARGUMENT_TYPE_U16,
    FORMAT_ARGUMENT_TYPE_U32,
    FORMAT_ARGUMENT_TYPE_U64,
    FORMAT_ARGUMENT_TYPE_I8,
    FORMAT_ARGUMENT_TYPE_I16,
    FORMAT_ARGUMENT_TYPE_I32,
    FORMAT_ARGUMENT_TYPE_I64,
    FORMAT_ARGUMENT_TYPE_POINTER,
    FORMAT_ARGUMENT_TYPE_CSTRING,
    FORMAT_ARGUMENT_TYPE_STRING,
} FormatArgumentType;

typedef union {
    i64    i;
    void*  p;
    String s;
} FormatArgument;

static
void pop_format_argument(FormatArgument* arg, FormatArgumentType type,
                         VariableArgumentList* va_list) {
    switch (type) {
    case FORMAT_ARGUMENT_TYPE_CHAR:    arg->i = (char  )variable_argument_get(*va_list, int   ); break;
    case FORMAT_ARGUMENT_TYPE_UCHAR:   arg->i = (uchar )variable_argument_get(*va_list, uint  ); break;
    case FORMAT_ARGUMENT_TYPE_SHORT:   arg->i = (short )variable_argument_get(*va_list, int   ); break;
    case FORMAT_ARGUMENT_TYPE_USHORT:  arg->i = (ushort)variable_argument_get(*va_list, uint  ); break;
    case FORMAT_ARGUMENT_TYPE_INT:     arg->i = (int   )variable_argument_get(*va_list, int   ); break;
    case FORMAT_ARGUMENT_TYPE_UINT:    arg->i = (uint  )variable_argument_get(*va_list, uint  ); break;
    case FORMAT_ARGUMENT_TYPE_LONG:    arg->i = (long  )variable_argument_get(*va_list, long  ); break;
    case FORMAT_ARGUMENT_TYPE_ULONG:   arg->i = (ulong )variable_argument_get(*va_list, ulong ); break;
    case FORMAT_ARGUMENT_TYPE_U8:      arg->i = (u8    )variable_argument_get(*va_list, uint  ); break;
    case FORMAT_ARGUMENT_TYPE_U16:     arg->i = (u16   )variable_argument_get(*va_list, uint  ); break;
    case FORMAT_ARGUMENT_TYPE_U32:     arg->i = (u32   )variable_argument_get(*va_list, u32   ); break;
    case FORMAT_ARGUMENT_TYPE_U64:     arg->i = (u64   )variable_argument_get(*va_list, u64   ); break;
    case FORMAT_ARGUMENT_TYPE_I8:      arg->i = (i8    )variable_argument_get(*va_list, int   ); break;
    case FORMAT_ARGUMENT_TYPE_I16:     arg->i = (i16   )variable_argument_get(*va_list, int   ); break;
    case FORMAT_ARGUMENT_TYPE_I32:     arg->i = (i32   )variable_argument_get(*va_list, i32   ); break;
    case FORMAT_ARGUMENT_TYPE_I64:     arg->i = (i64   )variable_argument_get(*va_list, i64   ); break;
    case FORMAT_ARGUMENT_TYPE_POINTER: arg->p = (void* )variable_argument_get(*va_list, void* ); break;
    case FORMAT_ARGUMENT_TYPE_CSTRING: arg->p = (void* )variable_argument_get(*va_list, char* ); break;
    case FORMAT_ARGUMENT_TYPE_STRING:  arg->s = (String)variable_argument_get(*va_list, String); break;
    }
}

static
void get_format_argument_at_index(FormatArgument* arg, FormatArgumentType* types,
                                  VariableArgumentList* va_list, int index) {
    FormatArgument dummy;
    for(int i = 0; i < index - 1; i++) {
        pop_format_argument(&dummy, types[i], va_list);
    }
    pop_format_argument(arg, types[index - 1], va_list);
}

static
int format_string_compare_(char* const restrict fp, String s) {
    for(int i = 0; i < s.length; i++) {
        if(s.data[i] != fp[i]) {
            return 0;
        }
    }
    return 1;
}

static
char* format_string_list(String string, char* const format, va_list arg_list) {
    char* sp = string.data;
    char* fp = format;

    int arg_pos = 0;
    
    FormatArgumentType types[16] = { };
    
    while(fp) {
        if(*fp == '{') {
            fp++;
            if(*fp == '{') {
                continue;
            }

            if('0' <= *fp && *fp <= '9') {
                arg_pos = 0;
                while('0' <= *fp && *fp <= '9') {
                    arg_pos = 10 * arg_pos + *fp - '0';
                    fp++;
                }
            } else {
                arg_pos++;
            }

            types[arg_pos] = FORMAT_ARGUMENT_TYPE_NULL;
            if(format_string_compare_(fp, L("c")))       types[arg_pos] = FORMAT_ARGUMENT_TYPE_CHAR;
            if(format_string_compare_(fp, L("u")))       types[arg_pos] = FORMAT_ARGUMENT_TYPE_UINT;
            if(format_string_compare_(fp, L("i")))       types[arg_pos] = FORMAT_ARGUMENT_TYPE_INT;
            if(format_string_compare_(fp, L("p")))       types[arg_pos] = FORMAT_ARGUMENT_TYPE_POINTER;
            if(format_string_compare_(fp, L("s")))       types[arg_pos] = FORMAT_ARGUMENT_TYPE_CSTRING;
            if(format_string_compare_(fp, L("S")))       types[arg_pos] = FORMAT_ARGUMENT_TYPE_STRING;
            if(format_string_compare_(fp, L("char")))    types[arg_pos] = FORMAT_ARGUMENT_TYPE_CHAR;
            if(format_string_compare_(fp, L("short")))   types[arg_pos] = FORMAT_ARGUMENT_TYPE_SHORT;
            if(format_string_compare_(fp, L("ushort")))  types[arg_pos] = FORMAT_ARGUMENT_TYPE_USHORT;
            if(format_string_compare_(fp, L("int")))     types[arg_pos] = FORMAT_ARGUMENT_TYPE_INT;
            if(format_string_compare_(fp, L("uint")))    types[arg_pos] = FORMAT_ARGUMENT_TYPE_UINT;
            if(format_string_compare_(fp, L("long")))    types[arg_pos] = FORMAT_ARGUMENT_TYPE_LONG;
            if(format_string_compare_(fp, L("ulong")))   types[arg_pos] = FORMAT_ARGUMENT_TYPE_ULONG;
            if(format_string_compare_(fp, L("u8")))      types[arg_pos] = FORMAT_ARGUMENT_TYPE_U8;
            if(format_string_compare_(fp, L("u16")))     types[arg_pos] = FORMAT_ARGUMENT_TYPE_U16;
            if(format_string_compare_(fp, L("u32")))     types[arg_pos] = FORMAT_ARGUMENT_TYPE_U32;
            if(format_string_compare_(fp, L("u64")))     types[arg_pos] = FORMAT_ARGUMENT_TYPE_U64;
            if(format_string_compare_(fp, L("i8")))      types[arg_pos] = FORMAT_ARGUMENT_TYPE_I8;
            if(format_string_compare_(fp, L("i16")))     types[arg_pos] = FORMAT_ARGUMENT_TYPE_I16;
            if(format_string_compare_(fp, L("i32")))     types[arg_pos] = FORMAT_ARGUMENT_TYPE_I32;
            if(format_string_compare_(fp, L("i64")))     types[arg_pos] = FORMAT_ARGUMENT_TYPE_I64;
            if(format_string_compare_(fp, L("pointer"))) types[arg_pos] = FORMAT_ARGUMENT_TYPE_POINTER;
            if(format_string_compare_(fp, L("cstring"))) types[arg_pos] = FORMAT_ARGUMENT_TYPE_CSTRING;
            if(format_string_compare_(fp, L("string")))  types[arg_pos] = FORMAT_ARGUMENT_TYPE_STRING;
            
            int field_width = 0;
            while('0' <= *fp && *fp <= '9') {
                field_width = 10 * field_width + *fp - '0';
            }
        }
        fp++;
    }
    
    fp = format;
    while(fp) {
        if(*fp == '{') {
            
        }
    }

    return NULL;
}

static
char* format_string(String string, char *format, ...) {
    VariableArgumentList va_list;
    variable_argument_begin(va_list, format);
    char* result = format_string_list(string, format, va_list);
    variable_argument_end(va_list);
    return result;
}
#endif


int c_string_equal(const char* restrict s1, const char* restrict s2) {
    do {
        if(*s1 != *s2) return 0;
    } while(*s1++);
    return 1;
}


void c_string_copy(char* restrict dest, const char* restrict src, uint n) {
    if(n == 0) return;
    int i = 0;
    for(; i < n - 1 && src[i]; i++) {
        dest[i] = src[i];
    }
    dest[i] = '\0';
}


#if defined(LINUX)

void path_init_c(Path* path, char* name) {
    path->fd_       = linux_syscall_open(name, OPEN_FLAGS_READ  | OPEN_FLAGS_COMMON, 0);
    path->fd_read_  = linux_syscall_open(name, OPEN_FLAGS_READ  | OPEN_FLAGS_COMMON, 0);
    path->fd_write_ = linux_syscall_open(name, OPEN_FLAGS_WRITE | OPEN_FLAGS_COMMON, 0);
    linux_syscall_fstat(path->fd_, &path->stat_);
}

void path_init(Path* path, String name) {
    char name_c[PATH_MAX];
    if(name.length >= PATH_MAX - 1) {
        fprintf(stderr, "File name \"%.*s\" is to long.\n", name.length, name.data);
    }
    c_string_copy(name_c, name.data, PATH_MAX);
    name_c[name.length] = '\0';
    path_init_c(path, name_c);
}

void path_create_c(Path* path, char* name) {
    path->fd_       = linux_syscall_open(name, OPEN_FLAGS_READ  | OPEN_FLAGS_COMMON | OPEN_FLAGS_CREATE, 0644);
    path->fd_read_  = linux_syscall_open(name, OPEN_FLAGS_READ  | OPEN_FLAGS_COMMON | OPEN_FLAGS_CREATE, 0644);
    path->fd_write_ = linux_syscall_open(name, OPEN_FLAGS_WRITE | OPEN_FLAGS_COMMON | OPEN_FLAGS_CREATE, 0644);
    linux_syscall_fstat(path->fd_, &path->stat_);
}

void path_create(Path* path, String name) {
    char name_c[PATH_MAX];
    if(name.length >= PATH_MAX - 1) {
        fprintf(stderr, "File name \"%.*s\" is to long.\n", name.length, name.data);
    }
    c_string_copy(name_c, name.data, PATH_MAX);
    name_c[name.length] = '\0';
    path_create_c(path, name_c);

}

void path_init_from_c(Path* path, Path* dir, char* name) {
    path->fd_       = linux_syscall_openat(name, dir->fd_, OPEN_FLAGS_READ  | OPEN_FLAGS_COMMON, 0);
    path->fd_read_  = linux_syscall_openat(name, dir->fd_, OPEN_FLAGS_READ  | OPEN_FLAGS_COMMON, 0);
    path->fd_write_ = linux_syscall_openat(name, dir->fd_, OPEN_FLAGS_WRITE | OPEN_FLAGS_COMMON, 0);
    linux_syscall_fstat(path->fd_, &path->stat_);
}

void path_init_from(Path* path, Path* dir, String name) {
    char name_c[PATH_MAX];
    if(name.length >= PATH_MAX - 1) {
        fprintf(stderr, "File name \"%.*s\" is to long.\n", name.length, name.data);
    }
    c_string_copy(name_c, name.data, PATH_MAX);
    name_c[name.length] = '\0';
    path_init_from_c(path, dir, name_c);
}

void path_create_from_c(Path* path, Path* dir, char* name) {
    path->fd_       = linux_syscall_openat(name, dir->fd_, OPEN_FLAGS_READ  | OPEN_FLAGS_COMMON | OPEN_FLAGS_CREATE, 0644);
    path->fd_read_  = linux_syscall_openat(name, dir->fd_, OPEN_FLAGS_READ  | OPEN_FLAGS_COMMON | OPEN_FLAGS_CREATE, 0644);
    path->fd_write_ = linux_syscall_openat(name, dir->fd_, OPEN_FLAGS_WRITE | OPEN_FLAGS_COMMON | OPEN_FLAGS_CREATE, 0644);
    linux_syscall_fstat(path->fd_, &path->stat_);
}

void path_create_from(Path* path, Path* dir, String name) {
    char name_c[PATH_MAX];
    if(name.length >= PATH_MAX - 1) {
        fprintf(stderr, "File name \"%.*s\" is to long.\n", name.length, name.data);
    }
    c_string_copy(name_c, name.data, PATH_MAX);
    name_c[name.length] = '\0';
    path_create_from_c(path, dir, name_c);
}

void path_init_working_directory(Path* path) {
    path_init_c(path, ".");
}

void path_copy(Path* orig, Path* copy) {
    *copy = (Path){ 0 };
    copy->fd_ = linux_syscall_dup(orig->fd_);
    linux_syscall_fstat(copy->fd_, &copy->stat_);
    copy->fd_read_ = linux_syscall_dup(orig->fd_read_);
    copy->fd_write_ = linux_syscall_dup(orig->fd_write_);
}

Buffer path_read_file_aligned(Path* path, Arena* arena, int alignment) {
    size_t file_size = path->stat_.st_size;
    Buffer result = { 0 };
    result.begin = arena_alloc(arena, file_size, alignment);
    result.end = result.begin + file_size;
    linux_syscall_read(path->fd_read_, result.begin, file_size);
    return result;
}

Buffer path_read_file(Path* path, Arena* arena) {
    return path_read_file_aligned(path, arena, 1);
}

void path_write_file(Path* path, Buffer buffer) {
    linux_syscall_write(path->fd_write_, buffer.begin, buffer.end - buffer.begin);
}

void path_close(Path* path) {
    if(path->fd_ != INVALID_FD) {
        linux_syscall_close(path->fd_);
    }
    if(path->fd_read_ != INVALID_FD) {
        linux_syscall_close(path->fd_read_);
    }
    if(path->fd_write_ != INVALID_FD) {
        linux_syscall_close(path->fd_write_);
    }
}

void path_navigate_c(Path* path, char* name) {
    int new_fd = linux_syscall_openat(name, path->fd_, OPEN_FLAGS_READ | OPEN_FLAGS_COMMON, 0);
    path_close(path);
    path->fd_ = new_fd;
    linux_syscall_fstat(path->fd_, &path->stat_);
    path->fd_read_  = linux_syscall_openat(name, path->fd_, OPEN_FLAGS_READ  | OPEN_FLAGS_COMMON, 0);
    path->fd_write_ = linux_syscall_openat(name, path->fd_, OPEN_FLAGS_WRITE | OPEN_FLAGS_COMMON, 0);
}

void path_navigate(Path* path, String name) {
    char name_c[PATH_MAX];
    if(name.length >= PATH_MAX - 1) {
        fprintf(stderr, "File name \"%.*s\" is to long.\n", name.length, name.data);
    }
    c_string_copy(name_c, name.data, name.length);
    name_c[name.length] = '\0';
    path_navigate_c(path, name_c);
}

int path_is_directory(Path* path) {
    return path->stat_.st_mode == FILE_MODE_DIRECTORY;
}

ulong path_get_file_size(Path* path) {
    return path->stat_.st_size;
}

String path_get_name(Path* path, Arena* arena) {
    char proc_path_name[256];
    sprintf(proc_path_name, "/proc/self/fd/%i", path->fd_);
    Stat sb;
    int e;
    if((e = linux_syscall_lstat(proc_path_name, &sb)) < 0) {
        fprintf(stderr, "Error: lstat");
        exit(1);
    }
    int supposed_file_name_length = sb.st_size + 1;
    if(sb.st_size == 0) supposed_file_name_length = PATH_MAX;
    String result;
    result.data   = arena_alloc_array(arena, supposed_file_name_length, char);
    int length = linux_syscall_readlink(proc_path_name, result.data,
                             supposed_file_name_length);
    if(length == -1) {
        perror("readlink");
        exit(1);
    }
    result.length = length;
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
        c_string_copy(result.data, last_slash + 1, result.length);
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
        if(c_string_equal(de->d_name, "."))
            continue;
        if(c_string_equal(de->d_name, ".."))
            continue;
        break;
    } while(1);
    path_close(&dir_iter->path);
    dir_iter->path.fd_       = linux_syscall_openat(de->d_name, dir_iter->dir_path->fd_,
                                              OPEN_FLAGS_READ  | OPEN_FLAGS_COMMON, 0);
    dir_iter->path.fd_read_  = linux_syscall_openat(de->d_name, dir_iter->dir_path->fd_,
                                              OPEN_FLAGS_READ  | OPEN_FLAGS_COMMON, 0);
    dir_iter->path.fd_write_ = linux_syscall_openat(de->d_name, dir_iter->dir_path->fd_,
                                              OPEN_FLAGS_WRITE | OPEN_FLAGS_COMMON, 0);
    linux_syscall_fstat(dir_iter->path.fd_, &dir_iter->path.stat_);
    return 1;
}

DirIter dir_iter_begin(Path* path) {
    DirIter result = { };
    result.dp_ = fdopendir(path->fd_);
    if(!result.dp_) {
        //fprintf(stderr, "%s\n", strerror(errno));
        exit(1);
    }
    result.dir_path = path;
    return result;
}

void dir_iter_end(DirIter* dir_iter) {
    path_close(&dir_iter->path);
    closedir(dir_iter->dp_);
}

#undef INVALID_FD
#elif defined(WINDOWS)


void global_init() {
    Win32SystemInfo system_info;
    GetSystemInfo(&system_info);
    global.page_size = system_info.dwPageSize;
    assert(0);
}


void* page_alloc(ulong num_pages) {
    if(!global.is_initialized) global_init();

    return malloc(global.page_size * num_pages);
    /* return VirtualAlloc(0, global.page_size * num_pages, */
    /*                     MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE); */
}

void page_dealloc(void* page, ulong num_pages) {
    if(!global.is_initialized) global_init();
    free(page);
//    VirtualFree(page, num_pages * global.page_size, MEM_RELEASE);
}
void* page_realloc(void* pages, ulong old_num_pages, ulong new_num_pages) {
    if(!global.is_initialized) global_init();
    return realloc(pages, global.page_size * new_num_pages);
}


void print_error(String s) {
    
}



static
wchar* utf8_to_wchar(String str) {
#if 0
    int length = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS,
                                     str.data, str.length, NULL, 0);
    LPWSTR result = LocalAlloc(LMEM_FIXED, length * sizeof(wchar_t));
    MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS,
                        str.data, str.length, result, length);
    return result;
#else
    return NULL;
#endif
}

static
wchar* utf8_to_wchar_c(char* str) {
#if 0
    int length = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS,
                                     str, -1, NULL, 0);
    LPWSTR result = LocalAlloc(LMEM_FIXED, length * sizeof(wchar_t));
    MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS,
                        str, -1, result, length);
    return result;
#else
    return NULL;
#endif
}

char* wchar_to_utf8_c(wchar* str) {
#if 0
    int length = WideCharToMultiByte(CP_UTF8, WC_NO_BEST_FIT_CHARS,
                                    str, -1, NULL, 0, NULL, NULL);
    char* result = LocalAlloc(LMEM_FIXED, length);
    WideCharToMultiByte(CP_UTF8, WC_NO_BEST_FIT_CHARS,
                        str, -1, result, length, NULL, NULL);
    return result;
#else
    return NULL;
#endif
}

static
String wchar_to_utf8(wchar* str) {
#if 0
    int length = WideCharToMultiByte(CP_UTF8, WC_NO_BEST_FIT_CHARS,
                                    str, -1, NULL, 0, NULL, NULL);
    String result = {
        .data = LocalAlloc(LMEM_FIXED, length),
        .length = length - 1
    };
    WideCharToMultiByte(CP_UTF8, WC_NO_BEST_FIT_CHARS,
                        str, -1, result.data, length, NULL, NULL);
    return result;
#else
    return (String){};
#endif
}


void path_load_stats(Path* path) {
    GetFileAttributesExW(path->path_name_, WIN32_GET_FILEEX_INFO_STANDARD, &path->data_);
}

void path_init_c(Path* path, char* name) {
    wchar* wname = utf8_to_wchar_c(name);
    ulong path_name_length = GetFullPathNameW(wname, 0, NULL, NULL);
    path->path_name_ = LocalAlloc(WIN32_LMEM_FIXED, path_name_length);
    GetFullPathNameW(wname, path_name_length, path->path_name_, NULL);
    LocalFree(wname);
    path->handle_ = CreateFileW(path->path_name_, WIN32_GENERIC_READ,
                                        WIN32_FILE_SHARE_READ, 0, WIN32_OPEN_EXISTING, 0, 0);
    path_load_stats(path);
}

void path_init(Path* path, String name) {
    char name_c[PATH_MAX];
    if(name.length >= PATH_MAX - 1) {
        fprintf(stderr, "File name \"%.*s\" is to long.\n", name.length, name.data);
    }
    c_string_copy(name_c, name.data, name.length);
    name_c[name.length] = '\0';
    path_init_c(path, name_c);
    path_load_stats(path);
}

void path_init_from_c(Path* path, Path* dir, char* name) {
    wchar* wname = utf8_to_wchar_c(name);
    if(PathIsRelativeW(wname)) {
        int path_length = lstrlenW(wname) + lstrlenW(dir->path_name_) + 1;
        path->path_name_ = LocalAlloc(WIN32_LMEM_FIXED, path_length * sizeof(wchar));
        lstrcpyW(path->path_name_, dir->path_name_);
        lstrcatW(path->path_name_, wname);
        LocalFree(wname);
    } else {
        path->path_name_ = wname;
    }
    path->handle_ = CreateFileW(path->path_name_, WIN32_GENERIC_READ,
                                WIN32_FILE_SHARE_READ, 0, WIN32_OPEN_EXISTING, 0, 0);
    path_load_stats(path);
}


void path_init_from(Path* path, Path* dir, String name) {
    wchar* wname = utf8_to_wchar(name);
    if(PathIsRelativeW(wname)) {
        int path_length = lstrlenW(wname) + lstrlenW(dir->path_name_) + 1;
        path->path_name_ = LocalAlloc(WIN32_LMEM_FIXED, path_length * sizeof(wchar));
        lstrcpyW(path->path_name_, dir->path_name_);
        lstrcatW(path->path_name_, wname);
        LocalFree(wname);
    } else {
        path->path_name_ = wname;
    }
    path->handle_ = CreateFileW(path->path_name_, WIN32_GENERIC_READ,
                                WIN32_FILE_SHARE_READ, 0, WIN32_OPEN_EXISTING, 0, 0);
    path_load_stats(path);
}

void path_create_c(Path* path, char* name) {
    wchar* wname = utf8_to_wchar_c(name);
    ulong path_name_length = GetFullPathNameW(wname, 0, NULL, NULL);
    path->path_name_ = LocalAlloc(WIN32_LMEM_FIXED, path_name_length);
    GetFullPathNameW(wname, path_name_length, path->path_name_, NULL);
    LocalFree(wname);
    path->handle_ = CreateFileW(path->path_name_, WIN32_GENERIC_READ,
                                        WIN32_FILE_SHARE_READ, 0, WIN32_OPEN_ALWAYS, 0, 0);
    path_load_stats(path);
}

void path_create(Path* path, String name) {
    char name_c[PATH_MAX];
    if(name.length >= PATH_MAX - 1) {
        fprintf(stderr, "File name \"%.*s\" is to long.\n", name.length, name.data);
    }
    c_string_copy(name_c, name.data, name.length);
    name_c[name.length] = '\0';
    path_init_c(path, name_c);
    path_load_stats(path);
}

void path_create_from_c(Path* path, Path* dir, char* name) {
    wchar* wname = utf8_to_wchar_c(name);
    if(PathIsRelativeW(wname)) {
        int path_length = lstrlenW(wname) + lstrlenW(dir->path_name_) + 1;
        path->path_name_ = LocalAlloc(WIN32_LMEM_FIXED, path_length * sizeof(wchar));
        lstrcpyW(path->path_name_, dir->path_name_);
        lstrcatW(path->path_name_, wname);
        LocalFree(wname);
    } else {
        path->path_name_ = wname;
    }
    path->handle_ = CreateFileW(path->path_name_, WIN32_GENERIC_READ,
                                WIN32_FILE_SHARE_READ, 0, WIN32_OPEN_ALWAYS, 0, 0);
    path_load_stats(path);
}


void path_create_from(Path* path, Path* dir, String name) {
    wchar* wname = utf8_to_wchar(name);
    if(PathIsRelativeW(wname)) {
        int path_length = lstrlenW(wname) + lstrlenW(dir->path_name_) + 1;
        path->path_name_ = LocalAlloc(WIN32_LMEM_FIXED, path_length * sizeof(wchar));
        lstrcpyW(path->path_name_, dir->path_name_);
        lstrcatW(path->path_name_, wname);
        LocalFree(wname);
    } else {
        path->path_name_ = wname;
    }
    path->handle_ = CreateFileW(path->path_name_, WIN32_GENERIC_READ,
                                WIN32_FILE_SHARE_READ, 0, WIN32_OPEN_ALWAYS, 0, 0);
    path_load_stats(path);
}


void path_init_working_directory(Path* path) {
    ulong path_name_length = GetCurrentDirectoryW(0, NULL);
    path->path_name_ = LocalAlloc(WIN32_LMEM_FIXED, path_name_length);
    GetCurrentDirectoryW(path_name_length, path->path_name_);
    path->handle_ = CreateFileW(path->path_name_, WIN32_GENERIC_READ,
                                        WIN32_FILE_SHARE_READ, 0, WIN32_OPEN_ALWAYS, 0, 0);
}

void path_copy(Path* orig, Path* copy) {
    *copy = (Path){ 0 };
    copy->handle_ = CreateFileW(orig->path_name_, WIN32_GENERIC_READ,
                                        WIN32_FILE_SHARE_READ, 0, WIN32_OPEN_EXISTING, 0, 0);
    int path_length = lstrlenW(orig->path_name_) + 1;
    copy->path_name_ = LocalAlloc(WIN32_LMEM_FIXED, path_length * sizeof(wchar_t));
    lstrcpyW(copy->path_name_, orig->path_name_);
    path_load_stats(copy);
}

Buffer path_open_file_aligned(Path* path, Arena* arena, int alignment) {
    ulong file_size = path_get_file_size(path);
    Buffer result = { 0 };
    result.begin = arena_alloc(arena, file_size, alignment);
    result.end = result.begin + file_size;
    ReadFile(path->handle_, result.begin, file_size, NULL, NULL);
    return result;
}

Buffer path_open_file(Path* path, Arena* arena) {
    return path_open_file_aligned(path, arena, 1);
}

void path_close(Path* path) {
    CloseHandle(path->handle_);
    LocalFree(path->path_name_);
}

/* TODO
void path_navigate_c(Path* path, char* name) {
    int new_fd = openat(path->fd_, name, O_RDONLY);
    path_close(path);
    path->fd_ = new_fd;
    path->has_stats = 0;
    path->invalid = 0;
}
*/

/* TODO?!
void path_navigate(Path* path, String name) {
    char name_c[PATH_MAX];
    if(name.length >= PATH_MAX - 1) {
        fprintf(stderr, "File name \"%.*s\" is to long.\n", name.length, name.data);
    }
    c_string_copy(name_c, name.data, name.length);
    name_c[name.length] = '\0';
    path_navigate_c(path, name_c);
}
*/


Buffer path_read_file_aligned(Path* path, Arena* arena, int alignment) {
    Win32LargeInteger file_size;
    GetFileSizeEx(path->handle_, &file_size);
    Buffer result = { 0 };
    result.begin = arena_alloc(arena, file_size.quad_part, alignment);
    result.end = result.begin + file_size.quad_part;
    ulong size_read;
    ReadFile(path->handle_, result.begin, file_size.quad_part, &size_read, 0);
    return result;
}

Buffer path_read_file(Path* path, Arena* arena) {
    return path_read_file_aligned(path, arena, 1);
}


void path_write_file(Path* path, Buffer buffer) {
    WriteFile(path->handle_, buffer.begin, buffer.end - buffer.begin, NULL, NULL);
}



int path_is_directory(Path* path) {
    path_load_stats(path);
    return path->data_.dwFileAttributes & WIN32_FILE_ATTRIBUTE_DIRECTORY;
}

ulong path_get_file_size(Path* path) {
    path_load_stats(path);
    Win32LargeInteger file_size_win;
    file_size_win.low_part  = path->data_.nFileSizeLow;
    file_size_win.high_part = path->data_.nFileSizeHigh;
    return (ulong)file_size_win.quad_part;
}

String path_get_name(Path* path, Arena* arena) {
    int path_name_length = WideCharToMultiByte(
        WIN32_CP_UTF8, WIN32_WC_NO_BEST_FIT_CHARS,
        path->path_name_, -1, NULL, 0, NULL, NULL);
    String result;
    result.data   = arena_alloc_array(arena, path_name_length, char);
    result.length = path_name_length;
    WideCharToMultiByte(WIN32_CP_UTF8, WIN32_WC_NO_BEST_FIT_CHARS,
                        path->path_name_, -1, result.data,
                        path_name_length, NULL, NULL);
    return result;
}

String path_get_base_name(Path* path, Arena* arena) {
    ulong path_name_size = GetFullPathNameW(path->path_name_, 0, NULL, NULL);
    wchar_t* base_name_win;
    wchar_t* path_name_win = arena_alloc_array(arena, path_name_size, wchar_t);
    GetFullPathNameW(path->path_name_, path_name_size, path_name_win, &base_name_win);
    String base_name = wchar_to_utf8(base_name_win);
    String result = { .length = base_name.length };
    result.data = arena_alloc_array(arena, base_name.length, char);
    for(int i = 0; i < result.length; i++) {
        result.data[i] = base_name.data[i];
    }
    LocalFree(base_name.data);
    return result;
    
}

int dir_iter_next(DirIter* dir_iter) {
    if(dir_iter->last_iteration) {
        return 1;
    }
    if(!dir_iter->first_iteration) {
        path_close(&dir_iter->path);
        dir_iter->last_iteration = 0;
    }
    dir_iter->handle_ =
        CreateFileW(dir_iter->find_file_data_.cFileName,
                            WIN32_GENERIC_READ, WIN32_FILE_SHARE_READ, 0,
                            WIN32_OPEN_EXISTING, 0, 0);
    
    b32 error =  FindNextFileW(dir_iter->handle_, &dir_iter->find_file_data_);
    if(!error) {
        if(GetLastError() == WIN32_ERROR_NO_MORE_FILES) {
            dir_iter->last_iteration = 1;
        } else {
            fprintf(stderr, "Error occured\n");
            exit(1);
        }
    }
    return 0;
}

DirIter dir_iter_begin(Path* path) {
    DirIter result = { 0 };
    int path_length = lstrlenW(path->path_name_) + 3;
    wchar* search_name = LocalAlloc(WIN32_LMEM_FIXED, path_length * sizeof(wchar));
    lstrcpyW(search_name, path->path_name_);
    lstrcatW(search_name, L"\\*");
    result.handle_ = FindFirstFileW(search_name, &result.find_file_data_);
    //TODO: Error
    result.dir_path = path;
    result.last_iteration = 1;
    result.last_iteration = 0;
    return result;
}

void dir_iter_end(DirIter* dir_iter) {
    path_close(&dir_iter->path);
    FindClose(dir_iter->handle_);
}

#elif defined(WASM)

void free(void *ptr);
void *calloc(size_t nmemb, size_t size);

typedef struct FILE FILE;
FILE* stderr;
int printf(const char *format, ...);
int fprintf(FILE* fp, const char *format, ...);

void* memset(void *s, int c, size_t n) {
    for(size_t i = 0; i < n; i++) {
        ((char*)s)[i] = c;
    }
    return s;
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

typedef enum {
    BML_PARSER_STATE_INDENT,
    BML_PARSER_STATE_INDENT_DECREASE,
    BML_PARSER_STATE_CHILD,
    BML_PARSER_STATE_ATTRIBUTE,
    BML_PARSER_STATE_ATTRIBUTE_END,
    BML_PARSER_STATE_EOF,
} BmlParserState;


BmlNode bml_skip(BmlParser* parser) {
    int depth = 1;
    while(depth > 0) {
        BmlNode node = bml_parse(parser);
        switch(node.type) {
        case BML_NODE: {
            //printf("Node\n");
            depth++;
            break;
        }
        case BML_NODE_END: {
            //printf("End\n");
            depth--;
            break;
        }
        default: {
            return node;
        }
        }
    }
    return bml_parse(parser);
}

void bml_parser_advance(BmlParser* parser) {
    if(parser->pos < (char*)parser->buffer.end) {
        parser->pos++;
    } else {
        parser->state = BML_PARSER_STATE_EOF;
    }
}

int bml_is_valid_char(char c)  {  //A-Z, a-z, 0-9, -.
    return (c >= 'a' && c <= 'z')
        || (c >= 'A' && c <= 'Z')
        || (c >= '0' && c <= '9')
        || c == '-' || c == '.';
}


void bml_parse_name(BmlParser* parser, BmlNode* node) {
    if(parser->pos >= (char*)parser->buffer.end) {
        parser->state = BML_PARSER_STATE_EOF;
        return;
    }
    char* begin = parser->pos;
    while(parser->state != BML_PARSER_STATE_EOF &&
          bml_is_valid_char(parser->pos[0])) {
        bml_parser_advance(parser);
    }
    if(parser->pos == begin) {
        node->type = BML_NODE_ERROR_INVALID_NODE_NAME;
    }
    node->name = (String) {
        .data = begin,
        .length = parser->pos - begin
    };
}

void bml_parse_data(BmlParser* parser, BmlNode* node) {
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
                //error_at_pos(parser->text_pos);
                fprintf(stderr, "Unescaped value.\n");
                exit(1);
                break;
            case '\r':
                if(parser->pos[1] == '\n') {
                    //error_at_pos(parser->text_pos);
                    fprintf(stderr, "Unescaped value.\n");
                    exit(1);
                }
            fallthrough default:
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
                //error_at_pos(parser->text_pos);
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

int bml_parse_indent(BmlParser* parser) {
    int result = 0;
    while(parser->state != BML_PARSER_STATE_EOF &&
          (parser->pos[0] == ' ' || parser->pos[0] == '\t')) {
        result++;
        bml_parser_advance(parser);
    }
    return result;
}

void bml_parser_init(BmlParser* parser, Buffer buffer) {
    *parser = (BmlParser) {
        .buffer        = buffer,
        .pos           = buffer.begin,
        .indent_levels = { },
        .state         = BML_PARSER_STATE_CHILD,
    };
}

static BmlNode end_node = { .type = BML_NODE_END };
static BmlNode eof_node = { .type = BML_NODE_EOF };

BmlNode bml_parse(BmlParser* parser) {
    BmlNode result = {};
    int cont = 1;
    while(cont) {
        switch(parser->state) {
        case BML_PARSER_STATE_INDENT: {
            int indent = bml_parse_indent(parser);
            if(parser->state == BML_PARSER_STATE_EOF) break;
            parser->indent = indent;
            int index = parser->indent_index;
            int current_indent = parser->indent_levels[index];
            if(current_indent < indent) {
                if(index == 15) {
                    return (BmlNode){ .type = BML_NODE_ERROR /* _NESTING_TOO_DEEP*/ };
                }
                parser->indent_index++;
                parser->indent_levels[index + 1] = parser->indent;
                parser->state = BML_PARSER_STATE_CHILD;
                break;
            } else if(current_indent > indent) {
                parser->state = BML_PARSER_STATE_INDENT_DECREASE;
                result = end_node;
                cont = 0;
                break;
            } else {
                parser->state = BML_PARSER_STATE_CHILD;
                result = end_node;
                cont = 0;
                break;
            }
        }
        case BML_PARSER_STATE_INDENT_DECREASE: {
            int index = parser->indent_index;
            int current_indent = parser->indent_levels[index];
            if(current_indent > parser->indent) {
                parser->indent_index--;
                result = end_node;
                cont = 0;
                break;
            } else if(current_indent < parser->indent) {
                result = (BmlNode){ .type = BML_NODE_ERROR /* _INDENTATION*/ };
                cont = 0;
                break;
            }
            parser->state = BML_PARSER_STATE_CHILD;
            break;
            //return end_node;
        }
        case BML_PARSER_STATE_CHILD: {
            result = (BmlNode) { .type = BML_NODE };
            if(parser->pos[0] == '\n') {
                parser->state = BML_PARSER_STATE_INDENT;
                bml_parser_advance(parser);
                break;
            }
            bml_parse_name(parser, &result);
            bml_parse_data(parser, &result);
            if(parser->state == BML_PARSER_STATE_EOF) break;
            parser->state = BML_PARSER_STATE_ATTRIBUTE;
            cont = 0;
            break;
        }
        case BML_PARSER_STATE_ATTRIBUTE: {
            while(parser->pos[0] == ' ') {
                bml_parser_advance(parser);
                if(parser->state == BML_PARSER_STATE_EOF) break;
            }
            if(parser->pos[0] == '\n') {
                bml_parser_advance(parser);
                if(parser->state == BML_PARSER_STATE_EOF) break;
                parser->state = BML_PARSER_STATE_INDENT;
                break;
            }
            result = (BmlNode) { .type = BML_NODE };
            bml_parse_name(parser, &result);
            bml_parse_data(parser, &result);
            if(parser->state == BML_PARSER_STATE_EOF) break;
            parser->state = BML_PARSER_STATE_ATTRIBUTE_END;
            cont = 0;
            break;
        }
        case BML_PARSER_STATE_ATTRIBUTE_END: {
            parser->state = BML_PARSER_STATE_ATTRIBUTE;
            result = end_node;
            cont = 0;
            break;
        }
        case BML_PARSER_STATE_EOF: {
            if(parser->indent_index >= 0) {
                parser->indent_index--;
                result =  end_node;
                cont = 0;
                break;
            } else {
                result = eof_node;
                cont = 0;
                break;
            }
        }
        }        
    }
#if 0
    switch(result.type) {
    case BML_NODE:
        printf("NODE %.*s: %.*s\n", result.name.length, result.name.data,
               result.value.length, result.value.data);
        break;
    case BML_NODE_END:
        printf("END\n");
        break;
    case BML_NODE_ERROR:
    case BML_NODE_ERROR_INVALID_NODE_NAME:
        printf("ERROR\n");
        break;
    case BML_NODE_EOF:
        printf("EOF\n");
        break;
    }
#endif
    return result;
}

#if 0
void bml_parse_attributes(BmlNode* node, BmlParser* parser) {
    while(1) {
        if(!parser->pos[0]) break;
        if(parser->pos[0] == '\n') {
            bml_parser_advance(parser);
            break;
        }
        if(parser->pos[0] != ' ') {
            //error_at_pos(parser->text_pos);
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

        char* begin = parser->pos;
        while(bml_is_valid_char(parser->pos[0])) bml_parser_advance(parser);
        if(parser->pos == begin) {
            //error_at_pos(parser->text_pos);
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


BmlNode* bml_parse_node(BmlParser* parser, int* indent) {
    if(parser->pos[0] == '\0') {
        return NULL;
    }
    if(parser->pos[0] == '\n') {
        if(*indent == 0) {
            bml_parser_advance(parser);
            *indent = bml_parse_indent(parser);
        }
    }
    BmlNode* node = arena_alloc_type(parser->arena, BmlNode);
    *node = (BmlNode) { };
    bml_parse_name(node, parser);
    bml_parse_data(node, parser);

    BmlNode** next_node_ptr = &node->child;

    // Parse attributes
    while(1) {
        if(!parser->pos[0]) break;
        if(parser->pos[0] == '\n') {
            bml_parser_advance(parser);
            break;
        }
        if(parser->pos[0] != ' ') {
            //error_at_pos(parser->text_pos);
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

        char* begin = parser->pos;
        while(bml_is_valid_char(parser->pos[0])) bml_parser_advance(parser);
        if(parser->pos == begin) {
            //error_at_pos(parser->text_pos);
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
            //error_at_pos(parser->text_pos);
            fprintf(stderr, "Indentation error.\n");
            exit(1);
        } else {
            BmlNode* new_node = bml_parse_node(parser, &new_indent);
            *next_node_ptr = new_node;
            next_node_ptr = &new_node->next;
            if(new_indent <= *indent) {
                *indent = new_indent;
                return node;
            } else if(new_indent != child_indent) {
                //error_at_pos(parser->text_pos);
                fprintf(stderr, "Indentation error.\n");
                exit(1);
            }
        }
    }
}

BmlNode* bml_parse(Buffer buffer, Arena* arena) {
    BmlParser parser = { };
    parser.buffer = buffer;
    parser.pos    = (char*)buffer.begin;
    //parser.text_pos.line_number = 1;
    parser.arena = arena;
    int indent = 0;
    BmlNode* root;
    BmlNode** next_node_ptr = &root;
    while(parser.pos[0] != '\0') {
        *next_node_ptr =  bml_parse_node(&parser, &indent);
        next_node_ptr = &((*next_node_ptr)->next);
    }
    return root;
}



void bml_print_node(BmlNode* node, int indent) {
    if(node) {
        printf("%*s%.*s: %.*s\n", indent, "", node->name.length, node->name.data,
               node->value.length, node->value.data);
        bml_print_node(node->child, indent + 2);
        bml_print_node(node->next, indent);
    }
}

#endif

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

#endif //BASE_IMPLEMENTATION_GUARD
#endif //BASE_IMPLEMENTATION
