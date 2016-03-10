#ifndef MOCKUP_FRAME_H
#define MOCKUP_FRAME_H

typedef struct {
    u8 width;
    u8 height;
    tile16_t* map16;
    u16* data;

    u32 background_color;
} frame_t;




#endif //MOCKUP_FRAME_H
