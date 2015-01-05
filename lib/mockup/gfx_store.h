#ifndef MOCKUP_GFX_STORE_H
#define MOCKUP_GFX_STORE_H

#include <stdint.h>

typedef struct {
    int length;
    uint8_t data[];
} gfx_page_t;

typedef struct {
    int num_pages;
    gfx_page_t* pages[];
} gfx_store_t;

void gfx_store_load();
void gfx_store_free();


#endif //MOCKUP_GFX_STORE_H
