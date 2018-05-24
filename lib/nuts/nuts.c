#include "base/base.h"
#include "nuts/nuts.h"

#define OPCODES_LIST                                                    \
    X(ADC) X(AND) X(ASL) X(BBR) X(BBS) X(BCC) X(BCS) X(BEQ) X(BIT) X(BMI) \
    X(BNE) X(BPL) X(BRA) X(BRK) X(BVC) X(BVS) X(BRL) X(CLC) X(CLD) X(CLI) \
    X(CLV) X(CMP) X(CPX) X(CPY) X(COP) X(DEC) X(DEX) X(DEY) X(EOR) X(INC) \
    X(INX) X(INY) X(JMP) X(JML) X(JSR) X(JSL) X(LDA) X(LDX) X(LDY) X(LSR) \
    X(MVP) X(MVN) X(NOP) X(ORA) X(PHA) X(PHP) X(PHX) X(PHY) X(PLA) X(PLP) \
    X(PLX) X(PLY) X(PHB) X(PHD) X(PHK) X(PLB) X(PLD) X(PEA) X(PEI) X(PER) \
    X(RMB) X(ROL) X(ROR) X(RTI) X(RTS) X(REP) X(RTL) X(SBC) X(SEC) X(SED) \
    X(SEI) X(SMB) X(STA) X(STX) X(STY) X(STZ) X(SEP) X(STP) X(TAX) X(TAY) \
    X(TRB) X(TSB) X(TSA) X(TAS) X(TDA) X(TAD) X(TSX) X(TXA) X(TXS) X(TYA) \
    X(TXY) X(TYX) X(TCD) X(TDC) X(TCS) X(TSC) X(WAI) X(WDM) X(XBA) X(XCE)

#define COMMANDS_LIST                                                   \
    X(INCSRC) X(FILLBYTE) X(INCBIN) X(FREEDATA) X(DB) X(DW) X(DL) X(DD) \
    X(ORG) X(A) X(X) X(Y) X(S) X(B) X(W) X(L) X(WARNPC) X(CLEARTABLE)   \
    X(PRINT) X(IF) X(ELSE) X(ENDIF) X(FILL) X(BASE) X(TABLE) X(OFF)

#define X(c) IDENT_##c,
enum {
    IDENT_OPCODE  = 0x0000,
    OPCODES_LIST
    IDENT_OPCODE_MAX,
    
    IDENT_COMMAND = 0x1000,
    COMMANDS_LIST
    IDENT_COMMAND_MAX,
};
#undef X

#define TOKEN_LIST                                                  \
    X(NULL) X(CMD_DELIM) X(COLON) X(LBRACE)  X(RBRACE)              \
    X(RBRACKET) X(LBRACKET) X(LPAREN)  X(RPAREN) X(HASH) X(EQUAL)   \
    X(COMMA) X(PLUS) X(MINUS) X(TIMES) X(SLASH) X(ANON_LABEL)       \
    X(LABEL_DEF) X(STRING) X(DEFINE) X(LESS) X(GREATER)             \
    X(LSHIFT) X(RSHIFT) X(DOT) X(NUM)                               \
    X(IDENTIFIER) X(PIPE) X(AMPERSAND) X(ARROW) X(EOF)              \
    X(LABEL) X(MAX) 
#define X(c) TOKEN_##c,
    enum { TOKEN_LIST };
#undef X

#define X(c) #c,
static char* commands[] = { COMMANDS_LIST };
static char* opcodes[] = { OPCODES_LIST };
static char* token_names[] = { TOKEN_LIST };

// These will be converted to lowercase in cmd_map_fill
static char commands_lower[][12] = { COMMANDS_LIST };
static char opcodes_lower[][4] = { OPCODES_LIST };
#undef X

typedef enum {
    MODE_IMPLIED,              // opc             implied
    MODE_ACCUMULATOR,          // opc a           accumulator
    MODE_DIRECT_8,             // opc $01         direct
    MODE_DIRECT_16,            // opc $0123       direct
    MODE_DIRECT_24,            // opc $012345     direct
    MODE_DIRECT_8_X,           // opc $01,x       direct_x
    MODE_DIRECT_16_X,          // opc $0123,x     direct_x 
    MODE_DIRECT_24_X,          // opc $012345,x   direct_x
    MODE_DIRECT_8_Y,           // opc $01,y       direct_y
    MODE_DIRECT_16_Y,          // opc $0123,y     direct_y
    MODE_DIRECT_8_S,           // opc $01,s       direct_s
    MODE_IMMEDIATE_8,          // opc #$01        immediate
    MODE_IMMEDIATE_16,         // opc #$0123      immediate
    MODE_INDIRECT_8,           // opc ($01)       indirect
    MODE_INDIRECT_16,          // opc ($0123)     indirect
    MODE_X_INDIRECT_8,         // opc ($01,x)     x_indirect
    MODE_X_INDIRECT_16,        // opc ($0123,x)   x_indirect
    MODE_INDIRECT_8_Y,         // opc ($01,y      indirect_y
    MODE_S_INDIRECT_8_Y,       // opc ($01,s),y   s_indirect_y
    MODE_LONG_INDIRECT_8,      // opc [$01]       long_indirect
    MODE_LONG_INDIRECT_16,     // opc [$0123]     long_indirect
    MODE_LONG_INDIRECT_8_Y,    // opc [$01],y     long_indirect_y
    MODE_SRC_DEST,             // opc $01, $23    src_dest
    MODE_MAX,
    MODE_UNDEFINED = MODE_MAX,
} OpcodeMode;

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
    PARSE_MODE_NOT_DEFINED,
} OpcodeParseMode;

static
OpcodeMode parse_mode_table[][3] = {
    [PARSE_MODE_DIRECT         ] = { MODE_DIRECT_8,          MODE_DIRECT_16,        MODE_DIRECT_24   },
    [PARSE_MODE_DIRECT_X       ] = { MODE_DIRECT_8_X,        MODE_DIRECT_16_X,      MODE_DIRECT_24_X },
    [PARSE_MODE_DIRECT_Y       ] = { MODE_DIRECT_8_Y,        MODE_DIRECT_16_Y,      MODE_UNDEFINED   },
    [PARSE_MODE_DIRECT_S       ] = { MODE_DIRECT_8_S,        MODE_UNDEFINED,        MODE_UNDEFINED   },
    [PARSE_MODE_IMMEDIATE      ] = { MODE_IMMEDIATE_8,       MODE_IMMEDIATE_16,     MODE_UNDEFINED   },
    [PARSE_MODE_INDIRECT       ] = { MODE_INDIRECT_8,        MODE_INDIRECT_16,      MODE_UNDEFINED   },
    [PARSE_MODE_X_INDIRECT     ] = { MODE_X_INDIRECT_8,      MODE_X_INDIRECT_16,    MODE_UNDEFINED   },
    [PARSE_MODE_INDIRECT_Y     ] = { MODE_INDIRECT_8_Y,      MODE_UNDEFINED,        MODE_UNDEFINED   },
    [PARSE_MODE_S_INDIRECT_Y   ] = { MODE_S_INDIRECT_8_Y,    MODE_UNDEFINED,        MODE_UNDEFINED   },
    [PARSE_MODE_LONG_INDIRECT  ] = { MODE_LONG_INDIRECT_8,   MODE_LONG_INDIRECT_16, MODE_UNDEFINED   },
    [PARSE_MODE_LONG_INDIRECT_Y] = { MODE_LONG_INDIRECT_8_Y, MODE_UNDEFINED,        MODE_UNDEFINED   },
};

static char* parse_mode_patterns[] = {
    [MODE_IMPLIED             ] = "%.*s",
    [MODE_ACCUMULATOR         ] = "%.*s a",
    [MODE_DIRECT_8            ] = "%.*s $01",
    [MODE_DIRECT_16           ] = "%.*s $0123",
    [MODE_DIRECT_24           ] = "%.*s $012345",
    [MODE_DIRECT_8_X          ] = "%.*s $01,x",
    [MODE_DIRECT_16_X         ] = "%.*s $0123,x" ,
    [MODE_DIRECT_24_X         ] = "%.*s $012345,x",
    [MODE_DIRECT_8_Y          ] = "%.*s $01,y",
    [MODE_DIRECT_16_Y         ] = "%.*s $0123,y",
    [MODE_DIRECT_8_S          ] = "%.*s $01,s",
    [MODE_IMMEDIATE_8         ] = "%.*s #$01",
    [MODE_IMMEDIATE_16        ] = "%.*s #$0123",
    [MODE_INDIRECT_8          ] = "%.*s ($01)",
    [MODE_INDIRECT_16         ] = "%.*s ($0123)",
    [MODE_X_INDIRECT_8        ] = "%.*s ($01,x)",
    [MODE_X_INDIRECT_16       ] = "%.*s ($0123,x)",
    [MODE_INDIRECT_8_Y        ] = "%.*s ($01),y",
    [MODE_S_INDIRECT_8_Y      ] = "%.*s ($01,s),y",
    [MODE_LONG_INDIRECT_8     ] = "%.*s [$01]",
    [MODE_LONG_INDIRECT_16    ] = "%.*s [$0123]",
    [MODE_LONG_INDIRECT_8_Y   ] = "%.*s [$01],y",
    [MODE_SRC_DEST            ] = "%.*s $01, $23",
};


