#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "r65816/rom.h"

int main(int argc, char* argv[]) {
    if(argc < 3 || argc > 4) {
        printf("Usage: expand FILE SIZE [FILE2]. Expands rom located at FILE to SIZE \n");
        printf("and writes it to FILE or FILE2, depending on whether FILE2 was given.\n");
        return 0;
    }
    r65816_rom_t rom;
    r65816_rom_load(&rom, argv[1]);
    uint8_t err = r65816_rom_expand(&rom, atoi(argv[2]));
    uint8_t header[512];
    memset(header, 0, 512);
    header[0] = 0x40;
    if(!err) r65816_rom_save_headered(&rom, argv[argc == 3 ? 1 : 3], header, 512);
}
