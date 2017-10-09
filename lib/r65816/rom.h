#ifndef R65816_ROM_H
#define R65816_ROM_H

#include "mapper.h"

typedef u8* r65816_bank;

typedef struct {
    char name[21];
    u8 layout;
    u8 cartridge_type;
    u8 rom_size;
    u8 ram_size;
    u8 country_code;
    u8 licencee_code;
    u8 version;
    u8 checksum_complement;
    u8 checksum;
    u8 unknown1[4];
    u16 interrupt_native[6];
    u8 unknown2[4];
    u16 interrupt_emulation[6];
} r65816_rom_header_t;


typedef struct {
    r65816_rom_header_t* header;
    r65816_mapper_t      read_mapper;
    r65816_mapper_t      write_mapper;
    u8**                 banks;
    u8*                  data;

    r65816_read_function_t  read;
    r65816_write_function_t write;
    r65816_ptr_function_t   ptr;
} r65816_rom_t;

void r65816_rom_init(r65816_rom_t* rom, vfs_t* vfs);
//void r65816_rom_init(r65816_rom_t* rom, u8* data, uint length);
void r65816_rom_load(r65816_rom_t* rom, char* path);
void r65816_rom_free(r65816_rom_t* rom);
void r65816_map(r65816_mapper_t* mapper, r65816_mapper_entry_t* entry);
void r65816_rom_save(r65816_rom_t* rom, char* path);
void r65816_rom_save_headered(r65816_rom_t* rom, char* path, u8* header, int headersize);
u8 r65816_guess_header(r65816_rom_t* rom);
u8 r65816_rom_expand(r65816_rom_t* rom, u8 size);
u8 r65816_rom_expand_lorom(r65816_rom_t* rom, u8 size);
u8 r65816_rom_expand_hirom(r65816_rom_t* rom, u8 size);
#endif //R65816_ROM_H
