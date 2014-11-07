#ifndef MOCKUP_CPU_H
#define MOCKUP_CPU_H

#include "stdio.h"
#include "string.h"

#include "rom.h"

struct regs {
    struct {
        uint32_t value : 24;
    } pc;
    union {
        uint16_t w;
        uint8_t l, h;
    } a, x, y, z, s, d;
    union {
        struct { uint8_t n:1, v:1, m:1, x:1, d:1, i:1, z:1, c:1; } flags;
        uint8_t b;
    };
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
    struct {
        uint32_t value : 24;
    } aa, rd;
    uint8_t sp, dp;
    

};
typedef struct cpu cpu_t;

void cpu_step(cpu_t* cpu);
void cpu_init(cpu_t* cpu, rom_t* rom);
void cpu_show_state(cpu_t* cpu, char ouput[256]);


#endif //MOCKUP_CPU_H