static i16 opcode_addr_mode[][MODE_MAX] = {
    //              IMPL, ACCU, DIR8, DI16, DI24, DI8X, D16X, D24X, DI8Y, D16Y, DI8S, IMM8, IM16, IND8, IN16, XIN8, XI16, IN8Y, SI8Y, LIN8, LI16, LI8Y, SRDE
    [IDENT_ADC] = {   -1,   -1, 0x65, 0x6D, 0x6F, 0x75, 0x7D, 0x7F,   -1, 0x79, 0x63, 0x69, 0x69, 0x72,   -1, 0x61,   -1, 0x71, 0x73, 0x67,   -1, 0x77,   -1 },
    [IDENT_AND] = {   -1,   -1, 0x25, 0x2D, 0x2F, 0x35, 0x3D, 0x3F,   -1, 0x39, 0x23, 0x29, 0x29, 0x32,   -1, 0x21,   -1, 0x31, 0x33, 0x27,   -1, 0x37,   -1 },
    [IDENT_ASL] = { 0x0A, 0x0A, 0x06, 0x0E,   -1, 0x16, 0x1E,   -1,   -1,   -1,   -1, 0x0A,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1 },
    [IDENT_BBR] = {   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1 },
    [IDENT_BBS] = {   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1 },
    [IDENT_BCC] = {   -1,   -1, 0x90,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1, 0x90,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1 },
    [IDENT_BCS] = {   -1,   -1, 0xB0,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1, 0xB0,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1 },
    [IDENT_BEQ] = {   -1,   -1, 0xF0,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1, 0xF0,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1 },
    [IDENT_BIT] = {   -1,   -1, 0x24, 0x2C,   -1, 0x34, 0x3C,   -1,   -1,   -1,   -1, 0x89, 0x89,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1 },
    [IDENT_BMI] = {   -1,   -1, 0x30,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1, 0x30,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1 },
    [IDENT_BNE] = {   -1,   -1, 0xD0,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1, 0xD0,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1 },
    [IDENT_BPL] = {   -1,   -1, 0x10,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1, 0x10,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1 },
    [IDENT_BRA] = {   -1,   -1, 0x80,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1, 0x80,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1 },
    [IDENT_BRK] = {   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1, 0x00,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1 },
    [IDENT_BVC] = {   -1,   -1, 0x50,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1, 0x50,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1 },
    [IDENT_BVS] = {   -1,   -1, 0x70,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1, 0x70,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1 },
    [IDENT_BRL] = {   -1,   -1,   -1, 0x82,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1, 0x82,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1 },
    [IDENT_CLC] = { 0x18,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1 },
    [IDENT_CLD] = { 0xD8,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1 },
    [IDENT_CLI] = { 0x58,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1 },
    [IDENT_CLV] = { 0xB8,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1 },
    [IDENT_CMP] = {   -1,   -1, 0xC5, 0xCD, 0xCF, 0xD5, 0xDD, 0xDF,   -1, 0xD9, 0xC3, 0xC9, 0xC9, 0xD2,   -1, 0xC1,   -1, 0xD1, 0xD3, 0xC7,   -1, 0xD7,   -1 },
    [IDENT_CPX] = {   -1,   -1, 0xE4, 0xEC,   -1,   -1,   -1,   -1,   -1,   -1,   -1, 0xE0, 0xE0,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1 },
    [IDENT_CPY] = {   -1,   -1, 0xC4, 0xCC,   -1,   -1,   -1,   -1,   -1,   -1,   -1, 0xC0, 0xC0,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1 },
    [IDENT_COP] = {   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1, 0x02,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1 },
    [IDENT_DEC] = { 0x3A, 0x3A, 0xC6, 0xCE,   -1, 0xD6, 0xDE,   -1,   -1,   -1,   -1, 0x3A,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1 },
    [IDENT_DEX] = { 0xCA,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1, 0xCA,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1 },
    [IDENT_DEY] = { 0x88,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1, 0x88,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1 },
    [IDENT_EOR] = {   -1,   -1, 0x45, 0x4D, 0x4F, 0x55, 0x5D, 0x5F,   -1, 0x59, 0x43, 0x49, 0x49, 0x52,   -1, 0x41,   -1, 0x51, 0x53, 0x47,   -1, 0x57,   -1 },
    [IDENT_INC] = { 0x1A, 0x1A, 0xE6, 0xEE,   -1, 0xF6, 0xFE,   -1,   -1,   -1,   -1, 0x1A,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1 },
    [IDENT_INX] = { 0xE8,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1, 0xE8,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1 },
    [IDENT_INY] = { 0xC8,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1, 0xC8,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1 },
    [IDENT_JMP] = {   -1,   -1,   -1, 0x4C,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1, 0x6C,   -1, 0x7C,   -1,   -1,   -1, 0xDC,   -1,   -1 },
    [IDENT_JML] = {   -1,   -1,   -1,   -1, 0x5C,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1 },
    [IDENT_JSR] = {   -1,   -1,   -1, 0x20,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1, 0xFC,   -1,   -1,   -1,   -1,   -1,   -1 },
    [IDENT_JSL] = {   -1,   -1,   -1,   -1, 0x22,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1 },
    [IDENT_LDA] = {   -1,   -1, 0xA5, 0xAD, 0xAF, 0xB5, 0xBD, 0xBF,   -1, 0xB9, 0xA3, 0xA9, 0xA9, 0xB2,   -1, 0xA1,   -1, 0xB1, 0xB3, 0xA7,   -1, 0xB7,   -1 },
    [IDENT_LDX] = {   -1,   -1, 0xA6, 0xAE,   -1,   -1,   -1,   -1, 0xB6, 0xBE,   -1, 0xA2, 0xA2,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1 },
    [IDENT_LDY] = {   -1,   -1, 0xA4, 0xAC,   -1, 0xB4, 0xBC,   -1,   -1,   -1,   -1, 0xA0, 0xA0,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1 },
    [IDENT_LSR] = { 0x4A, 0x4A, 0x46, 0x4E,   -1, 0x56, 0x5E,   -1,   -1,   -1,   -1, 0x4A,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1 },
    [IDENT_MVP] = {   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1, 0x44 },
    [IDENT_MVN] = {   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1, 0x54 },
    [IDENT_NOP] = { 0xEA,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1, 0xEA,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1 },
    [IDENT_ORA] = {   -1,   -1, 0x05, 0x0D, 0x0F, 0x15, 0x1D, 0x1F,   -1, 0x19, 0x03, 0x09, 0x09, 0x12,   -1, 0x01,   -1, 0x11, 0x13, 0x07,   -1, 0x17,   -1 },
    [IDENT_PHA] = { 0x48,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1 },
    [IDENT_PHP] = { 0x08,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1 },
    [IDENT_PHX] = { 0xDA,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1 },
    [IDENT_PHY] = { 0x5A,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1 },
    [IDENT_PLA] = { 0x68,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1 },
    [IDENT_PLP] = { 0x28,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1 },
    [IDENT_PLX] = { 0xFA,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1 },
    [IDENT_PLY] = { 0x7A,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1 },
    [IDENT_PHB] = { 0x8B,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1 },
    [IDENT_PHD] = { 0x0B,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1 },
    [IDENT_PHK] = { 0x4B,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1 },
    [IDENT_PLB] = { 0xAB,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1 },
    [IDENT_PLD] = { 0x2B,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1 },
    [IDENT_PEA] = {   -1,   -1,   -1, 0xF4,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1 },
    [IDENT_PEI] = {   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1, 0xD4,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1 },
    [IDENT_PER] = {   -1,   -1,   -1, 0x62,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1 },
    [IDENT_RMB] = {   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1 },
    [IDENT_ROL] = { 0x2A, 0x2A, 0x26, 0x2E,   -1, 0x36, 0x3E,   -1,   -1,   -1,   -1, 0x2A,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1 },
    [IDENT_ROR] = { 0x6A, 0x6A, 0x66, 0x6E,   -1, 0x76, 0x7E,   -1,   -1,   -1,   -1, 0x6A,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1 },
    [IDENT_RTI] = { 0x40,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1 },
    [IDENT_RTS] = { 0x60,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1 },
    [IDENT_REP] = {   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1, 0xC2,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1 },
    [IDENT_RTL] = { 0x6B,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1 },
    [IDENT_SBC] = {   -1,   -1, 0xE5, 0xED, 0xEF, 0xF5, 0xFD, 0xFF,   -1, 0xF9, 0xE3, 0xE9, 0xE9, 0xF2,   -1, 0xE1,   -1, 0xF1, 0xF3, 0xE7,   -1, 0xF7,   -1 },
    [IDENT_SEC] = { 0x38,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1 },
    [IDENT_SED] = { 0xF8,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1 },
    [IDENT_SEI] = { 0x78,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1 },
    [IDENT_SMB] = {   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1 },
    [IDENT_STA] = {   -1,   -1, 0x85, 0x8D, 0x8F, 0x95, 0x9D, 0x9F,   -1, 0x99, 0x83,   -1,   -1, 0x92,   -1, 0x81,   -1, 0x91, 0x93, 0x87,   -1, 0x97,   -1 },
    [IDENT_STX] = {   -1,   -1, 0x86, 0x8E,   -1,   -1,   -1,   -1, 0x96,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1 },
    [IDENT_STY] = {   -1,   -1, 0x84, 0x8C,   -1, 0x94,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1 },
    [IDENT_STZ] = {   -1,   -1, 0x64, 0x9C,   -1, 0x74, 0x9E,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1 },
    [IDENT_SEP] = {   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1, 0xE2,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1 },
    [IDENT_STP] = { 0xDB,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1 },
    [IDENT_TAX] = { 0xAA,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1 },
    [IDENT_TAY] = { 0xA8,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1 },
    [IDENT_TRB] = {   -1,   -1, 0x14, 0x1C,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1 },
    [IDENT_TSB] = {   -1,   -1, 0x04, 0x0C,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1 },
    [IDENT_TSA] = { 0x3B,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1 },
    [IDENT_TAS] = { 0x1B,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1 },
    [IDENT_TDA] = { 0x7B,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1 },
    [IDENT_TAD] = { 0x5B,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1 },
    [IDENT_TSX] = { 0xBA,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1 },
    [IDENT_TXA] = { 0x8A,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1 },
    [IDENT_TXS] = { 0x9A,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1 },
    [IDENT_TYA] = { 0x98,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1 },
    [IDENT_TXY] = { 0x9B,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1 },
    [IDENT_TYX] = { 0xBB,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1 },
    [IDENT_TCD] = { 0x5B,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1 },
    [IDENT_TDC] = { 0x7B,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1 },
    [IDENT_TCS] = { 0x1B,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1 },
    [IDENT_TSC] = { 0x3B,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1 },
    [IDENT_WAI] = { 0xCB,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1 },
    [IDENT_WDM] = {   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1, 0x42,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1 },
    [IDENT_XBA] = { 0xEB,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1 },
    [IDENT_XCE] = { 0xFB,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1 },
};

u32 num_bytes_of_u32(u32 i) {
    return (sizeof(int) * 8 - __builtin_clz(i) + 7) / 8;
}

u32 define_equal(Define e1, Define e2) {
    return string_equal(e1.name, e2.name);
}

