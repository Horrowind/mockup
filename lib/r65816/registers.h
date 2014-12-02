#ifndef LUBMOCKUP_REGISTERS_H
#define LUBMOCKUP_REGISTERS_H

#include "stdint.h"

struct r65816_reg24 {
  union {
    uint32_t d;
    struct { uint16_t w, wh; };
    struct { uint8_t  l, h, b, bh; };
  };
};
typedef struct r65816_reg24 r65816_reg24_t;

struct r65816_reg16 {
    union {
        uint16_t w;
        uint8_t l, h;
    };
};
typedef struct r65816_reg16 r65816_reg16_t;
struct r65816_regs {
    r65816_reg24_t pc;
    union {
        r65816_reg16_t r[6];
        r65816_reg16_t a, x, y, z, s, d;
    };
    union {
        struct { uint8_t n:1, v:1, m:1, x:1, d:1, i:1, z:1, c:1; };
        uint8_t b;
    } p;
    uint8_t db;
    uint8_t e : 1;
    
    uint8_t mdr;      //memory data register
    //uint16_t vector;  //interrupt vector address
};
typedef struct r65816_regs r65816_regs_t;


#endif //LUBMOCKUP_REGISTERS_H
