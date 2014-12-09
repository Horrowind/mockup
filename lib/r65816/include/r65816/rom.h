#ifndef R65816_ROM_H
#define R65816_ROM_H

#include <stdint.h>


typedef uint8_t* r65816_bank;

typedef struct {
    char name[21];
    uint8_t layout;
    uint8_t cartridge_type;
    uint8_t rom_size;
    uint8_t ram_size;
    uint8_t country_code;
    uint8_t licencee_code;
    uint8_t version;
    uint8_t checksum_complement;
    uint8_t checksum;
    uint8_t unknown1[4];
    uint16_t interrupt_native[6];
    uint8_t unknown2[4];
    uint16_t interrupt_emulation[6];
} r65816_rom_header_t;



typedef struct {
    const char*  name;
    uint32_t     banksize;
    unsigned int num_banks;
    r65816_bank* banks;
    uint8_t*     data;
    r65816_rom_header_t* header;
} r65816_rom_t;

void r65816_rom_load(r65816_rom_t* rom, const char* path);
void r65816_rom_free(r65816_rom_t* rom);
void r65816_rom_save(r65816_rom_t* rom, const char* path);
void r65816_rom_save_headered(r65816_rom_t* rom, const char* path, uint8_t* header, int headersize);
uint8_t r65816_rom_expand(r65816_rom_t* rom, uint8_t size);
uint8_t r65816_rom_expand_lorom(r65816_rom_t* rom, uint8_t size);
uint8_t r65816_rom_expand_hirom(r65816_rom_t* rom, uint8_t size);
#endif //R65816_ROM_H
