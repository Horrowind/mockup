#ifndef MOCKUP_GFX_STORE_H
#define MOCKUP_GFX_STORE_H

#include "wdc65816/wdc65816.h"

typedef struct {
    int length;
    u8* data;
} GFXPage;

typedef struct {
    int num_pages;
    GFXPage* pages;
} GFXStore;

void gfx_store_init(GFXStore* gfx_store, WDC65816Rom* rom);
void gfx_store_deinit(GFXStore* gfx_store);


#endif //MOCKUP_GFX_STORE_H
