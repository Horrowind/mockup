#ifndef POOL_H
#define POOL_H

#include "stdlib.h"
#include "stdint.h"

typedef struct {
    uint8_t* data;
    int length;
    int fill;
} pool_t;


void pool_init(pool_t* pool);
uint8_t* pool_alloc(pool_t* pool, int bytes);
void pool_empty(pool_t* pool);
void pool_deinit(pool_t* pool);

#endif //POOL_H
