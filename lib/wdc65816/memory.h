#ifndef WDC65816_MEMORY_H
#define WDC65816_MEMORY_H

#include "cpu.h"

u8 wdc65816_op_readpc(Wdc65816Cpu* cpu);
u8 wdc65816_op_readstack(Wdc65816Cpu* cpu);
u8 wdc65816_op_readstackn(Wdc65816Cpu* cpu);
u8 wdc65816_op_readaddr(Wdc65816Cpu* cpu, u32 addr);
u8 wdc65816_op_readlong(Wdc65816Cpu* cpu, u32 addr);
u8 wdc65816_op_readdbr(Wdc65816Cpu* cpu, u32 addr);
u8 wdc65816_op_readpbr(Wdc65816Cpu* cpu, u32 addr);
u8 wdc65816_op_readdp(Wdc65816Cpu* cpu, u32 addr);
u8 wdc65816_op_readsp(Wdc65816Cpu* cpu, u32 addr);
void wdc65816_op_writestack(Wdc65816Cpu* cpu, u8 data);
void wdc65816_op_writestackn(Wdc65816Cpu* cpu, u8 data);
void wdc65816_op_writeaddr(Wdc65816Cpu* cpu, u32 addr, u8 data);
void wdc65816_op_writelong(Wdc65816Cpu* cpu, u32 addr, u8 data);
void wdc65816_op_writedbr(Wdc65816Cpu* cpu, u32 addr, u8 data);
void wdc65816_op_writepbr(Wdc65816Cpu* cpu, u32 addr, u8 data);
void wdc65816_op_writedp(Wdc65816Cpu* cpu, u32 addr, u8 data);
void wdc65816_op_writesp(Wdc65816Cpu* cpu, u32 addr, u8 data);


#endif //WDC65816_MEMORY_H
