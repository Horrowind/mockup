#include <stdint.h>
#include <linux/limits.h>

typedef char string_t[PATH_MAX];

typedef enum target_type {
    target_type_static_library,
    target_type_dynamic_library,
    target_type_executable
} target_type_t;

typedef struct {
    string_t file;
} object_t;

typedef struct {
    string_t name;
    target_type_t type;

    object_t* objects;
    int       objects_length;
    
    string_t* libraries;
    int       libraries_length;
    string_t* paths;
    int       paths_length;

    string_t  flags;
} target_t;

struct {
    string_t gcc_path;
    string_t clang_path;
    string_t emcc_path;
    string_t ar_path;
} tool_paths;

typedef struct {
    enum {
        compiler_gcc,
        compiler_clang,
        compiler_emcc
    } compiler;

    int optimization_level;

    string_t* includes;
    int       includes_length;
    string_t  flags;
} compiler_options_t;


void init();
target_t target_init(string_t name, target_type_t type);
void target_add_object(target_t* target, object_t object);
void target_set_flags(target_t* target, string_t flags);
void compiler_options_add_include(compiler_options_t* options, string_t path);
void compiler_options_set_flags(compiler_options_t* options, string_t flags);
uint32_t needs_rebuild(string_t file, compiler_options_t options, string_t build_dir);
object_t compile(string_t file, compiler_options_t options, string_t build_dir);
void build(target_t target, string_t build_dir);


#ifdef BOGUS_IMPLEMENTATION
#ifndef BOGUS_FILE_NAME
#define BOGUS_FILE_NAME "bogus.c"
#endif
#define _GNU_SOURCE
#include "assert.h"
#include "errno.h"
#include "limits.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "sys/stat.h"
#include "time.h"
#include <unistd.h>

