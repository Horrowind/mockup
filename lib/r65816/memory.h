#ifndef R65816_MEMORY_H
#define R65816_MEMORY_H

#include "cpu.h"

uint8_t op_readpc(cpu_t* cpu);
uint8_t op_readstack(cpu_t* cpu);
uint8_t op_readstackn(cpu_t* cpu);
uint8_t op_readaddr(cpu_t* cpu, uint32_t addr);
uint8_t op_readlong(cpu_t* cpu, uint32_t addr);
uint8_t op_readdbr(cpu_t* cpu, uint32_t addr);
uint8_t op_readpbr(cpu_t* cpu, uint32_t addr);
uint8_t op_readdp(cpu_t* cpu, uint32_t addr);
uint8_t op_readsp(cpu_t* cpu, uint32_t addr);
void op_writestack(cpu_t* cpu, uint8_t data);
void op_writestackn(cpu_t* cpu, uint8_t data);
void op_writeaddr(cpu_t* cpu, uint32_t addr, uint8_t data);
void op_writelong(cpu_t* cpu, uint32_t addr, uint8_t data);
void op_writedbr(cpu_t* cpu, uint32_t addr, uint8_t data);
void op_writepbr(cpu_t* cpu, uint32_t addr, uint8_t data);
void op_writedp(cpu_t* cpu, uint32_t addr, uint8_t data);
void op_writesp(cpu_t* cpu, uint32_t addr, uint8_t data);

#endif //R65816_MEMORY_H
