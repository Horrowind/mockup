#ifndef BOGUS_H
#define BOGUS_H

#include "misc.h"
#include "platform.h"

typedef enum target_type {
    target_type_static_library,
    target_type_dynamic_library,
    target_type_executable
} target_type_t;

typedef enum {
    platform_linux,
    platform_windows,
    platform_web
} platform_t;

typedef struct {
    path_t file;
    path_t build_cmd;
    path_t check_cmd;
} object_t;

typedef struct {
    path_t name;
    path_t file;
    path_t output_dir;
    target_type_t type;
    platform_t platform;
    object_t* objects;
    int       objects_length;
    
    path_t* libraries;
    int       libraries_length;
    path_t* library_paths;
    int       library_paths_length;

    path_t flags;
} target_t;

struct {
    path_t gcc_path;
    path_t gpp_path;
    path_t clang_path;
    path_t clangpp_path;
    path_t emcc_path;
    path_t empp_path;
    path_t mxe_gcc_path;
    path_t mxe_gpp_path;
    path_t ar_path;
    path_t mxe_ar_path;
} tool_paths;

typedef enum {
        compiler_gcc,
        compiler_clang,
        compiler_mxe_gcc,
        compiler_emcc,
        compiler_gpp,
        compiler_clangpp,
        compiler_mxe_gpp,
        compiler_empp
} compiler_t;

typedef struct {
    compiler_t compiler;

    int optimization_level;
    path_t* includes;
    int       includes_length;
    path_t flags;
} compiler_options_t;





#endif //BOGUS_H
