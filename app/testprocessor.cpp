#include <fstream>
#include <iostream>
#include "testprocessor.hpp"

TestCPU::TestCPU(const char* path, bool debug) {
    m_debug = debug;
    std::ifstream rom_file(path, std::ios::in | std::ios::binary | std::ios::ate);
    if(rom_file.is_open()) {
	m_size = (unsigned int) rom_file.tellg() - 512;
	m_rom = new uint8_t[m_size];
        rom_file.seekg(512, std::ios::beg);
        rom_file.read((char*)m_rom, m_size);
    }
    m_ram = new uint8_t[0x20000];
    for(int i = 0; i < 0x2000; i++) m_ram[i] = 0;
    init();
};

TestCPU::~TestCPU() {
    delete[] m_rom;
    delete[] m_ram;
};


uint8_t TestCPU::op_read(uint32_t addr) {
    
    if(m_debug) std::cout<<"Read: " << std::hex<<addr <<std::endl;

    if(addr & 0xFF8000) {
        if(addr >= 0x7E0000 && addr < 0x800000)
            return m_ram[addr-0x7E0000];
        return m_rom[((addr & 0x7f0000) >> 1) + (addr & 0x7fff)];
    }
    if(addr >= 0x002000) {
        std::cerr<<"Err: Read: " << std::hex<<addr <<std::endl;
    }
    return m_ram[addr];
    
}

void TestCPU::op_write(uint32_t addr, uint8_t data) {
    
    if(addr & 0xFF8000) {
        if(addr >= 0x7E0000 && addr < 0x800000) {
            m_ram[addr-0x7E0000] = data;
        } else {
            std::cerr<<"Err: Wrote: " << std::hex<<addr <<std::endl;
        //Todo: ERROR
        }
    } else {
        if(addr >= 0x002000) {
            std::cerr<<"Err: Wrote: " << std::hex<<addr <<std::endl;
        } else {
            m_ram[addr] = data;
        }
    }
    if(m_debug) std::cout<<"Write: " << std::hex<<addr << ": " << std::hex <<(int)data << std::endl;
}

void TestCPU::step() {
     (this->*opcode_table[op_readpc()])();
}

void TestCPU::init() {
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

int TestCPU::get_cur_pos() {
    return regs.pc;
}

void TestCPU::set_cur_pos(int addr) {
    regs.pc = addr;
}

bool TestCPU::filled_stack() {
    return regs.s.h != 1 || regs.s.l != 0;
}

uint8 TestCPU::disassembler_read(uint32 addr) {
    return op_read(addr);
}

void TestCPU::op_io() { }
void TestCPU::last_cycle() { }
bool TestCPU::interrupt_pending() { return false; }
