#include "smw.h"

void smw_init(smw_t* smw, r65816_rom_t* rom) {
    smw->rom = rom;
    gfx_store_init(&smw->gfx_pages, rom);
    for(int i = 0; i < 512; i++) {
        printf("Level %03x started\n", i);
        switch(i) {
        /* case 0x009: //Layer 2 */
        /* case 0x00E: //Layer 2 */
        /* case 0x01A: //Layer 2 */
        /* case 0x0C4: //Layer 2 */
        /* case 0x0D4: //Layer 2 */
        /* case 0x0DC: //Layer 2 */
        /* case 0x0E7: //Layer 2 */
        /* case 0x0EB: //Layer 2 */
        /* case 0x0F0: //Layer 2 */
        /* case 0x0FB: //Layer 2 */
        /* case 0x111: //Layer 2 */
        /* case 0x115: //Layer 2 */
        /* case 0x11D: //Layer 2 */
        /* case 0x1CE: //Layer 2 */
        /* case 0x1CF: //Layer 2 */
        /* case 0x1DA: //Layer 2 */
        /* case 0x1E2: //Layer 2 */
        /* case 0x1E3: //Layer 2 */
        /* case 0x1E6: //Layer 2 */
        /* case 0x1E7: //Layer 2 */
        /* case 0x1E8: //Layer 2 */
        /* case 0x1E9: //Layer 2 */
        /* case 0x1EC: //Layer 2 */
        /* case 0x1EF: //Layer 2 */
        /* case 0x1F3: //Layer 2 */
        /* case 0x1F9: //Layer 2 */
            printf("Unable to render %03x\n", i);
            break;
        default:
            level_init(&smw->levels[i], rom, i, &smw->gfx_pages);
            break;
        };
        //printf("Level %03x done\n", i);
    }
}

void smw_deinit(smw_t* smw) {
    for(int i = 0; i < 512; i++) {
        level_deinit(&smw->levels[i]);
    }
    gfx_store_deinit(&smw->gfx_pages);
}

void smw_init_path(smw_t* smw, const char* path) {
    r65816_rom_t* rom = malloc(sizeof(r65816_rom_t));
    r65816_rom_load(rom, path);
    smw_init(smw, rom);
}

void smw_deinit_path(smw_t* smw) {
    smw_deinit(smw);
    free(smw->rom);
}
