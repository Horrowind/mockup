%{
//#define YYDEBUG 1
//#define YYMAXDEPTH 10000
#include <stdio.h>
#include <base/variable_string.h>
#include <base/datasizes.h>

#include "parse.h"

// stuff from flex that bison needs to know about:
/* extern int yylex(); */
/* extern int yyparse(); */
/* extern FILE *yyin; */
/* extern void yyerror(const char *s); */

%}

/*%define lr.type ielr*/
%glr-parser

%union {
    string_t val_ident;
    u8       val_byte;
    u16      val_word;
    u32      val_long;
    u32      val_double;
    int      number;
}
                        
                        
%token                  NEWLINE NUMBER STRING COLON LESS GREATER DOT
%token                  EXCLAM HASH TILDE DOLLAR AT EQUAL LPAREN RPAREN LCURLY RCURLY LBRACK RBRACK
%token                  QMARK COMMA SEMICOL  MACRO ENDMACRO MACRO_LPAREN MACRO_RPAREN MACRO_COMMA MACRO_IDENT
%token                  FILL FILLBYTE INCBIN PAD PADBYTE DB DW DL DD SKIP NSPACE ON OFF ORG WARNPC BASE FREESP
%token                  FREECODE FREEDATA AUTO NOASSUME BANK ERROR WARN PUSHPC PULLPC
%token                  LABELDEC
%token                  SUBLABEL
%token                  FWDLABEL BWDLABEL
%token                  LOROM HIROM NOROM SFXROM SA1ROM FSA1ROM

%right                  POWER
%left                   MULT DIV PERCENT
%left                   PLUS MINUS
%left                   LSHIFT RSHIFT
%left                   AMPER PIPE HAT
                        
%token                  ADC AND ASL BCC BLT BCS BGE BEQ BIT BMI BNE BPL BRA BRK BRL BVC BVS CLC CLD CLI CLV
%token                  CMP COP CPX CPY DEC DEX DEY EOR INC INX INY JML JMP JSL JSR LDA LDX LDY LSR MVN MVP
%token                  NOP ORA PEA PEI PER PHA PHB PHD PHK PHP PHX PHY PLA PLB PLD PLP PLX PLY REP ROL ROR
%token                  RTI RTL RTS SBC SEC SED SEI SEP STA STP STX STY STZ TAX TAY TCD TCS TDC TRB TSB TSC
%token                  TSX TXA TXS TXY TYA TYX WAI WDM XBA XCE

%token                  SPECBYTE SPECWORD SPECLONG TOC_S TOC_X TOC_Y

%token  <val_byte>      BYTE
%token  <val_word>      WORD
%token  <val_long>      LONG
%token  <val_doub>      DOUBLE
%token  <val_ident>     IDENT
                        
%%



program:        program stmt end_command
        |       program LABELDEC stmt end_command
        |       program SUBLABEL stmt end_command
        |       program LABELDEC end_command
        |       program SUBLABEL end_command
        |       program end_command
        |       ;

end_command:    NEWLINE
        |       COLON
        ;

stmt:           stmt_macro
        |       stmt_fill
        |       stmt_fillbyte
        |       stmt_incbin
        |       stmt_pad
        |       stmt_padbyte
        |       stmt_db
        |       stmt_dw
        |       stmt_dl
        |       stmt_dd
        |       stmt_skip
        |       stmt_namespace
        |       stmt_org
        |       stmt_warnpc
        |       stmt_base
        |       stmt_freespace
        |       stmt_freecode
        |       stmt_freedata
        |       stmt_warn
        |       stmt_error
        |       stmt_bank
        |       stmt_opcode
        |       stmt_pushpc
        |       stmt_pullpc
        |       stmt_norom
        |       stmt_lorom
        |       stmt_hirom
        |       stmt_sfxrom
        |       stmt_sa1rom
        |       stmt_fullsa1rom
        /* |       stmt_macro_call */
        /* |       stmt_assert */
                ;
/* stmt_asser:     ASSERT comparison */
/*         ; */

stmt_macro:     MACRO ENDMACRO 
                ;

stmt_error:     ERROR
        |       ERROR STRING
                ;

stmt_warn:      WARN
        |       WARN STRING
                ;

stmt_db:        DB expr_list
        |       DB STRING
                ;

stmt_dw:        DW expr_list
                ;

stmt_dl:        DL expr_list
                ;

