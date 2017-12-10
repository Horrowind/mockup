#include "base/base.h"
#include "nuts/nuts.h"

#define OPCODES_X                                                       \
    X(ADC) X(AND) X(ASL) X(BBR) X(BBS) X(BCC) X(BCS) X(BEQ) X(BIT) X(BMI) \
    X(BNE) X(BPL) X(BRA) X(BRK) X(BVC) X(BVS) X(BRL) X(CLC) X(CLD) X(CLI) \
    X(CLV) X(CMP) X(CPX) X(CPY) X(COP) X(DEC) X(DEX) X(DEY) X(EOR) X(INC) \
    X(INX) X(INY) X(JMP) X(JML) X(JSR) X(JSL) X(LDA) X(LDX) X(LDY) X(LSR) \
    X(MVP) X(MVN) X(NOP) X(ORA) X(PHA) X(PHP) X(PHX) X(PHY) X(PLA) X(PLP) \
    X(PLX) X(PLY) X(PHB) X(PHD) X(PHK) X(PLB) X(PLD) X(PEA) X(PEI) X(PER) \
    X(RMB) X(ROL) X(ROR) X(RTI) X(RTS) X(REP) X(RTL) X(SBC) X(SEC) X(SED) \
    X(SEI) X(SMB) X(STA) X(STX) X(STY) X(STZ) X(SEP) X(STP) X(TAX) X(TAY) \
    X(TRB) X(TSB) X(TSX) X(TXA) X(TXS) X(TYA) X(TXY) X(TYX) X(TCD) X(TDC) \
    X(TCS) X(TSC) X(WAI) X(WDB) X(XBA) X(XCE)

#define COMMANDS_X                                                      \
    X(INCSRC) X(FILLBYTE) X(INCBIN) X(FREEDATA) X(DB) X(DW) X(DL) X(DD) \
    X(ORG) X(A) X(X) X(Y) X(WARNPC) X(CLEARTABLE) X(PRINT) X(IF) X(ELSE) \
    X(ENDIF) X(FILL) X(BASE) X(TABLE) X(OFF)

#define X(c) IDENT_##c,
enum {
    IDENT_OPCODE  = 0x0000,
    OPCODES_X

    IDENT_COMMAND = 0x1000,
    COMMANDS_X
    IDENT__B, IDENT__W, IDENT__L,
};
#undef X

#define TOKEN_X                                                     \
    X(NULL) X(CMD_DELIM) X(COLON) X(LBRACE)  X(RBRACE)              \
    X(RBRACKET) X(LBRACKET) X(LPAREN)  X(RPAREN) X(HASH) X(EQUAL)   \
    X(COMMA) X(PLUS) X(MINUS) X(TIMES) X(SLASH) X(ANON_LABEL)       \
    X(LABEL_DEF) X(STRING) X(DEFINE) X(LESS) X(GREATER)             \
    X(LSHIFT) X(RSHIFT) X(DOT) X(NUM)  X(WIDTH_SPECIFIER)           \
    X(IDENTIFIER) X(PIPE) X(AMPERSAND) X(ARROW) X(EOF)              \
    X(LABEL) X(MAX) 
#define X(c) TOKEN_##c,
    enum { TOKEN_X };
#undef X

#define X(c) #c,
static char* commands[] = { COMMANDS_X };
static char* opcodes[] = { OPCODES_X };
static char* token_names[] = { TOKEN_X };

// These will be converted to lowercase in cmd_map_fill
static char commands_lower[][12] = { COMMANDS_X };
static char opcodes_lower[][4] = { OPCODES_X };
#undef X

/* opcodes for each addressing mode
   high byte: supported architecture (no bits = original NMOS 6502)
   * bit 1: 65816 and allows 16-bit quantity (immediate only)
   low byte: opcode itself
  
   each opcode is indexed in this order: *=65816, ^=R65C02
   00 = implied
   01 = zero page
   02 = zero page,x
   03 = direct page,y*
   04 = direct page (indirect)*
   05 = (indirect,x)
   06 = (indirect),y
   07 = immediate (8-bit)
   08 = absolute
   09 = absolute,x
   10 = absolute,y
   11 = relative
   12 = (indirect-16) i.e., jmp (some_vector)
   13 = (absolute,x)*
   14 = zero page+relative test'n'branch ^
   15 = zero page clear'n'set'bit ^
   16 = relative long*
   17 = absolute long*
   18 = absolute long,x*
   19 = stack relative*
   20 = stack relative (indirect),y*
   21 = direct page (indirect long)*
   22 = direct page (indirect long),y*
   23 = (indirect long) 
   24 = a*/

