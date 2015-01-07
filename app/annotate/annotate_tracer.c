#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* #include <libxml/tree.h> */

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
    int state = 34;
    char c;
    while(!feof(fp)) {
        c = fgetc(fp);
        if(c == '\n') {
            state = 34;
        } else if(c == ';') {
            printf("Comment: ");
            while((c = fgetc(fp)) != '\n') printf("%c", c);
            printf("\n");
            state = 34;
        } else {
            if(state == 0) {
                if(c == '.') {
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
}
