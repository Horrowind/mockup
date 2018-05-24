#include "cpu.h"


u8 wdc65816_cpu_read(Wdc65816Cpu* cpu, u32 addr) {
    if(wdc65816_breakpoint_list_is_hit(&cpu->breakpoints_read, addr) && !cpu->stop_execution) {
        cpu->stop_execution = 1;
        cpu->breakpoint_address = addr;
        cpu->breakpoint_data = 0;
    }
    return wdc65816_mapper_read(&cpu->read_mapper, addr);
}

void wdc65816_cpu_write(Wdc65816Cpu* cpu, u32 addr, u8 data) {
    if(wdc65816_breakpoint_list_is_hit(&cpu->breakpoints_write, addr) && !cpu->stop_execution) {
        cpu->stop_execution = 1;
        cpu->breakpoint_address = addr;
        cpu->breakpoint_data = data;
    }
    wdc65816_mapper_write(&cpu->write_mapper, addr, data);
}

/* if((addr&0xFE0000)==0x7E0000) { */
/*     //if(addr >= 0x7E0000) { */
/* //if(addr >= 0x800000) { */
/* //	fprintf(stderr, "Err: %06x Read: %06x\n", cpu->regs.pc.d, addr); */
/*     //	return 0; */
/* //} */
/* //if((addr&0xF00000)==0x700000 || (addr&0x408000)==0x000000) { */
/*     return cpu->ram[addr - 0x7E0000]; */
/* } else if(addr & 0x008000) { */
/*     return cpu->rom->data[((addr&0x7F0000)>>1|(addr&0x7FFF))]; */
/* } else { */
/*     addr &= 0x7FFF; */
/*     if(addr >= 0x002000) { */
/*         if(addr <= 0x004500) { */
/*             return cpu->sreg[addr - 0x2000]; */
/*         } else { */
/*             fprintf(stderr, "Err: %06x Read: %06x\n", cpu->regs.pc.d, addr); */
/*             return 0; */
/*         } */
/*     } else { */
/*         return cpu->ram[addr]; */
/*     } */
/* } */

/* u8 wdc65816_cpu_read(Wdc65816Cpu* cpu, u32 addr) { */
/*     if(wdc65816_breakpoint_list_is_hit(&cpu->breakpoints_read, addr) && !cpu->stop_execution) { */
/*         cpu->stop_execution = 1; */
/*         cpu->breakpoint_address = addr; */
/*         cpu->breakpoint_data = 0; */
/*     } */
/*     if((addr&0xFE0000)==0x7E0000) { */
/*         //if(addr >= 0x7E0000) { */
/* 	//if(addr >= 0x800000) { */
/* 	//	fprintf(stderr, "Err: %06x Read: %06x\n", cpu->regs.pc.d, addr); */
/*         //	return 0; */
/* 	//} */
/*     //if((addr&0xF00000)==0x700000 || (addr&0x408000)==0x000000) { */
/*         return cpu->ram[addr - 0x7E0000]; */
/*     } else if(addr & 0x008000) { */
/*         return cpu->rom->data[((addr&0x7F0000)>>1|(addr&0x7FFF))]; */
/*     } else { */
/*         addr &= 0x7FFF; */
/*         if(addr >= 0x002000) { */
/*             if(addr <= 0x004500) { */
/*                 return cpu->sreg[addr - 0x2000]; */
/*             } else { */
/*                 fprintf(stderr, "Err: %06x Read: %06x\n", cpu->regs.pc.d, addr); */
/*                 return 0; */
/*             } */
/*         } else { */
/*             return cpu->ram[addr]; */
/*         } */
/*     } */
/* } */

/* void wdc65816_cpu_write(Wdc65816Cpu* cpu, u32 addr, u8 data) { */
   
/*     if(wdc65816_breakpoint_list_is_hit(&cpu->breakpoints_write, addr) && !cpu->stop_execution) { */
/*         cpu->stop_execution = 1; */
/*         cpu->breakpoint_address = addr; */
/*         cpu->breakpoint_data = data; */
/*     } */

/*     if(addr & 0xFF8000) { */
/*         if(addr >= 0x7E0000) { */
/*             cpu->ram[addr-0x7E0000] = data; */
/*         } else { */
/*             if(addr & 0x008000) { */
/*                 char output[256]; */
/*                 wdc65816_cpu_disassemble_opcode(cpu, output, cpu->regs.pc.d); */
/*                 printf("%s\n", output); */
/*                 fprintf(stderr, "%06x Error - Wrote: %06x\n", cpu->regs.pc.d, addr); */
/*                 getchar(); */
/*                 //Todo: ERROR */
/*             } else { */
/*                 cpu->ram[addr & 0x007FFF] = data; */
/*             } */
/*         } */
/*     } else { */
/*         if(addr >= 0x002000) { */
/*             if(addr <= 0x004500) { */
/*                 cpu->sreg[addr - 0x2000] = data; */
/*             } else { */
/*                 char output[256]; */
/*                 wdc65816_cpu_disassemble_opcode(cpu, output, cpu->regs.pc.d); */
/*                 printf("%s\n", output); */
/*                 fprintf(stderr, "%06x Error - Wrote: %06x\n", cpu->regs.pc.d, addr); */
/*                 getchar(); */
/*             } */
/*         } else { */
/*             cpu->ram[addr] = data; */
/*         } */
/*     } */
/* } */


u16 wdc65816_cpu_read16(Wdc65816Cpu* cpu, u32 addr) {
    return wdc65816_cpu_read(cpu, addr)
        | (wdc65816_cpu_read(cpu, addr + 1) << 4);
}

u32 wdc65816_cpu_read24(Wdc65816Cpu* cpu, u32 addr) {
    return wdc65816_cpu_read(cpu, addr)
        | (wdc65816_cpu_read(cpu, addr + 1) << 8)
        | (wdc65816_cpu_read(cpu, addr + 2) << 16);
}
