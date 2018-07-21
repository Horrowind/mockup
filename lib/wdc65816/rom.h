#ifndef R65816_ROM_H
#define R65816_ROM_H

#include "base/base.h"
#include "mapper.h"
#include "board.h"
//#define BANKS

typedef struct {
    Wdc65816Board board;
    Wdc65816MapperBuilder* rom;

    Wdc65816MemoryType types[16];
    String             contents[16];
    uint               sizes[16];
    
    Buffer             buffers[16];
    uint length;
    uint index;
} Wdc65816RomLoader;

typedef struct {
    String name;
    Wdc65816MemoryType type;
    ulong  size;
} Wdc65816MemoryBufferRequest;

Wdc65816RomLoader wdc65816_rom_loader_begin(Wdc65816MapperBuilder* rom, String manifest);
void wdc65816_rom_loader_next(Wdc65816RomLoader* loader);
Wdc65816MemoryBufferRequest wdc65816_rom_loader_get_buffer_request(Wdc65816RomLoader* loader,
                                                                   Buffer string_buffer);
void wdc65816_rom_loader_set_buffer(Wdc65816RomLoader* loader, Buffer buffer);
int wdc65816_rom_loader_end(Wdc65816RomLoader* loader);
void wdc65816_rom_deinit(Wdc65816MapperBuilder* rom);
void wdc65816_rom_save(Wdc65816MapperBuilder* rom, char* path);
#endif //R65816_ROM_H
