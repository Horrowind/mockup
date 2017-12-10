#include "cpu.h"
#include "dynarec.h"


u32 wdc65816_bb_equal(WDC65816BB bb1, WDC65816BB bb2) {
    return (bb1.key.k & WCC65816_BB_KEY_MASK) == (bb2.key.k & WCC65816_BB_KEY_MASK);
}

u32 wdc65816_bb_hash(WDC65816BB bb) {
    return bb.key.k & WCC65816_BB_KEY_MASK;
}

implement_hashmap(WDC65816BBMap, wdc65816_bb_map, WDC65816BB, wdc65816_bb_hash, wdc65816_bb_equal);


void wdc65816_generate_bb(WDC65816Cpu* cpu, WDC65816BBKey key) {
    
}

void wdc65816_cpu_run2(WDC65816Cpu* cpu) {
    WDC65816BB entry;
    entry.key.e = cpu->regs.e;
    entry.key.m = cpu->regs.p.m;
    entry.key.x = cpu->regs.p.x;

    WDC65816BB* found = wdc65816_bb_map_find(&cpu->bb_map, entry);

    if(found) {
        found->exec(cpu);
    } else {
        wdc65816_generate_bb(cpu, entry.key);
    }
}
