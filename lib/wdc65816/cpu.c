#include "cpu.h"
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
    for(uint i = 0; i < rom->entries_length; i++)
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

u8 wdc65816_cpu_read(Wdc65816Cpu* cpu, u32 addr) {
    if(wdc65816_breakpoint_list_is_hit(&cpu->breakpoints_read, addr) && !cpu->stop_execution) {
        cpu->stop_execution = 1;
        cpu->breakpoint_address = addr;
        cpu->breakpoint_data = 0;
    }
    return wdc65816_mapper_read(&cpu->read_mapper, addr);
}

void wdc65816_cpu_write(Wdc65816Cpu* cpu, u32 addr, u8 data) {
    if(wdc65816_breakpoint_list_is_hit(&cpu->breakpoints_write, addr) && !cpu->stop_execution) {
        cpu->stop_execution = 1;
        cpu->breakpoint_address = addr;
        cpu->breakpoint_data = data;
    }
    wdc65816_mapper_write(&cpu->write_mapper, addr, data);
}

u16 wdc65816_cpu_read16(Wdc65816Cpu* cpu, u32 addr) {
    return wdc65816_cpu_read(cpu, addr)
        | (wdc65816_cpu_read(cpu, addr + 1) << 4);
}

u32 wdc65816_cpu_read24(Wdc65816Cpu* cpu, u32 addr) {
    return wdc65816_cpu_read(cpu, addr)
        | (wdc65816_cpu_read(cpu, addr + 1) << 8)
        | (wdc65816_cpu_read(cpu, addr + 2) << 16);
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

