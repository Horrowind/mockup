#ifndef MOCKUP_SMW_ROM_H
#define MOCKUP_SMW_ROM_H

#include "wdc65816/wdc65816.h"

#include "freespace.h"
#include "gfx_store.h"
#include "level.h"
#include "overworld.h"

typedef struct {
    LevelPC    levels[512];
    GFXStore   gfx_pages;
    overworld_t   overworld;
    WDC65816Rom* rom;
    freespace_manager_t freespace_manager;
    WDC65816Cpu  cpu;
    Arena       arena;
    Arena       temp_arena;
} SMW;

void smw_init(SMW* smw, WDC65816Rom* rom, Arena* arena);
void smw_deinit(SMW* smw);

void smw_init_all(SMW* smw, WDC65816Rom* rom, Arena* arena);

void smw_level_load(SMW* smw, u16 level_num);
void smw_level_animate(SMW* smw, u16 level_num, u8 frame);
void smw_level_deinit(SMW* smw, u16 level_num);

void smw_level_refresh(SMW* smw, u16 level_num);

void smw_level_remove_layer1_object(SMW* smw, u16 level_num, uint object_index);
void smw_level_add_layer1_object(SMW* smw, u16 level_num, uint object_index, ObjectPC object);

#endif //MOCKUP_SMW_ROM_H
