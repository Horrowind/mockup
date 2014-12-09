#ifndef LIBR65816_BREAKPOINT_H
#define LIBR65816_BREAKPOINT_H

#include "stdint.h"
#include "stdlib.h"

typedef struct r65816_breakpoint {
    struct r65816_breakpoint* next;
    uint32_t address;
} r65816_breakpoint_t;


uint8_t r65816_breakpoint_is_hit(r65816_breakpoint_t* head, uint32_t address) {
    while(head != 0) {
        if(head->address == address) return 1; 
        head = head->next;
    } 
    return 0;
}

void r65816_breakpoint_add(r65816_breakpoint_t** head_ptr, uint32_t address) {
    r65816_breakpoint_t* bp = malloc(sizeof(r65816_breakpoint_t));
    bp->address = address;
    bp->next = *head_ptr;
    *head_ptr = bp;
}

void r65816_breakpoint_clear(r65816_breakpoint_t** head_ptr) {
    

}


#endif //LIBR65816_BREAKPOINT_H
