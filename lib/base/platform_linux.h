#if defined(LINUX)
#ifndef BASE_LINUX_H
#define BASE_LINUX_H

#define _GNU_SOURCE
#include <dirent.h>    // readdir, fdopendir, closedir
#include <stdlib.h>    // realloc
#include <math.h>

#define debug_break raise_signal(2 /*SIGINT*/)

typedef struct {
    long seconds;
    long nano_seconds;
} TimeSpec_;

typedef struct {
    ulong st_dev;
    u64   st_ino;
    long  st_nlink;

    uint  st_mode;
    uint  st_uid;
    uint  st_gid;
    uint  __pad0;
    u64   st_rdev;
    long  st_size;
    long  st_blksize;
    u64   st_blocks;

    TimeSpec_ st_atim;
    TimeSpec_ st_mtim;
    TimeSpec_ st_ctim;
    long __unused[3];
} Stat_;

typedef struct {
    int fd_;
    int fd_read_;
    int fd_write_;
    Stat_ stat_;
} Path;

typedef struct {
    Path path;
    Path* dir_path;
    DIR* dp_;
} DirIter;

#endif //BASE_LINUX
#endif //defined(LINUX)
