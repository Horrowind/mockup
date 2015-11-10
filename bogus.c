#define BOGUS_FILE_NAME __FILE__
#define BOGUS_IMPLEMENTATION
#include "bogus.h"

int target_platform = 5;

void build_mockup_imgui() {
    compiler_options_t c_options, cpp_options;
    target_t mockup_imgui = target_init("imockup", target_type_executable);
    target_t mockup_light = target_init("lmockup", target_type_executable);
    string_t build_dir;
    target_t libr65816;
    string_t libr65816_sources[] = {
            "lib/r65816/src/algorithms.c",
            "lib/r65816/src/breakpoint.c",
            "lib/r65816/src/cpu.c",
            "lib/r65816/src/cpu_io.c",
            "lib/r65816/src/disassembler.c",
            "lib/r65816/src/memory.c",
            "lib/r65816/src/opcode_misc.c",
            "lib/r65816/src/opcode_pc.c",
            "lib/r65816/src/opcode_read.c",
            "lib/r65816/src/opcode_rmw.c",
            "lib/r65816/src/opcode_write.c",
            "lib/r65816/src/rom.c",
            /* "lib/r65816/src/run.c", */
            /* "lib/r65816/src/run_jsl.c", */
            /* "lib/r65816/src/run_jsr.c", */
            "lib/r65816/src/run_table.c",
            "lib/r65816/src/step.c"
        };
    object_t libr65816_objects[array_length(libr65816_sources)];
    string_t libr65816_include = "lib/r65816/include/r65816";
    
    target_t libmockup;
    string_t libmockup_sources[] = {
            "lib/mockup/decode.c",
            "lib/mockup/gfx_store.c",
            "lib/mockup/layer.c",
            "lib/mockup/level.c",
            "lib/mockup/object.c",
            "lib/mockup/palette.c",
            "lib/mockup/smw.c",
            "lib/mockup/tiles.c",
            "lib/mockup/tileset.c"
        };
    object_t libmockup_objects[array_length(libmockup_sources)];
    string_t libmockup_include = "lib/r65816/include";

    string_t imgui_sources[] = {
        "lib/imgui/imgui_impl_sdl_opengl.cpp",
        "lib/imgui/imgui.cpp",
        "lib/imgui/imgui_demo.cpp",
        "lib/imgui/imgui_draw.cpp"
    };
    string_t imgui_includes[] = {
        "/usr/include/SDL2",
        "lib/imgui"
    };
    object_t imgui_objects[array_length(imgui_sources)];

    string_t mockup_light_includes[] = {
        "lib/r65816/include/",
        "lib/mockup/"
    };

    object_t mockup_light_o;
    
    string_t main_libraries[] = {
        "GLEW",
        "GL",
        "SDL2"
    };

    string_t main_library_paths[] = {
        "/usr/lib/x86_64-linux-gnu",
    };

    string_t main_source = "app/mockup_imgui/main.cpp";
    string_t main_includes[] = {
        "/usr/include/SDL2",
        "lib/imgui",
        "lib/r65816/include/",
        "lib/mockup/"        
    };
    object_t main_object;

    int optimization_level = 3;
    if(target_platform == 1) {
        c_options = compiler_options_init(compiler_gcc, optimization_level);
        cpp_options = compiler_options_init(compiler_gpp, optimization_level);
        
        mockup_imgui.platform = platform_linux;
        mockup_light.platform = platform_linux;
        strcpy(build_dir, "build/linux/");
    } else if(target_platform == 2) {
        string_t mxe_path = "/home/horrowind/Projects/mxe/usr/i686-w64-mingw32.static";
        c_options = compiler_options_init(compiler_mxe_gcc, optimization_level);
        cpp_options = compiler_options_init(compiler_mxe_gpp, optimization_level);
        mockup_imgui.platform = platform_windows;
        mockup_light.platform = platform_windows;
        strcpy(imgui_includes[0], mxe_path);
        strcat(imgui_includes[0], "/include/SDL2");
        strcpy(main_includes[0], mxe_path);
        strcat(main_includes[0], "/include/SDL2");
        strcpy(main_libraries[0], "glew32");
        strcpy(main_libraries[1], "opengl32 ");
        strcpy(main_libraries[2], "SDL2 -lSDL2main");
        strcpy(main_libraries[2], "mingw32 -lSDL2main -lSDL2 -mwindows -Wl,--no-undefined -lm -ldinput8 -ldxguid -ldxerr8 -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lshell32 -lversion -luuid -static-libgcc");
        strcpy(main_library_paths[0], "/home/horrowind/Projects/mxe/usr/i686-w64-mingw32.static/lib");
        compiler_options_set_flags(&cpp_options, "-Dmain=SDL_main");
        strcpy(build_dir, "build/windows/");
    } else if(target_platform == 3) {
        c_options = compiler_options_init(compiler_emcc, optimization_level);
        cpp_options = compiler_options_init(compiler_empp, optimization_level);
        mockup_imgui.platform = platform_web;
        mockup_light.platform = platform_web;
        strcpy(imgui_sources[0], "lib/imgui/imgui_impl_sdl_opengles.cpp");
        strcpy(build_dir, "build/web/");
        //target_set_flags(&mockup_imgui, " -s TOTAL_MEMORY=67108864 --preload-file build@/smw.sfc");
        target_set_flags(&mockup_light, " -s TOTAL_MEMORY=67108864 -s USE_CLOSURE_COMPILER=1 -s EMULATED_FUNCTION_POINTERS=1 -s OUTLINING_LIMIT=20000");
        target_set_flags(&mockup_imgui, " -s TOTAL_MEMORY=67108864 -s USE_CLOSURE_COMPILER=1 -s EMULATED_FUNCTION_POINTERS=1 -s OUTLINING_LIMIT=20000");
    }
    compiler_options_set_flags(&c_options, "-g ");
    { // libr65816
        compiler_options_t c_options2 = c_options; 
        compiler_options_add_include(&c_options2, libr65816_include);
        string_t build_obj_dir;
        strcpy(build_obj_dir, build_dir);
        strcat(build_obj_dir, "libr65816");
        
        compile_many(libr65816_objects, libr65816_sources, array_length(libr65816_sources), c_options2, build_obj_dir);
    }

    { // libmockup
        compiler_options_t c_options2 = c_options;
        compiler_options_add_include(&c_options2, libmockup_include);
        string_t build_obj_dir;
        strcpy(build_obj_dir, build_dir);
        strcat(build_obj_dir, "libmockup");
        compile_many(libmockup_objects, libmockup_sources, array_length(libmockup_sources), c_options2, build_obj_dir);
    }
    
    { // mockup_light
 
        compiler_options_t c_options = c_options;
        compiler_options_add_includes(&c_options, mockup_light_includes, array_length(mockup_light_includes));
        string_t build_obj_dir;
        strcpy(build_obj_dir, build_dir);
        strcat(build_obj_dir, "mockup_light");
        mockup_light_o = compile("app/mockup_light/main.c", c_options, build_obj_dir);
    }

    target_add_object(&mockup_light, mockup_light_o);
    target_add_objects(&mockup_light, libr65816_objects, array_length(libr65816_objects));
    target_add_objects(&mockup_light, libmockup_objects, array_length(libmockup_objects));
    build(&mockup_light, build_dir);
    
    { // imgui
        compiler_options_t cpp_options2 = cpp_options;
        compiler_options_add_includes(&cpp_options2, imgui_includes, array_length(imgui_includes));
        string_t build_obj_dir;
        strcpy(build_obj_dir, build_dir);
        strcat(build_obj_dir, "libimgui");
        compile_many(imgui_objects, imgui_sources, array_length(imgui_sources), cpp_options2, build_obj_dir);
    }
   
    { // main
        compiler_options_t cpp_options2 = cpp_options;
        compiler_options_add_includes(&cpp_options2, main_includes, array_length(main_includes));
        string_t build_obj_dir;
        strcpy(build_obj_dir, build_dir);
        strcat(build_obj_dir, "mockup_imgui");
        main_object = compile("app/mockup_imgui/main.cpp", cpp_options2, build_obj_dir);
    }


    
    target_add_object(&mockup_imgui, main_object);
    target_add_objects(&mockup_imgui, libr65816_objects, array_length(libr65816_sources));
    target_add_objects(&mockup_imgui, libmockup_objects, array_length(libmockup_sources));
    target_add_objects(&mockup_imgui, imgui_objects, array_length(imgui_sources));
    target_add_libraries(&mockup_imgui, main_libraries, array_length(main_libraries));
    target_add_library_paths(&mockup_imgui, main_library_paths, array_length(main_library_paths));
    build(&mockup_imgui, build_dir);
}



