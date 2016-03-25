/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Skeleton interface for Bison GLR parsers in C

   Copyright (C) 2002-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_NUTS_TAB_H_INCLUDED
# define YY_YY_NUTS_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    NEWLINE = 258,
    NUMBER = 259,
    STRING = 260,
    COLON = 261,
    LESS = 262,
    GREATER = 263,
    DOT = 264,
    EXCLAM = 265,
    HASH = 266,
    TILDE = 267,
    DOLLAR = 268,
    AT = 269,
    EQUAL = 270,
    LPAREN = 271,
    RPAREN = 272,
    LCURLY = 273,
    RCURLY = 274,
    LBRACK = 275,
    RBRACK = 276,
    QMARK = 277,
    COMMA = 278,
    SEMICOL = 279,
    MACRO = 280,
    ENDMACRO = 281,
    MACRO_LPAREN = 282,
    MACRO_RPAREN = 283,
    MACRO_COMMA = 284,
    MACRO_IDENT = 285,
    FILL = 286,
    FILLBYTE = 287,
    INCBIN = 288,
    PAD = 289,
    PADBYTE = 290,
    DB = 291,
    DW = 292,
    DL = 293,
    DD = 294,
    SKIP = 295,
    NSPACE = 296,
    ON = 297,
    OFF = 298,
    ORG = 299,
    WARNPC = 300,
    BASE = 301,
    FREESP = 302,
    FREECODE = 303,
    FREEDATA = 304,
    AUTO = 305,
    NOASSUME = 306,
    BANK = 307,
    ERROR = 308,
    WARN = 309,
    PUSHPC = 310,
    PULLPC = 311,
    LABELDEC = 312,
    SUBLABEL = 313,
    FWDLABEL = 314,
    BWDLABEL = 315,
    LOROM = 316,
    HIROM = 317,
    NOROM = 318,
    SFXROM = 319,
    SA1ROM = 320,
    FSA1ROM = 321,
    POWER = 322,
    MULT = 323,
    DIV = 324,
    PERCENT = 325,
    PLUS = 326,
    MINUS = 327,
    LSHIFT = 328,
    RSHIFT = 329,
    AMPER = 330,
    PIPE = 331,
    HAT = 332,
    ADC = 333,
    AND = 334,
    ASL = 335,
    BCC = 336,
    BLT = 337,
    BCS = 338,
    BGE = 339,
    BEQ = 340,
    BIT = 341,
    BMI = 342,
    BNE = 343,
    BPL = 344,
    BRA = 345,
    BRK = 346,
    BRL = 347,
    BVC = 348,
    BVS = 349,
    CLC = 350,
    CLD = 351,
    CLI = 352,
    CLV = 353,
    CMP = 354,
    COP = 355,
    CPX = 356,
    CPY = 357,
    DEC = 358,
    DEX = 359,
    DEY = 360,
    EOR = 361,
    INC = 362,
    INX = 363,
    INY = 364,
    JML = 365,
    JMP = 366,
    JSL = 367,
    JSR = 368,
    LDA = 369,
    LDX = 370,
    LDY = 371,
    LSR = 372,
    MVN = 373,
    MVP = 374,
    NOP = 375,
    ORA = 376,
    PEA = 377,
    PEI = 378,
    PER = 379,
    PHA = 380,
    PHB = 381,
    PHD = 382,
    PHK = 383,
    PHP = 384,
    PHX = 385,
    PHY = 386,
    PLA = 387,
    PLB = 388,
    PLD = 389,
    PLP = 390,
    PLX = 391,
    PLY = 392,
    REP = 393,
    ROL = 394,
    ROR = 395,
    RTI = 396,
    RTL = 397,
    RTS = 398,
    SBC = 399,
    SEC = 400,
    SED = 401,
    SEI = 402,
    SEP = 403,
    STA = 404,
    STP = 405,
    STX = 406,
    STY = 407,
    STZ = 408,
    TAX = 409,
    TAY = 410,
    TCD = 411,
    TCS = 412,
    TDC = 413,
    TRB = 414,
    TSB = 415,
    TSC = 416,
    TSX = 417,
    TXA = 418,
    TXS = 419,
    TXY = 420,
    TYA = 421,
    TYX = 422,
    WAI = 423,
    WDM = 424,
    XBA = 425,
    XCE = 426,
    SPECBYTE = 427,
    SPECWORD = 428,
    SPECLONG = 429,
    TOC_S = 430,
    TOC_X = 431,
    TOC_Y = 432,
    BYTE = 433,
    WORD = 434,
    LONG = 435,
    DOUBLE = 436,
    IDENT = 437
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 21 "nuts.y" /* glr.c:197  */

    string_t val_ident;
    u8       val_byte;
    u16      val_word;
    u32      val_long;
    u32      val_double;
    int      number;

#line 246 "nuts.tab.h" /* glr.c:197  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_NUTS_TAB_H_INCLUDED  */
