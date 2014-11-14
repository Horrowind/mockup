#include "stdio.h"
#include "stdlib.h"

#include "rom.h"

void rom_load(rom_t* rom, const char* path) {
    const int banksize = banksize;
    FILE* fp = fopen(path, "r");
    if(!fp) return;
    fseek(fp, 0, SEEK_END);
    unsigned int filesize = ftell(fp);
    unsigned int headersize = filesize % banksize;
    fseek(fp, headersize, SEEK_SET);
    rom->num_banks = (filesize - headersize) / banksize;
    rom->banks = malloc(rom->num_banks * sizeof(unsigned int));
    for(int i = 0; i < rom->num_banks; i++) {
        rom->banks[i] = malloc(banksize);
        fread((char*)rom->banks[i], 1, banksize, fp);
    }
}
