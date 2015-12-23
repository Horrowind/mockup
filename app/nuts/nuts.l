%{
#include <stdio.h>
#include "y.tab.h"

#define MAX_INCLUDE_DEPTH 10

YY_BUFFER_STATE include_stack[MAX_INCLUDE_DEPTH];
int include_stack_ptr = 0;

%}
                                                
                        digit   [0-9]
                        letter  [a-zA-Z]
                        ident   {letter}({letter}|{digit}|"_")
                        
                        
                        
%option case-insensitive
                        
%x COMMENT
%x MACRO_DECLLIST
%x MACRO_BODY
                        
                        
%%

";"            { BEGIN(COMMENT); }
<COMMENT>"\n"  { BEGIN(INITIAL); }
<COMMENT>.   { }


[ \t]          ;

  /* TODO: Implement numbers */
  /*               { return NUMBER   };*/
"+"            { return PLUS     };
"-"            { return MINUS    };
"/"            { return DIV      };
"*"            { return MULT     };
":"            { return COLON    };
"<"            { return LESS     };
">"            { return GREATER  };
"<<"           { return LSHIFT   };
">>"           { return RSHIFT   };
"."            { return DOT      };
"!"            { return EXCLAM   };
"#"            { return HASH     };
"&"            { return AMPER    };
"^"            { return HAT      };
"|"            { return PIPE     };
"~"            { return TILDE    };
"$"            { return DOLLAR   };
"@"            { return AT       };
"="            { return EQUAL    };
"{"            { return LCURLY   };
"}"            { return RCURLY   };
"("            { return LBRACE   };
")"            { return RBRACE   };
"["            { return LBRACK   };
"]"            { return RBRACK   };
"?"            { return QMARK    };
","            { return COMMA    };
[+]+           { return FWDLABEL };
[-]-           { return BWDLABEL };


  /* Asar Commands */

"macro"        { BEGIN(MACRO_DECLLIST); };
<MACRO_DECLLIST>[ \t]* { };
<MACRO_DECLLIST>"("    { return MACRO_LPAREN; };
<MACRO_DECLLIST>","    { return MACRO_COMMA; };
<MACRO_DECLLIST>ident  { return MACRO_IDENT; };
<MACRO_DECLLIST>")"    { 
    BEGIN(MACRO_BODY);
    return MACRO_RPAREN;
  };
<MACRO_BODY>"\0"       { error(); };
<MACRO_BODY>"\nendmacro"     {
    BEGIN(INITIAL);
    return ENDMACRO;
  };

"%"ident       {
  // Macro call
 
  };

"fill"         { return FILL     };
"fillbyte"     { return FILLBYTE };
"incbin"       { return INCBIN   };
"incsrc"       { return INCSRC   };
"pad"          { return PAD      };
"padbyte"      { return PADBYTE  };
"db"           { return DB       };
"dw"           { return DW       };
"dl"           { return DL       };
"dd"           { return DD       };
"skip"         { return SKIP     };
"namespace"    { return NSPACE   };
"on"           { return ON       };
"off"          { return OFF      };
"org"          { return ORG      };
"warnpc"       { return WARNPC   };
"base"         { return BASE     };
"freespace"    { return FREESP   };
"freecode"     { return FREECODE };
"freedata"     { return FREEDATA };
"pushpc"       { return PUSHPC   };
"pullpc"       { return PULLPC   };




  /* SFC Commands */
"adc"          { return ADC      };
"and"          { return AND      };
"asl"          { return ASL      };
"bcc"          { return BCC      };
"blt"          { return BLT      };
"bcs"          { return BCS      };
"bge"          { return BGE      };
"beq"          { return BEQ      };
"bit"          { return BIT      };
"bmi"          { return BMI      };
"bne"          { return BNE      };
"bpl"          { return BPL      };
"bra"          { return BRA      };
"brk"          { return BRK      };
"brl"          { return BRL      };
"bvc"          { return BVC      };
"bvs"          { return BVS      };
"clc"          { return CLC      };
"cld"          { return CLD      };
"cli"          { return CLI      };
"clv"          { return CLV      };
"cmp"          { return CMP      };
"cop"          { return COP      };
"cpx"          { return CPX      };
"cpy"          { return CPY      };
"dec"          { return DEC      };
"dex"          { return DEX      };
"dey"          { return DEY      };
"eor"          { return EOR      };
"inc"          { return INC      };
"inx"          { return INX      };
"iny"          { return INY      };
"jml"          { return JML      };
"jmp"          { return JMP      };
"jsl"          { return JSL      };
"jsr"          { return JSR      };
"lda"          { return LDA      };
"ldx"          { return LDX      };
"ldy"          { return LDY      };
"lsr"          { return LSR      };
"mvn"          { return MVN      };
"mvp"          { return MVP      };
"nop"          { return NOP      };
"ora"          { return ORA      };
"pea"          { return PEA      };
"pei"          { return PEI      };
"per"          { return PER      };
"pha"          { return PHA      };
"phb"          { return PHB      };
"phd"          { return PHD      };
"phk"          { return PHK      };
"php"          { return PHP      };
"phx"          { return PHX      };
"phy"          { return PHY      };
"pla"          { return PLA      };
"plb"          { return PLB      };
"pld"          { return PLD      };
"plp"          { return PLP      };
"plx"          { return PLX      };
"ply"          { return PLY      };
"rep"          { return REP      };
"rol"          { return ROL      };
"ror"          { return ROR      };
"rti"          { return RTI      };
"rtl"          { return RTL      };
"rts"          { return RTS      };
"sbc"          { return SBC      };
"sec"          { return SEC      };
"sed"          { return SED      };
"sei"          { return SEI      };
"sep"          { return SEP      };
"sta"          { return STA      };
"stp"          { return STP      };
"stx"          { return STX      };
"sty"          { return STY      };
"stz"          { return STZ      };
"tax"          { return TAX      };
"tay"          { return TAY      };
"tcd"          { return TCD      };
"tcs"          { return TCS      };
"tdc"          { return TDC      };
"trb"          { return TRB      };
"tsb"          { return TSB      };
"tsc"          { return TSC      };
"tsx"          { return TSX      };
"txa"          { return TXA      };
"txs"          { return TXS      };
"txy"          { return TXY      };
"tya"          { return TYA      };
"tyx"          { return TYX      };
"wai"          { return WAI      };
"wdm"          { return WDM      };
"xba"          { return XBA      };
"xce"          { return XCE      };

ident          { return IDENT    };

%%
int main(int, char**) {
	// lex through the input:
	yylex();
}
