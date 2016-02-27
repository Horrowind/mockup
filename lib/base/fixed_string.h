#ifndef BASE_FIXED_STRING
#define BASE_FIXED_STRING

#include "datasizes.h"
#include "variable_string.h"

#define fixed_string_t(size)                                            \
    struct {                                                            \
        char data[size];                                                \
        u16 length;                                                     \
    } 

#define define_fixed_string(name, size)                                 \
    typedef fixed_string_t(size) name##_t;                              \
    void name##_append(name##_t s1, name##_t s2);                       \
    name##_t name##_cat(name##_t s1, name##_t s2);                      \
    string_t name##_to_string(name##_t* s);                             \
    void name##_from_c_string(name##_t* s, char* cstring);              \
    char* name##_find_char(name##_t* s, char c);

#define implement_fixed_string(name, size)                              \
    inline void name##_append(name##_t s1, name##_t s2) {               \
        u16 size_to_copy;                                               \
        if(s1.length + s2.length > size) {                              \
            size_to_copy = size - s1.length;                            \
        } else {                                                        \
            size_to_copy = s2.length;                                   \
        }                                                               \
        for(int i = 0; i < size_to_copy; i++) {                         \
            s1.data[s1.length + i] = s2.data[i];                        \
        }                                                               \
    }                                                                   \
                                                                        \
    inline name##_t name##_cat(name##_t s1, name##_t s2) {              \
        name##_t result = s1;                                           \
        u16 size_to_copy;                                               \
        if(s1.length + s2.length > size) {                              \
            size_to_copy = size - s1.length;                            \
        } else {                                                        \
            size_to_copy = s2.length;                                   \
        }                                                               \
        for(int i = 0; i < size_to_copy; i++) {                         \
            result.data[s1.length + i] = s2.data[i];                    \
        }                                                               \
        return result;                                                  \
    }                                                                   \
                                                                        \
    inline char* name##_find_char(name##_t* s, char c) {                \
        char* iter = s->data;                                           \
        for(int i = 0; i < s->length; i++) {                            \
            if(*iter == c) return iter;                                 \
            iter++;                                                     \
        }                                                               \
    }                                                                   \
                                                                        \
    inline void name##_from_c_string(name##_t* s, char* cstring) {      \
        u16 length = 0;                                                 \
        while(*cstring != '\0' || length == size) {                     \
            s->data[length] = *cstring;                                 \
            cstring++;                                                  \
            length++;                                                   \
        }                                                               \
        s->length = length;                                             \
    }                                                                   \
                                                                        \
    inline string_t name##_to_string(name##_t* s) {                     \
        string_t result = {                                             \
            .data = (char*)&s->data,                                    \
            .length = s->length                                         \
        };                                                              \
        return result;                                                  \
    }

#define generate_fixed_string(name, size)                               \
    define_fixed_string(name, size);                                    \
    implement_fixed_string(name, size);                                 \
    

#endif //BASE_FIXED_STRING
