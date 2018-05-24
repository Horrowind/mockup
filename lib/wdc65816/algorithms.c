#include "base/base.h"

#include "algorithms.h"

void wdc65816_op_adc_b(Wdc65816Cpu* cpu) {
    int result;
    if(!cpu->regs.p.d) {
        result = cpu->regs.a.l + cpu->rd.l + cpu->regs.p.c;
    } else {
        result = (cpu->regs.a.l & 0x0f) + (cpu->rd.l & 0x0f) + (cpu->regs.p.c << 0);
        if(result > 0x09) result += 0x06;
        cpu->regs.p.c = (result > 0x0f);
        result = (cpu->regs.a.l & 0xf0) + (cpu->rd.l & 0xf0) + (cpu->regs.p.c << 4) + (result & 0x0f);
    }

    cpu->regs.p.v = 1 && (~(cpu->regs.a.l ^ cpu->rd.l) & (cpu->regs.a.l ^ result) & 0x80);
    if(cpu->regs.p.d && result > 0x9f) result += 0x60;
    cpu->regs.p.c = result > 0xff;
    cpu->regs.p.n = 1 && (result & 0x80);
    cpu->regs.p.z = ((u8)result == 0);

    cpu->regs.a.l = result;
}

void wdc65816_op_adc_w(Wdc65816Cpu* cpu) {
    int result;

    if(!cpu->regs.p.d) {
        result = cpu->regs.a.w + cpu->rd.w + cpu->regs.p.c;
    } else {
        result = (cpu->regs.a.w & 0x000f) + (cpu->rd.w & 0x000f) + (cpu->regs.p.c <<  0);
        if(result > 0x0009) result += 0x0006;
        cpu->regs.p.c = result > 0x000f;
        result = (cpu->regs.a.w & 0x00f0) + (cpu->rd.w & 0x00f0) + (cpu->regs.p.c <<  4) + (result & 0x000f);
        if(result > 0x009f) result += 0x0060;
        cpu->regs.p.c = result > 0x00ff;
        result = (cpu->regs.a.w & 0x0f00) + (cpu->rd.w & 0x0f00) + (cpu->regs.p.c <<  8) + (result & 0x00ff);
        if(result > 0x09ff) result += 0x0600;
        cpu->regs.p.c = result > 0x0fff;
        result = (cpu->regs.a.w & 0xf000) + (cpu->rd.w & 0xf000) + (cpu->regs.p.c << 12) + (result & 0x0fff);
    }

    cpu->regs.p.v = 1 && (~(cpu->regs.a.w ^ cpu->rd.w) & (cpu->regs.a.w ^ result) & 0x8000);
    if(cpu->regs.p.d && result > 0x9fff) result += 0x6000;
    cpu->regs.p.c = result > 0xffff;
    cpu->regs.p.n = 1 && (result & 0x8000);
    cpu->regs.p.z = (u16)result == 0;

    cpu->regs.a.w = result;
}

void wdc65816_op_and_b(Wdc65816Cpu* cpu) {
    cpu->regs.a.l &= cpu->rd.l;
    cpu->regs.p.n = 1 && (cpu->regs.a.l & 0x80);
    cpu->regs.p.z = cpu->regs.a.l == 0;
}

void wdc65816_op_and_w(Wdc65816Cpu* cpu) {
    cpu->regs.a.w &= cpu->rd.w;
    cpu->regs.p.n = 1 && (cpu->regs.a.w & 0x8000);
    cpu->regs.p.z = cpu->regs.a.w == 0;
}

void wdc65816_op_bit_b(Wdc65816Cpu* cpu) {
    cpu->regs.p.n = 1 && (cpu->rd.l & 0x80);
    cpu->regs.p.v = 1 && (cpu->rd.l & 0x40);
    cpu->regs.p.z = (cpu->rd.l & cpu->regs.a.l) == 0;
}

void wdc65816_op_bit_w(Wdc65816Cpu* cpu) {
    cpu->regs.p.n = 1 && (cpu->rd.w & 0x8000);
    cpu->regs.p.v = 1 && (cpu->rd.w & 0x4000);
    cpu->regs.p.z = (cpu->rd.w & cpu->regs.a.w) == 0;
}

