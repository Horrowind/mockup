#ifndef R65816_ROM_H
#define R65816_ROM_H

#include "base/base.h"
#include "mapper.h"

//#define BANKS

typedef struct {
    WDC65816Mapper        read_mapper;
    WDC65816Mapper        write_mapper;

    WDC65816ReadFunction  read;
    WDC65816WriteFunction write;
    WDC65816PtrFunction   ptr;
} WDC65816Rom;

void wdc65816_rom_init(WDC65816Rom* rom, VFS* vfs, Arena* arena);
void wdc65816_rom_free(WDC65816Rom* rom);
void wdc65816_map(WDC65816Mapper* mapper, WDC65816MapperEntry* entry);
void wdc65816_rom_save(WDC65816Rom* rom, char* path);
void wdc65816_rom_save_headered(WDC65816Rom* rom, char* path, u8* header, int headersize);
u8   wdc65816_rom_expand(WDC65816Rom* rom, u8 size);
u8   wdc65816_rom_expand_lorom(WDC65816Rom* rom, u8 size);
u8   wdc65816_rom_expand_hirom(WDC65816Rom* rom, u8 size);
#endif //R65816_ROM_H
