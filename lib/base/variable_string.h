#ifndef BASE_VARIABLE_STRING_H
#define BASE_VARIABLE_STRING_H

#include "misc.h"
#include "datasizes.h"

typedef struct {
    char* data;
    uint length;
} string_t;

#define L(cstr) (string_t){.data = cstr, .length = array_length(cstr) - 1 }

b32 string_equal(string_t s1, string_t s2);
string_t string_from_c_string(char* buf);


#ifdef BASE_VARIABLE_STRING_IMPLEMENTATION

b32 string_equal(string_t s1, string_t s2) {
    b32 result = 0;
    if(s1.length != s2.length) return 0;
    for(int i = 0; i < s1.length; i++) {
        if(s1.data[i] != s2.data[i]) return 0;
    }
    return 1;
}

string_t string_from_c_string(char* buf) {
    int length = 0;
    string_t s = {
        .data = buf
    };
    while(*buf != '\0') {
        buf++;
        length++;
    }
    
    s.length = length;
    return s;
}


#endif //BASE_VARIABLE_STRING_IMPLEMENTATION

#endif //BASE_VARIABLE_STRING_H