u32 define_hash(Define e) {
    return SuperFastHash(e.name);
}

u32 label_equal(Label e1, Label e2) {
    return string_equal(e1.name, e2.name);
}

u32 label_hash(Label e) {
    return SuperFastHash(e.name);
}

u32 identifier_hash(Identifier e) {
    String s = e.name;
    return SuperFastHash(s);
}

u32 identifier_equal(Identifier e1, Identifier e2) {
    return string_equal(e1.name, e2.name);
}

implement_hashmap(DefineMap, define_map, Define, define_hash, define_equal);
implement_hashmap(LabelMap, label_map, Label, label_hash, label_equal);
implement_hashmap(IdentifierMap, identifier_map, Identifier, identifier_hash, identifier_equal);
IdentifierMap identifier_map;

implement_stack(AddrStack, addr_stack, Interval);

#ifndef TEXT_POS
#undef error_at_pos
#define error_at_pos(a)
#undef warn_at_pos
#define warn_at_pos(a)
#endif

void identifier_map_fill() {
    identifier_map_init(&identifier_map, 256);
    for(int i = 0; i < array_length(opcodes); i++) {
        Identifier entry = {
            .name = string_from_c_string(opcodes[i]),
            .type = IDENT_OPCODE + i + 1,
        };
        identifier_map_insert(&identifier_map, entry);
        for(char* p = opcodes_lower[i]; *p; p++) *p = *p + ('a' - 'A');
        entry = (Identifier){
            .name = string_from_c_string(opcodes_lower[i]),
            .type = IDENT_OPCODE + i + 1,
        };
        identifier_map_insert(&identifier_map, entry);
    }
    
    for(int i = 0; i < array_length(commands); i++) {
        Identifier entry = {
            .name = string_from_c_string(commands[i]),
            .type = IDENT_COMMAND + i + 1,
        };
        identifier_map_insert(&identifier_map, entry);
        for(char* p = commands_lower[i]; *p; p++) *p = *p + ('a' - 'A');
        entry = (Identifier){
            .name = string_from_c_string(commands_lower[i]),
            .type = IDENT_COMMAND + i + 1,
        };
        identifier_map_insert(&identifier_map, entry);
    }
    
}

implement_stack(LexerStack, lexer_stack, LexerState);


static
int is_ident_char_start(char c) {
    return (c >= 'a' && c <= 'z')
        || (c >= 'A' && c <= 'Z')
        || (c == '_');
}

static
int is_ident_char(char c) {
    return (c >= 'a' && c <= 'z')
        || (c >= 'A' && c <= 'Z')
        || (c >= '0' && c <= '9')
        || (c == '_');
}

static
int is_whitespace(char c) {
    return c == ' ' || c == '\t';
}

static
void lexer_init(Lexer* lexer, FreeList* free_list) {
    lexer->free_list = free_list;
    lexer_stack_init(&lexer->lexer_stack, free_list, 512);
    define_map_init(&lexer->define_map, 1024);
}

static
void lexer_deinit(Lexer* lexer) {
    lexer_stack_deinit(&lexer->lexer_stack, lexer->free_list);
    define_map_deinit(&lexer->define_map);
}

static
void lexer_push_buffer(Lexer* lexer, Buffer buffer
#ifdef TEXT_POS
                       , TextPos text_pos
#endif
    ) {
    // Only add to the stack, if the buffer is non-empty
    if(buffer.begin < buffer.end) {
        LexerState lexer_state = {
            .buffer = buffer,
            .pos = buffer.begin,
#ifdef TEXT_POS
            .text_pos = text_pos,
#endif
        };
        lexer->top = lexer_stack_push(&lexer->lexer_stack, lexer_state);
        lexer->pos = buffer.begin;
        lexer->end = buffer.end;
#ifdef TEXT_POS
        lexer->text_pos = text_pos;
#endif
    }
    //print_stack(lexer->lexer_stack);
}


static
void lexer_push_file(Lexer* lexer, Path* file, Arena* arena) {
    Buffer buffer = path_open_file(file, arena);
    String file_name = path_get_name(file, arena);
#ifdef TEXT_POS
    TextPos text_pos = {
        .file_name = file_name,
        .line_number = 1,
        .line_pos = 0,
        .line_start = buffer.begin
    };
#endif
    lexer_push_buffer(lexer, buffer
#ifdef TEXT_POS
                      , text_pos
#endif
        );
}

static
void lexer_pop(Lexer* lexer) {
    LexerState* lexer_state;
    lexer->top = lexer_state = lexer_stack_pop(&lexer->lexer_stack);
    lexer->pos = lexer_state->pos;
    lexer->end = lexer_state->buffer.end;
    if(lexer_stack_is_empty(&lexer->lexer_stack)) lexer->top = NULL;
#ifdef TEXT_POS
    lexer->text_pos = lexer_state->text_pos;
#endif
    //print_stack(lexer->lexer_stack);
}



static
void lexer_advance(Lexer* lexer) {
    LexerState* lexer_state = lexer->top;
    if(!lexer_state) return;
    lexer_state->pos++;

    if(lexer_state->pos == (char*)lexer_state->buffer.end) {
        lexer_pop(lexer);
        return;
    }
    
#ifdef TEXT_POS
    lexer_state->text_pos.line_pos++;
    if(lexer_state->text_pos.line_start == lexer_state->pos) {
        lexer_state->text_pos.line_pos = 0;
        lexer_state->text_pos.line_number++;
    }
    if(lexer_state->pos[0] == '\n') lexer_state->text_pos.line_start = lexer_state->pos + 1;
#endif
    lexer->pos = lexer_state->pos;
    lexer->end = lexer_state->buffer.end;
#ifdef TEXT_POS
    lexer->text_pos = lexer_state->text_pos;
#endif
}



static
Token lexer_set_token_pos(Lexer* lexer) {
    Token result = {
#ifdef TEXT_POS
        .text_pos    = lexer->text_pos,
#endif
        .text.data   = lexer->pos
    };
    return result;
}




static
void lexer_get_token_number(Lexer* lexer, int base, Token* result) {
    // TODO: Check if each digit is smaller than base.
    u64 num = 0;
    if(lexer->pos[0] >= '0' && lexer->pos[0] <= '9') {
        num = lexer->pos[0] - '0';
    } else if(lexer->pos[0] >= 'A' && lexer->pos[0] <= 'F') {
        num = lexer->pos[0] - 'A' + 10;
    } else if(lexer->pos[0] >= 'a' && lexer->pos[0] <= 'f') {
        num = lexer->pos[0] - 'a' + 10;
    } else {
        error_at_pos(result->text_pos);
        fprintf(stderr, "Not a %i-ary digit!\n", base);
        exit(1);
    }
    u64 max_num = base;
    while(lexer->pos + 1 < lexer->end) {
        int digit;
        if(lexer->pos[1] >= '0' && lexer->pos[1] <= '9') {
            digit = lexer->pos[1] - '0';
        } else if(lexer->pos[1] >= 'A' && lexer->pos[1] <= 'F') {
            digit = lexer->pos[1] - 'A' + 10;
        } else if(lexer->pos[1] >= 'a' && lexer->pos[1] <= 'f') {
            digit = lexer->pos[1] - 'a' + 10;
        } else {
            break;
        }
        num = num * base + digit;
        max_num = max_num * base;
        lexer_advance(lexer);
    }
    result->type  = TOKEN_NUM;
    result->num   = num;
    result->text.length = lexer->pos - result->text.data + 1;
    result->bytes = num_bytes_of_u32(max_num - 1);
}

