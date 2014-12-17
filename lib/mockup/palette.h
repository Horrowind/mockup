#ifndef MOCKUP_PALETTE_H
#define MOCKUP_PALETTE_H

typedef struct {
    uint16_t[256] data;
} palette_t;

typedef struct {
    uint32_t[256] data;
} palette_pc_t;

void palette_load_level_palette(palette_t* palette, r65816_rom_t* rom, int num_level);

uint32_t palette_to_pc(palette_t* palette, uint8_t index) {
    uint16_t snes = palette->data[index];
    int r = ((snes & 0x1F) << 3);   r |= (r >> 5);
    int g = ((snes & 0x3E0) >> 2);  g |= (g >> 5);
    int b = ((snes & 0x7C00) >> 7); b |= (b >> 5);
    return 0xFF000000 + (r << 16) + (g << 8) + b;
}


#endif //MOCKUP_PALETTE_H
