#include "smw.h"

int main(int argc, char** argv) {
	smw_t smw;
    r65816_rom_t rom;
    if(argc > 1) {
        r65816_rom_load(&rom, argv[0]);
	} else {
        r65816_rom_load(&rom, "smw.sfc"); 
	}
    smw_init_all(&smw, &rom);
    smw_deinit_all(&smw);
    r65816_rom_free(&rom);
	return 0;
}
