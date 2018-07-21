#include "rom.h"

Wdc65816RomLoader wdc65816_rom_loader_begin(Wdc65816MapperBuilder* rom, String manifest) {
    Wdc65816RomLoader result = {
        .rom = rom,
    };

    enum {
        ROM_LOAD_START, ROM_LOAD_GAME,  ROM_LOAD_BOARD, ROM_LOAD_MEMORY, 
        ROM_LOAD_LEAF,  ROM_LOAD_ERROR, ROM_LOAD_END,
    } state = ROM_LOAD_START;
    Buffer buffer = buffer_from_string(manifest);
    BmlParser parser;
    bml_parser_init(&parser, buffer);
    BmlNode node = bml_parse(&parser);

    uint index = 0;
    int cont = 1;
    while(cont) {
        if(node.type == BML_NODE_ERROR) {
            // TODO
        } else if(node.type == BML_NODE_EOF) {
            break;
        }
        
        switch(state) {
        case ROM_LOAD_START: {
            if(string_equal(node.name, L("game"))) {
                state = ROM_LOAD_GAME;
                node = bml_parse(&parser);
            } else {
                node = bml_skip(&parser);
            }
            break;
        }

        case ROM_LOAD_GAME: {
            if(string_equal(node.name, L("board"))) {
                state = ROM_LOAD_BOARD;
                wdc65816_load_board(&result.board, string_trim_spaces(node.value));
                node = bml_parse(&parser);
            } else {
                node = bml_skip(&parser);
            }
            break;
        }

        case ROM_LOAD_BOARD: {
            if(node.type == BML_NODE) {
                if(string_equal(node.name, L("memory"))) {
                    state = ROM_LOAD_MEMORY;
                    node = bml_parse(&parser);
                } else {
                    node = bml_skip(&parser);
                }
            } else if(node.type == BML_NODE_END) {
                state = ROM_LOAD_START;
                node = bml_parse(&parser);
            } else {
                node = bml_skip(&parser);
            }
            break;
        }

        case ROM_LOAD_MEMORY: {
            if(node.type == BML_NODE) {
                String value = string_trim_spaces(node.value);
                if(string_equal(node.name, L("content"))) {
                    result.contents[index] = value;
                    state = ROM_LOAD_LEAF;
                    node = bml_parse(&parser);
                } else if(string_equal(node.name, L("type"))) {
                    if(string_equal(value, L("ROM"))) {
                        result.types[index] = WDC65816_MEMORY_ROM;
                    } else if(string_equal(value, L("RAM"))) {
                        result.types[index] = WDC65816_MEMORY_RAM;
                    }
                    state = ROM_LOAD_LEAF;
                    node = bml_parse(&parser);
                } else if(string_equal(node.name, L("size"))) {
                    result.sizes[index] = string_to_u64(value);
                    state = ROM_LOAD_LEAF;
                    node = bml_parse(&parser);
                } else {
                    node = bml_skip(&parser);
                }
            } else if(node.type == BML_NODE_END) {
                index++;
                state = ROM_LOAD_BOARD;
                node = bml_parse(&parser);
            } else {
                node = bml_skip(&parser);
            }
            break;
        }
        
        case ROM_LOAD_LEAF: {
            if(node.type == BML_NODE) {
                node = bml_skip(&parser);
            } else if(node.type == BML_NODE_END) {
                state = ROM_LOAD_MEMORY;
                node = bml_parse(&parser);
            }
            break;
        }
        case ROM_LOAD_ERROR: {
            state = ROM_LOAD_END;
            break;
        }
            
        case ROM_LOAD_END: {
            cont = 0;
        }
            
        }
    }
    result.length = index;

    for(int i = 0; i < result.board.maps_length; i++) {
        int found = 0;
        for(int j = 0; j < result.length; j++) {
            if(string_equal(result.board.map_properties[i].content, result.contents[j])
               && result.board.map_properties[i].type == result.types[j]) {
                found = 1;
                break;
            }
        }
        if(!found) {
            invalid_code_path;
        }
    }
    
    
    return result;
}

void wdc65816_rom_loader_next(Wdc65816RomLoader* loader) {
    loader->index++;
}

Wdc65816MemoryBufferRequest wdc65816_rom_loader_get_buffer_request(Wdc65816RomLoader* loader,
                                                                   Buffer string_buffer) {
    int index = loader->index;
    StringBuilder builder = string_builder(string_buffer);
    string_builder_append(&builder, loader->contents[index]);
    if(loader->types[index] ==  WDC65816_MEMORY_ROM) {
        string_builder_append(&builder, L(".rom"));
    } else {
        string_builder_append(&builder, L(".ram"));
    }
    String name = string_from_builder(builder);
    lowcase_string(name);
    Wdc65816MemoryBufferRequest result = {
        .size = loader->sizes[index],
        .type = loader->types[index],
        .name = name
    };
    return result;
}

void wdc65816_rom_loader_set_buffer(Wdc65816RomLoader* loader, Buffer buffer) {
    loader->buffers[loader->index] = buffer;
}


int wdc65816_rom_loader_end(Wdc65816RomLoader* loader) {
    Wdc65816MapperBuilder* read_mapper = loader->rom;
    if(loader->index < loader->length) {
        return 1;
    } else {
        for(int i = 0; i < loader->board.maps_length; i++) {
            for(int j = 0; j < loader->length; j++) {
                if(string_equal(loader->board.map_properties[i].content, loader->contents[j])
                   && loader->board.map_properties[i].type == loader->types[j]) {
                    Wdc65816MapperEntry read_entry = {
                        .map  = loader->board.map_properties[i],
                        .data = loader->buffers[j],
                    };
                    wdc65816_mapper_builder_add(read_mapper, &read_entry);
                    break;
                }
            }
        }
        return 0;
    }

}
