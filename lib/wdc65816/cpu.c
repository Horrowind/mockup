#include "cpu.h"
#include "disassembler.h"
#include "memory.h"
#include "table.h"

void wdc65816_cpu_init(Wdc65816Cpu* cpu, Wdc65816Rom* rom, Arena* arena) {
    cpu->regs.p.b = 0x24;
    cpu->rom  = rom;
    cpu->ram  = arena_alloc_array(arena, 0x20000, u8);
    cpu->sreg = arena_alloc_array(arena, 0x2500, u8);

    cpu->regs.pc.d = 0x000000;
    wdc65816_breakpoint_list_init(&cpu->breakpoints_exec);
    wdc65816_breakpoint_list_init(&cpu->breakpoints_read);
    wdc65816_breakpoint_list_init(&cpu->breakpoints_write);
    wdc65816_cpu_clear(cpu);
#ifdef DEBUG_PRINT_CPU_STATE
    cpu->debug = 0;
#endif

    Wdc65816MapperEntry ram_entry1 = {
        .bank_low  =   0x00,
        .bank_high =   0x3F,
        .addr_low  = 0x0000,
        .addr_high = 0x1FFF,
        .size      = 0x2000,
        .data      = cpu->ram,
        .name      = L(".ram"),
    };
    
    Wdc65816MapperEntry ram_entry2 = {
        .bank_low  =   0x80,
        .bank_high =   0xBF,
        .addr_low  = 0x0000,
        .addr_high = 0x1FFF,
        .size      = 0x2000,
        .data      = cpu->ram,
        .data_length= 0x2000,
        .name      = L(".ram"),
    };

    Wdc65816MapperEntry ram_entry3 = {
        .bank_low  =   0x7E,
        .bank_high =   0x7F,
        .addr_low  = 0x0000,
        .addr_high = 0xFFFF,
        .size      = 0x20000,
        .data      = cpu->ram,
        .data_length= 0x20000,
        .name      = L(".ram"),
    };

    Wdc65816MapperEntry sreg_entry = {
        .bank_low  =   0x00,
        .bank_high =   0xFF,
        .addr_low  = 0x2000,
        .addr_high = 0x44FF,
        .size      = 0x2500,
        .data      = cpu->sreg,
        .name      = L(".sreg"),
    };

    
    wdc65816_mapper_init(&cpu->read_mapper, arena);
    wdc65816_mapper_init(&cpu->write_mapper, arena);
    wdc65816_mapper_add(&cpu->read_mapper,  &ram_entry1);
    wdc65816_mapper_add(&cpu->read_mapper,  &ram_entry2);
    wdc65816_mapper_add(&cpu->read_mapper,  &ram_entry3);
    wdc65816_mapper_add(&cpu->read_mapper,  &sreg_entry);
    wdc65816_mapper_add(&cpu->write_mapper, &ram_entry1);
    wdc65816_mapper_add(&cpu->write_mapper, &ram_entry2);
    wdc65816_mapper_add(&cpu->write_mapper, &ram_entry3);
    wdc65816_mapper_add(&cpu->write_mapper, &sreg_entry);
    for(int i = 0; i < rom->read_mapper.num_entries; i++)
        wdc65816_mapper_add(&cpu->read_mapper, rom->read_mapper.entries + i);
    for(int i = 0; i < rom->write_mapper.num_entries; i++)
        wdc65816_mapper_add(&cpu->write_mapper, rom->write_mapper.entries + i);
        
    wdc65816_mapper_update(&cpu->read_mapper);
    wdc65816_mapper_update(&cpu->write_mapper);
}

void wdc65816_cpu_free(Wdc65816Cpu* cpu) {
    wdc65816_breakpoint_list_deinit(&cpu->breakpoints_exec);
    wdc65816_breakpoint_list_deinit(&cpu->breakpoints_read);
    wdc65816_breakpoint_list_deinit(&cpu->breakpoints_write);
}

void wdc65816_cpu_clear(Wdc65816Cpu* cpu) {
    cpu->regs.a.w = 0x0000;
    cpu->regs.x.w = 0x0000;
    cpu->regs.y.w = 0x0000;
    cpu->regs.z.w = 0x0000;
    cpu->regs.s.w = 0x01FF;
    cpu->regs.d.w = 0x0000;
    cpu->regs.db = 0x00;
    cpu->regs.p.b = 0x24;
    cpu->regs.e = 0;
    cpu->regs.mdr = 0x00;
    memset(cpu->ram, 0, 0x20000);
    memset(cpu->sreg, 0, 0x2500);
    wdc65816_breakpoint_list_deinit(&cpu->breakpoints_exec);
    wdc65816_breakpoint_list_deinit(&cpu->breakpoints_read);
    wdc65816_breakpoint_list_deinit(&cpu->breakpoints_write);

}

