#ifndef WDC65816_DYNAREC_H
#define WDC65816_DYNAREC_H

#include "base/base.h"

struct WDC65816Cpu;

typedef union {
    struct {
        u32 fill_ : 5;
        u32 e     : 1;
        u32 m     : 1;
        u32 x     : 1;
        u32 addr  : 24;
    };
    u32 k;
} WDC65816BBKey;
#define WCC65816_BB_KEY_MASK 0x07FFFFFF
_Static_assert(sizeof(WDC65816BBKey) == 4, "sizeof(wdc65816_bb_key_t) == 4");

typedef void(*wdc65816_bb_function_t)(struct WDC65816Cpu* cpu);

typedef struct {
    WDC65816BBKey key;
    wdc65816_bb_function_t exec;
} WDC65816BB;

u32 wdc65816_bb_equal(WDC65816BB bb1, WDC65816BB bb2);
u32 wdc65816_bb_hash(WDC65816BB bb);

define_hashmap(WDC65816BBMap, wdc65816_bb_map, WDC65816BB, wdc65816_bb_hash, wdc65816_bb_equal);

void wdc65816_cpu_run2(struct WDC65816Cpu* cpu);

#endif //WDC65816_DYNAREC_H
