#include <stdint.h>
#include <linux/limits.h>

#define array_length(array) (sizeof(array)/sizeof(array[0]))
#define max(a, b) (a > b ? a : b)
#define min(a, b) (a > b ? a : b)
typedef char string_t[PATH_MAX];

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
    string_t file;
    string_t build_cmd;
    string_t check_cmd;
} object_t;

typedef struct {
    string_t name;
    string_t file;
    string_t output_dir;
    target_type_t type;
    platform_t platform;
    object_t* objects;
    int       objects_length;
    
    string_t* libraries;
    int       libraries_length;
    string_t* library_paths;
    int       library_paths_length;

    string_t  flags;
} target_t;

struct {
    string_t gcc_path;
//    string_t gpp_path;
    string_t clang_path;
//    string_t clangpp_path;
    string_t emcc_path;
    /* string_t empp_path; */
    string_t mxe_gcc_path;
//    string_t mxe_gpp_path;
    string_t ar_path;
    string_t mxe_ar_path;
} tool_paths;

typedef enum {
        compiler_gcc,
        compiler_clang,
        compiler_mxe_gcc,
        compiler_emcc,
//        compiler_gpp,
//        compiler_clangpp,
//        compiler_mxe_gpp,
        /* compiler_empp */
} compiler_t;

typedef struct {
    compiler_t compiler;

    int optimization_level;
    string_t* includes;
    int       includes_length;
    string_t  flags;
} compiler_options_t;


void init();
target_t target_init(string_t name, target_type_t type);
void target_add_library(target_t* target, string_t library);
void target_add_libraries(target_t* target, string_t* libraries, int length);
void target_add_library_path(target_t* target, string_t library_path);
void target_add_library_paths(target_t* target, string_t* library_paths, int length);
void target_add_object(target_t* target, object_t object);
void target_add_objects(target_t* target, object_t* objects, int length);
void target_set_flags(target_t* target, string_t flags);
compiler_options_t compiler_options_init(compiler_t compiler, int level);
void compiler_options_add_include(compiler_options_t* options, string_t path);
void compiler_options_add_includes(compiler_options_t* options, string_t* path, int length);
void compiler_options_set_flags(compiler_options_t* options, string_t flags);
uint32_t needs_rebuild(string_t file, compiler_options_t options, string_t build_dir);
object_t compile(string_t file, compiler_options_t options, string_t build_dir);
int compile_many(object_t* objects, string_t* files, int length, compiler_options_t options, string_t build_dir);
int build(target_t* target, string_t build_dir);

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

typedef struct {
    string_t build_cmd;
    
} queue_item_t;

typedef struct {
    queue_item_t* data;
    int head;
    int length;
} queue_t;



int system2(char* cmd) {
    int result = system(cmd);
    if (!WIFEXITED(result) && WIFSIGNALED(result)) exit(1);
    return result;
}



time_t get_file_time(string_t path) {
    struct stat st_buf;
    if(stat(path, &st_buf) != -1) {
        return st_buf.st_mtim.tv_sec;
    }
    return 0;
}


