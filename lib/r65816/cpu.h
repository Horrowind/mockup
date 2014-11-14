#ifndef MOCKUP_CPU_H
#define MOCKUP_CPU_H

#include "stdio.h"
#include "string.h"

#include "rom.h"

struct reg24 {
  union {
    uint32_t d;
    struct { uint16_t w, wh; };
    struct { uint8_t  l, h, b, bh; };
  };
};
typedef struct reg24 reg24_t;

struct reg16 {
    union {
        uint16_t w;
        uint8_t l, h;
    };
};
typedef struct reg16 reg16_t;
struct regs {
    reg24_t pc;
    union {
        reg16_t r[6];
        reg16_t a, x, y, z, s, d;
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
typedef struct regs regs_t;

struct cpu {
    rom_t* rom;
    uint8_t* ram;
    uint8_t* sreg;


    regs_t regs;
    reg24_t aa, rd;
    uint8_t sp, dp;
    
    void (**opcode_table)(struct cpu*);
    void (*op_table[5 * 256])(struct cpu*);

};
typedef struct cpu cpu_t;

void cpu_step(cpu_t* cpu);
void cpu_init(cpu_t* cpu, rom_t* rom);
void cpu_show_state(cpu_t* cpu, char ouput[256]);
void cpu_disassemble_opcode(cpu_t* cpu, char* output, uint32_t addr);
void op_write(cpu_t* cpu, uint32_t addr, uint8_t data);
uint8_t op_read(cpu_t* cpu, uint32_t addr);
#endif //MOCKUP_CPU_H
