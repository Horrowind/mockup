#ifndef NUTS_PARSE_H
#define NUTS_PARSE_H

#include "base/variable_string.h"
#include "base/platform.h"
#include "base/hash.h"
#include "base/stack.h"

#ifndef IN_FLEX
#include "flex.h"
#endif //IN_FLEX

typedef struct {
    string_t identifier;
    string_t replacement;
} define_t;

u32 define_hash(define_t);
b32 define_equal(define_t, define_t);

define_hashmap(string_hash_map, string_t, SuperFastHash, string_equal);
define_hashmap(defines_hash_map, define_t, define_hash, define_equal);

string_hash_map_t identifier_map;
defines_hash_map_t defines_map;
void yyerror(const char *s);

//#define PRINT_TOKENS
#ifdef PRINT_TOKENS
#define return_token(token)                                 \
    printf("Found: " #token "\n");                          \
    return token;
#else
#define return_token(token)                                 \
    return token;
#endif
extern int yyparse();
void push_parse_buffer(string_t);
b32 pop_parse_buffer();

typedef struct {
    string_t buffer;
    struct yy_buffer_state buffer_state;
    char overwritten_end_char0;
    char overwritten_end_char1;
} parse_state_t;
define_stack(parse_stack, parse_state_t);
parse_stack_t parse_stack;


// Converts an ASCII character (0-9, a-f, A-F) into the respective (hexadecimal) value.
// Assumes that the character is in the above mentioned range.
#define char2hex(c) ((c) > 64 ? ((c) & 0xF) + 9 : (c) - '0')

/* void read_input(char* buf, unsigned long* result, int max_size) { */
/*     parse_state_t* state = parse_stack_top(&parse_stack); */
/*     state->last_string = state->string; */
/*     *result = min(max_size, state->string.length); */
/*     if(*result) memcpy(buf, state->string.data, *result); */
/*     state->string.data += *result; */
/*     state->string.length -= *result; */
/* } */





#endif //NUTS_PARSE_H
