#ifndef MOCKUP_TILES_HPP
#define MOCKUP_TILES_HPP

#include <cstdint>

namespace Mockup {
    struct Tile8 {
        uint8_t pixels[64];

        Tile8 const mirrored(bool flipX, bool flipY); 
	static Tile8 from3bpp(uint8_t* data);
    };
    
    struct Tile16 {
        uint32_t pixels[256];
        static Tile16 fromTile8(Tile8 t[4], const uint8_t (&palette)[4]);
    };
    
}

#endif //MOCKUP_TILES_HPP