static
Token lexer_get_token(Lexer* lexer) {
    Token result = { 0 };
    if(!lexer->top) {
        result.type = TOKEN_EOF;
        return result;
    }
    result = lexer_set_token_pos(lexer);

    result.type = TOKEN_NULL;
    while(result.type == TOKEN_NULL) {
        if(is_whitespace(lexer->pos[0])) {
            while(is_whitespace(lexer->pos[0])) {
                lexer_advance(lexer);
            }
            result = lexer_set_token_pos(lexer);
        }
        if(lexer->pos[0] == ';') {
            while(lexer->pos[0] != '\n') {
                if(lexer->pos[0] == '\0') {
                    result.type = TOKEN_EOF;
                    return result;
                }
                lexer_advance(lexer);
            }
            result.type = TOKEN_CMD_DELIM;

        } else if(lexer->pos[0] == '\r' && lexer->pos + 1 < lexer->end && lexer->pos[1] == '\n') {
            result.type = TOKEN_CMD_DELIM;
            lexer_advance(lexer);
        } else if(lexer->pos[0] == '\n' || lexer->pos[0] == ':') {
            result.type = TOKEN_CMD_DELIM;

        } else if(is_ident_char_start(lexer->pos[0])) {
            while(lexer->pos + 1 < lexer->end && is_ident_char(lexer->pos[1])) {
                lexer_advance(lexer);
            }
            if(lexer->pos[1] == ':') {
                result.type = TOKEN_LABEL_DEF;
                result.text.length = lexer->pos - result.text.data + 1;
            } else {
                result.text.length = lexer->pos - result.text.data + 1;
                Identifier entry = {
                    .name = result.text,
                };
                Identifier* found = identifier_map_find(&identifier_map, entry);
                if(found) {
                    result.type = TOKEN_IDENTIFIER;
                    result.ident_type = found->type;
                } else {
                    result.type = TOKEN_LABEL;
                }
            }

        } else if((lexer->pos[0] >= '0' && lexer->pos[0] <= '9')) {
            lexer_get_token_number(lexer, 10, &result);

        } else if(lexer->pos[0] == '$') {
            lexer_advance(lexer);
            result.text.data++;
            lexer_get_token_number(lexer, 16, &result);

        } else if(lexer->pos[0] == '%') {
            lexer_advance(lexer);
            result.text.data++;
            lexer_get_token_number(lexer, 2, &result);

        } else if(lexer->pos[0] == '#') {
            result.type = TOKEN_HASH;
            result.text.length = lexer->pos - result.text.data;
            
        } else if(lexer->pos[0] == '.') {
            result.type = TOKEN_DOT;
            result.text.length = lexer->pos - result.text.data;

        } else if(lexer->pos[0] == '<') {
            if(lexer->pos + 1 < lexer->end && lexer->pos[1] == '<') {
                lexer_advance(lexer);
                result.type = TOKEN_LSHIFT;
            } else {
                result.type = TOKEN_LESS;
            }
            result.text.length = lexer->pos - result.text.data;

        } else if(lexer->pos[0] == '>') {
            if(lexer->pos + 1 < lexer->end && lexer->pos[1] == '>') {
                lexer_advance(lexer);
                result.type = TOKEN_RSHIFT;
            } else {
                result.type = TOKEN_GREATER;
            }
            result.text.length = lexer->pos - result.text.data + 1;

        } else if(lexer->pos[0] == '(') {
            result.type = TOKEN_LPAREN;
            result.text.length = 1;

        } else if(lexer->pos[0] == ')') {
            result.type = TOKEN_RPAREN;
            result.text.length = 1;

        } else if(lexer->pos[0] == '[') {
            result.type = TOKEN_LBRACKET;
            result.text.length = 1;

        } else if(lexer->pos[0] == ']') {
            result.type = TOKEN_RBRACKET;
            result.text.length = 1;

        } else if(lexer->pos[0] == '{') {
            result.type = TOKEN_LBRACE;
            result.text.length = 1;

        } else if(lexer->pos[0] == '}') {
            result.type = TOKEN_RBRACE;
            result.text.length = 1;

        } else if(lexer->pos[0] == '=') {
            result.type = TOKEN_EQUAL;
            result.text.length = 1;

        } else if(lexer->pos[0] == ',') {
            result.type = TOKEN_COMMA;
            result.text.length = 1;

        } else if(lexer->pos[0] == '*') {
            result.type = TOKEN_TIMES;
            result.text.length = 1;

        } else if(lexer->pos[0] == '/') {
            result.type = TOKEN_SLASH;
            result.text.length = 1;

        } else if(lexer->pos[0] == '|') {
            result.type = TOKEN_PIPE;
            result.text.length = 1;

        } else if(lexer->pos[0] == '&') {
            result.type = TOKEN_AMPERSAND;
            result.text.length = 1;

        } else if(lexer->pos[0] == '!') {
            result.text.data = lexer->pos + 1;
            while(lexer->pos + 1 < lexer->end && is_ident_char(lexer->pos[1])) {
                lexer_advance(lexer);
            }

            result.text.length = lexer->pos - result.text.data + 1;
            
            Define define = {
                .name = result.text,
            };
            lexer_advance(lexer);
            Define* found = define_map_find(&lexer->define_map, define);
            if(found) {
                lexer_push_buffer(lexer, found->buffer
#ifdef TEXT_POS
                                  , found->text_pos
#endif
                    );
                result = lexer_set_token_pos(lexer);
                continue;
            } else {
                result.type = TOKEN_DEFINE;
            }


        } else if(lexer->pos[0] == '"') {
            lexer_advance(lexer);
            result.text.data++;
            while(lexer->pos[0] != '"') {
                if(lexer->pos[0] == '\0') {
                    error_at_pos(result.text_pos);
                    fprintf(stderr, "Unterminated string!\n");
                    exit(0);
                };
                lexer_advance(lexer);
            }
            result.text.length = lexer->pos - result.text.data;
            result.type = TOKEN_STRING;
            
        } else if(lexer->pos[0] == '+') {
            if(lexer->pos + 1 >= lexer->end || lexer->pos[1] != '+') {
                result.type = TOKEN_PLUS;
            } else {
                result.type = TOKEN_ANON_LABEL;
                while(lexer->pos + 1 < lexer->end && lexer->pos[1] == '+') {
                    lexer_advance(lexer);
                }
                result.num = lexer->pos - result.text.data;
            }

        } else if(lexer->pos[0] == '-') {
            if(lexer->pos + 1 >= lexer->end || (lexer->pos[1] != '-' && lexer->pos[1] != '>')) {
                result.type = TOKEN_MINUS;
            } else if(lexer->pos[1] == '>') {
                result.type = TOKEN_ARROW;
                    lexer_advance(lexer);
            } else {
                result.type = TOKEN_ANON_LABEL;
                while(lexer->pos + 1 < lexer->end && lexer->pos[1] == '-') {
                    lexer_advance(lexer);
                }
                result.num = -(lexer->pos - result.text.data);
            }

        } else {
            error_at_pos(lexer->top->text_pos);
            fprintf(stderr, "Unexpected character '%c'!\n", lexer->pos[0]);
            exit(1);
        }

        lexer_advance(lexer);
    }

    return result;
}




void parser_init(ParserState* parser, Wdc65816Rom* rom, Arena* arena, FreeList* free_list,
                 Path* file, Path* working_directory) {
    parser->token = (Token){ 0 };
    lexer_init(&parser->lexer, free_list);
    label_map_init(&parser->label_map, 512);
    parser->arena = arena;
    parser->rom   = rom;
    parser->statement_arena = arena_subarena(arena, MB(32));
    lexer_push_file(&parser->lexer, file, parser->arena);
    addr_stack_init(&parser->pc_stack, free_list, 256);
    Interval pc = { 0 };
    addr_stack_push(&parser->pc_stack, pc);
    parser->pc = addr_stack_top(&parser->pc_stack);
}

void parser_deinit(ParserState* parser) {
    label_map_deinit(&parser->label_map);
    lexer_deinit(&parser->lexer);
    // TODO:
    //addr_stack_deinit(&parser->pc_stack);
}



static
void parser_advance(ParserState* parser) {
    parser->token = lexer_get_token(&parser->lexer);
#if 0
    printf("%s(%.*s)\n", token_names[parser->token.type],
           parser->token.text.length, parser->token.text.data);
//    if(parser->token.type == TOKEN_CMD_DELIM) printf("\n");
//    if(parser->token.type == TOKEN_EOF) printf("\n");
#endif
}

#ifdef TEXT_POS
#define parser_expect(parser, token_type)                               \
    do {                                                                \
        if(parser->token.type != token_type) {                          \
            error_at_pos(parser->token.text_pos);                       \
            fprintf(stderr, "Expected token \"%s\", got \"%s\".\n", token_names[token_type], \
                    token_names[parser->token.type]);                   \
            exit(1);                                                    \
        }                                                               \
    } while(0);

#define parser_expect_expr(parser, expr)                                \
    do {                                                                \
        if(expr.type == EXPR_NULL) {                                    \
            error_at_pos(parser->token.text_pos);                       \
            fprintf(stderr, "Expected expression.\n");                  \
            exit(1);                                                    \
        }                                                               \
    } while(0);
#else
#define parser_expect(parser, token_type)                               \
    do {                                                                \
        if(parser->token.type != token_type) {                          \
            fprintf(stderr, "Expected token \"%s\", got \"%s\".\n", token_names[token_type], \
                    token_names[parser->token.type]);                   \
            exit(1);                                                    \
        }                                                               \
    } while(0);

#define parser_expect_expr(parser, expr)                                \
    do {                                                                \
        if(expr.type == EXPR_NULL) {                                    \
            fprintf(stderr, "Expected expression.\n");                  \
            exit(1);                                                    \
        }                                                               \
    } while(0);
#endif

typedef enum {
    EXPR_NULL,
    /* Binary operands */
    EXPR_MULT,
    EXPR_DIV,
    EXPR_ADD,
    EXPR_SUB,
    EXPR_LSHIFT,
    EXPR_RSHIFT,
    EXPR_BITAND,
    EXPR_BITOR,
    /* Unary operands */
    EXPR_NEG,
    /* Nullary operands */
    EXPR_NUM,
    EXPR_LABEL,
} ExprType;
    
typedef struct {
    Interval value;
    u8  width;
} Num;

typedef struct Expr_ {
    ExprType      type;
    struct Expr_* op1;
    struct Expr_* op2;
    Num           num;
    String        name;
} Expr;

static Num num_add(Num a, Num b) {
    Num result = {
        .value.min.d = a.value.min.d + b.value.min.d,
        .value.max.d = a.value.max.d + b.value.max.d,
        .width = max(a.width, b.width)
    };
    assert(result.value.min.d <= result.value.max.d);
    return result;
}

static Num num_sub(Num a, Num b) {
    Num result = {
        .value.min.d = a.value.min.d - b.value.max.d,
        .value.max.d = a.value.max.d - b.value.min.d,
        .width = max(a.width, b.width)
    };
    assert(result.value.min.d <= result.value.max.d);
    return result;
}

static Num num_mul(Num a, Num b) {
    Num result = {
        .value.min.d = a.value.min.d * b.value.min.d,
        .value.max.d = a.value.max.d * b.value.max.d,
        .width = max(a.width, b.width)
    };
    assert(result.value.min.d <= result.value.max.d);
    return result;
}

static Num num_div(Num a, Num b) {
    Num result = {
        .value.min.d = a.value.min.d / b.value.max.d,
        .value.max.d = a.value.max.d / b.value.min.d,
        .width = max(a.width, b.width)
    };
    assert(result.value.min.d <= result.value.max.d);
    return result;
}

static Num num_lshift(Num a, Num b) {
    if(b.value.min.d != b.value.max.d) {
        fprintf(stderr, "Shifting with an interval on the right hand side with non-zero length"
                "is not implemented\n");
        exit(1);
    }
    Num result = a;
    if(b.value.min.d) {
        result.value.min.d = a.value.min.d << b.value.min.d;
        result.value.max.d = a.value.max.d << b.value.min.d;
        result.width = min(3, a.width + b.value.min.d / 8);
    };
    assert(result.value.min.d <= result.value.max.d);
    return result;
}

static Num num_rshift(Num a, Num b) {
    if(b.value.min.d != b.value.max.d) {
        fprintf(stderr, "Shifting with an interval on the right hand side with non-zero length"
                "is not implemented\n");
        exit(1);
    }
    Num result = a;
    if(b.value.min.d) {
        result.value.min.d = a.value.min.d >> b.value.min.d;
        result.value.max.d = a.value.max.d >> b.value.min.d;
        result.width = min(1, a.width - b.value.min.d / 8);
    };
    assert(result.value.min.d <= result.value.max.d);
    return result;
}

static Num num_bitand(Num a, Num b) {
    //TODO: This is by no means correct.
    Num result = {
        .value.min.d = a.value.min.d & b.value.min.d,
        .value.max.d = a.value.max.d & b.value.max.d,
        .width = max(a.width, b.width)
    };
    assert(result.value.min.d <= result.value.max.d);
    return result;
}

