#include "cpu.h"
#include "memory.h"


inline u8 wdc65816_op_readpc(wdc65816_cpu_t* cpu) {
    return wdc65816_cpu_read(cpu, cpu->regs.pc.d++);
}

inline u8 wdc65816_op_readstack(wdc65816_cpu_t* cpu) {
    cpu->regs.e ? cpu->regs.s.l++ : cpu->regs.s.w++;
    return wdc65816_cpu_read(cpu, cpu->regs.s.w);
}

inline u8 wdc65816_op_readstackn(wdc65816_cpu_t* cpu) {
    return wdc65816_cpu_read(cpu, ++cpu->regs.s.w);
}

inline u8 wdc65816_op_readaddr(wdc65816_cpu_t* cpu, u32 addr) {
    return wdc65816_cpu_read(cpu, addr & 0xffff);
}

inline u8 wdc65816_op_readlong(wdc65816_cpu_t* cpu, u32 addr) {
    return wdc65816_cpu_read(cpu, addr & 0xffffff);
}

inline u8 wdc65816_op_readdbr(wdc65816_cpu_t* cpu, u32 addr) {
    return wdc65816_cpu_read(cpu, ((cpu->regs.db << 16) + addr) & 0xffffff);
}

inline u8 wdc65816_op_readpbr(wdc65816_cpu_t* cpu, u32 addr) {
    return wdc65816_cpu_read(cpu, (cpu->regs.pc.b << 16) + (addr & 0xffff));
}

inline u8 wdc65816_op_readdp(wdc65816_cpu_t* cpu, u32 addr) {
    if(cpu->regs.e && cpu->regs.d.l == 0x00) {
        return wdc65816_cpu_read(cpu, (cpu->regs.d.w & 0xff00) + ((cpu->regs.d.w + (addr & 0xffff)) & 0xff));
    } else {
        return wdc65816_cpu_read(cpu, (cpu->regs.d.w + (addr & 0xffff)) & 0xffff);
    }
}

inline u8 wdc65816_op_readsp(wdc65816_cpu_t* cpu, u32 addr) {
    return wdc65816_cpu_read(cpu, (cpu->regs.s.w + (addr & 0xffff)) & 0xffff);
}

inline void wdc65816_op_writestack(wdc65816_cpu_t* cpu, u8 data) {
    wdc65816_cpu_write(cpu, cpu->regs.s.w, data);
    cpu->regs.e ? cpu->regs.s.l-- : cpu->regs.s.w--;
}

inline void wdc65816_op_writestackn(wdc65816_cpu_t* cpu, u8 data) {
    wdc65816_cpu_write(cpu, cpu->regs.s.w--, data);
}

inline void wdc65816_op_writeaddr(wdc65816_cpu_t* cpu, u32 addr, u8 data) {
    wdc65816_cpu_write(cpu, addr & 0xffff, data);
}

inline void wdc65816_op_writelong(wdc65816_cpu_t* cpu, u32 addr, u8 data) {
    wdc65816_cpu_write(cpu, addr & 0xffffff, data);
}

inline void wdc65816_op_writedbr(wdc65816_cpu_t* cpu, u32 addr, u8 data) {
    wdc65816_cpu_write(cpu, ((cpu->regs.db << 16) + addr) & 0xffffff, data);
}

inline void wdc65816_op_writepbr(wdc65816_cpu_t* cpu, u32 addr, u8 data) {
    wdc65816_cpu_write(cpu, (cpu->regs.pc.b << 16) + (addr & 0xffff), data);
}

inline void wdc65816_op_writedp(wdc65816_cpu_t* cpu, u32 addr, u8 data) {
    if(cpu->regs.e && cpu->regs.d.l == 0x00) {
        wdc65816_cpu_write(cpu, (cpu->regs.d.w & 0xff00) + ((cpu->regs.d.w + (addr & 0xffff)) & 0xff), data);
    } else {
        wdc65816_cpu_write(cpu, (cpu->regs.d.w + (addr & 0xffff)) & 0xffff, data);
    }
}

inline void wdc65816_op_writesp(wdc65816_cpu_t* cpu, u32 addr, u8 data) {
    wdc65816_cpu_write(cpu, (cpu->regs.s.w + (addr & 0xffff)) & 0xffff, data);
}
