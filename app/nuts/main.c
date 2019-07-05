#include "nuts/nuts.h"

/* struct timespec timespec_add(struct timespec start, struct timespec end) { */
/* 	struct timespec tmp; */
/* 	if ((end.tv_nsec+start.tv_nsec) >= 1000000000) { */
/* 		tmp.tv_sec  = end.tv_sec  + start.tv_sec  + 1; */
/* 		tmp.tv_nsec = end.tv_nsec + start.tv_nsec - 1000000000; */
/* 	} else { */
/* 		tmp.tv_sec  = end.tv_sec  + start.tv_sec; */
/* 		tmp.tv_nsec = end.tv_nsec + start.tv_nsec; */
/* 	} */
/* 	return tmp; */
/* } */


/* struct timespec timespec_sub(struct timespec start, struct timespec end) { */
/* 	struct timespec tmp; */
/* 	if ((end.tv_nsec-start.tv_nsec)<0) { */
/* 		tmp.tv_sec = end.tv_sec-start.tv_sec-1; */
/* 		tmp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec; */
/* 	} else { */
/* 		tmp.tv_sec = end.tv_sec-start.tv_sec; */
/* 		tmp.tv_nsec = end.tv_nsec-start.tv_nsec; */
/* 	} */
/* 	return tmp; */
/* } */