stmt_dd:        DD expr_list
                ;

stmt_org:       ORG expr
                ;

stmt_base:      BASE OFF
                BASE expr
                ;

stmt_bank:      BANK AUTO
        |       BANK NOASSUME
        |       BANK expr
                ;

stmt_pushpc:   PUSHPC
        ;

stmt_pullpc:   PULLPC
        ;

stmt_warnpc:    WARNPC expr
                ;

stmt_rep:       REP expr
                ;

stmt_incbin:    INCBIN STRING
                ;


stmt_fill:      FILL /* TODO */
        ;
stmt_fillbyte:  FILLBYTE /* TODO */
        ;
stmt_freecode:  FREECODE /* TODO */
        ;
stmt_freedata:  FREEDATA /* TODO */
        ;
stmt_freespace: FREESP /* TODO */
        ;
stmt_namespace: NSPACE /* TODO */
        ;
stmt_pad:       PAD /* TODO */
        ;
stmt_padbyte:   PADBYTE /* TODO */
        ;
stmt_skip:      SKIP /* TODO */
        ;

stmt_norom:     NOROM
        ;
stmt_lorom:     LOROM
        ;
stmt_hirom:     HIROM
        ;
stmt_sfxrom:    SFXROM
        ;
stmt_sa1rom:    SA1ROM
        ;
stmt_fullsa1rom:FSA1ROM                
        ;


stmt_opcode:    stmt_adc
        |       stmt_and
        |       stmt_asl
        |       stmt_bcc
        |       stmt_bcs
        |       stmt_beq
        |       stmt_bit
        |       stmt_bmi
        |       stmt_bne
        |       stmt_bpl
        |       stmt_bra
        |       stmt_brk
        |       stmt_brl
        |       stmt_bvc
        |       stmt_bvs
        |       stmt_clc
        |       stmt_cld
        |       stmt_cli
        |       stmt_clv
        |       stmt_cmp
        |       stmt_cop
        |       stmt_cpx
        |       stmt_cpy
        |       stmt_dec
        |       stmt_dex
        |       stmt_dey
        |       stmt_eor
        |       stmt_inc
        |       stmt_inx
        |       stmt_iny
        |       stmt_jml
        |       stmt_jmp
        |       stmt_jsr
        |       stmt_jsl
        |       stmt_lda
        |       stmt_ldx
        |       stmt_ldy
        |       stmt_lsr
        |       stmt_mvn
        |       stmt_mvp
        |       stmt_nop
        |       stmt_ora
        |       stmt_pea
        |       stmt_pei
        |       stmt_per
        |       stmt_pha
        |       stmt_phb
        |       stmt_phd
        |       stmt_phk
        |       stmt_php
        |       stmt_phx
        |       stmt_phy
        |       stmt_pla
        |       stmt_plb
        |       stmt_pld
        |       stmt_plp
        |       stmt_plx
        |       stmt_ply
        |       stmt_rep
        |       stmt_rol
        |       stmt_ror
        |       stmt_rti
        |       stmt_rtl
        |       stmt_rts
        |       stmt_sbc
        |       stmt_sec
        |       stmt_sed
        |       stmt_sei
        |       stmt_sep
        |       stmt_sta
        |       stmt_stp
        |       stmt_stx
        |       stmt_sty
        |       stmt_stz
        |       stmt_tax
        |       stmt_tay
        |       stmt_tcd
        |       stmt_tcs
        |       stmt_tdc
        |       stmt_trb
        |       stmt_tsb
        |       stmt_tsc
        |       stmt_tsx
        |       stmt_txa
        |       stmt_txs
        |       stmt_txy
        |       stmt_tya
        |       stmt_tyx
        |       stmt_wai
        |       stmt_wdm
        |       stmt_xba
        |       stmt_xce
        ;

stmt_adc:
                ADC mode_gen /* byte: 65, word: 6D, long: 6F */
        |       ADC mode_genx /* byte: 75, word: 7D, long: 7F */
        |       ADC mode_idpx /* 61 */
        |       ADC mode_sr /* 63 */
        |       ADC mode_ildp /* 67 */
        |       ADC mode_const /* 69 */
        |       ADC mode_addr /* 6D */
        |       ADC mode_long /* 6F */
        |       ADC mode_idpy /* 71 */
        |       ADC mode_idp /* 72 */
        |       ADC mode_isry /* 73 */
        |       ADC mode_ildpy /* 77 */
        |       ADC mode_addry /* 79 */
        |       ADC mode_addrx /* 7D */
        |       ADC mode_longx /* 7F */
                ;


