#include <time.h>
#include "nuts/nuts.h"

struct timespec timespec_add(struct timespec start, struct timespec end) {
	struct timespec tmp;
	if ((end.tv_nsec+start.tv_nsec) >= 1000000000) {
		tmp.tv_sec  = end.tv_sec  + start.tv_sec  + 1;
		tmp.tv_nsec = end.tv_nsec + start.tv_nsec - 1000000000;
	} else {
		tmp.tv_sec  = end.tv_sec  + start.tv_sec;
		tmp.tv_nsec = end.tv_nsec + start.tv_nsec;
	}
	return tmp;
}


struct timespec timespec_sub(struct timespec start, struct timespec end) {
	struct timespec tmp;
	if ((end.tv_nsec-start.tv_nsec)<0) {
		tmp.tv_sec = end.tv_sec-start.tv_sec-1;
		tmp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
	} else {
		tmp.tv_sec = end.tv_sec-start.tv_sec;
		tmp.tv_nsec = end.tv_nsec-start.tv_nsec;
	}
	return tmp;
}

int main(int argc, char** argv) {
    if(argc != 2 && argc != 3) {
        printf("Usage: %s FILE [cartridge/folder]\n", argv[0]);
        return 0;
    }
    Arena arena = arena_create(MB(512));

    /* String cartrige_folder_path_name = (argc > 2) ? string_from_c_string(argv[2]) : L("SuperMarioWorld.sfc"); */
    /* Path cartrige_folder_path; */
    /* path_init(&cartrige_folder_path, cartrige_folder_path_name); */
    /* VFS cartrige_folder_vfs; */
    /* vfs_init(&cartrige_folder_vfs, 4); */
    /* vfs_add_dir(&cartrige_folder_vfs, &cartrige_folder_path, &arena); */

    /* Wdc65816Rom rom; */
    /* wdc65816_rom_init(&rom, &cartrige_folder_vfs, &arena); */

    nuts_global_init();

    Path working_dir;
    path_init_working_directory(&working_dir);
    FreeList sentinel;
    free_list_init(&sentinel);
    ErrorList error_list;
    error_list_init(&error_list, arena_subarena(&arena, MB(10)));


    StatementList stmt_list;
    String file_name = string_from_c_string(argv[1]); 
    struct timespec lex_start;
    struct timespec lex_end;
    struct timespec lex_time = { 0 };
    struct timespec parse_start;
    struct timespec parse_end;
    struct timespec parse_time = { 0 };
    struct timespec all_start;
    struct timespec all_end;
    struct timespec all_time;
    

    clock_gettime(CLOCK_REALTIME, &all_start);
    Parser parser;
    parser_init(&parser, &arena, &sentinel, &error_list, &stmt_list);
    while(1) {
        Path file;
        path_init(&file, file_name);
        Buffer file_buffer = path_read_file(&file, &arena);
        TokenList token_list;
        //printf("read %.*s\n", file_name.length, file_name.data);
        Text file_text = {
            .buffer = file_buffer,
            .name   = file_name
        };
        clock_gettime(CLOCK_REALTIME, &lex_start);
        lex(file_text, &token_list, &arena, &error_list);
        clock_gettime(CLOCK_REALTIME, &lex_end);
        lex_time = timespec_add(lex_time, timespec_sub(lex_start, lex_end));
        //token_list_print(token_list);
        clock_gettime(CLOCK_REALTIME, &parse_start);
        Result result = parse(&parser, token_list);
        clock_gettime(CLOCK_REALTIME, &parse_end);
        parse_time = timespec_add(parse_time, timespec_sub(parse_start, parse_end));

        if(result == RESULT_NEED_TOKEN_STREAM) {
            file_name = parser.needed_token_stream_file_name;
            continue;
        } else if(result == RESULT_OK) {
            break;
        }
    }

    for(int i = 0; i < error_list.length; i++) {
        describe_error(error_list.errors[i]);
    }
    
#if 0
    Assembler assembler;
    assembler_init(&assembler, &error_list, parser.statement_list, &arena);
    Result result = RESULT_ERROR;
    while(result != RESULT_OK) {
        result = assemble(&assembler);
        if(result == RESULT_NEED_FILE) {
            String file_name = assembler_get_file_name(&assembler);
            Path file;
            path_init(&file, file_name);
            Buffer file_buffer = path_read_file(&file, &arena);
            assembler_give_file(&assembler, file_buffer);
        }
    }
#endif
    parser_deinit(&parser);
    clock_gettime(CLOCK_REALTIME, &all_end);
    all_time = timespec_sub(all_start, all_end);
    printf("Lex:   %li.%06lims\n", lex_time.tv_sec * 1000 + lex_time.tv_nsec / 1000000, lex_time.tv_nsec % 1000000);
    printf("Parse: %li.%06lims\n", parse_time.tv_sec * 1000 + parse_time.tv_nsec / 1000000, parse_time.tv_nsec % 1000000);
    printf("All:   %li.%06lims\n", all_time.tv_sec * 1000 + all_time.tv_nsec / 1000000, all_time.tv_nsec % 1000000);

    return 0;
}
