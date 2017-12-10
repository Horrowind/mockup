#include "palette.h"

void palette_to_pc(Palette* palette, PalettePC* palette_pc) {
    for(int i = 0; i < 256; i++) {
        u16 snes = palette->data[i];
        int r = ((snes & 0x001F) << 3); r |= (r >> 5);
        int g = ((snes & 0x03E0) >> 2); g |= (g >> 5);
        int b = ((snes & 0x7C00) >> 7); b |= (b >> 5);
        if(i & 0x0F) {
            palette_pc->data[i] = (r ) + (g << 8) + (b << 16) + 0xFF000000;
        } else {
            palette_pc->data[i] = 0;
        }
    }
}
