#include "stdio.h"
#include "string.h"

#include "rom.h"

struct cpu {
    rom_t* m_rom;
    uint8_t* m_ram;
    uint8_t* m_sreg;

    CPUWrapper* cpu_wrapper;
};
typedef struct cpu cpu_t;

#ifdef __cplusplus
extern "C" {

#endif

void cpu_step(cpu_t* cpu);
void cpu_init(cpu_t* cpu, rom_t* rom);
void cpu_show_state(cpu_t* cpu, char[256] ouput);



#ifdef __cplusplus
} // extern "C"

#include "bsnes/processor/processor.hpp"
#include "bsnes/processor/r65816/r65816.hpp"

struct CPUWrapper : Processor::R65816 {
public:
    CPUWrapper(cpu_t*   _cpu,
               uint8_t (_op_read*)(cpu_t cpu, uint32_t addr),
               void    (_op_write*)(cpu_t cpu, uint32_t addr, uint8_t data)) {
        m_cpu = _cpu; m_op_read = _op_read; m_op_write = _op_write;
    }

    void    step()                                { (this->*opcode_table[op_readpc()])(); }

    uint8_t op_read(uint32_t addr)                { return (m_op_read)(cpu, addr); }
    void    op_write(uint32_t addr, uint8_t data) { (m_op_write)(cpu, addr, data); }

private:
    void op_io()             { }
    void last_cycle()        { }
    bool interrupt_pending() { return false; }
    uint8_t disassembler_read(uint32 addr)        { return op_read(addr); }

    cpu_t*   m_cpu;
    uint8_t (m_op_read*)(cpu_t cpu, uint32_t addr);
    void    (m_op_write*)(cpu_t cpu, uint32_t addr, uint8_t data);
};


#endif //__cplusplus
