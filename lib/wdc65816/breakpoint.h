#ifndef WDC65816_BREAKPOINT_H
#define WDC65816_BREAKPOINT_H

#include "base/base.h"

#define BP_MAX_LENGTH 5

typedef struct {
    u32 low;
    u32 high;
} Wdc65816Breakpoint;

typedef struct {
    int length;
    Wdc65816Breakpoint address[BP_MAX_LENGTH];
//    wdc65816_breakpoint_t* address;
} Wdc65816BreakpointList;


void wdc65816_breakpoint_list_init(Wdc65816BreakpointList* brk);
b32 wdc65816_breakpoint_list_is_hit(Wdc65816BreakpointList* brk, u32 address);
void wdc65816_breakpoint_list_add(Wdc65816BreakpointList* brk, u32 address);
void wdc65816_breakpoint_list_add_range(Wdc65816BreakpointList* brk, u32 address_low, u32 address_high);
void wdc65816_breakpoint_list_deinit(Wdc65816BreakpointList* brk);

#endif //WDC65816_BREAKPOINT_H
