#include "nuts.h"

NutsOpcodeAssembleData opcode_assemble_data_table[][PARSE_MODE_MAX][4] = {
    [ OPCODE_ADC ][ PARSE_MODE_DIRECT          ][1] = { .byte = 0x65, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_ADC ][ PARSE_MODE_DIRECT          ][2] = { .byte = 0x6D, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_ADC ][ PARSE_MODE_DIRECT          ][3] = { .byte = 0x6F, .mode = ASSEMBLE_MODE_3 },
    [ OPCODE_ADC ][ PARSE_MODE_DIRECT_X        ][1] = { .byte = 0x75, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_ADC ][ PARSE_MODE_DIRECT_X        ][2] = { .byte = 0x7D, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_ADC ][ PARSE_MODE_DIRECT_X        ][3] = { .byte = 0x7F, .mode = ASSEMBLE_MODE_3 },
    [ OPCODE_ADC ][ PARSE_MODE_DIRECT_Y        ][2] = { .byte = 0x79, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_ADC ][ PARSE_MODE_DIRECT_S        ][1] = { .byte = 0x63, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_ADC ][ PARSE_MODE_IMMEDIATE       ][0] = { .byte = 0x69, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_ADC ][ PARSE_MODE_IMMEDIATE       ][1] = { .byte = 0x69, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_ADC ][ PARSE_MODE_IMMEDIATE       ][2] = { .byte = 0x69, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_ADC ][ PARSE_MODE_IMMEDIATE       ][3] = { .byte = 0x69, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_ADC ][ PARSE_MODE_INDIRECT        ][1] = { .byte = 0x72, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_ADC ][ PARSE_MODE_X_INDIRECT      ][1] = { .byte = 0x61, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_ADC ][ PARSE_MODE_INDIRECT_Y      ][1] = { .byte = 0x71, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_ADC ][ PARSE_MODE_S_INDIRECT_Y    ][1] = { .byte = 0x73, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_ADC ][ PARSE_MODE_LONG_INDIRECT   ][1] = { .byte = 0x67, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_ADC ][ PARSE_MODE_LONG_INDIRECT_Y ][1] = { .byte = 0x77, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_AND ][ PARSE_MODE_DIRECT          ][1] = { .byte = 0x25, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_AND ][ PARSE_MODE_DIRECT          ][2] = { .byte = 0x2D, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_AND ][ PARSE_MODE_DIRECT          ][3] = { .byte = 0x2F, .mode = ASSEMBLE_MODE_3 },
    [ OPCODE_AND ][ PARSE_MODE_DIRECT_X        ][1] = { .byte = 0x35, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_AND ][ PARSE_MODE_DIRECT_X        ][2] = { .byte = 0x3D, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_AND ][ PARSE_MODE_DIRECT_X        ][3] = { .byte = 0x3F, .mode = ASSEMBLE_MODE_3 },
    [ OPCODE_AND ][ PARSE_MODE_DIRECT_Y        ][2] = { .byte = 0x39, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_AND ][ PARSE_MODE_DIRECT_S        ][1] = { .byte = 0x23, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_AND ][ PARSE_MODE_IMMEDIATE       ][0] = { .byte = 0x29, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_AND ][ PARSE_MODE_IMMEDIATE       ][1] = { .byte = 0x29, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_AND ][ PARSE_MODE_IMMEDIATE       ][2] = { .byte = 0x29, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_AND ][ PARSE_MODE_IMMEDIATE       ][3] = { .byte = 0x29, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_AND ][ PARSE_MODE_INDIRECT        ][1] = { .byte = 0x32, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_AND ][ PARSE_MODE_X_INDIRECT      ][1] = { .byte = 0x21, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_AND ][ PARSE_MODE_INDIRECT_Y      ][1] = { .byte = 0x31, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_AND ][ PARSE_MODE_S_INDIRECT_Y    ][1] = { .byte = 0x33, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_AND ][ PARSE_MODE_LONG_INDIRECT   ][1] = { .byte = 0x27, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_AND ][ PARSE_MODE_LONG_INDIRECT_Y ][1] = { .byte = 0x37, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_ASL ][ PARSE_MODE_DIRECT          ][1] = { .byte = 0x06, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_ASL ][ PARSE_MODE_DIRECT          ][2] = { .byte = 0x0E, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_ASL ][ PARSE_MODE_DIRECT_X        ][1] = { .byte = 0x16, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_ASL ][ PARSE_MODE_DIRECT_X        ][2] = { .byte = 0x1E, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_ASL ][ PARSE_MODE_IMMEDIATE       ][0] = { .byte = 0x0A, .mode = ASSEMBLE_MODE_R },
    [ OPCODE_ASL ][ PARSE_MODE_IMMEDIATE       ][1] = { .byte = 0x0A, .mode = ASSEMBLE_MODE_R },
    [ OPCODE_ASL ][ PARSE_MODE_IMMEDIATE       ][2] = { .byte = 0x0A, .mode = ASSEMBLE_MODE_R },
    [ OPCODE_ASL ][ PARSE_MODE_IMMEDIATE       ][3] = { .byte = 0x0A, .mode = ASSEMBLE_MODE_R },
    [ OPCODE_ASL ][ PARSE_MODE_IMPLIED         ][0] = { .byte = 0x0A, .mode = ASSEMBLE_MODE_0 },
    [ OPCODE_ASL ][ PARSE_MODE_ACCUMULATOR     ][0] = { .byte = 0x0A, .mode = ASSEMBLE_MODE_0 },
    [ OPCODE_BCC ][ PARSE_MODE_DIRECT          ][0] = { .byte = 0x90, .mode = ASSEMBLE_MODE_J },
    [ OPCODE_BCC ][ PARSE_MODE_DIRECT          ][1] = { .byte = 0x90, .mode = ASSEMBLE_MODE_J },
    [ OPCODE_BCC ][ PARSE_MODE_DIRECT          ][2] = { .byte = 0x90, .mode = ASSEMBLE_MODE_J },
    [ OPCODE_BCC ][ PARSE_MODE_DIRECT          ][3] = { .byte = 0x90, .mode = ASSEMBLE_MODE_J },
    [ OPCODE_BCS ][ PARSE_MODE_DIRECT          ][0] = { .byte = 0xB0, .mode = ASSEMBLE_MODE_J },
    [ OPCODE_BCS ][ PARSE_MODE_DIRECT          ][1] = { .byte = 0xB0, .mode = ASSEMBLE_MODE_J },
    [ OPCODE_BCS ][ PARSE_MODE_DIRECT          ][2] = { .byte = 0xB0, .mode = ASSEMBLE_MODE_J },
    [ OPCODE_BCS ][ PARSE_MODE_DIRECT          ][3] = { .byte = 0xB0, .mode = ASSEMBLE_MODE_J },
    [ OPCODE_BEQ ][ PARSE_MODE_DIRECT          ][0] = { .byte = 0xF0, .mode = ASSEMBLE_MODE_J },
    [ OPCODE_BEQ ][ PARSE_MODE_DIRECT          ][1] = { .byte = 0xF0, .mode = ASSEMBLE_MODE_J },
    [ OPCODE_BEQ ][ PARSE_MODE_DIRECT          ][2] = { .byte = 0xF0, .mode = ASSEMBLE_MODE_J },
    [ OPCODE_BEQ ][ PARSE_MODE_DIRECT          ][3] = { .byte = 0xF0, .mode = ASSEMBLE_MODE_J },
    [ OPCODE_BIT ][ PARSE_MODE_DIRECT          ][1] = { .byte = 0x24, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_BIT ][ PARSE_MODE_DIRECT          ][2] = { .byte = 0x2C, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_BIT ][ PARSE_MODE_DIRECT_X        ][1] = { .byte = 0x34, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_BIT ][ PARSE_MODE_DIRECT_X        ][2] = { .byte = 0x3C, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_BIT ][ PARSE_MODE_IMMEDIATE       ][0] = { .byte = 0x89, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_BIT ][ PARSE_MODE_IMMEDIATE       ][1] = { .byte = 0x89, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_BIT ][ PARSE_MODE_IMMEDIATE       ][2] = { .byte = 0x89, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_BIT ][ PARSE_MODE_IMMEDIATE       ][3] = { .byte = 0x89, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_BMI ][ PARSE_MODE_DIRECT          ][0] = { .byte = 0x30, .mode = ASSEMBLE_MODE_J },
    [ OPCODE_BMI ][ PARSE_MODE_DIRECT          ][1] = { .byte = 0x30, .mode = ASSEMBLE_MODE_J },
    [ OPCODE_BMI ][ PARSE_MODE_DIRECT          ][2] = { .byte = 0x30, .mode = ASSEMBLE_MODE_J },
    [ OPCODE_BMI ][ PARSE_MODE_DIRECT          ][3] = { .byte = 0x30, .mode = ASSEMBLE_MODE_J },
    [ OPCODE_BNE ][ PARSE_MODE_DIRECT          ][0] = { .byte = 0xD0, .mode = ASSEMBLE_MODE_J },
    [ OPCODE_BNE ][ PARSE_MODE_DIRECT          ][1] = { .byte = 0xD0, .mode = ASSEMBLE_MODE_J },
    [ OPCODE_BNE ][ PARSE_MODE_DIRECT          ][2] = { .byte = 0xD0, .mode = ASSEMBLE_MODE_J },
    [ OPCODE_BNE ][ PARSE_MODE_DIRECT          ][3] = { .byte = 0xD0, .mode = ASSEMBLE_MODE_J },
    [ OPCODE_BPL ][ PARSE_MODE_DIRECT          ][0] = { .byte = 0x10, .mode = ASSEMBLE_MODE_J },
    [ OPCODE_BPL ][ PARSE_MODE_DIRECT          ][1] = { .byte = 0x10, .mode = ASSEMBLE_MODE_J },
    [ OPCODE_BPL ][ PARSE_MODE_DIRECT          ][2] = { .byte = 0x10, .mode = ASSEMBLE_MODE_J },
    [ OPCODE_BPL ][ PARSE_MODE_DIRECT          ][3] = { .byte = 0x10, .mode = ASSEMBLE_MODE_J },
    [ OPCODE_BRA ][ PARSE_MODE_DIRECT          ][0] = { .byte = 0x80, .mode = ASSEMBLE_MODE_J },
    [ OPCODE_BRA ][ PARSE_MODE_DIRECT          ][1] = { .byte = 0x80, .mode = ASSEMBLE_MODE_J },
    [ OPCODE_BRA ][ PARSE_MODE_DIRECT          ][2] = { .byte = 0x80, .mode = ASSEMBLE_MODE_J },
    [ OPCODE_BRA ][ PARSE_MODE_DIRECT          ][3] = { .byte = 0x80, .mode = ASSEMBLE_MODE_J },
    [ OPCODE_BRK ][ PARSE_MODE_IMMEDIATE       ][1] = { .byte = 0x00, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_BVC ][ PARSE_MODE_DIRECT          ][0] = { .byte = 0x50, .mode = ASSEMBLE_MODE_J },
    [ OPCODE_BVC ][ PARSE_MODE_DIRECT          ][1] = { .byte = 0x50, .mode = ASSEMBLE_MODE_J },
    [ OPCODE_BVC ][ PARSE_MODE_DIRECT          ][2] = { .byte = 0x50, .mode = ASSEMBLE_MODE_J },
    [ OPCODE_BVC ][ PARSE_MODE_DIRECT          ][3] = { .byte = 0x50, .mode = ASSEMBLE_MODE_J },
    [ OPCODE_BVS ][ PARSE_MODE_DIRECT          ][0] = { .byte = 0x70, .mode = ASSEMBLE_MODE_J },
    [ OPCODE_BVS ][ PARSE_MODE_DIRECT          ][1] = { .byte = 0x70, .mode = ASSEMBLE_MODE_J },
    [ OPCODE_BVS ][ PARSE_MODE_DIRECT          ][2] = { .byte = 0x70, .mode = ASSEMBLE_MODE_J },
    [ OPCODE_BVS ][ PARSE_MODE_DIRECT          ][3] = { .byte = 0x70, .mode = ASSEMBLE_MODE_J },
    [ OPCODE_BRL ][ PARSE_MODE_DIRECT          ][0] = { .byte = 0x82, .mode = ASSEMBLE_MODE_K },
    [ OPCODE_BRL ][ PARSE_MODE_DIRECT          ][1] = { .byte = 0x82, .mode = ASSEMBLE_MODE_K },
    [ OPCODE_BRL ][ PARSE_MODE_DIRECT          ][2] = { .byte = 0x82, .mode = ASSEMBLE_MODE_K },
    [ OPCODE_BRL ][ PARSE_MODE_DIRECT          ][3] = { .byte = 0x82, .mode = ASSEMBLE_MODE_K },
    [ OPCODE_CLC ][ PARSE_MODE_IMPLIED         ][0] = { .byte = 0x18, .mode = ASSEMBLE_MODE_0 },
    [ OPCODE_CLD ][ PARSE_MODE_IMPLIED         ][0] = { .byte = 0xD8, .mode = ASSEMBLE_MODE_0 },
    [ OPCODE_CLI ][ PARSE_MODE_IMPLIED         ][0] = { .byte = 0x58, .mode = ASSEMBLE_MODE_0 },
    [ OPCODE_CLV ][ PARSE_MODE_IMPLIED         ][0] = { .byte = 0xB8, .mode = ASSEMBLE_MODE_0 },
    [ OPCODE_CMP ][ PARSE_MODE_DIRECT          ][1] = { .byte = 0xC5, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_CMP ][ PARSE_MODE_DIRECT          ][2] = { .byte = 0xCD, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_CMP ][ PARSE_MODE_DIRECT          ][3] = { .byte = 0xCF, .mode = ASSEMBLE_MODE_3 },
    [ OPCODE_CMP ][ PARSE_MODE_DIRECT_X        ][1] = { .byte = 0xD5, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_CMP ][ PARSE_MODE_DIRECT_X        ][2] = { .byte = 0xDD, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_CMP ][ PARSE_MODE_DIRECT_X        ][3] = { .byte = 0xDF, .mode = ASSEMBLE_MODE_3 },
    [ OPCODE_CMP ][ PARSE_MODE_DIRECT_Y        ][2] = { .byte = 0xD9, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_CMP ][ PARSE_MODE_DIRECT_S        ][1] = { .byte = 0xC3, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_CMP ][ PARSE_MODE_IMMEDIATE       ][0] = { .byte = 0xC9, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_CMP ][ PARSE_MODE_IMMEDIATE       ][1] = { .byte = 0xC9, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_CMP ][ PARSE_MODE_IMMEDIATE       ][2] = { .byte = 0xC9, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_CMP ][ PARSE_MODE_IMMEDIATE       ][3] = { .byte = 0xC9, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_CMP ][ PARSE_MODE_INDIRECT        ][1] = { .byte = 0xD2, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_CMP ][ PARSE_MODE_X_INDIRECT      ][1] = { .byte = 0xC1, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_CMP ][ PARSE_MODE_INDIRECT_Y      ][1] = { .byte = 0xD1, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_CMP ][ PARSE_MODE_S_INDIRECT_Y    ][1] = { .byte = 0xD3, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_CMP ][ PARSE_MODE_LONG_INDIRECT   ][1] = { .byte = 0xC7, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_CMP ][ PARSE_MODE_LONG_INDIRECT_Y ][1] = { .byte = 0xD7, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_CPX ][ PARSE_MODE_DIRECT          ][1] = { .byte = 0xE4, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_CPX ][ PARSE_MODE_DIRECT          ][2] = { .byte = 0xEC, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_CPX ][ PARSE_MODE_IMMEDIATE       ][0] = { .byte = 0xE0, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_CPX ][ PARSE_MODE_IMMEDIATE       ][1] = { .byte = 0xE0, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_CPX ][ PARSE_MODE_IMMEDIATE       ][2] = { .byte = 0xE0, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_CPX ][ PARSE_MODE_IMMEDIATE       ][3] = { .byte = 0xE0, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_CPY ][ PARSE_MODE_DIRECT          ][1] = { .byte = 0xC4, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_CPY ][ PARSE_MODE_DIRECT          ][2] = { .byte = 0xCC, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_CPY ][ PARSE_MODE_IMMEDIATE       ][0] = { .byte = 0xC0, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_CPY ][ PARSE_MODE_IMMEDIATE       ][1] = { .byte = 0xC0, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_CPY ][ PARSE_MODE_IMMEDIATE       ][2] = { .byte = 0xC0, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_CPY ][ PARSE_MODE_IMMEDIATE       ][3] = { .byte = 0xC0, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_COP ][ PARSE_MODE_IMMEDIATE       ][1] = { .byte = 0x02, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_DEC ][ PARSE_MODE_DIRECT          ][1] = { .byte = 0xC6, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_DEC ][ PARSE_MODE_DIRECT          ][2] = { .byte = 0xCE, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_DEC ][ PARSE_MODE_DIRECT_X        ][1] = { .byte = 0xD6, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_DEC ][ PARSE_MODE_DIRECT_X        ][2] = { .byte = 0xDE, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_DEC ][ PARSE_MODE_IMMEDIATE       ][0] = { .byte = 0x3A, .mode = ASSEMBLE_MODE_R },
    [ OPCODE_DEC ][ PARSE_MODE_IMMEDIATE       ][1] = { .byte = 0x3A, .mode = ASSEMBLE_MODE_R },
    [ OPCODE_DEC ][ PARSE_MODE_IMMEDIATE       ][2] = { .byte = 0x3A, .mode = ASSEMBLE_MODE_R },
    [ OPCODE_DEC ][ PARSE_MODE_IMMEDIATE       ][3] = { .byte = 0x3A, .mode = ASSEMBLE_MODE_R },
    [ OPCODE_DEC ][ PARSE_MODE_IMPLIED         ][0] = { .byte = 0x3A, .mode = ASSEMBLE_MODE_0 },
    [ OPCODE_DEC ][ PARSE_MODE_ACCUMULATOR     ][0] = { .byte = 0x3A, .mode = ASSEMBLE_MODE_0 },
    [ OPCODE_DEX ][ PARSE_MODE_IMMEDIATE       ][0] = { .byte = 0xCA, .mode = ASSEMBLE_MODE_R },
    [ OPCODE_DEX ][ PARSE_MODE_IMMEDIATE       ][1] = { .byte = 0xCA, .mode = ASSEMBLE_MODE_R },
    [ OPCODE_DEX ][ PARSE_MODE_IMMEDIATE       ][2] = { .byte = 0xCA, .mode = ASSEMBLE_MODE_R },
    [ OPCODE_DEX ][ PARSE_MODE_IMMEDIATE       ][3] = { .byte = 0xCA, .mode = ASSEMBLE_MODE_R },
    [ OPCODE_DEX ][ PARSE_MODE_IMPLIED         ][0] = { .byte = 0xCA, .mode = ASSEMBLE_MODE_0 },
    [ OPCODE_DEY ][ PARSE_MODE_IMMEDIATE       ][0] = { .byte = 0x88, .mode = ASSEMBLE_MODE_R },
    [ OPCODE_DEY ][ PARSE_MODE_IMMEDIATE       ][1] = { .byte = 0x88, .mode = ASSEMBLE_MODE_R },
    [ OPCODE_DEY ][ PARSE_MODE_IMMEDIATE       ][2] = { .byte = 0x88, .mode = ASSEMBLE_MODE_R },
    [ OPCODE_DEY ][ PARSE_MODE_IMMEDIATE       ][3] = { .byte = 0x88, .mode = ASSEMBLE_MODE_R },
    [ OPCODE_DEY ][ PARSE_MODE_IMPLIED         ][0] = { .byte = 0x88, .mode = ASSEMBLE_MODE_0 },
    [ OPCODE_EOR ][ PARSE_MODE_DIRECT          ][1] = { .byte = 0x45, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_EOR ][ PARSE_MODE_DIRECT          ][2] = { .byte = 0x4D, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_EOR ][ PARSE_MODE_DIRECT          ][3] = { .byte = 0x4F, .mode = ASSEMBLE_MODE_3 },
    [ OPCODE_EOR ][ PARSE_MODE_DIRECT_X        ][1] = { .byte = 0x55, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_EOR ][ PARSE_MODE_DIRECT_X        ][2] = { .byte = 0x5D, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_EOR ][ PARSE_MODE_DIRECT_X        ][3] = { .byte = 0x5F, .mode = ASSEMBLE_MODE_3 },
    [ OPCODE_EOR ][ PARSE_MODE_DIRECT_Y        ][2] = { .byte = 0x59, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_EOR ][ PARSE_MODE_DIRECT_S        ][1] = { .byte = 0x43, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_EOR ][ PARSE_MODE_IMMEDIATE       ][0] = { .byte = 0x49, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_EOR ][ PARSE_MODE_IMMEDIATE       ][1] = { .byte = 0x49, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_EOR ][ PARSE_MODE_IMMEDIATE       ][2] = { .byte = 0x49, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_EOR ][ PARSE_MODE_IMMEDIATE       ][3] = { .byte = 0x49, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_EOR ][ PARSE_MODE_INDIRECT        ][1] = { .byte = 0x52, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_EOR ][ PARSE_MODE_X_INDIRECT      ][1] = { .byte = 0x41, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_EOR ][ PARSE_MODE_INDIRECT_Y      ][1] = { .byte = 0x51, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_EOR ][ PARSE_MODE_S_INDIRECT_Y    ][1] = { .byte = 0x53, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_EOR ][ PARSE_MODE_LONG_INDIRECT   ][1] = { .byte = 0x47, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_EOR ][ PARSE_MODE_LONG_INDIRECT_Y ][1] = { .byte = 0x57, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_INC ][ PARSE_MODE_DIRECT          ][1] = { .byte = 0xE6, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_INC ][ PARSE_MODE_DIRECT          ][2] = { .byte = 0xEE, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_INC ][ PARSE_MODE_DIRECT_X        ][1] = { .byte = 0xF6, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_INC ][ PARSE_MODE_DIRECT_X        ][2] = { .byte = 0xFE, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_INC ][ PARSE_MODE_IMMEDIATE       ][0] = { .byte = 0x1A, .mode = ASSEMBLE_MODE_R },
    [ OPCODE_INC ][ PARSE_MODE_IMMEDIATE       ][1] = { .byte = 0x1A, .mode = ASSEMBLE_MODE_R },
    [ OPCODE_INC ][ PARSE_MODE_IMMEDIATE       ][2] = { .byte = 0x1A, .mode = ASSEMBLE_MODE_R },
    [ OPCODE_INC ][ PARSE_MODE_IMMEDIATE       ][3] = { .byte = 0x1A, .mode = ASSEMBLE_MODE_R },
    [ OPCODE_INC ][ PARSE_MODE_IMPLIED         ][0] = { .byte = 0x1A, .mode = ASSEMBLE_MODE_0 },
    [ OPCODE_INC ][ PARSE_MODE_ACCUMULATOR     ][0] = { .byte = 0x1A, .mode = ASSEMBLE_MODE_0 },
    [ OPCODE_INX ][ PARSE_MODE_IMMEDIATE       ][0] = { .byte = 0xE8, .mode = ASSEMBLE_MODE_R },
    [ OPCODE_INX ][ PARSE_MODE_IMMEDIATE       ][1] = { .byte = 0xE8, .mode = ASSEMBLE_MODE_R },
    [ OPCODE_INX ][ PARSE_MODE_IMMEDIATE       ][2] = { .byte = 0xE8, .mode = ASSEMBLE_MODE_R },
    [ OPCODE_INX ][ PARSE_MODE_IMMEDIATE       ][3] = { .byte = 0xE8, .mode = ASSEMBLE_MODE_R },
    [ OPCODE_INX ][ PARSE_MODE_IMPLIED         ][0] = { .byte = 0xE8, .mode = ASSEMBLE_MODE_0 },
    [ OPCODE_INY ][ PARSE_MODE_IMMEDIATE       ][0] = { .byte = 0xC8, .mode = ASSEMBLE_MODE_R },
    [ OPCODE_INY ][ PARSE_MODE_IMMEDIATE       ][1] = { .byte = 0xC8, .mode = ASSEMBLE_MODE_R },
    [ OPCODE_INY ][ PARSE_MODE_IMMEDIATE       ][2] = { .byte = 0xC8, .mode = ASSEMBLE_MODE_R },
    [ OPCODE_INY ][ PARSE_MODE_IMMEDIATE       ][3] = { .byte = 0xC8, .mode = ASSEMBLE_MODE_R },
    [ OPCODE_INY ][ PARSE_MODE_IMPLIED         ][0] = { .byte = 0xC8, .mode = ASSEMBLE_MODE_0 },
    [ OPCODE_JMP ][ PARSE_MODE_DIRECT          ][2] = { .byte = 0x4C, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_JMP ][ PARSE_MODE_DIRECT          ][3] = { .byte = 0x4C, .mode = ASSEMBLE_MODE_X },
    [ OPCODE_JMP ][ PARSE_MODE_INDIRECT        ][2] = { .byte = 0x6C, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_JMP ][ PARSE_MODE_X_INDIRECT      ][2] = { .byte = 0x7C, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_JMP ][ PARSE_MODE_LONG_INDIRECT   ][2] = { .byte = 0xDC, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_JML ][ PARSE_MODE_DIRECT          ][2] = { .byte = 0x5C, .mode = ASSEMBLE_MODE_3 },
    [ OPCODE_JML ][ PARSE_MODE_DIRECT          ][3] = { .byte = 0x5C, .mode = ASSEMBLE_MODE_3 },
    [ OPCODE_JML ][ PARSE_MODE_LONG_INDIRECT   ][2] = { .byte = 0xDC, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_JSR ][ PARSE_MODE_DIRECT          ][2] = { .byte = 0x20, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_JSR ][ PARSE_MODE_DIRECT          ][3] = { .byte = 0x20, .mode = ASSEMBLE_MODE_X },
    [ OPCODE_JSR ][ PARSE_MODE_X_INDIRECT      ][2] = { .byte = 0xFC, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_JSL ][ PARSE_MODE_DIRECT          ][2] = { .byte = 0x22, .mode = ASSEMBLE_MODE_3 },
    [ OPCODE_JSL ][ PARSE_MODE_DIRECT          ][3] = { .byte = 0x22, .mode = ASSEMBLE_MODE_3 },
    [ OPCODE_LDA ][ PARSE_MODE_DIRECT          ][1] = { .byte = 0xA5, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_LDA ][ PARSE_MODE_DIRECT          ][2] = { .byte = 0xAD, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_LDA ][ PARSE_MODE_DIRECT          ][3] = { .byte = 0xAF, .mode = ASSEMBLE_MODE_3 },
    [ OPCODE_LDA ][ PARSE_MODE_DIRECT_X        ][1] = { .byte = 0xB5, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_LDA ][ PARSE_MODE_DIRECT_X        ][2] = { .byte = 0xBD, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_LDA ][ PARSE_MODE_DIRECT_X        ][3] = { .byte = 0xBF, .mode = ASSEMBLE_MODE_3 },
    [ OPCODE_LDA ][ PARSE_MODE_DIRECT_Y        ][2] = { .byte = 0xB9, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_LDA ][ PARSE_MODE_DIRECT_S        ][1] = { .byte = 0xA3, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_LDA ][ PARSE_MODE_IMMEDIATE       ][0] = { .byte = 0xA9, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_LDA ][ PARSE_MODE_IMMEDIATE       ][1] = { .byte = 0xA9, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_LDA ][ PARSE_MODE_IMMEDIATE       ][2] = { .byte = 0xA9, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_LDA ][ PARSE_MODE_IMMEDIATE       ][3] = { .byte = 0xA9, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_LDA ][ PARSE_MODE_INDIRECT        ][1] = { .byte = 0xB2, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_LDA ][ PARSE_MODE_X_INDIRECT      ][1] = { .byte = 0xA1, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_LDA ][ PARSE_MODE_INDIRECT_Y      ][1] = { .byte = 0xB1, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_LDA ][ PARSE_MODE_S_INDIRECT_Y    ][1] = { .byte = 0xB3, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_LDA ][ PARSE_MODE_LONG_INDIRECT   ][1] = { .byte = 0xA7, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_LDA ][ PARSE_MODE_LONG_INDIRECT_Y ][1] = { .byte = 0xB7, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_LDX ][ PARSE_MODE_DIRECT          ][1] = { .byte = 0xA6, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_LDX ][ PARSE_MODE_DIRECT          ][2] = { .byte = 0xAE, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_LDX ][ PARSE_MODE_DIRECT_Y        ][1] = { .byte = 0xB6, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_LDX ][ PARSE_MODE_DIRECT_Y        ][2] = { .byte = 0xBE, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_LDX ][ PARSE_MODE_IMMEDIATE       ][0] = { .byte = 0xA2, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_LDX ][ PARSE_MODE_IMMEDIATE       ][1] = { .byte = 0xA2, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_LDX ][ PARSE_MODE_IMMEDIATE       ][2] = { .byte = 0xA2, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_LDX ][ PARSE_MODE_IMMEDIATE       ][3] = { .byte = 0xA2, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_LDY ][ PARSE_MODE_DIRECT          ][1] = { .byte = 0xA4, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_LDY ][ PARSE_MODE_DIRECT          ][2] = { .byte = 0xAC, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_LDY ][ PARSE_MODE_DIRECT_X        ][1] = { .byte = 0xB4, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_LDY ][ PARSE_MODE_DIRECT_X        ][2] = { .byte = 0xBC, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_LDY ][ PARSE_MODE_IMMEDIATE       ][0] = { .byte = 0xA0, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_LDY ][ PARSE_MODE_IMMEDIATE       ][1] = { .byte = 0xA0, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_LDY ][ PARSE_MODE_IMMEDIATE       ][2] = { .byte = 0xA0, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_LDY ][ PARSE_MODE_IMMEDIATE       ][3] = { .byte = 0xA0, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_LSR ][ PARSE_MODE_DIRECT          ][1] = { .byte = 0x46, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_LSR ][ PARSE_MODE_DIRECT          ][2] = { .byte = 0x4E, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_LSR ][ PARSE_MODE_DIRECT_X        ][1] = { .byte = 0x56, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_LSR ][ PARSE_MODE_DIRECT_X        ][2] = { .byte = 0x5E, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_LSR ][ PARSE_MODE_IMMEDIATE       ][0] = { .byte = 0x4A, .mode = ASSEMBLE_MODE_R },
    [ OPCODE_LSR ][ PARSE_MODE_IMMEDIATE       ][1] = { .byte = 0x4A, .mode = ASSEMBLE_MODE_R },
    [ OPCODE_LSR ][ PARSE_MODE_IMMEDIATE       ][2] = { .byte = 0x4A, .mode = ASSEMBLE_MODE_R },
    [ OPCODE_LSR ][ PARSE_MODE_IMMEDIATE       ][3] = { .byte = 0x4A, .mode = ASSEMBLE_MODE_R },
    [ OPCODE_LSR ][ PARSE_MODE_IMPLIED         ][0] = { .byte = 0x4A, .mode = ASSEMBLE_MODE_0 },
    [ OPCODE_LSR ][ PARSE_MODE_ACCUMULATOR     ][0] = { .byte = 0x4A, .mode = ASSEMBLE_MODE_0 },
    [ OPCODE_MVP ][ PARSE_MODE_DIRECT          ][2] = { .byte = 0x44, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_MVP ][ PARSE_MODE_SRC_DEST        ][0] = { .byte = 0x44, .mode = ASSEMBLE_MODE_S },
    [ OPCODE_MVP ][ PARSE_MODE_SRC_DEST        ][1] = { .byte = 0x44, .mode = ASSEMBLE_MODE_S },
    [ OPCODE_MVP ][ PARSE_MODE_SRC_DEST        ][2] = { .byte = 0x44, .mode = ASSEMBLE_MODE_S },
    [ OPCODE_MVP ][ PARSE_MODE_SRC_DEST        ][3] = { .byte = 0x44, .mode = ASSEMBLE_MODE_S },
    [ OPCODE_MVN ][ PARSE_MODE_DIRECT          ][2] = { .byte = 0x54, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_MVN ][ PARSE_MODE_SRC_DEST        ][0] = { .byte = 0x54, .mode = ASSEMBLE_MODE_S },
    [ OPCODE_MVN ][ PARSE_MODE_SRC_DEST        ][1] = { .byte = 0x54, .mode = ASSEMBLE_MODE_S },
    [ OPCODE_MVN ][ PARSE_MODE_SRC_DEST        ][2] = { .byte = 0x54, .mode = ASSEMBLE_MODE_S },
    [ OPCODE_MVN ][ PARSE_MODE_SRC_DEST        ][3] = { .byte = 0x54, .mode = ASSEMBLE_MODE_S },
    [ OPCODE_NOP ][ PARSE_MODE_IMMEDIATE       ][0] = { .byte = 0xEA, .mode = ASSEMBLE_MODE_R },
    [ OPCODE_NOP ][ PARSE_MODE_IMMEDIATE       ][1] = { .byte = 0xEA, .mode = ASSEMBLE_MODE_R },
    [ OPCODE_NOP ][ PARSE_MODE_IMMEDIATE       ][2] = { .byte = 0xEA, .mode = ASSEMBLE_MODE_R },
    [ OPCODE_NOP ][ PARSE_MODE_IMMEDIATE       ][3] = { .byte = 0xEA, .mode = ASSEMBLE_MODE_R },
    [ OPCODE_NOP ][ PARSE_MODE_IMPLIED         ][0] = { .byte = 0xEA, .mode = ASSEMBLE_MODE_0 },
    [ OPCODE_ORA ][ PARSE_MODE_DIRECT          ][1] = { .byte = 0x05, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_ORA ][ PARSE_MODE_DIRECT          ][2] = { .byte = 0x0D, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_ORA ][ PARSE_MODE_DIRECT          ][3] = { .byte = 0x0F, .mode = ASSEMBLE_MODE_3 },
    [ OPCODE_ORA ][ PARSE_MODE_DIRECT_X        ][1] = { .byte = 0x15, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_ORA ][ PARSE_MODE_DIRECT_X        ][2] = { .byte = 0x1D, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_ORA ][ PARSE_MODE_DIRECT_X        ][3] = { .byte = 0x1F, .mode = ASSEMBLE_MODE_3 },
    [ OPCODE_ORA ][ PARSE_MODE_DIRECT_Y        ][2] = { .byte = 0x19, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_ORA ][ PARSE_MODE_DIRECT_S        ][1] = { .byte = 0x03, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_ORA ][ PARSE_MODE_IMMEDIATE       ][0] = { .byte = 0x09, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_ORA ][ PARSE_MODE_IMMEDIATE       ][1] = { .byte = 0x09, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_ORA ][ PARSE_MODE_IMMEDIATE       ][2] = { .byte = 0x09, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_ORA ][ PARSE_MODE_IMMEDIATE       ][3] = { .byte = 0x09, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_ORA ][ PARSE_MODE_INDIRECT        ][1] = { .byte = 0x12, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_ORA ][ PARSE_MODE_X_INDIRECT      ][1] = { .byte = 0x01, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_ORA ][ PARSE_MODE_INDIRECT_Y      ][1] = { .byte = 0x11, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_ORA ][ PARSE_MODE_S_INDIRECT_Y    ][1] = { .byte = 0x13, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_ORA ][ PARSE_MODE_LONG_INDIRECT   ][1] = { .byte = 0x07, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_ORA ][ PARSE_MODE_LONG_INDIRECT_Y ][1] = { .byte = 0x17, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_PHA ][ PARSE_MODE_IMPLIED         ][0] = { .byte = 0x48, .mode = ASSEMBLE_MODE_0 },
    [ OPCODE_PHP ][ PARSE_MODE_IMPLIED         ][0] = { .byte = 0x08, .mode = ASSEMBLE_MODE_0 },
    [ OPCODE_PHX ][ PARSE_MODE_IMPLIED         ][0] = { .byte = 0xDA, .mode = ASSEMBLE_MODE_0 },
    [ OPCODE_PHY ][ PARSE_MODE_IMPLIED         ][0] = { .byte = 0x5A, .mode = ASSEMBLE_MODE_0 },
    [ OPCODE_PLA ][ PARSE_MODE_IMPLIED         ][0] = { .byte = 0x68, .mode = ASSEMBLE_MODE_0 },
    [ OPCODE_PLP ][ PARSE_MODE_IMPLIED         ][0] = { .byte = 0x28, .mode = ASSEMBLE_MODE_0 },
    [ OPCODE_PLX ][ PARSE_MODE_IMPLIED         ][0] = { .byte = 0xFA, .mode = ASSEMBLE_MODE_0 },
    [ OPCODE_PLY ][ PARSE_MODE_IMPLIED         ][0] = { .byte = 0x7A, .mode = ASSEMBLE_MODE_0 },
    [ OPCODE_PHB ][ PARSE_MODE_IMPLIED         ][0] = { .byte = 0x8B, .mode = ASSEMBLE_MODE_0 },
    [ OPCODE_PHD ][ PARSE_MODE_IMPLIED         ][0] = { .byte = 0x0B, .mode = ASSEMBLE_MODE_0 },
    [ OPCODE_PHK ][ PARSE_MODE_IMPLIED         ][0] = { .byte = 0x4B, .mode = ASSEMBLE_MODE_0 },
    [ OPCODE_PLB ][ PARSE_MODE_IMPLIED         ][0] = { .byte = 0xAB, .mode = ASSEMBLE_MODE_0 },
    [ OPCODE_PLD ][ PARSE_MODE_IMPLIED         ][0] = { .byte = 0x2B, .mode = ASSEMBLE_MODE_0 },
    [ OPCODE_PEA ][ PARSE_MODE_DIRECT          ][2] = { .byte = 0xF4, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_PEI ][ PARSE_MODE_INDIRECT        ][1] = { .byte = 0xD4, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_PER ][ PARSE_MODE_DIRECT          ][0] = { .byte = 0x62, .mode = ASSEMBLE_MODE_K },
    [ OPCODE_PER ][ PARSE_MODE_DIRECT          ][1] = { .byte = 0x62, .mode = ASSEMBLE_MODE_K },
    [ OPCODE_PER ][ PARSE_MODE_DIRECT          ][2] = { .byte = 0x62, .mode = ASSEMBLE_MODE_K },
    [ OPCODE_PER ][ PARSE_MODE_DIRECT          ][3] = { .byte = 0x62, .mode = ASSEMBLE_MODE_K },
    [ OPCODE_ROL ][ PARSE_MODE_DIRECT          ][1] = { .byte = 0x26, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_ROL ][ PARSE_MODE_DIRECT          ][2] = { .byte = 0x2E, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_ROL ][ PARSE_MODE_DIRECT_X        ][1] = { .byte = 0x36, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_ROL ][ PARSE_MODE_DIRECT_X        ][2] = { .byte = 0x3E, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_ROL ][ PARSE_MODE_IMMEDIATE       ][0] = { .byte = 0x2A, .mode = ASSEMBLE_MODE_R },
    [ OPCODE_ROL ][ PARSE_MODE_IMMEDIATE       ][1] = { .byte = 0x2A, .mode = ASSEMBLE_MODE_R },
    [ OPCODE_ROL ][ PARSE_MODE_IMMEDIATE       ][2] = { .byte = 0x2A, .mode = ASSEMBLE_MODE_R },
    [ OPCODE_ROL ][ PARSE_MODE_IMMEDIATE       ][3] = { .byte = 0x2A, .mode = ASSEMBLE_MODE_R },
    [ OPCODE_ROL ][ PARSE_MODE_IMPLIED         ][0] = { .byte = 0x2A, .mode = ASSEMBLE_MODE_0 },
    [ OPCODE_ROL ][ PARSE_MODE_ACCUMULATOR     ][0] = { .byte = 0x2A, .mode = ASSEMBLE_MODE_0 },
    [ OPCODE_ROR ][ PARSE_MODE_DIRECT          ][1] = { .byte = 0x66, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_ROR ][ PARSE_MODE_DIRECT          ][2] = { .byte = 0x6E, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_ROR ][ PARSE_MODE_DIRECT_X        ][1] = { .byte = 0x76, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_ROR ][ PARSE_MODE_DIRECT_X        ][2] = { .byte = 0x7E, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_ROR ][ PARSE_MODE_IMMEDIATE       ][0] = { .byte = 0x6A, .mode = ASSEMBLE_MODE_R },
    [ OPCODE_ROR ][ PARSE_MODE_IMMEDIATE       ][1] = { .byte = 0x6A, .mode = ASSEMBLE_MODE_R },
    [ OPCODE_ROR ][ PARSE_MODE_IMMEDIATE       ][2] = { .byte = 0x6A, .mode = ASSEMBLE_MODE_R },
    [ OPCODE_ROR ][ PARSE_MODE_IMMEDIATE       ][3] = { .byte = 0x6A, .mode = ASSEMBLE_MODE_R },
    [ OPCODE_ROR ][ PARSE_MODE_IMPLIED         ][0] = { .byte = 0x6A, .mode = ASSEMBLE_MODE_0 },
    [ OPCODE_ROR ][ PARSE_MODE_ACCUMULATOR     ][0] = { .byte = 0x6A, .mode = ASSEMBLE_MODE_0 },
    [ OPCODE_RTI ][ PARSE_MODE_IMPLIED         ][0] = { .byte = 0x40, .mode = ASSEMBLE_MODE_0 },
    [ OPCODE_RTS ][ PARSE_MODE_IMPLIED         ][0] = { .byte = 0x60, .mode = ASSEMBLE_MODE_0 },
    [ OPCODE_REP ][ PARSE_MODE_IMMEDIATE       ][1] = { .byte = 0xC2, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_RTL ][ PARSE_MODE_IMPLIED         ][0] = { .byte = 0x6B, .mode = ASSEMBLE_MODE_0 },
    [ OPCODE_SBC ][ PARSE_MODE_DIRECT          ][1] = { .byte = 0xE5, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_SBC ][ PARSE_MODE_DIRECT          ][2] = { .byte = 0xED, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_SBC ][ PARSE_MODE_DIRECT          ][3] = { .byte = 0xEF, .mode = ASSEMBLE_MODE_3 },
    [ OPCODE_SBC ][ PARSE_MODE_DIRECT_X        ][1] = { .byte = 0xF5, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_SBC ][ PARSE_MODE_DIRECT_X        ][2] = { .byte = 0xFD, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_SBC ][ PARSE_MODE_DIRECT_X        ][3] = { .byte = 0xFF, .mode = ASSEMBLE_MODE_3 },
    [ OPCODE_SBC ][ PARSE_MODE_DIRECT_Y        ][2] = { .byte = 0xF9, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_SBC ][ PARSE_MODE_DIRECT_S        ][1] = { .byte = 0xE3, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_SBC ][ PARSE_MODE_IMMEDIATE       ][0] = { .byte = 0xE9, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_SBC ][ PARSE_MODE_IMMEDIATE       ][1] = { .byte = 0xE9, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_SBC ][ PARSE_MODE_IMMEDIATE       ][2] = { .byte = 0xE9, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_SBC ][ PARSE_MODE_IMMEDIATE       ][3] = { .byte = 0xE9, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_SBC ][ PARSE_MODE_INDIRECT        ][1] = { .byte = 0xF2, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_SBC ][ PARSE_MODE_X_INDIRECT      ][1] = { .byte = 0xE1, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_SBC ][ PARSE_MODE_INDIRECT_Y      ][1] = { .byte = 0xF1, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_SBC ][ PARSE_MODE_S_INDIRECT_Y    ][1] = { .byte = 0xF3, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_SBC ][ PARSE_MODE_LONG_INDIRECT   ][1] = { .byte = 0xE7, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_SBC ][ PARSE_MODE_LONG_INDIRECT_Y ][1] = { .byte = 0xF7, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_SEC ][ PARSE_MODE_IMPLIED         ][0] = { .byte = 0x38, .mode = ASSEMBLE_MODE_0 },
    [ OPCODE_SED ][ PARSE_MODE_IMPLIED         ][0] = { .byte = 0xF8, .mode = ASSEMBLE_MODE_0 },
    [ OPCODE_SEI ][ PARSE_MODE_IMPLIED         ][0] = { .byte = 0x78, .mode = ASSEMBLE_MODE_0 },
    [ OPCODE_STA ][ PARSE_MODE_DIRECT          ][1] = { .byte = 0x85, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_STA ][ PARSE_MODE_DIRECT          ][2] = { .byte = 0x8D, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_STA ][ PARSE_MODE_DIRECT          ][3] = { .byte = 0x8F, .mode = ASSEMBLE_MODE_3 },
    [ OPCODE_STA ][ PARSE_MODE_DIRECT_X        ][1] = { .byte = 0x95, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_STA ][ PARSE_MODE_DIRECT_X        ][2] = { .byte = 0x9D, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_STA ][ PARSE_MODE_DIRECT_X        ][3] = { .byte = 0x9F, .mode = ASSEMBLE_MODE_3 },
    [ OPCODE_STA ][ PARSE_MODE_DIRECT_Y        ][2] = { .byte = 0x99, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_STA ][ PARSE_MODE_DIRECT_S        ][1] = { .byte = 0x83, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_STA ][ PARSE_MODE_INDIRECT        ][1] = { .byte = 0x92, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_STA ][ PARSE_MODE_X_INDIRECT      ][1] = { .byte = 0x81, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_STA ][ PARSE_MODE_INDIRECT_Y      ][1] = { .byte = 0x91, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_STA ][ PARSE_MODE_S_INDIRECT_Y    ][1] = { .byte = 0x93, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_STA ][ PARSE_MODE_LONG_INDIRECT   ][1] = { .byte = 0x87, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_STA ][ PARSE_MODE_LONG_INDIRECT_Y ][1] = { .byte = 0x97, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_STX ][ PARSE_MODE_DIRECT          ][1] = { .byte = 0x86, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_STX ][ PARSE_MODE_DIRECT          ][2] = { .byte = 0x8E, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_STX ][ PARSE_MODE_DIRECT_Y        ][1] = { .byte = 0x96, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_STY ][ PARSE_MODE_DIRECT          ][1] = { .byte = 0x84, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_STY ][ PARSE_MODE_DIRECT          ][2] = { .byte = 0x8C, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_STY ][ PARSE_MODE_DIRECT_X        ][1] = { .byte = 0x94, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_STZ ][ PARSE_MODE_DIRECT          ][1] = { .byte = 0x64, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_STZ ][ PARSE_MODE_DIRECT          ][2] = { .byte = 0x9C, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_STZ ][ PARSE_MODE_DIRECT_X        ][1] = { .byte = 0x74, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_STZ ][ PARSE_MODE_DIRECT_X        ][2] = { .byte = 0x9E, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_SEP ][ PARSE_MODE_IMMEDIATE       ][1] = { .byte = 0xE2, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_STP ][ PARSE_MODE_IMPLIED         ][0] = { .byte = 0xDB, .mode = ASSEMBLE_MODE_0 },
    [ OPCODE_TAX ][ PARSE_MODE_IMPLIED         ][0] = { .byte = 0xAA, .mode = ASSEMBLE_MODE_0 },
    [ OPCODE_TAY ][ PARSE_MODE_IMPLIED         ][0] = { .byte = 0xA8, .mode = ASSEMBLE_MODE_0 },
    [ OPCODE_TRB ][ PARSE_MODE_DIRECT          ][1] = { .byte = 0x14, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_TRB ][ PARSE_MODE_DIRECT          ][2] = { .byte = 0x1C, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_TSB ][ PARSE_MODE_DIRECT          ][1] = { .byte = 0x04, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_TSB ][ PARSE_MODE_DIRECT          ][2] = { .byte = 0x0C, .mode = ASSEMBLE_MODE_2 },
    [ OPCODE_TSX ][ PARSE_MODE_IMPLIED         ][0] = { .byte = 0xBA, .mode = ASSEMBLE_MODE_0 },
    [ OPCODE_TXA ][ PARSE_MODE_IMPLIED         ][0] = { .byte = 0x8A, .mode = ASSEMBLE_MODE_0 },
    [ OPCODE_TXS ][ PARSE_MODE_IMPLIED         ][0] = { .byte = 0x9A, .mode = ASSEMBLE_MODE_0 },
    [ OPCODE_TYA ][ PARSE_MODE_IMPLIED         ][0] = { .byte = 0x98, .mode = ASSEMBLE_MODE_0 },
    [ OPCODE_TXY ][ PARSE_MODE_IMPLIED         ][0] = { .byte = 0x9B, .mode = ASSEMBLE_MODE_0 },
    [ OPCODE_TYX ][ PARSE_MODE_IMPLIED         ][0] = { .byte = 0xBB, .mode = ASSEMBLE_MODE_0 },
    [ OPCODE_TCD ][ PARSE_MODE_IMPLIED         ][0] = { .byte = 0x5B, .mode = ASSEMBLE_MODE_0 },
    [ OPCODE_TDC ][ PARSE_MODE_IMPLIED         ][0] = { .byte = 0x7B, .mode = ASSEMBLE_MODE_0 },
    [ OPCODE_TCS ][ PARSE_MODE_IMPLIED         ][0] = { .byte = 0x1B, .mode = ASSEMBLE_MODE_0 },
    [ OPCODE_TSC ][ PARSE_MODE_IMPLIED         ][0] = { .byte = 0x3B, .mode = ASSEMBLE_MODE_0 },
    [ OPCODE_WAI ][ PARSE_MODE_IMPLIED         ][0] = { .byte = 0xCB, .mode = ASSEMBLE_MODE_0 },
    [ OPCODE_WDM ][ PARSE_MODE_IMMEDIATE       ][1] = { .byte = 0x42, .mode = ASSEMBLE_MODE_1 },
    [ OPCODE_XBA ][ PARSE_MODE_IMPLIED         ][0] = { .byte = 0xEB, .mode = ASSEMBLE_MODE_0 },
    [ OPCODE_XCE ][ PARSE_MODE_IMPLIED         ][0] = { .byte = 0xFB, .mode = ASSEMBLE_MODE_0 },
};

