#ifndef MOCKUP_ROM_H
#define MOCKUP_ROM_H

#include <stdint.h>

const int banksize = 0x8000;

typedef uint8_t* r65816_bank;

struct r65816_rom {
    const char*  name;
    r65816_bank* banks;
    unsigned int num_banks;
};
typedef struct r65816_rom r65816_rom_t;

void r65816_rom_load(r65816_rom_t* rom, const char* path);
void r65816_rom_expand(r65816_rom_t* rom, uint32_t size);


#endif //MOCKUP_ROM_H
