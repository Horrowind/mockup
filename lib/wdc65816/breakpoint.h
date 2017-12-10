#ifndef LIBR65816_BREAKPOINT_H
#define LIBR65816_BREAKPOINT_H

#include "base/base.h"

#define BP_MAX_LENGTH 5

typedef struct {
    u32 low;
    u32 high;
} WDC65816Breakpoint;

typedef struct {
    int length;
    WDC65816Breakpoint address[BP_MAX_LENGTH];
//    wdc65816_breakpoint_t* address;
} WDC65816BreakpointList;


void wdc65816_breakpoint_list_init(WDC65816BreakpointList* brk);
b32 wdc65816_breakpoint_list_is_hit(WDC65816BreakpointList* brk, u32 address);
void wdc65816_breakpoint_list_add(WDC65816BreakpointList* brk, u32 address);
void wdc65816_breakpoint_list_add_range(WDC65816BreakpointList* brk, u32 address_low, u32 address_high);
void wdc65816_breakpoint_list_deinit(WDC65816BreakpointList* brk);

#endif //LIBR65816_BREAKPOINT_H
