#ifndef LIBR65816_BREAKPOINT_H
#define LIBR65816_BREAKPOINT_H

#include "stdint.h"
#include "stdlib.h"

typedef struct {
    uint32_t low;
    uint32_t high;
} r65816_breakpoint_t;

typedef struct {
    int length;
    r65816_breakpoint_t* address;
} r65816_breakpoint_list_t;


void r65816_breakpoint_list_init(r65816_breakpoint_list_t* brk);
uint8_t r65816_breakpoint_list_is_hit(r65816_breakpoint_list_t brk, uint32_t address);
void r65816_breakpoint_list_add(r65816_breakpoint_list_t* brk, uint32_t address);
void r65816_breakpoint_list_add_range(r65816_breakpoint_list_t* brk, uint32_t address_low, uint32_t address_high);
void r65816_breakpoint_list_deinit(r65816_breakpoint_list_t* brk);

#endif //LIBR65816_BREAKPOINT_H
