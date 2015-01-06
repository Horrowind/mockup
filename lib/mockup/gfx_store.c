#include "stdlib.h"

#include "decode.h"
#include "gfx_store.h"


//TODO: Remove magic numbers.
void gfx_store_init(gfx_store_t* gfx_store, r65816_rom_t* rom) {
    gfx_store->pages = malloc(0x33 * sizeof(gfx_page_t));
    gfx_store->num_pages = 0x33;
    gfx_store->pages[32]->length = 12288;
    gfx_store->pages[32]->data = malloc(12288);
    gfx_store->pages[33]->length = 12288;
    gfx_store->pages[33]->data = malloc(12288);
    
    int addr_gfx32_pc = 0x40000;
    decode_lz2(rom->data + addr_gfx32_pc, gfx_store->pages[32]->data);
    int addr_gfx33_pc = 0x43FC0;
    decode_lz2(rom->data + addr_gfx33_pc, gfx_store->pages[33]->data);

    for(int i = 0; i < 0x31; i++) {
        uint8_t bank = rom->banks[0][0xB9F6];
        uint8_t addr = (rom->banks[0][0xB9C4] << 8) | rom->banks[0][0xB992];
        gfx_store->pages[i]->length = decode_lz2_unknown_size(rom->banks[bank] + addr, gfx_store->pages[i]->data);
    }
}

void gfx_store_deinit(gfx_store_t* gfx_store) {
    for(int i = 0; i < gfx_store->num_pages; i++) {
        free(gfx_store->pages[i]->data);
    }
    free(gfx_store->pages);
}
