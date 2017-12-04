#include "wdc65816/rom.h"
#include "base/base.h"

#define TEXT_POS

typedef int token_type_t;
typedef int ident_type_t;

typedef struct {
    string_t   name;
    buffer_t   buffer;
#ifdef TEXT_POS
    text_pos_t text_pos;
#endif
} define_t;

u32 define_equal(define_t e1, define_t e2);
u32 define_hash(define_t e);

typedef struct {
    string_t   name;
    u32        address;
#ifdef TEXT_POS
    text_pos_t definition_pos;
#endif
} label_t;

u32 label_equal(label_t e1, label_t e2);
u32 label_hash(label_t e);

typedef struct {
    string_t name;
    ident_type_t type;
} identifier_t;

u32 identifier_hash(identifier_t e);
u32 identifier_equal(identifier_t e1, identifier_t e2);

define_hashmap(define_map, define_t, define_hash, define_equal);
define_hashmap(label_map, label_t, label_hash, label_equal);
define_hashmap(identifier_map, identifier_t, identifier_hash, identifier_equal);

typedef struct {
    token_type_t type;
#ifdef TEXT_POS
    text_pos_t   text_pos;
#endif
    string_t     text;
    u64 num;
    ident_type_t ident_type;
    int bytes;
} token_t;

typedef struct {
    buffer_t   buffer;
    char*      pos;
    int        allocated_buffer : 1;
#ifdef TEXT_POS
    text_pos_t text_pos;
#endif
} lexer_state_t; 

define_stack(lexer_stack, lexer_state_t);

typedef struct {
    char* pos;
    char* end;
    lexer_state_t* top;
#ifdef TEXT_POS
    text_pos_t text_pos;
#endif
    define_map_t define_map;
    lexer_stack_t lexer_stack;
} lexer_t;

define_stack(addr_stack, u32);

typedef struct {
    addr_stack_t    pc_stack;
    lexer_t         lexer;
    token_t         token;
    label_map_t     label_map;
    pool_t          expr_pool;
    wdc65816_rom_t* rom;
    arena_t*        arena;
    path_t*         working_directory;
} parser_state_t;

void parser_global_init();
void parser_global_deinit();
void parser_init(parser_state_t* parser, wdc65816_rom_t* rom, arena_t* arena,
                 path_t* file, path_t* working_directory);
void parser_deinit(parser_state_t* parser);
void parse_asm(parser_state_t* parser);
