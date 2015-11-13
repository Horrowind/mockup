#include <stdio.h>

#include "object.h"
#include "r65816/cpu.h"

void object_list_init_level_data(object_list_pc_t* object_list, r65816_rom_t* rom, uint8_t* data) { }

//Sort from left to right, up to down
void objects_sort_left_right(object_pc_t* objects, int n) {
    int i, j;
    object_pc_t p, t;
    if (n < 2) return;
    p = objects[n / 2];
    for (i = 0, j = n - 1;; i++, j--) {
        while (objects[i].x < p.x || (objects[i].x == p.x && objects[i].y < p.y))
            i++;
        while (objects[i].x > p.x || (objects[i].x == p.x && objects[i].y > p.y))
            j--;
        if (i >= j)
            break;
        t = objects[i];
        objects[i] = objects[j];
        objects[j] = t;
    }
    object_list_sort_left_right(objects, i);
    object_list_sort_left_right(objects + i, n - i);
}

int object_list_pc_to_level_data(object_list_pc_t* object_list, uint8_t* data) {
    if(l->is_boss_level) return;


    objects_sort_left_right(object_list->objects, object_list->length);

    int index = 0;
    
}

void object_list_deinit_addr(object_list_pc_t* object_list) {
    free(object_list->objects);
}


int object_list_add(object_list_pc_t* object_list, object_pc_t object);
void object_list_del(object_list_pc_t* object_list, int index);
void object_list_rise(object_list_pc_t* object_list, int index);
void object_list_sink(object_list_pc_t* object_list, int index);
void object_list_pc_to_front(object_list_pc_t* object_list, int index);
void object_list_pc_to_back(object_list_pc_t* object_list, int index);
int object_list_get_zindex(object_list_pc_t* object_list, int index);

void object_list_pick(object_list_pc_t* object_list, int x, int y);

void object_render_to_layer16(object_pc_t* object, r65816_rom_t* rom, layer16_t* layer);

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

void object_list_render_to_layer16(object_list_pc_t* object_list, layer16_t* layer) {
    for(int i = 0; i < object_list->length; i++) {
        object_pc_t obj = object_list->objects[i];
        int obj_width = obj.bb_xmax - obj.bb_xmin + 1;
        for(int y = MAX(0, obj.bb_ymin); y < MIN(layer->height, obj.bb_ymax); y++) {
            for(int x = MAX(0, obj.bb_xmin); x < MIN(layer->width, obj.bb_xmax); x++) {
                layer->data[y * layer->width + x] = obj.tiles[(y - obj.bb_ymin)*obj_width + (x - obj.bb_xmin)];
            }
        }
    }
}

#undef MAX
#undef MIN
uint32_t object_list_serialize(object_list_pc_t* object_list, uint8_t* data);
