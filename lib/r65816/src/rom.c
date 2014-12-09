#include "stdio.h"
#include "stdlib.h"

#include "rom.h"

uint8_t r65816_guess_header(r65816_rom_t* rom);
uint8_t r65816_score_header(r65816_rom_t* rom, uint32_t address);

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
    fclose(fp);
    r65816_guess_header(rom);
    for(int i = 0; i < rom->num_banks; i++) {
	rom->banks[i] = rom->data + i * rom->banksize - 0x8000;
    }

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

uint8_t r65816_score_header(r65816_rom_t* rom, uint32_t address) {
    if(rom->banksize * rom->num_banks < address + 64){
	return 0;
    }
    uint8_t score = 0;
    r65816_rom_header_t* header = (r65816_rom_header_t*)rom->data + address;
    uint16_t reset_vector = header->interrupt_emulation[5];
    uint16_t checksum = header->checksum;
    uint16_t complement = header->checksum_complement;
    uint8_t resetop = rom->data[(address & ~0x7fff) | (reset_vector & 0x7fff)];
    uint8_t guessed_mapper = header->layout & ~0x10;

    if(reset_vector < 0x8000){
	return 0;
    }

    if(resetop == 0x78 || resetop == 0x18 || resetop == 0x38 ||
       resetop == 0x9c || resetop == 0x4c || resetop == 0x5c){
	score += 8;
    }

    if(resetop == 0xc2 || resetop == 0xe2 || resetop == 0xad || resetop == 0xae ||
       resetop == 0xac || resetop == 0xaf || resetop == 0xa9 || resetop == 0xa2 ||
       resetop == 0xa0 || resetop == 0x20 || resetop == 0x22){
	score += 4;
    }
    if(resetop == 0x40 || resetop == 0x60 || resetop == 0x6b ||
       resetop == 0xcd || resetop == 0xec || resetop == 0xcc)
	score -= 4;

    if(resetop == 0x00 || resetop == 0x02  || resetop == 0xdb ||
       resetop == 0x42 || resetop == 0xff){
	score -= 8;
    }

    if((checksum + complement) == 0xffff && (checksum != 0) && (complement != 0)){
	score += 4;
    }

    if(address == 0x007fc0 && guessed_mapper == 0x20){
	score += 2;
    }else if(address == 0x00ffc0 && guessed_mapper == 0x21){
	score += 2;
    }else if(address == 0x007fc0 && guessed_mapper == 0x22){
	score += 2;
    }else if(address == 0x40ffc0 && guessed_mapper == 0x25){
	score += 2;
    }
    if(header->licencee_code == 0x33){
	score += 2;
    }
    if(header->ram_size < 0x08){
	score++;
    }
    if(header->country_code < 14){
	score++;
    }
    if(header->cartridge_type < 0x08){
	score++;
    }else if(header->rom_size < 0x10){
	score++;
    }

    if(score < 0){
	return 0;
    }

    return score;
}

uint8_t r65816_guess_header(r65816_rom_t* rom) {
    uint8_t score_lo = r65816_score_header(rom, 0x007fc0);
    uint8_t score_hi = r65816_score_header(rom, 0x00ffc0);
    uint8_t score_ex = r65816_score_header(rom, 0x40ffc0);
    
    if(score_lo >= score_hi && score_lo >= score_ex + 4){
	rom->header = (r65816_rom_header_t*)rom->data + 0x007fc0;
	return score_lo;
    }else if(score_hi >= score_ex + 4){
        rom->header = (r65816_rom_header_t*)rom->data + 0x00ffc0;
	return score_hi;
    }else{
	rom->header = (r65816_rom_header_t*)rom->data + 0x40ffc0;
	return score_ex;
    }
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
    rom->header->rom_size = size;
    rom->num_banks = snes_header_sizes[size] / rom->banksize;
    return 0;
}
