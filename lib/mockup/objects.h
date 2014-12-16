#ifndef MOCKUP_OBJECT_H
#define MOCKUP_OBJECT_H

typedef struct object {
    uint32_t x;
    uint32_t y;
    uint8_t number;
    uint8_t settings;
    uint8_t extended : 1;
    int     z_index;
    struct object* next;
} object_list_t;

void object_list_load(object_list_t* objects, rom_t* rom, uint32_t address);
void object_list_add(object_list_t* objects, struct object object);
void object_list_del(object_list_t* objects, struct object* object);

void object_list_render(object_list_t* objects, map16_t* map16, layer16_t* layer);

#endif //MOCKUP_OBJECT_H
