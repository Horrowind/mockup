#include "pool.h"

void pool_init(pool_t* pool) {
    pool->data   = malloc(512);
    pool->length = 512;
    pool->fill   = 0;
}

uint8_t* pool_alloc(pool_t* pool, int bytes) {
    int oldfill = pool->fill;
    pool->fill += bytes;
    if(pool->fill > pool->length) {
        while(pool->fill > pool->length) {
            pool->length <<= 2;
        }
        pool->data = realloc(pool->data, pool->length);
    }
    
    return pool->data + oldfill;
}

void pool_empty(pool_t* pool) {
    pool->fill = 0;
}

void pool_deinit(pool_t* pool) {
    free(pool->data);
}