#define X(c) #c,
static char* opcodes[]         = { OPCODES_LIST };
static char opcodes_lower[][4] = { OPCODES_LIST };
#undef X

static
void error_at_pos(NutsTextPos text_pos) {
    c_print_format("%.*s:%i:%i: error\n", (int)text_pos.name.length, text_pos.name.data,
        text_pos.line_number, text_pos.line_pos);
    /* fstderr, "%.*s:%i:%i: error\n", text_pos.name.length, text_pos.name.data, */
    /*     text_pos.line_number, text_pos.line_pos); */
    char* line_end = text_pos.line_start;
    int length = 0;
    while(*line_end && *line_end != '\n') {
        if(line_end - text_pos.line_start < text_pos.line_pos) {
            if(*line_end == '\t') {
                length += 8;
            } else {
                length += 1;
            }
        }
        line_end++;
    }
    /* c_error_format("%.*s\n", (int)(line_end - text_pos.line_start), text_pos.line_start); */
    /* for(int i = 1; i < length; i++) c_error_format("~"); */
    /* c_error_format("^\n"); */
    c_print_format("%.*s\n", (int)(line_end - text_pos.line_start), text_pos.line_start);
    for(int i = 1; i < length; i++) c_print_format("~");
    c_print_format("^\n");
}

