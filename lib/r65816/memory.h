#ifndef R65816_MEMORY_H
#define R65816_MEMORY_H

#include "cpu.h"

<<<<<<< HEAD
uint8_t op_read(cpu_t* cpu, uint32_t addr);
void op_write(cpu_t* cpu, uint32_t addr, uint8_t data);

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
=======
uint8_t r65816_op_readpc(r65816_cpu_t* cpu);
uint8_t r65816_op_readstack(r65816_cpu_t* cpu);
uint8_t r65816_op_readstackn(r65816_cpu_t* cpu);
uint8_t r65816_op_readaddr(r65816_cpu_t* cpu, uint32_t addr);
uint8_t r65816_op_readlong(r65816_cpu_t* cpu, uint32_t addr);
uint8_t r65816_op_readdbr(r65816_cpu_t* cpu, uint32_t addr);
uint8_t r65816_op_readpbr(r65816_cpu_t* cpu, uint32_t addr);
uint8_t r65816_op_readdp(r65816_cpu_t* cpu, uint32_t addr);
uint8_t r65816_op_readsp(r65816_cpu_t* cpu, uint32_t addr);
void r65816_op_writestack(r65816_cpu_t* cpu, uint8_t data);
void r65816_op_writestackn(r65816_cpu_t* cpu, uint8_t data);
void r65816_op_writeaddr(r65816_cpu_t* cpu, uint32_t addr, uint8_t data);
void r65816_op_writelong(r65816_cpu_t* cpu, uint32_t addr, uint8_t data);
void r65816_op_writedbr(r65816_cpu_t* cpu, uint32_t addr, uint8_t data);
void r65816_op_writepbr(r65816_cpu_t* cpu, uint32_t addr, uint8_t data);
void r65816_op_writedp(r65816_cpu_t* cpu, uint32_t addr, uint8_t data);
void r65816_op_writesp(r65816_cpu_t* cpu, uint32_t addr, uint8_t data);
>>>>>>> e573f9e01bdb380e20c29036409f287f8f93efe9

#endif //R65816_MEMORY_H
