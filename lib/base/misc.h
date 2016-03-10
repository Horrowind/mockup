#ifndef BASE_MISC_H
#define BASE_MISC_H

#include "datasizes.h"

#define array_length(array) (sizeof(array)/sizeof(array[0]))
#define max(a, b) (a > b ? a : b)
#define min(a, b) (a < b ? a : b)



typedef struct {
    u8* data;
    int length;
} buffer_t;


#endif //BASE_MISC_H