void wdc65816_op_cmp_b(Wdc65816Cpu* cpu) {
    int r = cpu->regs.a.l - cpu->rd.l;
    cpu->regs.p.n = 1 && (r & 0x80);
    cpu->regs.p.z = (u8)r == 0;
    cpu->regs.p.c = r >= 0;
}

void wdc65816_op_cmp_w(Wdc65816Cpu* cpu) {
    int r = cpu->regs.a.w - cpu->rd.w;
    cpu->regs.p.n = 1 && (r & 0x8000);
    cpu->regs.p.z = (u8)r == 0;
    cpu->regs.p.c = r >= 0;
}

void wdc65816_op_cpx_b(Wdc65816Cpu* cpu) {
    int r = cpu->regs.x.l - cpu->rd.l;
    cpu->regs.p.n = 1 && (r & 0x80);
    cpu->regs.p.z = (u8)r == 0;
    cpu->regs.p.c = r >= 0;
}

void wdc65816_op_cpx_w(Wdc65816Cpu* cpu) {
    int r = cpu->regs.x.w - cpu->rd.w;
    cpu->regs.p.n = 1 && (r & 0x8000);
    cpu->regs.p.z = (u8)r == 0;
    cpu->regs.p.c = r >= 0;
}

void wdc65816_op_cpy_b(Wdc65816Cpu* cpu) {
    int r = cpu->regs.y.l - cpu->rd.l;
    cpu->regs.p.n = 1 && (r & 0x80);
    cpu->regs.p.z = (u8)r == 0;
    cpu->regs.p.c = r >= 0;
}

void wdc65816_op_cpy_w(Wdc65816Cpu* cpu) {
    int r = cpu->regs.y.w - cpu->rd.w;
    cpu->regs.p.n = 1 && (r & 0x8000);
    cpu->regs.p.z = (u16)r == 0;
    cpu->regs.p.c = r >= 0;
}

void wdc65816_op_eor_b(Wdc65816Cpu* cpu) {
    cpu->regs.a.l ^= cpu->rd.l;
    cpu->regs.p.n = 1 && (cpu->regs.a.l & 0x80);
    cpu->regs.p.z = cpu->regs.a.l == 0;
}

void wdc65816_op_eor_w(Wdc65816Cpu* cpu) {
    cpu->regs.a.w ^= cpu->rd.w;
    cpu->regs.p.n = 1 && (cpu->regs.a.w & 0x8000);
    cpu->regs.p.z = cpu->regs.a.w == 0;
}

void wdc65816_op_lda_b(Wdc65816Cpu* cpu) {
    cpu->regs.a.l = cpu->rd.l;
    cpu->regs.p.n = 1 && (cpu->regs.a.l & 0x80);
    cpu->regs.p.z = cpu->regs.a.l == 0;
}

void wdc65816_op_lda_w(Wdc65816Cpu* cpu) {
    cpu->regs.a.w = cpu->rd.w;
    cpu->regs.p.n = 1 && (cpu->regs.a.w & 0x8000);
    cpu->regs.p.z = cpu->regs.a.w == 0;
}

void wdc65816_op_ldx_b(Wdc65816Cpu* cpu) {
    cpu->regs.x.l = cpu->rd.l;
    cpu->regs.p.n = 1 && (cpu->regs.x.l & 0x80);
    cpu->regs.p.z = cpu->regs.x.l == 0;
}

void wdc65816_op_ldx_w(Wdc65816Cpu* cpu) {
    cpu->regs.x.w = cpu->rd.w;
    cpu->regs.p.n = 1 && (cpu->regs.x.w & 0x8000);
    cpu->regs.p.z = cpu->regs.x.w == 0;
}

void wdc65816_op_ldy_b(Wdc65816Cpu* cpu) {
    cpu->regs.y.l = cpu->rd.l;
    cpu->regs.p.n = 1 && (cpu->regs.y.l & 0x80);
    cpu->regs.p.z = cpu->regs.y.l == 0;
}

void wdc65816_op_ldy_w(Wdc65816Cpu* cpu) {
    cpu->regs.y.w = cpu->rd.w;
    cpu->regs.p.n = 1 && (cpu->regs.y.w & 0x8000);
    cpu->regs.p.z = cpu->regs.y.w == 0;
}

