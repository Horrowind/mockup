#ifndef BASE_GUARD
#define BASE_GUARD

#if defined(LINUX)
#define _GNU_SOURCE
#include <dirent.h>    // readdir, fdopendir, closedir
#include <stdlib.h>    // calloc, free
#include <string.h>    // stpncpy, strerror
#include <stdio.h>     // fprintf, sprintf
#elif defined(WINDOWS)
#define UNICODE
#define _UNICODE
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <shlwapi.h>
#include <pathcch.h>
#elif defined(WASM)
#include <sys/stat.h>
#endif


#if defined(WASM)
void* malloc(size_t size);
void  free(void *ptr);
void* calloc(size_t nmemb, size_t size);
void* memcpy(void *dest, const void *src, size_t n);
int   memcmp(const void *s1, const void *s2, size_t n);
void* memset(void *s, int c, size_t n);

void exit(int status);

typedef struct FILE FILE;
FILE* stderr;
int   printf(const char *format, ...);
int   fprintf(FILE* fp, const char *format, ...);
int   sprintf(char *str, const char *format, ...);
char* strcat(char *dest, const char *src);
#endif


// x86_64
typedef signed   char   i8;
typedef unsigned char   u8;
typedef          short  i16;
typedef unsigned short  u16;
typedef signed   int    i32;
typedef unsigned int    u32;
typedef unsigned int    b32;
typedef signed   long   i64;
typedef unsigned long   u64;
typedef unsigned long   b64;
typedef float           f32;
typedef double          f64;

typedef unsigned long   ulong;
typedef unsigned int    uint;
typedef unsigned short  ushort;
typedef unsigned char   uchar;

typedef          long   iptr;
typedef unsigned long   uptr;


//#define NULL ((void*)0)

#define STR_(x) #x
#define STR(x) STR_(x)
#define CODE_LOCATION STR(__FILE__) ": " STR(__LINE__) 

#define invalid_code_path assert(!"Invalid code path")
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
#define debug_break raise_signal(2 /*SIGINT*/)

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
void  page_free(void* pages, ulong num_pages);

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

b32 string_equal(String s1, String s2);
String string_from_c_string(char* buf);
u64 string_to_u64(String s);
u64 string_to_u64_base(String s, int base);

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

#endif


typedef struct {
#if defined(LINUX)
    int fd_;
    int fd_read_;
    int fd_write_;
    Stat stat_;
#elif defined(WINDOWS)
    HANDLE handle_;
    PWSTR  path_name_;
    WIN32_FILE_ATTRIBUTE_DATA data_;
#endif
} Path;

typedef struct {
    Path path;
    Path* dir_path;
#if defined(LINUX)
    DIR* dp_;
#elif defined(WINDOWS)
    HANDLE handle_;
    WIN32_FIND_DATA find_file_data_;
    int first_iteration : 1;
    int last_iteration  : 1;
#endif
} DirIter;

