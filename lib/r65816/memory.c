#include "cpu.h"
#include "memory.h"

uint8_t op_read(cpu_t* cpu, uint32_t addr) {
    if(addr & 0xFF8000) {
	if(addr >= 0x7E0000 && addr < 0x800000) {
	    return cpu->ram[addr-0x7E0000];
	} else if(addr & 0x008000) {
	    return cpu->rom->banks[(addr & 0x7f0000) >> 17][addr & 0x7fff];
	} else {
	    return cpu->ram[addr & 0x007FFF];
	}
    }
    if(addr >= 0x002000) {
	if(addr <= 0x004500) {
	    return cpu->sreg[addr - 0x2000];
	} else {
	    fprintf(stderr, "Err: %06x Read: %06x", cpu->regs.pc.d, addr);
	    //getchar();
	}
    }
    return cpu->ram[addr];
}

void op_write(cpu_t* cpu, uint32_t addr, uint8_t data) {
    if(addr & 0xFF8000) {
	if(addr >= 0x7E0000 && addr < 0x800000) {
	    cpu->ram[addr-0x7E0000] = data;
	} else {
	    if(addr & 0x008000) {
		fprintf(stderr, "Err: %06x Wrote: %06x", cpu->regs.pc.d, addr);
		//getchar();
		//Todo: ERROR
	    } else {
		cpu->ram[addr & 0x007FFF] = data;
	    }
	}
    } else {
	if(addr >= 0x002000) {
	    if(addr <= 0x004500) {
		cpu->sreg[addr - 0x2000] = data;
	    } else {
		fprintf(stderr, "Err: %06x Wrote: %06x", cpu->regs.pc.d, addr);
		//getchar();
	    }
	} else {
	    cpu->ram[addr] = data;
	}
    }
}


inline uint8_t op_readpc(cpu_t* cpu) {
  return op_read(cpu, (cpu->regs.pc.b << 16) + cpu->regs.pc.w++);
}

inline uint8_t op_readstack(cpu_t* cpu) {
  cpu->regs.e ? cpu->regs.s.l++ : cpu->regs.s.w++;
  return op_read(cpu, cpu->regs.s.w);
}

inline uint8_t op_readstackn(cpu_t* cpu) {
  return op_read(cpu, ++cpu->regs.s.w);
}

inline uint8_t op_readaddr(cpu_t* cpu, uint32_t addr) {
  return op_read(cpu, addr & 0xffff);
}

inline uint8_t op_readlong(cpu_t* cpu, uint32_t addr) {
  return op_read(cpu, addr & 0xffffff);
}

inline uint8_t op_readdbr(cpu_t* cpu, uint32_t addr) {
  return op_read(cpu, ((cpu->regs.db << 16) + addr) & 0xffffff);
}

inline uint8_t op_readpbr(cpu_t* cpu, uint32_t addr) {
  return op_read(cpu, (cpu->regs.pc.b << 16) + (addr & 0xffff));
}

inline uint8_t op_readdp(cpu_t* cpu, uint32_t addr) {
  if(cpu->regs.e && cpu->regs.d.l == 0x00) {
    return op_read(cpu, (cpu->regs.d.w & 0xff00) + ((cpu->regs.d.w + (addr & 0xffff)) & 0xff));
  } else {
    return op_read(cpu, (cpu->regs.d.w + (addr & 0xffff)) & 0xffff);
  }
}

inline uint8_t op_readsp(cpu_t* cpu, uint32_t addr) {
  return op_read(cpu, (cpu->regs.s.w + (addr & 0xffff)) & 0xffff);
}

inline void op_writestack(cpu_t* cpu, uint8_t data) {
  op_write(cpu, cpu->regs.s.w, data);
  cpu->regs.e ? cpu->regs.s.l-- : cpu->regs.s.w--;
}

inline void op_writestackn(cpu_t* cpu, uint8_t data) {
  op_write(cpu, cpu->regs.s.w--, data);
}

inline void op_writeaddr(cpu_t* cpu, uint32_t addr, uint8_t data) {
  op_write(cpu, addr & 0xffff, data);
}

inline void op_writelong(cpu_t* cpu, uint32_t addr, uint8_t data) {
  op_write(cpu, addr & 0xffffff, data);
}

inline void op_writedbr(cpu_t* cpu, uint32_t addr, uint8_t data) {
  op_write(cpu, ((cpu->regs.db << 16) + addr) & 0xffffff, data);
}

inline void op_writepbr(cpu_t* cpu, uint32_t addr, uint8_t data) {
  op_write(cpu, (cpu->regs.pc.b << 16) + (addr & 0xffff), data);
}

inline void op_writedp(cpu_t* cpu, uint32_t addr, uint8_t data) {
  if(cpu->regs.e && cpu->regs.d.l == 0x00) {
    op_write(cpu, (cpu->regs.d.w & 0xff00) + ((cpu->regs.d.w + (addr & 0xffff)) & 0xff), data);
  } else {
    op_write(cpu, (cpu->regs.d.w + (addr & 0xffff)) & 0xffff, data);
  }
}

inline void op_writesp(cpu_t* cpu, uint32_t addr, uint8_t data) {
  op_write(cpu, (cpu->regs.s.w + (addr & 0xffff)) & 0xffff, data);
}
