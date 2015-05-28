#include "stdlib.h"
#include "stdio.h"

#include "decode.h"
#include "gfx_store.h"


//TODO: Remove magic numbers.
void gfx_store_init(gfx_store_t* gfx_store, r65816_rom_t* rom) {

    int sum_length = 0;
    gfx_store->pages = malloc(0x34 * sizeof(gfx_page_t));
    gfx_store->num_pages = 0x34;

    for(int i = 0; i < 0x32; i++) {
        uint8_t bank = rom->banks[0][0xB9F6 + i];
        uint16_t addr = (rom->banks[0][0xB9C4 + i] << 8) | rom->banks[0][0xB992 + i];
        gfx_store->pages[i].length = decode_lz2_get_size(rom->banks[bank] + addr);
        sum_length += gfx_store->pages[i].length;
    }

    gfx_store->pages[0].data = malloc(sum_length + 23808 + 12288);
    sum_length = 0;
    
    for(int i = 0; i < 0x32; i++) {
        uint8_t bank = rom->banks[0][0xB9F6 + i];
        uint16_t addr = (rom->banks[0][0xB9C4 + i] << 8) | rom->banks[0][0xB992 + i];
        gfx_store->pages[i].data = gfx_store->pages[0].data + sum_length;
        sum_length += gfx_store->pages[i].length;
        decode_lz2(rom->banks[bank] + addr, gfx_store->pages[i].data);
    }
   
    gfx_store->pages[0x32].length = 23808;
    gfx_store->pages[0x32].data = gfx_store->pages[0].data + sum_length;
    gfx_store->pages[0x33].length = 12288;
    gfx_store->pages[0x33].data = gfx_store->pages[0].data + sum_length + 23808;

    
    const int addr_gfx32_pc = 0x40000;
    decode_lz2(rom->data + addr_gfx32_pc, gfx_store->pages[0x32].data);
    const int addr_gfx33_pc = 0x43FC0;
    decode_lz2(rom->data + addr_gfx33_pc, gfx_store->pages[0x33].data);
}

void gfx_store_deinit(gfx_store_t* gfx_store) {
    free(gfx_store->pages[0].data);
    free(gfx_store->pages);
}