void init() {
    strncpy(tool_paths.gcc_path, "/usr/bin/gcc -Wl,-fuse-ld=gold", PATH_MAX);
//    strncpy(tool_paths.gpp_path, "/usr/bin/g++", PATH_MAX);
    strncpy(tool_paths.clang_path, "/usr/bin/clang", PATH_MAX);
//    strncpy(tool_paths.clangpp_path, "/usr/bin/clang++", PATH_MAX);
    strncpy(tool_paths.emcc_path, "/home/horrowind/Projects/emscripten/emscripten/master/emcc", PATH_MAX);
    /* strncpy(tool_paths.empp_path, "/home/horrowind/Projects/emscripten/emscripten/master/em++", PATH_MAX); */
    strncpy(tool_paths.mxe_gcc_path, "/home/horrowind/Projects/mxe/usr/bin/i686-w64-mingw32.static-gcc", PATH_MAX);
//    strncpy(tool_paths.mxe_gpp_path, "/home/horrowind/Projects/mxe/usr/bin/i686-w64-mingw32.static-g++", PATH_MAX);
    strncpy(tool_paths.ar_path, "/usr/bin/ar", PATH_MAX);
    strncpy(tool_paths.mxe_ar_path, "/home/horrowind/Projects/mxe/usr/bin/i686-w64-mingw32.static-ar", PATH_MAX);

    string_t source_file = "bogus.c";
    string_t exec_name = "bogus";
    string_t exec_temp_name = "bogus_tmp";
    string_t exec_path, exec_dir;
    readlink("/proc/self/exe", exec_path, sizeof(string_t));
    memcpy(exec_dir, exec_path, sizeof(string_t));
    time_t object_time = get_file_time(exec_dir);
    char* last_occurence_of_slash = strrchr(exec_dir, '/');
    if(last_occurence_of_slash != NULL) {
        strcpy(exec_name, last_occurence_of_slash + 1);
        *last_occurence_of_slash = '\0';
        chdir(exec_dir);
    }
    
#ifndef BUILD_CMD
    compiler_options_t co;
    co.compiler = compiler_gcc;
    co.optimization_level = 2;
    //compiler_options_set_flags(&co, "-g");

    object_t main_o = compile(source_file, co, ".");
    target_t bogus = target_init(exec_temp_name, target_type_executable);
    target_add_object(&bogus, main_o);
    unlink(exec_temp_name);
    if(build(&bogus, ".")) {
        if(access(exec_temp_name, F_OK) != -1) {
            unlink(exec_name);
            rename(exec_temp_name, exec_name);
            char* argv[] = {"bogus", 0};
            execvp("bogus", argv);
            printf("Error: %s\n", strerror(errno));
        }
        exit(0);
    }
#else
    string_t filename;
    FILE* fp = popen(BUILD_CMD " -MM", "r");
    int i = 0;
    for(; (filename[i] = fgetc(fp)) != ':' && filename[i] != EOF; i++) { }
    filename[i] = '\0';
    time_t newest_source_time = get_file_time(filename);

    while(!feof(fp)) {
        string_t tmp;
        fscanf(fp, "%s", tmp);
        if(strcmp(tmp, "\\")) {
            newest_source_time = max(get_file_time(tmp), newest_source_time);
        }
    }
    
    if(newest_source_time > object_time) {
        unlink("bogus_tmp");
        printf(BUILD_CMD " -o bogus_tmp\n");
        system2(BUILD_CMD " -o bogus_tmp");
        if(access("bogus_tmp", F_OK) != -1) {
            unlink("bogus");
            rename("bogus_tmp", "bogus");
            char* argv[] = {"bogus", 0};
            execvp(exec_path, argv);
            printf("Error: %s\n", strerror(errno));
        }
        exit(0);
    }
#endif
}
target_t target_init(string_t name, target_type_t type) {
    target_t target;
    strncpy(target.name, name, sizeof(string_t));
    strncpy(target.file, "", sizeof(string_t));
    strncpy(target.output_dir, "", sizeof(string_t));
    target.type = type;
    target.objects = NULL;
    target.objects_length = 0;
    target.libraries = NULL;
    target.libraries_length = 0;
    target.library_paths = NULL;
    target.library_paths_length = 0;
    target.flags[0] = '\0';
    return target;
}

void target_add_library(target_t* target, string_t library) {
    target->libraries = realloc(target->libraries, (target->libraries_length + 1) * sizeof(string_t));
    strncpy(target->libraries[target->libraries_length], library, sizeof(string_t));
    target->libraries_length++;
}

void target_add_libraries(target_t* target, string_t* libraries, int length) {
    target->libraries = realloc(target->libraries, (target->libraries_length + length) * sizeof(string_t));
    for(int i = 0; i < length; i++) {
        strncpy(target->libraries[target->libraries_length + i], libraries[i], sizeof(string_t));
    }
    target->libraries_length += length;
}

void target_add_library_path(target_t* target, string_t paths) {
    target->library_paths = realloc(target->library_paths, (target->library_paths_length + 1) * sizeof(string_t));
    strncpy(target->library_paths[target->library_paths_length], paths, sizeof(string_t));
    target->library_paths_length++;
}

void target_add_library_paths(target_t* target, string_t* paths, int length) {
    target->library_paths = realloc(target->library_paths, (target->library_paths_length + length) * sizeof(string_t));
    for(int i = 0; i < length; i++) {
        strncpy(target->library_paths[target->library_paths_length + i], paths[i], sizeof(string_t));
    }
    target->library_paths_length += length;
}