static Num num_bitor(Num a, Num b) {
    //TODO: This is by no means correct.
    Num result = {
        .value.min.d = a.value.min.d | b.value.min.d,
        .value.max.d = a.value.max.d | b.value.max.d,
        .width = max(a.width, b.width)
    };
    assert(result.value.min.d <= result.value.max.d);
    return result;
}

typedef struct {
    int token_type;
    Num (*op)(Num, Num);
    int level;
} bin_op_hierarchy_entry;

static
const bin_op_hierarchy_entry hierarchy[] = {
    [EXPR_NULL  ] = { 0 },
    [EXPR_MULT  ] = { .token_type = TOKEN_TIMES,     .op = num_mul,    .level = 4 },
    [EXPR_DIV   ] = { .token_type = TOKEN_SLASH,     .op = num_div,    .level = 4 },
    [EXPR_BITAND] = { .token_type = TOKEN_AMPERSAND, .op = num_bitand, .level = 3 },
    [EXPR_BITOR ] = { .token_type = TOKEN_PIPE,      .op = num_bitor,  .level = 3 },
    [EXPR_LSHIFT] = { .token_type = TOKEN_LSHIFT,    .op = num_lshift, .level = 2 },
    [EXPR_RSHIFT] = { .token_type = TOKEN_RSHIFT,    .op = num_rshift, .level = 2 },
    [EXPR_ADD   ] = { .token_type = TOKEN_PLUS,      .op = num_add,    .level = 1 },
    [EXPR_SUB   ] = { .token_type = TOKEN_MINUS,     .op = num_sub,    .level = 1 },
};

static
Expr parse_expr_(ParserState* parser, int level) {
    Expr op1 = { 0 };
    Expr op2 = { 0 };
    
    if(parser->token.type == TOKEN_LPAREN) {
        parser_advance(parser);
        op1 = parse_expr_(parser, 0);
        parser_expect_expr(parser, op1);
        parser_expect(parser, TOKEN_RPAREN);
        parser_advance(parser);
    } else if(parser->token.type == TOKEN_PLUS) {
        op1 = parse_expr_(parser, 14);
    } else if(parser->token.type == TOKEN_MINUS) {
        op1 = parse_expr_(parser, 14);
        if(op1.type == EXPR_NUM) {
            i32 temp = op1.num.value.min.d;
            op1.num.value.min.d = -op1.num.value.max.d;
            op1.num.value.max.d = -temp;
        } else {
            // we copy op1 into neg_expr and use op1 below
            // as the result of the negation
            Expr* neg_expr = arena_alloc_type(parser->arena, Expr);
            *neg_expr = op1;
            op1 = (Expr) {
                .type = EXPR_NEG,
                .op1  = neg_expr,
            };
        }
    } else if(parser->token.type == TOKEN_NUM) {
        op1 = (Expr){
            .type  = EXPR_NUM,
            .num.value.min.d = parser->token.num,
            .num.value.max.d = parser->token.num,
            .num.width = parser->token.bytes,
        };
        parser_advance(parser);
    } else if(parser->token.type == TOKEN_LABEL) {
        op1 = (Expr){
            .type  = EXPR_LABEL,
            .name  = parser->token.text,
            .num.value = *parser->pc,
            .num.width = 3,
        };
        parser_advance(parser);
    } else {
        return (Expr){ 0 };
    }

#define parse_binary_operator(expr_type) {                              \
        if(level >= hierarchy[expr_type].level) break;                  \
        parser_advance(parser);                                         \
        op2 = parse_expr_(parser, hierarchy[expr_type].level);          \
        if(op1.type == EXPR_NUM && op2.type == EXPR_NUM) {              \
            op1.num   = hierarchy[expr_type].op(op1.num, op2.num);      \
        } else {                                                        \
            Expr* ops = arena_alloc_array(parser->arena, 2, Expr);      \
            ops[0] = op1; ops[1] = op2;                                 \
            op1 = (Expr){ .type = expr_type, .op1 = ops + 0, .op2 = ops + 1, \
                         .num = hierarchy[expr_type].op(op1.num, op2.num) }; \
        }                                                               \
        continue;                                                       \
    }

    while(1) {
        if(parser->token.type == TOKEN_TIMES)     parse_binary_operator(EXPR_MULT);
        if(parser->token.type == TOKEN_SLASH)     parse_binary_operator(EXPR_DIV);
        if(parser->token.type == TOKEN_AMPERSAND) parse_binary_operator(EXPR_BITAND);
        if(parser->token.type == TOKEN_PIPE)      parse_binary_operator(EXPR_BITOR);
        if(parser->token.type == TOKEN_LSHIFT)    parse_binary_operator(EXPR_LSHIFT);
        if(parser->token.type == TOKEN_RSHIFT)    parse_binary_operator(EXPR_RSHIFT);
        if(parser->token.type == TOKEN_PLUS)      parse_binary_operator(EXPR_ADD);
        if(parser->token.type == TOKEN_MINUS)     parse_binary_operator(EXPR_SUB);

        break;
	}
#undef parse_binary_operator
	return op1;
}

static
Expr parse_expr(ParserState* parser) {
    return parse_expr_(parser, 0);
}

static
void parser_write_pc(ParserState* parser, u8 data) {
    /* Wdc65816Reg24* pc = parser->pc; */
    /* if(parser->bank_crossed) { */
    /*     warn_at_pos(parser->token.text_pos); */
    /*     fprintf(stderr, "Bank crossing\n"); */
    /* } */
    /* if(pc->w == 0xFFFF) { */
    /*     parser->bank_crossed = 1; */
    /* } else { */
    /*     parser->bank_crossed = 0; */
    /* } */
        
    /* u8* ptr = wdc65816_mapper_ptr(&parser->rom->read_mapper, pc->d); */
    /* if(!ptr) { */
    /*     error_at_pos(parser->token.text_pos); */
    /*     fprintf(stderr, "Write to unmapped address.\n"); */
    /*     exit(1); */
    /* } */
    /* *ptr = data; */
    /* pc->w++; */
}

typedef struct {
    u8 op_code;
    Expr op1;
    Expr op2;
    Expr op3;
    ushort width;
} OpEncoding;

typedef enum {
    STATEMENT_TYPE_NULL,
    STATEMENT_TYPE_INCBIN,
    STATEMENT_TYPE_TABLE,
    STATEMENT_TYPE_CLEARTABLE,
    STATEMENT_TYPE_FILLBYTE,
    STATEMENT_TYPE_DB,
    STATEMENT_TYPE_ORG,
    STATEMENT_TYPE_WARNPC,
    STATEMENT_TYPE_FILL,
    STATEMENT_TYPE_BASE,
    STATEMENT_TYPE_W65816,
} StatementType;

typedef struct Statement {
    StatementType type;
    union {
        struct {
            String file_name;
            Expr location;
            u8 include_somewhere_else : 1;
        } incbin;
        struct {
            String file_name;
        } table;
        struct { } cleartable;
        struct {
            Expr length;
        } fill;
        struct {
            Expr expr;
        } fillbyte;
        struct {
            Expr* data;
            uint length;
            uint data_size;
        } db;
        struct {
            Expr addr;
        } org;
        struct { // base
            Expr addr;
            u8  off : 1;
        } base;
        struct { // base
            Expr expr;
        } warnpc;
        Expr value;
        OpEncoding wdc65816;
    };
} Statement;

void parse_and_exec_incsrc(ParserState* parser) {
    parser_advance(parser);
    parser_expect(parser, TOKEN_STRING);
    String file_name = parser->token.text;
    parser_advance(parser);
    Path* file = arena_alloc_type(parser->arena, Path);
    path_init_from(file, parser->working_directory, file_name);
    lexer_push_file(&parser->lexer, file, parser->arena);
    parser_advance(parser);
}

void parse_incbin(ParserState* parser, Statement* stmt) {
    parser_advance(parser);
    parser_expect(parser, TOKEN_STRING);

    stmt->incbin.file_name = parser->token.text;
    
    parser_advance(parser);
    stmt->type = STATEMENT_TYPE_INCBIN;
    stmt->incbin.include_somewhere_else = 0;
    stmt->incbin.location = (Expr) {
        .num.value = *parser->pc,
        .num.width = 3,
        .type = EXPR_NUM
    };

    if(parser->token.type == TOKEN_ARROW) {
        parser_advance(parser);
        Expr expr = parse_expr(parser);
        if(expr.type != EXPR_NUM) {
            error_at_pos(parser->token.text_pos);
            fprintf(stderr, "Expression not fully computable.\n");
            exit(1);
        }
        stmt->incbin.location = expr;
        stmt->incbin.include_somewhere_else = 1;
    }
}

void exec_incbin(ParserState* parser, Statement* stmt) {
    /* Path* file = arena_alloc_type(parser->arena, Path); */
    /* path_init_from(file, parser->working_directory, stmt->file_name); */
    /* Temp temp = temp_begin(parser->arena); */
    /* Buffer buffer = path_open_file(file, parser->arena); */
    /* ulong file_size = path_get_file_size(file); */

    /* wdc65816_mapper_write_range(&parser->rom->read_mapper, stmt->location, */
    /*                             stmt->location + file_size, buffer.begin); */
    /* temp_end(temp); */
    /* if(!stmt->include_somewhere_else) parser->pc->d += file_size; */
}

static
void parse_table(ParserState* parser, Statement* stmt) {
    parser_advance(parser);
    parser_expect(parser, TOKEN_STRING);
    stmt->type = STATEMENT_TYPE_TABLE;
    stmt->table.file_name = parser->token.text;
    parser_advance(parser);
}

static
void exec_table(ParserState* parser, Statement* stmt) {
    //TODO: Implement this
}

static
void parse_fillbyte(ParserState* parser, Statement* stmt) {
    parser_advance(parser);
    Expr expr = parse_expr(parser);
    stmt->fillbyte.expr = expr;
    stmt->type = STATEMENT_TYPE_FILLBYTE;
}

static
void exec_fillbyte(ParserState* parser, Statement* stmt) {
    Interval interval = stmt->fillbyte.expr.num.value;
    if(stmt->fillbyte.expr.type != EXPR_NUM || interval.min.d != interval.max.d) {
        error_at_pos(parser->token.text_pos);
        fprintf(stderr, "Expression not fully computable.\n");
        exit(1);
    }
    parser->fill_byte = interval.min.d;
}

