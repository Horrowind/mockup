#ifndef BASE_POOL_H
#define BASE_POOL_H

#include "assert.h"
#include "stdlib.h"
#include "datasizes.h"

typedef struct {
    u8* data;
    uint length;
    uint fill;
} pool_t;


void pool_init(pool_t* pool);
u8* pool_alloc(pool_t* pool, uint bytes);
void pool_empty(pool_t* pool);
void pool_deinit(pool_t* pool);


#endif //BASE_POOL_H
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
