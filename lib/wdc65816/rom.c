#include "rom.h"

static
void wdc65816_rom_parse_bml_map(BMLNode* map_node, WDC65816Mapper* mapper, WDC65816MapperEntry* entry) {
    BMLNode* node = map_node->child;
    entry->low  = 0;
    entry->high = 0;
    entry->mask = 0;
    while(node) {
        if(string_equal(node->name, L("address"))) {
            char* delim = node->value.data;
            while(*delim != ':') {
                delim++;
                if(delim - node->value.data >= node->value.length) {
                    fprintf(stderr, "Error in manifest!\n");
                    exit(1);
                }
            }

            String banks = { .data = node->value.data, .length = delim - node->value.data };
            String addrs = { .data = delim + 1,        .length = node->value.length - banks.length - 1};

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
                entry->bank_low  = (u8)string_to_u64_base(bank_low_str, 16);
                entry->bank_high = (u8)string_to_u64_base(bank_high_str, 16);
                entry->bank_high = entry->bank_high ? entry->bank_high : entry->bank_low;

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
                    entry->addr_low  = (u16)string_to_u64_base(addr_low_str, 16);
                    entry->addr_high = (u16)string_to_u64_base(addr_high_str, 16);
                    entry->addr_high = entry->addr_high ? entry->addr_high : entry->addr_low;
                    wdc65816_mapper_add(mapper, entry);
                }
            }

            (void)addrs.data;
        }
        if(string_equal(node->name, L("mask"))) {
            entry->mask = string_to_u64(node->value) & 0xFFFFFF;
        }
        node = node->next;
    }
}

static
void wdc65816_rom_parse_memory(BMLNode* node, WDC65816Mapper* mapper, VFS* vfs) {
    WDC65816MapperEntry entry = { 0 };
    BMLNode* node2 = node->child;
    while(node2) {
        if(string_equal(node2->name, L("name"))) {
            entry.name = node2->value;
            VFSEntry dummy = { .name = entry.name };
            VFSEntry* found = vfs_find(vfs, dummy);
            if(found) {
                entry.data = found->buffer.begin;
                entry.data_length = found->buffer.end - found->buffer.begin;
            } else {
                fprintf(stderr, "Error: File \"%.*s\" not found.\n",
                        entry.name.length, entry.name.data);
            }
        }
        if(string_equal(node2->name, L("size"))) {
            entry.size = (u32)string_to_u64(node2->value);
        }
        if(string_equal(node2->name, L("base"))) {
            entry.base = (u32)string_to_u64(node2->value);
        }

        // Todo: map could be defined before size/base.
        // So only call wdc65816_map, after completely parsing the node.
        if(string_equal(node2->name, L("map"))) {
            wdc65816_rom_parse_bml_map(node2, mapper, &entry);
        }
        node2 = node2->next;
    }
}

void wdc65816_rom_init(WDC65816Rom* rom, VFS* vfs, Arena* arena) {
    VFSEntry dummy = {.name = L("manifest.bml")};
    VFSEntry* manifest_entry = vfs_find(vfs, dummy);
    Buffer manifest = manifest_entry->buffer;
    *rom = (WDC65816Rom) { 0 };
    BMLNode* node = bml_parse(manifest, arena);
    //bml_print_node(node, 0);
    while(node) {
        if(string_equal(node->name, L("board"))) {
            BMLNode* node2 = node->child;
            while(node2) {
                if(string_equal(node2->name, L("rom"))) {
                    wdc65816_rom_parse_memory(node2, &rom->read_mapper, vfs);
                }
                if(string_equal(node2->name, L("ram"))) {
                    wdc65816_rom_parse_memory(node2, &rom->read_mapper, vfs);
                    wdc65816_rom_parse_memory(node2, &rom->write_mapper, vfs);
                }
                node2 = node2->next;
            }
        }
        
        node = node->next;
    }

    wdc65816_mapper_init_functions(&rom->read_mapper);
    rom->read  = rom->read_mapper.read;
    rom->write = rom->read_mapper.write;
    rom->ptr   = rom->read_mapper.ptr;
}


void wdc65816_rom_free(WDC65816Rom* rom) {
    // TODO!!!
    /* free(rom->data); */
    /* free(rom->banks); */
}