static
void db_statement_write(Statement* stmt, Expr expr, Arena* arena) {
    arena_alloc_array(arena, 1, Expr);
    stmt->db.data[stmt->db.length] = expr;
    stmt->db.length++;
}

static
void parse_db_entry(ParserState* parser, uint data_size, Statement* stmt) {
    Expr expr;
    if(parser->token.type == TOKEN_STRING) {
        for(int i = 0; i < parser->token.text.length; i++) {
            //TODO: Table conversion
            parser_write_pc(parser, ((u8*)parser->token.text.data)[i]);
        }
        parser_advance(parser);
    } else {
        expr = parse_expr(parser);
        if(expr.type == EXPR_NULL) {
            error_at_pos(parser->token.text_pos);
            fprintf(stderr, "Expected expression or string\n");
            exit(1);
        }
        db_statement_write(stmt, expr, parser->arena);
    }
}

static
void parse_db(ParserState* parser, uint data_size, Statement* stmt) {
    stmt->db.data = arena_alloc_array(parser->arena, 0, Expr);
    stmt->db.length = 0;
    stmt->db.data_size = data_size;
    stmt->type = STATEMENT_TYPE_DB;

    parser_advance(parser);
    parse_db_entry(parser, data_size, stmt);
    while(parser->token.type != TOKEN_CMD_DELIM) {
        if(parser->token.type != TOKEN_COMMA) {
            error_at_pos(parser->token.text_pos);
            fprintf(stderr, "Expected command delimiter or comma.\n");
            exit(1);
        } else {
            parser_advance(parser);
            parse_db_entry(parser, data_size, stmt);
        }
    }
}

static
void exec_db(ParserState* parser, Statement* stmt) {
    /* for(int i = 0; i < stmt->length; i++) { */
    /*     parser_write_pc(parser, stmt->data[i]); */
    /* } */
}

static
void parse_org(ParserState* parser, Statement* stmt) {
    parser_advance(parser);
    Expr expr = parse_expr(parser);
    parser_expect_expr(parser, expr);
    stmt->org.addr = expr;
    stmt->type = STATEMENT_TYPE_ORG;
}

void exec_org(ParserState* parser, Statement* stmt) {
    if(stmt->org.addr.type != EXPR_NUM) {
        error_at_pos(parser->token.text_pos);
        fprintf(stderr, "Expression not fully computable.\n");
        exit(1);
    }
    *parser->pc = stmt->org.addr.num.value;
    parser->bank_crossed = 0;
}

static
void parse_warnpc(ParserState* parser, Statement* stmt) {
    parser_advance(parser);
    Expr expr = parse_expr(parser);
    parser_expect_expr(parser, expr);
    stmt->value = expr;
    stmt->type = STATEMENT_TYPE_WARNPC;
}

static
void exec_warnpc(ParserState* parser, Statement* stmt) {
    // TODO: stmt->value.num.value.min.d is really ugly.
    // This should be renamed!
    if(stmt->value.type != EXPR_NUM || stmt->value.num.value.min.d != stmt->value.num.value.max.d) {
        error_at_pos(parser->token.text_pos);
        fprintf(stderr, "Expression not fully computable.\n");
        exit(1);
    }
    if(parser->pc->max.d > stmt->value.num.value.min.d) {
        warn_at_pos(parser->token.text_pos);
        fprintf(stderr, "warnpc: pc = [%06x, %06x] = [%i, %i] "
                "should be %06x = %i.\n", parser->pc->min.d, parser->pc->max.d,
                parser->pc->min.d, parser->pc->max.d,
                stmt->value.num.value.min.d, stmt->value.num.value.min.d);
        exit(1);
    }
}

static
void parse_cleartable(ParserState* parser, Statement* stmt) {
    parser_advance(parser);
    stmt->type = STATEMENT_TYPE_CLEARTABLE;
}

static
void exec_cleartable(ParserState* parser, Statement* stmt) {
    //TODO: Implement this.
}

static
void parse_fill(ParserState* parser, Statement* stmt) {
    parser_advance(parser);
    Expr expr = parse_expr(parser);
    stmt->fill.length = expr;
    stmt->type = STATEMENT_TYPE_FILL;
}

static
void exec_fill(ParserState* parser, Statement* stmt) {
    if(stmt->value.type != EXPR_NUM ||
       stmt->fill.length.num.value.min.d != stmt->fill.length.num.value.max.d) {
        error_at_pos(parser->token.text_pos);
        fprintf(stderr, "Expression not fully computable.\n");
        exit(1);
    }
    for(int i = 0; i < stmt->fill.length.num.value.min.d; i++) {
        parser_write_pc(parser, parser->fill_byte);
    }
}

void parse_base(ParserState* parser, Statement* stmt) {
    stmt->base.off = 0;
    parser_advance(parser);
    if(parser->token.type == TOKEN_IDENTIFIER && parser->token.ident_type == IDENT_OFF) {
        parser_advance(parser);
        stmt->base.off = 1;
    } else {
        Expr expr = parse_expr(parser);
        if(expr.type != EXPR_NUM) {
            error_at_pos(parser->token.text_pos);
            fprintf(stderr, "Expression not fully computable.\n");
            exit(1);
        }
        stmt->base.addr = expr;
    }
    stmt->type = STATEMENT_TYPE_BASE;
}

void exec_base(ParserState* parser, Statement* stmt) {
    //TODO: Implement this
}


static
Statement parse_statement(ParserState* parser, uint do_execute);

void parse_and_exec_if(ParserState* parser) {
    parser_advance(parser);
    Expr expr = parse_expr(parser);
    if(expr.type != EXPR_NUM || expr.num.value.min.d != expr.num.value.max.d) {
        error_at_pos(parser->token.text_pos);
        fprintf(stderr, "Expression not fully computable.\n");
        exit(1);
    }
    
    if(parser->token.type != TOKEN_CMD_DELIM) {
        error_at_pos(parser->lexer.top->text_pos);
        fprintf(stderr, "Unterminated statement\n");
        exit(1);
    }
    parser_advance(parser);
    uint execute_if_branch   = !!expr.num.value.min.d;
    uint execute_else_branch =  !expr.num.value.min.d;
    while(parser->token.type != TOKEN_IDENTIFIER ||
          (parser->token.ident_type != IDENT_ELSE && parser->token.ident_type != IDENT_ENDIF)) {
        if(parser->token.type == TOKEN_EOF) {
            error_at_pos(parser->token.text_pos);
            //TODO: Better error handling.
            //      The error location is the unclosed if branch
            fprintf(stderr, "Unclosed if branch.\n");
            exit(1);
        }
        parse_statement(parser, execute_if_branch);
    }
    
    if(parser->token.ident_type == IDENT_ELSE) {
        parser_advance(parser);
        while(parser->token.type != TOKEN_IDENTIFIER ||
              parser->token.ident_type != IDENT_ENDIF) {
            if(parser->token.type == TOKEN_EOF) {
                //TODO: Better error handling.
                //      The error location is the unclosed else branch
                error_at_pos(parser->token.text_pos);
                fprintf(stderr, "Unclosed else branch.\n");
                exit(1);
            }
            parse_statement(parser, execute_else_branch);
        }
    }
    parser_advance(parser);
}

#define UNSPECIFIED_WIDTH 10

static
void statement_wdc65816_nullary(Statement* stmt, u8 op_code) {
    stmt->wdc65816.op_code = op_code;
    stmt->wdc65816.width = 1;
}

static
void statement_wdc65816_unary(Statement* stmt, u8 op_code, Expr op, uint width) {
    stmt->wdc65816.op_code = op_code;
    stmt->wdc65816.op1 = op;
    stmt->wdc65816.width = width;
}

static
void statement_wdc65816_binary(Statement* stmt, u8 op_code, Expr op1, Expr op2) {
    stmt->wdc65816.op_code = op_code;
    stmt->wdc65816.op1 = op1;
    stmt->wdc65816.op2 = op2;
    stmt->wdc65816.width = 3;
}

typedef struct {
    u8 from_3_to_2 : 1;
    u8 from_3_to_1 : 1;
} OpcodeCast;

static const
OpcodeCast opcode_silence_cast_table[IDENT_OPCODE_MAX] = {
    [IDENT_ADC] = { .from_3_to_2 = 1 },
    [IDENT_AND] = { .from_3_to_2 = 1 },
    [IDENT_BCS] = { .from_3_to_1 = 1 },
    [IDENT_BCC] = { .from_3_to_1 = 1 },
    [IDENT_BEQ] = { .from_3_to_1 = 1 },
    [IDENT_BIT] = { .from_3_to_2 = 1 },
    [IDENT_BRA] = { .from_3_to_1 = 1 },
    [IDENT_BRL] = { .from_3_to_2 = 1 },
    [IDENT_BPL] = { .from_3_to_1 = 1 },
    [IDENT_BMI] = { .from_3_to_1 = 1 },
    [IDENT_BNE] = { .from_3_to_1 = 1 },
    [IDENT_BVC] = { .from_3_to_1 = 1 },
    [IDENT_BVS] = { .from_3_to_1 = 1 },
    [IDENT_CMP] = { .from_3_to_2 = 1 },
    [IDENT_EOR] = { .from_3_to_2 = 1 },
    [IDENT_LDA] = { .from_3_to_2 = 1 },
    [IDENT_LDX] = { .from_3_to_2 = 1 },
    [IDENT_LDY] = { .from_3_to_2 = 1 },
    [IDENT_JMP] = { .from_3_to_2 = 1 },
    [IDENT_JSR] = { .from_3_to_2 = 1 },
    [IDENT_ORA] = { .from_3_to_2 = 1 },
    [IDENT_SBC] = { .from_3_to_2 = 1 },
};


