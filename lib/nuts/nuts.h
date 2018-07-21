#include "wdc65816/registers.h"
#include "wdc65816/rom.h"
#include "base/base.h"


void nuts_global_init();
void nuts_global_deinit();

typedef struct {
    String name;
    Buffer buffer;
} Text;

typedef struct {
    String   name;
    int      line_number;
    int      line_pos;
    char*    line_start;
} TextPos;

void error_at_pos(TextPos text_pos);
void warn_at_pos(TextPos text_pos);

#define COMMANDS_LIST                                                   \
    X(INCSRC) X(FILLBYTE) X(INCBIN) X(FREEDATA) X(DB) X(DW) X(DL) X(DD) \
    X(ORG) X(A) X(X) X(Y) X(S) X(WARNPC) X(CLEARTABLE) X(PRINT) X(IF)   \
    X(ELSE) X(ENDIF) X(FILL) X(BASE) X(TABLE) X(OFF)

#define OPCODES_LIST                                                    \
    X(ADC) X(AND) X(ASL) X(BBR) X(BBS) X(BCC) X(BCS) X(BEQ) X(BIT)      \
    X(BMI) X(BNE) X(BPL) X(BRA) X(BRK) X(BVC) X(BVS) X(BRL) X(CLC)      \
    X(CLD) X(CLI) X(CLV) X(CMP) X(CPX) X(CPY) X(COP) X(DEC) X(DEX)      \
    X(DEY) X(EOR) X(INC) X(INX) X(INY) X(JMP) X(JML) X(JSR) X(JSL)      \
    X(LDA) X(LDX) X(LDY) X(LSR) X(MVP) X(MVN) X(NOP) X(ORA) X(PHA)      \
    X(PHP) X(PHX) X(PHY) X(PLA) X(PLP) X(PLX) X(PLY) X(PHB) X(PHD)      \
    X(PHK) X(PLB) X(PLD) X(PEA) X(PEI) X(PER) X(RMB) X(ROL) X(ROR)      \
    X(RTI) X(RTS) X(REP) X(RTL) X(SBC) X(SEC) X(SED) X(SEI) X(SMB)      \
    X(STA) X(STX) X(STY) X(STZ) X(SEP) X(STP) X(TAX) X(TAY) X(TRB)      \
    X(TSB) X(TSA) X(TAS) X(TDA) X(TAD) X(TSX) X(TXA) X(TXS) X(TYA)      \
    X(TXY) X(TYX) X(TCD) X(TDC) X(TCS) X(TSC) X(WAI) X(WDM) X(XBA) X(XCE)

typedef enum {
    IDENTIFIER  = 0x100,
    COMMAND     = 0x200, 
    OPCODE      = 0x400
} IdentifierType;

typedef enum {
    WIDTH_0, // Unused, but it is nice to have WIDTH_1 = 1, etc.
    WIDTH_1,
    WIDTH_2,
    WIDTH_3,
    WIDTH_4,
    WIDTH_UNSPECIFIED,
    WIDTH_MAX,
} Width;

typedef struct {
    String name;
    IdentifierType type;
    Width width;
} Identifier;

typedef enum {
#define X(c) COMMAND_##c,
    COMMANDS_LIST
#undef X
    COMMAND_MAX
} CommandType;


typedef enum {
#define X(c) OPCODE_##c,
    OPCODES_LIST
#undef X
    OPCODE_MAX
} OpcodeType;

#define TOKEN_LIST                                                      \
    X(NULL) X(CMD_DELIM) X(ANON_LABEL) X(STRING) X(DEFINE)              \
    X(LSHIFT) X(RSHIFT) X(NUM) X(ARROW) X(EOF) X(IDENTIFIER)            \
    X(COMMENT) X(MAX) 
#define X(c) TOKEN_##c,
typedef enum {
    TOKEM_ASCII_RANGE_ = 127,
    TOKEN_LIST
} TokenType;
#undef X

#define ERROR_LIST                                                      \
    X(OK) X(NEED_TOKEN_STREAM) X(NOT_A_N_ARY_DIGIT) X(EXPECTED_TOKEN)   \
    X(NOT_AN_EXPR) X(EXPR_NOT_STATIC) X(INCOMPATIBLE_OPERANDS)          \
    X(EXPR_NOT_INT) X(UNTERMINATED_STATEMENT) X(UNEXPECTED_TOKEN)       \
    X(UNEXPECTED_CHARACTER) X(UNDEFINED_DEFINE) X(NOT_A_VALID_COMMAND)  \
    X(UNTERMINATED_STRING) X(UNTERMINATED_IF) X(LABEL_ALREADY_DEFINED)  \
    X(TO_MANY_TOKENS) X(ELSE_WITHOUT_IF) X(ENDIF_WITHOUT_IF)            \
    X(TO_MANY_PASSES) X(FLOATING_LABEL) X(UNDEFINED_LABEL)              \
    X(JUMP_TO_LONG) X(WRONG_BANK) X(WRONG_OPCODE_USAGE)                 \
    X(VALUE_NEGATIVE)

#define WARNING_LIST                                                    \
    X(WIDTH_SPECIFIER_MEANINGLESS)

typedef enum {
#define X(c) ERROR_##c,
    ERROR_LIST
#undef X
#define X(c) WARNING_##c,
    WARNING_LIST
#undef X
} ErrorType;

