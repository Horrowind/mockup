#ifndef LIBMOCKUP_ASM_H
#define LIBMOCKUP_ASM_H

typedef struct {
    uint8_t* buffer;
    uint32_t location;
} a65816_program_state_t;


#define a65816_gen_op0(name, op0)                       \
    void a65816_op_##name(a65816_program_state_t* ps) { \
        a65816_write(ps, op0);                          \
    }

a65816_gen_op0("php", 0x08)
a65816_gen_op0("asl", 0x0a)
a65816_gen_op0("phd", 0x0b)
a65816_gen_op0("clc", 0x18)
a65816_gen_op0("inc", 0x1a)
a65816_gen_op0("tcs", 0x1b)
a65816_gen_op0("plp", 0x28)
a65816_gen_op0("rol", 0x2a)
a65816_gen_op0("pld", 0x2b)
a65816_gen_op0("sec", 0x38)
a65816_gen_op0("dec", 0x3a)
a65816_gen_op0("tsc", 0x3b)
a65816_gen_op0("rti", 0x40)
a65816_gen_op0("pha", 0x48)
a65816_gen_op0("lsr", 0x4a)
a65816_gen_op0("phk", 0x4b)
a65816_gen_op0("cli", 0x58)
a65816_gen_op0("phy", 0x5a)
a65816_gen_op0("tcd", 0x5b)
a65816_gen_op0("rts", 0x60)
a65816_gen_op0("pla", 0x68)
a65816_gen_op0("ror", 0x6a)
a65816_gen_op0("rtl", 0x6b)
a65816_gen_op0("sei", 0x78)
a65816_gen_op0("ply", 0x7a)
a65816_gen_op0("tdc", 0x7b)
a65816_gen_op0("dey", 0x88)
a65816_gen_op0("txa", 0x8a)
a65816_gen_op0("phb", 0x8b)
a65816_gen_op0("tya", 0x98)
a65816_gen_op0("txs", 0x9a)
a65816_gen_op0("txy", 0x9b)
a65816_gen_op0("tay", 0xa8)
a65816_gen_op0("tax", 0xaa)
a65816_gen_op0("plb", 0xab)
a65816_gen_op0("clv", 0xb8)
a65816_gen_op0("tsx", 0xba)
a65816_gen_op0("tyx", 0xbb)
a65816_gen_op0("iny", 0xc8)
a65816_gen_op0("dex", 0xca)
a65816_gen_op0("wai", 0xcb)
a65816_gen_op0("cld", 0xd8)
a65816_gen_op0("phx", 0xda)
a65816_gen_op0("stp", 0xdb)
a65816_gen_op0("inx", 0xe8)
a65816_gen_op0("nop", 0xea)
a65816_gen_op0("xba", 0xeb)
a65816_gen_op0("sed", 0xf8)
a65816_gen_op0("plx", 0xfa)
a65816_gen_op0("xce", 0xfb)

a65816_gen_op0("asl_imm", 0x0a)
a65816_gen_op0("lsr_imm", 0x4a)
a65816_gen_op0("rol_imm", 0x2a)
a65816_gen_op0("ror_imm", 0x6a)
a65816_gen_op0("inc_imm", 0x1a)
a65816_gen_op0("dec_imm", 0x3a)


#undef a65816_gen_op0

#define a65816_gen_op1_imm(name, opcode)                                \
    void a65816_op_##name(a65816_program_state_t* ps, uint8_t op1) {    \
        a65816_write(ps, opcode);                                       \
        a65816_write(ps, op1);                                          \
    }

a65816_gen_op1_imm("ORA", 0x09)
a65816_gen_op1_imm("AND", 0x29)
a65816_gen_op1_imm("EOR", 0x49)
a65816_gen_op1_imm("ADC", 0x69)
a65816_gen_op1_imm("BIT", 0x89)
a65816_gen_op1_imm("LDA", 0xA9)
a65816_gen_op1_imm("CMP", 0xC9)
a65816_gen_op1_imm("SBC", 0xE9)
a65816_gen_op1_imm("CPX", 0xE0)
a65816_gen_op1_imm("CPY", 0xC0)
a65816_gen_op1_imm("LDX", 0xA2)
a65816_gen_op1_imm("LDY", 0xA0)

a65816_gen_op1_imm("brk", 0x00)
a65816_gen_op1_imm("cop", 0x02)
a65816_gen_op1_imm("wdm", 0x42)
#undef a65816_gen_op1_imm


void a65816_bmi(a65816_program_state_t* ps, uint8_t op0) {
    
}


#endif //LIBMOCKUP_ASM_H
