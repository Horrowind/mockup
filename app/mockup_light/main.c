#include "mockup/smw.h"

char* strncpy2(char *dest, const char *src, size_t n) {
    size_t i;
    for (i = 0; i < n && src[i] != '\0'; i++) dest[i] = src[i];
    for ( ; i <= n; i++)                      dest[i] = '\0';

    return dest;
}


int main(int argc, char** argv) {
    arena_t arena = arena_create(MB(128));

    string_t path_name = (argc > 1) ? string_from_c_string(argv[1]) : L("SuperMarioWorld.sfc");
    path_t path;
    path_init(&path, path_name);
    vfs_t vfs;
    vfs_init(&vfs, 4);
    vfs_add_dir(&vfs, &path, &arena);
    
    wdc65816_rom_t rom;
    wdc65816_rom_init(&rom, &vfs, &arena);

    smw_t smw;
    smw_init_all(&smw, &rom, &arena);
    smw_deinit(&smw);
    wdc65816_rom_free(&rom);
    return 0;
}
