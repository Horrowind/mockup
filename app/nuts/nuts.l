%{
#include <stdio.h>

#define MAX_INCLUDE_DEPTH 10

//YY_BUFFER_STATE include_stack[MAX_INCLUDE_DEPTH];
//int include_stack_ptr = 0;
//#define YY_DECL extern int yylex()

void yyerror();
#include "nuts.tab.h"

#ifdef NDEBUG
#define return_token(token)                                \
if(!NDEBUG) printf("Found " #token "\n");                 \
return token;
#else
#define return_token(token)                                \
return token;
#endif



/* void yyerror(const char *s) { */
/* 	// might as well halt now: */
/*     printf("Error: %s\n"); */
/* } */

%}
%option noyywrap
%option yylineno
digit   [0-9]
hexdigit[0-9a-fA-F]
letter  [a-zA-Z]
ident   ({letter}|"_")({letter}|{digit}|"_")*
byte    "$"{hexdigit}?{hexdigit}                        
word    "$"{hexdigit}?{hexdigit}{hexdigit}{hexdigit}
long    "$"{hexdigit}?{hexdigit}{hexdigit}{hexdigit}{hexdigit}{hexdigit}
double    "$"{hexdigit}?{hexdigit}{hexdigit}{hexdigit}{hexdigit}{hexdigit}{hexdigit}{hexdigit}
number  {digit}+
%option case-insensitive
                        
%x COMMENT
%x MACRO_DECLLIST
%x MACRO_BODY
                        
                        
%%

";"            { BEGIN(COMMENT);                              };
<COMMENT>"\n"  { BEGIN(INITIAL); return_token(NEWLINE);       };
<COMMENT>.     {                                              };




[ \t]          { }
[\n]           { return_token(NEWLINE);  };
{word}         { return_token(WORD);     };
{byte}         { return_token(BYTE);     };
{long}         { return_token(LONG);     };
{double}       { return_token(DOUBLE);   };
{number}       { return_token(NUMBER);   };
"+"            { return_token(PLUS);     };
"-"            { return_token(MINUS);    };
"/"            { return_token(DIV);      };
"*"            { return_token(MULT);     };
"**"           { return_token(MULT);     };
":"            { return_token(COLON);    };
"<"            { return_token(LESS);     };
">"            { return_token(GREATER);  };
"<<"           { return_token(LSHIFT);   };
">>"           { return_token(RSHIFT);   };
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
\"(\\.|[^\\\"])*\" { return_token(STRING); };

   /* Asar Commands */

 "macro"                 {
     BEGIN(MACRO_DECLLIST);
     return_token(MACRO);
 };
 <MACRO_DECLLIST>[ \t]*  { };
 <MACRO_DECLLIST>"("     { return_token(MACRO_LPAREN); };
 <MACRO_DECLLIST>","     { return_token(MACRO_COMMA); };
 <MACRO_DECLLIST>{ident} { return_token(MACRO_IDENT); };
 <MACRO_DECLLIST>")"     {
     BEGIN(MACRO_BODY);
     return_token(MACRO_RPAREN);
   };
 <MACRO_BODY>"\0"       { yyerror("EOF in macro body"); };
 <MACRO_BODY>"\nendmacro"     {
     BEGIN(INITIAL);
     return_token(ENDMACRO);
   };
<MACRO_BODY>.           { };
 "%"ident       {
   // Macro call
 
   };

"fill"         { return_token(FILL);     };
"fillbyte"     { return_token(FILLBYTE); };
"incbin"       { return_token(INCBIN);   };
"incsrc"       { return_token(INCSRC);   };
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

{ident}          { return_token(IDENT);    };

%%
/* int main(int argc, char** argv) { */
/* 	// open a file handle to a particular file: */
/*     if(argc == 1) { */
/*         while(1) yylex(); */
/*     } else if(argc == 2) { */
/*         FILE* fp = fopen(argv[1], "r"); */
/*         // make sure it is valid: */
/*         if (!fp) { */
/*             printf("Could not open %s\n", argv[1]); */
/*         } else { */
/*             yyin = fp; */
/*             //do { */
/*                 yylex(); */
/*                 //} while (!feof(yyin)); */
/*         } */
/*     } else { */
/*         printf("Too many arguments.\n"); */
/*     }         */
/* } */

void yyerror(const char *s) {
	// might as well halt now:
    printf("Error on line %i: %s\n", yylineno, yytext);
}
