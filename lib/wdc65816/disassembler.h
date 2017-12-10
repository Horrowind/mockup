#ifndef MOCKUP_DISASSEMBLER_H
#define MOCKUP_DISASSEMBLER_H

#include "cpu.h"

enum {
  OPTYPE_DP = 0,    //dp
  OPTYPE_DPX,       //dp,x
  OPTYPE_DPY,       //dp,y
  OPTYPE_IDP,       //(dp)
  OPTYPE_IDPX,      //(dp,x)
  OPTYPE_IDPY,      //(dp),y
  OPTYPE_ILDP,      //[dp]
  OPTYPE_ILDPY,     //[dp],y
  OPTYPE_ADDR,      //addr
  OPTYPE_ADDRX,     //addr,x
  OPTYPE_ADDRY,     //addr,y
  OPTYPE_IADDRX,    //(addr,x)
  OPTYPE_ILADDR,    //[addr]
  OPTYPE_LONG,      //long
  OPTYPE_LONGX,     //long, x
  OPTYPE_SR,        //sr,s
  OPTYPE_ISRY,      //(sr,s),y
  OPTYPE_ADDR_PC,   //pbr:addr
  OPTYPE_IADDR_PC,  //pbr:(addr)
  OPTYPE_RELB,      //relb
  OPTYPE_RELW,      //relw
};

u8 wdc65816_dreadb(WDC65816Cpu* cpu, u32 addr);
u16 wdc65816_dreadw(WDC65816Cpu* cpu, u32 addr);
u32 wdc65816_dreadl(WDC65816Cpu* cpu, u32 addr);
u32 wdc65816_decode(WDC65816Cpu* cpu, u8 offset_type, u32 addr);

#endif //MOCKUP_DISASSEMBLER_H
