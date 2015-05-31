#ifndef MOCKUP_SPRITES_H
#define MOCKUP_SPRITES_H

typedef struct {
    uint8_t y1         : 4;
    uint8_t extra_bits : 2;
    uint8_t screen1    : 1;
    uint8_t y2         : 1;
    uint8_t x          : 4;
    uint8_t screen2    : 4;
    uint8_t enemy;
} sprite_t;

typedef struct {
    int x;
    int y;
    uint8_t flip_x : 1;
    uint8_t flip_y : 1;
    
    uint8_t palette;
    tile8_t tile;
} sprite_tile_t;



#endif //MOCKUP_SPRITES_H
