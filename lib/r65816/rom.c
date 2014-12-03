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
    rom->banks = malloc(rom->num_banks * sizeof(r65816_bank));
    rom->data = malloc(rom->num_banks * banksize);
    for(int i = 0; i < rom->num_banks; i++) {
        rom->banks[i] = rom->data + i * banksize;
        fread((char*)rom->bank[i], 1, banksize, fp);
    }
    fclose(fp);
}

void r65816_rom_free(r65816_rom_t* rom) {
    free(rom->data);
    free(rom->banks);
}

void r65816_rom_expand(r65816_rom_t* rom, uint8_t size) {


}
