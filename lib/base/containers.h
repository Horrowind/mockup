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
        if(!base_global.is_initialized) base_global_init();             \
        ulong num_pages = (size * sizeof(type) + base_global.page_size - 1) / \
            base_global.page_size;                                      \
        s->data = free_list_alloc(sentinel, num_pages);                 \
        s->fill = 0;                                                    \
        s->capacity = size;                                             \
    }                                                                   \
                                                                        \
    inline void prefix##_deinit(name* s, FreeList* sentinel) {          \
        ulong num_pages = (s->capacity * sizeof(type) +                 \
                           base_global.page_size - 1) /                 \
                           base_global.page_size;                       \
        free_list_add(sentinel, s->data, num_pages);                    \
    }                                                                   \
                                                                        \
    inline type* prefix##_push(name* s, type data) {                    \
        if(s->fill == s->capacity) {                                    \
            ulong old_num_pages = (s->capacity * sizeof(type) +         \
                                   base_global.page_size - 1) /         \
                                   base_global.page_size;               \
            s->capacity *= 2;                                           \
            ulong new_num_pages = (s->capacity * sizeof(type) +         \
                                   base_global.page_size - 1) /         \
                                   base_global.page_size;               \
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

define_hashmap(StringMap, string_map, String, hash_murmur3_string, string_equal);

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

#define generate_intern_hashmap(name, prefix, type, fun, cmp, cpy)      \
    define_intern_hashmap(name, prefix, type)                           \
    implement_intern_hashmap(name, prefix, type, fun, cmp, cpy)
