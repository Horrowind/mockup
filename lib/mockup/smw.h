#ifndef MOCKUP_SMW_ROM_H
#define MOCKUP_SMW_ROM_H

#include "r65816.h"

#include "freespace.h"
#include "gfx_store.h"
#include "level.h"
#include "overworld.h"

typedef struct {
    level_pc_t    levels[512];
    gfx_store_t   gfx_pages;
    overworld_t   overworld;
    r65816_rom_t* rom;
    freespace_manager_t freespace_manager;
} smw_t;

void smw_init(smw_t* smw, r65816_rom_t* rom);
void smw_deinit(smw_t* smw);

void smw_init_all(smw_t* smw, r65816_rom_t* rom);
void smw_deinit_all(smw_t* smw);

void smw_level_load(smw_t* smw, u16 level_num);
void smw_level_animate(smw_t* smw, u16 level_num, u8 frame);
void smw_level_deinit(smw_t* smw, u16 level_num);

void smw_level_remove_layer1_object(smw_t* smw, u16 level_num, uint object_index);

#endif //MOCKUP_SMW_ROM_H
