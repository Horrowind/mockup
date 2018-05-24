#include "wdc65816/registers.h"
#include "wdc65816/rom.h"
#include "base/base.h"

#define TEXT_POS

typedef int IdentType;

typedef struct {
    String  name;
    Buffer  buffer;
#ifdef TEXT_POS
    TextPos text_pos;
#endif
} Define;

u32 define_equal(Define e1, Define e2);
u32 define_hash(Define e);

typedef struct {
    String  name;
    u32     address;
#ifdef TEXT_POS
    TextPos definition_pos;
#endif
} Label;

u32 label_equal(Label e1, Label e2);
u32 label_hash(Label e);

typedef struct {
    String    name;
    IdentType type;
} Identifier;

u32 identifier_hash(Identifier e);
u32 identifier_equal(Identifier e1, Identifier e2);

define_hashmap(DefineMap, define_map, Define, define_hash, define_equal);
define_hashmap(LabelMap, label_map, Label, label_hash, label_equal);
define_hashmap(IdentifierMap, identifier_map, Identifier, identifier_hash, identifier_equal);

typedef struct {
    int       type;
#ifdef TEXT_POS
    TextPos   text_pos;
#endif
    String    text;
    u64       num;
    IdentType ident_type;
    int       bytes;
} Token;

typedef struct {
    Buffer   buffer;
    char*    pos;
    int      allocated_buffer : 1;
#ifdef TEXT_POS
    TextPos  text_pos;
#endif
} LexerState; 

define_stack(LexerStack, lexer_stack, LexerState);

typedef struct {
    char*       pos;
    char*       end;
    LexerState* top;
#ifdef TEXT_POS
    TextPos     text_pos;
#endif
    DefineMap   define_map;
    LexerStack  lexer_stack;
    FreeList*   free_list;
} Lexer;

typedef struct {
    Wdc65816Reg24 min;
    Wdc65816Reg24 max;
} Interval;

define_stack(AddrStack, addr_stack, Interval);

typedef struct Statement Statement;
typedef struct {
    Statement* stmts;
    uint length;
} StatementBlock;

typedef struct {
    AddrStack      pc_stack;
    Interval*      pc;                     //Short-hand for addr_stack_top(pc_stack)
    Lexer          lexer;
    Token          token;
    LabelMap       label_map;
    Wdc65816Rom*   rom;
    Arena*         arena;
    Arena          statement_arena;
    Path*          working_directory;

    u8             fill_byte;
    uint           bank_crossed : 1;
} ParserState;

void parser_global_init();
void parser_global_deinit();
void parser_init(ParserState* parser, Wdc65816Rom* rom, Arena* arena,
                 FreeList* free_list, Path* file, Path* working_directory);
void parser_deinit(ParserState* parser);
StatementBlock parse_asm(ParserState* parser);
