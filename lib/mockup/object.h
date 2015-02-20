#ifndef MOCKUP_OBJECT_H
#define MOCKUP_OBJECT_H

#include "stdint.h"
#include "r65816/rom.h"
#include "layer.h"

typedef struct {
    uint32_t x;
    uint32_t y;
    uint32_t z_index;
    union {
        uint8_t number;
        uint8_t settings;
        struct {
            uint8_t width : 4;
            uint8_t height : 4;
        };
    };
    uint8_t extended : 1;
} object_t;

typedef struct {
    int length;
    object_t* object;
} object_list_t;



void object_list_init(object_list_t* object_list);
void object_list_deinit(object_list_t* object_list);

void object_list_init_addr(object_list_t* object_list, r65816_rom_t* rom, uint32_t addr);
void object_list_deinit_addr(object_list_t* object_list);

void object_list_init_level_data(object_list_t* object_list, uint8_t* data);
void object_list_deinit_level_data(object_list_t* object_list);

void object_list_level_data_length(object_list_t* object_list);
void object_list_to_level_data(object_list_t* object_list, uint8_t* data);

void object_list_add(object_list_t* object_list, object_t object);
void object_list_del(object_list_t* object_list, object_t* object);

void object_render_to_layer16(object_t* object, r65816_rom_t* rom, layer16_t* layer);
void object_list_render_to_layer16(object_list_t* object_list, r65816_rom_t* rom, layer16_t* layer);
uint32_t object_list_serialize(object_list_t* object_list, uint8_t* data);
#endif //MOCKUP_OBJECT_H
