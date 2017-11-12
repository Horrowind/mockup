#ifndef MOCKUP_SMW_ROM_H
#define MOCKUP_SMW_ROM_H

#include "wdc65816/wdc65816.h"

#include "freespace.h"
#include "gfx_store.h"
#include "level.h"
#include "overworld.h"

typedef struct {
    level_pc_t    levels[512];
    gfx_store_t   gfx_pages;
    overworld_t   overworld;
    wdc65816_rom_t* rom;
    freespace_manager_t freespace_manager;
    wdc65816_cpu_t  cpu;
    arena_t       arena;
    arena_t       temp_arena;
} smw_t;

void smw_init(smw_t* smw, wdc65816_rom_t* rom, arena_t* arena);
void smw_deinit(smw_t* smw);

void smw_init_all(smw_t* smw, wdc65816_rom_t* rom, arena_t* arena);

void smw_level_load(smw_t* smw, u16 level_num);
void smw_level_animate(smw_t* smw, u16 level_num, u8 frame);
void smw_level_deinit(smw_t* smw, u16 level_num);

void smw_level_refresh(smw_t* smw, u16 level_num);

void smw_level_remove_layer1_object(smw_t* smw, u16 level_num, uint object_index);
void smw_level_add_layer1_object(smw_t* smw, u16 level_num, uint object_index, object_pc_t object);

#endif //MOCKUP_SMW_ROM_H
