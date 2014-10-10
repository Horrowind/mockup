#ifndef CPU_H
#define CPU_H

#include <string.h>
#include <cstdint>
#include "bsnes/processor/processor.hpp"
#include "bsnes/processor/r65816/r65816.hpp"

struct CPU : Processor::R65816 {
public:
    CPU(const char* path, bool debug = false);
    ~CPU();
    void step();
    void init();
    
    void setDebug(bool debug);
    
    uint8_t* m_rom;
    uint8_t* m_ram;
    uint8_t* m_sreg;
    unsigned int m_size;
    int get_cur_pos();
    void set_cur_pos(int addr);
    bool filled_stack();

    void run(uint32_t addrFrom, uint32_t addrTo);
    void clear_ram();

    uint8_t op_read(uint32_t addr);
    void op_write(uint32_t addr, uint8_t data);

private:
    void op_io();
    void last_cycle();
    bool interrupt_pending();

    uint8 disassembler_read(uint32 addr);
    bool m_debug;
};

#endif //CPU_H
