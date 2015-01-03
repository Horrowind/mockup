void smw_init(smw_t* smw, rom_t* rom) {
    smw->rom = rom;
    gfx_store_init(smw->gfx_pages, rom);
    for(int i = 0; i < 512; i++) {
        level_init(smw->levels[i], rom, smw->gfx_store, i);
    }
}

void smw_deinit(smw_t* smw) {
    for(int i = 0; i < 512; i++) {
        level_deinit(smw->levels[i]);
    }
    gfx_store_deinit(smw->gfx_pages);
}

void smw_init_path(smw_t* smw, const char* path) {
    rom_t* rom = malloc(sizeof(rom_t));
    r65816_rom_load(rom, path);
    smw_init(smw, rom);
}

void smw_deinit_path(smw_t* smw) {
    smw_deinit(smw, rom);
    free(smw->rom);
}