#if 0
static
void warn_at_pos(NutsTextPos text_pos) {
    c_print_format("%.*s:%i:%i: warning\n", (int)text_pos.name.length, text_pos.name.data,
        text_pos.line_number, text_pos.line_pos);
    char* line_end = text_pos.line_start;
    int length = 0;
    while(*line_end && *line_end != '\n') {
        if(line_end - text_pos.line_start < text_pos.line_pos) {
            if(*line_end == '\t') {
                length += 8;
            } else {
                length += 1;
            }
        }
        line_end++;
    }
    c_print_format("%.*s\n", (int)(line_end - text_pos.line_start), text_pos.line_start);
    for(int i = 0; i < length; i++) c_print_format("~");
    c_print_format("^\n");
}
#endif

#define X(c) #c,
static char* commands[] = { COMMANDS_LIST };
static char* token_names[] = { [TOKEM_ASCII_RANGE_] = "",  TOKEN_LIST };
const char* error_names[] = { ERROR_LIST };
// These will be converted to lowercase in cmd_map_fill
static char commands_lower[][12] = { COMMANDS_LIST };
#undef X

void nuts_global_init() {
    for(uint i = 0; i < array_length(opcodes); i++) {
        for(char* p = opcodes_lower[i]; *p; p++) *p = *p + ('a' - 'A');
    }
    
    for(uint i = 0; i < array_length(commands); i++) {
        for(char* p = commands_lower[i]; *p; p++) *p = *p + ('a' - 'A');
    }

    static char buffer[128][4];
    for(int i = 0; i < 128; i++) {
        buffer[i][0] = '\'';
        buffer[i][1] = i;
        buffer[i][2] = '\'';
        buffer[i][3] = '\0';
        token_names[i] = buffer[i];
    }
}