static
void parse_wdc65816_one_operand(Statement* stmt, Token op_code, OpcodeParseMode parse_mode,
                           Expr op, uint width_specifier) {
    int op_type = op_code.ident_type;
    OpcodeMode mode1 = parse_mode_table[parse_mode][0];
    OpcodeMode mode2 = parse_mode_table[parse_mode][1];
    OpcodeMode mode3 = parse_mode_table[parse_mode][2];
    i16 op_mnemo8  = mode1 == MODE_UNDEFINED ? -1 : opcode_addr_mode[op_type][mode1];
    i16 op_mnemo16 = mode2 == MODE_UNDEFINED ? -1 : opcode_addr_mode[op_type][mode2];
    i16 op_mnemo24 = mode3 == MODE_UNDEFINED ? -1 : opcode_addr_mode[op_type][mode3];
    uint width = op.num.width;
    OpcodeCast cast = opcode_silence_cast_table[op_type];
    if(op_mnemo8 == -1 && op_mnemo16 == -1 && op_mnemo24 == -1) {
        error_at_pos(op_code.text_pos);
        fprintf(stderr, parse_mode_patterns[mode1], op_code.text.length, op_code.text.data);
        fprintf(stderr, " is invalid.\n");
        exit(1);
    } else if(width_specifier != UNSPECIFIED_WIDTH) {
        if(width_specifier == 1 && op_mnemo8 != -1) {
            statement_wdc65816_unary(stmt, op_mnemo8, op, 1);
        } else if(width_specifier == 2 && op_mnemo16 != -1) {
            statement_wdc65816_unary(stmt, op_mnemo8, op, 2);
        } else if(width_specifier == 3 && op_mnemo24 != -1) {
            statement_wdc65816_unary(stmt, op_mnemo8, op, 3);
        } else {
            OpcodeMode mode_width = parse_mode_table[parse_mode][width_specifier - 1];
            error_at_pos(op_code.text_pos);
            fprintf(stderr, parse_mode_patterns[mode_width], op_code.text.length, op_code.text.data);
            fprintf(stderr, " is invalid.\n");
            exit(1);
        }
    } else if(width == 1) {
        if(op_mnemo8 != -1) {
            statement_wdc65816_unary(stmt, op_mnemo8, op, 1);
        } else if(op_mnemo16 != -1) {
            warn_at_pos(op_code.text_pos);
            fprintf(stderr, "Using one byte operand, which is not valid for opcode \"%.*s\" in $xx mode.\n"
                    "Upcasting to two bytes instead.\n", op_code.text.length, op_code.text.data);
            statement_wdc65816_unary(stmt, op_mnemo16, op, 2);
        } else if(op_mnemo16 != -1) {
            warn_at_pos(op_code.text_pos);
            fprintf(stderr, "Using one byte operand, which is not valid for opcode \"%.*s\" in $xx mode.\n"
                    "Upcasting to three bytes instead.\n", op_code.text.length, op_code.text.data);
            statement_wdc65816_unary(stmt, op_mnemo24, op, 3);
        } else {
            invalid_code_path;
        }
    } else if(width == 2) {
        if(op_mnemo16 != -1) {
            statement_wdc65816_unary(stmt, op_mnemo16, op, 2);
        } else if(op_mnemo24 != -1) {
            warn_at_pos(op_code.text_pos);
            fprintf(stderr, "Using two byte operand, which is not valid for opcode \"%.*s\" in $xx mode.\n"
                    "Upcasting to three bytes instead.\n", op_code.text.length, op_code.text.data);
            statement_wdc65816_unary(stmt, op_mnemo24, op, 3);
        } else if(op_mnemo8 != -1) {
            warn_at_pos(op_code.text_pos);
            fprintf(stderr, "Using two byte operand, which is not valid for opcode \"%.*s\" in $xx mode.\n"
                    "Downcasting to one bytes instead.\n", op_code.text.length, op_code.text.data);
            statement_wdc65816_unary(stmt, op_mnemo8, op, 1);
        } else {
            invalid_code_path;
        }
    } else if(width == 3) {
        if(op_mnemo24 != -1) {
            statement_wdc65816_unary(stmt, op_mnemo24, op, 3);
        } else if(op_mnemo16 != -1) {
            if(!cast.from_3_to_2) {
                warn_at_pos(op_code.text_pos);
                fprintf(stderr, "Using three byte operand, which is not valid for opcode \"%.*s\".\n"
                        "Downcasting to two bytes instead.\n", op_code.text.length, op_code.text.data);
            }
            statement_wdc65816_unary(stmt, op_mnemo16, op, 2);
        } else if(op_mnemo8 != -1) {
            if(!cast.from_3_to_1) {
                warn_at_pos(op_code.text_pos);
                fprintf(stderr, "Using three byte operand, which is not valid for opcode \"%.*s\".\n"
                        "Downcasting to one byte instead.\n", op_code.text.length, op_code.text.data);
            }
            statement_wdc65816_unary(stmt, op_mnemo8, op, 1);
        } else {
            invalid_code_path;
        }
    }
    stmt->type = STATEMENT_TYPE_W65816;
}


static
void parse_wdc65816_two_operands(Statement* stmt, Token op_code, Expr op1, Expr op2, uint width_specifier) {
    u8 op_byte = op_code.ident_type;
    i16 op_mnemo8 = opcode_addr_mode[op_byte][MODE_SRC_DEST];
    if(op_mnemo8 == -1) {
        error_at_pos(op_code.text_pos);
        fprintf(stderr, "Source-Destination mode invalid for opcode %.*s\n", op_code.text.length, op_code.text.data);
        exit(1);
    }
    if(width_specifier != UNSPECIFIED_WIDTH && width_specifier != 1) {
        warn_at_pos(op_code.text_pos);
        fprintf(stderr, "\"mvn\" and \"mvp\" need width specifier \".b\", or no width specifier.\n\n");
    }
    u32 width1 = op1.num.width; 
    u32 width2 = op2.num.width; 

    if(width_specifier != 1 && (width1 != 1 || width2 != 1)) {
        warn_at_pos(op_code.text_pos);
        fprintf(stderr, "Operands of \"%.*s\" are not of one byte length.\n"
                "Downcasting to one byte instead.\n\n", op_code.text.length, op_code.text.data);
    }

    statement_wdc65816_binary(stmt, op_mnemo8, op1, op2);
}

static
void parse_wdc65816_implied_mode(Statement* stmt, Token opcode, ushort width_specifier) {
    u8 op_byte = opcode.ident_type;
    i16 op_mnemo8 = opcode_addr_mode[op_byte][MODE_IMPLIED];
    if(op_mnemo8 == -1) {
        error_at_pos(opcode.text_pos);
        fprintf(stderr, "\"%.*s\" is not a valid command.\n", opcode.text.length, opcode.text.data);
        exit(1);
    }
    if(width_specifier != UNSPECIFIED_WIDTH) {
        warn_at_pos(opcode.text_pos);
        fprintf(stderr, "Width specifier meaningless for implied mode in opcode \"%.*s.\n",
                opcode.text.length, opcode.text.data);
    }

    statement_wdc65816_nullary(stmt, op_mnemo8);
}

static
void parse_wdc65816_accumulator_mode(Statement* stmt, Token opcode, ushort width_specifier) {
    u8 op_byte = opcode.ident_type;
    i16 op_mnemo8 = opcode_addr_mode[op_byte][MODE_ACCUMULATOR];
    if(op_mnemo8 == -1) {
        error_at_pos(opcode.text_pos);
        fprintf(stderr, "\"%.*s A\" is not a valid command.\n", opcode.text.length, opcode.text.data);
        exit(1);
    }
    if(width_specifier != UNSPECIFIED_WIDTH) {
        warn_at_pos(opcode.text_pos);
        fprintf(stderr, "Width specifier meaningless for \"%.*s A.\n",
                opcode.text.length, opcode.text.data);
    }

    statement_wdc65816_nullary(stmt, op_mnemo8);
}

static
void exec_w65816(ParserState* parser, Statement* stmt) {
    /* switch(stmt->op.width) { */
    /* case 1: */
    /*     parser_write_pc(parser, stmt->op.op_code); */
    /*     break; */
    /* case 2: */
    /*     parser_write_pc(parser, stmt->op.op_code); */
    /*     parser_write_pc(parser, stmt->op.op1); */
    /*     break; */
    /* case 3:  */
    /*     parser_write_pc(parser, stmt->op.op_code); */
    /*     parser_write_pc(parser, stmt->op.op1); */
    /*     parser_write_pc(parser, stmt->op.op2); */
    /*     break; */
    /* case 4: */
    /*     parser_write_pc(parser, stmt->op.op_code); */
    /*     parser_write_pc(parser, stmt->op.op1); */
    /*     parser_write_pc(parser, stmt->op.op2); */
    /*     parser_write_pc(parser, stmt->op.op3); */
    /*     break; */
    /* invalid_default_case; */
    /* } */
}


void exec_statement(ParserState* parser, Statement* stmt) {
    switch(stmt->type) {
    case STATEMENT_TYPE_NULL:
        break;
    case STATEMENT_TYPE_INCBIN:
        exec_incbin(parser, stmt);
        break;
    case STATEMENT_TYPE_TABLE:
        exec_table(parser, stmt); break;
    case STATEMENT_TYPE_CLEARTABLE:
        exec_cleartable(parser, stmt); break;
    case STATEMENT_TYPE_FILLBYTE:
        exec_fillbyte(parser, stmt); break;
    case STATEMENT_TYPE_DB:
        exec_db(parser, stmt); break;
    case STATEMENT_TYPE_ORG:
        exec_org(parser, stmt); break;
    case STATEMENT_TYPE_WARNPC:
        exec_warnpc(parser, stmt); break;
    case STATEMENT_TYPE_FILL:
        exec_fill(parser, stmt); break;
    case STATEMENT_TYPE_BASE:
        exec_base(parser, stmt); break;
    case STATEMENT_TYPE_W65816:
        exec_w65816(parser, stmt); break;
    }
}

