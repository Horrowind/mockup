#ifndef R65816_ROM_H
#define R65816_ROM_H

#include "base/base.h"
#include "mapper.h"

//#define BANKS

typedef struct {
    wdc65816_mapper_t      read_mapper;
    wdc65816_mapper_t      write_mapper;

    wdc65816_read_function_t  read;
    wdc65816_write_function_t write;
    wdc65816_ptr_function_t   ptr;
} wdc65816_rom_t;

void wdc65816_rom_init(wdc65816_rom_t* rom, vfs_t* vfs, arena_t* arena);
void wdc65816_rom_free(wdc65816_rom_t* rom);
void wdc65816_map(wdc65816_mapper_t* mapper, wdc65816_mapper_entry_t* entry);
void wdc65816_rom_save(wdc65816_rom_t* rom, char* path);
void wdc65816_rom_save_headered(wdc65816_rom_t* rom, char* path, u8* header, int headersize);
u8   wdc65816_rom_expand(wdc65816_rom_t* rom, u8 size);
u8   wdc65816_rom_expand_lorom(wdc65816_rom_t* rom, u8 size);
u8   wdc65816_rom_expand_hirom(wdc65816_rom_t* rom, u8 size);
#endif //R65816_ROM_H
