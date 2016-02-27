#include "stdlib.h"

#include "breakpoint.h"
#include "stdio.h"

//TODO: Implement a better allocation strategy.
void r65816_breakpoint_list_init(r65816_breakpoint_list_t* brk) {
    brk->length = 0;

    /* brk->address[0].low = 1; */
    /* brk->address[0].high = 0; */
    /* brk->address[1].low = 1; */
    /* brk->address[1].high = 0; */
    /* brk->address[2].low = 1; */
    /* brk->address[2].high = 0; */
    /* brk->address[3].low = 1; */
    /* brk->address[3].high = 0; */
    /* brk->address[4].low = 1; */
    /* brk->address[4].high = 0; */

//    brk->address = malloc(1);
}


b32 r65816_breakpoint_list_is_hit(r65816_breakpoint_list_t* brk, u32 address) {
//    for(int i = 0; i < BP_MAX_LENGTH; i++) {
    for(int i = 0; i < brk->length && brk->address[i].low <= address; i++) {
      if(brk->address[i].high >= address) return 1; 
    } 
    /* if((brk.address[0].low <= address && brk.address[0].high >= address) */
    /*    || (brk.address[1].low <= address && brk.address[1].high >= address) */
    /*    || (brk.address[2].low <= address && brk.address[2].high >= address) */
    /*    || (brk.address[3].low <= address && brk.address[3].high >= address) */
    /*    || (brk.address[4].low <= address && brk.address[4].high >= address) */
    /*     ) return 1; */ 
    return 0;
}

void r65816_breakpoint_list_add(r65816_breakpoint_list_t* brk, u32 address) {
//    brk->address = realloc(brk->address, (brk->length + 1) * sizeof(r65816_breakpoint_t));
    if(brk->length == 0) {
        brk->address[0].low = address;
        brk->address[0].high = address;
        brk->length = 1;
    } else {
        for(int i = 0; i < brk->length; i++) {
            if(brk->address[i].low > address) {
                for(int j = brk->length; j > i; j--) {
                    brk->address[j].low = brk->address[j - 1].low;
                    brk->address[j].high = brk->address[j - 1].high;
                }
                brk->address[i].low = address;
                brk->address[i].high = address;
                brk->length += 1;
                return;
            }
        }
        brk->address[brk->length].low = address;
        brk->address[brk->length].high = address;
        brk->length += 1;
    }
}

void r65816_breakpoint_list_add_range(r65816_breakpoint_list_t* brk, u32 address_low, u32 address_high) {
//    brk->address = realloc(brk->address, (brk->length + 1) * sizeof(r65816_breakpoint_t));
    if(brk->length == 0) {
        brk->address[0].low = address_low;
        brk->address[0].high = address_high;
        brk->length = 1;
    } else {
        for(int i = 0; i < brk->length; i++) {
            if(brk->address[i].low > address_low) {
                for(int j = brk->length; j > i; j--) {
                    brk->address[j].low = brk->address[j - 1].low;
                    brk->address[j].high = brk->address[j - 1].high;
                }
                brk->address[i].low = address_low;
                brk->address[i].high = address_high;
                brk->length += 1;
                return;
            }
        }
        brk->address[brk->length].low = address_low;
        brk->address[brk->length].high = address_high;
        brk->length += 1;
    }
}

void r65816_breakpoint_list_deinit(r65816_breakpoint_list_t* brk) {
//    free(brk->address);
//    brk->address = NULL;
    brk->length = 0;
}
