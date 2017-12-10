#include "nuts/nuts.h"

int main(int argc, char** argv) {
    if(argc != 2 && argc != 3) {
        printf("Usage: nuts FILE [cartridge/folder]\n");
        return 0;
    }
    Arena arena = arena_create(MB(64));

    String path_name = (argc > 2) ? string_from_c_string(argv[2]) : L("SuperMarioWorld.sfc");
    Path path;
    path_init(&path, path_name);
    VFS vfs;
    vfs_init(&vfs, 4);
    vfs_add_dir(&vfs, &path, &arena);

    WDC65816Rom rom;
    wdc65816_rom_init(&rom, &vfs, &arena);

    parser_global_init();
    ParserState parser;
    Path working_dir;
    path_init_working_directory(&working_dir);
    Path asm_file;
    path_init(&asm_file, string_from_c_string(argv[1]));
    parser_init(&parser, &rom, &arena, &asm_file, &working_dir);
    parse_asm(&parser);
    parser_deinit(&parser);
    parser_global_deinit();
    return 0;
}