static
NutsTextPos get_text_pos(NutsText text, String string) {
    char* pos = text.buffer.begin;
    NutsTextPos result = {
        .name = text.name,
        .line_number = 1,
        .line_pos    = 0,
        .line_start  = pos
    };
    while(pos < (char*)text.buffer.end) {
        if(pos == string.data) {
            break;
        }
        pos++;
        result.line_pos++;
        if(*pos == '\n') {
            result.line_start = pos + 1;
            result.line_number++;
            result.line_pos = 0;
        }
    }
    assert(pos == string.data);
    return result;
}

void nuts_error_list_init(NutsErrorList* error_list, Arena arena) {
    error_list->arena = arena;
    error_list->errors = arena_alloc_array(&arena, 0, NutsError);
    error_list->length = 0;
}

static
void error_add(NutsErrorList* error_list, NutsError error) {
    NutsError* new_error_in_list = arena_alloc_array(&error_list->arena, 1, NutsError);
    if(new_error_in_list) {
        *new_error_in_list = error;
        error_list->length++;
    }
}


#define return_on_error(error) do {if(error == RESULT_ERROR) return RESULT_ERROR;} while(0)


void nuts_describe_error(NutsError error) {
    if(error.text_pos.name.data) error_at_pos(error.text_pos);
    c_print_format("%s\n", error_names[error.type]);
    //c_error_format("%s\n", error_names[error.type]);
}

static
void byte_stream_init(NutsByteStream* stream, NutsText text) {
    stream->begin = text.buffer.begin;
    stream->end   = text.buffer.end;
    stream->pos   = text.buffer.begin;
    stream->text  = text;
}

static
NutsRune byte_stream_advance(NutsByteStream* stream) {
    if(stream->pos < (char*)stream->end) {
        stream->pos++;
        return *stream->pos;
    } else {
        return BYTE_STREAM_EOF;
    }
}

static
NutsRune byte_stream_current(NutsByteStream* byte_stream) {
    if(byte_stream->pos < (char*)byte_stream->end) {
        return byte_stream->pos[0];
    } else {
        return BYTE_STREAM_EOF;
    }
}


static
NutsRune byte_stream_lookahead(NutsByteStream* byte_stream) {
    if(byte_stream->pos + 1 < (char*)byte_stream->end) {
        return byte_stream->pos[1];
    } else {
        return BYTE_STREAM_EOF;
    }
}

static inline
int is_ident_char(NutsRune c) {
    return (c >= 'a' && c <= 'z')
        || (c >= 'A' && c <= 'Z')
        || (c >= '0' && c <= '9')
        || c == '_' || c == '.';
}

static inline
int is_whitespace(NutsRune c) {
    return c == ' ' || c == '\t';
}

static inline
int is_ascii(NutsRune c) {
    return c & 0xFFFFFF80 ? 0 : 1;
}

static inline
int is_eof(NutsRune c) {
    return c == BYTE_STREAM_EOF;
}


// NOTE: To get the width of a number, we count the number of digits used,
// compute the maximal value (max_num) which can be expressed with them and
// take the number of bytes which this max_num needs.
// Example: 000512 -> 999999 = 0xF423F -> five bytes are used.
// TODO: Parse floats
static warn_unused_result
NutsResult get_token_number(NutsByteStream* stream, int base, NutsToken* result, NutsErrorList* error_list) {
    NutsRune r = byte_stream_current(stream);
    // TODO: Check if each digit is smaller than base.
    u64 num = { 0 };
    if(r >= '0' && r <= '9') {
        num = r - '0';
    } else if(r >= 'A' && r <= 'F') {
        num = r - 'A' + 10;
    } else if(r >= 'a' && r <= 'f') {
        num = r - 'a' + 10;
    } else {
        result->string.length = stream->pos - result->string.data + 1;
        error_add(error_list, (NutsError){
           .type = ERROR_NOT_A_N_ARY_DIGIT,
           .text_pos = get_text_pos(stream->text, result->string),
           .not_a_n_ary_digit.base = base,
        });
        return RESULT_ERROR;
    }
    u64 max_num = base;
    while(!is_eof(byte_stream_lookahead(stream))) {
        r = byte_stream_lookahead(stream);
        int digit;
        if(r >= '0' && r <= '9') {
            digit = r - '0';
        } else if(r >= 'A' && r <= 'F') {
            digit = r - 'A' + 10;
        } else if(r >= 'a' && r <= 'f') {
            digit = r - 'a' + 10;
        } else {
            break;
        }
        num = num * base + digit;
        max_num = max_num * base;
        byte_stream_advance(stream);
    }
    result->type  = TOKEN_NUM;
    result->num   = num;
    result->string.length = stream->pos - result->string.data + 1;

    result->width = WIDTH_1;
    if(max_num > 0x0000100) result->width = WIDTH_2;
    if(max_num > 0x0010000) result->width = WIDTH_3;
    if(max_num > 0x1000000) result->width = WIDTH_4;
    return RESULT_OK;
}

static const
NutsToken token_eof = { .type = TOKEN_EOF };

static warn_unused_result
NutsResult get_token(NutsByteStream* stream, NutsToken* token, NutsErrorList* error_list,
                 NutsIdentifierMap* identifier_map) {
    *token = (NutsToken) {
        .string.data = stream->pos,
        .type        = TOKEN_NULL,
    };
    while(token->type == TOKEN_NULL) {
        NutsRune r = byte_stream_current(stream);
        if(is_eof(r)) {
            *token = token_eof;
            return RESULT_OK;
        }
        assert(is_ascii(r));
        if(!is_ascii(r)) exit(1);
        switch(r & 0x7F) {
        case '\t':
        case ' ': {
            while(!is_eof(r) && is_whitespace(r))  {
                r = byte_stream_advance(stream);
            }
            token->string.data = stream->pos;
            continue;
        }

        case ';': {
            NutsRune r = byte_stream_lookahead(stream);
            while(!is_eof(r) && r != '\n') {
                byte_stream_advance(stream);
                r = byte_stream_lookahead(stream);
            }
            token->type = TOKEN_COMMENT;
            break;
        }

        case '\r': {
            if(byte_stream_lookahead(stream) == '\n') {
                token->type = TOKEN_CMD_DELIM;
                byte_stream_advance(stream);
            } else {
                error_add(error_list, (NutsError){
                    .type = ERROR_UNEXPECTED_CHARACTER,
                    .text_pos = get_text_pos(stream->text, token->string)
                });
                return RESULT_ERROR;
            }
            break;
        }

        case '\n': {
            token->type = TOKEN_CMD_DELIM;
            break;
        }

        case 'A' ... 'Z':
        case 'a' ... 'z':
        case '_': case '.': {
            char* start = stream->pos;
            while(is_ident_char(byte_stream_lookahead(stream))) {
                byte_stream_advance(stream);
            }
            char* end = stream->pos;
            token->string = (String) {
                .data = start,
                .length = end - start + 1
            };
            token->identifier =
                nuts_identifier_map_find_or_insert(identifier_map,
                                                   (NutsIdentifier){
                                                       .name = token->string,
                                                       .type = IDENTIFIER,
                                                   });
            token->type = TOKEN_IDENTIFIER;
            break;
        }

        case '0' ... '9': {
            return_on_error(get_token_number(stream, 10, token, error_list));
            break;
        }

        case '$': {
            byte_stream_advance(stream);
            return_on_error(get_token_number(stream, 16, token, error_list));
            break;
        }

        case '%': {
            byte_stream_advance(stream);
            return_on_error(get_token_number(stream,  2, token, error_list));
            break;
        }

        case '<': {
            if(byte_stream_lookahead(stream) == '<') {
                byte_stream_advance(stream);
                token->type = TOKEN_LSHIFT;
            } else {
                token->type = '<';
            }
            break;
        }

        case '>': {
            if(byte_stream_lookahead(stream) == '>') {
                byte_stream_advance(stream);
                token->type = TOKEN_RSHIFT;
            } else {
                token->type = '>';
            }
            break;
        }

        case '#': case '&': case ':': case '(': case ')': case '[':
        case ']': case '{': case '}': case '=': case ',': case '*':
        case '/': case '|': {
            token->type = r;
            break;
        }

        case '!': {
            token->name.data = stream->pos + 1;
            while(is_ident_char(byte_stream_lookahead(stream))) {
                byte_stream_advance(stream);
            }
            token->name.length = stream->pos - token->name.data + 1;
            token->type = TOKEN_DEFINE;
            /* c_error_format("%.*s:%i:%i ", stream->text_pos.file_name.length, stream->text_pos.file_name.data, */
            /*         stream->text_pos.line_number, stream->text_pos.line_pos); */
            /* c_error_format("define: %.*s\n", token->string.length, token->string.data); */
            break;
        }

        case '"': {
            r = byte_stream_advance(stream);
            token->name.data = stream->pos;
            while(r != '"') {
                if(r == '\0') {
                    error_add(error_list, (NutsError){
                        .type = ERROR_UNTERMINATED_STRING,
                        .text_pos = get_text_pos(stream->text, token->string)
                    });
                    return RESULT_ERROR;
                };
                r = byte_stream_advance(stream);
            }
            token->name.length = stream->pos - token->string.data - 1;
            token->type = TOKEN_STRING;
            break;
        }

        case '+': {
            if(byte_stream_lookahead(stream) != '+') {
                token->type = '+';
            } else {
                token->type = TOKEN_ANON_LABEL;
                while(byte_stream_lookahead(stream) == '+') {
                    byte_stream_advance(stream);
                }
                token->num = stream->pos - token->string.data;
            }
            break;
        }

        case '-': {
            if(byte_stream_lookahead(stream) != '-' &&
               byte_stream_lookahead(stream) != '>') {
                token->type = '-';
            } else if(byte_stream_lookahead(stream) == '>') {
                token->type = TOKEN_ARROW;
                byte_stream_advance(stream);
            } else {
                token->type = TOKEN_ANON_LABEL;
                while(byte_stream_lookahead(stream) == '-') {
                    byte_stream_advance(stream);
                }
                token->num = -(stream->pos - token->string.data);
            }
            break;
        }

        default: {
            error_add(error_list, (NutsError){
                .type = ERROR_UNEXPECTED_CHARACTER,
                .text_pos = get_text_pos(stream->text, token->string)
            });
            return RESULT_ERROR;
        }
        }
        byte_stream_advance(stream);
        token->string.length = stream->pos - token->string.data;
    }
    return RESULT_OK;
}

