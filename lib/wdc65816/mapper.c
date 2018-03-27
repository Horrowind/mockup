#define __STDC_LIMIT_MACROS
#define __STDC_CONSTANT_MACROS

#include "mapper.h"

u32 wdc65816_mapper_reduce(u32 addr, u32 mask) {
    while(mask) {
        u32 bits = (mask & -mask) - 1;
        addr = ((addr >> 1) & ~bits) | (addr & bits);
        mask = (mask & (mask - 1)) >> 1;
    }
    return addr;
}

u32 wdc65816_mapper_mirror(uint addr, uint size) {
    if(size == 0) return 0;
    u32 base = 0;
    u32 mask = 1 << 23;
    while(addr >= size) {
        while(!(addr & mask)) mask >>= 1;
        addr -= mask;
        if(size > mask) {
            size -= mask;
            base += mask;
        }
        mask >>= 1;
    }
    return base + addr;
}

void wdc65816_mapper_add(Wdc65816Mapper* mapper, Wdc65816MapperEntry* entry) {
    if(mapper->num_entries >= 255) return;
    mapper->entries[mapper->num_entries] = *entry;
    mapper->num_entries++;
}


u8* wdc65816_mapper_read_range(Wdc65816Mapper* mapper, u32 addr_low, u32 addr_high, u8* data) {
    for(u32 full_addr = addr_low; full_addr < addr_high; full_addr++) {
        u8  bank = full_addr >> 16;
        u16 addr = full_addr & 0xFFFF;
        for(int i = 0; i < mapper->num_entries; i++) {
            Wdc65816MapperEntry* entry = &mapper->entries[i];
            if(entry->bank_low <= bank && bank <= entry->bank_high
               && entry->addr_low <= addr && addr <= entry->addr_high) {
                u32 offset = wdc65816_mapper_reduce(full_addr, entry->mask);
                if(entry->size) offset = entry->base + wdc65816_mapper_mirror(offset, entry->size - entry->base);
                data[full_addr - addr_low] = entry->data[offset];
            }
        }
    }
    return 0;
}

u8* wdc65816_mapper_write_range(Wdc65816Mapper* mapper, u32 addr_low, u32 addr_high, u8* data) {
    for(u32 full_addr = addr_low; full_addr < addr_high; full_addr++) {
        u8  bank = full_addr >> 16;
        u16 addr = full_addr & 0xFFFF;
        for(int i = 0; i < mapper->num_entries; i++) {
            Wdc65816MapperEntry* entry = &mapper->entries[i];
            if(entry->bank_low <= bank && bank <= entry->bank_high
               && entry->addr_low <= addr && addr <= entry->addr_high) {
                u32 offset = wdc65816_mapper_reduce(full_addr, entry->mask);
                if(entry->size) offset = entry->base + wdc65816_mapper_mirror(offset, entry->size - entry->base);
                entry->data[offset] = data[full_addr - addr_low];
            }
        }
    }
    return 0;
}

void wdc65816_mapper_init(Wdc65816Mapper* mapper) {
    *mapper = (Wdc65816Mapper){ 0 };
    global_init();
    mapper->data = page_alloc((sizeof(u8*) << 24) / global.page_size);
    mapper->num_entries = 0;
}

void wdc65816_mapper_deinit(Wdc65816Mapper* mapper) {
    page_free(mapper->data, (sizeof(u8*) << 24) / global.page_size);
}


void wdc65816_mapper_update(Wdc65816Mapper* mapper) {
    for(int i = 0; i < mapper->num_entries; i++) {
        Wdc65816MapperEntry* entry = &mapper->entries[i];
        for(u32 bank = entry->bank_low; bank <= entry->bank_high; bank++) {
            for(u32 addr = entry->addr_low; addr <= entry->addr_high; addr++) {
                u32 full_addr = (bank << 16) | addr;
                u32 offset = wdc65816_mapper_reduce(full_addr, entry->mask);
                if(entry->size) offset = entry->base + wdc65816_mapper_mirror(offset, entry->size - entry->base);
                mapper->data[full_addr] = entry->data + offset;
            }
        }
    }
}

u8 wdc65816_mapper_read(Wdc65816Mapper* mapper, u32 addr) {
    return *mapper->data[addr];
}

void wdc65816_mapper_write(Wdc65816Mapper* mapper, u32 addr, u8 data) {
    *mapper->data[addr] = data;
}

u8* wdc65816_mapper_ptr(Wdc65816Mapper* mapper, u32 addr) {
    return mapper->data[addr];
}

