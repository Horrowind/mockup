#include "board.h"

static
Wdc65816Board board_shvc_1a1b_06 = {
    .name = L("SHVC-1A1B-06"),
    .maps_length = 4,
    .map_properties = {
        {
            .low  = 0x008000,
            .high = 0x1fffff,
            .mask = 0x008000,
            .content = L("Program"),
            .type = WDC65816_MEMORY_ROM,
        },
        {
            .low  = 0x808000,
            .high = 0x9fffff,
            .mask = 0x008000,
            .content = L("Program"),
            .type = WDC65816_MEMORY_ROM,
        },
        {
            .low  = 0x700000,
            .high = 0x7dffff,
            .content = L("Save"),
            .type = WDC65816_MEMORY_RAM,
        },
        {
            .low  = 0xF00000,
            .high = 0xFFffff,
            .content = L("Save"),
            .type = WDC65816_MEMORY_RAM,
        },
    }
};


void wdc65816_load_board(Wdc65816Board* board, String board_name) {
    if(string_equal(board_name, L("SHVC-1A1B-06"))) {
        *board = board_shvc_1a1b_06;
    } else {
        not_implemented;
    }
    
#if 0

    enum {
        ROM_LOAD_START, ROM_LOAD_BOARD, ROM_LOAD_MEMORY, 
        ROM_LOAD_LEAF,  ROM_LOAD_ERROR, ROM_LOAD_END,
    } state = ROM_LOAD_START;
    Buffer buffer = buffer_from_string(boards_bml);
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

    enum {
        ROM_LOAD_START,   ROM_LOAD_BOARD,   ROM_LOAD_MEMORY, 
        ROM_LOAD_CONTENT, ROM_LOAD_SIZE,    ROM_LOAD_BASE,
        ROM_LOAD_MAP,     ROM_LOAD_MASK,    ROM_LOAD_ADDRESS,
        ROM_LOAD_TYPE,    ROM_LOAD_ERROR,   ROM_LOAD_END,
    } state = ROM_LOAD_START;
    Buffer buffer = buffer(boards_bml, sizeof(boards_bml));
    BmlParser parser;
    bml_parser_init(&parser, buffer);
    bml_parse(&parser);

    uint map_index    = 0;

    while(1) {
        BmlNode node = parser.last_node;
        Wdc65816Memory*        memory = board->memories + memory_index;
        Wdc65816MapProperties* map    = memory->map_properties + map_index;
        if(node.type >= BML_NODE_ERROR) {
            // TODO
        }
        
        switch(state) {
        case ROM_LOAD_START: {
            if(string_equal(node.name, L("board"))) {
                state = ROM_LOAD_BOARD;
                bml_parse(&parser);
            } else {
                bml_skip(&parser);
            }
            break;
        }

        case ROM_LOAD_BOARD: {
            if(node.type == BML_NODE) {
                if(string_equal(node.name, L("memory"))) {
                    state = ROM_LOAD_MEMORY;
                    bml_parse(&parser);
                } else {
                    bml_skip(&parser);
                }
            } else if(node.type == BML_NODE_END) {
                state = ROM_LOAD_START;
                bml_parse(&parser);
            } else {
                bml_skip(&parser);
            }
            break;
        }

        case ROM_LOAD_MEMORY: {
            if(node.type == BML_NODE) {
                if(string_equal(node.name, L("content"))) {
                    state = ROM_LOAD_CONTENT;
                    bml_parse(&parser);
                } else if(string_equal(node.name, L("type"))) {
                    state = ROM_LOAD_TYPE;
                    bml_parse(&parser);
                } else if(string_equal(node.name, L("size"))) {
                    state = ROM_LOAD_SIZE;
                    bml_parse(&parser);
                } else if(string_equal(node.name, L("base"))) {
                    state = ROM_LOAD_BASE;
                    bml_parse(&parser);
                } else if(string_equal(node.name, L("map"))) {
                    state = ROM_LOAD_MAP;
                    bml_parse(&parser);
                } else {
                    bml_skip(&parser);
                }
            } else if(node.type == BML_NODE_END) {
                memory_index++;
                state = ROM_LOAD_BOARD;
                bml_parse(&parser);
            } else {
                bml_skip(&parser);
            }
            break;
        }
        
        case ROM_LOAD_TYPE: {
            if(node.type == BML_NODE) {
                if(string_equal(node.name, L("RAM"))) {
                    memory->type = WDC65816_MEMORY_RAM;
                    bml_parse(&parser);
                } else if(string_equal(node.name, L("RAM"))) {
                    memory->type = WDC65816_MEMORY_RAM;
                    bml_parse(&parser);
                } else {
                    bml_skip(&parser);
                }
            } else if(node.type == BML_NODE_END) {
                state = ROM_LOAD_MEMORY;
                bml_parse(&parser);
            } else {
                bml_skip(&parser);
            }
            break;
        }
            
        case ROM_LOAD_CONTENT: {
            memory->content = node.value;
            if(node.type == BML_NODE_END) {
                state = ROM_LOAD_MEMORY;
                bml_parse(&parser);
            } else {
                bml_skip(&parser);
            }
            break;
        }

        case ROM_LOAD_MAP: {
            if(node.type == BML_NODE) {
                if(string_equal(node.name, L("address"))) {
                    state = ROM_LOAD_ADDRESS;
                    bml_parse(&parser);
                } else if(string_equal(node.name, L("size"))) {
                    state = ROM_LOAD_SIZE;
                    bml_parse(&parser);
                } else if(string_equal(node.name, L("base"))) {
                    state = ROM_LOAD_BASE;
                    bml_parse(&parser);
                } else if(string_equal(node.name, L("mask"))) {
                    state = ROM_LOAD_MASK;
                    bml_parse(&parser);
                } else {
                    bml_skip(&parser);
                }
            } else if(node.type == BML_NODE_END) {
                memory_index++;
                state = ROM_LOAD_BOARD;
                bml_parse(&parser);
            } else {
                bml_skip(&parser);
            }
            break;
        }

        case ROM_LOAD_SIZE: {
            map->size = string_to_u64(node.value);
            if(node.type == BML_NODE_END) {
                state = ROM_LOAD_MEMORY;
                bml_parse(&parser);
            } else {
                bml_skip(&parser);
            }
            break;
        }

        case ROM_LOAD_BASE: {
            map->base = string_to_u64(node.value);
            if(node.type == BML_NODE_END) {
                state = ROM_LOAD_MEMORY;
                bml_parse(&parser);
            } else {
                bml_skip(&parser);
            }
            break;
        }
            
            
        case ROM_LOAD_MASK: {
            map->mask = string_to_u64(node.value);
            if(node.type == BML_NODE_END) {
                state = ROM_LOAD_MAP;
                bml_parse(&parser);
            } else {
                bml_skip(&parser);
            }
            break;
        }
            
        case ROM_LOAD_ADDRESS: {
            String addr_string = node.value;
            char* delim = addr_string.data;
            while(*delim != ':') {
                delim++;
                if(delim - addr_string.data >= addr_string.length) {
                    fprintf(stderr, "Error in manifest!\n");
                    exit(1);
                }
            }

            String banks = { .data = addr_string.data, .length = delim - addr_string.data };
            String addrs = { .data = delim + 1,        .length = addr_string.length - banks.length - 1};

            string_split_foreach(banks, bank, ',') {
                char* delim = bank.data;
                while(*delim != '-') {
                    delim++;
                    if(delim - bank.data >= bank.length) {
                        break;
                    }
                }

                String bank_low_str  = { .data = bank.data, .length = delim - bank.data };
                String bank_high_str = { .data = delim + 1, .length = bank.length - bank_low_str.length + 1 };
                map->bank_low  = (u8)string_to_u64_base(bank_low_str, 16);
                map->bank_high = (u8)string_to_u64_base(bank_high_str, 16);
                map->bank_high = map->bank_high ? map->bank_high : map->bank_low;

                string_split_foreach(addrs, addr, ',') {
                    char* delim = addr.data;
                    while(*delim != '-') {
                        delim++;
                        if(delim - addr.data >= addr.length) {
                            break;
                        }
                    }

                    String addr_low_str  = { .data = addr.data, .length = delim - addr.data };
                    String addr_high_str = { .data = delim + 1, .length = addr.length - addr_low_str.length + 1 };
                    map->addr_low  = (u16)string_to_u64_base(addr_low_str, 16);
                    map->addr_high = (u16)string_to_u64_base(addr_high_str, 16);
                    map->addr_high = map->addr_high ? map->addr_high : map->addr_low;
                }
            }

            if(node.type == BML_NODE_END) {
                state = ROM_LOAD_MAP;
                bml_parse(&parser);
            } else {
                bml_skip(&parser);
            }
            break;
        }
            
        case ROM_LOAD_ERROR: {
            state = ROM_LOAD_END;
            break;
        }
            
        case ROM_LOAD_END: {
            state = ROM_LOAD_END;
            return;
        }
            
        }
    }
#endif
}

