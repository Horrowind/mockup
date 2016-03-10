#ifndef BASE_HASH_H
#define BASE_HASH_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "datasizes.h"
#include "variable_string.h"

#define get16bits(d) (*((const u16 *) (d)))

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
    void name##_resize(name##_t* map);                                  \
    type* name##_find(name##_t* map, type data);                        \
    void name##_insert(name##_t* map, type data);                       \
    void name##_insert2(name##_t* map, type data, u32 hash);


#define implement_hashmap(name, type, fun, cmp)                         \
    void name##_init(name##_t* map, int size) {                         \
        map->size = size;                                               \
        map->fill = 0;                                                  \
        uint num_bytes = size * sizeof(struct { type data; u32 hash;}); \
        map->entries = calloc(num_bytes);                               \
    }                                                                   \
                                                                        \
    void name##_insert2(name##_t* map, type data, u32 hash);            \
    void name##_resize(name##_t* map) {                                 \
        struct { type data; u32 hash;}* new_entries;                    \
        uint entry_size = sizeof(struct { type data; u32 hash;});       \
        new_entries = calloc(2 * map->size * entry_size);               \
        for(int i = 0; i < map->size; i++) {                            \
            type data = map->entries[i].data;                           \
            u32 hash = map->entries[i].hash;                            \
            if(hash != 0xFFFFFFFF)                                      \
                name##_insert2(map, data, hash);                        \
        }                                                               \
    }                                                                   \
                                                                        \
    inline                                                              \
    void name##_insert2(name##_t* map, type data, u32 hash) {           \
        u32 mask = (map->size - 1);                                     \
        u32 pos = hash & mask;                                          \
        u32 dist = 0;                                                   \
        for(;;) {                                                       \
            if(map->entries[pos].hash == 0xFFFFFFFF) {                  \
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
        if(map->fill * 10 > map->size * 9)                              \
            name##_resize(map);                                         \
    }                                                                   \
                                                                        \
                                                                        \
    void name##_insert(name##_t* map, type data) {                      \
        u32 mask = (map->size - 1);                                     \
        u32 hash = fun(data);                                           \
        name##_insert2(map, data, hash);                                \
    }                                                                   \
                                                                        \
    type* name##_find(name##_t* map, type data) {                       \
        u32 hash = fun(data);                                           \
        u32 mask = (map->size - 1);                                     \
        u32 dist = 0;                                                   \
        u32 pos = hash & mask;                                          \
        for(;;) {                                                       \
            if(map->entries[pos].hash == 0xFFFFFFFF) {                  \
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



#ifdef BASE_HASH_IMPLEMENTATION

u32 SuperFastHash (string_t str) {
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

#endif

#endif
