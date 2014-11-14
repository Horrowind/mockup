#ifndef MOCKUP_MISC_HPP
#define MOCKUP_MISC_HPP

int convertColor(int snes) {
    int r = ((snes & 0x1F) << 3);   r |= (r >> 5);
    int g = ((snes & 0x3E0) >> 2);  g |= (g >> 5);
    int b = ((snes & 0x7C00) >> 7); b |= (b >> 5);
    return 0xFF000000 + (r << 16) + (g << 8) + b;
}


#endif //MOCKUP_MISC_HPP
