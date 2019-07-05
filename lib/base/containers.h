#define define_stack(name, prefix, type)                                        \
    typedef struct {                                                            \
        Arena arena;                                                            \
        FreeList* free_list;                                                    \
    } name;                                                                     \
    extern void prefix##_init(name* s, FreeList* free_list, int size);          \
    extern void prefix##_deinit(name* s, FreeList* sentinel);                   \
    extern type* prefix##_push(name* s, type data);                             \
    extern type* prefix##_pop(name* s);                                         \
    extern type* prefix##_top(name* s);                                         \
    extern b32 prefix##_is_empty(name* s);

#define implement_stack(name, prefix, type)                                     \
    inline void prefix##_init(name* s, FreeList* free_list, int size) {         \
        Buffer buffer = free_list_alloc_buffer(free_list, size * sizeof(type)); \
        s->arena = arena_create_buffer(buffer);                                 \
        s->free_list = free_list;                                               \
    }                                                                           \
                                                                                \
    inline void prefix##_deinit(name* s, FreeList* sentinel) {                  \
        free_list_dealloc_buffer(sentinel, s->arena.buffer);                    \
    }                                                                           \
                                                                                \
    inline type* prefix##_push(name* s, type data) {                            \
        type* new_data = arena_alloc_type(&s->arena, type);                     \
        if(new_data) {                                                          \
            *new_data = data;                                                   \
             return new_data;                                                   \
        } else {                                                                \
            ulong old_size = buffer_length(s->arena.buffer);                    \
            ulong new_size = 2 * old_size;                                      \
            Buffer new_buffer = free_list_alloc_buffer(s->free_list, new_size); \
            copy_buffer(s->arena.buffer, new_buffer);                           \
            Arena new_arena = arena_create_buffer(new_buffer);                  \
            uptr old_fill = ptr_diff(s->arena.current, s->arena.buffer.begin);  \
            new_arena.current = ptr_add(new_arena.buffer.begin, old_fill);      \
            type* new_data = arena_alloc_type(&s->arena, type);                 \
            assert(new_data);                                                   \
            *new_data = data;                                                   \
            return new_data;                                                    \
        }                                                                       \
    }                                                                           \
                                                                                \
    inline type* prefix##_top(name* s) {                                        \
        return arena_array_last(&s->arena, type);                               \
    }                                                                           \
                                                                                \
    inline type* prefix##_pop(name* s) {                                        \
        return arena_dealloc_type(&s->arena, type);                             \
    }                                                                           \
                                                                                \
    inline void prefix##_clear(name* s) {                                       \
        arena_clear(&s->arena);                                                 \
    }                                                                           \
                                                                                \
    inline b32 prefix##_is_empty(name* s) {                                     \
        return s->arena.buffer.begin == s->arena.current;                       \
    }


#define generate_stack(name, prefix, type)                                      \
    define_stack(name, prefix, type);                                           \
    implement_stack(name, prefix, type);
        


#define get16bits(d) (*((const u16*) (d)))
#define get32bits(d) (*((const u32*) (d)))
#define MAX_FILL_NOMINATOR 9
#define MAX_FILL_DENOMINATOR 10

#define Hashmap(type)                                                           \
    struct {                                                                    \
        struct {                                                                \
            type data;                                                          \
            u32  hash : 31;                                                     \
            u32  used :  1;                                                     \
        }* entries;                                                             \
        uint size;                                                              \
        uint fill;                                                              \
        FreeList* free_list;                                                    \
    }

#define define_hashmap(name, prefix, type)                                      \
    typedef Hashmap(type) name;                                                 \
    void prefix##_init(name* map, FreeList* free_list, int size);               \
    void prefix##_deinit(name* map);                                            \
    void prefix##_resize(name* map);                                            \
    type* prefix##_find(name* map, type data);                                  \
    type* prefix##_find_hash(name* map, type data, u32 hash);                   \
    void prefix##_insert(name* map, type data);                                 \
    void prefix##_insert2(name* map, type data, u32 hash);


