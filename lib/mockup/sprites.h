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
    uint16_t tile_num;
} sprite_tile_t;

typedef struct {
    int x, y;
    uint8_t enemy;
    uint8_t extra_bits;
    int num_tiles;
    sprite_tile_t* tiles;   
} sprite_pc_t;

typedef struct {
    int length;
    sprite_pc_t* data;   
} sprite_list_pc_t;

#endif //MOCKUP_SPRITES_H
