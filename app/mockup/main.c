#include "mockup.h"
#define DEMO

extern char* sprite_names[256];
Mockup global_mockup = { };

#include "assets.c"
#include "viewers.c"
#if 0
#include "renderer.c"
#endif


void do_assemble(Mockup* mockup, Arena* temp_arena) {
    nuts_assembler_init(&mockup->assembler, &mockup->error_list, &mockup->ast, &mockup->rom, &mockup->free_list);
    
    NutsResult result = RESULT_ERROR;
    while(result != RESULT_OK) {
        result = nuts_assemble(&mockup->assembler);
        if(result == RESULT_NEED_FILE) {
            Buffer file_buffer;
            String file_name = nuts_assembler_get_file_name(&mockup->assembler);
            NamedBuffer dummy = { .name = file_name };
            NamedBuffer* found = named_buffer_map_find(&mockup->buffer_map, dummy);
            if(found) {
                file_buffer = found->buffer;
            } else {
                Path file;
                path_init(&file, file_name);
                file_buffer  = path_read_file(&file, temp_arena);
            }
            
            nuts_assembler_give_buffer(&mockup->assembler, file_buffer);
        } else if(result == RESULT_ERROR) {
            break;
        }
    }
}

 __attribute__((visibility("default")))
int update() {
    Mockup* mockup = &global_mockup;
    /* Input */
    GuiContext* context = &mockup->context;
    gui_begin_frame(context);

    //assets_clear(mockup);
    smw_clear(&mockup->smw);
    do_smw_viewer(context, &global_smw_viewer, mockup);

    /* GUI */
#if 0
    if(mu_begin(ctx, "Sprites", mu_rect(0, 0, 1200, 905),
                mu_WINDOW_MOVABLE|mu_WINDOW_SCALABLE|mu_WINDOW_BORDER|
                mu_WINDOW_MINIMIZABLE|mu_WINDOW_TITLE)) {
                
        mu_layout_row_begin(ctx, mu_STATIC, 10, 50);
        mu_layout_row_push(ctx, 20);
        mu_label(ctx, " ", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 200);
        mu_label(ctx, " ", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "00", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "00", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "00", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "00", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "00", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "00", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "14", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "14", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "14", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "14", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "14", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "15", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "15", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "15", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "15", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "15", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "15", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "15", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "15", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "15", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "15", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "15", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "15", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "15", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "15", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "15", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "15", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "15", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "15", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "15", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "15", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "15", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "16", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "16", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "16", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "16", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "16", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "16", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "16", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "16", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "16", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "16", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "16", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "16", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "18", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "18", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "19", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "1F", mu_TEXT_ALIGN_RIGHT);

        mu_layout_row_begin(ctx, mu_STATIC, 20, 50);
        mu_layout_row_push(ctx, 20);
        mu_label(ctx, " ", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 200);
        mu_label(ctx, " ", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "9E", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "AA", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "B6", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "C2", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "D8", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "E4", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "C8", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "D4", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "E0", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "EC", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "F8", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "04", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "10", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "1C", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "28", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "34", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "40", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "4C", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "58", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "64", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "70", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "7C", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "88", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "94", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "A0", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "AC", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "B8", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "C4", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "D0", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "DC", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "EA", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "F6", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "02", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "0E", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "1A", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "26", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "32", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "3E", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "4A", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "56", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "62", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "6E", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "7A", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "86", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "6C", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "7B", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "0F", mu_TEXT_ALIGN_RIGHT);
        mu_layout_row_push(ctx, 15); mu_label(ctx, "E2", mu_TEXT_ALIGN_RIGHT);
                
                
        for(int i = 0; i < level->sprites.length; i++) {
            SpriteTableEntries* entries = &level->sprites.data[i].table_entries;
            mu_layout_row_begin(ctx, mu_STATIC, 20, 50);
            mu_layout_row_push(ctx, 20);
            mu_labelf(ctx, mu_TEXT_ALIGN_RIGHT, "%i", i);
            char sprite_name[64];
            c_string_format_unsafe(sprite_name, "%s", sprite_names[level->sprites.data[i].number]);
            mu_layout_row_push(ctx, 200);
            mu_label(ctx, sprite_name, mu_TEXT_ALIGN_LEFT);
            mu_layout_row_push(ctx, 15); mu_labelf(ctx, mu_TEXT_ALIGN_RIGHT, "%02x", entries->entry_009E);
            mu_layout_row_push(ctx, 15); mu_labelf(ctx, mu_TEXT_ALIGN_RIGHT, "%02x", entries->entry_00AA);
            mu_layout_row_push(ctx, 15); mu_labelf(ctx, mu_TEXT_ALIGN_RIGHT, "%02x", entries->entry_00B6);
            mu_layout_row_push(ctx, 15); mu_labelf(ctx, mu_TEXT_ALIGN_RIGHT, "%02x", entries->entry_00C2);
            mu_layout_row_push(ctx, 15); mu_labelf(ctx, mu_TEXT_ALIGN_RIGHT, "%02x", entries->entry_00D8);
            mu_layout_row_push(ctx, 15); mu_labelf(ctx, mu_TEXT_ALIGN_RIGHT, "%02x", entries->entry_00E4);
            mu_layout_row_push(ctx, 15); mu_labelf(ctx, mu_TEXT_ALIGN_RIGHT, "%02x", entries->entry_14C8);
            mu_layout_row_push(ctx, 15); mu_labelf(ctx, mu_TEXT_ALIGN_RIGHT, "%02x", entries->entry_14D4);
            mu_layout_row_push(ctx, 15); mu_labelf(ctx, mu_TEXT_ALIGN_RIGHT, "%02x", entries->entry_14E0);
            mu_layout_row_push(ctx, 15); mu_labelf(ctx, mu_TEXT_ALIGN_RIGHT, "%02x", entries->entry_14EC);
            mu_layout_row_push(ctx, 15); mu_labelf(ctx, mu_TEXT_ALIGN_RIGHT, "%02x", entries->entry_14F8);
            mu_layout_row_push(ctx, 15); mu_labelf(ctx, mu_TEXT_ALIGN_RIGHT, "%02x", entries->entry_1504);
            mu_layout_row_push(ctx, 15); mu_labelf(ctx, mu_TEXT_ALIGN_RIGHT, "%02x", entries->entry_1510);
            mu_layout_row_push(ctx, 15); mu_labelf(ctx, mu_TEXT_ALIGN_RIGHT, "%02x", entries->entry_151C);
            mu_layout_row_push(ctx, 15); mu_labelf(ctx, mu_TEXT_ALIGN_RIGHT, "%02x", entries->entry_1528);
            mu_layout_row_push(ctx, 15); mu_labelf(ctx, mu_TEXT_ALIGN_RIGHT, "%02x", entries->entry_1534);
            mu_layout_row_push(ctx, 15); mu_labelf(ctx, mu_TEXT_ALIGN_RIGHT, "%02x", entries->entry_1540);
            mu_layout_row_push(ctx, 15); mu_labelf(ctx, mu_TEXT_ALIGN_RIGHT, "%02x", entries->entry_154C);
            mu_layout_row_push(ctx, 15); mu_labelf(ctx, mu_TEXT_ALIGN_RIGHT, "%02x", entries->entry_1558);
            mu_layout_row_push(ctx, 15); mu_labelf(ctx, mu_TEXT_ALIGN_RIGHT, "%02x", entries->entry_1564);
            mu_layout_row_push(ctx, 15); mu_labelf(ctx, mu_TEXT_ALIGN_RIGHT, "%02x", entries->entry_1570);
            mu_layout_row_push(ctx, 15); mu_labelf(ctx, mu_TEXT_ALIGN_RIGHT, "%02x", entries->entry_157C);
            mu_layout_row_push(ctx, 15); mu_labelf(ctx, mu_TEXT_ALIGN_RIGHT, "%02x", entries->entry_1588);
            mu_layout_row_push(ctx, 15); mu_labelf(ctx, mu_TEXT_ALIGN_RIGHT, "%02x", entries->entry_1594);
            mu_layout_row_push(ctx, 15); mu_labelf(ctx, mu_TEXT_ALIGN_RIGHT, "%02x", entries->entry_15A0);
            mu_layout_row_push(ctx, 15); mu_labelf(ctx, mu_TEXT_ALIGN_RIGHT, "%02x", entries->entry_15AC);
            mu_layout_row_push(ctx, 15); mu_labelf(ctx, mu_TEXT_ALIGN_RIGHT, "%02x", entries->entry_15B8);
            mu_layout_row_push(ctx, 15); mu_labelf(ctx, mu_TEXT_ALIGN_RIGHT, "%02x", entries->entry_15C4);
            mu_layout_row_push(ctx, 15); mu_labelf(ctx, mu_TEXT_ALIGN_RIGHT, "%02x", entries->entry_15D0);
            mu_layout_row_push(ctx, 15); mu_labelf(ctx, mu_TEXT_ALIGN_RIGHT, "%02x", entries->entry_15DC);
            mu_layout_row_push(ctx, 15); mu_labelf(ctx, mu_TEXT_ALIGN_RIGHT, "%02x", entries->entry_15EA);
            mu_layout_row_push(ctx, 15); mu_labelf(ctx, mu_TEXT_ALIGN_RIGHT, "%02x", entries->entry_15F6);
            mu_layout_row_push(ctx, 15); mu_labelf(ctx, mu_TEXT_ALIGN_RIGHT, "%02x", entries->entry_1602);
            mu_layout_row_push(ctx, 15); mu_labelf(ctx, mu_TEXT_ALIGN_RIGHT, "%02x", entries->entry_160E);
            mu_layout_row_push(ctx, 15); mu_labelf(ctx, mu_TEXT_ALIGN_RIGHT, "%02x", entries->entry_161A);
            mu_layout_row_push(ctx, 15); mu_labelf(ctx, mu_TEXT_ALIGN_RIGHT, "%02x", entries->entry_1626);
            mu_layout_row_push(ctx, 15); mu_labelf(ctx, mu_TEXT_ALIGN_RIGHT, "%02x", entries->entry_1632);
            mu_layout_row_push(ctx, 15); mu_labelf(ctx, mu_TEXT_ALIGN_RIGHT, "%02x", entries->entry_163E);
            mu_layout_row_push(ctx, 15); mu_labelf(ctx, mu_TEXT_ALIGN_RIGHT, "%02x", entries->entry_164A);
            mu_layout_row_push(ctx, 15); mu_labelf(ctx, mu_TEXT_ALIGN_RIGHT, "%02x", entries->entry_1656);
            mu_layout_row_push(ctx, 15); mu_labelf(ctx, mu_TEXT_ALIGN_RIGHT, "%02x", entries->entry_1662);
            mu_layout_row_push(ctx, 15); mu_labelf(ctx, mu_TEXT_ALIGN_RIGHT, "%02x", entries->entry_166E);
            mu_layout_row_push(ctx, 15); mu_labelf(ctx, mu_TEXT_ALIGN_RIGHT, "%02x", entries->entry_167A);
            mu_layout_row_push(ctx, 15); mu_labelf(ctx, mu_TEXT_ALIGN_RIGHT, "%02x", entries->entry_1686);
            mu_layout_row_push(ctx, 15); mu_labelf(ctx, mu_TEXT_ALIGN_RIGHT, "%02x", entries->entry_186C);
            mu_layout_row_push(ctx, 15); mu_labelf(ctx, mu_TEXT_ALIGN_RIGHT, "%02x", entries->entry_187B);
            mu_layout_row_push(ctx, 15); mu_labelf(ctx, mu_TEXT_ALIGN_RIGHT, "%02x", entries->entry_190F);
            mu_layout_row_push(ctx, 15); mu_labelf(ctx, mu_TEXT_ALIGN_RIGHT, "%02x", entries->entry_1FE2);

        }
    }                
            
    static int do_once = 1;
    if(do_once) {
        mu_window_collapse(ctx, "Sprites", mu_MINIMIZED);
        do_once = 0;
    }
#endif
    
    /* Draw */
    gui_end_frame(context);
    return gui_should_continue(context);
}

 __attribute__((visibility("default")))
