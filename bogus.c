#define BUILD_CMD "gcc bogus.c -g -lcurl"
#define BOGUS_FILE_NAME __FILE__
#define BOGUS_IMPLEMENTATION

#include "bogus.h"
#include "bogus_cred.h"

#include <stdio.h>
#include <curl/curl.h>
#include <sys/time.h>
#include <unistd.h>

//int target_platform;
enum {
	TARGET_LINUX,
	TARGET_WINDOWS,
	TARGET_WEB,
} target_platform;

int build_mockup() {
    compiler_options_t c_options;
    target_t mockup = target_init("imockup", target_type_executable);
    target_t mockup_light = target_init("lmockup", target_type_executable);
    string_t build_dir;
    target_t libr65816;
    string_t libr65816_sources[] = {
        "lib/r65816/build.c"
    };
    object_t libr65816_objects[array_length(libr65816_sources)];
    string_t libr65816_include = "lib/base";
    
    target_t libmockup;
    string_t libmockup_sources[] = {
        "lib/mockup/build.c"
    };
    object_t libmockup_objects[array_length(libmockup_sources)];
    string_t libmockup_includes[] = {
        "lib/base",
        "lib/r65816"
    };
    
    string_t mockup_light_includes[] = {
        "lib/r65816/include/",
        "lib/mockup/"
    };

    object_t mockup_light_o;
    
    string_t main_libraries[] = {
        "GLEW",
        "GL",
        "glfw"
    };

    string_t main_library_paths[] = {
        "/usr/lib/x86_64-linux-gnu",
    };

    string_t main_sources[] = {
        "app/mockup/main.c",
        "app/mockup/nuklear.c"
    };
    object_t main_objects[array_length(main_sources)];;
    string_t main_includes[] = {
        "",
        "lib/base",
        "lib/r65816",
        "lib/mockup"
    };

    int optimization_level = 1;
    if(target_platform == TARGET_LINUX) {
        c_options = compiler_options_init(compiler_gcc, optimization_level);
        
        mockup.platform = platform_linux;
        mockup_light.platform = platform_linux;
        strcpy(build_dir, "build/linux/");
    } else if(target_platform == TARGET_WINDOWS) {
        string_t mxe_path = "/home/horrowind/Projects/mxe/usr/i686-w64-mingw32.static/include";
        c_options = compiler_options_init(compiler_mxe_gcc, optimization_level);
        mockup.platform = platform_windows;
        mockup_light.platform = platform_windows;
        strcpy(main_includes[0], mxe_path);
        strcpy(main_libraries[0], "glew32 -lglfw3");
        strcpy(main_libraries[1], "opengl32 ");
        strcpy(main_libraries[2], "mingw32 -mwindows -Wl,--no-undefined -lm -ldinput8 -ldxguid -ldxerr8 -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lshell32 -lversion -luuid -static-libgcc");
        strcpy(main_library_paths[0], "/home/horrowind/Projects/mxe/usr/i686-w64-mingw32.static/lib");
        strcpy(build_dir, "build/windows/");
    } else if(target_platform == TARGET_WEB) {
        c_options = compiler_options_init(compiler_emcc, optimization_level);
        mockup.platform = platform_web;
        mockup_light.platform = platform_web;
        strcpy(build_dir, "build/web/");
        //target_set_flags(&mockup, " -s TOTAL_MEMORY=67108864 --preload-file build@/smw.sfc");
        target_set_flags(&mockup_light, " -s TOTAL_MEMORY=67108864 -s USE_CLOSURE_COMPILER=1 -s EMULATED_FUNCTION_POINTERS=1 -s OUTLINING_LIMIT=20000");
        target_set_flags(&mockup, " -s TOTAL_MEMORY=67108864 -s USE_CLOSURE_COMPILER=1 -s EMULATED_FUNCTION_POINTERS=1 -s OUTLINING_LIMIT=20000");
    } else {
	assert(0 && "Unsupported Platform");
    }
    compiler_options_set_flags(&c_options, "-g -Wall -Werror");
    { // libr65816
        compiler_options_t c_options2 = c_options;
        c_options2.optimization_level = 3;
        compiler_options_add_include(&c_options2, libr65816_include);
        string_t build_obj_dir;
        strcpy(build_obj_dir, build_dir);
        strcat(build_obj_dir, "libr65816");
        
        int result = compile_many(libr65816_objects, libr65816_sources, array_length(libr65816_sources), c_options2, build_obj_dir);
	if(result) return result;
    }

    { // libmockup
        compiler_options_t c_options2 = c_options;
        compiler_options_add_includes(&c_options2, libmockup_includes, array_length(libmockup_includes));
        string_t build_obj_dir;
        strcpy(build_obj_dir, build_dir);
        strcat(build_obj_dir, "libmockup");
        int result = compile_many(libmockup_objects, libmockup_sources, array_length(libmockup_sources), c_options2, build_obj_dir);
	if(result) return result;
    }
    
    { // mockup_light
 
        compiler_options_t c_options = c_options;
        compiler_options_add_includes(&c_options, mockup_light_includes, array_length(mockup_light_includes));
        string_t build_obj_dir;
        strcpy(build_obj_dir, build_dir);
        strcat(build_obj_dir, "mockup_light");
        mockup_light_o = compile("app/mockup_light/main.c", c_options, build_obj_dir);
	if(mockup_light_o.result) return mockup_light_o.result;
    }

    target_add_object(&mockup_light, mockup_light_o);
    target_add_objects(&mockup_light, libr65816_objects, array_length(libr65816_objects));
    target_add_objects(&mockup_light, libmockup_objects, array_length(libmockup_objects));
    int result = build(&mockup_light, build_dir);
    if(result) return result;

    { // main
        compiler_options_t c_options2 = c_options;
        compiler_options_add_includes(&c_options2, main_includes, array_length(main_includes));
        string_t build_obj_dir;
        strcpy(build_obj_dir, build_dir);
        strcat(build_obj_dir, "mockup");
        int result = compile_many(main_objects, main_sources, array_length(main_sources), c_options2, build_obj_dir);
	if(result) return result;
    }


    
    target_add_objects(&mockup, main_objects, array_length(main_objects));
    target_add_objects(&mockup, libr65816_objects, array_length(libr65816_sources));
    target_add_objects(&mockup, libmockup_objects, array_length(libmockup_sources));
    target_add_library_paths(&mockup, main_library_paths, array_length(main_library_paths));
    target_add_libraries(&mockup, main_libraries, array_length(main_libraries));
    result = build(&mockup, build_dir);
    return result;
}

