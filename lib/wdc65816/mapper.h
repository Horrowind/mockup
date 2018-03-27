#ifndef WDC65816_MAPPER_H
#define WDC65816_MAPPER_H

#include "base/base.h"

typedef struct {
    union {
        struct {
            u16 addr_low;
            u8  bank_low;
            u8  unused_low;
        };
        u32 low;
    };
    union {
        struct {
            u16 addr_high;
            u8  bank_high;
            u8  unused_high;
        };
        u32 high;
    };
    u32 base;
    u32 mask;
    u32 size;
    u8* data;
    int data_length;
    String name;
} Wdc65816MapperEntry;

typedef struct {
    u8                  num_entries;
    Wdc65816MapperEntry entries[256];
    u8**                data;
} Wdc65816Mapper;

void wdc65816_mapper_init(Wdc65816Mapper* mapper);
void wdc65816_mapper_deinit(Wdc65816Mapper* mapper);
void wdc65816_mapper_update(Wdc65816Mapper* mapper);
u8   wdc65816_mapper_read(Wdc65816Mapper* mapper, u32 addr);
void wdc65816_mapper_write(Wdc65816Mapper* mapper, u32 addr, u8 data);
u8*  wdc65816_mapper_ptr(Wdc65816Mapper* mapper, u32 addr);
void wdc65816_mapper_add(Wdc65816Mapper* mapper, Wdc65816MapperEntry* entry);
u8*  wdc65816_mapper_read_range(Wdc65816Mapper* mapper, u32 addr_low, u32 addr_high, u8* data);
u8*  wdc65816_mapper_write_range(Wdc65816Mapper* mapper, u32 addr_low, u32 addr_high, u8* data);
#endif //WDC65816_MAPPER_H
