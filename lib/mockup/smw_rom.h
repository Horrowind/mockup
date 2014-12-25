#ifndef MOCKUP_SMW_ROM_H
#define MOCKUP_SMW_ROM_H

#include "rom.h"

typedef struct {
    level_t     levels[512];
    gfx_store_t gfx_pages;
    overworld_t overworld;
    rom_t*      rom;
} smw_t;

void smw_load(smw_t* smw, rom_t* rom);
void smw_deinit(smw_t* smw);

void smw_load_path(smw_t* smw, const char* path);
void smw_deinit_path(smw_t* smw);

#endif //MOCKUP_SMW_ROM_H