#if 0
static
char boards_bml[] =
    "database\n"
    "  revision: 2018-06-01\n"
    "\n"
    "//Boards (Production)\n"
    "\n"
    "database\n"
    "  revision: 2018-05-16\n"
    "\n"
    "board: BANDAI-PT-923\n"
    "  memory type=ROM content=Program\n"
    "    map address=00-1f,80-9f:8000-ffff mask=0x8000\n"
    "  slot type=SufamiTurbo\n"
    "    rom\n"
    "      map address=20-3f,a0-bf:8000-ffff mask=0x8000\n"
    "    ram\n"
    "      map address=60-6f,e0-ef:0000-ffff\n"
    "  slot type=SufamiTurbo\n"
    "    rom\n"
    "      map address=40-5f,c0-df:0000-ffff mask=0x8000\n"
    "    ram\n"
    "      map address=70-7d,f0-ff:0000-ffff\n"
    "\n"
    "board: BSC-1A5B9P-01\n"
    "  memory type=RAM content=Save\n"
    "    map address=10-17:5000-5fff mask=0xf000\n"
    "  processor identifier=MCC\n"
    "    map address=00-0f:5000-5fff\n"
    "    mcu\n"
    "      map address=00-3f,80-bf:8000-ffff\n"
    "      map address=40-7d,c0-ff:0000-ffff\n"
    "      map address=20-3f,a0-bf:6000-7fff\n"
    "      memory type=ROM content=Program\n"
    "      memory type=RAM content=Download\n"
    "      slot type=BSMemory\n"
    "\n"
    "board: BSC-1A5M-02\n"
    "  memory type=ROM content=Program\n"
    "    map address=00-1f:8000-ffff mask=0x8000 base=0x000000\n"
    "    map address=20-3f:8000-ffff mask=0x8000 base=0x100000\n"
    "    map address=80-9f:8000-ffff mask=0x8000 base=0x200000\n"
    "    map address=a0-bf:8000-ffff mask=0x8000 base=0x100000\n"
    "  memory type=RAM content=Save\n"
    "    map address=70-7d,f0-ff:0000-7fff mask=0x8000\n"
    "  slot type=BSMemory\n"
    "    map address=c0-ef:0000-ffff\n"
    "\n"
    "board: BSC-1A7M-01\n"
    "  memory type=ROM content=Program\n"
    "    map address=00-1f:8000-ffff mask=0x8000 base=0x000000\n"
    "    map address=20-3f:8000-ffff mask=0x8000 base=0x100000\n"
    "    map address=80-9f:8000-ffff mask=0x8000 base=0x200000\n"
    "    map address=a0-bf:8000-ffff mask=0x8000 base=0x100000\n"
    "  memory type=RAM content=Save\n"
    "    map address=70-7d,f0-ff:0000-7fff mask=0x8000\n"
    "  slot type=BSMemory\n"
    "    map address=c0-ef:0000-ffff\n"
    "\n"
    "board: BSC-1J3M-01\n"
    "  memory type=ROM content=Program\n"
    "    map address=00-1f,80-9f:8000-ffff\n"
    "    map address=40-5f,c0-df:0000-ffff\n"
    "  memory type=RAM content=Save\n"
    "    map address=20-3f,a0-bf:6000-7fff mask=0xe000\n"
    "  slot type=BSMemory\n"
    "    map address=20-3f,a0-bf:8000-ffff\n"
    "    map address=60-7d,e0-ff:0000-ffff\n"
    "\n"
    "board: BSC-1J5M-01\n"
    "  memory type=ROM content=Program\n"
    "    map address=00-1f,80-9f:8000-ffff\n"
    "    map address=40-5f,c0-df:0000-ffff\n"
    "  memory type=RAM content=Save\n"
    "    map address=20-3f,a0-bf:6000-7fff mask=0xe000\n"
    "  slot type=BSMemory\n"
    "    map address=20-3f,a0-bf:8000-ffff\n"
    "    map address=60-7d,e0-ff:0000-ffff\n"
    "\n"
    "board: BSC-1L3B-01\n"
    "  processor architecture=W65C816S\n"
    "    map address=00-3f,80-bf:2200-23ff\n"
    "    mcu\n"
    "      map address=00-3f,80-bf:8000-ffff mask=0x408000\n"
    "      map address=c0-ff:0000-ffff\n"
    "      memory type=ROM content=Program\n"
    "      slot type=BSMemory\n"
    "    memory type=RAM content=Save\n"
    "      map address=00-3f,80-bf:6000-7fff size=0x2000\n"
    "      map address=40-4f:0000-ffff\n"
    "    memory type=RAM content=Internal\n"
    "      map address=00-3f,80-bf:3000-37ff size=0x800\n"
    "\n"
    "board: BSC-1L5B-01\n"
    "  processor architecture=W65C816S\n"
    "    map address=00-3f,80-bf:2200-23ff\n"
    "    mcu\n"
    "      map address=00-3f,80-bf:8000-ffff mask=0x408000\n"
    "      map address=c0-ff:0000-ffff\n"
    "      memory type=ROM content=Program\n"
    "      slot type=BSMemory\n"
    "    memory type=RAM content=Save\n"
    "      map address=00-3f,80-bf:6000-7fff size=0x2000\n"
    "      map address=40-4f:0000-ffff\n"
    "    memory type=RAM content=Internal\n"
    "      map address=00-3f,80-bf:3000-37ff size=0x800\n"
    "\n"
    "board: SGB-R-10\n"
    "  memory type=ROM content=Program\n"
    "    map address=00-7d,80-ff:8000-ffff mask=0x8000\n"
    "    map address=40-7d,c0-ff:0000-7fff mask=0x8000\n"
    "  processor identifier=ICD revision=2\n"
    "    map address=00-3f,80-bf:6000-67ff,7000-7fff\n"
    "    memory type=ROM content=Boot architecture=LR35902\n"
    "    slot type=GameBoy\n"
    "\n"
    "board: SHVC-1A0N-(01,02,10,20,30)\n"
    "  memory type=ROM content=Program\n"
    "    map address=00-7d,80-ff:8000-ffff mask=0x8000\n"
    "    map address=40-7d,c0-ff:0000-7fff mask=0x8000\n"
    "\n"
    "board: SHVC-1A1B-(04,05,06)\n"
    "  memory type=ROM content=Program\n"
    "    map address=00-1f,80-9f:8000-ffff mask=0x8000\n"
    "  memory type=RAM content=Save\n"
    "    map address=70-7d,f0-ff:0000-ffff\n"
    "\n"
    "board: SHVC-1A1M-(01,10,11,20)\n"
    "  memory type=ROM content=Program\n"
    "    map address=00-7d,80-ff:8000-ffff mask=0x8000\n"
    "  memory type=RAM content=Save\n"
    "    map address=70-7d,f0-ff:0000-7fff mask=0x8000\n"
    "\n"
    "board: SHVC-1A3B-(11,12,13)\n"
    "  memory type=ROM content=Program\n"
    "    map address=00-1f,80-9f:8000-ffff mask=0x8000\n"
    "  memory type=RAM content=Save\n"
    "    map address=70-7d,f0-ff:0000-ffff\n"
    "\n"
    "board: SHVC-1A3B-20\n"
    "  memory type=ROM content=Program\n"
    "    map address=00-7d,80-ff:8000-ffff mask=0x8000\n"
    "  memory type=RAM content=Save\n"
    "    map address=70-7d,f0-ff:0000-7fff mask=0x8000\n"
    "\n"
    "board: SHVC-1A3M-(10,20,21,30)\n"
    "  memory type=ROM content=Program\n"
    "    map address=00-7d,80-ff:8000-ffff mask=0x8000\n"
    "  memory type=RAM content=Save\n"
    "    map address=70-7d,f0-ff:0000-7fff mask=0x8000\n"
    "\n"
    "board: SHVC-1A5B-(02,04)\n"
    "  memory type=ROM content=Program\n"
    "    map address=00-1f,80-9f:8000-ffff mask=0x8000\n"
    "  memory type=RAM content=Save\n"
    "    map address=70-7d,f0-ff:0000-ffff\n"
    "\n"
    "board: SHVC-1A5M-(01,11,20)\n"
    "  memory type=ROM content=Program\n"
    "    map address=00-7d,80-ff:8000-ffff mask=0x8000\n"
    "  memory type=RAM content=Save\n"
    "    map address=70-7d,f0-ff:0000-7fff mask=0x8000\n"
    "\n"
    "board: SHVC-1B0N-(02,03,10)\n"
    "  memory type=ROM content=Program\n"
    "    map address=00-1f,80-9f:8000-ffff mask=0x8000\n"
    "  processor architecture=uPD7725\n"
    "    map address=30-3f,b0-bf:8000-ffff mask=0x3fff\n"
    "    memory type=ROM content=Program architecture=uPD7725\n"
    "    memory type=ROM content=Data architecture=uPD7725\n"
    "    memory type=RAM content=Data architecture=uPD7725\n"
    "    oscillator\n"
    "\n"
    "board: SHVC-1B5B-02\n"
    "  memory type=ROM content=Program\n"
    "    map address=00-1f,80-9f:8000-ffff mask=0x8000\n"
    "  memory type=RAM content=Save\n"
    "    map address=70-7d,f0-ff:0000-ffff\n"
    "  processor architecture=uPD7725\n"
    "    map address=20-3f,a0-bf:8000-ffff mask=0x3fff\n"
    "    memory type=ROM content=Program architecture=uPD7725\n"
    "    memory type=ROM content=Data architecture=uPD7725\n"
    "    memory type=RAM content=Data architecture=uPD7725\n"
    "    oscillator\n"
    "\n"
    "board: SHVC-1C0N\n"
    "  processor architecture=GSU\n"
    "    map address=00-3f,80-bf:3000-34ff\n"
    "    memory type=ROM content=Program\n"
    "      map address=00-1f,80-9f:8000-ffff mask=0x8000\n"
    "    memory type=RAM content=Save\n"
    "      map address=60-7d,e0-ff:0000-ffff\n"
    "\n"
    "board: SHVC-1C0N5S-01\n"
    "  processor architecture=GSU\n"
    "    map address=00-3f,80-bf:3000-34ff\n"
    "    memory type=ROM content=Program\n"
    "      map address=00-1f,80-9f:8000-ffff mask=0x8000\n"
    "    memory type=RAM content=Save\n"
    "      map address=60-7d,e0-ff:0000-ffff\n"
    "\n"
    "board: SHVC-1CA0N5S-01\n"
    "  processor architecture=GSU\n"
    "    map address=00-3f,80-bf:3000-34ff\n"
    "    memory type=ROM content=Program\n"
    "      map address=00-3f,80-bf:8000-ffff mask=0x8000\n"
    "      map address=40-5f,c0-df:0000-ffff\n"
    "    memory type=RAM content=Save\n"
    "      map address=00-3f,80-bf:6000-7fff size=0x2000\n"
    "      map address=70-71,f0-f1:0000-ffff\n"
    "\n"
    "board: SHVC-1CA0N6S-01\n"
    "  processor architecture=GSU\n"
    "    map address=00-3f,80-bf:3000-34ff\n"
    "    memory type=ROM content=Program\n"
    "      map address=00-3f,80-bf:8000-ffff mask=0x8000\n"
    "      map address=40-5f,c0-df:0000-ffff\n"
    "    memory type=RAM content=Save\n"
    "      map address=00-3f,80-bf:6000-7fff size=0x2000\n"
    "      map address=70-71,f0-f1:0000-ffff\n"
    "\n"
    "board: SHVC-1CA6B-01\n"
    "  processor architecture=GSU\n"
    "    map address=00-3f,80-bf:3000-34ff\n"
    "    memory type=ROM content=Program\n"
    "      map address=00-3f,80-bf:8000-ffff mask=0x8000\n"
    "      map address=40-5f,c0-df:0000-ffff\n"
    "    memory type=RAM content=Save\n"
    "      map address=00-3f,80-bf:6000-7fff size=0x2000\n"
    "      map address=70-71,f0-f1:0000-ffff\n"
    "\n"
    "board: SHVC-1CB0N7S-01\n"
    "  processor architecture=GSU\n"
    "    map address=00-3f,80-bf:3000-34ff\n"
    "    memory type=ROM content=Program\n"
    "      map address=00-3f:8000-ffff mask=0x8000\n"
    "      map address=40-5f:0000-ffff\n"
    "    memory type=RAM content=Save\n"
    "      map address=00-3f,80-bf:6000-7fff size=0x2000\n"
    "      map address=70-71:0000-ffff\n"
    "\n"
    "board: SHVC-1CB5B-20\n"
    "  processor architecture=GSU\n"
    "    map address=00-3f,80-bf:3000-34ff\n"
    "    memory type=ROM content=Program\n"
    "      map address=00-3f:8000-ffff mask=0x8000\n"
    "      map address=40-5f:0000-ffff\n"
    "    memory type=RAM content=Save\n"
    "      map address=00-3f,80-bf:6000-7fff size=0x2000\n"
    "      map address=70-71:0000-ffff\n"
    "\n"
    "board: SHVC-1CB7B-01\n"
    "  processor architecture=GSU\n"
    "    map address=00-3f,80-bf:3000-34ff\n"
    "    memory type=ROM content=Program\n"
    "      map address=00-3f:8000-ffff mask=0x8000\n"
    "      map address=40-5f:0000-ffff\n"
    "    memory type=RAM content=Save\n"
    "      map address=00-3f,80-bf:6000-7fff size=0x2000\n"
    "      map address=70-71:0000-ffff\n"
    "\n"
    "board: SHVC-1DC0N-01\n"
    "  processor architecture=HG51BS169\n"
    "    map address=00-3f,80-bf:6c00-6fff,7c00-7fff\n"
    "    memory type=ROM content=Program\n"
    "      map address=00-3f,80-bf:8000-ffff mask=0x8000\n"
    "    memory type=RAM content=Save\n"
    "      map address=70-77:0000-7fff\n"
    "    memory type=ROM content=Data architecture=HG51BS169\n"
    "    memory type=RAM content=Data architecture=HG51BS169\n"
    "      map address=00-3f,80-bf:6000-6bff,7000-7bff mask=0xf000\n"
    "    oscillator\n"
    "\n"
    "board: SHVC-1DS0B-20\n"
    "  memory type=ROM content=Program\n"
    "    map address=00-7d,80-ff:8000-ffff mask=0x8000\n"
    "  processor architecture=uPD96050\n"
    "    map address=60-67,e0-e7:0000-3fff\n"
    "    memory type=ROM content=Program architecture=uPD96050\n"
    "    memory type=ROM content=Data architecture=uPD96050\n"
    "    memory type=RAM content=Data architecture=uPD96050\n"
    "      map address=68-6f,e8-ef:0000-7fff mask=0x8000\n"
    "    oscillator\n"
    "\n"
    "board: SHVC-1J0N-(01,10,20)\n"
    "  memory type=ROM content=Program\n"
    "    map address=00-3f,80-bf:8000-ffff\n"
    "    map address=40-7d,c0-ff:0000-ffff\n"
    "\n"
    "board: SHVC-1J1M-(11,20)\n"
    "  memory type=ROM content=Program\n"
    "    map address=00-3f,80-bf:8000-ffff\n"
    "    map address=40-7d,c0-ff:0000-ffff\n"
    "  memory type=RAM content=Save\n"
    "    map address=20-3f,a0-bf:6000-7fff mask=0xe000\n"
    "\n"
    "board: SHVC-1J3B-01\n"
    "  memory type=ROM content=Program\n"
    "    map address=00-3f,80-bf:8000-ffff\n"
    "    map address=40-7d,c0-ff:0000-ffff\n"
    "  memory type=RAM content=Save\n"
    "    map address=20-3f,a0-bf:6000-7fff mask=0xe000\n"
    "\n"
    "board: SHVC-1J3M-(01,11,20)\n"
    "  memory type=ROM content=Program\n"
    "    map address=00-3f,80-bf:8000-ffff\n"
    "    map address=40-7d,c0-ff:0000-ffff\n"
    "  memory type=RAM content=Save\n"
    "    map address=20-3f,a0-bf:6000-7fff mask=0xe000\n"
    "\n"
    "board: SHVC-1J5M-(01,11,20)\n"
    "  memory type=ROM content=Program\n"
    "    map address=00-3f,80-bf:8000-ffff\n"
    "    map address=40-7d,c0-ff:0000-ffff\n"
    "  memory type=RAM content=Save\n"
    "    map address=20-3f,a0-bf:6000-7fff mask=0xe000\n"
    "\n"
    "board: SHVC-1K0N-01\n"
    "  memory type=ROM content=Program\n"
    "    map address=00-3f,80-bf:8000-ffff\n"
    "    map address=40-7d,c0-ff:0000-ffff\n"
    "  processor architecture=uPD7725\n"
    "    map address=00-1f,80-9f:6000-7fff mask=0xfff\n"
    "    memory type=ROM content=Program architecture=uPD7725\n"
    "    memory type=ROM content=Data architecture=uPD7725\n"
    "    memory type=RAM content=Data architecture=uPD7725\n"
    "    oscillator\n"
    "\n"
    "board: SHVC-1K1B-01\n"
    "  memory type=ROM content=Program\n"
    "    map address=00-3f,80-bf:8000-ffff\n"
    "    map address=40-7d,c0-ff:0000-ffff\n"
    "  memory type=RAM content=Save\n"
    "    map address=20-3f,a0-bf:6000-7fff mask=0xe000\n"
    "  processor architecture=uPD7725\n"
    "    map address=00-1f,80-9f:6000-7fff mask=0xfff\n"
    "    memory type=ROM content=Program architecture=uPD7725\n"
    "    memory type=ROM content=Data architecture=uPD7725\n"
    "    memory type=RAM content=Data architecture=uPD7725\n"
    "    oscillator\n"
    "\n"
    "board: SHVC-1L0N3S-01\n"
    "  processor architecture=W65C816S\n"
    "    map address=00-3f,80-bf:2200-23ff\n"
    "    mcu\n"
    "      map address=00-3f,80-bf:8000-ffff mask=0x408000\n"
    "      map address=c0-ff:0000-ffff\n"
    "      memory type=ROM content=Program\n"
    "    memory type=RAM content=Save\n"
    "      map address=00-3f,80-bf:6000-7fff size=0x2000\n"
    "      map address=40-4f:0000-ffff\n"
    "    memory type=RAM content=Internal\n"
    "      map address=00-3f,80-bf:3000-37ff size=0x800\n"
    "\n"
    "board: SHVC-1L3B-(02,11)\n"
    "  processor architecture=W65C816S\n"
    "    map address=00-3f,80-bf:2200-23ff\n"
    "    mcu\n"
    "      map address=00-3f,80-bf:8000-ffff mask=0x408000\n"
    "      map address=c0-ff:0000-ffff\n"
    "      memory type=ROM content=Program\n"
    "    memory type=RAM content=Save\n"
    "      map address=00-3f,80-bf:6000-7fff size=0x2000\n"
    "      map address=40-4f:0000-ffff\n"
    "    memory type=RAM content=Internal\n"
    "      map address=00-3f,80-bf:3000-37ff size=0x800\n"
    "\n"
    "board: SHVC-1L5B-(11,20)\n"
    "  processor architecture=W65C816S\n"
    "    map address=00-3f,80-bf:2200-23ff\n"
    "    mcu\n"
    "      map address=00-3f,80-bf:8000-ffff mask=0x408000\n"
    "      map address=c0-ff:0000-ffff\n"
    "      memory type=ROM content=Program\n"
    "    memory type=RAM content=Save\n"
    "      map address=00-3f,80-bf:6000-7fff size=0x2000\n"
    "      map address=40-4f:0000-ffff\n"
    "    memory type=RAM content=Internal\n"
    "      map address=00-3f,80-bf:3000-37ff size=0x800\n"
    "\n"
    "board: SHVC-1N0N-(01,10)\n"
    "  processor identifier=SDD1\n"
    "    map address=00-3f,80-bf:4800-480f\n"
    "    mcu\n"
    "      map address=00-3f,80-bf:8000-ffff\n"
    "      map address=c0-ff:0000-ffff\n"
    "      memory type=ROM content=Program\n"
    "\n"
    "board: SHVC-2A0N-01#A\n"
    "  memory type=ROM content=Program\n"
    "    map address=00-2f,80-af:8000-ffff mask=0x8000\n"
    "    map address=40-6f,c0-ef:0000-ffff mask=0x8000\n"
    "\n"
    "board: SHVC-2A0N-(01,10,11,20)\n"
    "  memory type=ROM content=Program\n"
    "    map address=00-7d,80-ff:8000-ffff mask=0x8000\n"
    "    map address=40-7d,c0-ff:0000-7fff mask=0x8000\n"
    "\n"
    "board: SHVC-2A1M-01\n"
    "  memory type=ROM content=Program\n"
    "    map address=00-7d,80-ff:8000-ffff mask=0x8000\n"
    "  memory type=RAM content=Save\n"
    "    map address=70-7d,f0-ff:0000-7fff mask=0x8000\n"
    "\n"
    "board: SHVC-2A3B-01\n"
    "  memory type=ROM content=Program\n"
    "    map address=00-3f,80-bf:8000-ffff mask=0x8000\n"
    "  memory type=RAM content=Save\n"
    "    map address=70-7d,f0-ff:0000-7fff mask=0x8000\n"
    "\n"
    "board: SHVC-2A3M-01#A\n"
    "  memory type=ROM content=Program\n"
    "    map address=00-3f,80-bf:8000-ffff mask=0x8000\n"
    "  memory type=RAM content=Save\n"
    "    map address=70-7d,f0-ff:0000-7fff mask=0x8000\n"
    "\n"
    "board: SHVC-2A3M-(01,11,20)\n"
    "  memory type=ROM content=Program\n"
    "    map address=00-7d,80-ff:8000-ffff mask=0x8000\n"
    "  memory type=RAM content=Save\n"
    "    map address=70-7d,f0-ff:0000-7fff mask=0x8000\n"
    "\n"
    "board: SHVC-2A5M-01\n"
    "  memory type=ROM content=Program\n"
    "    map address=00-7d,80-ff:8000-ffff mask=0x8000\n"
    "  memory type=RAM content=Save\n"
    "    map address=70-7d,f0-ff:0000-7fff mask=0x8000\n"
    "\n"
    "board: SHVC-2B3B-01\n"
    "  memory type=ROM content=Program\n"
    "    map address=00-3f,80-bf:8000-ffff mask=0x8000\n"
    "  memory type=RAM content=Save\n"
    "    map address=70-7d,f0-ff:0000-7fff mask=0x8000\n"
    "  processor architecture=uPD7725\n"
    "    map address=60-6f,e0-ef:0000-7fff mask=0x3fff\n"
    "    memory type=ROM content=Program architecture=uPD7725\n"
    "    memory type=ROM content=Data architecture=uPD7725\n"
    "    memory type=RAM content=Data architecture=uPD7725\n"
    "    oscillator\n"
    "\n"
    "board: SHVC-2DC0N-01\n"
    "  processor architecture=HG51BS169\n"
    "    map address=00-3f,80-bf:6c00-6fff,7c00-7fff\n"
    "    memory type=ROM content=Program\n"
    "      map address=00-3f,80-bf:8000-ffff mask=0x8000\n"
    "    memory type=RAM content=Save\n"
    "      map address=70-77:0000-7fff\n"
    "    memory type=ROM content=Data architecture=HG51BS169\n"
    "    memory type=RAM content=Data architecture=HG51BS169\n"
    "      map address=00-3f,80-bf:6000-6bff,7000-7bff mask=0xf000\n"
    "    oscillator\n"
    "\n"
    "board: SHVC-2E3M-01\n"
    "  memory type=ROM content=Program\n"
    "    map address=00-3f,80-bf:8000-ffff mask=0x8000\n"
    "  processor identifier=OBC1\n"
    "    map address=00-3f,80-bf:6000-7fff mask=0xe000\n"
    "    map address=70-71,f0-f1:6000-7fff,e000-ffff mask=0xe000\n"
    "    memory type=RAM content=Save\n"
    "\n"
    "board: SHVC-2J0N-(01,10,11,20)\n"
    "  memory type=ROM content=Program\n"
    "    map address=00-3f,80-bf:8000-ffff\n"
    "    map address=40-7d,c0-ff:0000-ffff\n"
    "\n"
    "board: SHVC-2J3M-(01,11,20)\n"
    "  memory type=ROM content=Program\n"
    "    map address=00-3f,80-bf:8000-ffff\n"
    "    map address=40-7d,c0-ff:0000-ffff\n"
    "  memory type=RAM content=Save\n"
    "    map address=10-1f,30-3f,90-9f,b0-bf:6000-7fff mask=0xe000\n"
    "\n"
    "board: SHVC-2J5M-01\n"
    "  memory type=ROM content=Program\n"
    "    map address=00-3f,80-bf:8000-ffff\n"
    "    map address=40-7d,c0-ff:0000-ffff\n"
    "  memory type=RAM content=Save\n"
    "    map address=10-1f,30-3f,90-9f,b0-bf:6000-7fff mask=0xe000\n"
    "\n"
    "board: SHVC-3J0N-01\n"
    "  memory type=ROM content=Program\n"
    "    map address=00-2f,80-af:8000-ffff\n"
    "    map address=40-6f,c0-ef:0000-ffff\n"
    "\n"
    "board: SHVC-BA0N-(01,10)\n"
    "  memory type=ROM content=Program\n"
    "    map address=00-7d,80-ff:8000-ffff mask=0x8000\n"
    "    map address=40-7d,c0-ff:0000-7fff mask=0x8000\n"
    "\n"
    "board: SHVC-BA1M-01\n"
    "  memory type=ROM content=Program\n"
    "    map address=00-7d,80-ff:8000-ffff mask=0x8000\n"
    "  memory type=RAM content=Save\n"
    "    map address=70-7d,f0-ff:0000-7fff mask=0x8000\n"
    "\n"
    "board: SHVC-BA3M-(01,10)\n"
    "  memory type=ROM content=Program\n"
    "    map address=00-7d,80-ff:8000-ffff mask=0x8000\n"
    "  memory type=RAM content=Save\n"
    "    map address=70-7d,f0-ff:0000-7fff mask=0x8000\n"
    "\n"
    "board: SHVC-BJ0N-(01,20)\n"
    "  memory type=ROM content=Program\n"
    "    map address=00-3f,80-bf:8000-ffff\n"
    "    map address=40-7d,c0-ff:0000-ffff\n"
    "\n"
    "board: SHVC-BJ1M-(10,20)\n"
    "  memory type=ROM content=Program\n"
    "    map address=00-3f,80-bf:8000-ffff\n"
    "    map address=40-7d,c0-ff:0000-ffff\n"
    "  memory type=RAM content=Save\n"
    "    map address=20-3f,a0-bf:6000-7fff mask=0xe000\n"
    "\n"
    "board: SHVC-BJ3M-(10,20)\n"
    "  memory type=ROM content=Program\n"
    "    map address=00-3f,80-bf:8000-ffff\n"
    "    map address=40-7d,c0-ff:0000-ffff\n"
    "  memory type=RAM content=Save\n"
    "    map address=20-3f,a0-bf:6000-7fff mask=0xe000\n"
    "\n"
    "board: SHVC-LDH3C-01\n"
    "  processor identifier=SPC7110\n"
    "    map address=00-3f,80-bf:4800-483f\n"
    "    map address=50,58:0000-ffff\n"
    "    mcu\n"
    "      map address=00-3f,80-bf:8000-ffff mask=0x800000\n"
    "      map address=c0-ff:0000-ffff mask=0xc00000\n"
    "      memory type=ROM content=Program\n"
    "      memory type=ROM content=Data\n"
    "    memory type=RAM content=Save\n"
    "      map address=00-3f,80-bf:6000-7fff mask=0xe000\n"
    "  rtc manufacturer=Epson\n"
    "    map address=00-3f,80-bf:4840-4842\n"
    "    memory type=RTC content=Time manufacturer=Epson\n"
    "\n"
    "board: SHVC-LN3B-01\n"
    "  memory type=RAM content=Save\n"
    "    map address=00-3f,80-bf:6000-7fff mask=0xe000\n"
    "    map address=70-73:0000-ffff\n"
    "  processor identifier=SDD1\n"
    "    map address=00-3f,80-bf:4800-480f\n"
    "    mcu\n"
    "      map address=00-3f,80-bf:8000-ffff\n"
    "      map address=c0-ff:0000-ffff\n"
    "      memory type=ROM content=Program\n"
    "\n"
    "board: SHVC-SGB2-01\n"
    "  memory type=ROM content=Program\n"
    "    map address=00-7d,80-ff:8000-ffff mask=0x8000\n"
    "    map address=40-7d,c0-ff:0000-7fff mask=0x8000\n"
    "  processor identifier=ICD revision=2\n"
    "    map address=00-3f,80-bf:6000-67ff,7000-7fff\n"
    "    memory type=ROM content=Boot architecture=LR35902\n"
    "    oscillator\n"
    "    slot type=GameBoy\n"
    "\n"
    "board: SHVC-YA0N-01\n"
    "  memory type=ROM content=Program\n"
    "    map address=00-7d,80-ff:8000-ffff mask=0x8000\n"
    "    map address=40-7d,c0-ff:0000-7fff mask=0x8000\n"
    "\n"
    "board: SHVC-YJ0N-01\n"
    "  memory type=ROM content=Program\n"
    "    map address=00-3f,80-bf:8000-ffff\n"
    "    map address=40-7d,c0-ff:0000-ffff\n"
    "\n"
    "//Boards (Generic)\n"
    "\n"
    "database\n"
    "  revision: 2018-06-01\n"
    "\n"
    "board: ARM-LOROM-RAM\n"
    "  memory type=ROM content=Program\n"
    "    map address=00-7d,80-ff:8000-ffff mask=0x8000\n"
    "    map address=40-6f,c0-ef:0000-7fff mask=0x8000\n"
    "  memory type=RAM content=Save\n"
    "    map address=70-7d,f0-ff:0000-ffff\n"
    "  processor architecture=ARM6\n"
    "    map address=00-3f,80-bf:3800-38ff\n"
    "    memory type=ROM content=Program architecture=ARM6\n"
    "    memory type=ROM content=Data architecture=ARM6\n"
    "    memory type=RAM content=Data architecture=ARM6\n"
    "    oscillator\n"
    "\n"
    "board: BS-HIROM-RAM\n"
    "  memory type=ROM content=Program\n"
    "    map address=00-1f,80-9f:8000-ffff\n"
    "    map address=40-5f,c0-df:0000-ffff\n"
    "  memory type=RAM content=Save\n"
    "    map address=20-3f,a0-bf:6000-7fff mask=0xe000\n"
    "  slot type=BSMemory\n"
    "    map address=20-3f,a0-bf:8000-ffff\n"
    "    map address=60-7d,e0-ff:0000-ffff\n"
    "\n"
    "board: BS-LOROM-RAM\n"
    "  memory type=ROM content=Program\n"
    "    map address=00-1f:8000-ffff base=0x000000 mask=0x8000\n"
    "    map address=20-3f:8000-ffff base=0x100000 mask=0x8000\n"
    "    map address=80-9f:8000-ffff base=0x200000 mask=0x8000\n"
    "    map address=a0-bf:8000-ffff base=0x100000 mask=0x8000\n"
    "  memory type=RAM content=Save\n"
    "    map address=70-7d,f0-ff:0000-7fff mask=0x8000\n"
    "  slot type=BSMemory\n"
    "    map address=c0-ef:0000-ffff\n"
    "\n"
    "board: BS-MCC-RAM\n"
    "  memory type=RAM content=Save\n"
    "    map address=10-17:5000-5fff mask=0xf000\n"
    "  processor identifier=MCC\n"
    "    map address=00-0f:5000-5fff\n"
    "    mcu\n"
    "      map address=00-3f,80-bf:8000-ffff\n"
    "      map address=40-7d,c0-ff:0000-ffff\n"
    "      map address=20-3f,a0-bf:6000-7fff\n"
    "      memory type=ROM content=Program\n"
    "      memory type=RAM content=Download\n"
    "      slot type=BSMemory\n"
    "\n"
    "board: BS-SA1-RAM\n"
    "  processor architecture=W65C816S\n"
    "    map address=00-3f,80-bf:2200-23ff\n"
    "    mcu\n"
    "      map address=00-3f,80-bf:8000-ffff mask=0x408000\n"
    "      map address=c0-ff:0000-ffff\n"
    "      memory type=ROM content=Program\n"
    "      slot type=BSMemory\n"
    "    memory type=RAM content=Save\n"
    "      map address=00-3f,80-bf:6000-7fff size=0x2000\n"
    "      map address=40-4f:0000-ffff\n"
    "    memory type=RAM content=Internal\n"
    "      map address=00-3f,80-bf:3000-37ff size=0x800\n"
    "\n"
    "board: EVENT-CC92\n"
    "  memory type=RAM content=Save\n"
    "    map address=70-7d,f0-ff:0000-7fff mask=0x8000\n"
    "  processor manufacturer=NEC architecture=uPD78214\n"
    "    identifier: Campus Challenge '92\n"
    "    map address=c0,e0:0000\n"
    "    mcu\n"
    "      map address=00-1f,80-9f:8000-ffff\n"
    "      memory type=ROM content=Program\n"
    "      memory type=ROM content=Level-1\n"
    "      memory type=ROM content=Level-2\n"
    "      memory type=ROM content=Level-3\n"
    "    dip\n"
    "  processor manufacturer=NEC architecture=uPD7725\n"
    "    map address=20-3f,a0-bf:8000-ffff mask=0x7fff\n"
    "    memory type=ROM content=Program architecture=uPD7725\n"
    "    memory type=ROM content=Data architecture=uPD7725\n"
    "    memory type=RAM content=Data architecture=uPD7725\n"
    "    oscillator\n"
    "\n"
    "board: EVENT-PF94\n"
    "  memory type=RAM content=Save\n"
    "    map address=30-3f,b0-bf:6000-7fff mask=0xe000\n"
    "  processor manufacturer=NEC architecture=uPD78214\n"
    "    identifier: PowerFest '94\n"
    "    map address=10,20:6000\n"
    "    mcu\n"
    "      map address=00-3f,80-bf:8000-ffff\n"
    "      map address=c0-ff:0000-ffff\n"
    "      memory type=ROM content=Program\n"
    "      memory type=ROM content=Level-1\n"
    "      memory type=ROM content=Level-2\n"
    "      memory type=ROM content=Level-3\n"
    "    dip\n"
    "  processor manufacturer=NEC architecture=uPD7725\n"
    "    map address=00-0f,80-8f:6000-7fff mask=0xfff\n"
    "    memory type=ROM content=Program architecture=uPD7725\n"
    "    memory type=ROM content=Data architecture=uPD7725\n"
    "    memory type=RAM content=Data architecture=uPD7725\n"
    "    oscillator\n"
    "\n"
    "board: EXHIROM-RAM\n"
    "  memory type=ROM content=Program\n"
    "    map address=00-3f:8000-ffff base=0x400000\n"
    "    map address=40-7d:0000-ffff base=0x400000\n"
    "    map address=80-bf:8000-ffff mask=0xc00000\n"
    "    map address=c0-ff:0000-ffff mask=0xc00000\n"
    "  memory type=RAM content=Save\n"
    "    map address=20-3f,a0-bf:6000-7fff mask=0xe000\n"
    "    map address=70-7d:0000-7fff\n"
    "\n"
    "board: EXHIROM-RAM-SHARPRTC\n"
    "  memory type=ROM content=Program\n"
    "    map address=00-3f:8000-ffff base=0x400000\n"
    "    map address=40-7d:0000-ffff base=0x400000\n"
    "    map address=80-bf:8000-ffff mask=0xc00000\n"
    "    map address=c0-ff:0000-ffff mask=0xc00000\n"
    "  memory type=RAM content=Save\n"
    "    map address=20-3f,a0-bf:6000-7fff mask=0xe000\n"
    "    map address=70-7d:0000-7fff\n"
    "  rtc manufacturer=Sharp\n"
    "    map address=00-3f,80-bf:2800-2801\n"
    "    memory type=RTC content=Time manufacturer=Sharp\n"
    "\n"
    "board: EXNEC-LOROM\n"
    "  memory type=ROM content=Program\n"
    "    map address=00-7d,80-ff:8000-ffff mask=0x8000\n"
    "  processor architecture=uPD96050\n"
    "    map address=60-67,e0-e7:0000-3fff\n"
    "    memory type=ROM content=Program architecture=uPD96050\n"
    "    memory type=ROM content=Data architecture=uPD96050\n"
    "    memory type=RAM content=Data architecture=uPD96050\n"
    "      map address=68-6f,e8-ef:0000-7fff mask=0x8000\n"
    "    oscillator\n"
    "\n"
    "board: GB-LOROM\n"
    "  memory type=ROM content=Program\n"
    "    map address=00-7d,80-ff:8000-ffff mask=0x8000\n"
    "    map address=40-7d,c0-ff:0000-7fff mask=0x8000\n"
    "  processor identifier=ICD revision=2\n"
    "    map address=00-3f,80-bf:6000-67ff,7000-7fff\n"
    "    memory type=ROM content=Boot architecture=LR35902\n"
    "    oscillator\n"
    "    slot type=GameBoy\n"
    "\n"
    "board: GSU-RAM\n"
    "  processor architecture=GSU\n"
    "    map address=00-3f,80-bf:3000-34ff\n"
    "    memory type=ROM content=Program\n"
    "      map address=00-3f,80-bf:8000-ffff mask=0x8000\n"
    "      map address=40-5f,c0-df:0000-ffff\n"
    "    memory type=RAM content=Save\n"
    "      map address=00-3f,80-bf:6000-7fff size=0x2000\n"
    "      map address=70-71,f0-f1:0000-ffff\n"
    "\n"
    "board: HIROM\n"
    "  memory type=ROM content=Program\n"
    "    map address=00-3f,80-bf:8000-ffff\n"
    "    map address=40-7d,c0-ff:0000-ffff\n"
    "\n"
    "board: HIROM-RAM\n"
    "  memory type=ROM content=Program\n"
    "    map address=00-3f,80-bf:8000-ffff\n"
    "    map address=40-7d,c0-ff:0000-ffff\n"
    "  memory type=RAM content=Save\n"
    "    map address=20-3f,a0-bf:6000-7fff mask=0xe000\n"
    "\n"
    "board: HITACHI-LOROM\n"
    "  processor architecture=HG51BS169\n"
    "    map address=00-3f,80-bf:6c00-6fff,7c00-7fff\n"
    "    memory type=ROM content=Program\n"
    "      map address=00-3f,80-bf:8000-ffff mask=0x8000\n"
    "    memory type=RAM content=Save\n"
    "      map address=70-77:0000-7fff\n"
    "    memory type=ROM content=Data architecture=HG51BS169\n"
    "    memory type=RAM content=Data architecture=HG51BS169\n"
    "      map address=00-3f,80-bf:6000-6bff,7000-7bff mask=0xf000\n"
    "    oscillator\n"
    "\n"
    "board: LOROM\n"
    "  memory type=ROM content=Program\n"
    "    map address=00-7d,80-ff:8000-ffff mask=0x8000\n"
    "\n"
    "board: LOROM-RAM#A\n"
    "  memory type=ROM content=Program\n"
    "    map address=00-3f,80-bf:8000-ffff mask=0x8000\n"
    "  memory type=RAM content=Save\n"
    "    map address=70-7d,f0-ff:0000-ffff mask=0x8000\n"
    "\n"
    "board: LOROM-RAM#B\n"
    "  memory type=ROM content=Program\n"
    "    map address=00-7d,80-ff:8000-ffff mask=0x8000\n"
    "  memory type=RAM content=Save\n"
    "    map address=70-7d,f0-ff:0000-7fff mask=0x8000\n"
    "\n"
    "board: NEC-HIROM\n"
    "  memory type=ROM content=Program\n"
    "    map address=00-3f,80-bf:8000-ffff\n"
    "    map address=40-7d,c0-ff:0000-ffff\n"
    "  processor architecture=uPD7725\n"
    "    map address=00-1f,80-9f:6000-7fff mask=0xfff\n"
    "    memory type=ROM content=Program architecture=uPD7725\n"
    "    memory type=ROM content=Data architecture=uPD7725\n"
    "    memory type=RAM content=Data architecture=uPD7725\n"
    "    oscillator\n"
    "\n"
    "board: NEC-HIROM-RAM\n"
    "  memory type=ROM content=Program\n"
    "    map address=00-3f,80-bf:8000-ffff\n"
    "    map address=40-7d,c0-ff:0000-ffff\n"
    "  memory type=RAM content=Save\n"
    "    map address=20-3f,a0-bf:6000-7fff mask=0xe000\n"
    "  processor architecture=uPD7725\n"
    "    map address=00-1f,80-9f:6000-7fff mask=0xfff\n"
    "    memory type=ROM content=Program architecture=uPD7725\n"
    "    memory type=ROM content=Data architecture=uPD7725\n"
    "    memory type=RAM content=Data architecture=uPD7725\n"
    "    oscillator\n"
    "\n"
    "board: NEC-LOROM\n"
    "  memory type=ROM content=Program\n"
    "    map address=00-1f,80-9f:8000-ffff mask=0x8000\n"
    "  processor architecture=uPD7725\n"
    "    map address=30-3f,b0-bf:8000-ffff mask=0x3fff\n"
    "    memory type=ROM content=Program architecture=uPD7725\n"
    "    memory type=ROM content=Data architecture=uPD7725\n"
    "    memory type=RAM content=Data architecture=uPD7725\n"
    "    oscillator\n"
    "\n"
    "board: NEC-LOROM-RAM#A\n"
    "  memory type=ROM content=Program\n"
    "    map address=00-1f,80-9f:8000-ffff mask=0x8000\n"
    "  memory type=RAM content=Save\n"
    "    map address=70-7d,f0-ff:0000-ffff\n"
    "  processor architecture=uPD7725\n"
    "    map address=20-3f,a0-bf:8000-ffff mask=0x3fff\n"
    "    memory type=ROM content=Program architecture=uPD7725\n"
    "    memory type=ROM content=Data architecture=uPD7725\n"
    "    memory type=RAM content=Data architecture=uPD7725\n"
    "    oscillator\n"
    "\n"
    "board: NEC-LOROM-RAM#B\n"
    "  memory type=ROM content=Program\n"
    "    map address=00-3f,80-bf:8000-ffff mask=0x8000\n"
    "  memory type=RAM content=Save\n"
    "    map address=70-7d,f0-ff:0000-7fff mask=0x8000\n"
    "  processor architecture=uPD7725\n"
    "    map address=60-6f,e0-ef:0000-7fff mask=0x3fff\n"
    "    memory type=ROM content=Program architecture=uPD7725\n"
    "    memory type=ROM content=Data architecture=uPD7725\n"
    "    memory type=RAM content=Data architecture=uPD7725\n"
    "    oscillator\n"
    "\n"
    "board: OBC1-LOROM-RAM\n"
    "  memory type=ROM content=Program\n"
    "    map address=00-3f,80-bf:8000-ffff mask=0x8000\n"
    "  processor identifier=OBC1\n"
    "    map address=00-3f,80-bf:6000-7fff mask=0xe000\n"
    "    map address=70-71,f0-f1:6000-7fff,e000-ffff mask=0xe000\n"
    "    memory type=RAM content=Save\n"
    "\n"
    "board: SA1-RAM\n"
    "  processor architecture=W65C816S\n"
    "    map address=00-3f,80-bf:2200-23ff\n"
    "    mcu\n"
    "      map address=00-3f,80-bf:8000-ffff mask=0x408000\n"
    "      map address=c0-ff:0000-ffff\n"
    "      memory type=ROM content=Program\n"
    "    memory type=RAM content=Save\n"
    "      map address=00-3f,80-bf:6000-7fff size=0x2000\n"
    "      map address=40-4f:0000-ffff\n"
    "    memory type=RAM content=Internal\n"
    "      map address=00-3f,80-bf:3000-37ff size=0x800\n"
    "\n"
    "board: SDD1\n"
    "  processor identifier=SDD1\n"
    "    map address=00-3f,80-bf:4800-480f\n"
    "    mcu\n"
    "      map address=00-3f,80-bf:8000-ffff\n"
    "      map address=c0-ff:0000-ffff\n"
    "      memory type=ROM content=Program\n"
    "\n"
    "board: SDD1-RAM\n"
    "  memory type=RAM content=Save\n"
    "    map address=00-3f,80-bf:6000-7fff mask=0xe000\n"
    "    map address=70-73:0000-ffff mask=0x8000\n"
    "  processor identifier=SDD1\n"
    "    map address=00-3f,80-bf:4800-480f\n"
    "    mcu\n"
    "      map address=00-3f,80-bf:8000-ffff\n"
    "      map address=c0-ff:0000-ffff\n"
    "      memory type=ROM content=Program\n"
    "\n"
    "board: SPC7110-RAM\n"
    "  processor identifier=SPC7110\n"
    "    map address=00-3f,80-bf:4800-483f\n"
    "    map address=50,58:0000-ffff\n"
    "    mcu\n"
    "      map address=00-3f,80-bf:8000-ffff mask=0x800000\n"
    "      map address=c0-ff:0000-ffff mask=0xc00000\n"
    "      memory type=ROM content=Program\n"
    "      memory type=ROM content=Data\n"
    "    memory type=RAM content=Save\n"
    "      map address=00-3f,80-bf:6000-7fff mask=0xe000\n"
    "\n"
    "board: SPC7110-RAM-EPSONRTC\n"
    "  processor identifier=SPC7110\n"
    "    map address=00-3f,80-bf:4800-483f\n"
    "    map address=50,58:0000-ffff\n"
    "    mcu\n"
    "      map address=00-3f,80-bf:8000-ffff mask=0x800000\n"
    "      map address=c0-ff:0000-ffff mask=0xc00000\n"
    "      memory type=ROM content=Program\n"
    "      memory type=ROM content=Data\n"
    "    memory type=RAM content=Save\n"
    "      map address=00-3f,80-bf:6000-7fff mask=0xe000\n"
    "  rtc manufacturer=Epson\n"
    "    map address=00-3f,80-bf:4840-4842\n"
    "    memory type=RTC content=Time manufacturer=Epson\n"
    "\n"
    "board: ST-LOROM\n"
    "  memory type=ROM content=Program\n"
    "    map address=00-1f,80-9f:8000-ffff mask=0x8000\n"
    "  slot type=SufamiTurbo\n"
    "    rom\n"
    "      map address=20-3f,a0-bf:8000-ffff mask=0x8000\n"
    "    ram\n"
    "      map address=60-6f,e0-ef:0000-ffff\n"
    "  slot type=SufamiTurbo\n"
    "    rom\n"
    "      map address=40-5f,c0-df:0000-ffff mask=0x8000\n"
    "    ram\n"
    "      map address=70-7d,f0-ff:0000-ffff\n"
    "\n";
#endif