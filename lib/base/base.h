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
typedef long      wchar;
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


#define STR_(x) #x
#define STR(x) STR_(x)
#define CODE_LOCATION STR(__FILE__) ": " STR(__LINE__) 

#include "platform_gcc.h"
#include "platform_clang.h"
#include "platform_msvc.h"

#define unused(x) ((void)x);
#define invalid_code_path assert(!"Invalid code path")
#define not_implemented assert(!"Not implemented")
#define invalid_default_case default: invalid_code_path; break

#define alignof(x) __alignof(x)


//#define NULL ((void*)0)

#define KB(x) (x * 1024ull)
#define MB(x) (x * 1024ull * 1024ull)
#define GB(x) (x * 1024ull * 1024ull * 1024ull)

#define array_length(array) (sizeof(array)/sizeof(array[0]))
#undef max
#undef min
#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))

////////////////////////////////
// Platform dependent structs //
////////////////////////////////

#include "platform_linux.h"
#include "platform_windows.h"
#include "platform_wasm.h"

////////////////////
// Base interface //
////////////////////

typedef struct  {
    uint page_size;
    uint is_initialized : 1;
} BaseGlobal;
extern BaseGlobal base_global;
void base_global_init();

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
void   copy_buffer(Buffer src, Buffer dest);
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
Buffer  arena_alloc_buffer(Arena* a, ulong size, ulong align);
void*   arena_dealloc(Arena* a);

typedef struct {
    Arena* arena;
    void*  old_current;
} Temp;

Temp temp_begin(Arena* a);
void temp_end(Temp tmp);

typedef struct {
    char* data;
    ulong length;
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

String string_format(Buffer buffer, String format, ...);
String string_format_list(Buffer buffer, String format, VariableArgumentList arg_list);
int c_string_format(char* buffer, int length, char* format, ...);
int c_string_format_list(char* buffer, int length, char* format, VariableArgumentList arg_list);

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
int   c_string_format(char* buffer, int length, char* format, ...);
int   c_string_format_unsafe(char* buffer, char* format, ...);

void  c_print_format(char* format, ...);
void  c_error_format(char* format, ...);

#define string_split_foreach(s, t, c)                                   \
    String t = { .data = (s).data };                                    \
    for(char* macro_c_ = (s).data; macro_c_ <= (s).data + (s).length;   \
        t.data = (*macro_c_ == c) ? macro_c_ + 1 : t.data,              \
            macro_c_++, t.length = macro_c_ - t.data)                   \
        if(*macro_c_ == c || macro_c_ == (s).data + (s).length)         \

#ifndef NDEBUG
#define error_fixed_cstring(s)  error(L(s))
#define assert(x) if(!(x)) {                                            \
        error_fixed_cstring("Assertion (" #x ") at " CODE_LOCATION ".\n"); \
        __builtin_trap();}
#else
#define assert(x)
#endif

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

void c_write(char* c);
void write(String s);

void c_error(char* c);
void error(String s);

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
u32  hash_murmur3(void* data, uint data_length);
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

define_hashmap(VFS, vfs, VFSEntry, vfs_entry_hash, vfs_entry_equal);

void vfs_add_dir(VFS* vfs, Path* path, Arena* arena);

#endif //BASE_GUARD

#ifdef BASE_IMPLEMENTATION
#ifndef BASE_IMPLEMENTATION_GUARD
#define BASE_IMPLEMENTATION_GUARD

BaseGlobal base_global = { 0 };

///////////////////////////////////////
// Platform dependent implementation //
///////////////////////////////////////

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

#endif //BASE_IMPLEMENTATION_GUARD
#endif //BASE_IMPLEMENTATION
