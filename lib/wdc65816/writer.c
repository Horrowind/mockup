#include "writer.h"

void wdc65816_writer_init(Wdc65816Writer* writer, Wdc65816MapperBuilder write_mapper) {
    *writer = (Wdc65816Writer) { };
    for(int addr = 0; addr < 1 << 24; addr++) {

    }
    
    unused(write_mapper);
}