stmt_and:       AND mode_gen /* byte: 25, word: 2D, long: 2F*/
        |       AND mode_genx /* byte: 35, word: 3D, long: 3F */
        |       AND mode_idpx /* 21 */
        |       AND mode_sr /* 23 */
        |       AND mode_ildp /* 27 */
        |       AND mode_const /* 29 */
        |       AND mode_addr /* 2D */
        |       AND mode_long /* 2F */
        |       AND mode_idpy /* 31 */
        |       AND mode_idp /* 32 */
        |       AND mode_isry /* 33 */
        |       AND mode_ildpy /* 37 */
        |       AND mode_addry /* 39 */
        |       AND mode_addrx /* 3D */
        |       AND mode_longx /* 3F */
                ;

stmt_asl:       ASL mode_gen /* byte: 06, word: 0E */
        |       ASL /* 0A */
        |       ASL mode_addr /* 0E */
        |       ASL mode_genx /* byte: 16, word: 1E */
        |       ASL mode_addrx /* 1E */
                ;

stmt_bcc:       BCC mode_nearlabel /* %90	Program Counter Relative */
                ;

stmt_bcs:       BCS mode_nearlabel /* %B0	Program Counter Relative */
                ;

stmt_beq:       BEQ mode_nearlabel /* %F0	Program Counter Relative */
                ;

stmt_bit:       BIT mode_gen /* byte: 24, word: 2C */
        |       BIT mode_addr /* 2C */
        |       BIT mode_genx /* byte: 34, word: 3C */
        |       BIT mode_addrx /* 3C */
        |       BIT mode_const /* 89 */
                ;

stmt_bmi:       BMI mode_nearlabel /* 30 */
                ;

stmt_bne:       BNE mode_nearlabel /* D0 */
                ;

stmt_bpl:       BPL mode_nearlabel /* 10 */
                ;

stmt_bra:       BRA mode_nearlabel /* 80 */
                ;

stmt_brk:       BRK /* 00 */
                ;

stmt_brl:       BRL mode_label /* 82 */
                ;

stmt_bvc:       BVC mode_nearlabel /* 50 */
                ;

stmt_bvs:       BVS mode_nearlabel /* 70 */
                ;

stmt_clc:       CLC /* 18 */
                ;

stmt_cld:       CLD /* D8 */
                ;

stmt_cli:       CLI /* 58 */
                ;

stmt_clv:       CLV /* B8 */
                ;

stmt_cmp:       CMP mode_gen /* byte: C5, word: CD, long: CF*/
        |       CMP mode_idpx /* C1 */
        |       CMP mode_sr /* C3 */
        |       CMP mode_ildp /* C7 */
        |       CMP mode_const /* C9 */
        |       CMP mode_addr /* CD */
        |       CMP mode_long /* CF */
        |       CMP mode_idpy /* D1 */
        |       CMP mode_idp /* D2 */
        |       CMP mode_isry /* D3 */
        |       CMP mode_genx /* byte: D5, word: DD, long: DF */
        |       CMP mode_ildpy /* D7 */
        |       CMP mode_addry /* D9 */
        |       CMP mode_addrx /* DD */
        |       CMP mode_longx /* DF */
                ;

stmt_cop:       COP mode_const /* 02 */
                ;

stmt_cpx:       CPX mode_const /* E0 */
        |       CPX mode_gen /* byte: E4, word: EC */
        |       CPX mode_addr /* EC */
                ;

stmt_cpy:       CPY mode_const /* C0 */
        |       CPY mode_gen /* byte: C4, word: CC */
        |       CPY mode_addr /* CC */
                ;

stmt_dec:       DEC /* 3A */
        |       DEC mode_gen /* byte: C6, word: CE */
        |       DEC mode_addr /* CE */
        |       DEC mode_genx /* byte: D6, word: DE */
        |       DEC mode_addrx /* DE */
                ;

stmt_dex:       DEX /* CA */
                ;

stmt_dey:       DEY /* 88 */
                ;

