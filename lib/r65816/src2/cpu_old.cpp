#include <cstdio>
#include <cstring>

extern "C" {
#include "cpu.h"
}

#include "cpu_old.hpp"

CPU::CPU(r65816_rom_t* rom, bool debug) {
    m_debug = debug;
	m_size = rom->banksize * rom->num_banks;
	m_rom = rom->data;
    m_ram = new uint8_t[0x20000];
    m_sreg = new uint8_t[0x2500];
    clear_ram();
    init();
};

CPU::~CPU() {
    delete[] m_rom;
    delete[] m_ram;
    delete[] m_sreg;
};


void CPU::setDebug(bool debug) {
    m_debug = debug;
}


void CPU::run(uint32_t addrFrom, uint32_t addrTo) {
    regs.pc = addrFrom;
    while(regs.pc != addrTo) {
        step();
    }
}

uint8_t CPU::op_read(uint32_t addr) {
    
    if(addr & 0xFF8000) {
        if(addr >= 0x7E0000 && addr < 0x800000) {
            return m_ram[addr-0x7E0000];
        } else if(addr & 0x008000) {
            return m_rom[((addr & 0x7f0000) >> 1) + (addr & 0x7fff)];
        } else {
            return m_ram[addr & 0x007FFF];
        }
    }
    if(addr >= 0x002000) {
        if(addr <= 0x004500) {
            return m_sreg[addr - 0x2000];
        } else {
            fprintf(stderr, "Err: %06x Read: %06x", regs.pc, addr);
            getchar();
        }
    }
    return m_ram[addr];
    
}

void CPU::op_write(uint32_t addr, uint8_t data) {
    
    if(addr & 0xFF8000) {
        if(addr >= 0x7E0000 && addr < 0x800000) {
            m_ram[addr-0x7E0000] = data;
        } else {
            if(addr & 0x008000) {
                fprintf(stderr, "Err: %06x Wrote: %06x", regs.pc, addr);
                getchar();
                //Todo: ERROR
            } else {
                m_ram[addr & 0x007FFF] = data;
            }
        }
    } else {
        if(addr >= 0x002000) {
            if(addr <= 0x004500) {
                m_sreg[addr - 0x2000] = data;
            } else {
                fprintf(stderr, "Err: %06x Wrote: %06x", regs.pc, addr);
                getchar();
            }
        } else {
            m_ram[addr] = data;
        }
    }
}

void CPU::clear_ram() {
    regs.s = 0x0100;
    memset(m_ram, 0, 20000);
}

void CPU::step() {
    if(m_debug) {
        char output[256];
        disassemble_opcode(output, regs.pc);
        printf("%s\n", output);
    }
    (this->*opcode_table[op_readpc()])();
}

void CPU::init() {
    regs.pc = 0x000000;
    regs.x.h = 0x00;
    regs.y.h = 0x00;
    regs.s.h = 0x01;
    regs.d = 0x0000;
    regs.db = 0x00;
    regs.p = 0x24;
    regs.e = 0;
    regs.mdr = 0x00;
    regs.wai = false;
    regs.vector = 0xfffc;
    update_table();
}

int CPU::get_cur_pos() {
    return regs.pc;
}

void CPU::set_cur_pos(int addr) {
    regs.pc = addr;
}

bool CPU::filled_stack() {
    return regs.s.h != 1 || regs.s.l != 0;
}

uint8 CPU::disassembler_read(uint32 addr) {
    return op_read(addr);
}

void CPU::op_io() { }
void CPU::last_cycle() { }
bool CPU::interrupt_pending() { return false; }