void       path_init(Path* path, String name);
void       path_init_c(Path* path, char* name);
void       path_init_working_directory(Path* path);
void       path_init_from_c(Path* path, Path* dir, char* name);
void       path_init_from(Path* path, Path* dir, String name);
void       path_copy(Path* orig, Path* copy);
int        path_can_read(Path* path);
Buffer     path_read_file(Path* path, Arena* arena);
Buffer     path_read_file_aligned(Path* path, Arena* arena, int alignment);
int        path_can_write(Path* path);
void       path_write_file(Path* path, Buffer buffer);
void       path_close(Path* path);
void       path_navigate(Path* path, String name);
void       path_navigate_c(Path* path, char* name);
ulong      path_get_file_size(Path* path);
int        path_is_directory(Path* path);
String     path_get_name(Path* path, Arena* arena); //TODO
String     path_get_base_name(Path* path, Arena* arena); //TODO
DirIter    dir_iter_begin(Path* path);
int        dir_iter_next(DirIter* dir_iter);
void       dir_iter_end(DirIter* dir_iter);


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
            u32 used = map->entries[i].used;                            \
            if(used) {                                                  \
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

#if defined(WASM)

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

#if defined(LINUX)

#define INVALID_FD (-1)

static __inline long syscall0(long n) {
	unsigned long ret;
	__asm__ __volatile__ ("syscall" : "=a"(ret) : "a"(n) : "rcx", "r11", "memory");
	return ret;
}

static __inline long syscall1(long n, long a1) {
	unsigned long ret;
	__asm__ __volatile__ ("syscall" : "=a"(ret) : "a"(n), "D"(a1) : "rcx", "r11", "memory");
	return ret;
}

static __inline long syscall2(long n, long a1, long a2) {
	unsigned long ret;
	__asm__ __volatile__ ("syscall" : "=a"(ret) : "a"(n), "D"(a1), "S"(a2)
						  : "rcx", "r11", "memory");
	return ret;
}

static __inline long syscall3(long n, long a1, long a2, long a3) {
	unsigned long ret;
	__asm__ __volatile__ ("syscall" : "=a"(ret) : "a"(n), "D"(a1), "S"(a2),
						  "d"(a3) : "rcx", "r11", "memory");
	return ret;
}

static __inline long syscall4(long n, long a1, long a2, long a3, long a4) {
	unsigned long ret;
	register long r10 __asm__("r10") = a4;
	__asm__ __volatile__ ("syscall" : "=a"(ret) : "a"(n), "D"(a1), "S"(a2),
						  "d"(a3), "r"(r10): "rcx", "r11", "memory");
	return ret;
}

static __inline long syscall5(long n, long a1, long a2, long a3, long a4, long a5) {
	unsigned long ret;
	register long r10 __asm__("r10") = a4;
	register long r8 __asm__("r8") = a5;
	__asm__ __volatile__ ("syscall" : "=a"(ret) : "a"(n), "D"(a1), "S"(a2),
						  "d"(a3), "r"(r10), "r"(r8) : "rcx", "r11", "memory");
	return ret;
}

static __inline long syscall6(long n, long a1, long a2, long a3, long a4, long a5, long a6) {
	unsigned long ret;
	register long r10 __asm__("r10") = a4;
	register long r8 __asm__("r8") = a5;
	register long r9 __asm__("r9") = a6;
	__asm__ __volatile__ ("syscall" : "=a"(ret) : "a"(n), "D"(a1), "S"(a2),
						  "d"(a3), "r"(r10), "r"(r8), "r"(r9) : "rcx", "r11", "memory");
	return ret;
}


#define SYSCALL_NARGS_X_(a,b,c,d,e,f,g,h,n,...) n
#define SYSCALL_NARGS_(...) SYSCALL_NARGS_X_(__VA_ARGS__,7,6,5,4,3,2,1,0,)
#define SYSCALL_CONCAT_X_(a,b) a##b
#define SYSCALL_CONCAT_(a,b) SYSCALL_CONCAT_X_(a,b)
#define SYSCALL_DISP_(b,...) SYSCALL_CONCAT_(b, SYSCALL_NARGS_(__VA_ARGS__))(__VA_ARGS__)

#define syscall(...) SYSCALL_DISP_(syscall,__VA_ARGS__)

enum {
    SYSCALL_READ           =   0,
    SYSCALL_WRITE          =   1,
    SYSCALL_OPEN           =   2,
    SYSCALL_CLOSE          =   3,
    SYSCALL_FSTAT          =   5,
    SYSCALL_LSTAT          =   6,
    SYSCALL_MMAP           =   9,
    SYSCALL_MUNMAP         =  11,
    SYSCALL_RT_SIGPROCMASK =  14,
    SYSCALL_MREMAP         =  25,
    SYSCALL_DUP            =  32,
    SYSCALL_READLINK       =  89,
    SYSCALL_GETTID         = 186,
    SYSCALL_TKILL          = 200,
    SYSCALL_EXIT_GROUP     = 231,
    SYSCALL_OPENAT         = 257,
} LinuxSyscall;

enum {
    OPEN_FLAGS_READ           =       0,
    OPEN_FLAGS_WRITE          =       1,
    OPEN_FLAGS_READ_WRITE     =       2,


    OPEN_FLAGS_LARGE_FILE     =       0,
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

long syscall_read(int file_descriptor, void* buf, unsigned long count) {
    return syscall(SYSCALL_READ, (long)file_descriptor, (long)buf, (long)count);
}

long syscall_write(int file_descriptor, const void* buf, unsigned long count) {
    return syscall(SYSCALL_WRITE, (long)file_descriptor, (long)buf, (long)count);
}

long syscall_open(const char* file_path, int flags, int mode) {
    return syscall(SYSCALL_OPEN, (long)file_path, (long)flags, (long)mode);
}

int syscall_close(int file_descriptor) {
    return (int)syscall(SYSCALL_CLOSE, (long)file_descriptor);
}

long syscall_fstat(int file_descriptor, Stat* stat) {
    return syscall(SYSCALL_FSTAT, (long)file_descriptor, (long)stat);
}

long syscall_lstat(const char* path_name, Stat* stat) {
    return syscall(SYSCALL_LSTAT, (long)path_name, (long)stat);
}

void* syscall_mmap(void* addr, unsigned long length, int prot,
                   int flags, int file_descriptor, off_t offset) {
    return (void*)syscall(SYSCALL_MMAP, (long)addr, (long)length, (long)prot, (long)flags,
                   (long)file_descriptor, (long)offset);
}

int syscall_munmap(void* start, size_t len) {
    return (int)syscall(SYSCALL_MUNMAP, (long)start, (long)len);
}

int syscall_rt_sigprocmask(int how, const ulong* signal_set,
                           ulong* old_signal_set, ulong signal_set_size) {
    return (int)syscall(SYSCALL_RT_SIGPROCMASK, (long)how, (long)signal_set,
                        (long)old_signal_set, (long)signal_set_size);
}

void* syscall_mremap(void* old_addr, unsigned long old_size,
                     unsigned long new_size, int flags, void* new_addr) {
    return (void*)syscall(SYSCALL_MREMAP, (long)old_addr, (long)old_size, (long)new_size, (long)flags,
                   (long)new_addr);
}

int syscall_dup(int file_descriptor) {
    return (int)syscall(SYSCALL_DUP, (long)file_descriptor);
}

int syscall_gettid() {
    return (int)syscall(SYSCALL_GETTID);
}

int syscall_tkill(int thread_id, int signal) {
    return (int)syscall(SYSCALL_TKILL, (long)thread_id, (long)signal);
}

long syscall_readlink(const char* restrict path_name, char* restrict buffer, unsigned long buffer_size) {
    return syscall(SYSCALL_READLINK, (long)path_name, (long)buffer, (long)buffer_size);
}

void syscall_exit_group(int status) {
    syscall(SYSCALL_EXIT_GROUP, (long)status);
}

long syscall_openat(const char* file_path, int file_descriptor, int flags, int mode) {
    return syscall(SYSCALL_OPENAT, (long)file_descriptor, (long)file_path, (long)flags, (long)mode);
}


void print(String s) {
    syscall_write(1, s.data, s.length);
}

void print_error(String s) {
    syscall_write(2, s.data, s.length);
}

void print_format(const char* s, ulong length) {
    syscall_write(1, s, length);
}

void error_print_format(const char* s, ulong length) {
    syscall_write(2, s, length);
}


_Noreturn
void exit(int status) {
    syscall_exit_group(status);
    for(;;) {}
}


int raise_signal(int signal) {
    int thread_id;
    int result;
    ulong signal_set[8];
    static const unsigned long app_mask[] = {
        0x7fffffff, 0xfffffffc
    };
    syscall_rt_sigprocmask(0 /*SIG_BLOCK*/, app_mask, signal_set, 8);
    thread_id = syscall_gettid();
    result    = syscall_tkill(thread_id, signal);
    syscall_rt_sigprocmask(2 /*SIG_SETMASK*/, signal_set, 0, 8);
    return result;
}


void global_init() {
    global.page_size      = 4096;
    global.is_initialized = 1;
}

void* page_alloc(ulong num_pages) {
    if(!global.is_initialized) global_init();
    return syscall_mmap(NULL, global.page_size * num_pages, MEMORY_FLAGS_READ | MEMORY_FLAGS_WRITE,
                        MEMORY_FLAGS_PRIVATE | MEMORY_FLAGS_ANONYMOUS, 0, 0);
}

void* page_realloc(void* pages, ulong old_num_pages, ulong new_num_pages) {
    if(!global.is_initialized) global_init();
    return syscall_mremap(pages, global.page_size * old_num_pages,
                          global.page_size * new_num_pages, MEMORY_FLAGS_MAYMOVE, 0);
}


void page_free(void* page, ulong num_pages) {
    if(!global.is_initialized) global_init();
    syscall_munmap(page, num_pages * global.page_size);
}


#elif defined(WINDOWS)

void global_init() {
    SYSTEM_INFO system_info;
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

void page_free(void* page, ulong num_pages) {
    if(!global.is_initialized) global_init();
    free(page);
//    VirtualFree(page, num_pages * global.page_size, MEM_RELEASE);
}
void* page_realloc(void* pages, ulong old_num_pages, ulong new_num_pages) {
    if(!global.is_initialized) global_init();
    return realloc(pages, global.page_size * new_num_pages);
}

#endif



Arena arena_create(ulong size) {
    if(!global.is_initialized) global_init();
    Buffer buffer = {
        .begin = page_alloc((size + global.page_size - 1) / global.page_size),
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

#if defined(LINUX)


void path_init_c(Path* path, char* name) {
    path->fd_       = syscall_open(name, OPEN_FLAGS_READ  | OPEN_FLAGS_COMMON, 0);
    path->fd_read_  = syscall_open(name, OPEN_FLAGS_READ  | OPEN_FLAGS_COMMON, 0);
    path->fd_write_ = syscall_open(name, OPEN_FLAGS_WRITE | OPEN_FLAGS_COMMON, 0);
    syscall_fstat(path->fd_, &path->stat_);
}

void path_init(Path* path, String name) {
    char name_c[PATH_MAX];
    if(name.length >= PATH_MAX - 1) {
        fprintf(stderr, "File name \"%.*s\" is to long.\n", name.length, name.data);
    }
    stpncpy(name_c, name.data, name.length);
    name_c[name.length] = '\0';
    path_init_c(path, name_c);
}

void path_init_from_c(Path* path, Path* dir, char* name) {
    path->fd_       = syscall_openat(name, dir->fd_, OPEN_FLAGS_READ  | OPEN_FLAGS_COMMON, 0);
    path->fd_read_  = syscall_openat(name, dir->fd_, OPEN_FLAGS_READ  | OPEN_FLAGS_COMMON, 0);
    path->fd_write_ = syscall_openat(name, dir->fd_, OPEN_FLAGS_WRITE | OPEN_FLAGS_COMMON, 0);
    syscall_fstat(path->fd_, &path->stat_);
}


void path_init_from(Path* path, Path* dir, String name) {
    char name_c[PATH_MAX];
    if(name.length >= PATH_MAX - 1) {
        fprintf(stderr, "File name \"%.*s\" is to long.\n", name.length, name.data);
    }
    stpncpy(name_c, name.data, name.length);
    name_c[name.length] = '\0';
    path_init_c(path, name_c);
}


void path_init_working_directory(Path* path) {
    path_init_c(path, ".");
}

void path_copy(Path* orig, Path* copy) {
    *copy = (Path){ 0 };
    copy->fd_ = syscall_dup(orig->fd_);
    syscall_fstat(copy->fd_, &copy->stat_);
    copy->fd_read_ = syscall_dup(orig->fd_read_);
    copy->fd_write_ = syscall_dup(orig->fd_write_);
}

Buffer path_read_file_aligned(Path* path, Arena* arena, int alignment) {
    size_t file_size = path->stat_.st_size;
    Buffer result = { 0 };
    result.begin = arena_alloc(arena, file_size, alignment);
    result.end = result.begin + file_size;
    syscall_read(path->fd_read_, result.begin, file_size);
    return result;
}

Buffer path_read_file(Path* path, Arena* arena) {
    return path_read_file_aligned(path, arena, 1);
}

void path_write_file(Path* path, Buffer buffer) {
    syscall_write(path->fd_write_, buffer.begin, buffer.end - buffer.begin);
}

void path_close(Path* path) {
    if(path->fd_ != INVALID_FD) {
        syscall_close(path->fd_);
    }
    if(path->fd_read_ != INVALID_FD) {
        syscall_close(path->fd_read_);
    }
    if(path->fd_write_ != INVALID_FD) {
        syscall_close(path->fd_write_);
    }
}

void path_navigate_c(Path* path, char* name) {
    int new_fd = syscall_openat(name, path->fd_, OPEN_FLAGS_READ | OPEN_FLAGS_COMMON, 0);
    path_close(path);
    path->fd_ = new_fd;
    syscall_fstat(path->fd_, &path->stat_);
    path->fd_read_  = syscall_openat(name, path->fd_, OPEN_FLAGS_READ  | OPEN_FLAGS_COMMON, 0);
    path->fd_write_ = syscall_openat(name, path->fd_, OPEN_FLAGS_WRITE | OPEN_FLAGS_COMMON, 0);
}

void path_navigate(Path* path, String name) {
    char name_c[PATH_MAX];
    if(name.length >= PATH_MAX - 1) {
        fprintf(stderr, "File name \"%.*s\" is to long.\n", name.length, name.data);
    }
    stpncpy(name_c, name.data, name.length);
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
    if((e = syscall_lstat(proc_path_name, &sb)) < 0) {
        fprintf(stderr, "lstat: %s", strerror(-e));
        exit(1);
    }
    int supposed_file_name_length = sb.st_size + 1;
    if(sb.st_size == 0) supposed_file_name_length = PATH_MAX;
    String result;
    result.data   = arena_alloc_array(arena, supposed_file_name_length, char);
    int length = syscall_readlink(proc_path_name, result.data,
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
        stpncpy(result.data, last_slash + 1, result.length);
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
    dir_iter->path.fd_       = syscall_openat(de->d_name, dir_iter->dir_path->fd_,
                                              OPEN_FLAGS_READ  | OPEN_FLAGS_COMMON, 0);
    dir_iter->path.fd_read_  = syscall_openat(de->d_name, dir_iter->dir_path->fd_,
                                              OPEN_FLAGS_READ  | OPEN_FLAGS_COMMON, 0);
    dir_iter->path.fd_write_ = syscall_openat(de->d_name, dir_iter->dir_path->fd_,
                                              OPEN_FLAGS_WRITE | OPEN_FLAGS_COMMON, 0);
    syscall_fstat(dir_iter->path.fd_, &dir_iter->path.stat_);
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

static
LPWSTR utf8_to_wchar(String str) {
    int length = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS,
                                     str.data, str.length, NULL, 0);
    LPWSTR result = LocalAlloc(LMEM_FIXED, length * sizeof(wchar_t));
    MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS,
                        str.data, str.length, result, length);
    return result;
}

static
LPWSTR utf8_to_wchar_c(char* str) {
    int length = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS,
                                     str, -1, NULL, 0);
    LPWSTR result = LocalAlloc(LMEM_FIXED, length * sizeof(wchar_t));
    MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS,
                        str, -1, result, length);
    return result;
}


char* wchar_to_utf8_c(LPWSTR str) {
    int length = WideCharToMultiByte(CP_UTF8, WC_NO_BEST_FIT_CHARS,
                                    str, -1, NULL, 0, NULL, NULL);
    char* result = LocalAlloc(LMEM_FIXED, length);
    WideCharToMultiByte(CP_UTF8, WC_NO_BEST_FIT_CHARS,
                        str, -1, result, length, NULL, NULL);
    return result;
}

static
String wchar_to_utf8(LPWSTR str) {
    int length = WideCharToMultiByte(CP_UTF8, WC_NO_BEST_FIT_CHARS,
                                    str, -1, NULL, 0, NULL, NULL);
    String result = {
        .data = LocalAlloc(LMEM_FIXED, length),
        .length = length - 1
    };
    WideCharToMultiByte(CP_UTF8, WC_NO_BEST_FIT_CHARS,
                        str, -1, result.data, length, NULL, NULL);
    return result;
}


void path_init_c(Path* path, char* name) {
    LPWSTR wname = utf8_to_wchar_c(name);
    DWORD path_name_length = GetFullPathNameW(wname, 0, NULL, NULL);
    path->path_name_ = LocalAlloc(LMEM_FIXED, path_name_length);
    GetFullPathNameW(wname, path_name_length, path->path_name_, NULL);
    LocalFree(wname);
    path->handle_ = CreateFileW(path->path_name_, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
    path->has_stats = 0;
    path->invalid = 0;
}

void path_init(Path* path, String name) {
    char name_c[PATH_MAX];
    if(name.length >= PATH_MAX - 1) {
        fprintf(stderr, "File name \"%.*s\" is to long.\n", name.length, name.data);
    }
    stpncpy(name_c, name.data, name.length);
    name_c[name.length] = '\0';
    path_init_c(path, name_c);
}

void path_init_from_c(Path* path, Path* dir, char* name) {
    LPWSTR wname = utf8_to_wchar_c(name);
    if(PathIsRelative(wname)) {
        int path_length = lstrlenW(wname) + lstrlenW(dir->path_name_) + 1;
        path->path_name_ = LocalAlloc(LMEM_FIXED, path_length * sizeof(wchar_t));
        lstrcpyW(path->path_name_, dir->path_name_);
        lstrcatW(path->path_name_, wname);
        LocalFree(wname);
    } else {
        path->path_name_ = wname;
    }
    path->handle_ = CreateFileW(path->path_name_, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
    path->has_stats = 0;
    path->invalid = 0;
}


void path_init_from(Path* path, Path* dir, String name) {
    LPWSTR wname = utf8_to_wchar(name);
    if(PathIsRelative(wname)) {
        int path_length = lstrlenW(wname) + lstrlenW(dir->path_name_) + 1;
        path->path_name_ = LocalAlloc(LMEM_FIXED, path_length * sizeof(wchar_t));
        lstrcpyW(path->path_name_, dir->path_name_);
        lstrcatW(path->path_name_, wname);
        LocalFree(wname);
    } else {
        path->path_name_ = wname;
    }
    path->handle_ = CreateFileW(path->path_name_, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
    path->has_stats = 0;
    path->invalid = 0;
}


void path_init_working_directory(Path* path) {
    DWORD path_name_length = GetCurrentDirectoryW(0, NULL);
    path->path_name_ = LocalAlloc(LMEM_FIXED, path_name_length);
    GetCurrentDirectoryW(path_name_length, path->path_name_);
    path->handle_ = CreateFileW(path->path_name_, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
    path->has_stats = 0;
    path->invalid = 0;
}

void path_load_stats(Path* path) {
    if(!path->has_stats) {
        GetFileAttributesExW(path->path_name_, GetFileExInfoStandard, &path->data_);
        path->has_stats = 1;
    }
}

void path_copy(Path* orig, Path* copy) {
    *copy = (Path){ 0 };
    copy->handle_ = CreateFileW(orig->path_name_, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
    int path_length = lstrlenW(orig->path_name_) + 1;
    copy->path_name_ = LocalAlloc(LMEM_FIXED, path_length * sizeof(wchar_t));
    lstrcpyW(copy->path_name_, orig->path_name_);
    if(orig->has_stats) path_load_stats(copy);
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
    if(!path->invalid) {
        CloseHandle(path->handle_);
        LocalFree(path->path_name_);
        path->invalid = 1;
    }
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
    stpncpy(name_c, name.data, name.length);
    name_c[name.length] = '\0';
    path_navigate_c(path, name_c);
}
*/

int path_is_directory(Path* path) {
    path_load_stats(path);
    return path->data_.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;
}

ulong path_get_file_size(Path* path) {
    path_load_stats(path);
    LARGE_INTEGER file_size_win;
    file_size_win.LowPart  = path->data_.nFileSizeLow;
    file_size_win.HighPart = path->data_.nFileSizeHigh;
    return file_size_win.QuadPart;
}

String path_get_name(Path* path, Arena* arena) {
    int path_name_length = WideCharToMultiByte(
        CP_UTF8, WC_NO_BEST_FIT_CHARS,
        path->path_name_, -1, NULL, 0, NULL, NULL);
    String result;
    result.data   = arena_alloc_array(arena, path_name_length, char);
    result.length = path_name_length;
    WideCharToMultiByte(CP_UTF8, WC_NO_BEST_FIT_CHARS,
                        path->path_name_, -1, result.data,
                        path_name_length, NULL, NULL);
    return result;
}

String path_get_base_name(Path* path, Arena* arena) {
    DWORD path_name_size = GetFullPathName(path->path_name_, 0, NULL, NULL);
    wchar_t* base_name_win;
    wchar_t* path_name_win = arena_alloc_array(arena, path_name_size, wchar_t);
    GetFullPathName(path->path_name_, path_name_size, path_name_win, &base_name_win);
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
                    GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
    
    BOOL error =  FindNextFile(dir_iter->handle_, &dir_iter->find_file_data_);
    if(!error) {
        if(GetLastError() == ERROR_NO_MORE_FILES) {
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
    PWSTR search_name = LocalAlloc(LMEM_FIXED, path_length * sizeof(wchar_t));
    lstrcpyW(search_name, path->path_name_);
    lstrcatW(search_name, L"\\*");
    result.handle_ = FindFirstFile(search_name, &result.find_file_data_);
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



typedef struct {
    Buffer  buffer;
    char*   pos;
    Arena*  arena;
    int     had_new_line;
} BMLParserState;


void bml_parser_advance(BMLParserState* parser) {
    parser->pos++;
}

int bml_is_valid_char(char p)  {  //A-Z, a-z, 0-9, -.
    return p - 'A' < 26u || p - 'a' < 26u || p - '0' < 10u || p - '-' < 2u;
}


void bml_parse_name(BMLNode* node, BMLParserState* parser) {
    char* begin = parser->pos;
    while(bml_is_valid_char(parser->pos[0])) bml_parser_advance(parser);
    if(parser->pos == begin) {
        //error_at_pos(parser->text_pos);
        fprintf(stderr, "Invalid node name.\n");
        exit(1);
    }
    node->name = (String) {
        .data = begin,
        .length = parser->pos - begin
    };
}

void bml_parse_data(BMLNode* node, BMLParserState* parser) {
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
            case '\r':
                if(parser->pos[1] == '\n') {
                    //error_at_pos(parser->text_pos);
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

void bml_parse_attributes(BMLNode* node, BMLParserState* parser) {
    BMLNode** next_node_ptr = &node->child;
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

        BMLNode* new_node = (BMLNode*)arena_alloc_type(parser->arena, BMLNode);
        *new_node = (BMLNode){ };
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

int bml_parse_indent(BMLParserState* parser) {
    int result = 0;
    while(parser->pos[0] == ' ' || parser->pos[0] == '\t') {
        result++;
        bml_parser_advance(parser);
    }
    return result;
}

BMLNode* bml_parse_node(BMLParserState* parser, int* indent) {
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
    *node = (BMLNode) { };
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

        BMLNode* new_node = arena_alloc_type(parser->arena, BMLNode);
        *new_node = (BMLNode){ };
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
            BMLNode* new_node = bml_parse_node(parser, &new_indent);
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

BMLNode* bml_parse(Buffer buffer, Arena* arena) {
    BMLParserState parser = { };
    parser.buffer = buffer;
    parser.pos    = (char*)buffer.begin;
    //parser.text_pos.line_number = 1;
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
        printf("%*s%.*s: %.*s\n", indent, "", node->name.length, node->name.data,
               node->value.length, node->value.data);
        bml_print_node(node->child, indent + 2);
        bml_print_node(node->next, indent);
    }
}



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
