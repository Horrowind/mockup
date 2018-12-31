#include "base/base.h"

int main(int argc, char** argv) {
    if(argc < 3) {
        c_print_format("Usage: %s SMW.sfc FILES\n", argv[0]);
        return 0;
    }
    
    Arena arena = arena_create(MB(64));
    Arena file_contents_arena = arena_create(MB(64));

    
    for(int i = 2; i < argc; i++) {
        Path path;
        path_init_c(&path, argv[i]);
        if(path_is_directory(&path)) {
            c_print_format("Dir\n");
            DirIter dir_iter = dir_iter_begin(&path);
            zero_buffer(arena.buffer);
            while(dir_iter_next(&dir_iter)) {
                String name   = path_get_base_name(&dir_iter.path, &arena);
                *arena_alloc_type(&file_contents_arena, u32) = name.length;
                Buffer name_buffer = arena_alloc_buffer(&file_contents_arena, name.length, alignof(u8));
                copy_buffer(buffer_from_string(name), name_buffer);
                Buffer contents = path_read_file(&dir_iter.path, &arena);
                *arena_alloc_type(&file_contents_arena, u32) = buffer_length(contents);
                Buffer contents_buffer = arena_alloc_buffer(&file_contents_arena, buffer_length(contents), alignof(u8));
                copy_buffer(contents, contents_buffer);
            }
            dir_iter_end(&dir_iter);
        } else {
            String name   = path_get_base_name(&path, &arena);
            *arena_alloc_type(&file_contents_arena, u32) = name.length;
            Buffer name_buffer = arena_alloc_buffer(&file_contents_arena, name.length, alignof(u8));
            copy_buffer(buffer_from_string(name), name_buffer);
            Buffer contents = path_read_file(&path, &arena);
            *arena_alloc_type(&file_contents_arena, u32) = buffer_length(contents);
            Buffer contents_buffer = arena_alloc_buffer(&file_contents_arena, buffer_length(contents), alignof(u8));
            copy_buffer(contents, contents_buffer);
        }
    }
    Path rom;
    path_init_c(&rom, argv[1]);
    Buffer rom_buffer = path_read_file(&rom, &arena);
    u8* src = rom_buffer.begin;
    for(u8* dest = file_contents_arena.buffer.begin; dest < (u8*)file_contents_arena.buffer.end; dest++) {
        if(src == rom_buffer.end) {
            src = rom_buffer.begin;
        }
        //*dest = (*dest) ^ (*src);
        src++;
    }

    Path file;
    path_create_c(&file, "smw.dis");
    Buffer file_contents_buffer = {
        .begin = file_contents_arena.buffer.begin,
        .end   = file_contents_arena.current
    };
    path_write_file(&file, file_contents_buffer);
    path_close(&file);
    return 0;
}
