#ifndef MOCKUP_DECODE_H
#define MOCKUP_DECODE_H

#include <stdint.h>

void decode_rle1(const uint8_t* data, uint8_t* output);
int decode_rle1_get_size(const uint8_t* data);
void decode_lz2(const uint8_t* data, uint8_t* output);
int decode_lz2_get_size(const uint8_t* data);


#endif //MOCKUP_DECODE_H
