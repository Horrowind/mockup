#include "tiles.hpp"

#include <iostream>

namespace Mockup {

    Tile8 const Tile8::mirrored(bool flipX, bool flipY) {
        Tile8 tile_new;
        const int itx = flipX ? -1 : 1;
        const int ity = flipY ? -1 : 1;
        
        int sy  = flipY ?  7 : 0;
        for(int i = 0; i < 8; i++) {
            int sx  = flipX ?  7 : 0;
            for(int j = 0; j < 8; j++) {
                tile_new.pixels[i * 8 + j] = pixels[sy * 8 + sx];
                sx += itx;
                
            }
            sy += ity;
        }
        return tile_new;
    }

    QImage const Tile8::toImage() {
        QImage img(8, 8, QImage::Format_Indexed8);
        for(int k = 0; k < 8; k++) img.setColor(k, 0xFF000000 + 0x222222 * k);
        for(int k = 0; k < 8; k++) {
            for(int l = 0; l < 8; l++) {
                img.setPixel(l, k, pixels[k*8+l]);
            }
        }
        return img;
    }


    Tile16 Tile16::fromTile8(Tile8 t[4], const uint8_t (&pal)[4]) {
        Tile16 tile;
        for(int i = 0; i < 8; i++) {
            for(int j = 0; j < 16; j++) {
                if(j < 8) {
                    tile.pixels[i * 16 + j] = 16 * pal[0] + t[0].pixels[i * 8 + j];
                } else {
                    tile.pixels[i * 16 + j] = 16 * pal[2] + t[2].pixels[i * 8 + j - 8];
                }
            }
        }
        for(int i = 8; i < 16; i++) {
            for(int j = 0; j < 16; j++) {
                if(j < 8) {
                    tile.pixels[i * 16 + j] = 16 * pal[1] + t[1].pixels[i * 8 + j - 64];
                } else {
                    tile.pixels[i * 16 + j] = 16 * pal[3] + t[3].pixels[i * 8 + j - 72];
                }                
            }
        }
        return tile;
    }

    QImage const Tile16::toImage(uint32_t palette[256]) {
        QImage img(16, 16, QImage::Format_Indexed8);
        img.setColorCount(256);
        for(int k = 0; k < 256; k++) img.setColor(k, palette[k]);
        for(int k = 0; k < 16; k++) {
            for(int l = 0; l < 16; l++) {
                img.setPixel(l, k, pixels[k*16+l]);
            }
        }
        return img;
    }
}