static
Statement parse_statement(ParserState* parser, uint do_execute) {
    Statement stmt = { 0 };
    switch(parser->token.type) {
    case TOKEN_IDENTIFIER: {
        if(parser->token.ident_type & IDENT_COMMAND) {
            switch(parser->token.ident_type) {

            case IDENT_INCBIN:     { parse_incbin    (parser, &stmt); break; }
            case IDENT_TABLE:      { parse_table     (parser, &stmt); break; }
            case IDENT_WARNPC:     { parse_warnpc    (parser, &stmt); break; }
            case IDENT_ORG:        { parse_org       (parser, &stmt); break; }
            case IDENT_FILL:       { parse_fill      (parser, &stmt); break; }
            case IDENT_FILLBYTE:   { parse_fillbyte  (parser, &stmt); break; }
            case IDENT_BASE:       { parse_base      (parser, &stmt); break; }
            case IDENT_CLEARTABLE: { parse_cleartable(parser, &stmt); break; }

            case IDENT_DB: { parse_db(parser, 0, &stmt); break; }
            case IDENT_DW: { parse_db(parser, 1, &stmt); break; }
            case IDENT_DL: { parse_db(parser, 2, &stmt); break; }
            case IDENT_DD: { parse_db(parser, 3, &stmt); break; }

            case IDENT_INCSRC: {
                parse_and_exec_incsrc(parser);
                return;
            }
            case IDENT_IF:     { parse_and_exec_if    (parser); break; }
            case IDENT_ELSE: {
                error_at_pos(parser->token.text_pos);
                fprintf(stderr, "Else without if.\n");
                exit(1);
            }
            case IDENT_ENDIF: {
                error_at_pos(parser->token.text_pos);
                fprintf(stderr, "Endif without if.\n");
                exit(1);
            }

            invalid_default_case;
            }
        } else {
            Token opcode = parser->token;
            parser_advance(parser);
            ushort width_specifier = UNSPECIFIED_WIDTH;

            if(parser->token.type == TOKEN_DOT) {
                parser_advance(parser);
                parser_expect(parser, TOKEN_IDENTIFIER);
                if(parser->token.ident_type == IDENT_B) {
                    width_specifier = 1;
                } else if(parser->token.ident_type == IDENT_W) {
                    width_specifier = 2;
                } else if(parser->token.ident_type == IDENT_L) {
                    width_specifier = 3;
                } else {
                    error_at_pos(parser->token.text_pos);
                    fprintf(stderr, "Expected \"b\", \"w\" or \"l\",\n");
                    exit(1);
                }
                parser_advance(parser);
            }

            if(parser->token.type == TOKEN_CMD_DELIM) {
                parse_wdc65816_implied_mode(stmt, opcode, width_specifier);
            } else if(parser->token.type == TOKEN_IDENTIFIER && parser->token.ident_type == IDENT_A) {
                parser_advance(parser);
                parse_wdc65816_accumulator_mode(stmt, opcode, width_specifier);
            } else if(parser->token.type ==  TOKEN_HASH) {
                parser_advance(parser);
                Expr expr = parse_expr(parser);
                parser_expect_expr(parser, expr);
                parse_wdc65816_one_operand(stmt, opcode, PARSE_MODE_IMMEDIATE, expr, width_specifier);
            } else if(parser->token.type == TOKEN_LPAREN) {
                parser_advance(parser);
                Expr expr = parse_expr(parser);
                parser_expect_expr(parser, expr);
                if(parser->token.type == TOKEN_COMMA) {
                    parser_advance(parser);
                    if(parser->token.type == TOKEN_IDENTIFIER && parser->token.ident_type == IDENT_X) {
                        parser_advance(parser);
                        parser_expect(parser, TOKEN_RPAREN);
                        parse_wdc65816_one_operand(stmt, opcode, PARSE_MODE_X_INDIRECT, expr, width_specifier);
                    } else if(parser->token.type == TOKEN_IDENTIFIER && parser->token.ident_type == IDENT_S) {
                        parser_advance(parser);
                        parser_expect(parser, TOKEN_RPAREN);
                        parser_advance(parser);
                        parser_expect(parser, TOKEN_COMMA);
                        parser_advance(parser);
                        if(parser->token.type != TOKEN_IDENTIFIER || parser->token.ident_type != IDENT_Y) {
                            error_at_pos(parser->token.text_pos);
                            fprintf(stderr, "Expected \"y\".\n");
                            exit(1);
                        }
                        parse_wdc65816_one_operand(stmt, opcode, PARSE_MODE_S_INDIRECT_Y, expr, width_specifier);
                    } else {
                        error_at_pos(parser->token.text_pos);
                        fprintf(stderr, "Expected \"x\" or \"s\".\n");
                        exit(1);
                    }
                    parser_advance(parser);

                } else if(parser->token.type == TOKEN_RPAREN) {
                    parser_advance(parser);
                    if(parser->token.type == TOKEN_CMD_DELIM) {
                        parse_wdc65816_one_operand(stmt, opcode, PARSE_MODE_INDIRECT, expr, width_specifier);
                    } else if(parser->token.type == TOKEN_COMMA) {
                        parser_advance(parser);
                        if(parser->token.type != TOKEN_IDENTIFIER || parser->token.ident_type != IDENT_Y) {
                            error_at_pos(parser->token.text_pos);
                            fprintf(stderr, "Expected \"x\".\n");
                            exit(1);
                        }
                        parser_advance(parser);
                        parse_wdc65816_one_operand(stmt, opcode, PARSE_MODE_INDIRECT_Y, expr, width_specifier);
                    }
                } else {
                    error_at_pos(parser->token.text_pos);
                    fprintf(stderr, "Expected ',' or ')'.\n");
                    exit(1);
                }
            } else if(parser->token.type == TOKEN_LBRACKET) {
                parser_advance(parser);
                Expr expr = parse_expr(parser);
                parser_expect(parser, TOKEN_RBRACKET);
                parser_advance(parser);
                if(parser->token.type == TOKEN_COMMA) {
                    parser_advance(parser);
                    if(parser->token.type != TOKEN_IDENTIFIER || parser->token.ident_type != IDENT_Y) {
                        error_at_pos(parser->token.text_pos);
                        fprintf(stderr, "Expected \"y\".\n");
                        exit(1);
                    }
                    parser_advance(parser);
                    parse_wdc65816_one_operand(stmt, opcode, PARSE_MODE_LONG_INDIRECT_Y, expr, width_specifier);
                } else if(parser->token.type == TOKEN_CMD_DELIM) {
                    parse_wdc65816_one_operand(stmt, opcode, PARSE_MODE_LONG_INDIRECT, expr, width_specifier);
                } else {
                    error_at_pos(parser->token.text_pos);
                    fprintf(stderr, "Expected ',' or ')'.\n");
                    exit(1);
                }
            } else {
                Expr expr = parse_expr(parser);
                if(expr.type != EXPR_NULL) {
                    if(parser->token.type == TOKEN_COMMA) {
                        parser_advance(parser);
                        if(parser->token.type == TOKEN_IDENTIFIER && parser->token.ident_type == IDENT_X) {
                            parser_advance(parser);
                            parse_wdc65816_one_operand(stmt, opcode, PARSE_MODE_DIRECT_X, expr, width_specifier);
                        } else if(parser->token.type == TOKEN_IDENTIFIER && parser->token.ident_type == IDENT_Y) {
                            parser_advance(parser);
                            parse_wdc65816_one_operand(stmt, opcode, PARSE_MODE_DIRECT_Y, expr, width_specifier);
                        } else if(parser->token.type == TOKEN_IDENTIFIER && parser->token.ident_type == IDENT_S) {
                            parser_advance(parser);
                            parse_wdc65816_one_operand(stmt, opcode, PARSE_MODE_DIRECT_S, expr, width_specifier);
                        } else {
                            Expr expr2 = parse_expr(parser);
                            parser_expect_expr(parser, expr2);
                            parse_wdc65816_two_operands(stmt, opcode, expr, expr2, width_specifier);
                        }
                    } else if(parser->token.type == TOKEN_CMD_DELIM) {
                        parse_wdc65816_one_operand(stmt, opcode, PARSE_MODE_DIRECT, expr, width_specifier);
                    } else {
                        error_at_pos(parser->token.text_pos);
                        fprintf(stderr, "Unexpected token.\n");
                        exit(1);
                    }
                } else {
                    error_at_pos(parser->token.text_pos);
                    fprintf(stderr, "Unexpected token.\n");
                    exit(1);
                }
            }
        }
        break;
    }

    case TOKEN_DEFINE: {
        Token token = parser->token;
        // We do not want that the lexer state changes during definition of a
        // define.
        LexerState* lexer_state = parser->lexer.top;
        parser_advance(parser);
        if(parser->token.type == TOKEN_EQUAL) {
            if(parser->lexer.top != lexer_state) {
                error_at_pos(parser->token.text_pos);
                fprintf(stderr, "Expected a definition\n");
                exit(1);
            }
            char* c = lexer_state->pos; 
            while((void*)c < lexer_state->buffer.end && c[0] != '\n' &&
                  !(c[0] == '\r' && (void*)c + 1 < lexer_state->buffer.end && c[1] == '\n')) {
                c++;
            }
            if(do_execute) {
                Define define = {
                    .name         = token.text,
                    .buffer.begin = lexer_state->pos,
                    .buffer.end   = c,
#ifdef TEXT_POS
                    .text_pos     = token.text_pos,
#endif
                };
                define_map_insert(&parser->lexer.define_map, define);
            }
            parser->lexer.pos = lexer_state->pos = c;
            parser_advance(parser);
        } else {
            error_at_pos(token.text_pos);
            fprintf(stderr, "Expected '=' for undefined !%.*s\n", token.text.length, token.text.data);
            exit(1);
        }
        break;
    }
        
    case TOKEN_LABEL_DEF:
    case TOKEN_PLUS:
    case TOKEN_MINUS:
    case TOKEN_ANON_LABEL:
        // TODO: Implement this.
        parser_advance(parser);
        break;
    case TOKEN_CMD_DELIM:
        break;
        
    default: {
        error_at_pos(parser->token.text_pos);
        fprintf(stderr, "Unexpected token \"%s\".\n", token_names[parser->token.type]);
        exit(1);
    }
    }
    
    if(parser->token.type != TOKEN_CMD_DELIM && parser->token.type != TOKEN_EOF) {
        error_at_pos(parser->lexer.top->text_pos);
        fprintf(stderr, "Unterminated statement\n");
        exit(1);
    }
    parser_advance(parser);

    //print_statement(stmt);
    if(do_execute) {
        exec_statement(parser, stmt);
    }
}

StatementBlock parse_asm(ParserState* parser) {
    parser_advance(parser);
    while(parser->token.type != TOKEN_EOF) {
        parse_statement(parser, /* do_execute = */ 1);
    }
}

void parser_global_init() {
    identifier_map_fill();
}
void parser_global_deinit() {
    identifier_map_deinit(&identifier_map);
}
