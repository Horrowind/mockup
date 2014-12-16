#ifndef MOCKUP_LAYER_H
#define MOCKUP_LAYER_H

typedef struct {
    int height;
    int width;

    uint16_t data;
    
    tile16_t* map;
    int map_length;
    
    
} layer16_t;


void layer_render_line(uint8_t* line, int linenum) {

/* void Level::renderLineFG(uint8_t* line, int linenum) { */
/*     int gy = linenum>>4; */
/*     int by = linenum%16; */
 
/*     for(int i = 0; i < l->width * 16; i++) { */
/* 	int pos = by * 16 + i % 16; */
/* 	line[i] = l->map16fg[l->layer1[gy * l->width + (i >> 4)]].pixels[pos]; */
/*     } */
 
/* } */

/* void Level::renderLineBG(uint8_t* line, int linenum) { */
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
/* uint8_t Level::renderPixel(int x, int y) { */
/*     int gx = x>>4; */
/*     int bx = x%16; */
/*     int gy = y>>4; */
/*     int by = y%16; */
/*     return l->map16fg[l->layer1[gy * l->width + gx]].pixels[by * 16 + bx]; */
/* } */





#endif //MOCKUP_LAYER_H