stmt_eor:       EOR mode_gen /* byte: 45, word: 4D, long: 4F */
        |       EOR mode_idpx /* 41 */
        |       EOR mode_sr /* 43 */
        |       EOR mode_ildp /* 47 */
        |       EOR mode_const /* 49 */
        |       EOR mode_addr /* 4D */
        |       EOR mode_long /* 4F */
        |       EOR mode_idpy /* 51 */
        |       EOR mode_idp /* 52 */
        |       EOR mode_isry /* 53 */
        |       EOR mode_genx /* byte: 55, word: 5D, long: 5F */
        |       EOR mode_ildpy /* 57 */
        |       EOR mode_addry /* 59 */
        |       EOR mode_addrx /* 5D */
        |       EOR mode_longx /* 5F */
                ;

stmt_inc:       INC /* 1A */
        |       INC mode_gen /* byte: E6, word: EE */
        |       INC mode_addr /* EE */
        |       INC mode_genx /* byte: F6, word: FE */
        |       INC mode_addrx /* FE */
                ;

stmt_inx:       INX /* E8 */
                ;

stmt_iny:       INY /* C8 */
                ;

stmt_jml:       JML mode_gen /* long: 5C */
        |       JML mode_long /* 5C */
        |       JML mode_iladdr /* DC */
                ;

stmt_jmp:       JMP mode_gen /* 4C */
        |       JMP mode_addr /* 4C */
        |       JMP mode_iaddr  /* 6C */
        |       JMP mode_iladdr /* DC */
        |       JMP mode_iaddrx /* 7C */
                ;

stmt_jsr:       JSR mode_gen /* 20 */
        |       JSR mode_addr /* 20 */
        |       JSR mode_iaddrx /* FC */
                ;

stmt_jsl:       JSL mode_gen /* 22 */
        |       JSL mode_long /* 22 */


stmt_lda:       LDA mode_gen /* byte: A5, word: AD, long: AF*/
        |       LDA mode_idpx /* A1 */
        |       LDA mode_sr /* A3 */
        |       LDA mode_ildp /* A7 */
        |       LDA mode_const /* A9 */
        |       LDA mode_addr /* AD */
        |       LDA mode_long /* AF */
        |       LDA mode_idpy /* B1 */
        |       LDA mode_idp /* B2 */
        |       LDA mode_isry /* B3 */
        |       LDA mode_genx /* byte: B5, word: BD, long: BF */
        |       LDA mode_ildpy /* B7 */
        |       LDA mode_addry /* B9 */
        |       LDA mode_addrx /* BD */
        |       LDA mode_longx /* BF */
                ;

stmt_ldx:       LDX mode_const /* A2 */
        |       LDX mode_gen /* byte: A6, word: AE */
        |       LDX mode_addr /* AE */
        |       LDX /*dp,Y B6	 DP Indexed,Y                                    <---- ??? TODO*/
        |       LDX mode_addry /* BE */
                ;

stmt_ldy:       LDY mode_const /* A0 */
        |       LDY mode_gen /* byte: A4, word: AC */
        |       LDY mode_addr /* AC */
        |       LDY mode_genx /* byte: B4, word: BC */
        |       LDY mode_addrx /* BC */
                ;

stmt_lsr:       LSR mode_gen /* byte: 46, word: 4E */
        |       LSR /* 4A */
        |       LSR mode_addr /* 4E */
        |       LSR mode_genx /* byte: 56, word: 5E */
        |       LSR mode_addrx /* 5E */
                ;

stmt_mvn:       MVN expr COMMA expr /* srcbk,destbk %54	Block Move */
                ;

stmt_mvp:       MVP expr COMMA expr /* srcbk,destbk %44	Block Move */
                ;

stmt_nop:       NOP /* EA */
        |       NOP mode_const /*Meta-Statement EA*/
                ;

stmt_ora:       ORA mode_gen /* byte: 05, word: 0D, long: 0F */
        |       ORA mode_idpx /* 01 */
        |       ORA mode_sr /* 03 */
        |       ORA mode_ildp /* 07 */
        |       ORA mode_const /* 09 */
        |       ORA mode_addr /* 0D */
        |       ORA mode_long /* 0F */
        |       ORA mode_idpy /* 11 */
        |       ORA mode_idp /* 12 */
        |       ORA mode_isry /* 13 */
        |       ORA mode_genx /* byte: 15, word: 1D, long: 1F */
        |       ORA mode_ildpy /* 17 */
        |       ORA mode_addry /* 19 */
        |       ORA mode_addrx /* 1D */
        |       ORA mode_longx /* 1F */
                ;

