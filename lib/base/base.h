#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifdef BASE_IMPLEMENTATION
#define BASE_HASH_IMPLEMENTATION
#define BASE_POOL_IMPLEMENTATION
#define BASE_VARIABLE_STRING_IMPLEMENTATION
#define BASE_BML_IMPLEMENTATION
#endif

#ifndef BASE_GUARD
#define BASE_GUARD

typedef char i8;
typedef unsigned char u8;
typedef short i16;
typedef unsigned short u16;
typedef int b32;
typedef int i32;
typedef unsigned int u32;
typedef long i64;
typedef unsigned long u64;
typedef float f32;
typedef double f64;

typedef unsigned int uint;
typedef unsigned short ushort;

#define array_length(array) (sizeof(array)/sizeof(array[0]))
#undef max
#undef min
#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))

typedef struct {
    void* data;
    void* current;
    void* end;
} arena_t;

/* typedef struct { */
    
/* } temp_arena_t; */

void arena_create(arena_t* arena, ulong size);

typedef struct {
    u8* begin;
    u8* end;
} buffer_t;

typedef enum {
    STRING_PARAMETER_ZERO_TERMINATED = 1 << 0,
} string_params_t;

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
    u8* data;
    uint length;
    uint fill;
} pool_t;

void pool_init(pool_t* pool);
u8*  pool_alloc(pool_t* pool, uint bytes);
void pool_empty(pool_t* pool);
void pool_deinit(pool_t* pool);

typedef struct {
    string_t file_name;
    int      line_number;
    int      line_pos;
    char*    line_start;
} text_pos_t;

typedef struct bml_node {
    string_t name;
    string_t value;
    struct bml_node* child;
    struct bml_node* next;
} bml_node_t;

bml_node_t* bml_parse(buffer_t buffer, pool_t* pool);
void bml_print_node(bml_node_t* node, int indent);

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



#define get16bits(d) (*((const u16 *) (d)))
#define get32bits(d) (*((const u32 *) (d)))
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


#endif //BASE_GUARD



#ifdef BASE_VARIABLE_STRING_IMPLEMENTATION
#ifndef BASE_VARIABLE_STRING_IMPLEMENTATION_GUARD
#define BASE_VARIABLE_STRING_IMPLEMENTATION_GUARD

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

#endif //BASE_VARIABLE_STRING_IMPLEMENTATION_GUARD
#endif //BASE_VARIABLE_STRING_IMPLEMENTATION

#ifdef BASE_HASH_IMPLEMENTATION
#ifndef BASE_HASH_IMPLEMENTATION_GUARD
#define BASE_HASH_IMPLEMENTATION_GUARD

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

#endif //BASE_HASH_IMPLEMENTATION_GUARD
#endif //BASE_HASH_IMPLEMENTATION



#ifdef BASE_POOL_IMPLEMENTATION
#ifndef BASE_POOL_IMPLEMENTATION_GUARD
#define BASE_POOL_IMPLEMENTATION_GUARD
void pool_init(pool_t* pool) {
    pool->data   = malloc(512);
    assert(pool->data);
    pool->length = 512;
    pool->fill   = 0;
}

u8* pool_alloc(pool_t* pool, uint bytes) {
    uint oldfill = pool->fill;
    pool->fill += bytes;
    if(pool->fill > pool->length) {
        while(pool->fill > pool->length) {
            pool->length <<= 2;
        }
        pool->data = realloc(pool->data, pool->length);
        assert(pool->data);
    }
    
    return pool->data + oldfill;
}

void pool_empty(pool_t* pool) {
    pool->fill = 0;
}

void pool_deinit(pool_t* pool) {
    free(pool->data);
}

#endif //BASE_POOL_IMPLEMENTATION_GUARD
#endif //BASE_POOL_IMPLEMENTATION

#ifdef BASE_BML_IMPLEMENTATION
#ifndef BASE_BML_IMPLEMENTATION_GUARD
#define BASE_BML_IMPLEMENTATION_GUARD

#ifdef NDEBUG
void error_at_pos(text_pos_t text_pos) {
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
#define error_at_pos(text_pos) error_at_pos2(text_pos, __LINE__)

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
    pool_t*    pool;
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

        bml_node_t* new_node = (bml_node_t*)pool_alloc(parser->pool, sizeof(bml_node_t));
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
    bml_node_t* node = (bml_node_t*)pool_alloc(parser->pool, sizeof(bml_node_t));
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

        bml_node_t* new_node = (bml_node_t*)pool_alloc(parser->pool, sizeof(bml_node_t));
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



bml_node_t* bml_parse(buffer_t buffer, pool_t* pool) {
    bml_parser_state_t parser = { 0 };
    parser.buffer = buffer;
    parser.pos    = (char*)buffer.begin;
    parser.text_pos.line_number = 1;
    parser.pool = pool;
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
#endif //BASE_BML_IMPLEMENTATION_GUARD
#endif //BASE_BML_IMPLEMENTATION
