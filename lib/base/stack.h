#ifndef BASE_STACK_H
#define BASE_STACK_H


#define define_stack(name, type)                                        \
    typedef struct {                                                    \
        type* data;                                                     \
        u16 fill;                                                       \
        u16 capacity;                                                   \
    } name##_t;                                                         \
    extern void name##_init(name##_t* s, int size);                     \
    extern void name##_deinit(name##_t* s);                             \
    extern void name##_push(name##_t* s, type data);                    \
    extern void name##_reserve(name##_t* s);                            \
    extern void name##_pop(name##_t* s);                                \
    extern type* name##_top(name##_t* s);                               \
    extern b32 name##_is_empty(name##_t* s);

#define implement_stack(name, type)                                     \
    inline void name##_init(name##_t* s, int size) {                    \
        s->data = malloc(size * sizeof(type));                          \
        s->fill = 0;                                                    \
        s->capacity = size;                                             \
    }                                                                   \
                                                                        \
    inline void name##_deinit(name##_t* s) {                            \
        free(s->data);                                                  \
    }                                                                   \
                                                                        \
    inline void name##_push(name##_t* s, type data) {                   \
        if(s->fill == s->capacity) {                                    \
            s->capacity *= 2;                                           \
            /*TODO: One should check for null here...        */         \
            s->data = realloc(s->data, s->capacity * sizeof(type));     \
        }                                                               \
        s->data[s->fill] = data;                                        \
        s->fill++;                                                      \
    }                                                                   \
                                                                        \
    inline void name##_reserve(name##_t* s) {                           \
        if(s->fill == s->capacity) {                                    \
            s->capacity *= 2;                                           \
            /*TODO: One should check for null here...        */         \
            s->data = realloc(s->data, s->capacity * sizeof(type));     \
        }                                                               \
        s->fill++;                                                      \
    }                                                                   \
                                                                        \
    inline type* name##_top(name##_t* s) {                              \
        return s->data + s->fill - 1;                                   \
    }                                                                   \
                                                                        \
    inline void name##_pop(name##_t* s) {                               \
        s->fill--;                                                      \
        return;                                                         \
    }                                                                   \
                                                                        \
    inline void name##_do_empty(name##_t* s) {                          \
        s->fill = 0;                                                    \
        return;                                                         \
    }                                                                   \
                                                                        \
    inline b32 name##_is_empty(name##_t* s) {                           \
        return s->fill == 0;                                            \
    }


#define generate_stack(name, type)                                      \
    define_stack(name, type);                                           \
    implement_stack(name, type);


#endif //BASE_STACK_H
