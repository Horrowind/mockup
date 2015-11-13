#ifndef MOCKUP_OBJECT_H
#define MOCKUP_OBJECT_H

#include "stdint.h"
#include "r65816/rom.h"
#include "layer.h"

typedef struct {
    uint32_t x;
    uint32_t y;
    uint8_t num : 6;
    uint8_t settings;

    uint16_t bb_xmin;
    uint16_t bb_ymin;
    uint16_t bb_xmax;
    uint16_t bb_ymax;
    uint16_t zindex;
    uint16_t* tiles;
    uint16_t overlap_data;
    uint16_t overlap_length;
} object_pc_t;

typedef struct {
    object_pc_t* objects;
    uint16_t* overlap_data;
    uint16_t length;
} object_list_pc_t;


void object_list_init(object_list_pc_t* object_list, r65816_rom_t* rom, uint8_t* data);
void object_list_deinit(object_list_pc_t* object_list);

int object_list_level_data_length(object_list_pc_t* object_list);
int object_list_pc_to_level_data(object_list_pc_t* object_list, uint8_t* data);

int object_list_add(object_list_pc_t* object_list, object_pc_t object);
void object_list_del(object_list_pc_t* object_list, int index);
void object_list_rise(object_list_pc_t* object_list, int index);
void object_list_sink(object_list_pc_t* object_list, int index);
void object_list_pc_to_front(object_list_pc_t* object_list, int index);
void object_list_pc_to_back(object_list_pc_t* object_list, int index);
int object_list_get_zindex(object_list_pc_t* object_list, int index);

void object_render_to_layer16(object_pc_t* object, r65816_rom_t* rom, layer16_t* layer);
void object_list_render_to_layer16(object_list_pc_t* object_list, layer16_t* layer);
uint32_t object_list_serialize(object_list_pc_t* object_list, uint8_t* data);
#endif //MOCKUP_OBJECT_H
