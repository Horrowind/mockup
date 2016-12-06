#include "smw.h"

int main(int argc, char** argv) {
    smw_t smw;
    r65816_rom_t rom;
#if 0
    if(argc > 1) {
        r65816_rom_load(&rom, argv[0]);
    } else {
        r65816_rom_load(&rom, "smw.sfc"); 
    }
#else

#include "smw_file.h"
    rom.banksize = 0x8000; //Currently only LOROM is supported
    unsigned int filesize = sizeof(smw_sfc) / sizeof(smw_sfc[0]);
    rom.num_banks = filesize / rom.banksize;
    rom.banks = malloc(rom.num_banks * sizeof(r65816_bank));
    rom.data = smw_sfc;
    r65816_guess_header(&rom);
    for(int i = 0; i < rom.num_banks; i++) {
        rom.banks[i] = rom.data + i * rom.banksize - 0x8000;
    }
#endif
    smw_init_all(&smw, &rom);
    smw_deinit_all(&smw);
    //r65816_rom_free(&rom);
    return 0;
}
