#ifndef MOCKUP_DECODE_H
#define MOCKUP_DECODE_H

#include <stdint.h>

void decode_rle1(const uint8_t* data, uint8_t* output);
int decode_rle1_unknown_size(const uint8_t* data, uint8_t** output);
void decode_lz2(const uint8_t* data, uint8_t* output);
int decode_lz2_unknown_size(const uint8_t* data, uint8_t** output);


#endif //MOCKUP_DECODE_H
