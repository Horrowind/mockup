#ifndef MOCKUP_DECODE_H
#define MOCKUP_DECODE_H

#include <datasizes.h>

void decode_rle1(const u8* data, u8* output);
int decode_rle1_get_size(const u8* data);
void decode_lz2(const u8* data, u8* output);
int decode_lz2_get_size(const u8* data);


#endif //MOCKUP_DECODE_H