#if 0
static
void token_list_print(NutsTokenList token_list) {
    for(uint i = 0; i < token_list.length; i++) {
        NutsToken token = token_list.data[i];
        c_print_format("%s(%.*s)\n", token_names[token.type],
               (int)token.string.length, token.string.data);
    }
}
#endif

static
void token_stream_init(NutsTokenStream* stream, NutsTokenList list) {
    stream->begin = list.data;
    stream->end   = list.data + list.length;
    stream->pos   = list.data;
    stream->text  = list.text;
}

static
void token_stream_advance(NutsTokenStream* stream) {
    if(stream->pos < stream->end) {
        stream->pos++;
    }
}

static
NutsToken token_stream_current_token(NutsTokenStream* stream) {
    if(stream->pos < stream->end) {
        return stream->pos[0];
    } else {
        return token_eof;
    }
}


#define BATCH_SIZE 16
NutsResult nuts_lex(NutsText text, NutsTokenList* list, Arena* arena, NutsErrorList* error_list,
           NutsIdentifierMap* identifier_map) {
    *list = (NutsTokenList) {
        .text   = text,
        .data   = arena_alloc_array(arena, 0, NutsToken),
        .length = 0
    };
    NutsByteStream stream;
    byte_stream_init(&stream, text);
    NutsToken token_buffer[BATCH_SIZE];
    int length = 0;
    int eof = 0;
    while(!eof) {
        for(int i = 0; i < BATCH_SIZE && !eof; i++) {
            return_on_error(get_token(&stream, &token_buffer[i], error_list, identifier_map));
            length++;
            if(token_buffer[i].type == TOKEN_EOF) eof = 1;
        }
        NutsToken* tokens = arena_alloc_array(arena, BATCH_SIZE, NutsToken);
        if(!tokens) {
            error_add(error_list, (NutsError) {
                .type     = ERROR_TO_MANY_TOKENS,
                //.text_pos = get_text_pos(token_text, token_string)
            });

            return RESULT_ERROR;
        }

        // memcpy
        for(int i = 0; i < BATCH_SIZE; i++) {
            tokens[i] = token_buffer[i];
        }
    }
    list->length = length;
    return RESULT_OK;
}

implement_stack(NutsTokenStreamStack, nuts_token_stream_stack, NutsTokenStream);

static
u32 define_equal(NutsDefine e1, NutsDefine e2) {
    return string_equal(e1.name, e2.name);
}

static
u32 define_hash(NutsDefine e) {
    return hash_murmur3_string(e.name);
}
implement_hashmap(NutsDefineMap, nuts_define_map, NutsDefine, define_hash, define_equal);

static
u32 identifier_equal(NutsIdentifier e1, NutsIdentifier e2) {
    return string_equal(e1.name, e2.name);
}

static
u32 identifier_hash(NutsIdentifier e) {
    return hash_murmur3_string(e.name);
}

static
NutsIdentifier* identifier_copy(Arena* arena, NutsIdentifier e) {
    NutsIdentifier* copy = arena_alloc_type(arena, NutsIdentifier);
    *copy = e;

    Buffer buffer = arena_alloc_buffer(arena, e.name.length, 1);
    copy_buffer(buffer_from_string(e.name), buffer);
    copy->name = string_from_buffer(buffer);
    return copy;
}

implement_intern_hashmap(NutsIdentifierMap, nuts_identifier_map, NutsIdentifier,
                         identifier_hash, identifier_equal, identifier_copy);


static
void parser_advance_stream(NutsParser* parser);
static warn_unused_result
NutsResult parser_expect(NutsParser* parser, NutsTokenType token_type);
    
static const
NutsExpr null_expr = { 0 };

static const
struct bin_op_hierarchy_entry {
    uint token_type;
    uint level;
} hierarchy[] = {
    [EXPR_MUL] = { .token_type = '*',          .level = 4 },
    [EXPR_DIV] = { .token_type = '/',          .level = 4 },
    [EXPR_BAN] = { .token_type = '&',          .level = 3 },
    [EXPR_BOR] = { .token_type = '|',          .level = 3 },
    [EXPR_LSH] = { .token_type = TOKEN_LSHIFT, .level = 2 },
    [EXPR_RSH] = { .token_type = TOKEN_RSHIFT, .level = 2 },
    [EXPR_ADD] = { .token_type = '+',          .level = 1 },
    [EXPR_SUB] = { .token_type = '-',          .level = 1 },
};



static
NutsIdentifier* label_full_name(String sublabel_name,
                            String label_name,
                            NutsIdentifierMap* identifier_map) {
    char full_name[4096] = {};
    int pos = 0;
    uint num_dots = 0;
    uint copy_index = 0;
    while(num_dots < sublabel_name.length) {
        if(sublabel_name.data[num_dots] != '.') break;
        while(label_name.data[copy_index] != '.' &&
              copy_index < label_name.length) {
            full_name[pos++] = label_name.data[copy_index++];
        }
        full_name[pos++] = '.';
        copy_index++;
        num_dots++;
    }
    
    for(uint j = num_dots; j < sublabel_name.length; j++) {
        full_name[pos++] = sublabel_name.data[j];
    }
   
    return nuts_identifier_map_find_or_insert(identifier_map, (NutsIdentifier) {
        .name.data = full_name, .name.length = pos
    });
}


static warn_unused_result
NutsResult parse_expr_(NutsParser* parser, NutsExpr* expr, uint level) {
    NutsExpr op1 = { 0 };
    NutsExpr op2 = { 0 };
    String string = {
        .data = parser->token.string.data
    };
    if(parser->token.type == '(') {
        parser_advance_stream(parser);
        return_on_error(parse_expr_(parser, &op1, 0));
        return_on_error(parser_expect(parser, ')'));
        parser_advance_stream(parser);
    } else if(parser->token.type == '+') {
        return_on_error(parse_expr_(parser, &op1, 14));
    } else if(parser->token.type == '-') {
        NutsText token_text = parser->stream_stack_top->text;
        String token_string = parser->token.string;
        return_on_error(parse_expr_(parser, &op1, 14));
        if(op1.value.type != VALUE_INT) {
            error_add(parser->error_list, (NutsError) {
                .type     = ERROR_INCOMPATIBLE_OPERANDS,
                .text_pos = get_text_pos(token_text, token_string)
            });
            return RESULT_ERROR;
        }
        NutsExpr* neg_expr = arena_alloc_type(parser->arena, NutsExpr);
        *neg_expr = op1;
        op1 = (NutsExpr) {
            .type = EXPR_NEG,
            .value_type = VALUE_INT,
            .op1  = neg_expr,
        };
    } else if(parser->token.type == TOKEN_NUM) {
        op1 = (NutsExpr) {
            .type       = EXPR_VALUE,
            .value.type = VALUE_INT,
            .value.i    = parser->token.num,
            .width      = parser->token.width,
        };
        parser_advance_stream(parser);
    } else if(parser->token.type == TOKEN_STRING) {
        op1 = (NutsExpr) {
            .type        = EXPR_VALUE,
            .value.type  = VALUE_STRING,
            .value.s     = parser->token.name,
            .width       = WIDTH_UNSPECIFIED,
        };
        parser_advance_stream(parser);
    } else if(parser->token.type == TOKEN_IDENTIFIER) {
        NutsIdentifier* ident = label_full_name(parser->token.identifier->name,
                                            parser->last_label_name,
                                            &parser->ast->identifier_map);
        op1 = (NutsExpr){
            .type        = EXPR_LABEL,
            .value.type  = VALUE_LABEL,
            .value.l     = ident,
            .width       = WIDTH_3,
        };
        parser_advance_stream(parser);
    } else {
        return RESULT_NOT_AN_EXPR;
    }

#define parse_binary_operator(expr_type) {                                      \
        if(level >= hierarchy[expr_type].level) break;                          \
        NutsText token_text = parser->stream_stack_top->text;                   \
        String token_string = parser->token.string;                             \
        parser_advance_stream(parser);                                          \
        return_on_error(parse_expr_(parser, &op2, hierarchy[expr_type].level)); \
        if((op1.value_type != VALUE_INT && op1.value_type != VALUE_LABEL) ||    \
           (op2.value_type != VALUE_INT && op2.value_type != VALUE_LABEL)) {    \
            error_add(parser->error_list, (NutsError) {                         \
                .type = ERROR_INCOMPATIBLE_OPERANDS,                            \
                .text_pos = get_text_pos(token_text, token_string)              \
            });                                                                 \
            return RESULT_ERROR;                                                \
        }                                                                       \
        NutsExpr* ops = arena_alloc_array(parser->arena, 2, NutsExpr);          \
        ops[0] = op1; ops[1] = op2;                                             \
        op1 = (NutsExpr){                                                       \
            .type = expr_type,                                                  \
            .value_type = VALUE_INT,                                            \
            .op1 = ops + 0,                                                     \
            .op2 = ops + 1,                                                     \
            .width = max(op1.width, op2.width),                                 \
        };                                                                      \
        continue;                                                               \
    }
            
    while(1) {
        if(parser->token.type == '*') parse_binary_operator(EXPR_MUL);
        if(parser->token.type == '/') parse_binary_operator(EXPR_DIV);
        if(parser->token.type == '&') parse_binary_operator(EXPR_BAN);
        if(parser->token.type == '|') parse_binary_operator(EXPR_BOR);
        if(parser->token.type == TOKEN_LSHIFT) parse_binary_operator(EXPR_LSH);
        if(parser->token.type == TOKEN_RSHIFT) parse_binary_operator(EXPR_RSH);
        if(parser->token.type == '+') parse_binary_operator(EXPR_ADD);
        if(parser->token.type == '-') parse_binary_operator(EXPR_SUB);

        break;
	}
#undef parse_binary_operator

	*expr = op1;
    string.length = parser->token.string.data + parser->token.string.length - string.data;
    expr->string = string;
    return RESULT_OK;
}

static warn_unused_result
NutsResult parse_expr(NutsParser* parser, NutsExpr* expr) {
    return parse_expr_(parser, expr, 0);
}

static warn_unused_result
NutsResult eval_expr(NutsExpr* expr, NutsValue* value,
                 NutsLabelMap* label_map,
                 NutsErrorList* error_list) {
    NutsValue val1, val2;
    switch(expr->type) {
    case EXPR_ADD: {
        return_on_error(eval_expr(expr->op1, &val1, label_map, error_list));
        return_on_error(eval_expr(expr->op2, &val2, label_map, error_list));

        value->i = val1.i + val2.i;
        value->type = VALUE_INT;
        break;
    }
        
    case EXPR_SUB: {
        return_on_error(eval_expr(expr->op1, &val1, label_map, error_list));
        return_on_error(eval_expr(expr->op2, &val2, label_map, error_list));

        value->i = val1.i - val2.i;
        value->type = VALUE_INT;
        break;
    }

    case EXPR_MUL: {
        return_on_error(eval_expr(expr->op1, &val1, label_map, error_list));
        return_on_error(eval_expr(expr->op2, &val2, label_map, error_list));

        value->i = val1.i * val2.i;
        value->type = VALUE_INT;
        break;
    }
        
    case EXPR_DIV: {
        return_on_error(eval_expr(expr->op1, &val1, label_map, error_list));
        return_on_error(eval_expr(expr->op2, &val2, label_map, error_list));

        value->i = val1.i / val2.i;
        value->type = VALUE_INT;
        break;
    }

    case EXPR_LSH: {
        return_on_error(eval_expr(expr->op1, &val1, label_map, error_list));
        return_on_error(eval_expr(expr->op2, &val2, label_map, error_list));

        value->i = val1.i << val2.i;
        value->type = VALUE_INT;
        break;
    }

    case EXPR_RSH: {
        return_on_error(eval_expr(expr->op1, &val1, label_map, error_list));
        return_on_error(eval_expr(expr->op2, &val2, label_map, error_list));

        value->i = val1.i >> val2.i;
        value->type = VALUE_INT;
        break;
    }

    case EXPR_BAN: {
        return_on_error(eval_expr(expr->op1, &val1, label_map, error_list));
        return_on_error(eval_expr(expr->op2, &val2, label_map, error_list));

        value->i = val1.i & val2.i;
        value->type = VALUE_INT;
        break;
    }

    case EXPR_BOR: {
        return_on_error(eval_expr(expr->op1, &val1, label_map, error_list));
        return_on_error(eval_expr(expr->op2, &val2, label_map, error_list));

        value->i = val1.i | val2.i;
        value->type = VALUE_INT;
        break;
    }

    case EXPR_NEG: {
        return_on_error(eval_expr(expr->op1, &val1, label_map, error_list));

        value->i = -val1.i;
        value->type = VALUE_INT;
        break;
    }

    case EXPR_VALUE: {
            *value = expr->value;
        break;
    }
    case EXPR_LABEL: {
        NutsLabel* found = nuts_label_map_find(label_map, (NutsLabel) { .name = expr->value.l->name });
        if(found) {
            value->i = found->addr;
            value->type = VALUE_LABEL;
        } else {
            return RESULT_UNDEFINED_LABEL;
        }
        break;
    } 

    invalid_default_case;
    }
    return RESULT_OK;
}

