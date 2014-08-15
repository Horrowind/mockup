#ifndef TESTPROCESSOR_H
#define TESTPROCESSOR_H

#include <string.h>
#include <cstdint>
#include "bsnes/processor/processor.hpp"
#include "bsnes/processor/r65816/r65816.hpp"

struct TestCPU : Processor::R65816 {
public:
    TestCPU(const char* path);
    ~TestCPU();
    void step();
    void init();
    uint8_t* m_rom;
    uint8_t* m_ram;
    uint8_t* m_object_low;
    uint8_t* m_object_high;
    unsigned int m_size;
    int get_cur_pos();
    void set_cur_pos(int addr);
    bool filled_stack();

private:
    void op_io();
    void last_cycle();
    bool interrupt_pending();
    uint8_t op_read(uint32_t addr);
    void op_write(uint32_t addr, uint8_t data);
    uint8 disassembler_read(uint32 addr);
};

#endif //TESTPROCESSOR_H
