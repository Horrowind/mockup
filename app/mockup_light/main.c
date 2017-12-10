#include "mockup/smw.h"

char* strncpy2(char *dest, const char *src, size_t n) {
    size_t i;
    for (i = 0; i < n && src[i] != '\0'; i++) dest[i] = src[i];
    for ( ; i <= n; i++)                      dest[i] = '\0';

    return dest;
}


int main(int argc, char** argv) {
    Arena arena = arena_create(MB(128));

    String path_name = (argc > 1) ? string_from_c_string(argv[1]) : L("SuperMarioWorld.sfc");
    Path path;
    path_init(&path, path_name);
    VFS vfs;
    vfs_init(&vfs, 4);
    vfs_add_dir(&vfs, &path, &arena);
    
    WDC65816Rom rom;
    wdc65816_rom_init(&rom, &vfs, &arena);

    SMW smw;
    smw_init_all(&smw, &rom, &arena);
    smw_deinit(&smw);
    wdc65816_rom_free(&rom);
    return 0;
}
