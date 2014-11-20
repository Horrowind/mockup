#ifndef MOCKUP_CPU_H
#define MOCKUP_CPU_H

#include "stdio.h"
#include "string.h"

#include "rom.h"

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

struct r65816_cpu {
    r65816_rom_t* rom;
    uint8_t* ram;
    uint8_t* sreg;


    r65816_regs_t regs;
    r65816_reg24_t aa, rd;
    uint8_t sp, dp;
    
    void (**opcode_table)(struct r65816_cpu*);
    void (*op_table[5 * 256])(struct r65816_cpu*);

};
typedef struct r65816_cpu r65816_cpu_t;

void r65816_cpu_step(r65816_cpu_t* cpu);
void r65816_cpu_init(r65816_cpu_t* cpu, r65816_rom_t* rom);
void r65816_cpu_show_state(r65816_cpu_t* cpu, char ouput[256]);
void r65816_cpu_disassemble_opcode(r65816_cpu_t* cpu, char* output, uint32_t addr);
void r65816_op_write(r65816_cpu_t* cpu, uint32_t addr, uint8_t data);
uint8_t r65816_op_read(r65816_cpu_t* cpu, uint32_t addr);
#endif //MOCKUP_CPU_H
