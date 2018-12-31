#define FixedString(size)                                               \
    struct {                                                            \
        char data[size];                                                \
        u16 length;                                                     \
    } 

#define define_fixed_string(name, prefix, size)                         \
    typedef FixedString(size) name;                                     \
    void prefix##_append(name s1, name s2);                             \
    name prefix##_cat(name s1, name s2);                                \
    String prefix##_to_string(name* s);                                 \
    void prefix##_from_c_string(name* s, char* cstring);                \
    char* prefix##_find_char(name* s, char c);

#define implement_fixed_string(name, prefix, size)                      \
    inline void prefix##_append(name s1, name s2) {                     \
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
    inline name prefix##_cat(name s1, name s2) {                        \
        name result = s1;                                               \
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
    inline char* prefix##_find_char(name* s, char c) {                  \
        char* iter = s->data;                                           \
        for(int i = 0; i < s->length; i++) {                            \
            if(*iter == c) return iter;                                 \
            iter++;                                                     \
        }                                                               \
    }                                                                   \
                                                                        \
    inline void prefix##_from_c_string(name* s, char* cstring) {        \
        u16 length = 0;                                                 \
        while(*cstring != '\0' || length == size) {                     \
            s->data[length] = *cstring;                                 \
            cstring++;                                                  \
            length++;                                                   \
        }                                                               \
        s->length = length;                                             \
    }                                                                   \
                                                                        \
    inline String prefix##_to_string(name* s) {                         \
        String result = {                                               \
            .data = (char*)&s->data,                                    \
            .length = s->length                                         \
        };                                                              \
        return result;                                                  \
    }

#define generate_fixed_string(name, prefix, size)                       \
    define_fixed_string(name, prefix, size);                            \
    implement_fixed_string(name, prefix, size);