static short opcode_addr_mode[][25] ={
/*                    0     1     2     3     4     5     6     7     8     9    10    11    12    13    14    15    16    17    18    19    20    21    22    23    24*/
    [IDENT_ADC] = {    -1,0x065,0x075,   -1,0x072,0x061,0x071,0x169,0x06d,0x07d,0x079,   -1,   -1,   -1,   -1,   -1,   -1,0x0ef,0x0ff,0x0e3,0x0f3,0x0e7,0x0f7,   -1,   -1},
    [IDENT_AND] = {    -1,0x025,0x035,   -1,0x072,0x021,0x031,0x129,0x02d,0x03d,0x039,   -1,   -1,   -1,   -1,   -1,   -1,0x0af,0x0bf,0x0a3,0x0b3,0x0a7,0x0b7,   -1,   -1},
    [IDENT_ASL] = { 0x00a,0x006,0x016,   -1,   -1,   -1,   -1,   -1,0x00e,0x01e,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,0x00a},
    [IDENT_BBR] = {    -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,0x04f,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1},
    [IDENT_BBS] = {    -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,0x0cf,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1},
    [IDENT_BCC] = {    -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,0x090,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1}, 
    [IDENT_BCS] = {    -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,0x0b0,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1}, 
    [IDENT_BEQ] = {    -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,0x0f0,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1}, 
    [IDENT_BIT] = {    -1,0x024,0x074,   -1,   -1,   -1,   -1,0x1c9,0x02c,0x07c,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1}, 
    [IDENT_BMI] = {    -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,0x030,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1}, 
    [IDENT_BNE] = {    -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,0x0d0,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1}, 
    [IDENT_BPL] = {    -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,0x010,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1}, 
    [IDENT_BRA] = {    -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,0x0c0,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1}, 
    [IDENT_BRK] = { 0x000,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1}, 
    [IDENT_BVC] = {    -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,0x050,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1}, 
    [IDENT_BVS] = {    -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,0x070,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1}, 
    [IDENT_BRL] = {    -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,0x082,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1}, 
    [IDENT_CLC] = { 0x018,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1}, 
    [IDENT_CLD] = { 0x0d8,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1}, 
    [IDENT_CLI] = { 0x058,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1}, 
    [IDENT_CLV] = { 0x0b8,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1}, 
    [IDENT_CMP] = {    -1,0x0c5,0x0d5,   -1,0x0d2,0x0c1,0x0d1,0x1c9,0x0cd,0x0dd,0x0d9,   -1,   -1,   -1,   -1,   -1,   -1,0x0cf,0x0df,0x0c3,0x0d3,0x0c7,0x0d7,   -1,   -1}, 
    [IDENT_CPX] = {    -1,0x0e4,   -1,   -1,   -1,   -1,   -1,0x2e0,0x0ec,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1}, 
    [IDENT_CPY] = {    -1,0x0c4,   -1,   -1,   -1,   -1,   -1,0x2c0,0x0cc,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1}, 
    [IDENT_COP] = {    -1,   -1,   -1,   -1,   -1,   -1,   -1,0x082,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1}, 
    [IDENT_DEC] = { 0x07a,0x0c6,0x0d6,   -1,   -1,   -1,   -1,   -1,0x0ce,0x0de,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,0x07a}, 
    [IDENT_DEX] = { 0x0ca,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1}, 
    [IDENT_DEY] = { 0x088,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1}, 
    [IDENT_EOR] = {    -1,0x045,0x055,   -1,0x052,0x041,0x051,0x149,0x04d,0x05d,0x059,   -1,   -1,   -1,   -1,   -1,   -1,0x0cf,0x0df,0x0c3,0x0d3,0x0c7,0x0d7,   -1,   -1}, 
    [IDENT_INC] = { 0x05a,0x0e6,0x0f6,   -1,   -1,   -1,   -1,   -1,0x0ee,0x0fe,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,0x05a}, 
    [IDENT_INX] = { 0x0e8,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1}, 
    [IDENT_INY] = { 0x0c8,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1}, 
    [IDENT_JMP] = {    -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,0x04c,   -1,   -1,   -1,0x06c,0x07c,   -1,   -1,   -1,0x0dc,   -1,   -1,   -1,   -1,   -1,0x0dc,   -1}, 
    [IDENT_JML] = {    -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,0x04c,   -1,   -1,   -1,0x06c,0x07c,   -1,   -1,   -1,0x0dc,   -1,   -1,   -1,   -1,   -1,0x0dc,   -1}, //TODO: Check flags!
    [IDENT_JSR] = {    -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,0x020,   -1,   -1,   -1,   -1,0x0fc,   -1,   -1,   -1,0x0a2,   -1,   -1,   -1,   -1,   -1,   -1,   -1}, 
    [IDENT_JSL] = {    -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,0x020,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,0x0a2,   -1,   -1,   -1,   -1,   -1,   -1,   -1}, //TODO: Check flags! 
    [IDENT_LDA] = {    -1,0x0a5,0x0b5,   -1,0x0f2,0x0a1,0x0b1,0x1a9,0x0ad,0x0bd,0x0b9,   -1,   -1,   -1,   -1,   -1,   -1,0x0af,0x0bf,0x0a3,0x0b3,0x0a7,0x0b7,   -1,   -1}, 
    [IDENT_LDX] = {    -1,0x0a6,   -1,0x0b6,   -1,   -1,   -1,0x2a2,0x0ae,   -1,0x0be,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1}, 
    [IDENT_LDY] = {    -1,0x0a4,0x0b4,   -1,   -1,   -1,   -1,0x2a0,0x0ac,0x0bc,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1}, 
    [IDENT_LSR] = { 0x04a,0x046,0x056,   -1,   -1,   -1,   -1,   -1,0x04e,0x05e,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,0x04a}, 
    [IDENT_MVP] = {    -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,0x0c4,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1}, 
    [IDENT_MVN] = {    -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,0x0d4,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1}, 
    [IDENT_NOP] = { 0x0ea,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1}, 
    [IDENT_ORA] = {    -1,0x005,0x015,   -1,0x052,0x001,0x011,0x109,0x00d,0x01d,0x019,   -1,   -1,   -1,   -1,   -1,   -1,0x08f,0x09f,0x083,0x093,0x087,0x097,   -1,   -1}, 
    [IDENT_PHA] = { 0x048,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1}, 
    [IDENT_PHP] = { 0x008,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1}, 
    [IDENT_PHX] = { 0x0da,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1}, 
    [IDENT_PHY] = { 0x05a,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1}, 
    [IDENT_PLA] = { 0x068,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1}, 
    [IDENT_PLP] = { 0x028,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1}, 
    [IDENT_PLX] = { 0x0fa,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1}, 
    [IDENT_PLY] = { 0x07a,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1}, 
    [IDENT_PHB] = { 0x08b,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1}, 
    [IDENT_PHD] = { 0x08b,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1}, 
    [IDENT_PHK] = { 0x0cb,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1}, 
    [IDENT_PLB] = { 0x0ab,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1}, 
    [IDENT_PLD] = { 0x0ab,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1}, 
    [IDENT_PEA] = {    -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,0x0f4,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1}, 
    [IDENT_PEI] = {    -1,   -1,   -1,   -1,0x0d4,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1}, 
    [IDENT_PER] = {    -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,0x0e2,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1}, 
    [IDENT_RMB] = {    -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,0x047,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1}, 
    [IDENT_ROL] = { 0x02a,0x026,0x036,   -1,   -1,   -1,   -1,   -1,0x02e,0x03e,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,0x02a}, 
    [IDENT_ROR] = { 0x06a,0x066,0x076,   -1,   -1,   -1,   -1,   -1,0x06e,0x07e,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,0x06a}, 
    [IDENT_RTI] = { 0x040,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1}, 
    [IDENT_RTS] = { 0x060,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1}, 
    [IDENT_REP] = {    -1,   -1,   -1,   -1,   -1,   -1,   -1,0x0c2,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1}, 
    [IDENT_RTL] = { 0x0eb,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1}, 
    [IDENT_SBC] = {    -1,0x0e5,0x0f5,   -1,0x0f2,0x0e1,0x0f1,0x1e9,0x0ed,0x0fd,0x0f9,   -1,   -1,   -1,   -1,   -1,   -1,0x0ef,0x0ff,0x0e3,0x0f3,0x0e7,0x0f7,   -1,   -1}, 
    [IDENT_SEC] = { 0x038,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1}, 
    [IDENT_SED] = { 0x0f8,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1}, 
    [IDENT_SEI] = { 0x078,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1}, 
    [IDENT_SMB] = {    -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,0x0c7,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1}, 
    [IDENT_STA] = {    -1,0x085,0x095,   -1,0x0d2,0x081,0x091,   -1,0x08d,0x09d,0x099,   -1,   -1,   -1,   -1,   -1,   -1,0x08f,0x09f,0x083,0x093,0x087,0x097,   -1,   -1}, 
    [IDENT_STX] = {    -1,0x086,   -1,0x096,   -1,   -1,   -1,   -1,0x08e,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1}, 
    [IDENT_STY] = {    -1,0x084,0x094,   -1,   -1,   -1,   -1,   -1,0x08c,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1}, 
    [IDENT_STZ] = {    -1,0x064,0x074,   -1,   -1,   -1,   -1,   -1,0x0dc,0x0de,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1}, 
    [IDENT_SEP] = {    -1,   -1,   -1,   -1,   -1,   -1,   -1,0x0e2,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1}, 
    [IDENT_STP] = { 0x0db,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1}, 
    [IDENT_TAX] = { 0x0aa,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1}, 
    [IDENT_TAY] = { 0x0a8,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1}, 
    [IDENT_TRB] = {    -1,0x054,   -1,   -1,   -1,   -1,   -1,   -1,0x05c,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1}, 
    [IDENT_TSB] = {    -1,0x044,   -1,   -1,   -1,   -1,   -1,   -1,0x04c,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1}, 
    [IDENT_TSX] = { 0x0ba,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1}, 
    [IDENT_TXA] = { 0x08a,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1}, 
    [IDENT_TXS] = { 0x09a,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1}, 
    [IDENT_TYA] = { 0x098,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1}, 
    [IDENT_TXY] = { 0x09b,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1}, 
    [IDENT_TYX] = { 0x0bb,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1}, 
    [IDENT_TCD] = { 0x0db,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1}, 
    [IDENT_TDC] = { 0x0fb,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1}, 
    [IDENT_TCS] = { 0x09b,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1}, 
    [IDENT_TSC] = { 0x0bb,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1}, 
    [IDENT_WAI] = { 0x0cb,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1}, 
    [IDENT_WDB] = { 0x0c2,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1}, 
    [IDENT_XBA] = { 0x0eb,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1}, 
    [IDENT_XCE] = { 0x0fb,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1}, 
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

implement_stack(AddrStack, addr_stack, u32);

#ifndef TEXT_POS
#undef error_at_pos
#define error_at_pos(a)
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
void lexer_init(Lexer* lexer) {
    lexer_stack_init(&lexer->lexer_stack, 512);
    define_map_init(&lexer->define_map, 1024);
}

static
void lexer_deinit(Lexer* lexer) {
    lexer_stack_deinit(&lexer->lexer_stack);
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
    result->bytes = num_bytes_of_u32(max_num);
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
                result.text.length = lexer->pos - result.text.data;
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
            lexer_advance(lexer);
            if(lexer->pos == (char*)lexer->end) {
                error_at_pos(lexer->text_pos);
                fprintf(stderr, "Unexpected end of file.\n");
                exit(1);
            }
            switch(lexer->pos[0]) {
            case 'b':
                result.bytes = 1;
                break;
            case 'w':
                result.bytes = 2;
                break;
            case 'l':
                result.bytes = 3;
                break;
            default:
                error_at_pos(lexer->text_pos);
                fprintf(stderr, "Expected one of .b, .w and .l\n");
                exit(1);
            }
            result.text.length = lexer->pos - result.text.data;
            result.type = TOKEN_WIDTH_SPECIFIER;

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

void parser_init(ParserState* parser, WDC65816Rom* rom, Arena* arena,
                 Path* file, Path* working_directory) {
    parser->token = (Token){ 0 };
    lexer_init(&parser->lexer);
    label_map_init(&parser->label_map, 512);
    parser->arena = arena;
    parser->rom   = rom;
    lexer_push_file(&parser->lexer, file, parser->arena);
    addr_stack_init(&parser->pc_stack, 256);
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

typedef struct expr {
    ExprType  type;
    struct expr* op1;
    struct expr* op2;
    u32          num;
    String     name;
    u8           bytes;
} Expr;

static u64 uadd   (u64 a, u64 b) {return a  + b;}
static u64 usub   (u64 a, u64 b) {return a  - b;}
static u64 umul   (u64 a, u64 b) {return a  * b;}
static u64 udiv   (u64 a, u64 b) {return a  / b;}
static u64 ulshift(u64 a, u64 b) {return a << b;}
static u64 urshift(u64 a, u64 b) {return a >> b;}
static u64 ubitand(u64 a, u64 b) {return a  & b;}
static u64 ubitor (u64 a, u64 b) {return a  | b;}

typedef struct {
    TokenType token_type;
    ExprType Exprype;
    u64 (*op)(u64, u64);
    int level;
    int optimize;
} bin_op_hierarchy_entry;

static
const bin_op_hierarchy_entry hierarchy[] = {
    [EXPR_NULL]   = { 0 },
    [EXPR_MULT]   = { .token_type = TOKEN_TIMES,     .op = umul,    .level = 4, .optimize = 0 },
    [EXPR_DIV]    = { .token_type = TOKEN_SLASH,     .op = udiv,    .level = 4, .optimize = 0 },
    [EXPR_BITAND] = { .token_type = TOKEN_AMPERSAND, .op = ubitand, .level = 3, .optimize = 1 },
    [EXPR_BITOR]  = { .token_type = TOKEN_PIPE,      .op = ubitor,  .level = 3, .optimize = 0 },
    [EXPR_LSHIFT] = { .token_type = TOKEN_LSHIFT,    .op = ulshift, .level = 2, .optimize = 1 },
    [EXPR_RSHIFT] = { .token_type = TOKEN_RSHIFT,    .op = urshift, .level = 2, .optimize = 1 },
    [EXPR_ADD]    = { .token_type = TOKEN_PLUS,      .op = uadd,    .level = 1, .optimize = 0 },
    [EXPR_SUB]    = { .token_type = TOKEN_MINUS,     .op = usub,    .level = 1, .optimize = 0 },
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
            op1.num = -op1.num;
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
            .num   = parser->token.num,
            .bytes = parser->token.bytes,
        };
        parser_advance(parser);
    } else if(parser->token.type == TOKEN_LABEL) {
        op1 = (Expr){
            .type  = EXPR_LABEL,
            .name  = parser->token.text,
            .bytes = 3,
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
            op1.bytes = max(op1.bytes, op2.bytes);                      \
            op1.num   = hierarchy[expr_type].op(op1.num, op2.num);      \
        } else {                                                        \
            Expr* ops = arena_alloc_array(parser->arena, 2, Expr);  \
            ops[0] = op1; ops[1] = op2;                                 \
            Expr op1 = { .type  = expr_type, .op1   = ops + 0, .op2   = ops + 1, \
                           .bytes = max(ops[0].bytes, ops[1].bytes)};   \
            if(hierarchy[expr_type].optimize && op2.type == EXPR_NUM) { \
                u32 max_num = (1 << (op1.bytes * 8)) - 1;               \
                op1.bytes = num_bytes_of_u32(                           \
                    hierarchy[expr_type].op(max_num, op2.num));         \
            }                                                           \
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
void parse_statement(ParserState* parser) {
    u32* pc = addr_stack_top(&parser->pc_stack);
    switch(parser->token.type) {
    case TOKEN_IDENTIFIER: {
        if(parser->token.ident_type & IDENT_COMMAND) {
            switch(parser->token.ident_type) {
            case IDENT_INCSRC: {
                parser_advance(parser);
                parser_expect(parser, TOKEN_STRING);

                String file_name = parser->token.text;
                Path* file = arena_alloc_type(parser->arena, Path);
                path_init_from(file, parser->working_directory, file_name);
                parser_advance(parser);
                lexer_push_file(&parser->lexer, file, parser->arena);
                parser_advance(parser);
                return;
            }
                
            case IDENT_INCBIN: {
                parser_advance(parser);
                parser_expect(parser, TOKEN_STRING);

                String file_name = parser->token.text;
                Path* file = arena_alloc_type(parser->arena, Path);
                path_init_from(file, parser->working_directory, file_name);

                parser_advance(parser);
                int include_somewhere_else = 0;
                u32 addr = *pc;

                if(parser->token.type == TOKEN_ARROW) {
                    parser_advance(parser);
                    Expr expr = parse_expr(parser);
                    if(expr.type != EXPR_NUM) {
                        error_at_pos(parser->token.text_pos);
                        fprintf(stderr, "Expression not fully computable.\n");
                        exit(1);
                    }
                    addr = expr.num;
                    include_somewhere_else = 1;
                }

                Temp temp = temp_begin(parser->arena);
                Buffer buffer = path_open_file(file, parser->arena);
                ulong file_size = path_get_file_size(file);

                wdc65816_mapper_write_range(&parser->rom->read_mapper, addr,
                                            addr + file_size, buffer.begin);
                temp_end(temp);
                if(!include_somewhere_else) *addr_stack_top(&parser->pc_stack) += file_size;
                break;
            }

            case IDENT_TABLE: {
                parser_advance(parser);
                parser_expect(parser, TOKEN_STRING);
                //result.string1 = parser->token.text;
                parser_advance(parser);
                break;
            }
                
            case IDENT_WARNPC: {
                parser_advance(parser);
                Expr expr = parse_expr(parser);
                parser_expect_expr(parser, expr);
                if(expr.type != EXPR_NUM) {
                    error_at_pos(parser->token.text_pos);
                    fprintf(stderr, "Expression not fully computable.\n");
                    exit(1);
                }
                if(*addr_stack_top(&parser->pc_stack) > expr.num) {
                    u32 pc = *addr_stack_top(&parser->pc_stack);
                    error_at_pos(parser->token.text_pos);
                    fprintf(stderr, "warnpc: pc = %06x = %i, "
                            "should be %06x = %i.\n", pc, pc,
                            expr.num, expr.num);
                    exit(1);
                }
                break;
            }

            case IDENT_ORG: {
                parser_advance(parser);
                Expr expr = parse_expr(parser);
                parser_expect_expr(parser, expr);
                if(expr.type != EXPR_NUM) {
                    error_at_pos(parser->token.text_pos);
                    fprintf(stderr, "Expression not fully computable.\n");
                    exit(1);
                }
                *addr_stack_top(&parser->pc_stack) = expr.num;
                break;
            }

            case IDENT_DB:
            case IDENT_DW:
            case IDENT_DL:
            case IDENT_DD: {
                int length = parser->token.ident_type - IDENT_DB + 1;
                parser_advance(parser);
                Expr expr;
                if(parser->token.type == TOKEN_STRING) {
                    /* if(pc + length * parser->token.text.length > parser->size) { */
                    /*     error_at_pos(parser->token.text_pos); */
                    /*     fprintf(stderr, "String to large"); */
                    /*     exit(1); */
                    /* } */
                    /* u8* rom_ptr = parser->data + parser->pc; */
                    for(int i = 0; i < parser->token.text.length; i++) {
                        //TODO: Table conversion
                        /* *rom_ptr++ = ((u8*)parser->token.text.data)[i]; */
                        /* for(int j = 1; j < length; j++) *rom_ptr++ = 0; */
                        //(*parser->rom->ptr(*addr_stack_top(&parser->pc_stack), NULL)) += parser->token.text.length * length;
                    }
                    *addr_stack_top(&parser->pc_stack) += parser->token.text.length * length;
                    parser_advance(parser);
                } else {
                    expr = parse_expr(parser);
                    if(expr.type == EXPR_NULL) {
                        error_at_pos(parser->token.text_pos);
                        fprintf(stderr, "Expected expression or string\n");
                        exit(1);
                    }
                    switch(length) {
                    case 4:
                        parser->rom->write(*pc, expr.num >> 24);
                        *pc += 1;
                    case 3:
                        parser->rom->write(*pc, expr.num >> 16);
                        *pc += 1;
                    case 2:
                        parser->rom->write(*pc, expr.num >>  8);
                        *pc += 1;
                    case 1:
                        parser->rom->write(*pc, expr.num);
                        *pc += 1;
                        break;
                    default:
                        error_at_pos(parser->token.text_pos);
                        fprintf(stderr, "Length %i on line " STR(__LINE__) " in " STR(__FILE__) " should not occur.\n", length);
                        exit(1);
                    }
                }
                while(parser->token.type != TOKEN_CMD_DELIM) {
                    if(parser->token.type != TOKEN_COMMA) {
                        error_at_pos(parser->token.text_pos);
                        fprintf(stderr, "Expected command delimiter or comma.\n");
                        exit(1);
                    } else {
                        parser_advance(parser);
                        if(parser->token.type == TOKEN_STRING) {
                            /* if(parser->pc + length * parser->token.text.length > parser->size) { */
                            /*     error_at_pos(parser->token.text_pos); */
                            /*     fprintf(stderr, "String to large"); */
                            /*     exit(1); */
                            /* } */
                            /* u8* rom_ptr = parser->data + parser->pc; */
                            for(int i = 0; i < parser->token.text.length; i++) {
                                //TODO: Table conversion
                                /* *rom_ptr++ = ((u8*)parser->token.text.data)[i]; */
                                /* for(int j = 1; j < length; j++) *rom_ptr++ = 0; */
                            }
                            *pc += parser->token.text.length * length;
                            parser_advance(parser);
                        } else {
                            expr = parse_expr(parser);
                            if(expr.type == EXPR_NULL) {
                                error_at_pos(parser->token.text_pos);
                                fprintf(stderr, "Expected expression or string\n");
                                exit(1);
                            }
                        }
                    }
                }
                break;
            }

            case IDENT_IF: {
                parser_advance(parser);
                Expr expr = parse_expr(parser);
                if(expr.type != EXPR_NUM) {
                    error_at_pos(parser->token.text_pos);
                    fprintf(stderr, "Expression not fully computable.\n");
                    exit(1);
                }
                break;
            }

            case IDENT_ELSE: {
                parser_advance(parser);
                break;
            }

            case IDENT_ENDIF: {
                parser_advance(parser);
                break;
            }

            case IDENT_FILL: {
                parser_advance(parser);
                Expr expr = parse_expr(parser);
                if(expr.type != EXPR_NUM) {
                    error_at_pos(parser->token.text_pos);
                    fprintf(stderr, "Expression not fully computable.\n");
                    exit(1);
                }
                break;
            }

            case IDENT_FILLBYTE: {
                parser_advance(parser);
                Expr expr = parse_expr(parser);
                if(expr.type != EXPR_NUM) {
                    error_at_pos(parser->token.text_pos);
                    fprintf(stderr, "Expression not fully computable.\n");
                    exit(1);
                }
                break;
            }

            case IDENT_BASE: {
                parser_advance(parser);
                if(parser->token.type == TOKEN_IDENTIFIER && parser->token.ident_type == IDENT_OFF) {
                    parser_advance(parser);
                } else {
                    Expr expr = parse_expr(parser);
                    if(expr.type != EXPR_NUM) {
                        error_at_pos(parser->token.text_pos);
                        fprintf(stderr, "Expression not fully computable.\n");
                        exit(1);
                    }                    
                    break;
                }
            }

            case IDENT_CLEARTABLE: {
                parser_advance(parser);
                break;
            }

            default: {
                error_at_pos(parser->token.text_pos);
                fprintf(stderr, "Unhandled command.\n");
                exit(1);
                break;
            }
            }
                
        } else {
            Token opcode = parser->token;
            parser_advance(parser);
            //short bytes = 0; // 0 = unsure
            short width_specifier = 0;
            //short length = 0;
            //u8    code[4];
            u8    op_byte = opcode.ident_type;
            //code[0] = opcode_addr_mode[op_byte][0];
            if(opcode.ident_type == IDENT_MVN || opcode.ident_type == IDENT_MVP) {
                Expr op1 = parse_expr(parser);
                parser_expect_expr(parser, op1);
                parser_expect(parser, TOKEN_COMMA);
                parser_advance(parser);
                Expr op2 = parse_expr(parser);
                parser_expect_expr(parser, op2);
            } else {
                if(parser->token.type == TOKEN_WIDTH_SPECIFIER) {
                    parser_advance(parser);
                    //bytes = width_specifier = parser->token.bytes;
                }
                if(parser->token.type == TOKEN_CMD_DELIM) {
                    if(opcode_addr_mode[op_byte][0] == -1) {
                        error_at_pos(opcode.text_pos);
                        fprintf(stderr, "Implied mode invalid for opcode \"%.*s\".\n", opcode.text.length, opcode.text.data);
                        exit(1);
                    }
                    if(width_specifier != 0) {
                        error_at_pos(opcode.text_pos);
                        fprintf(stderr, "Width specifier meaningless for implied mode in opcode \"%.*s.\n",
                                opcode.text.length, opcode.text.data);
                        exit(1);
                    }
                    //length  = 1;
                } else if(parser->token.type == TOKEN_IDENTIFIER && parser->token.ident_type == IDENT_A) {
                    parser_advance(parser);
                    if(opcode_addr_mode[op_byte][24] == -1) {
                        error_at_pos(opcode.text_pos);
                        fprintf(stderr, "Implied mode invalid for opcode \"%.*s\".\n", opcode.text.length, opcode.text.data);
                        exit(1);
                    }
                    if(width_specifier != 0) {
                        error_at_pos(opcode.text_pos);
                        fprintf(stderr, "Width specifier meaningless for implied mode in opcode \"%.*s.\n",
                                opcode.text.length, opcode.text.data);
                        exit(1);
                    }
                    //length  = 1;
                } else if(parser->token.type ==  TOKEN_HASH) {
                    if(opcode_addr_mode[opcode.ident_type][7] == -1) {
                        error_at_pos(opcode.text_pos);
                        fprintf(stderr, "Immediate mode invalid for opcode %.*s\n", opcode.text.length, opcode.text.data);
                        exit(1);
                    }
                    parser_advance(parser);
                    Expr expr = parse_expr(parser);
                    parser_expect_expr(parser, expr);
                } else if(parser->token.type == TOKEN_LPAREN) {
                    parser_advance(parser);
                    Expr expr = parse_expr(parser);
                    parser_expect_expr(parser, expr);
                    if(parser->token.type == TOKEN_COMMA) {
                        parser_advance(parser);
                        parser_expect(parser, TOKEN_IDENTIFIER);
                    } else if(parser->token.type == TOKEN_RPAREN) {
                        parser_advance(parser);
                        if(parser->token.type == TOKEN_COMMA) {
                            parser_advance(parser);
                            parser_expect(parser, TOKEN_IDENTIFIER);
                            parser_advance(parser);
                        }
                    } else {
                        error_at_pos(parser->token.text_pos);
                        fprintf(stderr, "Expected ',' or ')'.\n");
                        exit(1);
                    }
                } else if(parser->token.type == TOKEN_LBRACKET) {
                    parser_advance(parser);
                    parse_expr(parser);
                    if(parser->token.type == TOKEN_COMMA) {
                        parser_advance(parser);
                        parser_expect(parser, TOKEN_IDENTIFIER);
                        parser_advance(parser);
                        parser_expect(parser, TOKEN_LBRACKET);
                        parser_advance(parser);
                    } else if(parser->token.type == TOKEN_RBRACKET) {
                        parser_advance(parser);
                        if(parser->token.type == TOKEN_COMMA) {
                            parser_advance(parser);
                            parser_expect(parser, TOKEN_IDENTIFIER);
                            parser_advance(parser);
                        }
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
                            parser_expect(parser, TOKEN_IDENTIFIER);
                            parser_advance(parser);
                            if(parser->token.type == TOKEN_COMMA) {
                                parser_advance(parser);
                                parser_expect(parser, TOKEN_IDENTIFIER);
                                parser_advance(parser);
                            }
                        }
                    } else {
                        error_at_pos(parser->token.text_pos);
                        fprintf(stderr, "Unexpected token.\n");
                        exit(1);
                    }
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
            Define define = {
                .name         = token.text,
                .buffer.begin = lexer_state->pos,
                .buffer.end   = c,
#ifdef TEXT_POS
                .text_pos     = token.text_pos,
#endif
            };
            define_map_insert(&parser->lexer.define_map, define);
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
        // TODO
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
}


void parse_asm(ParserState* parser) {
    parser_advance(parser);
    while(parser->token.type != TOKEN_EOF) {
        parse_statement(parser);
    }
}

void parser_global_init() {
    identifier_map_fill();
}
void parser_global_deinit() {
    identifier_map_deinit(&identifier_map);
}
