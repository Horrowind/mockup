#include "mapper.h"

u32 wdc65816_mapper_reduce(u32 addr, u32 mask) {
    while(mask) {
        u32 bits = (mask & -mask) - 1;
        addr = ((addr >> 1) & ~bits) | (addr & bits);
        mask = (mask & (mask - 1)) >> 1;
    }
    return addr;
}

u32 wdc65816_mapper_mirror(uint addr, uint size) {
    if(size == 0) return 0;
    u32 base = 0;
    u32 mask = 1 << 23;
    while(addr >= size) {
        while(!(addr & mask)) mask >>= 1;
        addr -= mask;
        if(size > mask) {
            size -= mask;
            base += mask;
        }
        mask >>= 1;
    }
    return base + addr;
}
