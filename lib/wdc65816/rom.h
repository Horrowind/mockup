#ifndef R65816_ROM_H
#define R65816_ROM_H

#include "base/base.h"
#include "mapper.h"

//#define BANKS

typedef struct {
    Wdc65816Mapper        read_mapper;
    Wdc65816Mapper        write_mapper;
} Wdc65816Rom;

void wdc65816_rom_init(Wdc65816Rom* rom, VFS* vfs, Arena* arena);
void wdc65816_rom_free(Wdc65816Rom* rom);
void wdc65816_map(Wdc65816Mapper* mapper, Wdc65816MapperEntry* entry);
void wdc65816_rom_save(Wdc65816Rom* rom, char* path);
void wdc65816_rom_save_headered(Wdc65816Rom* rom, char* path, u8* header, int headersize);
u8   wdc65816_rom_expand(Wdc65816Rom* rom, u8 size);
u8   wdc65816_rom_expand_lorom(Wdc65816Rom* rom, u8 size);
u8   wdc65816_rom_expand_hirom(Wdc65816Rom* rom, u8 size);
#endif //R65816_ROM_H