void wdc65816_cpu_add_exec_bp(Wdc65816Cpu* cpu, u32 addr) {
    wdc65816_breakpoint_list_add(&cpu->breakpoints_exec, addr);
}

void wdc65816_cpu_add_read_bp(Wdc65816Cpu* cpu, u32 addr) {
    wdc65816_breakpoint_list_add(&cpu->breakpoints_read, addr);
}

void wdc65816_cpu_add_write_bp(Wdc65816Cpu* cpu, u32 addr) {
    wdc65816_breakpoint_list_add(&cpu->breakpoints_write, addr);
}

void wdc65816_cpu_add_exec_bp_range(Wdc65816Cpu* cpu, u32 addr_low, u32 addr_high) {
    wdc65816_breakpoint_list_add_range(&cpu->breakpoints_exec, addr_low, addr_high);
}

void wdc65816_cpu_add_read_bp_range(Wdc65816Cpu* cpu, u32 addr_low, u32 addr_high) {
    wdc65816_breakpoint_list_add_range(&cpu->breakpoints_read, addr_low, addr_high);
}

void wdc65816_cpu_add_write_bp_range(Wdc65816Cpu* cpu, u32 addr_low, u32 addr_high) {
    wdc65816_breakpoint_list_add_range(&cpu->breakpoints_write, addr_low, addr_high);
}


void wdc65816_cpu_clear_exec_bp(Wdc65816Cpu* cpu) {
    wdc65816_breakpoint_list_deinit(&cpu->breakpoints_exec);
}

void wdc65816_cpu_clear_read_bp(Wdc65816Cpu* cpu) {
    wdc65816_breakpoint_list_deinit(&cpu->breakpoints_read);
}

void wdc65816_cpu_clear_write_bp(Wdc65816Cpu* cpu) {
    wdc65816_breakpoint_list_deinit(&cpu->breakpoints_write);
}



void wdc65816_cpu_show_state(Wdc65816Cpu* cpu, char* output) {
    wdc65816_cpu_disassemble_opcode(cpu, output, cpu->regs.pc.d);
}

