#ifndef R65816_ROM_H
#define R65816_ROM_H

#include <stdint.h>


typedef uint8_t* r65816_bank;

struct r65816_rom {
    const char*  name;
    uint32_t     banksize;
    unsigned int num_banks;
    r65816_bank* banks;
    uint8_t*     data;
};
typedef struct r65816_rom r65816_rom_t;

void r65816_rom_load(r65816_rom_t* rom, const char* path);
void r65816_rom_free(r65816_rom_t* rom);
void r65816_rom_save(r65816_rom_t* rom, const char* path);
void r65816_rom_save_headered(r65816_rom_t* rom, const char* path, uint8_t* header, int headersize);
uint8_t r65816_rom_expand(r65816_rom_t* rom, uint8_t size);


#endif //R65816_ROM_H
