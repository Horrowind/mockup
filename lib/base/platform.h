#ifndef BASE_PLATFORM_H
#define BASE_PLATFORM_H

#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <linux/limits.h>

#include "fixed_string.h"

// TODO: path should be required to end in \0 for ease of use with
// OS functions.
define_fixed_string(path, PATH_MAX);

u8* open_file_in_memory(path_t path);
string_t read_file_to_buffer(char* path);
int system2(char* cmd);
time_t get_file_time(path_t path);

#ifdef BASE_PLATFORM_IMPLEMENTATION

implement_fixed_string(path, PATH_MAX);

u8* open_file_in_memory(path_t path) {
    u8* result = NULL;
    int fd = open(path.data, O_RDONLY);
    if(fd != -1) {
        struct stat st_buf;
        if(fstat(fd, &st_buf) != -1) {
            int length = st_buf.st_size;
            result = mmap(NULL /*void *addr*/,
                          length /*size_t length*/,
                          PROT_READ | PROT_WRITE /*int prot*/,
                          0 /*int flags*/,
                          fd,
                          0 /* off_t offset*/);
            if(result == MAP_FAILED) result = NULL;
        }
    }
    close(fd);
    return result;
}

string_t read_file_to_buffer(char* path) {
    string_t buffer = { .data = 0, .length = 0 };
    /* char path[PATH_MAX] = { 0 }; */
    /* memcpy(path, string.data, string.length); */
    int fd = open(path, O_RDONLY);
    if(fd != -1) {
        struct stat sb;
        if (fstat(fd, &sb) != -1) {
            buffer.length = sb.st_size;
            buffer.data = mmap(0, sb.st_size + 2, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
        } else {
            printf("Error during fstat\n");
        }
    } else {
        int err = errno;
        printf("Could not open %s\n", path);
        switch(err) {
        case ENOENT:
            printf("No such file or directory");
            break;
        }
    }
    return buffer;
}


int system2(char* cmd) {
    int result = system(cmd);
    if (!WIFEXITED(result) && WIFSIGNALED(result)) exit(1);
    return result;
}

time_t get_file_time(path_t path) {
    struct stat st_buf;
    if(stat(path.data, &st_buf) != -1) {
        return st_buf.st_mtim.tv_sec;
    }
    return 0;
}

#endif //BASE_PLATFORM_IMPLEMENTATION
#endif //BASE_PLATFORM_H
