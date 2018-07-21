#ifndef MOCKUP_SMW_H
#define MOCKUP_SMW_H

#include "wdc65816/wdc65816.h"

#include "gfx_store.h"
#include "level.h"

typedef struct {
    LevelPC      levels[512];
    GFXStore     gfx_pages;
    Wdc65816Cpu  cpu;
    Arena        arena;
    Arena        temp_arena;
} SMW;

void smw_init(SMW* smw, Wdc65816MapperBuilder* rom, Arena* arena);
void smw_deinit(SMW* smw);

void smw_init_all(SMW* smw, Wdc65816MapperBuilder* rom, Arena* arena);

void smw_level_load(SMW* smw, u16 level_num);
void smw_level_animate(SMW* smw, u16 level_num, u8 frame);
void smw_level_deinit(SMW* smw, u16 level_num);

void smw_level_refresh(SMW* smw, u16 level_num);

void smw_level_remove_layer1_object(SMW* smw, u16 level_num, uint object_index);
void smw_level_add_layer1_object(SMW* smw, u16 level_num, uint object_index, ObjectPC object);
Buffer smw_level_serialize_fast(SMW* smw, u16 level_num);
#endif //MOCKUP_SMW_H
