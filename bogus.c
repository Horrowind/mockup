#define BOGUS_FILE_NAME __FILE__
#define BOGUS_IMPLEMENTATION
#include "bogus.h"

int main(int argc, char** argv) {    
    init();
    string_t build_dir = "build/";
    {
        compiler_options_t co;
        co.compiler = compiler_gcc;
        co.optimization_level = 0;
        compiler_options_add_include(&co, "lib/r65816/include/r65816");
        string_t build_obj_dir = "build/libr65816/";

        object_t algorithms_o   = compile("lib/r65816/src/algorithms.c", co, build_obj_dir);
        object_t breakpoint_o   = compile("lib/r65816/src/breakpoint.c", co, build_obj_dir);
        object_t cpu_o          = compile("lib/r65816/src/cpu.c", co, build_obj_dir);
        object_t disassembler_o = compile("lib/r65816/src/disassembler.c", co, build_obj_dir);
        object_t memory_o       = compile("lib/r65816/src/memory.c", co, build_obj_dir);
        object_t opcode_misc_o  = compile("lib/r65816/src/opcode_misc.c", co, build_obj_dir);
        object_t opcode_pc_o    = compile("lib/r65816/src/opcode_pc.c", co, build_obj_dir);
        object_t opcode_read_o  = compile("lib/r65816/src/opcode_read.c", co, build_obj_dir);
        object_t opcode_rmw_o   = compile("lib/r65816/src/opcode_rmw.c", co, build_obj_dir);
        object_t opcode_write_o = compile("lib/r65816/src/opcode_write.c", co, build_obj_dir);
        object_t rom_o          = compile("lib/r65816/src/rom.c", co, build_obj_dir);
        object_t run_o          = compile("lib/r65816/src/run.c", co, build_obj_dir);
        object_t run_jsl_o      = compile("lib/r65816/src/run_jsl.c", co, build_obj_dir);
        object_t run_jsr_o      = compile("lib/r65816/src/run_jsr.c", co, build_obj_dir);
        object_t step_o         = compile("lib/r65816/src/step.c", co, build_obj_dir);
        
        target_t libr65816 = target_init("libr65816", target_type_static_library);

        target_add_object(&libr65816, algorithms_o);
        target_add_object(&libr65816, breakpoint_o);
        target_add_object(&libr65816, cpu_o);
        target_add_object(&libr65816, disassembler_o);
        target_add_object(&libr65816, memory_o);
        target_add_object(&libr65816, opcode_misc_o);
        target_add_object(&libr65816, opcode_pc_o);
        target_add_object(&libr65816, opcode_read_o);
        target_add_object(&libr65816, opcode_rmw_o);
        target_add_object(&libr65816, opcode_write_o);
        target_add_object(&libr65816, rom_o);
        target_add_object(&libr65816, run_o);
        target_add_object(&libr65816, run_jsl_o);
        target_add_object(&libr65816, run_jsr_o);
        target_add_object(&libr65816, step_o);

        build(libr65816, build_dir);

    }

    {
        compiler_options_t co;
        co.compiler = compiler_gcc;
        co.optimization_level = 3;
        compiler_options_add_include(&co, "lib/r65816/include/");
        string_t build_obj_dir = "build/libmockup/";

        object_t decode_o    = compile("lib/mockup/decode.c", co, build_obj_dir);
        object_t gfx_store_o = compile("lib/mockup/gfx_store.c", co, build_obj_dir);
        object_t layer_o     = compile("lib/mockup/layer.c", co, build_obj_dir);
        object_t level_o     = compile("lib/mockup/level.c", co, build_obj_dir);
        object_t object_o    = compile("lib/mockup/object.c", co, build_obj_dir);
        object_t palette_o   = compile("lib/mockup/palette.c", co, build_obj_dir);
        object_t smw_o       = compile("lib/mockup/smw.c", co, build_obj_dir);
        object_t tiles_o     = compile("lib/mockup/tiles.c", co, build_obj_dir);
        object_t tileset_o   = compile("lib/mockup/tileset.c", co, build_obj_dir);

        target_t libmockup = target_init("libmockup", target_type_static_library);

        target_add_object(&libmockup, decode_o);
        target_add_object(&libmockup, gfx_store_o);
        target_add_object(&libmockup, layer_o);
        target_add_object(&libmockup, level_o);
        target_add_object(&libmockup, object_o);
        target_add_object(&libmockup, palette_o);
        target_add_object(&libmockup, smw_o);
        target_add_object(&libmockup, tiles_o);
        target_add_object(&libmockup, tileset_o);
        build(libmockup, build_dir);
    }

    {
        compiler_options_t co;
        co.compiler = compiler_gcc;
        co.optimization_level = 3;
        compiler_options_add_include(&co, "lib/r65816/include/");
        compiler_options_add_include(&co, "lib/mockup/");

        string_t build_obj_dir = "build/mockup_light/";
        object_t main_o = compile("app/mockup_light/main.c", co, build_obj_dir);

        target_t mockup_light = target_init("lmockup", target_type_executable);

        target_add_object(&mockup_light, main_o);
        build(mockup_light, build_dir);
    }
}
