#include "cpu.h"
#include "disassembler.h"
#include "memory.h"
#include "table.h"

uint wdc65816_cpu_get_work_buffer_size() {
    return 0x20000 + 0x2500 + 2 * wdc65816_mapper_get_buffer_size();
}

void wdc65816_cpu_init(Wdc65816Cpu* cpu, Wdc65816MapperBuilder* rom, u8* work_buffer) {
    uint mapper_buffer_size = wdc65816_mapper_get_buffer_size();
    cpu->regs.p.b = 0x24;
    cpu->rom  = rom;
    u8** read_map_buffer  = (u8**)(work_buffer + 0 * mapper_buffer_size);
    u8** write_map_buffer = (u8**)(work_buffer + 1 * mapper_buffer_size);
    cpu->ram              =        work_buffer + 2 * mapper_buffer_size + 0x000000;
    cpu->sreg             =        work_buffer + 2 * mapper_buffer_size + 0x020000;
    
    cpu->regs.pc.d = 0x000000;
    wdc65816_breakpoint_list_init(&cpu->breakpoints_exec);
    wdc65816_breakpoint_list_init(&cpu->breakpoints_read);
    wdc65816_breakpoint_list_init(&cpu->breakpoints_write);
    wdc65816_cpu_clear(cpu);
#ifdef DEBUG_PRINT_CPU_STATE
    cpu->debug = 1;
#endif

    Wdc65816MapperEntry ram_entry1 = {
        .map = {
            .bank_low    =   0x00,
            .bank_high   =   0x3F,
            .addr_low    = 0x0000,
            .addr_high   = 0x1FFF,
            .size        = 0x2000,
            .content     = L("Work"),
        },
        .data = buffer(cpu->ram, 0x2000),
    };
        
    Wdc65816MapperEntry ram_entry2 = {
        .map = {
            .bank_low    =   0x80,
            .bank_high   =   0xBF,
            .addr_low    = 0x0000,
            .addr_high   = 0x1FFF,
            .size        = 0x2000,
            .content     = L("Work"),
        },
        .data = buffer(cpu->ram, 0x2000),
    };

    Wdc65816MapperEntry ram_entry3 = {
        .map = {
            .bank_low    =   0x7E,
            .bank_high   =   0x7F,
            .addr_low    = 0x0000,
            .addr_high   = 0xFFFF,
            .size        = 0x20000,
            .content     = L("Work"),
        },
        .data = buffer(cpu->ram, 0x20000),
    };

    Wdc65816MapperEntry sreg_entry = {
        .map = {
            .bank_low  =   0x00,
            .bank_high =   0xFF,
            .addr_low  = 0x2000,
            .addr_high = 0x44FF,
            .size      = 0x2500,
            .content   = L("SReg"),
        },
        .data = buffer(cpu->sreg, 0x2500),
    };

    Wdc65816MapperBuilder read_builder;
    wdc65816_mapper_builder_init(&read_builder);

    Wdc65816MapperBuilder write_builder;
    wdc65816_mapper_builder_init(&write_builder);

    wdc65816_mapper_builder_add(&read_builder,  &ram_entry1);
    wdc65816_mapper_builder_add(&read_builder,  &ram_entry2);
    wdc65816_mapper_builder_add(&read_builder,  &ram_entry3);
    wdc65816_mapper_builder_add(&read_builder,  &sreg_entry);
    wdc65816_mapper_builder_add(&write_builder, &ram_entry1);
    wdc65816_mapper_builder_add(&write_builder, &ram_entry2);
    wdc65816_mapper_builder_add(&write_builder, &ram_entry3);
    wdc65816_mapper_builder_add(&write_builder, &sreg_entry);
    for(int i = 0; i < rom->entries_length; i++)
        wdc65816_mapper_builder_add(&read_builder, rom->entries + i);
        
    wdc65816_mapper_init(&cpu->read_mapper,  &read_builder,  read_map_buffer);
    wdc65816_mapper_init(&cpu->write_mapper, &write_builder, write_map_buffer);

    wdc65816_mapper_builder_deinit(&read_builder);
    wdc65816_mapper_builder_deinit(&write_builder);
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
    zero_buffer(buffer(cpu->ram,  0x20000));
    zero_buffer(buffer(cpu->sreg,  0x2500));
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
  char t[256], u[256], v[256];
  char* s = output;

  pc.d = addr;

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
  case 0x00: { c_string_format_unsafe(t, "brk #$%.2x              ", op8                                         );   } break;
  case 0x01: { c_string_format_unsafe(t, "ora ($%.2x,x)   [%.6x]",   op8,  wdc65816_decode(cpu, OPTYPE_IDPX, op8));   } break;
  case 0x02: { c_string_format_unsafe(t, "cop #$%.2x              ", op8                                         );   } break;
  case 0x03: { c_string_format_unsafe(t, "ora $%.2x,s     [%.6x]",   op8,  wdc65816_decode(cpu, OPTYPE_SR,   op8));   } break;
  case 0x04: { c_string_format_unsafe(t, "tsb $%.2x       [%.6x]",   op8,  wdc65816_decode(cpu, OPTYPE_DP,   op8));   } break;
  case 0x05: { c_string_format_unsafe(t, "ora $%.2x       [%.6x]",   op8,  wdc65816_decode(cpu, OPTYPE_DP,   op8));   } break;
  case 0x06: { c_string_format_unsafe(t, "asl $%.2x       [%.6x]",   op8,  wdc65816_decode(cpu, OPTYPE_DP,   op8));   } break;
  case 0x07: { c_string_format_unsafe(t, "ora [$%.2x]     [%.6x]",   op8,  wdc65816_decode(cpu, OPTYPE_ILDP, op8));   } break;
  case 0x08: { c_string_format_unsafe(t, "php                   "                                                );   } break;
  case 0x09: { if(a8) c_string_format_unsafe(t, "ora #$%.2x              ", op8);
               else c_string_format_unsafe(t, "ora #$%.4x            ", op16); } break;
  case 0x0a: { c_string_format_unsafe(t, "asl a                 "                                                );   } break;
  case 0x0b: { c_string_format_unsafe(t, "phd                   "); } break;
  case 0x0c: { c_string_format_unsafe(t, "tsb $%.4x     [%.6x]",     op16, wdc65816_decode(cpu, OPTYPE_ADDR,  op16)); } break;
  case 0x0d: { c_string_format_unsafe(t, "ora $%.4x     [%.6x]",     op16, wdc65816_decode(cpu, OPTYPE_ADDR,  op16)); } break;
  case 0x0e: { c_string_format_unsafe(t, "asl $%.4x     [%.6x]",     op16, wdc65816_decode(cpu, OPTYPE_ADDR,  op16)); } break;
  case 0x0f: { c_string_format_unsafe(t, "ora $%.6x   [%.6x]",       op24, wdc65816_decode(cpu, OPTYPE_LONG,  op24)); } break;
  case 0x10: { c_string_format_unsafe(t, "bpl $%.4x     [%.6x]",     (u16)(wdc65816_decode(cpu, OPTYPE_RELB,  op8)),
                                                            wdc65816_decode(cpu, OPTYPE_RELB, op8));   } break;
  case 0x11: { c_string_format_unsafe(t, "ora ($%.2x),y   [%.6x]",   op8,  wdc65816_decode(cpu, OPTYPE_IDPY,  op8));  } break;
  case 0x12: { c_string_format_unsafe(t, "ora ($%.2x)     [%.6x]",   op8,  wdc65816_decode(cpu, OPTYPE_IDP,   op8));  } break;
  case 0x13: { c_string_format_unsafe(t, "ora ($%.2x,s),y [%.6x]",   op8,  wdc65816_decode(cpu, OPTYPE_ISRY,  op8));  } break;
  case 0x14: { c_string_format_unsafe(t, "trb $%.2x       [%.6x]",   op8,  wdc65816_decode(cpu, OPTYPE_DP,    op8));  } break;
  case 0x15: { c_string_format_unsafe(t, "ora $%.2x,x     [%.6x]",   op8,  wdc65816_decode(cpu, OPTYPE_DPX,   op8));  } break;
  case 0x16: { c_string_format_unsafe(t, "asl $%.2x,x     [%.6x]",   op8,  wdc65816_decode(cpu, OPTYPE_DPX,   op8));  } break;
  case 0x17: { c_string_format_unsafe(t, "ora [$%.2x],y   [%.6x]",   op8,  wdc65816_decode(cpu, OPTYPE_ILDPY, op8));  } break;
  case 0x18: { c_string_format_unsafe(t, "clc                   "); } break;
  case 0x19: { c_string_format_unsafe(t, "ora $%.4x,y   [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDRY, op16));     } break;
  case 0x1a: { c_string_format_unsafe(t, "inc                   "); } break;
  case 0x1b: { c_string_format_unsafe(t, "tcs                   "); } break;
  case 0x1c: { c_string_format_unsafe(t, "trb $%.4x     [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDR, op16)); } break;
  case 0x1d: { c_string_format_unsafe(t, "ora $%.4x,x   [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDRX, op16)); } break;
  case 0x1e: { c_string_format_unsafe(t, "asl $%.4x,x   [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDRX, op16)); } break;
  case 0x1f: { c_string_format_unsafe(t, "ora $%.6x,x [%.6x]", op24, wdc65816_decode(cpu, OPTYPE_LONGX, op24)); } break;
  case 0x20: { c_string_format_unsafe(t, "jsr $%.4x     [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDR_PC, op16)); } break;
  case 0x21: { c_string_format_unsafe(t, "and ($%.2x,x)   [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_IDPX, op8)); } break;
  case 0x22: { c_string_format_unsafe(t, "jsl $%.6x   [%.6x]", op24, wdc65816_decode(cpu, OPTYPE_LONG, op24)); } break;
  case 0x23: { c_string_format_unsafe(t, "and $%.2x,s     [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_SR, op8)); } break;
  case 0x24: { c_string_format_unsafe(t, "bit $%.2x       [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_DP, op8)); } break;
  case 0x25: { c_string_format_unsafe(t, "and $%.2x       [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_DP, op8)); } break;
  case 0x26: { c_string_format_unsafe(t, "rol $%.2x       [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_DP, op8)); } break;
  case 0x27: { c_string_format_unsafe(t, "and [$%.2x]     [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_ILDP, op8)); } break;
  case 0x28: { c_string_format_unsafe(t, "plp                   "); } break;
  case 0x29: { if(a8) c_string_format_unsafe(t, "and #$%.2x              ", op8);
               else   c_string_format_unsafe(t, "and #$%.4x            ", op16); } break;
  case 0x2a: { c_string_format_unsafe(t, "rol a                 "); } break;
  case 0x2b: { c_string_format_unsafe(t, "pld                   "); } break;
  case 0x2c: { c_string_format_unsafe(t, "bit $%.4x     [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDR, op16)); } break;
  case 0x2d: { c_string_format_unsafe(t, "and $%.4x     [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDR, op16)); } break;
  case 0x2e: { c_string_format_unsafe(t, "rol $%.4x     [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDR, op16)); } break;
  case 0x2f: { c_string_format_unsafe(t, "and $%.6x   [%.6x]", op24, wdc65816_decode(cpu, OPTYPE_LONG, op24)); } break;
  case 0x30: { c_string_format_unsafe(t, "bmi $%.4x     [%.6x]", (u16)(wdc65816_decode(cpu, OPTYPE_RELB, op8)), wdc65816_decode(cpu, OPTYPE_RELB, op8)); } break;
  case 0x31: { c_string_format_unsafe(t, "and ($%.2x),y   [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_IDPY, op8)); } break;
  case 0x32: { c_string_format_unsafe(t, "and ($%.2x)     [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_IDP, op8)); } break;
  case 0x33: { c_string_format_unsafe(t, "and ($%.2x,s),y [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_ISRY, op8)); } break;
  case 0x34: { c_string_format_unsafe(t, "bit $%.2x,x     [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_DPX, op8)); } break;
  case 0x35: { c_string_format_unsafe(t, "and $%.2x,x     [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_DPX, op8)); } break;
  case 0x36: { c_string_format_unsafe(t, "rol $%.2x,x     [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_DPX, op8)); } break;
  case 0x37: { c_string_format_unsafe(t, "and [$%.2x],y   [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_ILDPY, op8)); } break;
  case 0x38: { c_string_format_unsafe(t, "sec                   "); } break;
  case 0x39: { c_string_format_unsafe(t, "and $%.4x,y   [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDRY, op16)); } break;
  case 0x3a: { c_string_format_unsafe(t, "dec                   "); } break;
  case 0x3b: { c_string_format_unsafe(t, "tsc                   "); } break;
  case 0x3c: { c_string_format_unsafe(t, "bit $%.4x,x   [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDRX, op16)); } break;
  case 0x3d: { c_string_format_unsafe(t, "and $%.4x,x   [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDRX, op16)); } break;
  case 0x3e: { c_string_format_unsafe(t, "rol $%.4x,x   [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDRX, op16)); } break;
  case 0x3f: { c_string_format_unsafe(t, "and $%.6x,x [%.6x]", op24, wdc65816_decode(cpu, OPTYPE_LONGX, op24)); } break;
  case 0x40: { c_string_format_unsafe(t, "rti                   "); } break;
  case 0x41: { c_string_format_unsafe(t, "eor ($%.2x,x)   [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_IDPX, op8)); } break;
  case 0x42: { c_string_format_unsafe(t, "wdm                   "); } break;
  case 0x43: { c_string_format_unsafe(t, "eor $%.2x,s     [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_SR, op8)); } break;
  case 0x44: { c_string_format_unsafe(t, "mvp $%.2x,$%.2x           ", op1, op8); } break;
  case 0x45: { c_string_format_unsafe(t, "eor $%.2x       [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_DP, op8)); } break;
  case 0x46: { c_string_format_unsafe(t, "lsr $%.2x       [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_DP, op8)); } break;
  case 0x47: { c_string_format_unsafe(t, "eor [$%.2x]     [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_ILDP, op8)); } break;
  case 0x48: { c_string_format_unsafe(t, "pha                   "); } break;
  case 0x49: { if(a8) c_string_format_unsafe(t, "eor #$%.2x              ", op8);
               else   c_string_format_unsafe(t, "eor #$%.4x            ", op16); } break;
  case 0x4a: { c_string_format_unsafe(t, "lsr a                 "); } break;
  case 0x4b: { c_string_format_unsafe(t, "phk                   "); } break;
  case 0x4c: { c_string_format_unsafe(t, "jmp $%.4x     [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDR_PC, op16)); } break;
  case 0x4d: { c_string_format_unsafe(t, "eor $%.4x     [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDR, op16)); } break;
  case 0x4e: { c_string_format_unsafe(t, "lsr $%.4x     [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDR, op16)); } break;
  case 0x4f: { c_string_format_unsafe(t, "eor $%.6x   [%.6x]", op24, wdc65816_decode(cpu, OPTYPE_LONG, op24)); } break;
  case 0x50: { c_string_format_unsafe(t, "bvc $%.4x     [%.6x]", (u16)(wdc65816_decode(cpu, OPTYPE_RELB, op8)), wdc65816_decode(cpu, OPTYPE_RELB, op8)); } break;
  case 0x51: { c_string_format_unsafe(t, "eor ($%.2x),y   [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_IDPY, op8)); } break;
  case 0x52: { c_string_format_unsafe(t, "eor ($%.2x)     [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_IDP, op8)); } break;
  case 0x53: { c_string_format_unsafe(t, "eor ($%.2x,s),y [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_ISRY, op8)); } break;
  case 0x54: { c_string_format_unsafe(t, "mvn $%.2x,$%.2x           ", op1, op8); } break;
  case 0x55: { c_string_format_unsafe(t, "eor $%.2x,x     [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_DPX, op8)); } break;
  case 0x56: { c_string_format_unsafe(t, "lsr $%.2x,x     [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_DPX, op8)); } break;
  case 0x57: { c_string_format_unsafe(t, "eor [$%.2x],y   [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_ILDPY, op8)); } break;
  case 0x58: { c_string_format_unsafe(t, "cli                   "); } break;
  case 0x59: { c_string_format_unsafe(t, "eor $%.4x,y   [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDRY, op16)); } break;
  case 0x5a: { c_string_format_unsafe(t, "phy                   "); } break;
  case 0x5b: { c_string_format_unsafe(t, "tcd                   "); } break;
  case 0x5c: { c_string_format_unsafe(t, "jml $%.6x   [%.6x]", op24, wdc65816_decode(cpu, OPTYPE_LONG, op24)); } break;
  case 0x5d: { c_string_format_unsafe(t, "eor $%.4x,x   [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDRX, op16)); } break;
  case 0x5e: { c_string_format_unsafe(t, "lsr $%.4x,x   [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDRX, op16)); } break;
  case 0x5f: { c_string_format_unsafe(t, "eor $%.6x,x [%.6x]", op24, wdc65816_decode(cpu, OPTYPE_LONGX, op24)); } break;
  case 0x60: { c_string_format_unsafe(t, "rts                   "); } break;
  case 0x61: { c_string_format_unsafe(t, "adc ($%.2x,x)   [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_IDPX, op8)); } break;
  case 0x62: { c_string_format_unsafe(t, "per $%.4x     [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDR, op16)); } break;
  case 0x63: { c_string_format_unsafe(t, "adc $%.2x,s     [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_SR, op8)); } break;
  case 0x64: { c_string_format_unsafe(t, "stz $%.2x       [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_DP, op8)); } break;
  case 0x65: { c_string_format_unsafe(t, "adc $%.2x       [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_DP, op8)); } break;
  case 0x66: { c_string_format_unsafe(t, "ror $%.2x       [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_DP, op8)); } break;
  case 0x67: { c_string_format_unsafe(t, "adc [$%.2x]     [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_ILDP, op8)); } break;
  case 0x68: { c_string_format_unsafe(t, "pla                   "); } break;
  case 0x69: { if(a8) c_string_format_unsafe(t, "adc #$%.2x              ", op8);
               else   c_string_format_unsafe(t, "adc #$%.4x            ", op16); } break;
  case 0x6a: { c_string_format_unsafe(t, "ror a                 "); } break;
  case 0x6b: { c_string_format_unsafe(t, "rtl                   "); } break;
  case 0x6c: { c_string_format_unsafe(t, "jmp ($%.4x)   [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_IADDR_PC, op16)); } break;
  case 0x6d: { c_string_format_unsafe(t, "adc $%.4x     [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDR, op16)); } break;
  case 0x6e: { c_string_format_unsafe(t, "ror $%.4x     [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDR, op16)); } break;
  case 0x6f: { c_string_format_unsafe(t, "adc $%.6x   [%.6x]", op24, wdc65816_decode(cpu, OPTYPE_LONG, op24)); } break;
  case 0x70: { c_string_format_unsafe(t, "bvs $%.4x     [%.6x]", (u16)(wdc65816_decode(cpu, OPTYPE_RELB, op8)), wdc65816_decode(cpu, OPTYPE_RELB, op8)); } break;
  case 0x71: { c_string_format_unsafe(t, "adc ($%.2x),y   [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_IDPY, op8)); } break;
  case 0x72: { c_string_format_unsafe(t, "adc ($%.2x)     [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_IDP, op8)); } break;
  case 0x73: { c_string_format_unsafe(t, "adc ($%.2x,s),y [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_ISRY, op8)); } break;
  case 0x74: { c_string_format_unsafe(t, "stz $%.2x,x     [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_DPX, op8)); } break;
  case 0x75: { c_string_format_unsafe(t, "adc $%.2x,x     [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_DPX, op8)); } break;
  case 0x76: { c_string_format_unsafe(t, "ror $%.2x,x     [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_DPX, op8)); } break;
  case 0x77: { c_string_format_unsafe(t, "adc [$%.2x],y   [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_ILDPY, op8)); } break;
  case 0x78: { c_string_format_unsafe(t, "sei                   "); } break;
  case 0x79: { c_string_format_unsafe(t, "adc $%.4x,y   [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDRY, op16)); } break;
  case 0x7a: { c_string_format_unsafe(t, "ply                   "); } break;
  case 0x7b: { c_string_format_unsafe(t, "tdc                   "); } break;
  case 0x7c: { c_string_format_unsafe(t, "jmp ($%.4x,x) [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_IADDRX, op16)); } break;
  case 0x7d: { c_string_format_unsafe(t, "adc $%.4x,x   [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDRX, op16)); } break;
  case 0x7e: { c_string_format_unsafe(t, "ror $%.4x,x   [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDRX, op16)); } break;
  case 0x7f: { c_string_format_unsafe(t, "adc $%.6x,x [%.6x]", op24, wdc65816_decode(cpu, OPTYPE_LONGX, op24)); } break;
  case 0x80: { c_string_format_unsafe(t, "bra $%.4x     [%.6x]", (u16)(wdc65816_decode(cpu, OPTYPE_RELB, op8)), wdc65816_decode(cpu, OPTYPE_RELB, op8)); } break;
  case 0x81: { c_string_format_unsafe(t, "sta ($%.2x,x)   [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_IDPX, op8)); } break;
  case 0x82: { c_string_format_unsafe(t, "brl $%.4x     [%.6x]", (u16)(wdc65816_decode(cpu, OPTYPE_RELW, op16)), wdc65816_decode(cpu, OPTYPE_RELW, op16)); } break;
  case 0x83: { c_string_format_unsafe(t, "sta $%.2x,s     [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_SR, op8)); } break;
  case 0x84: { c_string_format_unsafe(t, "sty $%.2x       [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_DP, op8)); } break;
  case 0x85: { c_string_format_unsafe(t, "sta $%.2x       [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_DP, op8)); } break;
  case 0x86: { c_string_format_unsafe(t, "stx $%.2x       [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_DP, op8)); } break;
  case 0x87: { c_string_format_unsafe(t, "sta [$%.2x]     [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_ILDP, op8)); } break;
  case 0x88: { c_string_format_unsafe(t, "dey                   "); } break;
  case 0x89: { if(a8) c_string_format_unsafe(t, "bit #$%.2x              ", op8);
               else c_string_format_unsafe(t, "bit #$%.4x            ", op16); } break;
  case 0x8a: { c_string_format_unsafe(t, "txa                   "); } break;
  case 0x8b: { c_string_format_unsafe(t, "phb                   "); } break;
  case 0x8c: { c_string_format_unsafe(t, "sty $%.4x     [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDR, op16)); } break;
  case 0x8d: { c_string_format_unsafe(t, "sta $%.4x     [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDR, op16)); } break;
  case 0x8e: { c_string_format_unsafe(t, "stx $%.4x     [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDR, op16)); } break;
  case 0x8f: { c_string_format_unsafe(t, "sta $%.6x   [%.6x]", op24, wdc65816_decode(cpu, OPTYPE_LONG, op24)); } break;
  case 0x90: { c_string_format_unsafe(t, "bcc $%.4x     [%.6x]", (u16)(wdc65816_decode(cpu, OPTYPE_RELB, op8)), wdc65816_decode(cpu, OPTYPE_RELB, op8)); } break;
  case 0x91: { c_string_format_unsafe(t, "sta ($%.2x),y   [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_IDPY, op8)); } break;
  case 0x92: { c_string_format_unsafe(t, "sta ($%.2x)     [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_IDP, op8)); } break;
  case 0x93: { c_string_format_unsafe(t, "sta ($%.2x,s),y [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_ISRY, op8)); } break;
  case 0x94: { c_string_format_unsafe(t, "sty $%.2x,x     [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_DPX, op8)); } break;
  case 0x95: { c_string_format_unsafe(t, "sta $%.2x,x     [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_DPX, op8)); } break;
  case 0x96: { c_string_format_unsafe(t, "stx $%.2x,y     [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_DPY, op8)); } break;
  case 0x97: { c_string_format_unsafe(t, "sta [$%.2x],y   [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_ILDPY, op8)); } break;
  case 0x98: { c_string_format_unsafe(t, "tya                   "); } break;
  case 0x99: { c_string_format_unsafe(t, "sta $%.4x,y   [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDRY, op16)); } break;
  case 0x9a: { c_string_format_unsafe(t, "txs                   "); } break;
  case 0x9b: { c_string_format_unsafe(t, "txy                   "); } break;
  case 0x9c: { c_string_format_unsafe(t, "stz $%.4x     [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDR, op16)); } break;
  case 0x9d: { c_string_format_unsafe(t, "sta $%.4x,x   [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDRX, op16)); } break;
  case 0x9e: { c_string_format_unsafe(t, "stz $%.4x,x   [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDRX, op16)); } break;
  case 0x9f: { c_string_format_unsafe(t, "sta $%.6x,x [%.6x]", op24, wdc65816_decode(cpu, OPTYPE_LONGX, op24)); } break;
  case 0xa0: { if(x8) c_string_format_unsafe(t, "ldy #$%.2x              ", op8);
               else   c_string_format_unsafe(t, "ldy #$%.4x            ", op16); } break;
  case 0xa1: { c_string_format_unsafe(t, "lda ($%.2x,x)   [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_IDPX, op8)); } break;
  case 0xa2: { if(x8) c_string_format_unsafe(t, "ldx #$%.2x              ", op8);
               else   c_string_format_unsafe(t, "ldx #$%.4x            ", op16); } break;
  case 0xa3: { c_string_format_unsafe(t, "lda $%.2x,s     [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_SR, op8)); } break;
  case 0xa4: { c_string_format_unsafe(t, "ldy $%.2x       [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_DP, op8)); } break;
  case 0xa5: { c_string_format_unsafe(t, "lda $%.2x       [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_DP, op8)); } break;
  case 0xa6: { c_string_format_unsafe(t, "ldx $%.2x       [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_DP, op8)); } break;
  case 0xa7: { c_string_format_unsafe(t, "lda [$%.2x]     [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_ILDP, op8)); } break;
  case 0xa8: { c_string_format_unsafe(t, "tay                   "); } break;
  case 0xa9: { if(a8) c_string_format_unsafe(t, "lda #$%.2x              ", op8);
               else   c_string_format_unsafe(t, "lda #$%.4x            ", op16); } break;
  case 0xaa: { c_string_format_unsafe(t, "tax                   "); } break;
  case 0xab: { c_string_format_unsafe(t, "plb                   "); } break;
  case 0xac: { c_string_format_unsafe(t, "ldy $%.4x     [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDR, op16)); } break;
  case 0xad: { c_string_format_unsafe(t, "lda $%.4x     [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDR, op16)); } break;
  case 0xae: { c_string_format_unsafe(t, "ldx $%.4x     [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDR, op16)); } break;
  case 0xaf: { c_string_format_unsafe(t, "lda $%.6x   [%.6x]", op24, wdc65816_decode(cpu, OPTYPE_LONG, op24)); } break;
  case 0xb0: { c_string_format_unsafe(t, "bcs $%.4x     [%.6x]", (u16)(wdc65816_decode(cpu, OPTYPE_RELB, op8)), wdc65816_decode(cpu, OPTYPE_RELB, op8)); } break;
  case 0xb1: { c_string_format_unsafe(t, "lda ($%.2x),y   [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_IDPY, op8)); } break;
  case 0xb2: { c_string_format_unsafe(t, "lda ($%.2x)     [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_IDP, op8)); } break;
  case 0xb3: { c_string_format_unsafe(t, "lda ($%.2x,s),y [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_ISRY, op8)); } break;
  case 0xb4: { c_string_format_unsafe(t, "ldy $%.2x,x     [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_DPX, op8)); } break;
  case 0xb5: { c_string_format_unsafe(t, "lda $%.2x,x     [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_DPX, op8)); } break;
  case 0xb6: { c_string_format_unsafe(t, "ldx $%.2x,y     [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_DPY, op8)); } break;
  case 0xb7: { c_string_format_unsafe(t, "lda [$%.2x],y   [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_ILDPY, op8)); } break;
  case 0xb8: { c_string_format_unsafe(t, "clv                   "); } break;
  case 0xb9: { c_string_format_unsafe(t, "lda $%.4x,y   [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDRY, op16)); } break;
  case 0xba: { c_string_format_unsafe(t, "tsx                   "); } break;
  case 0xbb: { c_string_format_unsafe(t, "tyx                   "); } break;
  case 0xbc: { c_string_format_unsafe(t, "ldy $%.4x,x   [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDRX, op16)); } break;
  case 0xbd: { c_string_format_unsafe(t, "lda $%.4x,x   [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDRX, op16)); } break;
  case 0xbe: { c_string_format_unsafe(t, "ldx $%.4x,y   [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDRY, op16)); } break;
  case 0xbf: { c_string_format_unsafe(t, "lda $%.6x,x [%.6x]", op24, wdc65816_decode(cpu, OPTYPE_LONGX, op24)); } break;
  case 0xc0: { if(x8) c_string_format_unsafe(t, "cpy #$%.2x              ", op8);
               else  c_string_format_unsafe(t, "cpy #$%.4x            ", op16); } break;
  case 0xc1: { c_string_format_unsafe(t, "cmp ($%.2x,x)   [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_IDPX, op8)); } break;
  case 0xc2: { c_string_format_unsafe(t, "rep #$%.2x              ", op8); } break;
  case 0xc3: { c_string_format_unsafe(t, "cmp $%.2x,s     [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_SR, op8)); } break;
  case 0xc4: { c_string_format_unsafe(t, "cpy $%.2x       [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_DP, op8)); } break;
  case 0xc5: { c_string_format_unsafe(t, "cmp $%.2x       [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_DP, op8)); } break;
  case 0xc6: { c_string_format_unsafe(t, "dec $%.2x       [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_DP, op8)); } break;
  case 0xc7: { c_string_format_unsafe(t, "cmp [$%.2x]     [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_ILDP, op8)); } break;
  case 0xc8: { c_string_format_unsafe(t, "iny                   "); } break;
  case 0xc9: { if(a8) c_string_format_unsafe(t, "cmp #$%.2x              ", op8);
               else c_string_format_unsafe(t, "cmp #$%.4x            ", op16); } break;
  case 0xca: { c_string_format_unsafe(t, "dex                   "); } break;
  case 0xcb: { c_string_format_unsafe(t, "wai                   "); } break;
  case 0xcc: { c_string_format_unsafe(t, "cpy $%.4x     [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDR, op16)); } break;
  case 0xcd: { c_string_format_unsafe(t, "cmp $%.4x     [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDR, op16)); } break;
  case 0xce: { c_string_format_unsafe(t, "dec $%.4x     [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDR, op16)); } break;
  case 0xcf: { c_string_format_unsafe(t, "cmp $%.6x   [%.6x]", op24, wdc65816_decode(cpu, OPTYPE_LONG, op24)); } break;
  case 0xd0: { c_string_format_unsafe(t, "bne $%.4x     [%.6x]", (u16)(wdc65816_decode(cpu, OPTYPE_RELB, op8)), wdc65816_decode(cpu, OPTYPE_RELB, op8)); } break;
  case 0xd1: { c_string_format_unsafe(t, "cmp ($%.2x),y   [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_IDPY, op8)); } break;
  case 0xd2: { c_string_format_unsafe(t, "cmp ($%.2x)     [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_IDP, op8)); } break;
  case 0xd3: { c_string_format_unsafe(t, "cmp ($%.2x,s),y [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_ISRY, op8)); } break;
  case 0xd4: { c_string_format_unsafe(t, "pei ($%.2x)     [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_IDP, op8)); } break;
  case 0xd5: { c_string_format_unsafe(t, "cmp $%.2x,x     [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_DPX, op8)); } break;
  case 0xd6: { c_string_format_unsafe(t, "dec $%.2x,x     [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_DPX, op8)); } break;
  case 0xd7: { c_string_format_unsafe(t, "cmp [$%.2x],y   [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_ILDPY, op8)); } break;
  case 0xd8: { c_string_format_unsafe(t, "cld                   "); } break;
  case 0xd9: { c_string_format_unsafe(t, "cmp $%.4x,y   [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDRY, op16)); } break;
  case 0xda: { c_string_format_unsafe(t, "phx                   "); } break;
  case 0xdb: { c_string_format_unsafe(t, "stp                   "); } break;
  case 0xdc: { c_string_format_unsafe(t, "jmp [$%.4x]   [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ILADDR, op16)); } break;
  case 0xdd: { c_string_format_unsafe(t, "cmp $%.4x,x   [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDRX, op16)); } break;
  case 0xde: { c_string_format_unsafe(t, "dec $%.4x,x   [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDRX, op16)); } break;
  case 0xdf: { c_string_format_unsafe(t, "cmp $%.6x,x [%.6x]", op24, wdc65816_decode(cpu, OPTYPE_LONGX, op24)); } break;
  case 0xe0: { if(x8) c_string_format_unsafe(t, "cpx #$%.2x              ", op8);
               else c_string_format_unsafe(t, "cpx #$%.4x            ", op16); } break;
  case 0xe1: { c_string_format_unsafe(t, "sbc ($%.2x,x)   [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_IDPX, op8)); } break;
  case 0xe2: { c_string_format_unsafe(t, "sep #$%.2x              ", op8); } break;
  case 0xe3: { c_string_format_unsafe(t, "sbc $%.2x,s     [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_SR, op8)); } break;
  case 0xe4: { c_string_format_unsafe(t, "cpx $%.2x       [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_DP, op8)); } break;
  case 0xe5: { c_string_format_unsafe(t, "sbc $%.2x       [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_DP, op8)); } break;
  case 0xe6: { c_string_format_unsafe(t, "inc $%.2x       [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_DP, op8)); } break;
  case 0xe7: { c_string_format_unsafe(t, "sbc [$%.2x]     [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_ILDP, op8)); } break;
  case 0xe8: { c_string_format_unsafe(t, "inx                   "); } break;
  case 0xe9: { if(a8) c_string_format_unsafe(t, "sbc #$%.2x              ", op8);
               else c_string_format_unsafe(t, "sbc #$%.4x            ", op16); } break;
  case 0xea: { c_string_format_unsafe(t, "nop                   "); } break;
  case 0xeb: { c_string_format_unsafe(t, "xba                   "); } break;
  case 0xec: { c_string_format_unsafe(t, "cpx $%.4x     [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDR, op16)); } break;
  case 0xed: { c_string_format_unsafe(t, "sbc $%.4x     [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDR, op16)); } break;
  case 0xee: { c_string_format_unsafe(t, "inc $%.4x     [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDR, op16)); } break;
  case 0xef: { c_string_format_unsafe(t, "sbc $%.6x   [%.6x]", op24, wdc65816_decode(cpu, OPTYPE_LONG, op24)); } break;
  case 0xf0: { c_string_format_unsafe(t, "beq $%.4x     [%.6x]", (u16)(wdc65816_decode(cpu, OPTYPE_RELB, op8)), wdc65816_decode(cpu, OPTYPE_RELB, op8)); } break;
  case 0xf1: { c_string_format_unsafe(t, "sbc ($%.2x),y   [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_IDPY, op8)); } break;
  case 0xf2: { c_string_format_unsafe(t, "sbc ($%.2x)     [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_IDP, op8)); } break;
  case 0xf3: { c_string_format_unsafe(t, "sbc ($%.2x,s),y [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_ISRY, op8)); } break;
  case 0xf4: { c_string_format_unsafe(t, "pea $%.4x     [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDR, op16)); } break;
  case 0xf5: { c_string_format_unsafe(t, "sbc $%.2x,x     [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_DPX, op8)); } break;
  case 0xf6: { c_string_format_unsafe(t, "inc $%.2x,x     [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_DPX, op8)); } break;
  case 0xf7: { c_string_format_unsafe(t, "sbc [$%.2x],y   [%.6x]", op8, wdc65816_decode(cpu, OPTYPE_ILDPY, op8)); } break;
  case 0xf8: { c_string_format_unsafe(t, "sed                   "); } break;
  case 0xf9: { c_string_format_unsafe(t, "sbc $%.4x,y   [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDRY, op16)); } break;
  case 0xfa: { c_string_format_unsafe(t, "plx                   "); } break;
  case 0xfb: { c_string_format_unsafe(t, "xce                   "); } break;
  case 0xfc: { c_string_format_unsafe(t, "jsr ($%.4x,x) [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_IADDRX, op16)); } break;
  case 0xfd: { c_string_format_unsafe(t, "sbc $%.4x,x   [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDRX, op16)); } break;
  case 0xfe: { c_string_format_unsafe(t, "inc $%.4x,x   [%.6x]", op16, wdc65816_decode(cpu, OPTYPE_ADDRX, op16)); } break;
  case 0xff: { c_string_format_unsafe(t, "sbc $%.6x,x [%.6x]", op24, wdc65816_decode(cpu, OPTYPE_LONGX, op24)); } break;
  }

  #undef op8
  #undef op16
  #undef op24
  #undef a8
  #undef x8

  c_string_format_unsafe(u, "A:%.4x X:%.4x Y:%.4x S:%.4x D:%.4x DB:%.2x ",
    cpu->regs.a.w, cpu->regs.x.w, cpu->regs.y.w, cpu->regs.s.w, cpu->regs.d.w, cpu->regs.db);

  if(cpu->regs.e) {
    c_string_format_unsafe(v, "%c%c%c%c%c%c%c%c",
      cpu->regs.p.n ? 'N' : 'n', cpu->regs.p.v ? 'V' : 'v',
      cpu->regs.p.m ? '1' : '0', cpu->regs.p.x ? 'B' : 'b',
      cpu->regs.p.d ? 'D' : 'd', cpu->regs.p.i ? 'I' : 'i',
      cpu->regs.p.z ? 'Z' : 'z', cpu->regs.p.c ? 'C' : 'c');
  } else {
    c_string_format_unsafe(v, "%c%c%c%c%c%c%c%c",
      cpu->regs.p.n ? 'N' : 'n', cpu->regs.p.v ? 'V' : 'v',
      cpu->regs.p.m ? 'M' : 'm', cpu->regs.p.x ? 'X' : 'x',
      cpu->regs.p.d ? 'D' : 'd', cpu->regs.p.i ? 'I' : 'i',
      cpu->regs.p.z ? 'Z' : 'z', cpu->regs.p.c ? 'C' : 'c');
  }

  c_string_format_unsafe(s, "%.6x %s %s%s",  (u32)pc.d, t, u, v);
}
