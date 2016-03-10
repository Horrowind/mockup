#ifndef LIBR65816_BREAKPOINT_H
#define LIBR65816_BREAKPOINT_H

#include <stdlib.h>
#include "datasizes.h"

#define BP_MAX_LENGTH 5

typedef struct {
    u32 low;
    u32 high;
} r65816_breakpoint_t;

typedef struct {
    int length;
    r65816_breakpoint_t address[BP_MAX_LENGTH];
//    r65816_breakpoint_t* address;
} r65816_breakpoint_list_t;


void r65816_breakpoint_list_init(r65816_breakpoint_list_t* brk);
b32 r65816_breakpoint_list_is_hit(r65816_breakpoint_list_t* brk, u32 address);
void r65816_breakpoint_list_add(r65816_breakpoint_list_t* brk, u32 address);
void r65816_breakpoint_list_add_range(r65816_breakpoint_list_t* brk, u32 address_low, u32 address_high);
void r65816_breakpoint_list_deinit(r65816_breakpoint_list_t* brk);

#endif //LIBR65816_BREAKPOINT_H
