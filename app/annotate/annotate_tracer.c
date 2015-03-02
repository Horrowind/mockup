#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dismap.h"

#define PRELABEL  1
#define LABEL     2
#define POSTLABEL 3
#define HEX       4
#define CODE      5
#define DATA      6
#define COMMENT   7

int main(int argc, char* argv[]) {
    /* xmlDocPtr doc = xmlNewDoc("1.0"); */
    /* xmlNodePtr root_node = xmlNewNode(NULL, "annotate"); */
    /* xmlDocSetRootElement(doc, root_node); */
    /* xmlNewChild(root_node, NULL, BAD_CAST "node1", */
    /*             BAD_CAST "content of node 1"); */
    /* if(argc != 2) { */
    /*     return -1; */
    /* } */
    FILE* fp = fopen(argv[1], "r");
    if(fp == 0) return -1;
    fseek(fp, 0, SEEK_END);
    unsigned int size = ftell(fp);
    unsigned char* buffer = malloc(size);
    rewind(fp);
    for(int i = 0; i < size; i++) buffer[i] = fgetc(fp);

    dismap_t dismap;
    dismap_init(&dismap);
    
    
    int state = LABEL;

    char c;
    while(!feof(fp)) {
        c = fgetc(fp);
        if(c == '\n') {
            printf("\n");
            state = PRELABEL;
        } else if(c == ';') {
            printf("Comment: ");
            while((c = fgetc(fp)) != '\n') printf("%c", c);
            printf("\n");
            state = STATE;
        } else {
            if(state >= 0 && c != ' ') {
                if(c != '.') {
                    printf("Code: ");
                } else {
                    printf("Data: ");
                }
                state = -1;
            };
            if(state == -1) {
                printf("%c", c);
            } else {
                if(state == 34 && c!= ' ') {
                    printf("Label: ");
                    while(!feof(fp)) {
                        printf("%c", c);
                        c = fgetc(fp);
                        if(c == ':' || c == '\n') break;
                        state--;
                    }
                    printf(" ");
                }
                state--;
            }
        }
    }
    printf("\n");
}