static warn_unused_result
NutsResult static_eval_expr(NutsExpr* expr, NutsValue* value,
                        NutsErrorList* error_list) {
    NutsValue val1, val2;
    switch(expr->type) {
    case EXPR_ADD: {
        return_on_error(static_eval_expr(expr->op1, &val1, error_list));
        return_on_error(static_eval_expr(expr->op2, &val2, error_list));

        value->i = val1.i + val2.i;
        value->type = VALUE_INT;
        break;
    }
        
    case EXPR_SUB: {
        return_on_error(static_eval_expr(expr->op1, &val1, error_list));
        return_on_error(static_eval_expr(expr->op2, &val2, error_list));

        value->i = val1.i - val2.i;
        value->type = VALUE_INT;
        break;
    }

    case EXPR_MUL: {
        return_on_error(static_eval_expr(expr->op1, &val1, error_list));
        return_on_error(static_eval_expr(expr->op2, &val2, error_list));

        value->i = val1.i * val2.i;
        value->type = VALUE_INT;
        break;
    }
        
    case EXPR_DIV: {
        return_on_error(static_eval_expr(expr->op1, &val1, error_list));
        return_on_error(static_eval_expr(expr->op2, &val2, error_list));

        value->i = val1.i / val2.i;
        value->type = VALUE_INT;
        break;
    }

    case EXPR_LSH: {
        return_on_error(static_eval_expr(expr->op1, &val1, error_list));
        return_on_error(static_eval_expr(expr->op2, &val2, error_list));

        value->i = val1.i << val2.i;
        value->type = VALUE_INT;
        break;
    }

    case EXPR_RSH: {
        return_on_error(static_eval_expr(expr->op1, &val1, error_list));
        return_on_error(static_eval_expr(expr->op2, &val2, error_list));

        value->i = val1.i >> val2.i;
        value->type = VALUE_INT;
        break;
    }

    case EXPR_BAN: {
        return_on_error(static_eval_expr(expr->op1, &val1, error_list));
        return_on_error(static_eval_expr(expr->op2, &val2, error_list));

        value->i = val1.i & val2.i;
        value->type = VALUE_INT;
        break;
    }

    case EXPR_BOR: {
        return_on_error(static_eval_expr(expr->op1, &val1, error_list));
        return_on_error(static_eval_expr(expr->op2, &val2, error_list));

        value->i = val1.i | val2.i;
        value->type = VALUE_INT;
        break;
    }

    case EXPR_NEG: {
        return_on_error(static_eval_expr(expr->op1, &val1, error_list));

        value->i = -val1.i;
        value->type = VALUE_INT;
        break;
    }

    case EXPR_VALUE: {
        *value = expr->value;
        break;
    }

    case EXPR_LABEL: {
        //TODO: Add text_pos;
        error_add(error_list, (NutsError) {
            .type     = ERROR_EXPR_NOT_STATIC,
            //.text_pos = text_pos
        });
        return RESULT_ERROR;
    }

    invalid_default_case;
    }
    return RESULT_OK;
}

static
int token_seperates_statements(NutsToken token) {
    return token.type == TOKEN_CMD_DELIM
        || token.type == TOKEN_COMMENT
        || token.type == ':'
        || token.type == TOKEN_EOF;
}

void nuts_ast_init(NutsAST* ast, FreeList* free_list) {
    ast->free_list = free_list;
    ast->statement_list.length = 0;
    ast->statement_list.data = free_list_alloc_array(free_list, 512*1024, NutsStatement);
    nuts_identifier_map_init(&ast->identifier_map, free_list, 4096, MB(1));
    
    for(uint i = 0; i < array_length(commands); i++) {
        NutsIdentifier entry = {
            .name = string_from_c_string(commands[i]),
            .type = COMMAND | i,
        };
        nuts_identifier_map_insert(&ast->identifier_map, entry);

        entry = (NutsIdentifier){
            .name = string_from_c_string(commands_lower[i]),
            .type = COMMAND | i,
        };
        nuts_identifier_map_insert(&ast->identifier_map, entry);
    }

    for(uint i = 0; i < array_length(opcodes); i++) {
        u8 widths[7] = { WIDTH_UNSPECIFIED,
                         WIDTH_1, WIDTH_2, WIDTH_3,
                         WIDTH_1, WIDTH_2, WIDTH_3 };
        char buffer[7][6] = {"",
                             "   .b", "   .w", "   .l",
                             "   .B", "   .W", "   .L"};
        for(int j = 0; j < 7; j++) {
            buffer[j][0] = opcodes_lower[i][0];
            buffer[j][1] = opcodes_lower[i][1];
            buffer[j][2] = opcodes_lower[i][2];
            NutsIdentifier entry = (NutsIdentifier){
                .name = string_from_c_string(buffer[j]),
                .type = OPCODE | i,
                .width = widths[j]
            };
            nuts_identifier_map_insert(&ast->identifier_map, entry);
            buffer[j][0] = opcodes[i][0];
            buffer[j][1] = opcodes[i][1];
            buffer[j][2] = opcodes[i][2];
            entry = (NutsIdentifier){
                .name = string_from_c_string(buffer[j]),
                .type = OPCODE | i,
                .width = widths[j]
            };
            nuts_identifier_map_insert(&ast->identifier_map, entry);
        }
    }
}

void ast_deinit(NutsAST* ast) {
    free_list_dealloc_array(ast->free_list, ast->statement_list.data, 512*1024, NutsStatement);
    nuts_identifier_map_deinit(&ast->identifier_map);
}

static
int identifier_type_is_command(NutsIdentifierType type) {
    return type & COMMAND;
}

static
int identifier_is_command(NutsIdentifierType type, NutsCommandType cmd_type) {
    return type == (COMMAND | cmd_type);
}

static
int identifier_type_is_opcode(NutsIdentifierType type) {
    return type & OPCODE;
}

#if 0
static
int identifier_is_opcode(NutsIdentifierType type, NutsOpcodeType op_type) {
    return type == (OPCODE | op_type);
}
#endif

static
int identifier_type_is_identifier(NutsIdentifierType type) {
    return type == IDENTIFIER;
}



void nuts_parser_init(NutsParser* parser, Arena* arena, FreeList* free_list,
                      NutsErrorList* error_list, NutsAST* ast) {
    parser->token = (NutsToken){ 0 };
    parser->token_count = 0;
    parser->arena = arena;
    parser->free_list = free_list;
    parser->error_list = error_list;
    parser->ast = ast;

    nuts_token_stream_stack_init(&parser->stream_stack, free_list, 8);
    nuts_define_map_init(&parser->define_map, free_list, 512);

    parser->last_label_name.length = 0;
}

void parser_deinit(NutsParser* parser) {
    nuts_token_stream_stack_deinit(&parser->stream_stack, parser->free_list);
    nuts_define_map_deinit(&parser->define_map);
}


#define DO_EXPAND_DEFINES 1
#define DONT_EXPAND_DEFINES 0

static
void parser_update_stream(NutsParser* parser, int expand_defines) {
    parser->token = token_stream_current_token(parser->stream_stack_top);
    while(1) {
        if(parser->token.type == TOKEN_DEFINE && expand_defines == DO_EXPAND_DEFINES) {
            NutsDefine define = {
                .name = parser->token.name,
            };
            NutsDefine* found = nuts_define_map_find(&parser->define_map, define);
            if(found) {
                //c_print_format("Read !%.*s\n", define.name.length, define.name.data);
                token_stream_advance(parser->stream_stack_top);
                NutsTokenStream token_stream;
                token_stream_init(&token_stream, found->token_list);
                nuts_token_stream_stack_push(&parser->stream_stack, token_stream);
                parser->stream_stack_top = nuts_token_stream_stack_top(&parser->stream_stack);
                parser->token = token_stream_current_token(parser->stream_stack_top);
                continue;
            } else {
                c_error_format("Define \"%.*s\" not found\n", (int)parser->token.name.length, parser->token.name.data);
                debug_break;
            }
        } else if(parser->token.type == TOKEN_EOF) {
            nuts_token_stream_stack_pop(&parser->stream_stack);
            if(nuts_token_stream_stack_is_empty(&parser->stream_stack)) {
                return;
            }
            parser->stream_stack_top = nuts_token_stream_stack_top(&parser->stream_stack);
            parser->token = token_stream_current_token(parser->stream_stack_top);
            continue;
        }
        break;
    };
}


static
void parser_advance_stream(NutsParser* parser) {
    token_stream_advance(parser->stream_stack_top);
    parser_update_stream(parser, DO_EXPAND_DEFINES);
    parser->token_count++;
    /* c_print_format("%s(%.*s)\n", token_names[parser->token.type], */
    /*        parser->token.string.length, parser->token.string.data); */
}


static warn_unused_result
NutsResult parser_expect(NutsParser* parser, NutsTokenType token_type) {
    if(parser->token.type != token_type) {
        error_add(parser->error_list, (NutsError) {
                .type = ERROR_EXPECTED_TOKEN,
                //.text_pos = parser->token.text_pos,
                .expected_token.expected_type = token_type,
                .expected_token.actual_type   = parser->token.type
            });
        return RESULT_ERROR;
    }
    return RESULT_OK;
}


static warn_unused_result
NutsResult parse_incsrc(NutsParser* parser, NutsStatement* stmt) {
    (void)stmt;
    parser_advance_stream(parser);
    return_on_error(parser_expect(parser, TOKEN_STRING));
    parser->needed_token_stream_file_name = parser->token.name;
    parser_advance_stream(parser);
    return RESULT_OK;
}


static warn_unused_result
NutsResult parse_incbin(NutsParser* parser, NutsStatement* stmt) {
    parser_advance_stream(parser);
    return_on_error(parser_expect(parser, TOKEN_STRING));

    stmt->incbin.file_name = parser->token.name;
    
    parser_advance_stream(parser);
    stmt->type = STATEMENT_TYPE_INCBIN;
    stmt->incbin.include_somewhere_else = 0;

    if(parser->token.type == TOKEN_ARROW) {
        parser_advance_stream(parser);
        //TextPos text_pos = parser->token.text_pos;
        NutsExpr expr;
        return_on_error(parse_expr(parser, &expr));
        NutsValue value;
        return_on_error(static_eval_expr(&expr, &value, parser->error_list));
        if(value.type != VALUE_INT) {
            error_add(parser->error_list, (NutsError) {
                    .type = ERROR_EXPR_NOT_INT
                    //.text_pos = text_pos
                });
            return RESULT_ERROR;
        }
        stmt->incbin.location = expr;
        stmt->incbin.include_somewhere_else = 1;
    }
    return RESULT_OK;
}

#if 0
static
Result parse_table(Parser* parser, Statement* stmt) {
    parser_advance_stream(parser);
    return_on_error(parser_expect(parser, TOKEN_STRING));
    stmt->type = STATEMENT_TYPE_TABLE;
    stmt->table.file_name = parser->token.name;
    parser_advance_stream(parser);
    return RESULT_OK;
}

static
Result parse_warnpc(Parser* parser, Statement* stmt) {
    parser_advance_stream(parser);
    Expr expr;
    return_on_error(parse_expr(parser, &expr));
    stmt->warnpc.expr = expr;
    stmt->type = STATEMENT_TYPE_WARNPC;
    return RESULT_OK;
}

static
Result parse_fill(Parser* parser, Statement* stmt) {
    parser_advance_stream(parser);
    Expr expr;
    return_on_error(parse_expr(parser, &expr));
    stmt->fill.length = expr;
    stmt->type = STATEMENT_TYPE_FILL;
    return RESULT_OK;
}


static
Result parse_fillbyte(Parser* parser, Statement* stmt) {
    parser_advance_stream(parser);
    Expr expr;
    return_on_error(parse_expr(parser, &expr));
    stmt->fillbyte.expr = expr;
    stmt->type = STATEMENT_TYPE_FILLBYTE;
    return RESULT_OK;
}

static
Result parse_base(Parser* parser, Statement* stmt) {
    stmt->base.off = 0;
    parser_advance_stream(parser);
    if(parser->token.type == TOKEN_IDENTIFIER && parser->token.identifier->type == COMMAND_OFF) {
        parser_advance_stream(parser);
        parser->current_bank = parser->current_org_bank;
        stmt->base.off = 1;
    } else {
        //TextPos text_pos = parser->token.text_pos;
        Expr expr;
        return_on_error(parse_expr(parser, &expr));
        Value value;
        return_on_error(static_eval_expr(&expr, &value, /*text_pos,*/ parser->error_list));
        if(value.type != VALUE_INT) {
            error_add(parser->error_list, (Error) {
                .type = ERROR_EXPR_NOT_INT,
                //.text_pos = text_pos
            });
            return RESULT_ERROR;
        }
        parser->current_bank = value.i >> 16;
        stmt->base.addr = expr;
    }
    stmt->type = STATEMENT_TYPE_BASE;
    return RESULT_OK;
}

static
NutsResult parse_cleartable(Parser* parser, Statement* stmt) {
    parser_advance_stream(parser);
    stmt->type = STATEMENT_TYPE_CLEARTABLE;
    return RESULT_OK;
}
#endif

static warn_unused_result
NutsResult parse_org(NutsParser* parser, NutsStatement* stmt) {
    parser_advance_stream(parser);
    NutsExpr expr;
    return_on_error(parse_expr(parser, &expr));
    NutsValue value = { 0 };
    return_on_error(static_eval_expr(&expr, &value, parser->error_list));
    if(value.type != VALUE_INT) {
        error_add(parser->error_list, (NutsError) {
            .type = ERROR_EXPR_NOT_INT,
            //.text_pos = text_pos
        });
        return RESULT_ERROR;
    }
    stmt->org.addr = expr;
    
    stmt->type = STATEMENT_TYPE_ORG;
    return RESULT_OK;
}

static warn_unused_result
NutsResult parse_db(NutsParser* parser, NutsWidth width, NutsStatement* stmt) {
    stmt->db.list_sentinel.next = &stmt->db.list_sentinel;
    NutsExprList* node = stmt->db.list_sentinel.next;
    parser_advance_stream(parser);
    NutsExpr expr;
    return_on_error(parse_expr(parser, &expr));
    NutsExpr* new_expr = arena_alloc_array(parser->arena, 1, NutsExpr);
    *new_expr = expr;
    NutsExprList* new_node = arena_alloc_array(parser->arena, 1, NutsExprList);
    new_node->expr = new_expr;
    node->next = new_node;
    node = new_node;
    while(!token_seperates_statements(parser->token)) {
        if(parser->token.type != ',') {
            error_add(parser->error_list, (NutsError) {
                .type = ERROR_UNEXPECTED_TOKEN,
                .text_pos = get_text_pos(parser->stream_stack_top->text, parser->token.string)
            });
            return RESULT_ERROR;
        } else {
            parser_advance_stream(parser);
            return_on_error(parse_expr(parser, &expr));
            new_expr = arena_alloc_array(parser->arena, 1, NutsExpr);
            *new_expr = expr;
            NutsExprList* new_node = arena_alloc_array(parser->arena, 1, NutsExprList);
            new_node->expr = new_expr;
            node->next = new_node;
            node = new_node;
        }
    }
    node->next = &stmt->db.list_sentinel;
    stmt->db.width = width;
    stmt->type = STATEMENT_TYPE_DB;
    return RESULT_OK;
}

