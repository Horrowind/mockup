#ifndef MOCKUP_SMW_ROM_H
#define MOCKUP_SMW_ROM_H

#include "rom.h"

typedef struct {
    level_t      levels[512];
    gfx_store_t* gfx_pages;
    overworld_t* overworld;
} smw_rom_t;



#endif //MOCKUP_SMW_ROM_H
