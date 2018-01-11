#ifndef WDC65816_REGISTERS_H
#define WDC65816_REGISTERS_H

#include "base/base.h"

typedef struct {
  union {
    u32 d;
    struct { u16 w, wh; };
    struct { u8  l, h, b, bh; };
  };
} Wdc65816Reg24;

typedef struct {
    union {
        u16 w;
        struct {
            u8 l, h;
        };
    };
} Wdc65816Reg16;
typedef struct {
    Wdc65816Reg24 pc;
    union {
        Wdc65816Reg16 r[6];
        struct {
            Wdc65816Reg16 a, x, y, z, s, d;
        };
    };
    u8 db;
    
    u8 mdr;      //memory data register
    //u16 vector;  //interrupt vector address
    u8 e : 1;
    union {
        struct { u8 c:1, z:1, i:1, d:1, x:1, m:1, v:1, n:1; };
        u8 b;
    } p;
} Wdc65816Regs;


#endif //WDC65816_REGISTERS_H
