#ifndef R65816_MAPPER_H
#define R65816_MAPPER_H

#include "base/base.h"

typedef u8 (*WDC65816ReadFunction)(u32 addr);
typedef void (*WDC65816WriteFunction)(u32 addr, u8 data);
typedef u8* (*WDC65816PtrFunction)(u32 addr);

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
} WDC65816MapperEntry;

typedef struct {
    u8 num_entries;
    WDC65816MapperEntry   entries[256];
    WDC65816ReadFunction  read;
    WDC65816WriteFunction write;
    WDC65816PtrFunction   ptr;
} WDC65816Mapper;

void wdc65816_mapper_add(WDC65816Mapper* mapper, WDC65816MapperEntry* entry);
void wdc65816_mapper_init_functions(WDC65816Mapper* mapper);
u8* wdc65816_mapper_read_range(WDC65816Mapper* mapper, u32 addr_low, u32 addr_high, u8* data);
u8* wdc65816_mapper_write_range(WDC65816Mapper* mapper, u32 addr_low, u32 addr_high, u8* data);
#endif //R65816_MAPPER_H
