#include "mockup/smw.h"

int main(int argc, char** argv) {
    Arena arena = arena_create(MB(1024));
    Arena temp_arena = arena_subarena(&arena, MB(256));

    String path_name = (argc > 1)
        ? string_from_c_string(argv[1])
        : L("SuperMarioWorld.sfc");
    Path path;
    path_init(&path, path_name);
    Path manifest_path;
    path_init_from_c(&manifest_path, &path, "manifest.bml");
    Buffer manifest_buffer = path_read_file(&manifest_path, &arena);

    Wdc65816MapperBuilder rom;
    
    char name[256];
    Buffer name_buffer = buffer(name, 256);
    for(Wdc65816RomLoader loader = wdc65816_rom_loader_begin(&rom, string_from_buffer(manifest_buffer));
        wdc65816_rom_loader_end(&loader);
        wdc65816_rom_loader_next(&loader)) {
        Wdc65816MemoryBufferRequest request = wdc65816_rom_loader_get_buffer_request(&loader, name_buffer);
        Buffer file_buffer;
        if(request.type == WDC65816_MEMORY_ROM) {
            Path file_path;
            path_init_from(&file_path, &path, request.name);
            
            file_buffer = path_read_file(&file_path, &arena);
        } else {
            file_buffer = buffer(arena_alloc_array(&arena, request.size, u8), request.size);
        }
        wdc65816_rom_loader_set_buffer(&loader, file_buffer);
    }
    
    SMW smw;
    smw_init_all(&smw, &rom, &arena, &temp_arena);
    smw_deinit(&smw);
    return 0;
}
