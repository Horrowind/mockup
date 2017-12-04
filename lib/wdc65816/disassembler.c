#include "memory.h"
#include "disassembler.h"

u8 wdc65816_dreadb(wdc65816_cpu_t* cpu, u32 addr) {
  if((addr & 0x40ffff) >= 0x2000 && (addr & 0x40ffff) <= 0x5fff) {
    //$[00-3f|80-bf]:[2000-5fff]
    //do not read MMIO registers within debugger
    return 0x00;
  }
  return wdc65816_cpu_read(cpu, addr);
}

u16 wdc65816_dreadw(wdc65816_cpu_t* cpu, u32 addr) {
  u16 r;
  r  = wdc65816_dreadb(cpu, (addr + 0) & 0xffffff) <<  0;
  r |= wdc65816_dreadb(cpu, (addr + 1) & 0xffffff) <<  8;
  return r;
}

u32 wdc65816_dreadl(wdc65816_cpu_t* cpu, u32 addr) {
  u32 r;
  r  = wdc65816_dreadb(cpu, (addr + 0) & 0xffffff) <<  0;
  r |= wdc65816_dreadb(cpu, (addr + 1) & 0xffffff) <<  8;
  r |= wdc65816_dreadb(cpu, (addr + 2) & 0xffffff) << 16;
  return r;
}

u32 wdc65816_decode(wdc65816_cpu_t* cpu, u8 offset_type, u32 addr) {
  u32 r = 0;

  switch(offset_type) {
    case OPTYPE_DP:
      r = (cpu->regs.d.w + (addr & 0xffff)) & 0xffff;
      break;
    case OPTYPE_DPX:
      r = (cpu->regs.d.w + cpu->regs.x.w + (addr & 0xffff)) & 0xffff;
      break;
    case OPTYPE_DPY:
      r = (cpu->regs.d.w + cpu->regs.y.w + (addr & 0xffff)) & 0xffff;
      break;
    case OPTYPE_IDP:
      addr = (cpu->regs.d.w + (addr & 0xffff)) & 0xffff;
      r = (cpu->regs.db << 16) + wdc65816_dreadw(cpu, addr);
      break;
    case OPTYPE_IDPX:
      addr = (cpu->regs.d.w + cpu->regs.x.w + (addr & 0xffff)) & 0xffff;
      r = (cpu->regs.db << 16) + wdc65816_dreadw(cpu, addr);
      break;
    case OPTYPE_IDPY:
      addr = (cpu->regs.d.w + (addr & 0xffff)) & 0xffff;
      r = (cpu->regs.db << 16) + wdc65816_dreadw(cpu, addr) + cpu->regs.y.w;
      break;
    case OPTYPE_ILDP:
      addr = (cpu->regs.d.w + (addr & 0xffff)) & 0xffff;
      r = wdc65816_dreadl(cpu, addr);
      break;
    case OPTYPE_ILDPY:
      addr = (cpu->regs.d.w + (addr & 0xffff)) & 0xffff;
      r = wdc65816_dreadl(cpu, addr) + cpu->regs.y.w;
      break;
    case OPTYPE_ADDR:
      r = (cpu->regs.db << 16) + (addr & 0xffff);
      break;
    case OPTYPE_ADDR_PC:
      r = (cpu->regs.pc.b << 16) + (addr & 0xffff);
      break;
    case OPTYPE_ADDRX:
      r = (cpu->regs.db << 16) + (addr & 0xffff) + cpu->regs.x.w;
      break;
    case OPTYPE_ADDRY:
      r = (cpu->regs.db << 16) + (addr & 0xffff) + cpu->regs.y.w;
      break;
    case OPTYPE_IADDR_PC:
      r = (cpu->regs.pc.b << 16) + (addr & 0xffff);
      break;
    case OPTYPE_IADDRX:
      r = (cpu->regs.pc.b << 16) + ((addr + cpu->regs.x.w) & 0xffff);
      break;
    case OPTYPE_ILADDR:
      r = addr;
      break;
    case OPTYPE_LONG:
      r = addr;
      break;
    case OPTYPE_LONGX:
      r = (addr + cpu->regs.x.w);
      break;
    case OPTYPE_SR:
      r = (cpu->regs.s.w + (addr & 0xff)) & 0xffff;
      break;
    case OPTYPE_ISRY:
      addr = (cpu->regs.s.w + (addr & 0xff)) & 0xffff;
      r = (cpu->regs.db << 16) + wdc65816_dreadw(cpu, addr) + cpu->regs.y.w;
      break;
    case OPTYPE_RELB:
      r  = (cpu->regs.pc.b << 16) + ((cpu->regs.pc.w + 2) & 0xffff);
      r += (i8)(addr);
      break;
    case OPTYPE_RELW:
      r  = (cpu->regs.pc.b << 16) + ((cpu->regs.pc.w + 3) & 0xffff);
      r += (i16)(addr);
      break;
  }

  return(r & 0xffffff);
}

