#include "string.h"

#include "disassembler.h"

uint8_t dreadb(cpu_t* cpu, uint32_t addr) {
  if((addr & 0x40ffff) >= 0x2000 && (addr & 0x40ffff) <= 0x5fff) {
    //$[00-3f|80-bf]:[2000-5fff]
    //do not read MMIO registers within debugger
    return 0x00;
  }
  return op_read(cpu, addr);
}

uint16_t dreadw(cpu_t* cpu, uint32_t addr) {
  uint16_t r;
  r  = dreadb(cpu, (addr + 0) & 0xffffff) <<  0;
  r |= dreadb(cpu, (addr + 1) & 0xffffff) <<  8;
  return r;
}

uint32_t dreadl(cpu_t* cpu, uint32_t addr) {
  uint32_t r;
  r  = dreadb(cpu, (addr + 0) & 0xffffff) <<  0;
  r |= dreadb(cpu, (addr + 1) & 0xffffff) <<  8;
  r |= dreadb(cpu, (addr + 2) & 0xffffff) << 16;
  return r;
}

uint32_t decode(cpu_t* cpu, uint8_t offset_type, uint32_t addr) {
  uint32_t r = 0;

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
      r = (cpu->regs.db << 16) + dreadw(cpu, addr);
      break;
    case OPTYPE_IDPX:
      addr = (cpu->regs.d.w + cpu->regs.x.w + (addr & 0xffff)) & 0xffff;
      r = (cpu->regs.db << 16) + dreadw(cpu, addr);
      break;
    case OPTYPE_IDPY:
      addr = (cpu->regs.d.w + (addr & 0xffff)) & 0xffff;
      r = (cpu->regs.db << 16) + dreadw(cpu, addr) + cpu->regs.y.w;
      break;
    case OPTYPE_ILDP:
      addr = (cpu->regs.d.w + (addr & 0xffff)) & 0xffff;
      r = dreadl(cpu, addr);
      break;
    case OPTYPE_ILDPY:
      addr = (cpu->regs.d.w + (addr & 0xffff)) & 0xffff;
      r = dreadl(cpu, addr) + cpu->regs.y.w;
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
      r = (cpu->regs.db << 16) + dreadw(cpu, addr) + cpu->regs.y.w;
      break;
    case OPTYPE_RELB:
      r  = (cpu->regs.pc.b << 16) + ((cpu->regs.pc.w + 2) & 0xffff);
      r += (int8_t)(addr);
      break;
    case OPTYPE_RELW:
      r  = (cpu->regs.pc.b << 16) + ((cpu->regs.pc.w + 3) & 0xffff);
      r += (int16_t)(addr);
      break;
  }

  return(r & 0xffffff);
}

