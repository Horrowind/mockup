#include "wdc65816/registers.h"
#include "wdc65816/rom.h"
#include "base/base.h"


void nuts_global_init();
void nuts_global_deinit();

typedef struct {
    String name;
    Buffer buffer;
} NutsText;

typedef struct {
    String   name;
    int      line_number;
    int      line_pos;
    char*    line_start;
} NutsTextPos;

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
} NutsIdentifierType;

typedef enum {
    WIDTH_0, // Unused, but it is nice to have WIDTH_1 = 1, etc.
    WIDTH_1,
    WIDTH_2,
    WIDTH_3,
    WIDTH_4,
    WIDTH_UNSPECIFIED,
    WIDTH_MAX,
} NutsWidth;

typedef struct {
    String name;
    NutsIdentifierType type;
    NutsWidth width;
} NutsIdentifier;

typedef enum {
#define X(c) COMMAND_##c,
    COMMANDS_LIST
#undef X
    COMMAND_MAX
} NutsCommandType;


typedef enum {
#define X(c) OPCODE_##c,
    OPCODES_LIST
#undef X
    OPCODE_MAX
} NutsOpcodeType;

#define TOKEN_LIST                                                      \
    X(NULL) X(CMD_DELIM) X(ANON_LABEL) X(STRING) X(DEFINE)              \
    X(LSHIFT) X(RSHIFT) X(NUM) X(ARROW) X(EOF) X(IDENTIFIER)            \
    X(COMMENT) X(MAX) 
#define X(c) TOKEN_##c,
typedef enum {
    TOKEM_ASCII_RANGE_ = 127,
    TOKEN_LIST
} NutsTokenType;
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
} NutsErrorType;

typedef struct {
    char* begin;
    char* end;
    char* pos;
    NutsText  text;
} NutsByteStream;

#define BYTE_STREAM_EOF 0xFFFFFFFF
typedef u32 NutsRune;

typedef enum {
    VALUE_INT,
    VALUE_FLOAT,
    VALUE_STRING,
    VALUE_LABEL,
} NutsValueType;


typedef struct {
    NutsValueType type;
    union {
        u64    i;
        f64    f;
        String s;
        NutsIdentifier* l;
    };
} NutsValue;

define_intern_hashmap(NutsIdentifierMap, nuts_identifier_map, NutsIdentifier);

typedef struct {
    uint        type;
    String      string;
    union {
        String      name;
        NutsIdentifier* identifier;
        struct {
            u64 num;
            u8  width;
        };
    };
} NutsToken;


typedef struct {
    NutsText   text;
    NutsToken* data;
    uint   length;
} NutsTokenList;


void nuts_token_list_print(NutsTokenList token_list);

typedef enum {
    ERROR_LEVEL_OK,
    ERROR_LEVEL_NOTE,
    ERROR_LEVEL_WARNING,
    ERROR_LEVEL_ERROR,
} NutsErrorLevel;

typedef struct {
    NutsErrorType type;
    NutsTextPos   text_pos;
    union {
        struct { uint base; } not_a_n_ary_digit;
        struct {
            NutsTokenType expected_type;
            NutsTokenType actual_type;
        } expected_token;
    };
} NutsError;

typedef struct {
    Arena  arena;
    NutsError* errors;
    int   length;
} NutsErrorList;

typedef enum {
    RESULT_OK,
    RESULT_ERROR,
    RESULT_NEED_TOKEN_STREAM,
    RESULT_NEED_FILE,
    RESULT_NOT_AN_EXPR,
    RESULT_UNDEFINED_LABEL,
} NutsResult;

void nuts_error_list_init(NutsErrorList* error_list, Arena arena);
void nuts_error_add(NutsErrorList* error_list, NutsError error);

void nuts_describe_error(NutsError error);

NutsResult nuts_lex(NutsText text, NutsTokenList* list, Arena* arena, NutsErrorList* error_list,
                    NutsIdentifierMap* identifier_map);
    
typedef struct {
    NutsText   text;
    NutsToken* begin;
    NutsToken* end;
    NutsToken* pos;
} NutsTokenStream;

define_stack(NutsTokenStreamStack, nuts_token_stream_stack, NutsTokenStream);

typedef struct {
    String        name;
    NutsTokenList token_list;
} NutsDefine;

define_hashmap(NutsDefineMap, nuts_define_map, NutsDefine);

typedef enum {
    EXPR_NULL,
    /* Binary operands */
    EXPR_MUL,
    EXPR_DIV,
    EXPR_ADD,
    EXPR_SUB,
    EXPR_LSH,
    EXPR_RSH,
    EXPR_BAN,
    EXPR_BOR,
    EXPR_DOT,
    /* Unary operands */
    EXPR_NEG,
    /* Nullary operands */
    EXPR_LABEL,
    EXPR_VALUE,
} NutsExprType;

typedef struct Expr_ {
    NutsExprType  type;
    NutsValueType value_type;
    String    string;
    NutsWidth     width;
    union {
        /* Binary operands */
        struct {
            struct Expr_* op1;
            struct Expr_* op2;
        };
        /* Unary operands */
        struct Expr_* op;
        /* Number */
        NutsValue value;
    };
} NutsExpr;

typedef struct ExprList_{
    NutsExpr* expr;
    struct ExprList_* next;
} NutsExprList;

typedef enum {
    PARSE_MODE_DIRECT,
    PARSE_MODE_DIRECT_X,
    PARSE_MODE_DIRECT_Y,
    PARSE_MODE_DIRECT_S,
    PARSE_MODE_IMMEDIATE,
    PARSE_MODE_INDIRECT,
    PARSE_MODE_X_INDIRECT,
    PARSE_MODE_INDIRECT_Y,
    PARSE_MODE_S_INDIRECT_Y,
    PARSE_MODE_LONG_INDIRECT,
    PARSE_MODE_LONG_INDIRECT_Y,
    PARSE_MODE_IMPLIED,
    PARSE_MODE_ACCUMULATOR,
    PARSE_MODE_SRC_DEST,
    PARSE_MODE_MAX,
} NutsOpcodeParseMode;