void wdc65816_cpu_disassemble_opcode(Wdc65816Cpu* cpu, char* output, u32 addr) {
  static Wdc65816Reg24 pc;
  char t[256];
  char* s = output;

  pc.d = addr;
  sprintf(s, "%.6x ",  (u32)pc.d);

  u8 op  = wdc65816_dreadb(cpu, pc.d); pc.w++;
  u8 op0 = wdc65816_dreadb(cpu, pc.d); pc.w++;
  u8 op1 = wdc65816_dreadb(cpu, pc.d); pc.w++;
  u8 op2 = wdc65816_dreadb(cpu, pc.d);

  #define op8  ((op0))
  #define op16 ((op0) | (op1 << 8))
  #define op24 ((op0) | (op1 << 8) | (op2 << 16))
  #define a8   (cpu->regs.e || cpu->regs.p.m)
  #define x8   (cpu->regs.e || cpu->regs.p.x)

  switch(op) {
  case 0x00: { sprintf(t, "brk #$%.2x              ", op8); } break;
  case 0x01: { sprintf(t, "ora ($%.2x,x)   [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_IDPX, op8)); } break;
  case 0x02: { sprintf(t, "cop #$%.2x              ", op8); } break;
  case 0x03: { sprintf(t, "ora $%.2x,s     [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_SR, op8)); } break;
  case 0x04: { sprintf(t, "tsb $%.2x       [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_DP, op8)); } break;
  case 0x05: { sprintf(t, "ora $%.2x       [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_DP, op8)); } break;
  case 0x06: { sprintf(t, "asl $%.2x       [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_DP, op8)); } break;
  case 0x07: { sprintf(t, "ora [$%.2x]     [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_ILDP, op8)); } break;
  case 0x08: { sprintf(t, "php                   "); } break;
  case 0x09: { if(a8) sprintf(t, "ora #$%.2x              ", op8);
               else sprintf(t, "ora #$%.4x            ", op16); } break;
  case 0x0a: { sprintf(t, "asl a                 "); } break;
  case 0x0b: { sprintf(t, "phd                   "); } break;
  case 0x0c: { sprintf(t, "tsb $%.4x     [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDR, op16)); } break;
  case 0x0d: { sprintf(t, "ora $%.4x     [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDR, op16)); } break;
  case 0x0e: { sprintf(t, "asl $%.4x     [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDR, op16)); } break;
  case 0x0f: { sprintf(t, "ora $%.6x   [%.6x]", op24, wdc65816_decode(cpu, OPTYPE_LONG, op24)); } break;
  case 0x10: { sprintf(t, "bpl $%.4x     [%.6x]", (u16)(wdc65816_decode(cpu, OPTYPE_RELB, op8)), wdc65816_decode(cpu, OPTYPE_RELB, op8)); } break;
  case 0x11: { sprintf(t, "ora ($%.2x),y   [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_IDPY, op8)); } break;
  case 0x12: { sprintf(t, "ora ($%.2x)     [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_IDP, op8)); } break;
  case 0x13: { sprintf(t, "ora ($%.2x,s),y [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_ISRY, op8)); } break;
  case 0x14: { sprintf(t, "trb $%.2x       [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_DP, op8)); } break;
  case 0x15: { sprintf(t, "ora $%.2x,x     [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_DPX, op8)); } break;
  case 0x16: { sprintf(t, "asl $%.2x,x     [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_DPX, op8)); } break;
  case 0x17: { sprintf(t, "ora [$%.2x],y   [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_ILDPY, op8)); } break;
  case 0x18: { sprintf(t, "clc                   "); } break;
  case 0x19: { sprintf(t, "ora $%.4x,y   [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDRY, op16)); } break;
  case 0x1a: { sprintf(t, "inc                   "); } break;
  case 0x1b: { sprintf(t, "tcs                   "); } break;
  case 0x1c: { sprintf(t, "trb $%.4x     [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDR, op16)); } break;
  case 0x1d: { sprintf(t, "ora $%.4x,x   [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDRX, op16)); } break;
  case 0x1e: { sprintf(t, "asl $%.4x,x   [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDRX, op16)); } break;
  case 0x1f: { sprintf(t, "ora $%.6x,x [%.6x]", op24, wdc65816_decode(cpu, OPTYPE_LONGX, op24)); } break;
  case 0x20: { sprintf(t, "jsr $%.4x     [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDR_PC, op16)); } break;
  case 0x21: { sprintf(t, "and ($%.2x,x)   [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_IDPX, op8)); } break;
  case 0x22: { sprintf(t, "jsl $%.6x   [%.6x]", op24, wdc65816_decode(cpu, OPTYPE_LONG, op24)); } break;
  case 0x23: { sprintf(t, "and $%.2x,s     [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_SR, op8)); } break;
  case 0x24: { sprintf(t, "bit $%.2x       [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_DP, op8)); } break;
  case 0x25: { sprintf(t, "and $%.2x       [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_DP, op8)); } break;
  case 0x26: { sprintf(t, "rol $%.2x       [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_DP, op8)); } break;
  case 0x27: { sprintf(t, "and [$%.2x]     [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_ILDP, op8)); } break;
  case 0x28: { sprintf(t, "plp                   "); } break;
  case 0x29: { if(a8) sprintf(t, "and #$%.2x              ", op8);
               else   sprintf(t, "and #$%.4x            ", op16); } break;
  case 0x2a: { sprintf(t, "rol a                 "); } break;
  case 0x2b: { sprintf(t, "pld                   "); } break;
  case 0x2c: { sprintf(t, "bit $%.4x     [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDR, op16)); } break;
  case 0x2d: { sprintf(t, "and $%.4x     [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDR, op16)); } break;
  case 0x2e: { sprintf(t, "rol $%.4x     [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDR, op16)); } break;
  case 0x2f: { sprintf(t, "and $%.6x   [%.6x]", op24, wdc65816_decode(cpu, OPTYPE_LONG, op24)); } break;
  case 0x30: { sprintf(t, "bmi $%.4x     [%.6x]", (u16)(wdc65816_decode(cpu, OPTYPE_RELB, op8)), wdc65816_decode(cpu, OPTYPE_RELB, op8)); } break;
  case 0x31: { sprintf(t, "and ($%.2x),y   [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_IDPY, op8)); } break;
  case 0x32: { sprintf(t, "and ($%.2x)     [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_IDP, op8)); } break;
  case 0x33: { sprintf(t, "and ($%.2x,s),y [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_ISRY, op8)); } break;
  case 0x34: { sprintf(t, "bit $%.2x,x     [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_DPX, op8)); } break;
  case 0x35: { sprintf(t, "and $%.2x,x     [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_DPX, op8)); } break;
  case 0x36: { sprintf(t, "rol $%.2x,x     [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_DPX, op8)); } break;
  case 0x37: { sprintf(t, "and [$%.2x],y   [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_ILDPY, op8)); } break;
  case 0x38: { sprintf(t, "sec                   "); } break;
  case 0x39: { sprintf(t, "and $%.4x,y   [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDRY, op16)); } break;
  case 0x3a: { sprintf(t, "dec                   "); } break;
  case 0x3b: { sprintf(t, "tsc                   "); } break;
  case 0x3c: { sprintf(t, "bit $%.4x,x   [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDRX, op16)); } break;
  case 0x3d: { sprintf(t, "and $%.4x,x   [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDRX, op16)); } break;
  case 0x3e: { sprintf(t, "rol $%.4x,x   [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDRX, op16)); } break;
  case 0x3f: { sprintf(t, "and $%.6x,x [%.6x]", op24, wdc65816_decode(cpu, OPTYPE_LONGX, op24)); } break;
  case 0x40: { sprintf(t, "rti                   "); } break;
  case 0x41: { sprintf(t, "eor ($%.2x,x)   [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_IDPX, op8)); } break;
  case 0x42: { sprintf(t, "wdm                   "); } break;
  case 0x43: { sprintf(t, "eor $%.2x,s     [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_SR, op8)); } break;
  case 0x44: { sprintf(t, "mvp $%.2x,$%.2x           ", op1, op8); } break;
  case 0x45: { sprintf(t, "eor $%.2x       [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_DP, op8)); } break;
  case 0x46: { sprintf(t, "lsr $%.2x       [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_DP, op8)); } break;
  case 0x47: { sprintf(t, "eor [$%.2x]     [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_ILDP, op8)); } break;
  case 0x48: { sprintf(t, "pha                   "); } break;
  case 0x49: { if(a8) sprintf(t, "eor #$%.2x              ", op8);
               else   sprintf(t, "eor #$%.4x            ", op16); } break;
  case 0x4a: { sprintf(t, "lsr a                 "); } break;
  case 0x4b: { sprintf(t, "phk                   "); } break;
  case 0x4c: { sprintf(t, "jmp $%.4x     [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDR_PC, op16)); } break;
  case 0x4d: { sprintf(t, "eor $%.4x     [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDR, op16)); } break;
  case 0x4e: { sprintf(t, "lsr $%.4x     [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDR, op16)); } break;
  case 0x4f: { sprintf(t, "eor $%.6x   [%.6x]", op24, wdc65816_decode(cpu, OPTYPE_LONG, op24)); } break;
  case 0x50: { sprintf(t, "bvc $%.4x     [%.6x]", (u16)(wdc65816_decode(cpu, OPTYPE_RELB, op8)), wdc65816_decode(cpu, OPTYPE_RELB, op8)); } break;
  case 0x51: { sprintf(t, "eor ($%.2x),y   [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_IDPY, op8)); } break;
  case 0x52: { sprintf(t, "eor ($%.2x)     [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_IDP, op8)); } break;
  case 0x53: { sprintf(t, "eor ($%.2x,s),y [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_ISRY, op8)); } break;
  case 0x54: { sprintf(t, "mvn $%.2x,$%.2x           ", op1, op8); } break;
  case 0x55: { sprintf(t, "eor $%.2x,x     [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_DPX, op8)); } break;
  case 0x56: { sprintf(t, "lsr $%.2x,x     [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_DPX, op8)); } break;
  case 0x57: { sprintf(t, "eor [$%.2x],y   [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_ILDPY, op8)); } break;
  case 0x58: { sprintf(t, "cli                   "); } break;
  case 0x59: { sprintf(t, "eor $%.4x,y   [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDRY, op16)); } break;
  case 0x5a: { sprintf(t, "phy                   "); } break;
  case 0x5b: { sprintf(t, "tcd                   "); } break;
  case 0x5c: { sprintf(t, "jml $%.6x   [%.6x]", op24, wdc65816_decode(cpu, OPTYPE_LONG, op24)); } break;
  case 0x5d: { sprintf(t, "eor $%.4x,x   [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDRX, op16)); } break;
  case 0x5e: { sprintf(t, "lsr $%.4x,x   [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDRX, op16)); } break;
  case 0x5f: { sprintf(t, "eor $%.6x,x [%.6x]", op24, wdc65816_decode(cpu, OPTYPE_LONGX, op24)); } break;
  case 0x60: { sprintf(t, "rts                   "); } break;
  case 0x61: { sprintf(t, "adc ($%.2x,x)   [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_IDPX, op8)); } break;
  case 0x62: { sprintf(t, "per $%.4x     [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDR, op16)); } break;
  case 0x63: { sprintf(t, "adc $%.2x,s     [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_SR, op8)); } break;
  case 0x64: { sprintf(t, "stz $%.2x       [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_DP, op8)); } break;
  case 0x65: { sprintf(t, "adc $%.2x       [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_DP, op8)); } break;
  case 0x66: { sprintf(t, "ror $%.2x       [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_DP, op8)); } break;
  case 0x67: { sprintf(t, "adc [$%.2x]     [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_ILDP, op8)); } break;
  case 0x68: { sprintf(t, "pla                   "); } break;
  case 0x69: { if(a8) sprintf(t, "adc #$%.2x              ", op8);
               else   sprintf(t, "adc #$%.4x            ", op16); } break;
  case 0x6a: { sprintf(t, "ror a                 "); } break;
  case 0x6b: { sprintf(t, "rtl                   "); } break;
  case 0x6c: { sprintf(t, "jmp ($%.4x)   [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_IADDR_PC, op16)); } break;
  case 0x6d: { sprintf(t, "adc $%.4x     [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDR, op16)); } break;
  case 0x6e: { sprintf(t, "ror $%.4x     [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDR, op16)); } break;
  case 0x6f: { sprintf(t, "adc $%.6x   [%.6x]", op24, wdc65816_decode(cpu, OPTYPE_LONG, op24)); } break;
  case 0x70: { sprintf(t, "bvs $%.4x     [%.6x]", (u16)(wdc65816_decode(cpu, OPTYPE_RELB, op8)), wdc65816_decode(cpu, OPTYPE_RELB, op8)); } break;
  case 0x71: { sprintf(t, "adc ($%.2x),y   [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_IDPY, op8)); } break;
  case 0x72: { sprintf(t, "adc ($%.2x)     [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_IDP, op8)); } break;
  case 0x73: { sprintf(t, "adc ($%.2x,s),y [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_ISRY, op8)); } break;
  case 0x74: { sprintf(t, "stz $%.2x,x     [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_DPX, op8)); } break;
  case 0x75: { sprintf(t, "adc $%.2x,x     [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_DPX, op8)); } break;
  case 0x76: { sprintf(t, "ror $%.2x,x     [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_DPX, op8)); } break;
  case 0x77: { sprintf(t, "adc [$%.2x],y   [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_ILDPY, op8)); } break;
  case 0x78: { sprintf(t, "sei                   "); } break;
  case 0x79: { sprintf(t, "adc $%.4x,y   [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDRY, op16)); } break;
  case 0x7a: { sprintf(t, "ply                   "); } break;
  case 0x7b: { sprintf(t, "tdc                   "); } break;
  case 0x7c: { sprintf(t, "jmp ($%.4x,x) [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_IADDRX, op16)); } break;
  case 0x7d: { sprintf(t, "adc $%.4x,x   [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDRX, op16)); } break;
  case 0x7e: { sprintf(t, "ror $%.4x,x   [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDRX, op16)); } break;
  case 0x7f: { sprintf(t, "adc $%.6x,x [%.6x]", op24, wdc65816_decode(cpu, OPTYPE_LONGX, op24)); } break;
  case 0x80: { sprintf(t, "bra $%.4x     [%.6x]", (u16)(wdc65816_decode(cpu, OPTYPE_RELB, op8)), wdc65816_decode(cpu, OPTYPE_RELB, op8)); } break;
  case 0x81: { sprintf(t, "sta ($%.2x,x)   [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_IDPX, op8)); } break;
  case 0x82: { sprintf(t, "brl $%.4x     [%.6x]", (u16)(wdc65816_decode(cpu, OPTYPE_RELW, op16)), wdc65816_decode(cpu, OPTYPE_RELW, op16)); } break;
  case 0x83: { sprintf(t, "sta $%.2x,s     [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_SR, op8)); } break;
  case 0x84: { sprintf(t, "sty $%.2x       [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_DP, op8)); } break;
  case 0x85: { sprintf(t, "sta $%.2x       [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_DP, op8)); } break;
  case 0x86: { sprintf(t, "stx $%.2x       [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_DP, op8)); } break;
  case 0x87: { sprintf(t, "sta [$%.2x]     [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_ILDP, op8)); } break;
  case 0x88: { sprintf(t, "dey                   "); } break;
  case 0x89: { if(a8) sprintf(t, "bit #$%.2x              ", op8);
               else sprintf(t, "bit #$%.4x            ", op16); } break;
  case 0x8a: { sprintf(t, "txa                   "); } break;
  case 0x8b: { sprintf(t, "phb                   "); } break;
  case 0x8c: { sprintf(t, "sty $%.4x     [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDR, op16)); } break;
  case 0x8d: { sprintf(t, "sta $%.4x     [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDR, op16)); } break;
  case 0x8e: { sprintf(t, "stx $%.4x     [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDR, op16)); } break;
  case 0x8f: { sprintf(t, "sta $%.6x   [%.6x]", op24, wdc65816_decode(cpu, OPTYPE_LONG, op24)); } break;
  case 0x90: { sprintf(t, "bcc $%.4x     [%.6x]", (u16)(wdc65816_decode(cpu, OPTYPE_RELB, op8)), wdc65816_decode(cpu, OPTYPE_RELB, op8)); } break;
  case 0x91: { sprintf(t, "sta ($%.2x),y   [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_IDPY, op8)); } break;
  case 0x92: { sprintf(t, "sta ($%.2x)     [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_IDP, op8)); } break;
  case 0x93: { sprintf(t, "sta ($%.2x,s),y [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_ISRY, op8)); } break;
  case 0x94: { sprintf(t, "sty $%.2x,x     [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_DPX, op8)); } break;
  case 0x95: { sprintf(t, "sta $%.2x,x     [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_DPX, op8)); } break;
  case 0x96: { sprintf(t, "stx $%.2x,y     [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_DPY, op8)); } break;
  case 0x97: { sprintf(t, "sta [$%.2x],y   [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_ILDPY, op8)); } break;
  case 0x98: { sprintf(t, "tya                   "); } break;
  case 0x99: { sprintf(t, "sta $%.4x,y   [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDRY, op16)); } break;
  case 0x9a: { sprintf(t, "txs                   "); } break;
  case 0x9b: { sprintf(t, "txy                   "); } break;
  case 0x9c: { sprintf(t, "stz $%.4x     [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDR, op16)); } break;
  case 0x9d: { sprintf(t, "sta $%.4x,x   [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDRX, op16)); } break;
  case 0x9e: { sprintf(t, "stz $%.4x,x   [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDRX, op16)); } break;
  case 0x9f: { sprintf(t, "sta $%.6x,x [%.6x]", op24, wdc65816_decode(cpu, OPTYPE_LONGX, op24)); } break;
  case 0xa0: { if(x8) sprintf(t, "ldy #$%.2x              ", op8);
               else   sprintf(t, "ldy #$%.4x            ", op16); } break;
  case 0xa1: { sprintf(t, "lda ($%.2x,x)   [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_IDPX, op8)); } break;
  case 0xa2: { if(x8) sprintf(t, "ldx #$%.2x              ", op8);
               else   sprintf(t, "ldx #$%.4x            ", op16); } break;
  case 0xa3: { sprintf(t, "lda $%.2x,s     [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_SR, op8)); } break;
  case 0xa4: { sprintf(t, "ldy $%.2x       [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_DP, op8)); } break;
  case 0xa5: { sprintf(t, "lda $%.2x       [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_DP, op8)); } break;
  case 0xa6: { sprintf(t, "ldx $%.2x       [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_DP, op8)); } break;
  case 0xa7: { sprintf(t, "lda [$%.2x]     [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_ILDP, op8)); } break;
  case 0xa8: { sprintf(t, "tay                   "); } break;
  case 0xa9: { if(a8) sprintf(t, "lda #$%.2x              ", op8);
               else   sprintf(t, "lda #$%.4x            ", op16); } break;
  case 0xaa: { sprintf(t, "tax                   "); } break;
  case 0xab: { sprintf(t, "plb                   "); } break;
  case 0xac: { sprintf(t, "ldy $%.4x     [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDR, op16)); } break;
  case 0xad: { sprintf(t, "lda $%.4x     [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDR, op16)); } break;
  case 0xae: { sprintf(t, "ldx $%.4x     [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDR, op16)); } break;
  case 0xaf: { sprintf(t, "lda $%.6x   [%.6x]", op24, wdc65816_decode(cpu, OPTYPE_LONG, op24)); } break;
  case 0xb0: { sprintf(t, "bcs $%.4x     [%.6x]", (u16)(wdc65816_decode(cpu, OPTYPE_RELB, op8)), wdc65816_decode(cpu, OPTYPE_RELB, op8)); } break;
  case 0xb1: { sprintf(t, "lda ($%.2x),y   [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_IDPY, op8)); } break;
  case 0xb2: { sprintf(t, "lda ($%.2x)     [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_IDP, op8)); } break;
  case 0xb3: { sprintf(t, "lda ($%.2x,s),y [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_ISRY, op8)); } break;
  case 0xb4: { sprintf(t, "ldy $%.2x,x     [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_DPX, op8)); } break;
  case 0xb5: { sprintf(t, "lda $%.2x,x     [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_DPX, op8)); } break;
  case 0xb6: { sprintf(t, "ldx $%.2x,y     [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_DPY, op8)); } break;
  case 0xb7: { sprintf(t, "lda [$%.2x],y   [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_ILDPY, op8)); } break;
  case 0xb8: { sprintf(t, "clv                   "); } break;
  case 0xb9: { sprintf(t, "lda $%.4x,y   [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDRY, op16)); } break;
  case 0xba: { sprintf(t, "tsx                   "); } break;
  case 0xbb: { sprintf(t, "tyx                   "); } break;
  case 0xbc: { sprintf(t, "ldy $%.4x,x   [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDRX, op16)); } break;
  case 0xbd: { sprintf(t, "lda $%.4x,x   [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDRX, op16)); } break;
  case 0xbe: { sprintf(t, "ldx $%.4x,y   [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDRY, op16)); } break;
  case 0xbf: { sprintf(t, "lda $%.6x,x [%.6x]", op24, wdc65816_decode(cpu, OPTYPE_LONGX, op24)); } break;
  case 0xc0: { if(x8) sprintf(t, "cpy #$%.2x              ", op8);
               else  sprintf(t, "cpy #$%.4x            ", op16); } break;
  case 0xc1: { sprintf(t, "cmp ($%.2x,x)   [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_IDPX, op8)); } break;
  case 0xc2: { sprintf(t, "rep #$%.2x              ", op8); } break;
  case 0xc3: { sprintf(t, "cmp $%.2x,s     [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_SR, op8)); } break;
  case 0xc4: { sprintf(t, "cpy $%.2x       [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_DP, op8)); } break;
  case 0xc5: { sprintf(t, "cmp $%.2x       [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_DP, op8)); } break;
  case 0xc6: { sprintf(t, "dec $%.2x       [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_DP, op8)); } break;
  case 0xc7: { sprintf(t, "cmp [$%.2x]     [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_ILDP, op8)); } break;
  case 0xc8: { sprintf(t, "iny                   "); } break;
  case 0xc9: { if(a8) sprintf(t, "cmp #$%.2x              ", op8);
               else sprintf(t, "cmp #$%.4x            ", op16); } break;
  case 0xca: { sprintf(t, "dex                   "); } break;
  case 0xcb: { sprintf(t, "wai                   "); } break;
  case 0xcc: { sprintf(t, "cpy $%.4x     [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDR, op16)); } break;
  case 0xcd: { sprintf(t, "cmp $%.4x     [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDR, op16)); } break;
  case 0xce: { sprintf(t, "dec $%.4x     [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDR, op16)); } break;
  case 0xcf: { sprintf(t, "cmp $%.6x   [%.6x]", op24, wdc65816_decode(cpu, OPTYPE_LONG, op24)); } break;
  case 0xd0: { sprintf(t, "bne $%.4x     [%.6x]", (u16)(wdc65816_decode(cpu, OPTYPE_RELB, op8)), wdc65816_decode(cpu, OPTYPE_RELB, op8)); } break;
  case 0xd1: { sprintf(t, "cmp ($%.2x),y   [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_IDPY, op8)); } break;
  case 0xd2: { sprintf(t, "cmp ($%.2x)     [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_IDP, op8)); } break;
  case 0xd3: { sprintf(t, "cmp ($%.2x,s),y [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_ISRY, op8)); } break;
  case 0xd4: { sprintf(t, "pei ($%.2x)     [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_IDP, op8)); } break;
  case 0xd5: { sprintf(t, "cmp $%.2x,x     [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_DPX, op8)); } break;
  case 0xd6: { sprintf(t, "dec $%.2x,x     [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_DPX, op8)); } break;
  case 0xd7: { sprintf(t, "cmp [$%.2x],y   [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_ILDPY, op8)); } break;
  case 0xd8: { sprintf(t, "cld                   "); } break;
  case 0xd9: { sprintf(t, "cmp $%.4x,y   [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDRY, op16)); } break;
  case 0xda: { sprintf(t, "phx                   "); } break;
  case 0xdb: { sprintf(t, "stp                   "); } break;
  case 0xdc: { sprintf(t, "jmp [$%.4x]   [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ILADDR, op16)); } break;
  case 0xdd: { sprintf(t, "cmp $%.4x,x   [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDRX, op16)); } break;
  case 0xde: { sprintf(t, "dec $%.4x,x   [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDRX, op16)); } break;
  case 0xdf: { sprintf(t, "cmp $%.6x,x [%.6x]", op24, wdc65816_decode(cpu, OPTYPE_LONGX, op24)); } break;
  case 0xe0: { if(x8) sprintf(t, "cpx #$%.2x              ", op8);
               else sprintf(t, "cpx #$%.4x            ", op16); } break;
  case 0xe1: { sprintf(t, "sbc ($%.2x,x)   [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_IDPX, op8)); } break;
  case 0xe2: { sprintf(t, "sep #$%.2x              ", op8); } break;
  case 0xe3: { sprintf(t, "sbc $%.2x,s     [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_SR, op8)); } break;
  case 0xe4: { sprintf(t, "cpx $%.2x       [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_DP, op8)); } break;
  case 0xe5: { sprintf(t, "sbc $%.2x       [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_DP, op8)); } break;
  case 0xe6: { sprintf(t, "inc $%.2x       [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_DP, op8)); } break;
  case 0xe7: { sprintf(t, "sbc [$%.2x]     [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_ILDP, op8)); } break;
  case 0xe8: { sprintf(t, "inx                   "); } break;
  case 0xe9: { if(a8) sprintf(t, "sbc #$%.2x              ", op8);
               else sprintf(t, "sbc #$%.4x            ", op16); } break;
  case 0xea: { sprintf(t, "nop                   "); } break;
  case 0xeb: { sprintf(t, "xba                   "); } break;
  case 0xec: { sprintf(t, "cpx $%.4x     [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDR, op16)); } break;
  case 0xed: { sprintf(t, "sbc $%.4x     [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDR, op16)); } break;
  case 0xee: { sprintf(t, "inc $%.4x     [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDR, op16)); } break;
  case 0xef: { sprintf(t, "sbc $%.6x   [%.6x]", op24, wdc65816_decode(cpu, OPTYPE_LONG, op24)); } break;
  case 0xf0: { sprintf(t, "beq $%.4x     [%.6x]", (u16)(wdc65816_decode(cpu, OPTYPE_RELB, op8)), wdc65816_decode(cpu, OPTYPE_RELB, op8)); } break;
  case 0xf1: { sprintf(t, "sbc ($%.2x),y   [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_IDPY, op8)); } break;
  case 0xf2: { sprintf(t, "sbc ($%.2x)     [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_IDP, op8)); } break;
  case 0xf3: { sprintf(t, "sbc ($%.2x,s),y [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_ISRY, op8)); } break;
  case 0xf4: { sprintf(t, "pea $%.4x     [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDR, op16)); } break;
  case 0xf5: { sprintf(t, "sbc $%.2x,x     [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_DPX, op8)); } break;
  case 0xf6: { sprintf(t, "inc $%.2x,x     [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_DPX, op8)); } break;
  case 0xf7: { sprintf(t, "sbc [$%.2x],y   [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_ILDPY, op8)); } break;
  case 0xf8: { sprintf(t, "sed                   "); } break;
  case 0xf9: { sprintf(t, "sbc $%.4x,y   [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDRY, op16)); } break;
  case 0xfa: { sprintf(t, "plx                   "); } break;
  case 0xfb: { sprintf(t, "xce                   "); } break;
  case 0xfc: { sprintf(t, "jsr ($%.4x,x) [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_IADDRX, op16)); } break;
  case 0xfd: { sprintf(t, "sbc $%.4x,x   [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDRX, op16)); } break;
  case 0xfe: { sprintf(t, "inc $%.4x,x   [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDRX, op16)); } break;
  case 0xff: { sprintf(t, "sbc $%.6x,x [%.6x]", op24, wdc65816_decode(cpu, OPTYPE_LONGX, op24)); } break;
  }

  #undef op8
  #undef op16
  #undef op24
  #undef a8
  #undef x8

  strcat(s, t);
  strcat(s, " ");

  sprintf(t, "A:%.4x X:%.4x Y:%.4x S:%.4x D:%.4x DB:%.2x ",
    cpu->regs.a.w, cpu->regs.x.w, cpu->regs.y.w, cpu->regs.s.w, cpu->regs.d.w, cpu->regs.db);
  strcat(s, t);

  if(cpu->regs.e) {
    sprintf(t, "%c%c%c%c%c%c%c%c",
      cpu->regs.p.n ? 'N' : 'n', cpu->regs.p.v ? 'V' : 'v',
      cpu->regs.p.m ? '1' : '0', cpu->regs.p.x ? 'B' : 'b',
      cpu->regs.p.d ? 'D' : 'd', cpu->regs.p.i ? 'I' : 'i',
      cpu->regs.p.z ? 'Z' : 'z', cpu->regs.p.c ? 'C' : 'c');
  } else {
    sprintf(t, "%c%c%c%c%c%c%c%c",
      cpu->regs.p.n ? 'N' : 'n', cpu->regs.p.v ? 'V' : 'v',
      cpu->regs.p.m ? 'M' : 'm', cpu->regs.p.x ? 'X' : 'x',
      cpu->regs.p.d ? 'D' : 'd', cpu->regs.p.i ? 'I' : 'i',
      cpu->regs.p.z ? 'Z' : 'z', cpu->regs.p.c ? 'C' : 'c');
  }

  strcat(s, t);
}
