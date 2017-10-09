#ifndef R65816_MAPPER_H
#define R65816_MAPPER_H

#include "base/base.h"

typedef u8 (*r65816_read_function_t)(u32 addr);
typedef void (*r65816_write_function_t)(u32 addr, u8 data);
typedef u8* (*r65816_ptr_function_t)(u32 addr);

typedef struct {
    r65816_read_function_t  read;
    r65816_write_function_t write;
    r65816_ptr_function_t   ptr;
} r65816_mapper_function_triple_t;

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
} r65816_mapper_entry_t;

typedef struct {
    u8 num_entries;
    r65816_mapper_entry_t entries[256];
} r65816_mapper_t;

u32 r65816_mapper_reduce(u32 addr, u32 mask);
u32 r65816_mapper_mirror(uint addr, uint size);
void r65816_mapper_add(r65816_mapper_t* mapper, r65816_mapper_entry_t* entry);
u8   r65816_mapper_read(r65816_mapper_t* mapper, u32 full_addr);
void r65816_mapper_write(r65816_mapper_t* mapper, u32 full_addr, u8 data);
u8*  r65816_mapper_ptr(r65816_mapper_t* mapper, u32 full_addr);

r65816_mapper_function_triple_t r65816_mapper_create_function_triple(r65816_mapper_t* mapper);
#endif //R65816_MAPPER_H
