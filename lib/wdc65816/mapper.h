#ifndef WDC65816_MAPPER_H
#define WDC65816_MAPPER_H

#include "base/base.h"

typedef enum {
    WDC65816_MEMORY_ROM,
    WDC65816_MEMORY_RAM,
} Wdc65816MemoryType;

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
    
    Wdc65816MemoryType type;
    String content;
} Wdc65816MapProperties;

typedef struct {
    Wdc65816MapProperties map;
    Buffer data;
} Wdc65816MapperEntry;

typedef struct {
    uint                entries_length;
    Wdc65816MapperEntry entries[256];
} Wdc65816MapperBuilder;

typedef struct {
    Wdc65816MapperEntry entries[256];
    uint                entries_length;
    u8**                data;
} Wdc65816Mapper;

void wdc65816_mapper_builder_init(Wdc65816MapperBuilder* map_builder);
void wdc65816_mapper_builder_deinit(Wdc65816MapperBuilder* map_builder);
void wdc65816_mapper_builder_add(Wdc65816MapperBuilder* map_builder, Wdc65816MapperEntry* entry);

ulong wdc65816_mapper_get_buffer_size();
void wdc65816_mapper_init(Wdc65816Mapper* mapper, Wdc65816MapperBuilder* builder, u8** buffer);
void wdc65816_mapper_deinit(Wdc65816Mapper* mapper);
u8   wdc65816_mapper_read(Wdc65816Mapper* mapper, u32 addr);
void wdc65816_mapper_write(Wdc65816Mapper* mapper, u32 addr, u8 data);
u8*  wdc65816_mapper_ptr(Wdc65816Mapper* mapper, u32 addr);
u8*  wdc65816_mapper_read_range(Wdc65816Mapper* mapper, u32 addr_low, u32 addr_high, u8* data);
u8*  wdc65816_mapper_write_range(Wdc65816Mapper* mapper, u32 addr_low, u32 addr_high, u8* data);
#endif //WDC65816_MAPPER_H