static warn_unused_result
NutsResult parse_statement(NutsParser* parser, NutsStatement* stmt);

static warn_unused_result
NutsResult parse_if(NutsParser* parser) {
    parser_advance_stream(parser);
    NutsExpr expr;
    return_on_error(parse_expr(parser, &expr));
    NutsValue value;
    return_on_error(static_eval_expr(&expr, &value, /*text_pos,*/ parser->error_list));
    if(value.type != VALUE_INT) {
        error_add(parser->error_list, (NutsError) {
            .type = ERROR_EXPR_NOT_INT,
            //.text_pos = text_pos
        });
        return RESULT_ERROR;
    }
    if(!token_seperates_statements(parser->token)) {
        error_add(parser->error_list, (NutsError) {
            .type = ERROR_UNTERMINATED_STATEMENT,
            .text_pos = get_text_pos(parser->stream_stack_top->text, parser->token.string),
        });
    }
    parser_advance_stream(parser);
    while(parser->token.type != TOKEN_IDENTIFIER ||
          (!identifier_is_command(parser->token.identifier->type, COMMAND_ELSE) &&
           !identifier_is_command(parser->token.identifier->type, COMMAND_ENDIF))) {
        if(parser->token.type == TOKEN_EOF) {
            // TODO: Better error handling.
            //       The error location is the unclosed if branch
            error_add(parser->error_list, (NutsError) {
                .type = ERROR_UNTERMINATED_IF,
                .text_pos = get_text_pos(parser->stream_stack_top->text, parser->token.string),
            });
            return RESULT_ERROR;
        }
        if(value.i) {
            NutsStatement stmt;
            // BUG: This should be used.
            NutsResult result = parse_statement(parser, &stmt);
            unused(result);
        } else {
            parser_advance_stream(parser);
        }
    }
    
    if(parser->token.identifier->type == (COMMAND | COMMAND_ELSE)) {
        parser_advance_stream(parser);
        while(parser->token.type != TOKEN_IDENTIFIER ||
              parser->token.identifier->type != (COMMAND | COMMAND_ENDIF)) {
            if(parser->token.type == TOKEN_EOF) {
                // TODO: Better error handling.
                //       The error location is the unclosed if branch
                //error_at_pos(parser->token.text_pos);
                error_add(parser->error_list, (NutsError) {
                    .type = ERROR_UNTERMINATED_IF,
                    .text_pos = get_text_pos(parser->stream_stack_top->text, parser->token.string),
                });
                return RESULT_ERROR;
            }
            if(!value.i) {
                NutsStatement stmt;
                // BUG: This should be used.
                NutsResult result = parse_statement(parser, &stmt);
                unused(result);
            } else {
                parser_advance_stream(parser);
            }
        }
    }
    parser_advance_stream(parser);
    return RESULT_OK;
}

static warn_unused_result
NutsResult statement_wdc65816(NutsParser* parser,
                          NutsStatement* stmt,
                          NutsToken opcode,
                          NutsOpcodeParseMode parse_mode,
                          NutsExpr expr1, NutsExpr expr2) {
    (void)parser;
    u8 op_type = opcode.identifier->type;
    stmt->wdc65816 = (NutsOpEncoding) {
        .op_type = op_type,
        .parse_mode = parse_mode,
        .expr1   = expr1,
        .expr2   = expr2,
        .width   = (opcode.identifier->width != WIDTH_UNSPECIFIED
                    ? opcode.identifier->width
                    : expr1.width),
    };
    stmt->type = STATEMENT_TYPE_WDC65816;
    return RESULT_OK;
}

static warn_unused_result
NutsResult parse_wdc65816(NutsParser* parser, NutsStatement* stmt) {
    NutsToken opcode = parser->token;
    (void)opcode;
    parser_advance_stream(parser);

    if(token_seperates_statements(parser->token)) {
        return_on_error(statement_wdc65816(parser, stmt, opcode, PARSE_MODE_IMPLIED,
                                           null_expr, null_expr));
    } else if(parser->token.type == TOKEN_IDENTIFIER &&
              identifier_is_command(parser->token.identifier->type, COMMAND_A)) {
        parser_advance_stream(parser);
        return_on_error(statement_wdc65816(parser, stmt, opcode, PARSE_MODE_ACCUMULATOR,
                                           null_expr, null_expr));
    } else if(parser->token.type ==  '#') {
        parser_advance_stream(parser);
        NutsExpr expr;
        return_on_error(parse_expr(parser, &expr));
        return_on_error(statement_wdc65816(parser, stmt, opcode, PARSE_MODE_IMMEDIATE,
                                           expr, null_expr));
    } else if(parser->token.type == '(') {
        parser_advance_stream(parser);
        NutsExpr expr;
        return_on_error(parse_expr(parser, &expr));
        if(parser->token.type == ',') {
            parser_advance_stream(parser);
            if(parser->token.type == TOKEN_IDENTIFIER &&
               identifier_is_command(parser->token.identifier->type, COMMAND_X)) {
                parser_advance_stream(parser);
                return_on_error(parser_expect(parser, ')'));
                return_on_error(statement_wdc65816(parser, stmt, opcode, PARSE_MODE_X_INDIRECT,
                                                   expr, null_expr));
            } else if(parser->token.type == TOKEN_IDENTIFIER &&
                      identifier_is_command(parser->token.identifier->type, COMMAND_S)) {
                parser_advance_stream(parser);
                return_on_error(parser_expect(parser, ')'));
                parser_advance_stream(parser);
                return_on_error(parser_expect(parser, ','));
                parser_advance_stream(parser);
                if(parser->token.type != TOKEN_IDENTIFIER ||
                   !identifier_is_command(parser->token.identifier->type, COMMAND_Y)) {
                    error_add(parser->error_list, (NutsError) {
                        .type = ERROR_UNEXPECTED_TOKEN,
                        .text_pos = get_text_pos(parser->stream_stack_top->text, parser->token.string)
                    });
                    return RESULT_ERROR;
                }
                return_on_error(statement_wdc65816(parser, stmt, opcode, PARSE_MODE_S_INDIRECT_Y,
                                                   expr, null_expr));
            } else {
                error_add(parser->error_list, (NutsError) {
                    .type = ERROR_UNEXPECTED_TOKEN,
                    .text_pos = get_text_pos(parser->stream_stack_top->text, parser->token.string)
                });
                return RESULT_ERROR;
            }
            parser_advance_stream(parser);

        } else if(parser->token.type == ')') {
            parser_advance_stream(parser);
            if(token_seperates_statements(parser->token)) {
                return_on_error(statement_wdc65816(parser, stmt, opcode, PARSE_MODE_INDIRECT,
                                                   expr, null_expr));
            } else if(parser->token.type == ',') {
                parser_advance_stream(parser);
                if(parser->token.type != TOKEN_IDENTIFIER ||
                   !identifier_is_command(parser->token.identifier->type, COMMAND_Y)) {
                    error_add(parser->error_list, (NutsError) {
                        .type = ERROR_UNEXPECTED_TOKEN,
                        .text_pos = get_text_pos(parser->stream_stack_top->text, parser->token.string)
                    });
                    return RESULT_ERROR;
                }
                parser_advance_stream(parser);
                return_on_error(statement_wdc65816(parser, stmt, opcode, PARSE_MODE_INDIRECT_Y,
                                                   expr, null_expr));
            }
        } else {
            error_add(parser->error_list, (NutsError) {
                .type = ERROR_UNEXPECTED_TOKEN,
                .text_pos = get_text_pos(parser->stream_stack_top->text, parser->token.string)
            });
            return RESULT_ERROR;
        }
    } else if(parser->token.type == '[') {
        parser_advance_stream(parser);
        NutsExpr expr;
        return_on_error(parse_expr(parser, &expr));
        return_on_error(parser_expect(parser, ']'));
        parser_advance_stream(parser);
        if(parser->token.type == ',') {
            parser_advance_stream(parser);
            if(parser->token.type != TOKEN_IDENTIFIER ||
               !identifier_is_command(parser->token.identifier->type, COMMAND_Y)) {
                error_add(parser->error_list, (NutsError) {
                    .type = ERROR_UNEXPECTED_TOKEN,
                    .text_pos = get_text_pos(parser->stream_stack_top->text, parser->token.string)
                });
                return RESULT_ERROR;
            }
            parser_advance_stream(parser);
            return_on_error(statement_wdc65816(parser, stmt, opcode, PARSE_MODE_LONG_INDIRECT_Y,
                                               expr, null_expr));
        } else if(token_seperates_statements(parser->token)) {
            return_on_error(statement_wdc65816(parser, stmt, opcode, PARSE_MODE_LONG_INDIRECT,
                                               expr, null_expr));
        } else {
            error_add(parser->error_list, (NutsError) {
                .type = ERROR_UNEXPECTED_TOKEN,
                .text_pos = get_text_pos(parser->stream_stack_top->text, parser->token.string)
            });
            return RESULT_ERROR;
        }
    } else {
        NutsExpr expr;
        return_on_error(parse_expr(parser, &expr));
        if(parser->token.type == ',') {
            parser_advance_stream(parser);
            if(parser->token.type == TOKEN_IDENTIFIER &&
               identifier_is_command(parser->token.identifier->type, COMMAND_X)) {
                parser_advance_stream(parser);
                return_on_error(statement_wdc65816(parser, stmt, opcode, PARSE_MODE_DIRECT_X,
                                                   expr, null_expr));
            } else if(parser->token.type == TOKEN_IDENTIFIER &&
                      identifier_is_command(parser->token.identifier->type, COMMAND_Y)) {
                parser_advance_stream(parser);
                return_on_error(statement_wdc65816(parser, stmt, opcode, PARSE_MODE_DIRECT_Y,
                                                   expr, null_expr));
            } else if(parser->token.type == TOKEN_IDENTIFIER &&
                      identifier_is_command(parser->token.identifier->type, COMMAND_S)) {
                parser_advance_stream(parser);
                return_on_error(statement_wdc65816(parser, stmt, opcode, PARSE_MODE_DIRECT_S,
                                                   expr, null_expr));
            } else {
                NutsExpr expr2;
                return_on_error(parse_expr(parser, &expr2));
                return_on_error(statement_wdc65816(parser, stmt, opcode, PARSE_MODE_SRC_DEST,
                                                   expr, expr2));
            }
        } else if(token_seperates_statements(parser->token)) {
            return_on_error(statement_wdc65816(parser, stmt, opcode, PARSE_MODE_DIRECT,
                                               expr, null_expr));
        }
    }
    return RESULT_OK;
}

static warn_unused_result
NutsResult parse_label_def(NutsParser* parser,
                           NutsStatement* stmt) {
    stmt->type = STATEMENT_TYPE_LABEL_DEF;
    NutsToken label = parser->token;
    NutsIdentifier* ident = label_full_name(label.string,
                                        parser->last_label_name,
                                        &parser->ast->identifier_map);
    stmt->label_def.name = ident->name;
    parser->last_label_name = ident->name;
    parser_advance_stream(parser);
    return RESULT_OK;
}

static warn_unused_result
NutsResult parse_statement(NutsParser* parser, NutsStatement* stmt) {
    stmt->type = STATEMENT_TYPE_NULL;
    stmt->tokens.data = parser->stream_stack_top->pos;
    stmt->tokens.text = parser->stream_stack_top->text;
    uint token_begin_count = parser->token_count;
    switch(parser->token.type) {
    case TOKEN_IDENTIFIER: {
        if(identifier_type_is_command(parser->token.identifier->type)) {
            switch(parser->token.identifier->type & 0xFF) {
#if 0
            case COMMAND_TABLE:      { return_on_error(parse_table     (parser, stmt)); break; }
            case COMMAND_WARNPC:     { return_on_error(parse_warnpc    (parser, stmt)); break; }
            case COMMAND_FILL:       { return_on_error(parse_fill      (parser, stmt)); break; }
            case COMMAND_FILLBYTE:   { return_on_error(parse_fillbyte  (parser, stmt)); break; }
            case COMMAND_BASE:       { return_on_error(parse_base      (parser, stmt)); break; }
            case COMMAND_CLEARTABLE: { return_on_error(parse_cleartable(parser, stmt)); break; }
#endif
            case COMMAND_ORG:        { return_on_error(parse_org       (parser, stmt)); break; }
            case COMMAND_INCBIN:     { return_on_error(parse_incbin    (parser, stmt)); break; }
            case COMMAND_INCSRC:     {
                return_on_error(parse_incsrc(parser, stmt));
                return RESULT_NEED_TOKEN_STREAM;
            }

            case COMMAND_DB:         { return_on_error(parse_db(parser, WIDTH_1, stmt));      break; }
            case COMMAND_DW:         { return_on_error(parse_db(parser, WIDTH_2, stmt));      break; }
            case COMMAND_DL:         { return_on_error(parse_db(parser, WIDTH_3, stmt));      break; }
            case COMMAND_DD:         { return_on_error(parse_db(parser, WIDTH_4, stmt));      break; }

            case COMMAND_IF:         { return_on_error(parse_if(parser));               break; }
            case COMMAND_ELSE: {
                error_add(parser->error_list, (NutsError) {
                    .type = ERROR_ELSE_WITHOUT_IF,
                    .text_pos = get_text_pos(parser->stream_stack_top->text, parser->token.string)
                });
                return RESULT_ERROR;
            }
            case COMMAND_ENDIF: {
                error_add(parser->error_list, (NutsError) {
                    .type = ERROR_ENDIF_WITHOUT_IF,
                    .text_pos = get_text_pos(parser->stream_stack_top->text, parser->token.string)
                });
                return RESULT_ERROR;
            }
            default:
                error_add(parser->error_list, (NutsError) {
                    .type = ERROR_UNEXPECTED_TOKEN,
                    .text_pos = get_text_pos(parser->stream_stack_top->text, parser->token.string)
                });
                return RESULT_ERROR;
            }
        } else if(identifier_type_is_opcode(parser->token.identifier->type)) {
            return_on_error(parse_wdc65816(parser, stmt));
        } else {
            assert(identifier_type_is_identifier(parser->token.identifier->type));
            return_on_error(parse_label_def(parser, stmt)); break;
            break;
            
        }
        break;
    }
    case TOKEN_DEFINE: {
        NutsText text = parser->stream_stack_top->text;
        String name = parser->token.name;
        parser_advance_stream(parser);
        NutsTokenStream* stream = parser->stream_stack_top;
        if(parser->token.type == '=') {
            stream->pos++;
            NutsDefine define = {
                .name = name,
                .token_list.data = stream->pos
            };
            while(!token_seperates_statements(*stream->pos)) {
                stream->pos++;
                define.token_list.length++;
            }
            nuts_define_map_insert(&parser->define_map, define);
            parser_update_stream(parser, DO_EXPAND_DEFINES);
        } else {
            error_add(parser->error_list, (NutsError) {
                .type = ERROR_UNDEFINED_DEFINE,
                .text_pos = get_text_pos(text, name)
            });
            return RESULT_ERROR;
        }
        stmt->type = STATEMENT_TYPE_EMPTY;
        break;
    }

    case '+':
    case '-':
    case TOKEN_ANON_LABEL: {
        // TODO: Implement this.
        parser_advance_stream(parser);
        break;
    }

    case TOKEN_COMMENT:
    case TOKEN_CMD_DELIM: {
        stmt->type = STATEMENT_TYPE_EMPTY;
        break;
    }

    default: {
        error_add(parser->error_list, (NutsError) {
            .type = ERROR_UNEXPECTED_TOKEN,
            .text_pos = get_text_pos(parser->stream_stack_top->text, parser->token.string)
        });
        return RESULT_ERROR;
    }
    }

    if(parser->token.type == TOKEN_COMMENT) {
        parser_advance_stream(parser);
    }
    if(!token_seperates_statements(parser->token)) {
        error_add(parser->error_list, (NutsError) {
            .type = ERROR_UNTERMINATED_STATEMENT,
            .text_pos = get_text_pos(parser->stream_stack_top->text, parser->token.string)
        });
        return RESULT_ERROR;
    }

    // NOTE: We don't want to expand defines at
    // the start of a new statement to
    // distinguish between define definition
    // and define expansion.
    uint token_end_count = parser->token_count;
    stmt->tokens.length = token_end_count - token_begin_count;

    token_stream_advance(parser->stream_stack_top);
    parser_update_stream(parser, DONT_EXPAND_DEFINES);

    return RESULT_OK;
}