void wdc65816_op_ora_b(Wdc65816Cpu* cpu) {
    cpu->regs.a.l |= cpu->rd.l;
    cpu->regs.p.n = 1 && (cpu->regs.a.l & 0x80);
    cpu->regs.p.z = cpu->regs.a.l == 0;
}

void wdc65816_op_ora_w(Wdc65816Cpu* cpu) {
    cpu->regs.a.w |= cpu->rd.w;
    cpu->regs.p.n = 1 && (cpu->regs.a.w & 0x8000);
    cpu->regs.p.z = cpu->regs.a.w == 0;
}

void wdc65816_op_sbc_b(Wdc65816Cpu* cpu) {
    int result;
    cpu->rd.l ^= 0xff;

    if(!cpu->regs.p.d) {
        result = cpu->regs.a.l + cpu->rd.l + cpu->regs.p.c;
    } else {
        result = (cpu->regs.a.l & 0x0f) + (cpu->rd.l & 0x0f) + (cpu->regs.p.c << 0);
        if(result <= 0x0f) result -= 0x06;
        cpu->regs.p.c = result > 0x0f;
        result = (cpu->regs.a.l & 0xf0) + (cpu->rd.l & 0xf0) + (cpu->regs.p.c << 4) + (result & 0x0f);
    }

    cpu->regs.p.v = 1 && (~(cpu->regs.a.l ^ cpu->rd.l) & (cpu->regs.a.l ^ result) & 0x80);
    if(cpu->regs.p.d && result <= 0xff) result -= 0x60;
    cpu->regs.p.c = 1 && (result > 0xff);
    cpu->regs.p.n = 1 && (result & 0x80);
    cpu->regs.p.z = (u8)result == 0;

    cpu->regs.a.l = result;
}

void wdc65816_op_sbc_w(Wdc65816Cpu* cpu) {
    int result;
    cpu->rd.w ^= 0xffff;

    if(!cpu->regs.p.d) {
        result = cpu->regs.a.w + cpu->rd.w + cpu->regs.p.c;
    } else {
        result = (cpu->regs.a.w & 0x000f) + (cpu->rd.w & 0x000f) + (cpu->regs.p.c <<  0);
        if(result <= 0x000f) result -= 0x0006;
        cpu->regs.p.c = result > 0x000f;
        result = (cpu->regs.a.w & 0x00f0) + (cpu->rd.w & 0x00f0) + (cpu->regs.p.c <<  4) + (result & 0x000f);
        if(result <= 0x00ff) result -= 0x0060;
        cpu->regs.p.c = result > 0x00ff;
        result = (cpu->regs.a.w & 0x0f00) + (cpu->rd.w & 0x0f00) + (cpu->regs.p.c <<  8) + (result & 0x00ff);
        if(result <= 0x0fff) result -= 0x0600;
        cpu->regs.p.c = result > 0x0fff;
        result = (cpu->regs.a.w & 0xf000) + (cpu->rd.w & 0xf000) + (cpu->regs.p.c << 12) + (result & 0x0fff);
    }

    cpu->regs.p.v = 1 && (~(cpu->regs.a.w ^ cpu->rd.w) & (cpu->regs.a.w ^ result) & 0x8000);
    if(cpu->regs.p.d && result <= 0xffff) result -= 0x6000;
    cpu->regs.p.c = 1 && (result > 0xffff);
    cpu->regs.p.n = 1 && (result & 0x8000);
    cpu->regs.p.z = (u16)result == 0;

    cpu->regs.a.w = result;
}

void wdc65816_op_inc_b(Wdc65816Cpu* cpu) {
    cpu->rd.l++;
    cpu->regs.p.n = 1 && (cpu->rd.l & 0x80);
    cpu->regs.p.z = (cpu->rd.l == 0);
}

void wdc65816_op_inc_w(Wdc65816Cpu* cpu) {
    cpu->rd.w++;
    cpu->regs.p.n = 1 && (cpu->rd.w & 0x8000);
    cpu->regs.p.z = (cpu->rd.w == 0);
}

void wdc65816_op_dec_b(Wdc65816Cpu* cpu) {
    cpu->rd.l--;
    cpu->regs.p.n = 1 && (cpu->rd.l & 0x80);
    cpu->regs.p.z = cpu->rd.l == 0;
}

void wdc65816_op_dec_w(Wdc65816Cpu* cpu) {
    cpu->rd.w--;
    cpu->regs.p.n = 1 && (cpu->rd.w & 0x8000);
    cpu->regs.p.z = cpu->rd.w == 0;
}

