#ifndef LIBR65816_BREAKPOINT_H
#define LIBR65816_BREAKPOINT_H

typedef struct breakpoint {
    struct breakpoint* next;
    int address;
    uint8_t read  : 1;
    uint8_t write : 1;
    uint8_t exec  : 1;
} breakpoint_t;


#endif //LIBR65816_BREAKPOINT_H
