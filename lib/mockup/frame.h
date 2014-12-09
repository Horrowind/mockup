#ifndef MOCKUP_FRAME_H
#define MOCKUP_FRAME_H

typedef struct {
    uint8_t width;
    uint8_t height;
    tile16_t* map16;
    uint16_t* data;

    uint32_t background_color;
} frame_t;




#endif //MOCKUP_FRAME_H