typedef struct {
    char* begin;
    char* end;
    char* pos;
    Text  text;
} ByteStream;

#define BYTE_STREAM_EOF 0xFFFFFFFF
typedef u32 Rune;

typedef enum {
    VALUE_INT,
    VALUE_FLOAT,
    VALUE_STRING,
    VALUE_LABEL,
} ValueType;


typedef struct {
    ValueType type;
    union {
        u64    i;
        f64    f;
        String s;
        Identifier* l;
    };
} Value;


u32 identifier_equal(Identifier e1, Identifier e2);
u32 identifier_hash(Identifier e);
define_intern_hashmap(IdentifierMap, identifier_map, Identifier);

typedef struct {
    int         type;
    String      string;
    union {
        String      name;
        Identifier* identifier;
        struct {
            u64 num;
            u8  width;
        };
    };
} Token;


typedef struct {
    Text   text;
    Token* data;
    uint   length;
} TokenList;


void token_list_print(TokenList token_list);

typedef enum {
    ERROR_LEVEL_OK,
    ERROR_LEVEL_NOTE,
    ERROR_LEVEL_WARNING,
    ERROR_LEVEL_ERROR,
} ErrorLevel;

typedef struct {
    ErrorType type;
    TextPos   text_pos;
    union {
        struct { uint base; } not_a_n_ary_digit;
        struct {
            TokenType expected_type;
            TokenType actual_type;
        } expected_token;
    };
} Error;

typedef struct {
    Arena  arena;
    Error* errors;
    int   length;
} ErrorList;

typedef enum {
    RESULT_OK,
    RESULT_ERROR,
    RESULT_NEED_TOKEN_STREAM,
    RESULT_NEED_FILE,
    RESULT_NOT_AN_EXPR,
    RESULT_UNDEFINED_LABEL,
} Result;

void error_list_init(ErrorList* error_list, Arena arena);
void error_add(ErrorList* error_list, Error error);

void describe_error(Error error);

Result lex(Text text, TokenList* list, Arena* arena, ErrorList* error_list,
           IdentifierMap* identifier_map);
    
typedef struct {
    Text   text;
    Token* begin;
    Token* end;
    Token* pos;
} TokenStream;

define_stack(TokenStreamStack, token_stream_stack, TokenStream);

typedef struct {
    String    name;
    TokenList token_list;
} Define;

u32 define_equal(Define e1, Define e2);
u32 define_hash(Define e);
define_hashmap(DefineMap, define_map, Define, define_hash, define_equal);

typedef struct Statement_ Statement;

typedef struct {
    Statement* data;
    uint length;
} StatementList;

typedef struct {
    IdentifierMap identifier_map;
    StatementList statement_list;
} AST;

void ast_init(AST* ast, Arena* arena);

typedef struct {
    // Lexing statements
    u32 insert_incsrc     : 1;
    u32 insert_label_def  : 1;
    // Parsing statements
    u32 insert_table      : 1;
    u32 insert_cleartable : 1;
} ParserOptions;

typedef struct {
    TokenStreamStack stream_stack;
    // Short-hand for token_stream_stack_top(.token_stream_stack)
    TokenStream*     stream_stack_top;
    // Short-hand for .token_stream_stack->pos[0]
    Token            token;
    Arena*           arena;
    Arena            statement_arena;
    FreeList*        free_list;
    DefineMap        define_map;
    AST*             ast;
    ErrorList*       error_list;
    
    String           needed_token_stream_file_name;
    ParserOptions    options;
    Statement*       current_stmt;

    String           last_label_name;
} Parser;

void parser_init(Parser* parser, Arena* arena, FreeList* free_list,
                 ErrorList* error_list, AST* ast);
void parser_deinit(Parser* parser);
Result parse(Parser* parser, TokenList list);

typedef struct Expr_ Expr;

typedef struct {
    Expr* expr;
    uint  offset;
    uint  width;
} Relocation;

typedef struct {
    u32         base_addr;
    Buffer      buffer; //64k buffer?
    u8*         current;
    Relocation* relocations;
} Block;

void wdc65816_rom_write_bytes(Wdc65816MapperBuilder* rom, u32 addr, u32 bytes, uint width);
void wdc65816_rom_write_expr(Wdc65816MapperBuilder* rom, u32 addr, Expr* expr, uint width);
void wdc65816_rom_write_buffer(Wdc65816MapperBuilder* rom, u32 addr, Buffer buffer);

typedef struct {
    String name;
    u32    addr;
    u32    pass;
} Label;

u32 label_hash(Label e);
u32 label_equal(Label e1, Label e2);
define_hashmap(LabelMap, label_map, Label, label_hash, label_equal);

typedef struct {
    ErrorList* error_list;
    AST*       ast;
    Statement* current_statement;

    Wdc65816Mapper* rom;
    
    LabelMap label_map;
    Expr*  fill_byte;
    String file_name;
    Buffer buffer;

    u32 addr;

    int pass;
    int last_pass;
    int max_num_passes;
    int rerun;
} Assembler;

void assembler_init(Assembler* assembler,
                    ErrorList* error_list,
                    AST* ast,
                    Wdc65816Mapper* rom);

Result assemble(Assembler* assembler);
String assembler_get_file_name(Assembler* assembler);
void   assembler_give_buffer(Assembler* assembler, Buffer buffer);
