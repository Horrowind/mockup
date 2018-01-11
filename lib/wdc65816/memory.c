#include "cpu.h"
#include "memory.h"


inline u8 wdc65816_op_readpc(Wdc65816Cpu* cpu) {
    return wdc65816_cpu_read(cpu, cpu->regs.pc.d++);
}

inline u8 wdc65816_op_readstack(Wdc65816Cpu* cpu) {
    cpu->regs.e ? cpu->regs.s.l++ : cpu->regs.s.w++;
    return wdc65816_cpu_read(cpu, cpu->regs.s.w);
}

inline u8 wdc65816_op_readstackn(Wdc65816Cpu* cpu) {
    return wdc65816_cpu_read(cpu, ++cpu->regs.s.w);
}

inline u8 wdc65816_op_readaddr(Wdc65816Cpu* cpu, u32 addr) {
    return wdc65816_cpu_read(cpu, addr & 0xffff);
}

inline u8 wdc65816_op_readlong(Wdc65816Cpu* cpu, u32 addr) {
    return wdc65816_cpu_read(cpu, addr & 0xffffff);
}

inline u8 wdc65816_op_readdbr(Wdc65816Cpu* cpu, u32 addr) {
    return wdc65816_cpu_read(cpu, ((cpu->regs.db << 16) + addr) & 0xffffff);
}

inline u8 wdc65816_op_readpbr(Wdc65816Cpu* cpu, u32 addr) {
    return wdc65816_cpu_read(cpu, (cpu->regs.pc.b << 16) + (addr & 0xffff));
}

inline u8 wdc65816_op_readdp(Wdc65816Cpu* cpu, u32 addr) {
    if(cpu->regs.e && cpu->regs.d.l == 0x00) {
        return wdc65816_cpu_read(cpu, (cpu->regs.d.w & 0xff00) + ((cpu->regs.d.w + (addr & 0xffff)) & 0xff));
    } else {
        return wdc65816_cpu_read(cpu, (cpu->regs.d.w + (addr & 0xffff)) & 0xffff);
    }
}

inline u8 wdc65816_op_readsp(Wdc65816Cpu* cpu, u32 addr) {
    return wdc65816_cpu_read(cpu, (cpu->regs.s.w + (addr & 0xffff)) & 0xffff);
}

inline void wdc65816_op_writestack(Wdc65816Cpu* cpu, u8 data) {
    wdc65816_cpu_write(cpu, cpu->regs.s.w, data);
    cpu->regs.e ? cpu->regs.s.l-- : cpu->regs.s.w--;
}

inline void wdc65816_op_writestackn(Wdc65816Cpu* cpu, u8 data) {
    wdc65816_cpu_write(cpu, cpu->regs.s.w--, data);
}

inline void wdc65816_op_writeaddr(Wdc65816Cpu* cpu, u32 addr, u8 data) {
    wdc65816_cpu_write(cpu, addr & 0xffff, data);
}

inline void wdc65816_op_writelong(Wdc65816Cpu* cpu, u32 addr, u8 data) {
    wdc65816_cpu_write(cpu, addr & 0xffffff, data);
}

inline void wdc65816_op_writedbr(Wdc65816Cpu* cpu, u32 addr, u8 data) {
    wdc65816_cpu_write(cpu, ((cpu->regs.db << 16) + addr) & 0xffffff, data);
}

inline void wdc65816_op_writepbr(Wdc65816Cpu* cpu, u32 addr, u8 data) {
    wdc65816_cpu_write(cpu, (cpu->regs.pc.b << 16) + (addr & 0xffff), data);
}

inline void wdc65816_op_writedp(Wdc65816Cpu* cpu, u32 addr, u8 data) {
    if(cpu->regs.e && cpu->regs.d.l == 0x00) {
        wdc65816_cpu_write(cpu, (cpu->regs.d.w & 0xff00) + ((cpu->regs.d.w + (addr & 0xffff)) & 0xff), data);
    } else {
        wdc65816_cpu_write(cpu, (cpu->regs.d.w + (addr & 0xffff)) & 0xffff, data);
    }
}

inline void wdc65816_op_writesp(Wdc65816Cpu* cpu, u32 addr, u8 data) {
    wdc65816_cpu_write(cpu, (cpu->regs.s.w + (addr & 0xffff)) & 0xffff, data);
}
