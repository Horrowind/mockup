#ifndef MOCKUP_DECODE_H
#define MOCKUP_DECODE_H

#include "base/base.h"


// TODO: Add length to buffer / replace u8* with buffer_t
void decode_rle1(const u8* data, u8* output);
int decode_rle1_get_size(const u8* data);
void decode_lz2(const u8* data, u8* output);
int decode_lz2_get_size(const u8* data);

void encode_lz2(u8* data_begin, u8* data_end, u8* output);


#endif //MOCKUP_DECODE_H
