#ifndef MOCKUP_OBJECT_H
#define MOCKUP_OBJECT_H

#include "r65816/rom.h"
#include "stdint.h"

typedef struct {
    uint32_t x;
    uint32_t y;
    uint8_t number;
    uint8_t settings;
    uint8_t extended : 1;
    int     z_index;
} object_t;

typedef struct object_entry {
    object_t object;
    struct object_entry* next;
} object_list_t;

void object_list_load_from_level(object_list_t* object_list, r65816_rom_t* rom, int num_level);
void object_list_deinit(object_list_t* object_list);
void object_render_to_layer16(object_t* object, r65816_rom_t* rom, layer16_t layer);
#endif //MOCKUP_OBJECT_H
