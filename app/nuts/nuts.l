%{
#include <stdio.h>

#include "base/variable_string.h"
#include "base/hash.h"
#include "base/stack.h"


#define IN_FLEX
#include "parse.h"
#undef IN_FLEX
#include "nuts.tab.h"

string_t current_identifier;
int parenthesis_level;
%}
%option noyywrap
%option yylineno
                       
digit        [0-9]
hexdigit     [0-9a-fA-F]
letter       [a-zA-Z]
ident        ({letter}|"_")({letter}|{digit}|"_")*
byte         "$"{hexdigit}?{hexdigit}                        
word         "$"{hexdigit}?{hexdigit}{hexdigit}{hexdigit}
long         "$"{hexdigit}?{hexdigit}{hexdigit}{hexdigit}{hexdigit}{hexdigit}
double       "$"{hexdigit}?{hexdigit}{hexdigit}{hexdigit}{hexdigit}{hexdigit}{hexdigit}{hexdigit}
number       {digit}+
string       \"(\\.|[^\\\"])*\"
nowhitespace [^ \"\t\r\n][^ \t\r\n]*


%option case-insensitive
                        
%x COMMENT
%x MACRO_DECLLIST_A
%x MACRO_DECLLIST_B
%x MACRO_DECLLIST_C
%x MACRO_DECLLIST_D
%x MACRO_DECLLIST_E
%x MACROCALL_DECLLIST_A
%x MACROCALL_DECLLIST_B
%x MACROCALL_DECLLIST_C
%x MACROCALL_DECLLIST_D
%x MACROCALL_DECLLIST_E
%x MACRO_BODY
%x INCL
%x DEFINE_DEF
%x DEFINE_CALL

%%

";"            { BEGIN(COMMENT);                              };
<COMMENT>"\n"  { BEGIN(INITIAL); return_token(NEWLINE);       };
<COMMENT>.     {                                              };




[ \r\t]        { }
{byte}         {
                 if(yyleng == 2) {
                     yylval.val_word  = char2hex(yytext[1]) <<  0;
                 } else {
                     yylval.val_word |= char2hex(yytext[1]) <<  4;
                     yylval.val_word |= char2hex(yytext[2]) <<  0;
                 }
#ifdef PRINT_TOKENS
                 printf("Found: BYTE (%02x)\n", yylval.val_byte);
#endif
                 return BYTE;
               };

{word}         {
                 if(yyleng == 4) {
                     yylval.val_word  = char2hex(yytext[1]) <<  8;
                     yylval.val_word |= char2hex(yytext[2]) <<  4;
                     yylval.val_word |= char2hex(yytext[3]) <<  0;
                 } else {
                     yylval.val_word  = char2hex(yytext[1]) << 12;
                     yylval.val_word |= char2hex(yytext[2]) <<  8;
                     yylval.val_word |= char2hex(yytext[3]) <<  4;
                     yylval.val_word |= char2hex(yytext[4]) <<  0;
                 }
#ifdef PRINT_TOKENS
                 printf("Found: WORD (%04x)\n", yylval.val_word);
#endif
                 return WORD;
               };

{long}         {
                 if(yyleng == 6) {
                     yylval.val_long  = char2hex(yytext[1]) << 16;
                     yylval.val_long |= char2hex(yytext[2]) << 12;
                     yylval.val_long |= char2hex(yytext[3]) <<  8;
                     yylval.val_long |= char2hex(yytext[4]) <<  4;
                     yylval.val_long |= char2hex(yytext[5]) <<  0;
                 } else {
                     yylval.val_long  = char2hex(yytext[1]) << 20;
                     yylval.val_long |= char2hex(yytext[2]) << 16;
                     yylval.val_long |= char2hex(yytext[3]) << 12;
                     yylval.val_long |= char2hex(yytext[4]) <<  8;
                     yylval.val_long |= char2hex(yytext[5]) <<  4;
                     yylval.val_long |= char2hex(yytext[6]) <<  0;
                 }
#ifdef PRINT_TOKENS
                 printf("Found: LONG (%06x)\n", yylval.val_long);
#endif
                 return LONG;
               };

{double}       {
                 if(yyleng == 8) {
                     yylval.val_double  = char2hex(yytext[1]) << 24;
                     yylval.val_double |= char2hex(yytext[2]) << 20;
                     yylval.val_double |= char2hex(yytext[3]) << 16;
                     yylval.val_double |= char2hex(yytext[4]) << 12;
                     yylval.val_double |= char2hex(yytext[5]) <<  8;
                     yylval.val_double |= char2hex(yytext[6]) <<  4;
                     yylval.val_double |= char2hex(yytext[7]) <<  0;
                 } else {
                     yylval.val_double  = char2hex(yytext[1]) << 28;
                     yylval.val_double |= char2hex(yytext[2]) << 24;
                     yylval.val_double |= char2hex(yytext[3]) << 20;
                     yylval.val_double |= char2hex(yytext[4]) << 16;
                     yylval.val_double |= char2hex(yytext[5]) << 12;
                     yylval.val_double |= char2hex(yytext[6]) <<  8;
                     yylval.val_double |= char2hex(yytext[7]) <<  4;
                     yylval.val_double |= char2hex(yytext[8]) <<  0;
                 }
#ifdef PRINT_TOKENS
                 printf("Found: DOUBLE (%08x)\n", yylval.val_double);
#endif
                 return DOUBLE;
               };

{number}       { return_token(NUMBER);   };
"+"            { return_token(PLUS);     };
"-"            { return_token(MINUS);    };
"/"            { return_token(DIV);      };
"*"            { return_token(MULT);     };
"**"           { return_token(MULT);     };
"<"            { return_token(LESS);     };
">"            { return_token(GREATER);  };
"<<"           { return_token(LSHIFT);   };
">>"           { return_token(RSHIFT);   };
":"            { return_token(COLON);    };
"."            { return_token(DOT);      };
"!"            { return_token(EXCLAM);   };
"#"            { return_token(HASH);     };
"&"            { return_token(AMPER);    };
"^"            { return_token(HAT);      };
"%"            { return_token(PERCENT);  };
"|"            { return_token(PIPE);     };
"~"            { return_token(TILDE);    };
"$"            { return_token(DOLLAR);   };
"@"            { return_token(AT);       };
"="            { return_token(EQUAL);    };
"{"            { return_token(LCURLY);   };
"}"            { return_token(RCURLY);   };
"("            { return_token(LPAREN);   };
")"            { return_token(RPAREN);   };
"["            { return_token(LBRACK);   };
"]"            { return_token(RBRACK);   };
"?"            { return_token(QMARK);    };
","            { return_token(COMMA);    };
".b"           { return_token(SPECBYTE); };
".w"           { return_token(SPECWORD); };
".l"           { return_token(SPECLONG); };
[+]+           { return_token(FWDLABEL); };
[-]-           { return_token(BWDLABEL); };
{ident}":"     { return_token(LABELDEC); };
"."{ident}":"? { return_token(SUBLABEL); };
{string}       { return_token(STRING);   };
"!"{ident}[ \t]*"=" {
                 current_identifier.data = yytext + 1; // We ignore the first '!'.
                 uint length;
                 for(length = yyleng - 1;
                     yytext[length] == ' ' || yytext[length] == '\t';
                     length--) { };
                 current_identifier.length = length - 2; // -1 from starting after the first '!'
                                                         // and another -1 for starting with index 0 
#ifdef PRINT_TOKENS
                 printf("Defined identifier \"%.*s\"\n", current_identifier.length, current_identifier.data);
#endif
                 BEGIN(DEFINE_DEF);
               }

"!"{ident}     {
                 define_t define = {
                     .name = {
                         .data = yytext + 1,
                         .length = yyleng - 1
                     }
                 };
                 define_t* real_define;
                 if(real_define = defines_hash_map_find(&defines_map, define)) {
                     push_parse_buffer(real_define->replacement, real_define->filename);
                 } else {
                     yyerror("Error: Undefined define");
                 }

               }
<DEFINE_DEF>[^:\r\n]*(":"|("\r"?"\n")) {
                 define_t define = {
                     .name = current_identifier,
                     .filename = parse_stack_top(&parse_stack)->filename,
                     .line_num = yylineno,
                     .replacement = {
                         .data = yytext,
                         .length = yyleng - 1
                     }
                 };
                 defines_hash_map_insert(&defines_map, define);
                 BEGIN(INITIAL);
               }

"macro"                  {
     BEGIN(MACRO_DECLLIST_A);
 };

<MACRO_DECLLIST_A>{ident}  {
#ifdef PRINT_TOKENS
                           printf("Found: MACRO NAME (%s)\n", yytext);
#endif
                           BEGIN(MACRO_DECLLIST_B);
                           return_token(MACRO);
                         }
<MACRO_DECLLIST_A>[ \t\r]* { };
<MACRO_DECLLIST_A>.      { yyerror("Expected identifier"); };
<MACRO_DECLLIST_A>"\n"   { yyerror("Expected identifier, got NEWLINE"); };
<MACRO_DECLLIST_B>"("    { BEGIN(MACRO_DECLLIST_C); }
<MACRO_DECLLIST_B>[ \t\r]* { };
<MACRO_DECLLIST_B>.      { yyerror("Expected identifier"); };
<MACRO_DECLLIST_B>"\n"   { yyerror("Expected identifier, got NEWLINE"); };
<MACRO_DECLLIST_C>{ident}  {
#ifdef PRINT_TOKENS
                           printf("Found MACRO IDENTIFIER (%s)\n", yytext);
#endif
                           BEGIN(MACRO_DECLLIST_D);
                         }
<MACRO_DECLLIST_C>[ \t\r]* { };
<MACRO_DECLLIST_C>")"    { BEGIN(MACRO_BODY); };
<MACRO_DECLLIST_C>.      { yyerror("Expected identifier or ')'"); };
<MACRO_DECLLIST_C>"\n"   { yyerror("Expected identifier or ')', got NEWLINE"); };
<MACRO_DECLLIST_D>[ \t\r]* { };
<MACRO_DECLLIST_D>")"    {BEGIN(MACRO_BODY); }
<MACRO_DECLLIST_D>","    {BEGIN(MACRO_DECLLIST_E); }
<MACRO_DECLLIST_D>.      { yyerror("Expected ','"); };
<MACRO_DECLLIST_D>"\n"   { yyerror("Expected ',', got NEWLINE"); };
<MACRO_DECLLIST_E>{ident}  {
                           printf("Found macro identifier: %s\n", yytext);
                           BEGIN(MACRO_DECLLIST_D);
                         }
<MACRO_DECLLIST_E>[ \t\r]* { };
<MACRO_DECLLIST_E>")"    { BEGIN(MACRO_BODY); };
<MACRO_DECLLIST_E>.      { yyerror("Expected identifier"); };
<MACRO_DECLLIST_E>"\n"   { yyerror("Expected identifier, got NEWLINE"); };

<MACRO_BODY>"\0"        { yyerror("EOF in macro body"); };
<MACRO_BODY>"\nendmacro"  {
                            BEGIN(INITIAL);
                            return_token(ENDMACRO);
                        };
<MACRO_BODY>.|\n        { };
 "%"{ident}             { BEGIN(MACROCALL_DECLLIST_B);  };
<MACROCALL_DECLLIST_B>"("    {
                               parenthesis_level = 1;
                               BEGIN(MACROCALL_DECLLIST_C);
                             }
<MACROCALL_DECLLIST_B>[ \t\r]* { };
<MACROCALL_DECLLIST_B>.      { yyerror("Expected '('"); };
<MACROCALL_DECLLIST_B>"\n"   { yyerror("Expected '(', got NEWLINE"); };
<MACROCALL_DECLLIST_C>"("    { parenthesis_level++; };
<MACROCALL_DECLLIST_C>")"    {
                               parenthesis_level--;
                               if(parenthesis_level == 0) {
                                   //TODO: Call macro here.
                                   BEGIN(INITIAL);
                               }
                             }
<MACROCALL_DECLLIST_C>","    {
                               if(parenthesis_level == 1) {
                                   //TODO: Manage macro parameter here
                                   //      Count the number of parameters too.
                               }
                             };
<MACROCALL_DECLLIST_C>.      { };
<MACROCALL_DECLLIST_C>"\n"   { yyerror("Unexpected NEWLINE"); };

"incbin"       { return_token(INCBIN);   };
"incsrc"       { BEGIN(INCL);             };
     
<INCL>[ \t]*     /* eat the whitespace */
<INCL>{nowhitespace} {
                   //got the include file name
                   string_t new_file = read_file_to_buffer(yytext);
                   string_t filename = { .data = yytext, .length = yyleng };
#ifdef PRINT_TOKENS
                   printf("Open1: %s\n", yytext);
#endif
                   push_parse_buffer(new_file, filename);
                   BEGIN(INITIAL);
               }

<INCL>{string} {
                   char buffer[256];
                   strncpy(buffer, yytext, 256);
                   buffer[strlen(buffer) - 1] = '\0';
                   string_t new_file = read_file_to_buffer(buffer + 1);
                   string_t filename = { .data = yytext + 1, .length = yyleng - 2 };
#ifdef PRINT_TOKENS
                   printf("Open2: %s\n", buffer + 1);
#endif
                   push_parse_buffer(new_file, filename);
                   BEGIN(INITIAL);
               }

<<EOF>>        {
                   if(pop_parse_buffer()) {
                       yyterminate();
                   }
               }



"fill"         { return_token(FILL);     };
"fillbyte"     { return_token(FILLBYTE); };

"pad"          { return_token(PAD);      };
"padbyte"      { return_token(PADBYTE);  };
"db"           { return_token(DB);       };
"dw"           { return_token(DW);       };
"dl"           { return_token(DL);       };
"dd"           { return_token(DD);       };
"skip"         { return_token(SKIP);     };
"namespace"    { return_token(NSPACE);   };
"on"           { return_token(ON);       };
"off"          { return_token(OFF);      };
"org"          { return_token(ORG);      };
"warnpc"       { return_token(WARNPC);   };
"base"         { return_token(BASE);     };
"freespace"    { return_token(FREESP);   };
"freecode"     { return_token(FREECODE); };
"freedata"     { return_token(FREEDATA); };
"pushpc"       { return_token(PUSHPC);   };
"pullpc"       { return_token(PULLPC);   };
"norom"        { return_token(NOROM);    };
"lorom"        { return_token(LOROM);    };
"hirom"        { return_token(HIROM);    };
"sfxrom"       { return_token(SFXROM);   };
"sa1rom"       { return_token(SA1ROM);   };
"fullsa1rom"   { return_token(FSA1ROM);  };

  /* SFC Commands */
"adc"          { return_token(ADC);      };
"and"          { return_token(AND);      };
"asl"          { return_token(ASL);      };
"bcc"          { return_token(BCC);      };
"blt"          { return_token(BLT);      };
"bcs"          { return_token(BCS);      };
"bge"          { return_token(BGE);      };
"beq"          { return_token(BEQ);      };
"bit"          { return_token(BIT);      };
"bmi"          { return_token(BMI);      };
"bne"          { return_token(BNE);      };
"bpl"          { return_token(BPL);      };
"bra"          { return_token(BRA);      };
"brk"          { return_token(BRK);      };
"brl"          { return_token(BRL);      };
"bvc"          { return_token(BVC);      };
"bvs"          { return_token(BVS);      };
"clc"          { return_token(CLC);      };
"cld"          { return_token(CLD);      };
"cli"          { return_token(CLI);      };
"clv"          { return_token(CLV);      };
"cmp"          { return_token(CMP);      };
"cop"          { return_token(COP);      };
"cpx"          { return_token(CPX);      };
"cpy"          { return_token(CPY);      };
"dec"          { return_token(DEC);      };
"dex"          { return_token(DEX);      };
"dey"          { return_token(DEY);      };
"eor"          { return_token(EOR);      };
"inc"          { return_token(INC);      };
"inx"          { return_token(INX);      };
"iny"          { return_token(INY);      };
"jml"          { return_token(JML);      };
"jmp"          { return_token(JMP);      };
"jsl"          { return_token(JSL);      };
"jsr"          { return_token(JSR);      };
"lda"          { return_token(LDA);      };
"ldx"          { return_token(LDX);      };
"ldy"          { return_token(LDY);      };
"lsr"          { return_token(LSR);      };
"mvn"          { return_token(MVN);      };
"mvp"          { return_token(MVP);      };
"nop"          { return_token(NOP);      };
"ora"          { return_token(ORA);      };
"pea"          { return_token(PEA);      };
"pei"          { return_token(PEI);      };
"per"          { return_token(PER);      };
"pha"          { return_token(PHA);      };
"phb"          { return_token(PHB);      };
"phd"          { return_token(PHD);      };
"phk"          { return_token(PHK);      };
"php"          { return_token(PHP);      };
"phx"          { return_token(PHX);      };
"phy"          { return_token(PHY);      };
"pla"          { return_token(PLA);      };
"plb"          { return_token(PLB);      };
"pld"          { return_token(PLD);      };
"plp"          { return_token(PLP);      };
"plx"          { return_token(PLX);      };
"ply"          { return_token(PLY);      };
"rep"          { return_token(REP);      };
"rol"          { return_token(ROL);      };
"ror"          { return_token(ROR);      };
"rti"          { return_token(RTI);      };
"rtl"          { return_token(RTL);      };
"rts"          { return_token(RTS);      };
"sbc"          { return_token(SBC);      };
"sec"          { return_token(SEC);      };
"sed"          { return_token(SED);      };
"sei"          { return_token(SEI);      };
"sep"          { return_token(SEP);      };
"sta"          { return_token(STA);      };
"stp"          { return_token(STP);      };
"stx"          { return_token(STX);      };
"sty"          { return_token(STY);      };
"stz"          { return_token(STZ);      };
"tax"          { return_token(TAX);      };
"tay"          { return_token(TAY);      };
"tcd"          { return_token(TCD);      };
"tcs"          { return_token(TCS);      };
"tdc"          { return_token(TDC);      };
"trb"          { return_token(TRB);      };
"tsb"          { return_token(TSB);      };
"tsc"          { return_token(TSC);      };
"tsx"          { return_token(TSX);      };
"txa"          { return_token(TXA);      };
"txs"          { return_token(TXS);      };
"txy"          { return_token(TXY);      };
"tya"          { return_token(TYA);      };
"tyx"          { return_token(TYX);      };
"wai"          { return_token(WAI);      };
"wdm"          { return_token(WDM);      };
"xba"          { return_token(XBA);      };
"xce"          { return_token(XCE);      };
"x"            { return_token(TOC_X);    };
"y"            { return_token(TOC_Y);    };
"s"            { return_token(TOC_S);    };

\n             { return_token(NEWLINE);  };
{ident}        {
                   yylval.val_ident = (string_t){
                       .data = yytext,
                       .length = yyleng
                   };
                   u32 hash = SuperFastHash(yylval.val_ident);
                   if(!string_hash_map_find(&identifier_map, yylval.val_ident)) {
                       string_hash_map_insert2(&identifier_map, yylval.val_ident, hash);
                   }
#ifdef PRINT_TOKENS
                   printf("Found: IDENT (%s)\n", yytext);
#endif
                   return_token(IDENT);
               };

%%

