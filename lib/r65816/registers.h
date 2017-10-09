#ifndef LIBMOCKUP_REGISTERS_H
#define LIBMOCKUP_REGISTERS_H

struct r65816_reg24 {
  union {
    u32 d;
    struct { u16 w, wh; };
    struct { u8  l, h, b, bh; };
  };
};
typedef struct r65816_reg24 r65816_reg24_t;

struct r65816_reg16 {
    union {
        u16 w;
        struct {
            u8 l, h;
        };
    };
};
typedef struct r65816_reg16 r65816_reg16_t;
struct r65816_regs {
    r65816_reg24_t pc;
    union {
        r65816_reg16_t r[6];
        struct {
            r65816_reg16_t a, x, y, z, s, d;
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
};
typedef struct r65816_regs r65816_regs_t;


#endif //LIBMOCKUP_REGISTERS_H