/* void build_mockup_imgui() { */
/*     string_t sources[] = { */
/*         "app/mockup_imgui/main.cpp", */
/*         "app/mockup_imgui/imgui_impl_sdl.cpp", */
/*         "lib/imgui/imgui.cpp", */
/*         "lib/imgui/imgui_demo.cpp", */
/*         "lib/imgui/imgui_draw.cpp" */
/*     }; */

/*     string_t libraries[] = { */
/*         "GL", */
/*         "SDL2" */
/*     }; */

/*     string_t library_paths[] = { */
/*         "/usr/lib/x86_64-linux-gnu", */
/*     }; */
    
/*     object_t objects[array_length(sources)]; */
/*     compiler_options_t co; */
/*     target_t mockup_imgui = target_init("imockup", target_type_executable); */
/*     switch(2) { */
/*     case 0: */
/*         co = compiler_options_init(compiler_gpp, 0); */
/*         compiler_options_add_include(&co, "/usr/include/SDL2"); */
/*         compiler_options_add_include(&co, "lib/imgui"); */
/*         compiler_options_set_flags(&co, "-D_REENTRANT"); */
/*         mockup_imgui.platform = platform_linux; */
/*         break; */
/*     case 1: */
/*         break; */
/*     case 2: */
/*         co = compiler_options_init(compiler_empp, 2); */
/*         compiler_options_add_include(&co, "lib/imgui"); */
/*         mockup_imgui.platform = platform_web; */
/*         target_set_flags(&mockup_imgui, " -s USE_SDL=2"); */
/*         break; */
/*     } */
/*     compile_many(objects, sources, array_length(sources), co, "build/mockup_imgui"); */

    
/*     target_add_objects(&mockup_imgui, objects, array_length(objects)); */
/*     target_add_libraries(&mockup_imgui, libraries, array_length(libraries)); */
/*     target_add_library_paths(&mockup_imgui, library_paths, array_length(library_paths)); */
/*     string_t build_dir = "build/"; */
/*     build(&mockup_imgui, build_dir); */
/* } */

int main(int argc, char** argv) {
    init();
    int tp = target_platform;
    if(tp & 1) {
        target_platform = 1;
        build_mockup_imgui();
    }
    if(tp & 2) {
        target_platform = 2;
        build_mockup_imgui();
    }
    if(tp & 4) {
        target_platform = 3;
        build_mockup_imgui();
    }
} 
