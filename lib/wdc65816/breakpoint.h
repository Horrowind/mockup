#ifndef LIBR65816_BREAKPOINT_H
#define LIBR65816_BREAKPOINT_H

#include <stdlib.h>
#include "base/base.h"

#define BP_MAX_LENGTH 5

typedef struct {
    u32 low;
    u32 high;
} wdc65816_breakpoint_t;

typedef struct {
    int length;
    wdc65816_breakpoint_t address[BP_MAX_LENGTH];
//    wdc65816_breakpoint_t* address;
} wdc65816_breakpoint_list_t;


void wdc65816_breakpoint_list_init(wdc65816_breakpoint_list_t* brk);
b32 wdc65816_breakpoint_list_is_hit(wdc65816_breakpoint_list_t* brk, u32 address);
void wdc65816_breakpoint_list_add(wdc65816_breakpoint_list_t* brk, u32 address);
void wdc65816_breakpoint_list_add_range(wdc65816_breakpoint_list_t* brk, u32 address_low, u32 address_high);
void wdc65816_breakpoint_list_deinit(wdc65816_breakpoint_list_t* brk);

#endif //LIBR65816_BREAKPOINT_H