void target_add_object(target_t* target, object_t object) {
    target->objects = realloc(target->objects, (target->objects_length + 1) * sizeof(object_t));
    strncpy(target->objects[target->objects_length].file, object.file, sizeof(string_t));
    target->objects_length++;
}

void target_add_objects(target_t* target, object_t* objects, int length) {
    target->objects = realloc(target->objects, (target->objects_length + length) * sizeof(object_t));
    for(int i = 0; i < length; i++) {
        strncpy(target->objects[target->objects_length + i].file, objects[i].file, sizeof(string_t));
    }
    target->objects_length += length;
}

void target_set_flags(target_t* target, string_t flags) {
    strcpy(target->flags, flags);
}

compiler_options_t compiler_options_init(compiler_t compiler, int level) {
    compiler_options_t options;
    options.compiler = compiler;
    options.flags[0] = '\0';       
    options.optimization_level = level;
    options.includes = NULL;
    options.includes_length = 0;
    return options;
}

void compiler_options_add_include(compiler_options_t* options, char* path) {
    options->includes = realloc(options->includes, (options->includes_length + 1) * sizeof(string_t));
    realpath(path, options->includes[options->includes_length]);
    strcat(options->includes[options->includes_length], "/");
    options->includes_length++;
}

void compiler_options_add_includes(compiler_options_t* options, string_t* paths, int length) {
    options->includes = realloc(options->includes, (options->includes_length + length) * sizeof(string_t));
    for(int i = 0; i < length; i++) {
        realpath(paths[i], options->includes[options->includes_length + i]);
        strcat(options->includes[options->includes_length + i], "/");
    }
    options->includes_length += length;
}

void compiler_options_set_flags(compiler_options_t* options, string_t flags) {
    strcpy(options->flags, flags);
}


object_t compile(string_t source, compiler_options_t options, string_t build_dir) {
    object_t object;
    object.build_cmd[0] = '\0';
    object.check_cmd[0] = '\0';
    string_t filename = "";

    memset(filename, 0, sizeof(string_t));
    
    object.file[0] = '\0';
    
    char* last_occurence_of_slash = strrchr(source, '/');
    if(last_occurence_of_slash != NULL) {
        strcpy(filename, last_occurence_of_slash + 1);
    } else {
        strcpy(filename, source);
    }

    
    struct stat st_buf;
    if(stat(build_dir, &st_buf) != -1 && S_ISDIR(st_buf.st_mode)) {
        realpath(build_dir, object.file);
        strcat(object.file, "/");
        strcat(object.file, filename);
        strcat(object.file, ".o");

        if(options.compiler == compiler_gcc) {
            strcat(object.build_cmd, tool_paths.gcc_path);
        } else if(options.compiler == compiler_clang) {
            strcat(object.build_cmd, tool_paths.clang_path);
        } else if(options.compiler == compiler_mxe_gcc) {
            strcat(object.build_cmd, tool_paths.mxe_gcc_path);
        } else if (options.compiler == compiler_emcc){
            strcat(object.build_cmd, tool_paths.emcc_path);
        /* } else if(options.compiler == compiler_gpp) { */
        /*     strcat(object.build_cmd, tool_paths.gpp_path); */
        /* } else if(options.compiler == compiler_clangpp) { */
        /*     strcat(object.build_cmd, tool_paths.clangpp_path); */
        /* } else if(options.compiler == compiler_mxe_gpp) { */
        /*     strcat(object.build_cmd, tool_paths.mxe_gpp_path); */
        /* } else { */
        /*     assert(options.compiler == compiler_empp); */
        /*     strcat(object.build_cmd, tool_paths.empp_path); */
        } else {
            assert(0 && "Unspecified compiler");
        }

        assert(options.optimization_level >= 0 && options.optimization_level <= 3);
        strcat(object.build_cmd, " -O0 ");
        object.build_cmd[strlen(object.build_cmd) - 2] += options.optimization_level;
        
        strcat(object.build_cmd, options.flags);
        for(int i = 0; i < options.includes_length; i++) {
            strcat(object.build_cmd, " -I");
            strcat(object.build_cmd, options.includes[i]);
        }

        strcat(object.build_cmd, " -c ");
        strcat(object.build_cmd, source);

        strcpy(object.check_cmd, object.build_cmd);
        strcat(object.check_cmd, " -MM");
                
        strcat(object.build_cmd, " -o ");
        strcat(object.build_cmd, object.file);

        time_t newest_source_time = get_file_time(source);
        time_t object_time = get_file_time(object.file);

        //fprintf(stderr, "%s\n", object.check_cmd);
        FILE* fp = popen(object.check_cmd, "r");
        while(fgetc(fp) != ':') {
            if(feof(fp)) {
                fprintf(stderr, "Error while checking dependencies. Command \n%s\n could not be executed.\n", object.check_cmd);
                exit(1);
            }
        };
        while(!feof(fp)) {
            string_t tmp;
            fscanf(fp, "%s", tmp);
            if(strcmp(tmp, "\\")) {
                newest_source_time = max(get_file_time(tmp), newest_source_time);
            }
        }
        if(newest_source_time > object_time) {
            printf("%s\n", object.build_cmd);
            system2(object.build_cmd);
        }
    } else {
        printf("No such directory: %s\n", build_dir);
    }
    return object;
}


