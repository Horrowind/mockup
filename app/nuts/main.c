#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <linux/limits.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>


#define BASE_IMPLEMENTATION
#include "base/base.h"
#include "parse.h"

int main(int argc, char** argv) {
#if YYDEBUG!=0
    yydebug = 1;
#endif
	// open a file handle to a particular file:
    if(argc != 2) {
        printf("Run with exactly one argument.\n");
    } else {
        string_t main_path = string_from_c_string(argv[1]);
        
        string_t main_buffer = read_file_to_buffer(argv[1]);
        /* parse_state_t state = { */
        /*     .string = main_buffer, */
        /* }; */
        parse_stack_init(&parse_stack, 10);

        /* string_hash_map_init(&identifier_map, 4096*4); */
        /* defines_hash_map_init(&defines_map, 4096*4); */
        string_hash_map_init(&identifier_map, 2);
        defines_hash_map_init(&defines_map, 2);
        
        if(main_buffer.data) {
            //while(!parse_stack_is_empty(&parse_stack)) yyparse();
            push_parse_buffer(main_buffer, main_path);
            /* while(1) */
                yyparse();
        }
    }
}
