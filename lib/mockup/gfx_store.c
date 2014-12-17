#include "decode.h"
#include "gfx_store.h"


void gfx_store_load(rom_t* rom, gfx_store_t* gfx_store) {
    gfx_page_t* gfx_page;
    uint8_t gfx33_chr[12288];
    int addr_gfx32_pc = 0x40000;
    decrypt_LZ2(l->rom->data + addr_gfx32_pc, gfx32_chr);
    int addr_gfx33_pc = 0x43FC0;
    decrypt_LZ2(l->rom->data + addr_gfx33_pc, gfx33_chr);
  
    for(int i = 0; i < SIZE_OF_GFX_32; i++) {
        l->gfx32_33[i] = tile8_from_4bpp(gfx32_chr + 32 * i);
    }
    for(int i = 0; i < SIZE_OF_GFX_33; i++) {
        l->gfx32_33[i + SIZE_OF_GFX_32] = tile8_from_3bpp(gfx33_chr + 24 * i);
    }
}
