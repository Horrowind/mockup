#include "stdint.h"
#include "stdlib.h"

#include "breakpoint.h"
#include "stdio.h"

//TODO: Implement a better allocation strategy.
void r65816_breakpoint_list_init(r65816_breakpoint_list_t* brk) {
    brk->length = 0;
    brk->address = malloc(1);
}


uint8_t r65816_breakpoint_list_is_hit(r65816_breakpoint_list_t brk, uint32_t address) {
    for(int i = 0; i < brk.length; i++) {
        if(brk.address[i].low <= address && brk.address[i].high >= address) return 1; 
    } 
    return 0;
}

void r65816_breakpoint_list_add(r65816_breakpoint_list_t* brk, uint32_t address) {
    brk->address = realloc(brk->address, (brk->length + 1) * sizeof(r65816_breakpoint_t));
    brk->address[brk->length].low = address;
    brk->address[brk->length].high = address;
    brk->length += 1;
}

void r65816_breakpoint_list_add_range(r65816_breakpoint_list_t* brk, uint32_t address_low, uint32_t address_high) {
    brk->address = realloc(brk->address, (brk->length + 1) * sizeof(r65816_breakpoint_t));
    brk->address[brk->length].low = address_low;
    brk->address[brk->length].high = address_high;
    brk->length += 1;
}

void r65816_breakpoint_list_deinit(r65816_breakpoint_list_t* brk) {
    free(brk->address);
    brk->address = NULL;
    brk->length = 0;
}
