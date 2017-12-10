#ifndef R65816_MEMORY_H
#define R65816_MEMORY_H

#include "cpu.h"

u8 wdc65816_op_readpc(WDC65816Cpu* cpu);
u8 wdc65816_op_readstack(WDC65816Cpu* cpu);
u8 wdc65816_op_readstackn(WDC65816Cpu* cpu);
u8 wdc65816_op_readaddr(WDC65816Cpu* cpu, u32 addr);
u8 wdc65816_op_readlong(WDC65816Cpu* cpu, u32 addr);
u8 wdc65816_op_readdbr(WDC65816Cpu* cpu, u32 addr);
u8 wdc65816_op_readpbr(WDC65816Cpu* cpu, u32 addr);
u8 wdc65816_op_readdp(WDC65816Cpu* cpu, u32 addr);
u8 wdc65816_op_readsp(WDC65816Cpu* cpu, u32 addr);
void wdc65816_op_writestack(WDC65816Cpu* cpu, u8 data);
void wdc65816_op_writestackn(WDC65816Cpu* cpu, u8 data);
void wdc65816_op_writeaddr(WDC65816Cpu* cpu, u32 addr, u8 data);
void wdc65816_op_writelong(WDC65816Cpu* cpu, u32 addr, u8 data);
void wdc65816_op_writedbr(WDC65816Cpu* cpu, u32 addr, u8 data);
void wdc65816_op_writepbr(WDC65816Cpu* cpu, u32 addr, u8 data);
void wdc65816_op_writedp(WDC65816Cpu* cpu, u32 addr, u8 data);
void wdc65816_op_writesp(WDC65816Cpu* cpu, u32 addr, u8 data);


#endif //R65816_MEMORY_H
