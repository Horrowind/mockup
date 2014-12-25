#ifndef MOCKUP_SMW_ROM_H
#define MOCKUP_SMW_ROM_H

#include "rom.h"

typedef struct {
    level_t      levels[512];
    gfx_store_t* gfx_pages;
    overworld_t* overworld;
    rom_t*       rom;
} smw_t;

void smw_load(smw_t* smw);

#endif //MOCKUP_SMW_ROM_H
