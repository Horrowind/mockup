#include "mockup/smw.h"

int main(int argc, char** argv) {
    Arena arena = arena_create(MB(1024));

    String path_name = (argc > 1) ? string_from_c_string(argv[1]) : L("SuperMarioWorld.sfc");
    Path path;
    path_init(&path, path_name);
    VFS vfs;
    vfs_init(&vfs, 4);
    vfs_add_dir(&vfs, &path, &arena);
    
    Wdc65816Rom rom;
    wdc65816_rom_init(&rom, &vfs, &arena);

    SMW smw;
    smw_init_all(&smw, &rom, &arena);
    smw_deinit(&smw);
    wdc65816_rom_deinit(&rom);
    return 0;
}