int perform_upload() {

    struct item {
        char* source;
        char* target;
    } items[] = {
        { .source = "html/mockup.html", .target = "/index.html" },
        { .source = "html/favicon.ico", .target = "/favicon.ico" },
        { .source = "build/web/imockup.js", .target = "/imockup.js" },
        { .source = "build/web/imockup.js.mem", .target = "/imockup.js.mem" },
        { .source = "build/web/imockup.html.mem", .target = "/imockup.html.mem" },
    };

    CURL* handles[sizeof(items) / sizeof(struct item)];
    CURLcode res;
    int still_running;
    
    FILE* fh[sizeof(items) / sizeof(struct item)];
 
    for(int i = 0; i < sizeof(items) / sizeof(struct item); i++) {
        handles[i] = curl_easy_init();
        if(handles[i]) {
            fh[i] = fopen(items[i].source, "rb");
            curl_off_t size;
            fseek (fh[i], 0, SEEK_END);
            size = ftell(fh[i]);
            rewind(fh[i]);
            char target_path[256] = UPLOAD_URL;
            strcat(target_path, items[i].target);
            curl_easy_setopt(handles[i], CURLOPT_URL, target_path);
            curl_easy_setopt(handles[i], CURLOPT_UPLOAD, 1L);
            curl_easy_setopt(handles[i], CURLOPT_READDATA, fh[i]);
            curl_easy_setopt(handles[i], CURLOPT_INFILESIZE_LARGE, size);
            printf("Uploading %s\n", items[i].target);
            curl_easy_perform(handles[i]);
        }
    }

    for(int i = 0; i < sizeof(items) / sizeof(struct item); i++) {
        curl_easy_cleanup(handles[i]);
    }
 
    return 0;
}


int main(int argc, char** argv) {
    init();
    int tp = 0;
    char c;
    while((c = getopt(argc, argv, "lwhu")) != -1) {
        switch(c) {
        case 'l':
            tp |= 1;
            break;
        case 'w':
            tp |= 2;
            break;
        case 'h':
            tp |= 4;
            break;
        case 'u':
            tp |= 8;
            break;
        }
    }
    if(tp == 0) tp = 1;
    
    if(tp & 1) {
        target_platform = TARGET_LINUX;
        int result = build_mockup();
	if(result) return 1;
    }
    if(tp & 2) {
        target_platform = TARGET_WINDOWS;
        int result = build_mockup();
	if(result) return 1;
    }
    if(tp & 4) {
        target_platform = TARGET_WEB;
        int result = build_mockup();
	if(result) return 1;
    }
    if(tp & 8) {
        perform_upload();
    }

} 