typedef struct {
    u8 byte;
    u8 mode;
} NutsOpcodeAssembleData;

typedef enum {
    ASSEMBLE_MODE_ERROR,
    ASSEMBLE_MODE_0,
    ASSEMBLE_MODE_1,
    ASSEMBLE_MODE_2,
    ASSEMBLE_MODE_3,
    ASSEMBLE_MODE_R, // Repeat
    ASSEMBLE_MODE_J, // Long jump
    ASSEMBLE_MODE_K, // Short jump
    ASSEMBLE_MODE_S,
    ASSEMBLE_MODE_X,
} NutsAssembleMode;


typedef enum {
    STATEMENT_TYPE_NULL,
    STATEMENT_TYPE_EMPTY,
    STATEMENT_TYPE_INCSRC,
    STATEMENT_TYPE_INCBIN,
    STATEMENT_TYPE_DB,
    STATEMENT_TYPE_ORG,
    STATEMENT_TYPE_LABEL_DEF,
    STATEMENT_TYPE_WDC65816,
#if 0
    STATEMENT_TYPE_TABLE,
    STATEMENT_TYPE_CLEARTABLE,
    STATEMENT_TYPE_FILLBYTE,
    STATEMENT_TYPE_WARNPC,
    STATEMENT_TYPE_FILL,
    STATEMENT_TYPE_BASE,
#endif
} NutsStatementType;

typedef struct {
    u8    op_type;
    NutsOpcodeParseMode parse_mode;
    NutsExpr  expr1;
    NutsExpr  expr2;
    NutsWidth width;
} NutsOpEncoding;


typedef struct {
    NutsStatementType type;
    NutsTokenList     tokens;
    String            comment;
    union {
        struct {
            String file_name;
        } incsrc;
        struct {
            String file_name;
            NutsExpr location;
            u8 include_somewhere_else : 1;
        } incbin;
        struct {
            String file_name;
        } table;
        struct { } cleartable;
        struct {
            NutsExpr length;
        } fill;
        struct {
            NutsExpr expr;
        } fillbyte;
        struct {
            NutsExprList list_sentinel;
            NutsWidth width;
        } db;
        struct {
            NutsExpr addr;
        } org;
        struct {
            NutsExpr addr;
            u8  off : 1;
        } base;
        struct {
            NutsExpr expr;
        } warnpc;
        struct {
            String name;
        } label_def;
        NutsOpEncoding wdc65816;
    };
} NutsStatement;

typedef struct {
    NutsStatement* data;
    uint length;
} NutsStatementList;

typedef struct {
    NutsIdentifierMap identifier_map;
    NutsStatementList statement_list;
    FreeList*         free_list;
} NutsAST;

void nuts_ast_init(NutsAST* ast, FreeList* free_list);

typedef struct {
    NutsTokenStreamStack stream_stack;
    // Short-hand for token_stream_stack_top(.token_stream_stack)
    NutsTokenStream*     stream_stack_top;
    // Short-hand for .token_stream_stack->pos[0]
    NutsToken            token;
    uint                 token_count;
    Arena*               arena;
    Arena                statement_arena;
    FreeList*            free_list;
    NutsDefineMap        define_map;
    NutsAST*             ast;
    NutsErrorList*       error_list;
    
    String               needed_token_stream_file_name;
    NutsStatement*       current_stmt;

    String               last_label_name;
} NutsParser;

void nuts_parser_init(NutsParser* parser, Arena* arena, FreeList* free_list,
                      NutsErrorList* error_list, NutsAST* ast);
void nuts_parser_deinit(NutsParser* parser);
NutsResult nuts_parse(NutsParser* parser, NutsTokenList list);

typedef struct NutsExpr_ NutsNutsExpr;

typedef struct {
    NutsExpr* expr;
    uint  offset;
    uint  width;
} NutsRelocation;

typedef struct {
    u32         base_addr;
    Buffer      buffer; //64k buffer?
    u8*         current;
    NutsRelocation* relocations;
} NutsBlock;


//TODO: Cleanup
void wdc65816_rom_write_bytes(Wdc65816MapperBuilder* rom, u32 addr, u32 bytes, uint width);
void wdc65816_rom_write_expr(Wdc65816MapperBuilder* rom, u32 addr, NutsExpr* expr, uint width);
void wdc65816_rom_write_buffer(Wdc65816MapperBuilder* rom, u32 addr, Buffer buffer);

typedef struct {
    String name;
    u32    addr;
    u32    pass;
} NutsLabel;

define_hashmap(NutsLabelMap, nuts_label_map, NutsLabel);

typedef struct {
    NutsErrorList* error_list;
    NutsAST*       ast;
    NutsStatement* current_statement;

    Wdc65816Mapper* rom;
    
    NutsLabelMap label_map;
    NutsExpr*  fill_byte;
    String file_name;
    Buffer buffer;

    u32 addr;

    u32 pass;
    u32 last_pass;
    u32 max_num_passes;
    b32 rerun;
} NutsAssembler;

void nuts_assembler_init(NutsAssembler* assembler,
                         NutsErrorList* error_list,
                         NutsAST* ast,
                         Wdc65816Mapper* rom,
                         FreeList* free_list);

NutsResult nuts_assemble(NutsAssembler* assembler);
String nuts_assembler_get_file_name(NutsAssembler* assembler);
void   nuts_assembler_give_buffer(NutsAssembler* assembler, Buffer buffer);
