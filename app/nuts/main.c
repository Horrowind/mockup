#include <errno.h>

#include <dirent.h>
#include <fcntl.h>
#include <libgen.h>

#include "nuts/nuts.h"

int main(int argc, char** argv) {
    if(argc != 2 || argc != 3) {
        printf("Usage: nuts FILE [cartridge/folder]\n");
        return 0;
    }
    arena_t arena = arena_create(MB(16));

    char* path = (argc > 2) ? argv[2] : "smw.sfc";

    int  dd = open(path, O_RDONLY | O_DIRECTORY | __O_PATH);
    DIR* dp = opendir(path);
    if(!dp) {
        fprintf(stderr, "Error: Failed to open input directory \"%s\"\n", strerror(errno));
        exit(1);
    }

    vfs_t vfs;
    vfs_init(&vfs, 4);
    
    struct dirent* de;
    while((de = readdir(dp))) {
        printf("File: %s\n", de->d_name);
        if (!strcmp (de->d_name, "."))
            continue;
        if (!strcmp (de->d_name, ".."))
            continue;
        vfs_entry_t entry = { 0 };
        int fd = openat(dd, de->d_name, O_RDONLY);
        FILE* fp = fdopen(fd, "r");
        if(!fp) {
            fprintf(stderr, "Error: could not open file \"%s\"\n", de->d_name);
            exit(1);
        }
        entry.name = string_from_c_string(basename(de->d_name));
        fseek(fp, 0, SEEK_END);
        long int file_size = ftell(fp);
        u8* data = malloc(file_size + 1);
        rewind(fp);
        fread(data, file_size, 1, fp);
        fclose(fp);
        data[file_size] = 0;
        entry.buffer = (buffer_t){ .begin = data, .end = data + file_size};
        printf("%016lx %016lx\n", (u64)entry.buffer.begin, (u64)entry.buffer.end);
        vfs_insert(&vfs, entry);
    }

    wdc65816_rom_t rom;
    wdc65816_rom_init(&rom, &vfs, &arena);

    parser_global_init();
    parser_state_t parser;
    parser_init(&parser, string_from_c_string(argv[1]), &rom, &arena);
    parse_asm(&parser);
    parser_deinit(&parser);
    parser_global_deinit();
    return 0;
}
