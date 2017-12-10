#include "decode.h"

void decode_rle1(const u8* data, u8* output) {
    u8 cmd, length;
    int pos = 0;
    u8* tmp = output;
    while((cmd = data[pos]) != 0xFF || data[pos + 1] != 0xFF) {
        length = cmd & 0b01111111;
        if(cmd & 0x80) {
            memset(output, data[pos + 1], length + 1);
            output += length + 1;
            pos += 2;
        } else {
            memcpy(output, data + pos + 1, length + 1);
            output += length + 1;
            pos += length + 2;
        }
    }
    output = tmp;
}

int decode_rle1_get_size(const u8* data) {
    int size = 0, pos = 0;
    u8 cmd, length;
    while((cmd = data[pos]) != 0xFF || data[pos + 1] != 0xFF) {
        length = cmd & 0b01111111;
        size += length + 1;
        pos += 2 + (cmd >> 7) * (cmd & 0x7F);
    }
    return size;
}

void decode_lz2(const u8* data, u8* output) {
    u8 cmd; 
    int length;
    int pos = 0, pos_out = 0, copy_pos;
        
    while((cmd = data[pos]) != 0xFF) {
        //printf("huhu: %08x", output + pos_out);
        if((cmd & 0xE0) == 0xE0) {
            pos++;
            length = ((cmd & 0x03) << 8) + (int)data[pos] + 1;
            cmd = cmd << 3;
        } else {
            length = (cmd & 0b00011111) + 1;
        }
        switch(cmd & 0xE0) {

        case 0x00:
            memcpy(output + pos_out, data + pos + 1, length);
            pos += length + 1;
            break;

        case 0x20:
            memset(output + pos_out, data[pos + 1], length);
            pos += 2;
            break;

        case 0x40:
            for(int i = 0; i < length; i++) {
                output[pos_out + i] = data[pos + 1 + (i & 1)];
            }
            pos += 3;
            break;

        case 0x60:
            for(int i = 0; i < length; i++) {
                output[pos_out + i] = data[pos + 1] + i;
            }
            pos += 2;
            break;

        case 0x80:
            copy_pos = (data[pos + 1] << 8) | data[pos + 2];
            for(int i = 0; i < length; i++) {
                output[pos_out + i] = output[copy_pos + i];
            }
            pos += 3;
            break;

        default:
            printf("Error\n");
            //TODO: Error
            return;
        }
        pos_out += length;
    }
}

int decode_lz2_get_size(const u8* data) {
    u8 cmd, length;
    int pos = 0, size = 0;
    while((cmd = data[pos]) != 0xFF) {
        if((cmd & 0xE0) == 0xE0) {
            pos++;
            length = ((cmd & 0x03) << 8) + (int)data[pos] + 1;
            cmd = cmd << 3;
        } else {
            length = (cmd & 0b00011111) + 1;
        }
        switch(cmd & 0xE0) {
                
        case 0x00:
            pos += length + 1;
            break;

        case 0x20:
            pos += 2;
            break;

        case 0x40:
            pos += 3;
            break;

        case 0x60:
            pos += 2;
            break;

        case 0x80:
            pos += 3;
            break;

        default:
            printf("Error\n");
            //TODO: Error
            return -1;
        }
        size += length;
    }
    return size;
}

typedef enum {
    LZ2_COMMAND_DIRECT_COPY,
    LZ2_COMMAND_BYTE_FILL,
    LZ2_COMMAND_WORD_FILL,
    LZ2_COMMAND_INCREASING_FILL,
    LZ2_COMMAND_REPEAT,
    LZ2_COMMAND_UNUSED1,
    LZ2_COMMAND_UNUSED2,
    LZ2_COMMAND_LONG_CMD    
} LZ2Commands;

void encode_lz2(u8* data_begin, u8* data_end, u8* output) {
    for(u8* data_read = data_begin; data_read < data_end;) {
//        LZ2Commands best_cmd;
        uint best_length = 0xFFFFFFFF;
//        uint best_cost = 0xFFFFFFFF;

        { // Byte fill
            uint byte_fill_length = 1;
            u8* next_bytes = data_read + 1;
            while(*next_bytes == *data_read && next_bytes < data_end) {
                next_bytes++;
                byte_fill_length++;
            }
            if(best_length > byte_fill_length) {
//                best_cmd = LZ2_COMMAND_BYTE_FILL;
                best_length = byte_fill_length;
            }
        }

/*         { // Byte fill */
/*             uint byte_fill_length = 1; */
/*             u8* next_bytes = data_read + 1; */
/*             while(*next_bytes == *data_read && next_bytes < data_end) { */
/*                 next_bytes++; */
/*                 byte_fill_length++; */
/*             } */
/*             if(best_length > byte_fill_length) { */
/*                 best_cmd = LZ2_COMMAND_DIRECT_FILL; */
/*                 best_length = byte_fill_length; */
/*             } */
/*             uint byte_fill_cost = 2; */
/*         } */

/*         { // Word fill */
/*             uint word_fill_length = 1; */
/*             u8* next_bytes = data_read + 1; */
/*             while(*next_bytes == *data_read && next_bytes < data_end) { */
/*                 next_bytes++; */
/*                 byte_fill_length++; */
/*             } */
/*             if(best_length > byte_fill_length) { */
/*                 best_cmd = LZ2_COMMAND_DIRECT_FILL; */
/*                 best_length = byte_fill_length; */
/*             } */
/*         } */
    }
}
        
