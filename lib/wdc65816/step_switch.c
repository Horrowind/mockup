
#include "algorithms.h"
#include "disassembler.h"
#include "memory.h"
#include "table.h"


#define opA(id, name)                                   \
    case (0x400 | id): wdc65816_op_##name(cpu); return;   \
    case (0x300 | id): wdc65816_op_##name(cpu); return;   \
    case (0x200 | id): wdc65816_op_##name(cpu); return;   \
    case (0x100 | id): wdc65816_op_##name(cpu); return;   \
    case (0x000 | id): wdc65816_op_##name(cpu); return;   \

#define opE(id, name)                                       \
    case (0x400 | id): wdc65816_op_##name##_e(cpu); return;   \
    case (0x300 | id): wdc65816_op_##name##_n(cpu); return;   \
    case (0x200 | id): wdc65816_op_##name##_n(cpu); return;   \
    case (0x100 | id): wdc65816_op_##name##_n(cpu); return;   \
    case (0x000 | id): wdc65816_op_##name##_n(cpu); return;   \

#define opM(id, name)                               \
    case (0x400 | id): wdc65816_op_##name##_b(cpu); return;   \
    case (0x300 | id): wdc65816_op_##name##_b(cpu); return;   \
    case (0x200 | id): wdc65816_op_##name##_b(cpu); return;   \
    case (0x100 | id): wdc65816_op_##name##_w(cpu); return;   \
    case (0x000 | id): wdc65816_op_##name##_w(cpu); return;   \

#define opX(id, name)                               \
    case (0x400 | id): wdc65816_op_##name##_b(cpu); return;   \
    case (0x300 | id): wdc65816_op_##name##_b(cpu); return;   \
    case (0x200 | id): wdc65816_op_##name##_w(cpu); return;   \
    case (0x100 | id): wdc65816_op_##name##_b(cpu); return;   \
    case (0x000 | id): wdc65816_op_##name##_w(cpu); return;   \

#define opA_flags(id, name) opA(id, name)
#define opE_flags(id, name) opE(id, name)

