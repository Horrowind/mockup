#ifndef R65816_MAPPER_H
#define R65816_MAPPER_H

#include "base/base.h"

typedef u8 (*wdc65816_read_function_t)(u32 addr);
typedef void (*wdc65816_write_function_t)(u32 addr, u8 data);
typedef u8* (*wdc65816_ptr_function_t)(u32 addr);

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
    string_t name;
} wdc65816_mapper_entry_t;

typedef struct {
    u8 num_entries;
    wdc65816_mapper_entry_t entries[256];
    wdc65816_read_function_t  read;
    wdc65816_write_function_t write;
    wdc65816_ptr_function_t   ptr;
} wdc65816_mapper_t;

void wdc65816_mapper_add(wdc65816_mapper_t* mapper, wdc65816_mapper_entry_t* entry);
void wdc65816_mapper_init_functions(wdc65816_mapper_t* mapper);
u8* wdc65816_mapper_read_range(wdc65816_mapper_t* mapper, u32 addr_low, u32 addr_high, u8* data);
u8* wdc65816_mapper_write_range(wdc65816_mapper_t* mapper, u32 addr_low, u32 addr_high, u8* data);
#endif //R65816_MAPPER_H
