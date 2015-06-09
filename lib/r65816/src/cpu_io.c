#include "stdio.h"
#include "cpu.h"

uint8_t r65816_cpu_read(r65816_cpu_t* cpu, uint32_t addr) {
    if(r65816_breakpoint_list_is_hit(cpu->breakpoints_write, addr) && !cpu->stop_execution) {
        cpu->stop_execution = 1;
        cpu->breakpoint_address = addr;
        cpu->breakpoint_data = 0;
    }
    
    if(addr >= 0x7E0000 && addr < 0x800000) {
        return cpu->ram[addr-0x7E0000];
    } else if(addr & 0x008000) {
        return cpu->rom->data[((addr&0x7F0000)>>1|(addr&0x7FFF))];
    } else {
        addr &= 0x7FFF;
        if(addr >= 0x002000) {
            if(addr <= 0x004500) {
                return cpu->sreg[addr - 0x2000];
            } else {
                fprintf(stderr, "Err: %06x Read: %06x\n", cpu->regs.pc.d, addr);
                return 0;
            }
        } else {
            return cpu->ram[addr];
        }
    }
}

void r65816_cpu_write(r65816_cpu_t* cpu, uint32_t addr, uint8_t data) {
   
    if(r65816_breakpoint_list_is_hit(cpu->breakpoints_write, addr) && !cpu->stop_execution) {
        cpu->stop_execution = 1;
        cpu->breakpoint_address = addr;
        cpu->breakpoint_data = data;
    }

    if(addr & 0xFF8000) {
        if(addr >= 0x7E0000 && addr < 0x800000) {
            cpu->ram[addr-0x7E0000] = data;
        } else {
            if(addr & 0x008000) {
                char output[256];
                r65816_cpu_disassemble_opcode(cpu, output, cpu->regs.pc.d);
                printf("%s\n", output);
                fprintf(stderr, "%06x Error - Wrote: %06x\n", cpu->regs.pc.d, addr);
                getchar();
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
                char output[256];
                r65816_cpu_disassemble_opcode(cpu, output, cpu->regs.pc.d);
                printf("%s\n", output);
                fprintf(stderr, "%06x Error - Wrote: %06x\n", cpu->regs.pc.d, addr);
                getchar();
            }
        } else {
            cpu->ram[addr] = data;
        }
    }
}


uint16_t r65816_cpu_read16(r65816_cpu_t* cpu, uint32_t addr) {
    return r65816_cpu_read(cpu, addr)
        | (r65816_cpu_read(cpu, addr + 1) << 4);
}

uint32_t r65816_cpu_read24(r65816_cpu_t* cpu, uint32_t addr) {
    return r65816_cpu_read(cpu, addr)
        | (r65816_cpu_read(cpu, addr + 1) << 8)
        | (r65816_cpu_read(cpu, addr + 2) << 16);
}