void wdc65816_cpu_step(WDC65816Cpu* cpu) {
#ifdef DEBUG_PRINT_CPU_STATE
    if(cpu->debug) {
        char output[256];
        wdc65816_cpu_disassemble_opcode(cpu, output, cpu->regs.pc.d);
        printf("%s\n", output);
    }
#endif //DEBUG_PRINT_CPU_STATE
    
    int switchcase = (cpu->regs.e ? (1 << 10) : ((cpu->regs.p.m << 9) | (cpu->regs.p.x << 8))) | wdc65816_op_readpc(cpu);

    switch(switchcase) {
        opE(0x00, interrupt_brk);
        opM(0x01, read_idpx_ora);
        opE(0x02, interrupt_cop);
        opM(0x03, read_sr_ora);
        opM(0x04, adjust_dp_tsb);
        opM(0x05, read_dp_ora);
        opM(0x06, adjust_dp_asl);
        opM(0x07, read_ildp_ora);
        opA(0x08, php);
        opM(0x09, read_const_ora);
        opM(0x0a, asl_imm);
        opE(0x0b, phd);
        opM(0x0c, adjust_addr_tsb);
        opM(0x0d, read_addr_ora);
        opM(0x0e, adjust_addr_asl);
        opM(0x0f, read_long_ora);
        opA(0x10, branch_bpl);
        opM(0x11, read_idpy_ora);
        opM(0x12, read_idp_ora);
        opM(0x13, read_isry_ora);
        opM(0x14, adjust_dp_trb);
        opM(0x15, read_dpr_ora);
        opM(0x16, adjust_dpx_asl);
        opM(0x17, read_ildpy_ora);
        opA(0x18, flag_clc);
        opM(0x19, read_addry_ora);
        opM(0x1a, adjust_imm_inca);
        opE(0x1b, tcs);
        opM(0x1c, adjust_addr_trb);
        opM(0x1d, read_addrx_ora);
        opM(0x1e, adjust_addrx_asl);
        opM(0x1f, read_longx_ora);
        opA(0x20, jsr_addr);
        opM(0x21, read_idpx_and);
        opE(0x22, jsr_long);
        opM(0x23, read_sr_and);
        opM(0x24, read_dp_bit);
        opM(0x25, read_dp_and);
        opM(0x26, adjust_dp_rol);
        opM(0x27, read_ildpy_and);
        opE_flags(0x28, plp);
        opM(0x29, read_const_and);
        opM(0x2a, rol_imm);
        opE(0x2b, pld);
        opM(0x2c, read_addr_bit);
        opM(0x2d, read_addr_and);
        opM(0x2e, adjust_addr_rol);
        opM(0x2f, read_long_and);
        opA(0x30, branch_bmi);
        opM(0x31, read_idpy_and);
        opM(0x32, read_idp_and);
        opM(0x33, read_isry_and);
        opM(0x34, read_dpr_bit);
        opM(0x35, read_dpr_and);
        opM(0x36, adjust_dpx_rol);
        opM(0x37, read_ildpy_and);
        opA(0x38, flag_sec);
        opM(0x39, read_addry_and);
        opM(0x3a, adjust_imm_deca);
        opA(0x3b, transfer_tsc_w);
        opM(0x3c, read_addrx_bit);
        opM(0x3d, read_addrx_and);
        opM(0x3e, adjust_addrx_rol);
        opM(0x3f, read_longx_and);
        opE_flags(0x40, rti);
        opM(0x41, read_idpx_eor);
        opA(0x42, wdm);
        opM(0x43, read_sr_eor);
        opX(0x44, move_mvp);
        opM(0x45, read_dp_eor);
        opM(0x46, adjust_dp_lsr);
        opM(0x47, read_ildpy_eor);
        opM(0x48, push_pha);
        opM(0x49, read_const_eor);
        opM(0x4a, lsr_imm);
        opA(0x4b, phk);
        opA(0x4c, jmp_addr);
        opM(0x4d, read_addr_eor);
        opM(0x4e, adjust_addr_lsr);
        opM(0x4f, read_long_eor);
        opA(0x50, branch_bvc);
        opM(0x51, read_idpy_eor);
        opM(0x52, read_idp_eor);
        opM(0x53, read_isry_eor);
        opX(0x54, move_mvn);
        opM(0x55, read_dpr_eor);
        opM(0x56, adjust_dpx_lsr);
        opM(0x57, read_ildpy_eor);
        opA(0x58, flag_cli);
        opM(0x59, read_addry_eor);
        opX(0x5a, push_phy);
        opA(0x5b, transfer_tcd_w);
        opA(0x5c, jmp_long);
        opM(0x5d, read_addrx_eor);
        opM(0x5e, adjust_addrx_lsr);
        opM(0x5f, read_longx_eor);
        opA(0x60, rts);
        opM(0x61, read_idpx_adc);
        opE(0x62, per);
        opM(0x63, read_sr_adc);
        opM(0x64, write_dp_stz);
        opM(0x65, read_dp_adc);
        opM(0x66, adjust_dp_ror);
        opM(0x67, read_ildpy_adc);
        opM(0x68, pull_pla);
        opM(0x69, read_const_adc);
        opM(0x6a, ror_imm);
        opE(0x6b, rtl);
        opA(0x6c, jmp_iaddr);
        opM(0x6d, read_addr_adc);
        opM(0x6e, adjust_addr_ror);
        opM(0x6f, read_long_adc);
        opA(0x70, branch_bvs);
        opM(0x71, read_idpy_adc);
        opM(0x72, read_idp_adc);
        opM(0x73, read_isry_adc);
        opM(0x74, write_dpr_stz);
        opM(0x75, read_dpr_adc);
        opM(0x76, adjust_dpx_ror);
        opM(0x77, read_ildpy_adc);
        opA(0x78, flag_sli);
        opM(0x79, read_addry_adc);
        opX(0x7a, pull_ply);
        opA(0x7b, transfer_tdc_w);
        opA(0x7c, jmp_iaddrx);
        opM(0x7d, read_addrx_adc);
        opM(0x7e, adjust_addrx_ror);
        opM(0x7f, read_longx_adc);
        opA(0x80, bra);
        opM(0x81, sta_idpx);
        opA(0x82, brl);
        opM(0x83, sta_sr);
        opX(0x84, write_dp_sty);
        opM(0x85, write_dp_sta);
        opX(0x86, write_dp_stx);
        opM(0x87, sta_ildp);
        opX(0x88, adjust_imm_decy);
        opM(0x89, read_bit_const);
        opM(0x8a, transfer_txa);
        opA(0x8b, phb);
        opX(0x8c, write_addr_sty);
        opM(0x8d, write_addr_sta);
        opX(0x8e, write_addr_stx);
        opM(0x8f, write_longr_sta);
        opA(0x90, branch_bcc);
        opM(0x91, sta_idpy);
        opM(0x92, sta_idp);
        opM(0x93, sta_isry);
        opX(0x94, write_dpr_sty);
        opM(0x95, write_dpr_sta);
        opX(0x96, write_dpr_stx);
        opM(0x97, sta_ildpy);
        opM(0x98, transfer_tya);
        opM(0x99, write_addrr_stay);
        opE(0x9a, txs);
        opX(0x9b, transfer_txy);
        opM(0x9c, write_addr_stz);
        opM(0x9d, write_addrr_stax);
        opM(0x9e, write_addrr_stz);
        opM(0x9f, write_longr_stax);
        opX(0xa0, read_const_ldy);
        opM(0xa1, read_idpx_lda);
        opX(0xa2, read_const_ldx);
        opM(0xa3, read_sr_lda);
        opX(0xa4, read_dp_ldy);
        opM(0xa5, read_dp_lda);
        opX(0xa6, read_dp_ldx);
        opM(0xa7, read_ildpy_lda);
        opX(0xa8, transfer_tay);
        opM(0xa9, read_const_lda);
        opX(0xaa, transfer_tax);
        opA(0xab, plb);
        opX(0xac, read_addr_ldy);
        opM(0xad, read_addr_lda);
        opX(0xae, read_addr_ldx);
        opM(0xaf, read_long_lda);
        opA(0xb0, branch_bcs);
        opM(0xb1, read_idpy_lda);
        opM(0xb2, read_idp_lda);
        opM(0xb3, read_isry_lda);
        opX(0xb4, read_dpr_ldy);
        opM(0xb5, read_dpr_lda);
        opX(0xb6, read_dpr_ldx);
        opM(0xb7, read_ildpy_lda);
        opA(0xb8, flag_clv);
        opM(0xb9, read_addry_lda);
        opX(0xba, tsx);
        opX(0xbb, transfer_tyx);
        opX(0xbc, read_addrx_ldy);
        opM(0xbd, read_addrx_lda);
        opX(0xbe, read_addry_ldx);
        opM(0xbf, read_longx_lda);
        opX(0xc0, read_const_cpy);
        opM(0xc1, read_idpx_cmp);
        opE(0xc2, pflag_rep);
        opM(0xc3, read_sr_cmp);
        opX(0xc4, read_dp_cpy);
        opM(0xc5, read_dp_cmp);
        opM(0xc6, adjust_dp_dec);
        opM(0xc7, read_ildpy_cmp);
        opX(0xc8, adjust_imm_incy);
        opM(0xc9, read_const_cmp);
        opX(0xca, adjust_imm_decx);
        opA(0xcb, wai);
        opX(0xcc, read_addr_cpy);
        opM(0xcd, read_addr_cmp);
        opM(0xce, adjust_addr_dec);
        opM(0xcf, read_long_cmp);
        opA(0xd0, branch_bne);
        opM(0xd1, read_idpy_cmp);
        opM(0xd2, read_idp_cmp);
        opM(0xd3, read_isry_cmp);
        opE(0xd4, pei);
        opM(0xd5, read_dpr_cmp);
        opM(0xd6, adjust_dpx_dec);
        opM(0xd7, read_ildpy_cmp);
        opA(0xd8, flag_cld);
        opM(0xd9, read_addry_cmp);
        opX(0xda, push_phx);
        opA(0xdb, stp);
        opA(0xdc, jmp_iladdr);
        opM(0xdd, read_addrx_cmp);
        opM(0xde, adjust_addrx_dec);
        opM(0xdf, read_longx_cmp);
        opX(0xe0, read_const_cpx);
        opM(0xe1, read_idpx_sbc);
        opE(0xe2, pflag_sep);
        opM(0xe3, read_sr_sbc);
        opX(0xe4, read_dp_cpx);
        opM(0xe5, read_dp_sbc);
        opM(0xe6, adjust_dp_inc);
        opM(0xe7, read_ildpy_sbc);
        opX(0xe8, adjust_imm_incx);
        opM(0xe9, read_const_sbc);
        opA(0xea, nop);
        opA(0xeb, xba);
        opX(0xec, read_addr_cpx);
        opM(0xed, read_addr_sbc);
        opM(0xee, adjust_addr_inc);
        opM(0xef, read_long_sbc);
        opA(0xf0, branch_beq);
        opM(0xf1, read_idpy_sbc);
        opM(0xf2, read_idp_sbc);
        opM(0xf3, read_isry_sbc);
        opE(0xf4, pea);
        opM(0xf5, read_dpr_sbc);
        opM(0xf6, adjust_dpx_inc);
        opM(0xf7, read_ildpy_sbc);
        opA(0xf8, flag_sed);
        opM(0xf9, read_addry_sbc);
        opX(0xfa, pull_plx);
        opA_flags(0xfb, xce);
        opE(0xfc, jsr_iaddrx);
        opM(0xfd, read_addrx_sbc);
        opM(0xfe, adjust_addrx_inc);
        opM(0xff, read_longx_sbc);
    }
#undef opA
#undef opE
#undef opM
#undef opX

}
