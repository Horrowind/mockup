

/* typedef struct { */
/*     level_t      levels[512]; */
/*     gfx_store_t* gfx_pages; */
/*     overworld_t* overworld; */
/* } smw_rom_t; */

void smw_load(smw_t* smw, rom_t* rom) {
  smw->rom = rom;
  smw->gfx_pages = malloc(sizeof(gfx_store_t));
  gfx_store_load(smw->gfx_pages, rom);
  for(int i = 0; i < 512; i++) {
    level_load(smw->levels[i], rom, smw->gfx_store, i);
  }
}




/* level_t* level_init(const char* path) { */

/*     l->path = path; */
/*     r65816_rom_load(l->rom, path); */
/*     return l; */
/* } */
