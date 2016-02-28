#include "parse.h"


implement_hashmap(string_hash_map, string_t, SuperFastHash, string_equal);
implement_hashmap(defines_hash_map, define_t, define_hash, define_equal);
implement_stack(parse_stack, parse_state_t);

u32 define_hash(define_t define) {
    return SuperFastHash(define.identifier);
}

b32 define_equal(define_t define1, define_t define2) {
    return string_equal(define1.identifier, define2.identifier);
}



void yyerror(const char *s) {
	// might as well halt now:
    printf("Error on line %i (%s): %s\n", yylineno, yytext, s);
    exit(-1);
}


// This function assumes that buffer is a string which one can access two characters after buffer.data + buffer.length
void push_parse_buffer(string_t buffer) {
    parse_stack_reserve(&parse_stack);
    parse_state_t* state = parse_stack_top(&parse_stack);
    state->buffer = buffer;
    state->overwritten_end_char0 = buffer.data[buffer.length + 0];
    state->overwritten_end_char1 = buffer.data[buffer.length + 1];
    buffer.data[buffer.length + 0] = '\0';
    buffer.data[buffer.length + 1] = '\0';
    
    state->buffer_state.yy_buf_size = buffer.length; 
    state->buffer_state.yy_buf_pos = state->buffer_state.yy_ch_buf = buffer.data;
    state->buffer_state.yy_is_our_buffer = 0;
    state->buffer_state.yy_input_file = NULL;
    state->buffer_state.yy_n_chars = buffer.length;
    state->buffer_state.yy_is_interactive = 0;
    state->buffer_state.yy_at_bol = 1;
    state->buffer_state.yy_fill_buffer = 0;
    state->buffer_state.yy_buffer_status = 0; /* = YY_BUFFER_NEW */

    yy_switch_to_buffer(&state->buffer_state);
    return;
}

b32 pop_parse_buffer() {
    parse_state_t* state = parse_stack_top(&parse_stack);
    state->buffer.data[state->buffer.length + 0] = state->overwritten_end_char0;
    state->buffer.data[state->buffer.length + 1] = state->overwritten_end_char1;
    parse_stack_pop(&parse_stack);
    if(parse_stack_is_empty(&parse_stack)) {
        return 1;
    } else {
        yy_switch_to_buffer(&(parse_stack_top(&parse_stack))->buffer_state);
        return 0;
    }
}