int main(int argc, char** argv) {
    if(argc != 2 && argc != 3) {
        c_print_format("Usage: %s FILE [cartridge/folder]\n", argv[0]);
        return 0;
    }
    Arena arena = arena_create(MB(1024));
    String cartridge_folder_path_name = (argc > 2)
        ? string_from_c_string(argv[2])
        : L("SuperMarioWorld.sfc");
    Path cartridge_folder_path;
    path_init(&cartridge_folder_path, cartridge_folder_path_name);
    Path manifest_path;
    path_init_from_c(&manifest_path, &cartridge_folder_path, "manifest.bml");
    Buffer manifest_buffer = path_read_file(&manifest_path, &arena);

    Wdc65816MapperBuilder rom_builder = { 0 };
    
    char name[256];
    Buffer name_buffer = buffer(name, 256);
    Buffer rom_buffer;
    for(Wdc65816RomLoader loader = wdc65816_rom_loader_begin(&rom_builder,
                                                             string_from_buffer(manifest_buffer));
        wdc65816_rom_loader_end(&loader);
        wdc65816_rom_loader_next(&loader)) {
        Wdc65816MemoryBufferRequest request = wdc65816_rom_loader_get_buffer_request(&loader,
                                                                                     name_buffer);
        Buffer file_buffer = buffer(arena_alloc_array(&arena, request.size, u8), request.size);
        if(string_equal(request.name, L("program.rom")) &&
           request.type == WDC65816_MEMORY_ROM) {
            rom_buffer = file_buffer;
        }
        wdc65816_rom_loader_set_buffer(&loader, file_buffer);
    }

    uint mapper_buffer_size = wdc65816_mapper_get_buffer_size();
    u8* work_buffer = arena_alloc_array(&arena, mapper_buffer_size, u8);
    Wdc65816Mapper rom;
    wdc65816_mapper_init(&rom, &rom_builder, (u8**)work_buffer);

    nuts_global_init();

    Path working_dir;
    path_init_working_directory(&working_dir);
    FreeList free_list;
    Arena free_list_arena = arena_subarena(&arena, MB(512));
    free_list_init(&free_list, free_list_arena, 4096);
    Arena error_list_arena = arena_subarena(&arena, MB(10));
    NutsErrorList error_list;
    
    nuts_error_list_init(&error_list, error_list_arena);


    NutsAST ast;
    nuts_ast_init(&ast, &free_list);
    String file_name = string_from_c_string(argv[1]); 
    /* struct timespec lex_start,      lex_end,      lex_time = { 0 }; */
    /* struct timespec parse_start,    parse_end,    parse_time = { 0 }; */
    /* struct timespec assemble_start, assemble_end, assemble_time = { 0 }; */
    /* struct timespec all_start,      all_end,      all_time; */
    

    /* clock_gettime(CLOCK_REALTIME, &all_start); */
    NutsParser parser;
    nuts_parser_init(&parser, &arena, &free_list, &error_list, &ast);
    int error_num = 0;
    while(1) {
        Path file;
        path_init(&file, file_name);
        Buffer file_buffer = path_read_file(&file, &arena);
        NutsTokenList token_list;
        //c_print_format("read %.*s\n", file_name.length, file_name.data);
        NutsText file_text = {
            .buffer = file_buffer,
            .name   = file_name
        };
        /* clock_gettime(CLOCK_REALTIME, &lex_start); */
        NutsResult result = nuts_lex(file_text, &token_list, &arena, &error_list, &ast.identifier_map);
        if(result == RESULT_ERROR) {
            for(;error_num < error_list.length; error_num++) {
                nuts_describe_error(error_list.errors[error_num]);
            }
        }
        /* clock_gettime(CLOCK_REALTIME, &lex_end); */
        /* lex_time = timespec_add(lex_time, timespec_sub(lex_start, lex_end)); */
        /* clock_gettime(CLOCK_REALTIME, &parse_start); */
        result = nuts_parse(&parser, token_list);
        /* clock_gettime(CLOCK_REALTIME, &parse_end); */
        /* parse_time = timespec_add(parse_time, timespec_sub(parse_start, parse_end)); */

        if(result == RESULT_NEED_TOKEN_STREAM) {
            file_name = parser.needed_token_stream_file_name;
            continue;
        } else if(result == RESULT_OK) {
            break;
        } else if(result == RESULT_ERROR) {
            for(;error_num < error_list.length; error_num++) {
                nuts_describe_error(error_list.errors[error_num]);
            }
            return 1;
        } else {
            invalid_code_path;
        }
    }

#if 0
    for(int i = 0; i < ast.statement_list.length; i++) {
        Statement* stmt = ast.statement_list.data + i;
        c_print_format("%6i: ", i);
        for(int j = 0; j < stmt->tokens.length; j++) {
            Token token = stmt->tokens.data[j];
            c_print_format("%.*s ", token.string.length, token.string.data);
        }
        c_print_format("\n");
    }
    c_print_format("Finished.\n");
#endif    

    /* clock_gettime(CLOCK_REALTIME, &assemble_start); */
    NutsAssembler assembler;
    nuts_assembler_init(&assembler, &error_list, &ast, &rom, &free_list);
    NutsResult result = RESULT_ERROR;
    while(result != RESULT_OK) {
        result = nuts_assemble(&assembler);
        if(result == RESULT_NEED_FILE) {
            String file_name = nuts_assembler_get_file_name(&assembler);
            Path file;
            path_init(&file, file_name);
            Buffer file_buffer = path_read_file(&file, &arena);
            nuts_assembler_give_buffer(&assembler, file_buffer);
        } else if(result == RESULT_ERROR) {
            break;
        }
    }
    /* clock_gettime(CLOCK_REALTIME, &assemble_end); */
    /* assemble_time = timespec_add(assemble_time, timespec_sub(assemble_start, assemble_end)); */

    for(int i = 0; i < error_list.length; i++) {
        nuts_describe_error(error_list.errors[i]);
    }

    /* parser_deinit(&parser); */
    /* clock_gettime(CLOCK_REALTIME, &all_end); */
    /* all_time = timespec_sub(all_start, all_end); */
    /* c_print_format("Lex:      %li.%06lims\n",      lex_time.tv_sec * 1000 +      lex_time.tv_nsec / 1000000,      lex_time.tv_nsec % 1000000); */
    /* c_print_format("Parse:    %li.%06lims\n",    parse_time.tv_sec * 1000 +    parse_time.tv_nsec / 1000000,    parse_time.tv_nsec % 1000000); */
    /* c_print_format("Assemble: %li.%06lims\n", assemble_time.tv_sec * 1000 + assemble_time.tv_nsec / 1000000, assemble_time.tv_nsec % 1000000); */
    /* c_print_format("All:      %li.%06lims\n",      all_time.tv_sec * 1000 +      all_time.tv_nsec / 1000000,      all_time.tv_nsec % 1000000); */

    Path rom_path;
    path_create_from(&rom_path, &cartridge_folder_path, L("program.rom"));
    path_write_file(&rom_path, rom_buffer);
    
    return 0;
}
