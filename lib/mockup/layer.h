#ifndef MOCKUP_LAYER_H
#define MOCKUP_LAYER_H

#include "palette.h"
#include "tiles.h"

typedef struct {
    int height;
    int width;
    u16* data;
} layer16_t;

void layer16_init(layer16_t* layer);
void layer16_deinit(layer16_t* layer);


/*void layer16_render_line(layer16_t layer, int num_line, u8* line) {
    int gy = num_line>>4;
    int by = num_line%16;
    for(int i = 0; i < layer->width * 16; i++) {
	int pos = by * 16 + i % 16;
	line[i] = layer->map[layer->data[gy * layer->width + (i >> 4)]].pixels[pos];
    }
}*/

/* void Level::renderLineBG(u8* line, int linenum) { */
/*     if(l->hasLayer2BG) { */
/* 	int gy = (linenum>>4) % 27; */
/* 	int by = linenum%16; */
/* 	if(l->hasLayer2BG) { */
/* 	    for(int i = 0; i < l->width * 16; i++) { */
/* 		int pos = by * 16 + i % 16; */
/* 		line[i] = l->map16bg[l->layer2[((i & 0x100) >> 4) * 27 + gy * 16 + ((i & 0x0FF) >> 4)]].pixels[pos]; */
/* 	    } */
/* 	}  */
/*     } else if(l->hasLayer2Objects) { */
/* 	int gy = linenum>>4; */
/* 	int by = linenum%16; */
            
/* 	for(int i = 0; i < l->width * 16; i++) { */
/* 	    int pos = by * 16 + i % 16; */
/* 	    line[i] = l->map16bg[l->layer2[gy * l->width + (i >> 4)]].pixels[pos]; */
/* 	} */
/*     } else { */
/* 	for(int i = 0; i < l->width * 16; i++) line[i] = 0; */
/*     } */
/* } */

/* //TODO: Method is not updated, see renderLine(F/B)G */
/* u8 Level::renderPixel(int x, int y) { */
/*     int gx = x>>4; */
/*     int bx = x%16; */
/*     int gy = y>>4; */
/*     int by = y%16; */
/*     return l->map16fg[l->layer1[gy * l->width + gx]].pixels[by * 16 + bx]; */
/* } */





#endif //MOCKUP_LAYER_H