void init() {
    strncpy(tool_paths.gcc_path, "/usr/bin/gcc", PATH_MAX);
    strncpy(tool_paths.clang_path, "/usr/bin/clang", PATH_MAX);
    strncpy(tool_paths.emcc_path, "/home/horrowind/Projects/emscripten/emscripten/tag-1.34.9/emcc", PATH_MAX);
    strncpy(tool_paths.ar_path, "/usr/bin/ar", PATH_MAX);

    string_t source_file = "bogus.c";
    string_t exec_name = BOGUS_FILE_NAME;
    string_t exec_dir;
    readlink("/proc/self/exe", exec_dir, sizeof(string_t));
    char* last_occurence_of_slash = strrchr(exec_dir, '/');
    if(last_occurence_of_slash != NULL) {
        strcpy(exec_name, last_occurence_of_slash + 1);
        *last_occurence_of_slash = '\0';
        chdir(exec_dir);
    }
    
    struct stat st_buf;
    time_t time1 = 0;
    time_t time2 = INT_MAX;
    if(stat(source_file, &st_buf) != -1) {
        time1 = st_buf.st_mtim.tv_sec;
        if(stat(exec_name, &st_buf) != -1) {
            time2 = st_buf.st_mtim.tv_sec;
        }
    }
    
    if(time1 > time2) {
        compiler_options_t co;
        co.compiler = compiler_gcc;
        co.optimization_level = 2;
        compiler_options_set_flags(&co, "-g");

        object_t main_o = compile(source_file, co, ".");
        target_t bogus = target_init(exec_name, target_type_executable);
        target_add_object(&bogus, main_o);
        build(bogus, ".");
        char* argv[] = {"bogus", 0};
        execv("bogus", argv);
    }

}
target_t target_init(string_t name, target_type_t type) {
    target_t target;
    strncpy(target.name, name, sizeof(string_t));
    target.type = type;
    target.objects = NULL;
    target.objects_length = 0;
    target.libraries = NULL;
    target.libraries_length = 0;
    return target;
}
void target_add_object(target_t* target, object_t object) {
    target->objects = realloc(target->objects, (target->objects_length + 1) * sizeof(object_t));
    strncpy(target->objects[target->objects_length].file, object.file, sizeof(object_t));
    target->objects_length++;
}
void target_set_flags(target_t* target, string_t flags) {
    strcpy(target->flags, flags);
}
void compiler_options_add_include(compiler_options_t* options, string_t path) {
    options->includes = realloc(options->includes, (options->includes_length + 1) * sizeof(string_t));
    realpath(path, options->includes[options->includes_length]);
    strcat(options->includes[options->includes_length], "/");
    options->includes_length++;
}
void compiler_options_set_flags(compiler_options_t* options, string_t flags) {
    strcpy(options->flags, flags);
}
object_t compile(string_t path, compiler_options_t options, string_t build_dir) {
    object_t object;
    string_t build_cmd = "";
    string_t filename = "";

    memset(filename, 0, sizeof(string_t));
    
    object.file[0] = '\0';
    
    char* last_occurence_of_slash = strrchr(path, '/');
    if(last_occurence_of_slash != NULL) {
        strcpy(filename, last_occurence_of_slash + 1);
    } else {
        strcpy(filename, path);
    }

    
    struct stat st_buf;
    if(stat(build_dir, &st_buf) != -1 && S_ISDIR(st_buf.st_mode)) {
        realpath(build_dir, object.file);
        strcat(object.file, "/");
        strcat(object.file, filename);
        strcat(object.file, ".o");


        if(stat(object.file, &st_buf)) {
            time_t time1 = st_buf.st_mtim.tv_sec;
            if(stat(path, &st_buf)) {
                time_t time2 = st_buf.st_mtim.tv_sec;
            }
        }

        
        if(options.compiler == compiler_gcc) {
            strcat(build_cmd, tool_paths.gcc_path);
        } else if(options.compiler == compiler_clang) {
            strcat(build_cmd, tool_paths.clang_path);
        } else {
            assert(options.compiler == compiler_emcc);
            strcat(build_cmd, tool_paths.emcc_path);
        }

        assert(options.optimization_level >= 0 && options.optimization_level <= 3);
        strcat(build_cmd, " -O0");
        build_cmd[strlen(build_cmd) - 1] += options.optimization_level;
        
        strcat(build_cmd, " ");
        strcat(build_cmd, options.flags);
        for(int i = 0; i < options.includes_length; i++) {
            strcat(build_cmd, " -I");
            strcat(build_cmd, options.includes[i]);
        }

        strcat(build_cmd, " -o ");
        strcat(build_cmd, object.file);

        strcat(build_cmd, " -c ");
        strcat(build_cmd, path);


        printf("%s\n", build_cmd);
        system(build_cmd);

    }
    return object;
}
void build(target_t target, string_t build_dir) {
    string_t build_cmd = "";
    string_t output_file;
    
    struct stat st_buf;
    if(stat(build_dir, &st_buf) != -1 && S_ISDIR(st_buf.st_mode)) {
        realpath(build_dir, output_file);
        strcat(output_file, "/");
        strcat(output_file, target.name);
    
        if(target.type == target_type_static_library) {
            strncat(build_cmd, tool_paths.ar_path, PATH_MAX);
            strncat(build_cmd, " cq ", PATH_MAX);
            strncat(build_cmd, output_file, PATH_MAX);
            strncat(build_cmd, ".a ", PATH_MAX);
            
        } else if(target.type == target_type_dynamic_library) {
            strncat(build_cmd, tool_paths.ar_path, PATH_MAX);
        } else {
            assert(target.type == target_type_executable);
            strncat(build_cmd, tool_paths.gcc_path, PATH_MAX);
            strcat(build_cmd, " -o ");
            strcat(build_cmd, output_file);
        }

        for(int i = 0; i < target.objects_length; i++) {
            strcat(build_cmd, " ");
            strcat(build_cmd, target.objects[i].file);
        }



        printf("%s\n", build_cmd);
        system(build_cmd);
        if(target.type == target_type_static_library) {
            strcpy(build_cmd, tool_paths.ar_path);
            strcat(build_cmd, " -s ");
            strcat(build_cmd, output_file);
            strcat(build_cmd, ".a ");
            printf("%s\n", build_cmd);
            system(build_cmd);

        }
    }
}

#endif
