#ifndef WDC65816_WRITER
#define WDC65816_WRITER

#include "mapper.h"

typedef struct {
    u8*  data;
    uint length;
} Wdc65816WriterEntry;

typedef struct {
    Wdc65816WriterEntry entries[1024];
    uint entries_length;
} Wdc65816Writer;

void wdc65816_writer_init(Wdc65816Writer* writer, Wdc65816MapperBuilder write_mapper);

#endif //WDC65816_WRITER