stmt_pea:       PEA mode_gen  /* F4 */
        |       PEA mode_addr /* F4 */
                ;

stmt_pei:       PEI mode_idp /* %D4	Stack (mode_idp */
                ;

stmt_per:       PER mode_gen  /* 62 */
        |       PER mode_addr /* 62	Stack (PC Relative Long */
                ;

stmt_pha:       PHA /* 48 */
                ;

stmt_phb:       PHB /* 8B */
                ;

stmt_phd:       PHD /* 0B */
                ;

stmt_phk:       PHK /* 4B */
                ;

stmt_php:       PHP /* 08 */
                ;

stmt_phx:       PHX /* DA */
                ;

stmt_phy:       PHY /* 5A */
                ;

stmt_pla:       PLA /* 68 */
                ;

stmt_plb:       PLB /* AB */
                ;

stmt_pld:       PLD /* 2B */
                ;

stmt_plp:       PLP /* 28 */
                ;

stmt_plx:       PLX /* FA */
                ;

stmt_ply:       PLY /* 7A */
                ;

stmt_rep:       REP mode_const /* C2 */
                ;

stmt_rol:       ROL mode_gen /* byte: 26, word: 2E */
        |       ROL /* 2A */
        |       ROL mode_addr /* 2E */
        |       ROL mode_genx /* byte: 36, word: 3E */
        |       ROL mode_addrx /* 3E */
                ;

stmt_ror:       ROR mode_gen /* byte: 66, word: 6E */
        |       ROR /* 6A */
        |       ROR mode_addr /* 6E */
        |       ROR mode_genx /* byte: 76, word: 7E */
        |       ROR mode_addrx /* 7E */
                ;

stmt_rti:       RTI /* 40 */
                ;

stmt_rtl:       RTL /* 6B */
                ;

stmt_rts:       RTS /* 60 */
                ;

stmt_sbc:       SBC mode_gen /* byte: E5, word: ED, long: EF */
        |       SBC mode_idpx /* E1 */
        |       SBC mode_sr /* E3 */
        |       SBC mode_ildp /* E7 */
        |       SBC mode_const /* E9 */
        |       SBC mode_addr /* ED */
        |       SBC mode_long /* EF */
        |       SBC mode_idpy /* F1 */
        |       SBC mode_idp /* F2 */
        |       SBC mode_isry /* F3 */
        |       SBC mode_genx /* byte: F5, word: FD, long: FF */
        |       SBC mode_ildpy /* F7 */
        |       SBC mode_addry /* F9 */
        |       SBC mode_addrx /* FD */
        |       SBC mode_longx /* FF */
                ;


stmt_sec:       SEC /* 38 */
                ;

stmt_sed:       SED /* F8 */
                ;

stmt_sei:       SEI /* 78 */
                ;

stmt_sep:       SEP mode_const /* E2 */
                ;

stmt_sta:       STA mode_gen /* byte: 85, word: 8D, long: 8F */
        |       STA mode_idpx /* 81 */
        |       STA mode_sr /* 83 */
        |       STA mode_ildp /* 87 */
        |       STA mode_addr /* 8D */
        |       STA mode_long /* 8F */
        |       STA mode_idpy /* 91 */
        |       STA mode_idp /* 92 */
        |       STA mode_isry /* 93 */
        |       STA mode_genx /* byte: 95, word: 9D, long: 9F */
        |       STA mode_ildpy /* 97 */
        |       STA mode_addry /* 99 */
        |       STA mode_addrx /* 9D */
        |       STA mode_longx /* 9F */
                ;

stmt_stp:       STP /* DB */
                ;

stmt_stx:       STX mode_gen /* byte: 86, word: 8E */
        |       STX mode_addr /* 8E */
        |       STX mode_dpy /* 96 */
                ;

stmt_sty:       STY mode_gen /* byte: 84, word: 8C */
        |       STY mode_addr /* 8C */
        |       STY mode_genx /* 94 */
                ;

stmt_stz:       STZ mode_gen /* byte: 64, word: 8C */
        |       STZ mode_genx /* byte: 74, word: 9E */
        |       STZ mode_addr /* 9C */
        |       STZ mode_addrx /* 9E */
                ;

stmt_tax:       TAX /* AA */
                ;