void wdc65816_op_asl_b(Wdc65816Cpu* cpu) {
    cpu->regs.p.c = 1 && (cpu->rd.l & 0x80);
    cpu->rd.l = cpu->rd.l << 1;
    cpu->regs.p.n = 1 && (cpu->rd.l & 0x80);
    cpu->regs.p.z = cpu->rd.l == 0;
}

void wdc65816_op_asl_w(Wdc65816Cpu* cpu) {
    cpu->regs.p.c = 1 && (cpu->rd.w & 0x8000);
    cpu->rd.l = cpu->rd.l << 1;
    cpu->regs.p.n = 1 && (cpu->rd.w & 0x8000);
    cpu->regs.p.z = cpu->rd.w == 0;
}

void wdc65816_op_lsr_b(Wdc65816Cpu* cpu) {
    cpu->regs.p.c = 1 && (cpu->rd.l & 1);
    cpu->rd.l >>= 1;
    cpu->regs.p.n = 1 && (cpu->rd.l & 0x80);
    cpu->regs.p.z = cpu->rd.l == 0;
}

void wdc65816_op_lsr_w(Wdc65816Cpu* cpu) {
    cpu->regs.p.c = 1 && (cpu->rd.w & 1);
    cpu->rd.w >>= 1;
    cpu->regs.p.n = 1 && (cpu->rd.w & 0x8000);
    cpu->regs.p.z = cpu->rd.w == 0;
}

void wdc65816_op_rol_b(Wdc65816Cpu* cpu) {
    unsigned carry = (unsigned)cpu->regs.p.c;
    cpu->regs.p.c = 1 && (cpu->rd.l & 0x80);
    cpu->rd.l = (cpu->rd.l << 1) | carry;
    cpu->regs.p.n = 1 && (cpu->rd.l & 0x80);
    cpu->regs.p.z = cpu->rd.l == 0;
}

void wdc65816_op_rol_w(Wdc65816Cpu* cpu) {
    unsigned carry = (unsigned)cpu->regs.p.c;
    cpu->regs.p.c = 1 && (cpu->rd.w & 0x8000);
    cpu->rd.w = (cpu->rd.w << 1) | carry;
    cpu->regs.p.n = 1 && (cpu->rd.w & 0x8000);
    cpu->regs.p.z = cpu->rd.w == 0;
}

void wdc65816_op_ror_b(Wdc65816Cpu* cpu) {
    unsigned carry = (unsigned)cpu->regs.p.c << 7;
    cpu->regs.p.c = 1 && (cpu->rd.l & 1);
    cpu->rd.l = carry | (cpu->rd.l >> 1);
    cpu->regs.p.n = 1 && (cpu->rd.l & 0x80);
    cpu->regs.p.z = cpu->rd.l == 0;
}

void wdc65816_op_ror_w(Wdc65816Cpu* cpu) {
    unsigned carry = (unsigned)cpu->regs.p.c << 15;
    cpu->regs.p.c = 1 && (cpu->rd.w & 1);
    cpu->rd.w = carry | (cpu->rd.w >> 1);
    cpu->regs.p.n = 1 && (cpu->rd.w & 0x8000);
    cpu->regs.p.z = cpu->rd.w == 0;
}

void wdc65816_op_trb_b(Wdc65816Cpu* cpu) {
    cpu->regs.p.z = (cpu->rd.l & cpu->regs.a.l) == 0;
    cpu->rd.l &= ~cpu->regs.a.l;
}

void wdc65816_op_trb_w(Wdc65816Cpu* cpu) {
    cpu->regs.p.z = (cpu->rd.w & cpu->regs.a.w) == 0;
    cpu->rd.w &= ~cpu->regs.a.w;
}

void wdc65816_op_tsb_b(Wdc65816Cpu* cpu) {
    cpu->regs.p.z = (cpu->rd.l & cpu->regs.a.l) == 0;
    cpu->rd.l |= cpu->regs.a.l;
}

void wdc65816_op_tsb_w(Wdc65816Cpu* cpu) {
    cpu->regs.p.z = (cpu->rd.w & cpu->regs.a.w) == 0;
    cpu->rd.w |= cpu->regs.a.w;
}
