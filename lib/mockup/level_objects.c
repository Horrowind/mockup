#include <assert.h>
#include <stdio.h>

#include "r65816.h"
#include "addresses.h"
#include "level.h"
#include "pool.h"

//Sort from left to right, up to down
void level_objects_sort_left_right(object_pc_t** objects, int n) {
    int i, j;
    object_pc_t* p, * t;
    if (n < 2) return;
    p = objects[n / 2];
    for (i = 0, j = n - 1;; i++, j--) {
        while (objects[i]->x < p->x || (objects[i]->x == p->x && objects[i]->y < p->y))
            i++;
        while (objects[j]->x > p->x || (objects[j]->x == p->x && objects[j]->y > p->y))
            j--;
        if (i >= j)
            break;
        t = objects[i];
        objects[i] = objects[j];
        objects[j] = t;
    }
    level_objects_sort_left_right(objects, i);
    level_objects_sort_left_right(objects + i, n - i);
}

int level_object_list_pc_to_level_data(object_list_pc_t* object_list, u8* output_data, int max_length) {

    int length = object_list->length;
    if(length == 0) return 0;
    
    // Get overlap data
    struct overlap_array {
        u16* data;
        int length;
    }* overlaps = malloc(length * sizeof(struct overlap_array));
    
    pool_t overlap_pool;
    pool_init(&overlap_pool);
    object_pc_t* objects = object_list->objects;

    for(int i = 1; i < length; i++) {
        overlaps[i].data = (u16*)(overlap_pool.data + overlap_pool.fill);
        for(int j = 0; j < i; j++) {

            if(!(objects[i].bb_xmin > objects[j].bb_xmax
                 || objects[i].bb_xmax < objects[j].bb_xmin
                 || objects[i].bb_ymin > objects[j].bb_ymax
                 || objects[i].bb_ymax < objects[j].bb_ymin)) {
                //The bounding boxes overlap
                u16* overlap_instance = (u16*)pool_alloc(&overlap_pool, sizeof(u16));
                *overlap_instance = j;
            }
        }
        overlaps[i].length = (overlap_pool.data + overlap_pool.fill - (u8*)overlaps[i].data) / sizeof(u16);
    }
    
    object_pc_t** object_pointers = malloc(2 * length * sizeof(object_pc_t*));
    object_pc_t** object_pointers2 = object_pointers + length;
    for(int i = 0; i < length; i++) {
        object_pointers[i] = object_list->objects + i;
    }
    level_objects_sort_left_right(object_pointers, length);
    printf("Step 2\n");
    int index = 0;
    while(index < length) {
        for(int i = 0;; i++) {
            object_pc_t* object = object_pointers[i];
            if(object == NULL) continue;

            if(overlaps[i].length == 0) {
                object_pointers2[index] = object;
                object_pointers[i] = NULL;
                u16 object_index = (object - object_list->objects) / sizeof(u16);
                for(int j = 0; j < length; j++) {
                    for(int k = 0; k < overlaps[j].length; k++) {
                        if(overlaps[j].data[k] == object_index) {
                            // if one of the objects was overlapped by the found object,
                            // forget about this by copying the last overlap over it
                            // and decrementing.
                            overlaps[j].data[k]
                                = overlaps[j].data[overlaps[j].length - 1];
                            overlaps[j].length--;
                            break;
                        }
                    }
                }
                index++;
                break;
            }
        }
    }

    free(overlaps);
    pool_deinit(&overlap_pool);
    printf("Step 3\n");

    // We now have a topological sorted array in object_pointers2. We can generate level data from it
    // by inserting necessary screen jumps.
    // TODO: Insert screen exits.
    int size = 0;
    for(int i = 0; i < length - 1; i++) {
        u8* data;
        object_pc_t* object = object_pointers2[i];
        int screen1 = object_pointers2[i]->x / 16;
        int screen2 = object_pointers2[i + 1]->x / 16;

        if(size + 3 > max_length) return size;

        if(screen1 == screen2) {
            data = output_data + size;
            data[0] = 0;
        } else if(screen1 + 1 == screen2) {
            data = output_data + size;
            data[0] = 0x80;
        } else {
            data = output_data + size;
            data[0] = screen2;
            data[1] = 0; // We can savely set this to zero. 
            data[2] = 1;
            if(size + 3 > max_length) return size;
            data += 3;
            size += 3;
        }
        size += 3;
        data[0] = (object->num >> 4) << 5;
        assert(!(object->y & (0xFFE0))); 
        data[0] |= object->y;
        data[1] = (object->num << 4);
        data[1] |= object->x;
        data[2] = object->settings;
    }

    return size;
}

void level_move_object(level_pc_t* level, int index, u16 x, u16 y, gfx_store_t* gfx_store) {
    object_list_pc_t* object_list = &level->layer1_objects;
        
    object_list->objects[index].x = x;
    object_list->objects[index].y = y;
    u8* addr = level->rom->data + ((0x0EF0F0&0x7F0000)>>1|(0x0EF0F0&0x7FFF));
    memcpy(addr, &level->header, 5);
    level_object_list_pc_to_level_data(&level->layer1_objects, addr + 5, 5000);
    printf("Finished\n");
    r65816_rom_t* rom = level->rom;
    int num_level = level->num_level;

    level->rom->data[level_layer1_data_table_pc + 2 + 3 * num_level] = 0x0E;
    level->rom->data[level_layer1_data_table_pc + 1 + 3 * num_level] = 0xF0;
    level->rom->data[level_layer1_data_table_pc + 0 + 3 * num_level] = 0xF0;

    level_deinit(level);
    level_init(level, rom, num_level, gfx_store);
}





#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

void level_object_list_render_to_layer16(object_list_pc_t* object_list, layer16_t* layer) {
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

