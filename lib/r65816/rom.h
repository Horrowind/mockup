#ifndef R65816_ROM_H
#define R65816_ROM_H

#include <stdint.h>

const int banksize = 0x8000;

typedef uint8_t* bank;

struct rom {
    const char*  name;
    bank*        banks;
    unsigned int num_banks;
};
typedef struct rom rom_t;

void rom_load(rom_t* rom, const char* path);
void rom_expand(rom_t* rom, uint32_t size);


#endif //R65816_ROM_H
