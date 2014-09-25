#ifndef MOCKUP_TILES_HPP
#define MOCKUP_TILES_HPP

#include <cstdint>
#include <QImage>

namespace Mockup {
    struct Tile8 {
        uint8_t pixels[64];

        Tile8 const mirrored(bool flipX, bool flipY); 
        QImage const toImage();
    };
    
    struct Tile16 {
        uint32_t pixels[256];
        static Tile16 fromTile8(Tile8 t[4], const uint8_t (&palette)[4]);

        QImage const toImage(uint32_t palette[256]);
    };
    
}

#endif //MOCKUP_TILES_HPP
