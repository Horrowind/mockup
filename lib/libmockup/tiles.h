#ifndef MOCKUP_TILES_HPP
#define MOCKUP_TILES_HPP

#include <stdint>

struct tile8 {
    uint8_t pixels[64];
};
typedef struct tile8 tile8_t;
    
struct tile16 {
    uint32_t pixels[256];
};


tile8_t tile8_flip(tile8_t tile, bool flipX, bool flipY)
tile8_t tile8_from3bpp(uint8_t* data);
tile8_t tile8_from4bpp(uint8_t* data);

tile16_t tile16_fromTile8(tile8_t t[4], uint8_t* palette);
    
#endif //MOCKUP_TILES_HPP
