#include "stdio.h"
#include "stdlib.h"

#include "rom.h"

void r65816_rom_load(r65816_rom_t* rom, const char* path) {
    const int banksize = banksize;
    FILE* fp = fopen(path, "r");
    if(!fp) return;
    fseek(fp, 0, SEEK_END);
    unsigned int filesize = ftell(fp);
    unsigned int headersize = filesize % banksize;
    fseek(fp, headersize, SEEK_SET);
    rom->num_banks = (filesize - headersize) / banksize;
    rom->banks = malloc(rom->num_banks * sizeof(int) + banksize * rom->num_banks);
    rom->banks[0] = (r65816_bank)(rom->banks + rom->num_banks * sizeof(int));
    for(int i = 0; i < rom->num_banks; i++) {
        rom->banks[i] = rom->banks[0] + i * banksize;
        fread((char*)rom->banks[i], 1, banksize, fp);
    }
}

void r65816_rom_free(r65816_rom_t* rom) {
    for(int i = 0; i < rom->num_banks; i++) {
        free(rom->banks[i]);
    }
    free(rom->banks);
}
