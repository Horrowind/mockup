#include <stdio.h>

#include "object.h"
#include "r65816/cpu.h"

void object_list_init_level_data(object_list_pc_t* object_list, r65816_rom_t* rom, uint8_t* data) {
    /* object_list->length = object_list_level_data_length(object_list, data); */
    /* object_list->objects = malloc(object_list->length); */
    /* int index = 0; */
    
    /* uint8_t screen = 0; */
    /* uint8_t c1, c2, c3; */
    /* r65816_cpu_t cpu; */
    /* r65816_cpu_init(&cpu, rom); */
    /* r65816_cpu_add_exec_bp(0x0583D2); */
    /* r65816_cpu_add_exec_bp(0x0583B8); */
    /* r65816_breakpoint_add_range(cpu.breakpoints_write, 0xC800, 0xFFFF); */
    /* r65816_breakpoint_add_range(cpu.breakpoints_write, 0x1C800, 0x1FFFF); */
    /* while((c1 = *data) != 0xFF) { */
    /*     c2 = *(data + 1); */
    /*     c3 = *(data + 2); */
    /*     if(output & 0x80) { */
    /*         screen++;                */
    /*     } */
    /*     uint8_t obj = ((c1 & 0x60) >> 1) | (c2 >> 4); */
    /*     uint8_t x = c2 & 0x0F; */
    /*     uint8_t y = c1 & 0x1F; */
    /*     if(obj == 0) { */
    /*         if(c3 != 0) { */
    /*             if(c3 == 1) { */
    /*                 screen = c1; */
    /*             } else { */
    /*                 r65816_cpu_clear(&cpu); */

    /*                 for(int i = 0; i < 0x14 * 27 * 16; i++) { */
    /*                     cpu.ram[0x0C800 + i] = 0x25; */
    /*                     cpu.ram[0x1C800 + i] = 0; */
    /*                 } */
                    
                    
    /*                 // create a new level at 7FA000, which consists only of the considered object */
    /*                 cpu.ram[0x1A000] = screen; // 1st byte of screen jump */
    /*                 cpu.ram[0x1A001] = 0x00;   // 2nd byte of screen jump */
    /*                 cpu.ram[0x1A002] = 0x01;   // 3rd byte of screen jump */
    /*                 cpu.ram[0x1A003] = c1;     // 1st byte of the object itself */
    /*                 cpu.ram[0x1A004] = c2;     // 2nd byte of the object itself */
    /*                 cpu.ram[0x1A005] = c3;     // 3rd byte of the object itself */
    /*                 cpu.ram[0x1A006] = 0xFF;   // end byte */

    /*                 // set up level data pointer to this fake level. */
    /*                 cpu.ram[0x65] = 0x7F; */
    /*                 cpu.ram[0x66] = 0xA0; */
    /*                 cpu.ram[0x67] = 0x00; */

    /*                 while(1) { */
    /*                     cpu.run(0x0583AC); */
    /*                     if(cpu.regs.d == 0x583B8) break; */
    /*                 } */
    /*             } */
    /*             data += 3; */
    /*         } else { */
    /*             data += 4; */
    /*         } */
    /*     } else { */
            
    /*         data += 3; */
    /*     } */
    /* } */
    /* void r65816_cpu_free(&cpu, rom); */
}

int object_list_level_data_length(object_list_pc_t* object_list) {
    int length = 0;
    /* uint8_t c1, c2, c3; */
    /* while((c1 = *data) != 0xFF) { */
    /*     c2 = *(data + 1); */
    /*     c3 = *(data + 2); */
    /*     uint8_t obj = (c1 & 0x60) | c2; // We just check later if obj != 0, so this is not accurate for performance reasons. */
    /*     if(obj == 0) { */
    /*         if(c3 != 0) { */
    /*             if(c3 != 1) { */
    /*                 length++; */
    /*             } */
    /*             data += 3; */
    /*         } else { */
    /*             data += 4; */
    /*         } */
    /*     } else { */
    /*         length++; */
    /*         data += 3; */
    /*     } */
    /* } */
    return length;
}

void object_list_deinit_addr(object_list_pc_t* object_list) {
    free(object_list->objects);
}

void object_list_pc_to_level_data(object_list_pc_t* object_list, uint8_t* data);

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