#define implement_hashmap(name, prefix, type, fun, cmp)                         \
    void prefix##_init(name* map, FreeList* free_list, int size) {              \
        map->size = size;                                                       \
        map->fill = 0;                                                          \
        map->entries = free_list_alloc(free_list, size * sizeof(map->entries[0])); \
        zero_buffer(buffer(map->entries, size * sizeof(map->entries[0])));      \
        map->free_list = free_list;                                             \
    }                                                                           \
                                                                                \
    void prefix##_deinit(name* map) {                                           \
        free_list_dealloc(map->free_list, map->entries, map->size * sizeof(map->entries[0])); \
    }                                                                           \
                                                                                \
    void prefix##_insert2(name* map, type data, u32 hash);                      \
    void prefix##_resize(name* map) {                                           \
        name new_map;                                                           \
        prefix##_init(&new_map, map->free_list, 2 * map->size);                 \
        for(uint i = 0; i < map->size; i++) {                                   \
            type data = map->entries[i].data;                                   \
            u32 hash = map->entries[i].hash;                                    \
            u32 used = map->entries[i].used;                                    \
            if(used) {                                                          \
                prefix##_insert2(&new_map, data, hash);                         \
            }                                                                   \
        }                                                                       \
        free_list_dealloc(map->free_list, map->entries, map->size * sizeof(map->entries[0])); \
        *map = new_map;                                                         \
    }                                                                           \
                                                                                \
    inline                                                                      \
    void prefix##_insert2(name* map, type data, u32 hash) {                     \
        u32 mask = (map->size - 1);                                             \
        u32 pos = hash & mask;                                                  \
        u32 dist = 0;                                                           \
        for(;;) {                                                               \
            if(!map->entries[pos].used) {                                       \
                map->entries[pos].data = data;                                  \
                map->entries[pos].hash = hash;                                  \
                map->entries[pos].used = 1;                                     \
                break;                                                          \
            } else {                                                            \
                u32 probed_hash = map->entries[pos].hash;                       \
                u32 probed_dist = (probed_hash - pos) & mask;                   \
                if(probed_dist < dist) {                                        \
                    type tmp_data = data;                                       \
                    data = map->entries[pos].data;                              \
                    map->entries[pos].data = tmp_data;                          \
                                                                                \
                    u32 tmp_hash = hash;                                        \
                    hash = probed_hash;                                         \
                    map->entries[pos].hash = tmp_hash;                          \
                    map->entries[pos].used = 1;                                 \
                }                                                               \
            }                                                                   \
            pos = (pos + 1) & mask;                                             \
            dist++;                                                             \
        }                                                                       \
        map->fill++;                                                            \
        if(map->fill * MAX_FILL_DENOMINATOR > map->size * MAX_FILL_NOMINATOR)   \
            prefix##_resize(map);                                               \
    }                                                                           \
                                                                                \
                                                                                \
    void prefix##_insert(name* map, type data) {                                \
        u32 hash = fun(data);                                                   \
        prefix##_insert2(map, data, hash);                                      \
    }                                                                           \
                                                                                \
    type* prefix##_find(name* map, type data) {                                 \
        u32 hash = fun(data);                                                   \
        return prefix##_find_hash(map, data, hash);                             \
    }                                                                           \
                                                                                \
    type* prefix##_find_hash(name* map, type data, u32 hash) {                  \
        u32 mask = (map->size - 1);                                             \
        u32 dist = 0;                                                           \
        u32 pos = hash & mask;                                                  \
        for(;;) {                                                               \
            if(!map->entries[pos].used) {                                       \
                return NULL;                                                    \
            } else  if(cmp(map->entries[pos].data, data)) {                     \
                return &map->entries[pos].data;                                 \
            } else {                                                            \
                u32 probed_hash = map->entries[pos].hash;                       \
                u32 probed_dist = (probed_hash - pos) & mask;                   \
                if(probed_dist < dist) {                                        \
                    return NULL;                                                \
                }                                                               \
                pos = (pos + 1) & mask;                                         \
                dist++;                                                         \
            }                                                                   \
        }                                                                       \
    }

#define generate_hashmap(Name, name, type, fun, cmp)                            \
    define_hashmap(Name, name, type, fun, cmp)                                  \
    implement_hashmap(Name, name, type, fun, cmp)

define_hashmap(StringMap, string_map, String);

#define InternHashmap(type)                                                     \
    struct {                                                                    \
        FreeList* free_list;                                                    \
        Arena arena;                                                            \
        struct {                                                                \
            type* data;                                                         \
            u32  hash : 31;                                                     \
            u32  used :  1;                                                     \
        }* entries;                                                             \
        uint size;                                                              \
        uint fill;                                                              \
    }

#define define_intern_hashmap(name, prefix, type)                               \
    typedef InternHashmap(type) name;                                           \
    void  prefix##_init(name* map, FreeList* free_list, int size, ulong intern_size); \
    void  prefix##_deinit(name* map);                                           \
    void  prefix##_resize(name* map);                                           \
    type* prefix##_find(name* map, type data);                                  \
    type* prefix##_find_hash(name* map, type data, u32 hash);                   \
    void  prefix##_insert(name* map, type data);                                \
    type* prefix##_find_or_insert(name* map, type data);                        \
    void  prefix##_insert2(name* map, type* data, u32 hash);


