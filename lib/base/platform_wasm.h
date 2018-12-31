#if defined(WASM)
#ifndef BASE_PLATFORM_WASM_H
#define BASE_PLATFORM_WASM_H

typedef struct {
    int fd;
} Path;

typedef struct {
    Path path;
    int index;
} DirIter;

#define NULL ((void*)0)

void* realloc(void* ptr, unsigned long size);


#endif
#endif //BASE_PLATFORM_WASM_H