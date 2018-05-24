#include "decode.h"
#include "gfx_store.h"


//TODO: Remove magic numbers.
void gfx_store_init(GFXStore* gfx_store, Wdc65816Rom* rom, Arena* arena) {

    int sum_length = 0;
    gfx_store->pages = arena_alloc_array(arena, 0x34, GFXPage);
    gfx_store->num_pages = 0x34;

    for(int i = 0; i < 0x32; i++) {
        //u8 bank = rom->banks[0][0xB9F6 + i];
        u8 bank = wdc65816_mapper_read(&rom->read_mapper, 0x00B9F6 + i);
        //u16 addr = (rom->banks[0][0xB9C4 + i] << 8) | rom->banks[0][0xB992 + i];
        u16 addr = (wdc65816_mapper_read(&rom->read_mapper, 0x00B9C4 + i) << 8) |
            wdc65816_mapper_read(&rom->read_mapper, 0x00B992 + i);
        //gfx_store->pages[i].length = decode_lz2_get_size(rom->banks[bank] + addr);
        gfx_store->pages[i].length =
            decode_lz2_get_size(wdc65816_mapper_ptr(&rom->read_mapper, (bank << 16)  + addr));
        sum_length += gfx_store->pages[i].length;
    }

    gfx_store->pages[0].data = arena_alloc_array(arena, sum_length + 23808 + 12288, u8);
    sum_length = 0;
    
    for(int i = 0; i < 0x32; i++) {
        u8 bank = wdc65816_mapper_read(&rom->read_mapper, 0x00B9F6 + i);
        u16 addr = (wdc65816_mapper_read(&rom->read_mapper, 0x00B9C4 + i) << 8)
            | wdc65816_mapper_read(&rom->read_mapper, 0x00B992 + i);
        gfx_store->pages[i].data = gfx_store->pages[0].data + sum_length;
        sum_length += gfx_store->pages[i].length;
        decode_lz2(wdc65816_mapper_ptr(&rom->read_mapper, bank * 0x10000 + addr),
                   gfx_store->pages[i].data);
    }
   
    gfx_store->pages[0x32].length = 23808;
    gfx_store->pages[0x32].data = gfx_store->pages[0].data + sum_length;
    gfx_store->pages[0x33].length = 12288;
    gfx_store->pages[0x33].data = gfx_store->pages[0].data + sum_length + 23808;

    
    const int addr_gfx32_sfc = 0x088000;
    decode_lz2(wdc65816_mapper_ptr(&rom->read_mapper, addr_gfx32_sfc), gfx_store->pages[0x32].data);
    const int addr_gfx33_sfc = 0x08BFC0;
    decode_lz2(wdc65816_mapper_ptr(&rom->read_mapper, addr_gfx33_sfc), gfx_store->pages[0x33].data);
}

void gfx_store_deinit(GFXStore* gfx_store) {

}
