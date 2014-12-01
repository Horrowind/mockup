#include <stdint.h>
#include <stdlib.h>

void base64_init();
void base64_free();

char* base64_encode(const uint8_t data,  size_t input_length, size_t* output_length);
uint8_t* base64_decode(const char *data, size_t input_length, size_t *output_length);