warn_unused_result
NutsResult nuts_parse(NutsParser* parser, NutsTokenList token_list) {
    NutsTokenStream stream;
    token_stream_init(&stream, token_list);
    nuts_token_stream_stack_push(&parser->stream_stack, stream);
    parser->stream_stack_top = nuts_token_stream_stack_top(&parser->stream_stack);
    parser_update_stream(parser, DONT_EXPAND_DEFINES);
    NutsStatementList* stmt_list = &parser->ast->statement_list;
    NutsResult parse_result = RESULT_OK;
    while(parser->token.type != TOKEN_EOF) {
        NutsStatement* stmt = stmt_list->data + stmt_list->length;
        assert(stmt_list->length < 512 * 1024);
        NutsResult parse_statement_result = parse_statement(parser, stmt);
        if(parse_statement_result == RESULT_ERROR) {
            do {
                token_stream_advance(parser->stream_stack_top);
                parser_update_stream(parser, DO_EXPAND_DEFINES);
            } while(!token_seperates_statements(parser->token));
            parse_result = RESULT_ERROR;
        } else if(parse_statement_result == RESULT_NEED_TOKEN_STREAM) {
            parse_result = RESULT_NEED_TOKEN_STREAM;
            break;
        }
        stmt_list->length++;
    }
    return parse_result;
}

#define LABEL_ADDR_UNSPECIFIED 0xFFFFFFFF

static
u32 nuts_label_hash(NutsLabel e) {
    return hash_murmur3_string(e.name);
}

static
u32 nuts_label_equal(NutsLabel e1, NutsLabel e2) {
    return string_equal(e1.name, e2.name);
}

implement_hashmap(NutsLabelMap, nuts_label_map, NutsLabel, nuts_label_hash, nuts_label_equal);


static
NutsResult assembler_write_bytes(NutsAssembler* assembler, u32 bytes, int width) {
    for(int i = 0; i < width; i++) {
        if(assembler->last_pass) {
            wdc65816_mapper_write(assembler->rom, assembler->addr, bytes);
        }
        assembler->addr++;
        bytes >>= 8;
    }
    return RESULT_OK;
}


static
NutsResult assembler_write_expr(NutsAssembler* assembler, NutsExpr* expr, uint width) {
    NutsValue value = { };
    return_on_error(eval_expr(expr, &value, &assembler->label_map, assembler->error_list));
    if(value.type == VALUE_INT) {
        return assembler_write_bytes(assembler, value.i, width);
    } else if(value.type == VALUE_STRING) {
        for(uint i = 0; i < value.s.length; i++) {
            return_on_error(assembler_write_bytes(assembler, value.s.data[i], width));
        }
        return RESULT_OK;
    } else if(value.type == VALUE_LABEL) {
        return assembler_write_bytes(assembler, value.i, width);
    } else {
        invalid_code_path;
        return RESULT_ERROR;
    }
}

static
NutsResult assembler_write_buffer(NutsAssembler* assembler, Buffer buffer) {
    uint length = buffer_length(buffer);
    if(assembler->last_pass) {
        wdc65816_mapper_write_range(assembler->rom, assembler->addr, assembler->addr + length, buffer.begin);
    }
    assembler->addr += length;
    return RESULT_OK;
}


void nuts_assembler_init(NutsAssembler* assembler,
                         NutsErrorList* error_list,
                         NutsAST* ast,
                         Wdc65816Mapper* rom,
                         FreeList* free_list) {
    assembler->ast = ast;
    assembler->current_statement = ast->statement_list.data;
    assembler->error_list = error_list;
    assembler->rom = rom;
    nuts_label_map_init(&assembler->label_map, free_list, 4096);
    assembler->buffer = (Buffer) { }; 
    assembler->addr = 0; 
    assembler->last_pass = 0;
    assembler->rerun = 0;
    assembler->pass = 1; 
    assembler->max_num_passes = 2;
}

NutsResult nuts_assemble(NutsAssembler* assembler) {
    NutsStatementList* stmt_list = &assembler->ast->statement_list;
    NutsStatement* stmt_end = stmt_list->data + stmt_list->length;
    for(;assembler->pass <= assembler->max_num_passes; assembler->pass++) {
        if(assembler->pass == assembler->max_num_passes) {
            assembler->last_pass = 1;
        }
        for(; assembler->current_statement < stmt_end; assembler->current_statement++) {
            NutsStatement* stmt = assembler->current_statement;
            switch(stmt->type) {
            case STATEMENT_TYPE_EMPTY: { break; };
            case STATEMENT_TYPE_INCBIN: {
                if(assembler->buffer.begin != NULL) {
                    return_on_error(assembler_write_buffer(assembler,
                                                           assembler->buffer));
                    assembler->buffer.begin = NULL;
                } else {
                    assembler->file_name = stmt->incbin.file_name;
                    return RESULT_NEED_FILE;
                }
                break;
            }
            case STATEMENT_TYPE_DB: {
                NutsWidth width  = stmt->db.width;
                
                for(NutsExprList* node = stmt->db.list_sentinel.next;
                    node != &stmt->db.list_sentinel;
                    node = node->next) {
                    assembler_write_expr(assembler, node->expr, width);
                }
                break;
            }
            case STATEMENT_TYPE_ORG: {
                NutsValue value = { 0 };
                return_on_error(static_eval_expr(&stmt->org.addr, &value, assembler->error_list));
                if(value.type != VALUE_INT) {
                    error_add(assembler->error_list, (NutsError) {
                        .type = ERROR_EXPR_NOT_INT,
                        //.text_pos = text_pos
                    });
                    return RESULT_ERROR;
                }
                assembler->addr = value.i;
                /* c_print_format("org %06x\n", assembler->addr); */
                break;
            }
            case STATEMENT_TYPE_LABEL_DEF: {
                String label_name = stmt->label_def.name;
                NutsLabel* found = nuts_label_map_find(&assembler->label_map,
                                              (NutsLabel){ .name = label_name });
                if(found) {
                    if(found->pass == assembler->pass) {
                        error_add(assembler->error_list, (NutsError) {
                            .type = ERROR_LABEL_ALREADY_DEFINED,
                            //.text_pos = get_text_pos(parser->stream_stack_top->text, parser->token.string)
                        });
                    }
                    if(found->addr != assembler->addr) {
                        if(assembler->last_pass) {
                            error_add(assembler->error_list, (NutsError) {
                                .type = ERROR_FLOATING_LABEL,
                                //.text_pos = get_text_pos(parser->stream_stack_top->text, parser->token.string)
                            });
                        }
                        assembler->rerun = 1;
                    }
                    found->addr = assembler->addr;
                    found->pass = assembler->pass;
                } else {
                    nuts_label_map_insert(&assembler->label_map,
                                     (NutsLabel){ .name = stmt->label_def.name, .pass = assembler->pass, .addr = assembler->addr });
                }
                break;
            }
            case STATEMENT_TYPE_WDC65816: {
                u8 op_type = stmt->wdc65816.op_type;
                NutsOpcodeParseMode parse_mode = stmt->wdc65816.parse_mode;
                NutsWidth width = stmt->wdc65816.width;
                NutsOpcodeAssembleData opcode_data = opcode_assemble_data_table[op_type][parse_mode][width];
                switch(opcode_data.mode) {
                case ASSEMBLE_MODE_0: {
                    assembler_write_bytes(assembler, opcode_data.byte, 1);
                    break;
                }
                case ASSEMBLE_MODE_1: {
                    assembler_write_bytes(assembler, opcode_data.byte, 1);
                    assembler_write_expr(assembler, &stmt->wdc65816.expr1, WIDTH_1);
                    break;
                }
                case ASSEMBLE_MODE_2: {
                    assembler_write_bytes(assembler, opcode_data.byte, 1);
                    assembler_write_expr(assembler, &stmt->wdc65816.expr1, WIDTH_2);
                    break;
                }
                case ASSEMBLE_MODE_3: {
                    assembler_write_bytes(assembler, opcode_data.byte, 1);
                    assembler_write_expr(assembler, &stmt->wdc65816.expr1, WIDTH_3);
                    break;
                }
                case ASSEMBLE_MODE_R: {
                    NutsValue value = { };
                    NutsResult result = eval_expr(&stmt->wdc65816.expr1, &value,
                                              &assembler->label_map, assembler->error_list);
                    if(result == RESULT_UNDEFINED_LABEL) {
                        if(assembler->last_pass) {
                            error_add(assembler->error_list, (NutsError) {
                                .type = ERROR_UNDEFINED_LABEL,
                                //.text_pos = get_text_pos(parser->stream_stack_top->text, parser->token.string)
                            });
                        }
                        assembler->rerun = 1;
                    }
#if 0
                    if(value.i < 0) {
                        error_add(assembler->error_list, (Error) {
                            .type = ERROR_VALUE_NEGATIVE,
                            //.text_pos = get_text_pos(parser->stream_stack_top->text, parser->token.string)
                        });
                    }
#endif
                    for(u32 i = 0; i < value.i; i++) {
                        assembler_write_bytes(assembler, opcode_data.byte, 1);
                    }
                    break;
                }

                case ASSEMBLE_MODE_J: {
                    NutsValue value = { };
                    NutsResult result = eval_expr(&stmt->wdc65816.expr1, &value,
                                              &assembler->label_map, assembler->error_list);
                    if(result == RESULT_UNDEFINED_LABEL) {
                        if(assembler->last_pass) {
                            error_add(assembler->error_list, (NutsError) {
                                .type = ERROR_UNDEFINED_LABEL,
                                //.text_pos = get_text_pos(parser->stream_stack_top->text, parser->token.string)
                            });
                        }
                    }
                    
                    i32 diff = (i32)value.i - (i32)assembler->addr - 2;
                    if(value.type == VALUE_LABEL) {
                        if(diff < -128 || diff > 127) {
                            if(assembler->last_pass) {
                                error_add(assembler->error_list, (NutsError) {
                                   .type = ERROR_JUMP_TO_LONG,
                                   //.text_pos = get_text_pos(parser->stream_stack_top->text, parser->token.string)
                                });
                            }
                            assembler->rerun = 1;
                        }
                        assembler_write_bytes(assembler, opcode_data.byte, 1);
                        assembler_write_bytes(assembler, diff, 1);
                    } else {
                        assembler_write_bytes(assembler, opcode_data.byte, 1);
                        assembler_write_bytes(assembler, value.i, 1);
                    }
                    break;
                }
                case ASSEMBLE_MODE_K: {
                    NutsValue value = { };
                    NutsResult result = eval_expr(&stmt->wdc65816.expr1, &value,
                                              &assembler->label_map, assembler->error_list);
                    if(result == RESULT_UNDEFINED_LABEL) {
                        if(assembler->last_pass) {
                            error_add(assembler->error_list, (NutsError) {
                                .type = ERROR_UNDEFINED_LABEL,
                                //.text_pos = get_text_pos(parser->stream_stack_top->text, parser->token.string)
                            });
                        }
                        assembler->rerun = 1;
                    } 
                    i32 diff = (i32)value.i - (i32)assembler->addr - 3;
                    if(value.type == VALUE_LABEL) {
                        if(diff < -32768 || diff > 32767) {
                            if(assembler->last_pass) {
                                error_add(assembler->error_list, (NutsError) {
                                   .type = ERROR_JUMP_TO_LONG,
                                   //.text_pos = get_text_pos(parser->stream_stack_top->text, parser->token.string)
                                });
                            }
                            assembler->rerun = 1;
                        }
                        assembler_write_bytes(assembler, opcode_data.byte, 1);
                        assembler_write_bytes(assembler, diff, 2);
                    } else {
                        assembler_write_bytes(assembler, opcode_data.byte, 1);
                        assembler_write_bytes(assembler, value.i, 2);
                    }
                    break;
                }
                case ASSEMBLE_MODE_S: {
                    assembler_write_bytes(assembler, opcode_data.byte, 1);
                    assembler_write_expr(assembler, &stmt->wdc65816.expr1, 1);
                    assembler_write_expr(assembler, &stmt->wdc65816.expr2, 1);
                    break;
                }
                case ASSEMBLE_MODE_X: {
                    NutsValue value = { };
                    NutsResult result = eval_expr(&stmt->wdc65816.expr1, &value,
                                              &assembler->label_map, assembler->error_list);
                    if(result == RESULT_UNDEFINED_LABEL) {
                        if(assembler->last_pass) {
                            error_add(assembler->error_list, (NutsError) {
                                .type = ERROR_UNDEFINED_LABEL,
                                //.text_pos = get_text_pos(parser->stream_stack_top->text, parser->token.string)
                            });
                        }
                        assembler->rerun = 1;
                    }
                    u32 bank = value.i >> 16;
                    if(bank != 0 && bank != assembler->addr >> 16) {
                        if(assembler->last_pass) {
                            error_add(assembler->error_list, (NutsError) {
                                .type = ERROR_WRONG_BANK,
                                //.text_pos = get_text_pos(parser->stream_stack_top->text, parser->token.string)
                            });
                        }
                        assembler->rerun = 1;
                    }
                    return_on_error(assembler_write_bytes(assembler, opcode_data.byte, 1));
                    assembler_write_bytes(assembler, value.i, 2);
                    break;
                }
                default: {
                    error_add(assembler->error_list, (NutsError) {
                        .type = ERROR_WRONG_OPCODE_USAGE,
                        .text_pos = get_text_pos(stmt->tokens.text, stmt->tokens.data[0].string)
                    });
                    break;
                }
                }
                break;
            }
            invalid_default_case;
            }
        }
        if(assembler->last_pass && assembler->rerun) {
            error_add(assembler->error_list, (NutsError) {
                .type = ERROR_TO_MANY_PASSES,
            });
            return RESULT_ERROR;
        }
        assembler->current_statement = assembler->ast->statement_list.data;
        assembler->rerun = 0;
        assembler->addr = 0;
    }
    return RESULT_OK;
}

void nuts_assembler_give_buffer(NutsAssembler* assembler, Buffer buffer) {
    assert(buffer.begin);
    assembler->buffer = buffer;
}

String nuts_assembler_get_file_name(NutsAssembler* assembler) {
    return assembler->file_name;
}

