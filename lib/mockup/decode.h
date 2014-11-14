#ifndef MOCKUP_DECODE_H
#define MOCKUP_DECODE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void decryptRLE1(const uint8_t* data, uint8_t* output) {
    uint8_t cmd, length;
    int pos = 0;
    uint8_t* tmp = output;
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

int decryptRLE1UnkownSize(const uint8_t* data, uint8_t* output) {
    int size = 0, pos = 0;
    uint8_t cmd, length;
    while((cmd = data[pos]) != 0xFF || data[pos + 1] != 0xFF) {
	length = cmd & 0b01111111;
	size += length + 1;
	pos += 2 + (cmd >> 7) * (cmd & 0x7F);
    }

    output = malloc(size);
    decryptRLE1(data, output);
    return size;
}

void decryptLZ2(const uint8_t* data, uint8_t* output) {
    uint8_t cmd; 
    int length;
    int pos = 0, pos_out = 0, copy_pos;
        
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

int decryptLZ2UnknownSize(const uint8_t* data, uint8_t* output) {
    uint8_t cmd, length;
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

    output = malloc(size);
    decryptLZ2(data, output);
    return size;
}


#endif //MOCKUP_DECODE_H
