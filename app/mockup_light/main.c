#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <dirent.h>
#include <fcntl.h>
#include <linux/limits.h>
#include <libgen.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "mockup/smw.h"

char* strncpy2(char *dest, const char *src, size_t n) {
    size_t i;
    for (i = 0; i < n && src[i] != '\0'; i++) dest[i] = src[i];
    for ( ; i <= n; i++)                      dest[i] = '\0';

    return dest;
}


int main(int argc, char** argv) {
    char* path = (argc > 1) ? argv[1] : "smw.sfc";

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
    
    r65816_rom_t rom;
    r65816_rom_init(&rom, &vfs);

    smw_t smw;
    smw_init_all(&smw, &rom);
    smw_deinit_all(&smw);
    r65816_rom_free(&rom);
    return 0;
}