int init(int argc, char** argv) {
         
#if !defined(WASM)
    if(argc != 2 && argc != 3) {
        c_print_format("Usage: mockup FILE [FOLDER]\n"
               "\n"
               "FILE contains a path to the main asm file of a diassembly\n"
               "[FOLDER] is an optional path to a Cartridge folder\n"
            );
        return -1;
    }
#endif
    Mockup* mockup = &global_mockup;

    // Memory
    Arena* arena = &mockup->arena;
    Arena* temp_arena = &mockup->temp_arena;
    FreeList* free_list = &mockup->free_list;
    *arena = arena_create(MB(2024));
    *temp_arena = arena_subarena(arena, MB(256));
    free_list_init(free_list, arena_subarena(arena, MB(256)), 4096);

    // GUI
    gui_init(&mockup->context, arena);    
    gui_set_window_name(&mockup->context, L("Mockup"));
    
    Arena persistent_arena = arena_subarena(arena, MB(4));
    Arena transient_arena = arena_subarena(temp_arena, MB(8));
    unused(persistent_arena);
    unused(transient_arena);

    String cartridge_folder_path_name = (argc > 2)
        ? string_from_c_string(argv[2])
        : L("SuperMarioWorld.sfc");
    Path cartridge_folder_path;
    path_init(&cartridge_folder_path, cartridge_folder_path_name);
    Path manifest_path;
    path_init_from_c(&manifest_path, &cartridge_folder_path, "manifest.bml");
    Buffer manifest_buffer = path_read_file(&manifest_path, arena);

    Wdc65816MapperBuilder rom_builder = { };
    
    char name[256];
    Buffer name_buffer = buffer(name, 256);
    for(Wdc65816RomLoader loader = wdc65816_rom_loader_begin(&rom_builder,
                                                             string_from_buffer(manifest_buffer));
        wdc65816_rom_loader_end(&loader);
        wdc65816_rom_loader_next(&loader)) {
        Wdc65816MemoryBufferRequest request = wdc65816_rom_loader_get_buffer_request(&loader,
                                                                                     name_buffer);
        Buffer file_buffer = buffer(arena_alloc_array(arena, request.size, u8), request.size);
        if(string_equal(request.name, L("program.rom")) &&
           request.type == WDC65816_MEMORY_ROM) {
               mockup->rom_buffer = file_buffer;
        }
        wdc65816_rom_loader_set_buffer(&loader, file_buffer);
    }

    uint mapper_buffer_size = wdc65816_mapper_get_buffer_size();
    u8* work_buffer = arena_alloc_array(arena, mapper_buffer_size, u8);
    wdc65816_mapper_init(&mockup->rom, &rom_builder, (u8**)work_buffer);

    Path working_dir;
    path_init_working_directory(&working_dir);
    
    nuts_global_init();
    nuts_ast_init(&mockup->ast, free_list);
    
    String file_name = string_from_c_string(argv[1]); 
    nuts_error_list_init(&mockup->error_list, arena_subarena(arena, MB(5)));

    NutsParser parser;
    nuts_parser_init(&parser, arena, free_list, &mockup->error_list, &mockup->ast);
    int error_num = 0;
    while(1) {
        Path file;
        path_init(&file, file_name);
        Buffer file_buffer = path_read_file(&file, arena);
        NutsTokenList token_list;
        //c_print_format("read %.*s\n", file_name.length, file_name.data);
        NutsText file_text = {
            .buffer = file_buffer,
            .name   = file_name
        };
        NutsResult result = nuts_lex(file_text, &token_list, arena, &mockup->error_list, &mockup->ast.identifier_map);
        if(result == RESULT_ERROR) {
            for(;error_num < mockup->error_list.length; error_num++) {
                nuts_describe_error(mockup->error_list.errors[error_num]);
            }
        }
        //token_list_print(token_list);
        result = nuts_parse(&parser, token_list);
        
        if(result == RESULT_NEED_TOKEN_STREAM) {
            file_name = parser.needed_token_stream_file_name;
            continue;
        } else if(result == RESULT_OK) {
            break;
        } else if(result == RESULT_ERROR) {
            for(;error_num < mockup->error_list.length; error_num++) {
                nuts_describe_error(mockup->error_list.errors[error_num]);
            }
            return 1;
        } else {
            invalid_code_path;
        }
    }

    named_buffer_map_init(&mockup->buffer_map, free_list, 128);
    do_assemble(mockup, arena);
    for(int i = 0; i < mockup->error_list.length; i++) {
        nuts_describe_error(mockup->error_list.errors[i]);
    }

    smw_init(&mockup->smw, &rom_builder, arena, temp_arena);
    for(int i = 0; i < 512; i++) {
        gui_image_data_init(&mockup->assets.palette[i]);
        gui_image_data_init(&mockup->assets.map8[i]);
        gui_image_data_init(&mockup->assets.sprite_map8[i]);
        gui_image_data_init(&mockup->assets.map16_fg[i]);
        gui_image_data_init(&mockup->assets.map16_bg[i]);
        gui_image_data_init(&mockup->assets.level[i]);
    }
    return 1;
}

int deinit() {
    Mockup* mockup = &global_mockup;

#if 0
    Path rom_path;
    path_create_from(&rom_path, &mockup->cartridge_folder_path, L("program.rom"));
    path_write_file(&rom_path, mockup->rom_buffer);
#endif
    gui_deinit(&mockup->context);
    return 1;
}