stmt_tay:       TAY /* A8 */
                ;

stmt_tcd:       TCD /* 5B */
                ;

stmt_tcs:       TCS /* 1B */
                ;

stmt_tdc:       TDC /* 7B */
                ;

stmt_trb:       TRB mode_gen /* byte: 14, word: 1C */
        |       TRB mode_addr /* 1C */
                ;

stmt_tsb:       TSB mode_gen /* byte: 04, word: 0C */
        |       TSB mode_addr /* 0C */
                ;

stmt_tsc:       TSC /* 3B */
                ;

stmt_tsx:       TSX /* BA */
                ;

stmt_txa:       TXA /* 8A */
                ;

stmt_txs:       TXS /* 9A */
                ;

stmt_txy:       TXY /* 9B */
                ;

stmt_tya:       TYA /* 98 */
                ;

stmt_tyx:       TYX /* BB */
                ;

stmt_wai:       WAI /* CB */
                ;

stmt_wdm:       WDM /* 42 */
                ;

stmt_xba:       XBA /* EB */
                ;

stmt_xce:       XCE /* FB */
                ;


/* mode_imm:       HASH DOLLAR NUMBER //#$2424 */
/*                 ; */

mode_gen:       expr 
                ;
mode_genx:      expr COMMA TOC_X
                ;
mode_idp:       LPAREN expr RPAREN
                ; 
mode_idpx:      LPAREN expr COMMA TOC_X RPAREN
                ;
mode_idpy:      LPAREN expr COMMA TOC_Y RPAREN
                ;
mode_ildp:      LBRACK expr RBRACK
                ;
mode_ildpy:     LBRACK expr RBRACK COMMA TOC_Y
                ;
mode_isry:      LPAREN expr COMMA TOC_S RPAREN COMMA TOC_Y
                ;
mode_dpy:       expr COMMA TOC_Y
                ;
mode_sr:        expr COMMA TOC_S
                ;
mode_iladdr:    LBRACK expr RBRACK
/*              mode_iladdr:    LBRACK long RBRACK*/
        ;
mode_iaddr:     LPAREN expr RPAREN
        ;
mode_iaddrx:    LPAREN expr COMMA TOC_X RPAREN 
        ;
mode_addr:      SPECBYTE expr
        |       SPECWORD expr
        ;

mode_addrx:     SPECBYTE expr COMMA TOC_X
        |       SPECWORD expr COMMA TOC_X
        ;

mode_addry:     expr COMMA TOC_Y
        |       SPECBYTE expr COMMA TOC_Y
        |       SPECWORD expr COMMA TOC_Y
        ;

mode_long:      SPECLONG expr
        ;

mode_longx:     SPECLONG expr COMMA TOC_X
        ;

mode_const:     HASH expr
        |       SPECBYTE HASH expr
        |       SPECWORD HASH expr
        ;



expr_list:      expr
        |       expr COMMA expr_list
                ;

expr:           BYTE
        |       WORD
        |       LONG
        |       DOUBLE
        |       NUMBER
        |       expr2 PLUS expr2
        |       expr2 MINUS expr2
        |       expr2 MULT expr2
        |       expr2 DIV expr2
        |       expr2 POWER expr2
        |       expr2 LSHIFT expr2
        |       expr2 RSHIFT expr2
        |       expr2 PERCENT expr2
        |       expr2 AMPER expr2
        |       expr2 PIPE expr2
        |       expr2 HAT expr2
        |       EXCLAM IDENT
        |       SUBLABEL
        |       IDENT
        ;

expr2:          BYTE
        |       WORD
        |       LONG
        |       DOUBLE
        |       NUMBER
        |       expr2 PLUS expr2
        |       expr2 MINUS expr2
        |       expr2 MULT expr2
        |       expr2 DIV expr2
        |       expr2 POWER expr2
        |       expr2 LSHIFT expr2
        |       expr2 RSHIFT expr2
        |       expr2 PERCENT expr2
        |       expr2 AMPER expr2
        |       expr2 PIPE expr2
        |       expr2 HAT expr2
        |       LPAREN expr RPAREN
        |       EXCLAM IDENT
        |       SUBLABEL
        |       IDENT
        ;

mode_label:     IDENT
        |       SUBLABEL
        ;
mode_nearlabel: SUBLABEL
        |       IDENT
        ;

%%