int compile_many(object_t* objects, string_t* paths, int length, compiler_options_t options, string_t build_dir) {
    int successful = 1;
    for(int i = 0; i < length; i++) {
        objects[i] = compile(paths[i], options, build_dir);
        //if(objects[i].file[0] == '\0') successful = 0;
    }
    return successful;
}

int build(target_t* target, string_t build_dir) {
    string_t build_cmd = "";
    
    
    struct stat st_buf;
    if(stat(build_dir, &st_buf) != -1 && S_ISDIR(st_buf.st_mode)) {
        realpath(build_dir, target->output_dir);
        strcat(target->output_dir, "/");

        strcpy(target->file, target->output_dir);
        strcat(target->file, target->name);
        if(target->type == target_type_static_library) {
            strncat(build_cmd, tool_paths.ar_path, PATH_MAX);
            strncat(build_cmd, " cq ", PATH_MAX);
            strncat(target->file, ".a ", PATH_MAX);
            
        } else if(target->type == target_type_dynamic_library) {
            strncat(build_cmd, tool_paths.ar_path, PATH_MAX);
        } else {
            assert(target->type == target_type_executable);
            if(target->platform == platform_linux) {
                strncat(build_cmd, tool_paths.gcc_path, PATH_MAX);
            } else if(target->platform == platform_windows) {
                strncat(build_cmd, tool_paths.mxe_gcc_path, PATH_MAX);
            } else {
                strncat(build_cmd, tool_paths.emcc_path, PATH_MAX);
                strncat(build_cmd, " -O3", PATH_MAX);
                strncat(build_cmd, " -g3 -s USE_GLFW=3", PATH_MAX);
            }
            strcat(build_cmd, " -o ");
            if(target->platform == platform_web) {
                strncat(target->file, ".js", PATH_MAX);
            }
        }
        
        strncat(build_cmd, target->file, PATH_MAX);

        time_t newest_object_time = 0;
        time_t target_time = get_file_time(target->file);
        
        for(int i = 0; i < target->objects_length; i++) {
            strcat(build_cmd, " ");
            strcat(build_cmd, target->objects[i].file);
            newest_object_time = max(get_file_time(target->objects[i].file), newest_object_time);
        }
        
        if(newest_object_time > target_time) {
            if(target->type == target_type_executable) {
                for(int i = 0; i < target->library_paths_length; i++) {
                    strcat(build_cmd, " -L");
                    strcat(build_cmd, target->library_paths[i]);
                }

                for(int i = 0; i < target->libraries_length; i++) {
                    strcat(build_cmd, " -l");
                    strcat(build_cmd, target->libraries[i]);
                }
            }

            strcat(build_cmd, target->flags);

        
            printf("%s\n", build_cmd);
            int result = system2(build_cmd);
            if(result == 0 && target->type == target_type_static_library) {
                strcpy(build_cmd, tool_paths.ar_path);
                strcat(build_cmd, " -s ");
                strcat(build_cmd, target->file);

                printf("%s\n", build_cmd);
                return !system2(build_cmd);
            }
        }
        return 0;
    }
    return 1;
}

#endif
