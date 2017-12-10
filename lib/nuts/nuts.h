#include "wdc65816/rom.h"
#include "base/base.h"

#define TEXT_POS

typedef int TokenType;
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
    TokenType type;
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
} Lexer;

define_stack(AddrStack, addr_stack, u32);

typedef struct {
    AddrStack    pc_stack;
    Lexer        lexer;
    Token        token;
    LabelMap     label_map;
    WDC65816Rom* rom;
    Arena*       arena;
    Path*        working_directory;
} ParserState;

void parser_global_init();
void parser_global_deinit();
void parser_init(ParserState* parser, WDC65816Rom* rom, Arena* arena,
                 Path* file, Path* working_directory);
void parser_deinit(ParserState* parser);
void parse_asm(ParserState* parser);
