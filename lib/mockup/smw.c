#include "smw.h"

void smw_init_all(smw_t* smw, r65816_rom_t* rom) {
    smw->rom = rom;
    gfx_store_init(&smw->gfx_pages, rom);
    for(int i = 0; i < 512; i++) {
        printf("load level %03x\n", i);
        level_init(&smw->levels[i], rom, i, &smw->gfx_pages);
    }
}

void smw_deinit_all(smw_t* smw) {
    for(int i = 0; i < 512; i++) {
        level_deinit(&smw->levels[i]);
    }
    gfx_store_deinit(&smw->gfx_pages);
}


void smw_init(smw_t* smw, r65816_rom_t* rom) {
    smw->rom = rom;
    gfx_store_init(&smw->gfx_pages, rom);
    for(int i = 0; i < 512; i++) {
        smw->levels[i].rom = NULL;
    }

}

void smw_deinit(smw_t* smw) {
    for(int i = 0; i < 512; i++) {
        if(smw->levels[i].rom != NULL)
            level_deinit(&smw->levels[i]);
    }
    gfx_store_deinit(&smw->gfx_pages);
}

void smw_level_load(smw_t* smw, int level_num) {
    level_init(&smw->levels[level_num], smw->rom, level_num, &smw->gfx_pages);
}
