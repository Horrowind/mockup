#include "algorithms.h"
#include "disassembler.h"
#include "memory.h"
#include "table.h"

void wdc65816_initialize_opcode_table(wdc65816_cpu_t* cpu) {
    enum {
        table_EM =    0,  // 8-bit accumulator,  8-bit index (emulation mode)
        table_MX =  256,  // 8-bit accumulator,  8-bit index
        table_Mx =  512,  // 8-bit accumulator, 16-bit index
        table_mX =  768,  //16-bit accumulator,  8-bit index
        table_mx = 1024,  //16-bit accumulator, 16-bit index
    };
#define opA(  id, name       )                                          \
    cpu->op_table[table_EM + id] = cpu->op_table[table_MX + id]         \
        = cpu->op_table[table_Mx + id] = cpu->op_table[table_mX + id]   \
        = cpu->op_table[table_mx + id] = &wdc65816_op_##name;

#define opE(  id, name       )                                  \
    cpu->op_table[table_EM + id] = &wdc65816_op_##name##_e;       \
    cpu->op_table[table_MX + id] = cpu->op_table[table_Mx + id] \
        = cpu->op_table[table_mX + id]                          \
        = cpu->op_table[table_mx + id] = &wdc65816_op_##name##_n;
    
#define opM(  id, name       )                                          \
    cpu->op_table[table_EM + id] = cpu->op_table[table_MX + id]         \
        = cpu->op_table[table_Mx + id] = &wdc65816_op_##name##_b;         \
    cpu->op_table[table_mX + id] = cpu->op_table[table_mx + id] = &wdc65816_op_##name##_w;
    
#define opX(  id, name       )                                          \
    cpu->op_table[table_EM + id] = cpu->op_table[table_MX + id]         \
        = cpu->op_table[table_mX + id] = &wdc65816_op_##name##_b;         \
    cpu->op_table[table_Mx + id] = cpu->op_table[table_mx + id] = &wdc65816_op_##name##_w;  
    

#include "opcode_table.c"

#undef opA
#undef opE
#undef opM
#undef opX

}

void wdc65816_update_table(wdc65816_cpu_t* cpu) {
    enum {
        table_EM =    0,  // 8-bit accumulator,  8-bit index (emulation mode)
        table_MX =  256,  // 8-bit accumulator,  8-bit index
        table_Mx =  512,  // 8-bit accumulator, 16-bit index
        table_mX =  768,  //16-bit accumulator,  8-bit index
        table_mx = 1024,  //16-bit accumulator, 16-bit index
    };
    if(cpu->regs.e) {
        cpu->opcode_table = &cpu->op_table[table_EM];
    } else if(cpu->regs.p.m) {
        if(cpu->regs.p.x) {
            cpu->opcode_table = &cpu->op_table[table_MX];
        } else {
            cpu->opcode_table = &cpu->op_table[table_Mx];
        }
    } else {
        if(cpu->regs.p.x) {
            cpu->opcode_table = &cpu->op_table[table_mX];
        } else {
            cpu->opcode_table = &cpu->op_table[table_mx];
        }
    }
}