#define implement_intern_hashmap(name, prefix, type, fun, cmp, cpy)             \
    void prefix##_init(name* map, FreeList* free_list, int size, ulong intern_size) { \
        Buffer intern_buffer = buffer(free_list_alloc(free_list, intern_size), intern_size); \
        map->arena = arena_create_buffer(intern_buffer);                        \
        map->size = size;                                                       \
        map->fill = 0;                                                          \
        map->entries = free_list_alloc(free_list, size * sizeof(map->entries[0])); \
        map->free_list = free_list;                                             \
    }                                                                           \
                                                                                \
    void prefix##_deinit(name* map) {                                           \
        free_list_dealloc(map->free_list, map->entries, map->size * sizeof(map->entries[0])); \
    }                                                                           \
                                                                                \
    void prefix##_insert2(name* map, type* data, u32 hash);                     \
    void prefix##_resize(name* map) {                                           \
        name new_map;                                                           \
        new_map.size = 2 * map->size;                                           \
        new_map.fill = 0;                                                       \
        new_map.entries = free_list_alloc(map->free_list, new_map.size * sizeof(map->entries[0])); \
        new_map.arena = map->arena;                                             \
        new_map.free_list = map->free_list;                                     \
        for(uint i = 0; i < map->size; i++) {                                   \
            type* data = map->entries[i].data;                                  \
            u32 hash = map->entries[i].hash;                                    \
            u32 used = map->entries[i].used;                                    \
            if(used) {                                                          \
                prefix##_insert2(&new_map, data, hash);                         \
            }                                                                   \
        }                                                                       \
        free_list_dealloc(map->free_list, map->entries, map->size * sizeof(map->entries[0])); \
        *map = new_map;                                                         \
    }                                                                           \
                                                                                \
    inline                                                                      \
    void prefix##_insert2(name* map, type* data, u32 hash) {                    \
        u32 mask = (map->size - 1);                                             \
        u32 pos = hash & mask;                                                  \
        u32 dist = 0;                                                           \
        for(;;) {                                                               \
            if(!map->entries[pos].used) {                                       \
                map->entries[pos].data = data;                                  \
                map->entries[pos].hash = hash;                                  \
                map->entries[pos].used = 1;                                     \
                break;                                                          \
            } else {                                                            \
                u32 probed_hash = map->entries[pos].hash;                       \
                u32 probed_dist = (probed_hash - pos) & mask;                   \
                if(probed_dist < dist) {                                        \
                    type* tmp_data = data;                                      \
                    data = map->entries[pos].data;                              \
                    map->entries[pos].data = tmp_data;                          \
                                                                                \
                    u32 tmp_hash = hash;                                        \
                    hash = probed_hash;                                         \
                    map->entries[pos].hash = tmp_hash;                          \
                    map->entries[pos].used = 1;                                 \
                }                                                               \
            }                                                                   \
            pos = (pos + 1) & mask;                                             \
            dist++;                                                             \
        }                                                                       \
        map->fill++;                                                            \
        if(map->fill * MAX_FILL_DENOMINATOR > map->size * MAX_FILL_NOMINATOR)   \
            prefix##_resize(map);                                               \
    }                                                                           \
                                                                                \
    type* prefix##_find(name* map, type data) {                                 \
        u32 hash = fun(data);                                                   \
        return prefix##_find_hash(map, data, hash);                             \
    }                                                                           \
                                                                                \
    type* prefix##_find_hash(name* map, type data, u32 hash) {                  \
        u32 mask = (map->size - 1);                                             \
        u32 dist = 0;                                                           \
        u32 pos = hash & mask;                                                  \
        for(;;) {                                                               \
            if(!map->entries[pos].used) {                                       \
                return NULL;                                                    \
            } else  if(cmp(*map->entries[pos].data, data)) {                    \
                return map->entries[pos].data;                                  \
            } else {                                                            \
                u32 probed_hash = map->entries[pos].hash;                       \
                u32 probed_dist = (probed_hash - pos) & mask;                   \
                if(probed_dist < dist) {                                        \
                    return NULL;                                                \
                }                                                               \
                pos = (pos + 1) & mask;                                         \
                dist++;                                                         \
            }                                                                   \
        }                                                                       \
    }                                                                           \
                                                                                \
    type* prefix##_find_or_insert(name* map, type data) {                       \
        u32 hash = fun(data);                                                   \
        type* found = prefix##_find_hash(map, data, hash);                      \
        if(found) return found;                                                 \
        type* new_data = cpy(&map->arena, data);                                \
        prefix##_insert2(map, new_data, hash);                                  \
        return new_data;                                                        \
    }                                                                           \
                                                                                \
    void prefix##_insert(name* map, type data) {                                \
        u32 hash = fun(data);                                                   \
        type* new_data = cpy(&map->arena, data);                                \
        prefix##_insert2(map, new_data, hash);                                  \
    }

#define generate_intern_hashmap(name, prefix, type, hash, cmp, cpy)             \
    define_intern_hashmap(name, prefix, type)                                   \
    implement_intern_hashmap(name, prefix, type, hash, cmp, cpy)
