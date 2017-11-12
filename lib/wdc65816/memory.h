#ifndef R65816_MEMORY_H
#define R65816_MEMORY_H

#include "cpu.h"

u8 wdc65816_op_readpc(wdc65816_cpu_t* cpu);
u8 wdc65816_op_readstack(wdc65816_cpu_t* cpu);
u8 wdc65816_op_readstackn(wdc65816_cpu_t* cpu);
u8 wdc65816_op_readaddr(wdc65816_cpu_t* cpu, u32 addr);
u8 wdc65816_op_readlong(wdc65816_cpu_t* cpu, u32 addr);
u8 wdc65816_op_readdbr(wdc65816_cpu_t* cpu, u32 addr);
u8 wdc65816_op_readpbr(wdc65816_cpu_t* cpu, u32 addr);
u8 wdc65816_op_readdp(wdc65816_cpu_t* cpu, u32 addr);
u8 wdc65816_op_readsp(wdc65816_cpu_t* cpu, u32 addr);
void wdc65816_op_writestack(wdc65816_cpu_t* cpu, u8 data);
void wdc65816_op_writestackn(wdc65816_cpu_t* cpu, u8 data);
void wdc65816_op_writeaddr(wdc65816_cpu_t* cpu, u32 addr, u8 data);
void wdc65816_op_writelong(wdc65816_cpu_t* cpu, u32 addr, u8 data);
void wdc65816_op_writedbr(wdc65816_cpu_t* cpu, u32 addr, u8 data);
void wdc65816_op_writepbr(wdc65816_cpu_t* cpu, u32 addr, u8 data);
void wdc65816_op_writedp(wdc65816_cpu_t* cpu, u32 addr, u8 data);
void wdc65816_op_writesp(wdc65816_cpu_t* cpu, u32 addr, u8 data);


#endif //R65816_MEMORY_H
