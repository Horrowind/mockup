#include "stdio.h"
#include "stdlib.h"

#include "rom.h"

void r65816_rom_load(r65816_rom_t* rom, const char* path) {
    FILE* fp = fopen(path, "r");
    if(!fp) return;
    fseek(fp, 0, SEEK_END);
    rom->banksize = 0x8000; //Currently only LOROM is supported
    unsigned int filesize = ftell(fp);
    unsigned int headersize = filesize % rom->banksize;
    fseek(fp, headersize, SEEK_SET);
    rom->num_banks = (filesize - headersize) / rom->banksize;
    rom->banks = malloc(rom->num_banks * sizeof(r65816_bank));
    rom->data = malloc(rom->num_banks * rom->banksize);
    fread((char*)rom->data, 1, rom->num_banks * rom->banksize, fp);
    for(int i = 0; i < rom->num_banks; i++) {
        rom->banks[i] = rom->data + i * rom->banksize;
        
    }
    fclose(fp);
}

void r65816_rom_save(r65816_rom_t* rom, const char* path) {
    FILE* fp = fopen(path, "w");
    if(!fp) return;
    fwrite(rom->data, 1, rom->num_banks * rom->banksize, fp);
    fclose(fp);
}

void r65816_rom_save_headered(r65816_rom_t* rom, const char* path, uint8_t* header, int headersize) {
    FILE* fp = fopen(path, "w");
    if(!fp) return;
    fwrite(header, 1, headersize, fp);
    fwrite(rom->data, 1, rom->num_banks * rom->banksize, fp);
    fclose(fp);
}



void r65816_rom_free(r65816_rom_t* rom) {
    free(rom->data);
    free(rom->banks);
}

static uint32_t snes_header_sizes[0x0D] = {
    0x000000, //0
    0x000800, //1
    0x001000, //2
    0x002000, //3
    0x004000, //4
    0x008000, //5
    0x010000, //6
    0x020000, //7
    0x040000, //8
    0x080000, //9
    0x100000, //A
    0x200000, //B
    0x400000  //C
};

// Uses the value in the SNES-Rom header (byte 0x17), see snes_header_sizes
uint8_t r65816_rom_expand(r65816_rom_t* rom, uint8_t size) {
    if(size > 0x0D) return -1;
    uint8_t* ptr = realloc(rom->data, snes_header_sizes[size]);
    if(ptr == 0) return -1;
    rom->data = ptr;
    rom->data[0x7fd7] = size;
    rom->num_banks = snes_header_sizes[size] / rom->banksize;
    return 0;
}
