#include "stdint.h"
#include "stdlib.h"

#include "breakpoint.h"

void r65816_breakpoint_init(r65816_breakpoint_t* brk) {
    brk->length = 0;
    brk->address = NULL;
}


uint8_t r65816_breakpoint_is_hit(r65816_breakpoint_t* brk, uint32_t address) {
    for(int i = 0; i < brk->length; i++) {
        if(brk->address[i] == address) return 1; 
    } 
    return 0;
}

void r65816_breakpoint_add(r65816_breakpoint_t* brk, uint32_t address) {
    realloc(brk->address, (brk->length + 1) * sizeof(uint32_t));
    brk->address[++brk->length] = address;
}

void r65816_breakpoint_deinit(r65816_breakpoint_t* brk) {
    free(brk->address);
    brk->address = NULL;
    brk->length = 0;
}
