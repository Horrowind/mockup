#define __STDC_LIMIT_MACROS
#define __STDC_CONSTANT_MACROS

#include "mapper.h"

static
u32 wdc65816_mapper_reduce(u32 addr, u32 mask) {
    while(mask) {
        u32 bits = (mask & -mask) - 1;
        addr = ((addr >> 1) & ~bits) | (addr & bits);
        mask = (mask & (mask - 1)) >> 1;
    }
    return addr;
}

static
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

void wdc65816_mapper_builder_add(Wdc65816MapperBuilder* mapper, Wdc65816MapperEntry* entry) {
    if(mapper->entries_length >= 255) return;
    mapper->entries[mapper->entries_length] = *entry;
    mapper->entries_length++;
}


u8* wdc65816_mapper_read_range(Wdc65816Mapper* mapper, u32 addr_low, u32 addr_high, u8* data) {
    for(u32 full_addr = addr_low; full_addr < addr_high; full_addr++) {
        u8  bank = full_addr >> 16;
        u16 addr = full_addr & 0xFFFF;
        for(uint i = 0; i < mapper->entries_length; i++) {
            Wdc65816MapperEntry* entry = &mapper->entries[i];
            if(entry->map.bank_low <= bank && bank <= entry->map.bank_high
               && entry->map.addr_low <= addr && addr <= entry->map.addr_high) {
                u32 offset = wdc65816_mapper_reduce(full_addr, entry->map.mask);
                if(entry->map.size) offset = entry->map.base + wdc65816_mapper_mirror(offset, entry->map.size - entry->map.base);
                data[full_addr - addr_low] = ((u8*)entry->data.begin)[offset];
            }
        }
    }
    return 0;
}

u8* wdc65816_mapper_write_range(Wdc65816Mapper* mapper, u32 addr_low, u32 addr_high, u8* data) {
    for(u32 full_addr = addr_low; full_addr < addr_high; full_addr++) {
        u8  bank = full_addr >> 16;
        u16 addr = full_addr & 0xFFFF;
        for(uint i = 0; i < mapper->entries_length; i++) {
            Wdc65816MapperEntry* entry = &mapper->entries[i];
            if(entry->map.bank_low <= bank && bank <= entry->map.bank_high
               && entry->map.addr_low <= addr && addr <= entry->map.addr_high) {
                u32 offset = wdc65816_mapper_reduce(full_addr, entry->map.mask);
                if(entry->map.size) offset = entry->map.base + wdc65816_mapper_mirror(offset, entry->map.size - entry->map.base);
                ((u8*)entry->data.begin)[offset] = data[full_addr - addr_low];
            }
        }
    }
    return 0;
}

void wdc65816_mapper_builder_init(Wdc65816MapperBuilder* mapper_builder) {
    *mapper_builder = (Wdc65816MapperBuilder){
        .entries_length = 0
    };
}

void wdc65816_mapper_builder_deinit(Wdc65816MapperBuilder* mapper_builder) {
    unused(mapper_builder);
}


void wdc65816_mapper_init(Wdc65816Mapper* mapper, Wdc65816MapperBuilder* builder, u8** buffer) {
    *mapper = (Wdc65816Mapper) {
        .entries_length = builder->entries_length,
        .data = buffer
    };
    
    for(uint i = 0; i < mapper->entries_length; i++) {
        Wdc65816MapperEntry* entry = builder->entries + i;
        mapper->entries[i] = builder->entries[i];
        for(u32 bank = entry->map.bank_low; bank <= entry->map.bank_high; bank++) {
            for(u32 addr = entry->map.addr_low; addr <= entry->map.addr_high; addr++) {
                u32 full_addr = (bank << 16) | addr;
                u32 offset = wdc65816_mapper_reduce(full_addr, entry->map.mask);
                if(entry->map.size) offset = entry->map.base + wdc65816_mapper_mirror(offset, entry->map.size - entry->map.base);
                mapper->data[full_addr] = entry->data.begin + offset;
            }
        }
    }
}

ulong wdc65816_mapper_get_buffer_size() {
    return sizeof(u8*) << 24;
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

