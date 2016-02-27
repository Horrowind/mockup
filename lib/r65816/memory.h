#ifndef R65816_MEMORY_H
#define R65816_MEMORY_H

#include "cpu.h"

u8 r65816_op_readpc(r65816_cpu_t* cpu);
u8 r65816_op_readstack(r65816_cpu_t* cpu);
u8 r65816_op_readstackn(r65816_cpu_t* cpu);
u8 r65816_op_readaddr(r65816_cpu_t* cpu, u32 addr);
u8 r65816_op_readlong(r65816_cpu_t* cpu, u32 addr);
u8 r65816_op_readdbr(r65816_cpu_t* cpu, u32 addr);
u8 r65816_op_readpbr(r65816_cpu_t* cpu, u32 addr);
u8 r65816_op_readdp(r65816_cpu_t* cpu, u32 addr);
u8 r65816_op_readsp(r65816_cpu_t* cpu, u32 addr);
void r65816_op_writestack(r65816_cpu_t* cpu, u8 data);
void r65816_op_writestackn(r65816_cpu_t* cpu, u8 data);
void r65816_op_writeaddr(r65816_cpu_t* cpu, u32 addr, u8 data);
void r65816_op_writelong(r65816_cpu_t* cpu, u32 addr, u8 data);
void r65816_op_writedbr(r65816_cpu_t* cpu, u32 addr, u8 data);
void r65816_op_writepbr(r65816_cpu_t* cpu, u32 addr, u8 data);
void r65816_op_writedp(r65816_cpu_t* cpu, u32 addr, u8 data);
void r65816_op_writesp(r65816_cpu_t* cpu, u32 addr, u8 data);


#endif //R65816_MEMORY_H
