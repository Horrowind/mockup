#ifndef MOCKUP_SMW_ROM_H
#define MOCKUP_SMW_ROM_H

#include "r65816/rom.h"
#include "level.h"
#include "gfx_store.h"
#include "overworld.h"
typedef struct {
    level_t       levels[512];
    gfx_store_t   gfx_pages;
    overworld_t   overworld;
    r65816_rom_t* rom;
} smw_t;

void smw_init(smw_t* smw, r65816_rom_t* rom);
void smw_deinit(smw_t* smw);

void smw_init_path(smw_t* smw, const char* path);
void smw_deinit_path(smw_t* smw);

#endif //MOCKUP_SMW_ROM_H
