#ifndef LIBR65816_BREAKPOINT_H
#define LIBR65816_BREAKPOINT_H

#include "stdint.h"
#include "stdlib.h"

typedef struct r65816_breakpoint {
    int length;
    uint32_t* address;
} r65816_breakpoint_t;


void r65816_breakpoint_init(r65816_breakpoint_t* brk);
uint8_t r65816_breakpoint_is_hit(r65816_breakpoint_t brk, uint32_t address);
void r65816_breakpoint_add(r65816_breakpoint_t* brk, uint32_t address);
void r65816_breakpoint_deinit(r65816_breakpoint_t* brk);

#endif //LIBR65816_BREAKPOINT_H
