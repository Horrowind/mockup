#include "assert.h"
#include "stdio.h"

#include "rom.h"

u8 r65816_guess_header(r65816_rom_t* rom);
u8 r65816_score_header(r65816_rom_t* rom, u32 address);

static
void r65816_rom_parse_bml_map(bml_node_t* map_node, r65816_mapper_t* mapper, r65816_mapper_entry_t* entry) {
    bml_node_t* node = map_node->child;
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

            string_t banks = { .data = node->value.data, .length = delim - node->value.data };
            string_t addrs = { .data = delim + 1,        .length = node->value.length - banks.length - 1};

            string_split_foreach(banks, bank, ',') {
                char* delim = bank.data;
                while(*delim != '-') {
                    delim++;
                    if(delim - bank.data >= bank.length) {
                        break;
                    }
                }

                string_t bank_low_str  = { .data = bank.data, .length = delim - bank.data };
                string_t bank_high_str = { .data = delim + 1, .length = bank.length - bank_low_str.length + 1 };
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

                    string_t addr_low_str  = { .data = addr.data, .length = delim - addr.data };
                    string_t addr_high_str = { .data = delim + 1, .length = addr.length - addr_low_str.length + 1 };
                    entry->addr_low  = (u16)string_to_u64_base(addr_low_str, 16);
                    entry->addr_high = (u16)string_to_u64_base(addr_high_str, 16);
                    entry->addr_high = entry->addr_high ? entry->addr_high : entry->addr_low;
                    r65816_mapper_add(mapper, entry);
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
void r65816_rom_parse_memory(bml_node_t* node, r65816_mapper_t* mapper, vfs_t* vfs) {
    r65816_mapper_entry_t entry = { 0 };
    bml_node_t* node2 = node->child;
    while(node2) {
        if(string_equal(node2->name, L("name"))) {
            entry.name = node2->value;
            vfs_entry_t dummy = { .name = entry.name };
            vfs_entry_t* found = vfs_find(vfs, dummy);
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
        // So only call r65816_map, after completely parsing the node.
        if(string_equal(node2->name, L("map"))) {
            r65816_rom_parse_bml_map(node2, mapper, &entry);
        }
        node2 = node2->next;
    }
}

void r65816_rom_init(r65816_rom_t* rom, vfs_t* vfs) {
    vfs_entry_t dummy = {.name = L("manifest.bml")};
    vfs_entry_t* manifest_entry = vfs_find(vfs, dummy);
    buffer_t manifest = manifest_entry->buffer;
    *rom = (r65816_rom_t) { 0 };
    pool_t manifest_pool;
    pool_init(&manifest_pool);
    pool_alloc(&manifest_pool, 4096);
    pool_empty(&manifest_pool);
    bml_node_t* node = bml_parse(manifest, &manifest_pool);
    //bml_print_node(node, 0);
    while(node) {
        if(string_equal(node->name, L("board"))) {
            bml_node_t* node2 = node->child;
            while(node2) {
                if(string_equal(node2->name, L("rom"))) {
                    r65816_rom_parse_memory(node2, &rom->read_mapper, vfs);
                }
                if(string_equal(node2->name, L("ram"))) {
                    r65816_rom_parse_memory(node2, &rom->read_mapper, vfs);
                    r65816_rom_parse_memory(node2, &rom->write_mapper, vfs);
                }
                node2 = node2->next;
            }
        }
        
        node = node->next;
    }

    r65816_mapper_function_triple_t triple =
        r65816_mapper_create_function_triple(&rom->read_mapper);
    rom->read  = triple.read;
    rom->write = triple.write;
    rom->ptr   = triple.ptr;

    // HACK: Remove after converting everything to rom->read()
    rom->banks = malloc(0x1F * sizeof(r65816_bank));
    rom->data = rom->ptr(0x008000);
    for(int i = 0; i < 0x1F; i++) {
        rom->banks[i] = rom->ptr((i << 16) | 0x008000) - 0x8000;
    }
}


/* void r65816_rom_init(r65816_rom_t* rom, u8* data, uint length) {*/
/*     rom->banksize = 0x8000; *///Currently only LOROM is supported
/*     unsigned int headersize = length % rom->banksize; */
/*     rom->num_banks = (length - headersize) / rom->banksize; */
/*     rom->banks = malloc(rom->num_banks * sizeof(r65816_bank)); */
/*     rom->data = data; */
/*     r65816_guess_header(rom); */
/*     for(int i = 0; i < rom->num_banks; i++) { */
/*         rom->banks[i] = rom->data + i * rom->banksize - 0x8000; */
/*     } */

/* } */


/* void r65816_rom_load(r65816_rom_t* rom, char* path) { */
/*     FILE* fp = fopen(path, "r"); */
/*     assert(fp != 0); */
/*     fseek(fp, 0, SEEK_END); */
/*     rom->banksize = 0x8000; //Currently only LOROM is supported */
/*     unsigned int filesize = ftell(fp); */
/*     assert(filesize != 0); */
/*     unsigned int headersize = filesize % rom->banksize; */
/*     fseek(fp, headersize, SEEK_SET); */
/*     rom->num_banks = (filesize - headersize) / rom->banksize; */
/*     rom->banks = malloc(rom->num_banks * sizeof(r65816_bank)); */
/*     rom->data = malloc(rom->num_banks * rom->banksize); */
/*     fread((char*)rom->data, 1, rom->num_banks * rom->banksize, fp); */
/*     fclose(fp); */
/*     r65816_guess_header(rom); */
/*     for(int i = 0; i < rom->num_banks; i++) { */
/*         rom->banks[i] = rom->data + i * rom->banksize - 0x8000; */
/*     } */

/* } */

/* void r65816_rom_save(r65816_rom_t* rom, char* path) { */
/*     FILE* fp = fopen(path, "w"); */
/*     if(!fp) return; */
/*     fwrite(rom->data, 1, rom->num_banks * rom->banksize, fp); */
/*     fclose(fp); */
/* } */

/* void r65816_rom_save_headered(r65816_rom_t* rom, char* path, u8* header, int headersize) { */
/*     FILE* fp = fopen(path, "w"); */
/*     if(!fp) return; */
/*     fwrite(header, 1, headersize, fp); */
/*     fwrite(rom->data, 1, rom->num_banks * rom->banksize, fp); */
/*     fclose(fp); */
/* } */


void r65816_rom_free(r65816_rom_t* rom) {
    // TODO!!!
    /* free(rom->data); */
    /* free(rom->banks); */
}

/* u8 r65816_score_header(r65816_rom_t* rom, u32 address) { */
/*     if(rom->banksize * rom->num_banks < address + 64){ */
/*         return 0; */
/*     } */
/*     u8 score = 0; */
/*     r65816_rom_header_t* header = (r65816_rom_header_t*)(rom->data + address); */
/*     u16 reset_vector = header->interrupt_emulation[5]; */
/*     u16 checksum = header->checksum; */
/*     u16 complement = header->checksum_complement; */
/*     u8 resetop = rom->data[(address & ~0x7fff) | (reset_vector & 0x7fff)]; */
/*     u8 guessed_mapper = header->layout & ~0x10; */

/*     if(reset_vector < 0x8000){ */
/*         return 0; */
/*     } */

/*     if(resetop == 0x78 || resetop == 0x18 || resetop == 0x38 || */
/*        resetop == 0x9c || resetop == 0x4c || resetop == 0x5c){ */
/*         score += 8; */
/*     } */

/*     if(resetop == 0xc2 || resetop == 0xe2 || resetop == 0xad || resetop == 0xae || */
/*        resetop == 0xac || resetop == 0xaf || resetop == 0xa9 || resetop == 0xa2 || */
/*        resetop == 0xa0 || resetop == 0x20 || resetop == 0x22){ */
/*         score += 4; */
/*     } */
/*     if(resetop == 0x40 || resetop == 0x60 || resetop == 0x6b || */
/*        resetop == 0xcd || resetop == 0xec || resetop == 0xcc) */
/*         score -= 4; */

/*     if(resetop == 0x00 || resetop == 0x02  || resetop == 0xdb || */
/*        resetop == 0x42 || resetop == 0xff){ */
/*         score -= 8; */
/*     } */

/*     if((checksum + complement) == 0xffff && (checksum != 0) && (complement != 0)){ */
/*         score += 4; */
/*     } */

/*     if(address == 0x007fc0 && guessed_mapper == 0x20){ */
/*         score += 2; */
/*     }else if(address == 0x00ffc0 && guessed_mapper == 0x21){ */
/*         score += 2; */
/*     }else if(address == 0x007fc0 && guessed_mapper == 0x22){ */
/*         score += 2; */
/*     }else if(address == 0x40ffc0 && guessed_mapper == 0x25){ */
/*         score += 2; */
/*     } */
/*     if(header->licencee_code == 0x33){ */
/*         score += 2; */
/*     } */
/*     if(header->ram_size < 0x08){ */
/*         score++; */
/*     } */
/*     if(header->country_code < 14){ */
/*         score++; */
/*     } */
/*     if(header->cartridge_type < 0x08){ */
/*         score++; */
/*     }else if(header->rom_size < 0x10){ */
/*         score++; */
/*     } */

/*     if(score < 0){ */
/*         return 0; */
/*     } */

/*     return score; */
/* } */

/* u8 r65816_guess_header(r65816_rom_t* rom) { */
/*     u8 score_lo = r65816_score_header(rom, 0x007fc0); */
/*     u8 score_hi = r65816_score_header(rom, 0x00ffc0); */
/*     u8 score_ex = r65816_score_header(rom, 0x40ffc0); */
    
/*     if(score_lo >= score_hi && score_lo >= score_ex + 4){ */
/*         rom->header = (r65816_rom_header_t*)(rom->data + 0x007fc0); */
/*         return score_lo; */
/*     }else if(score_hi >= score_ex + 4){ */
/*         rom->header = (r65816_rom_header_t*)(rom->data + 0x00ffc0); */
/*         return score_hi; */
/*     }else{ */
/*         rom->header = (r65816_rom_header_t*)(rom->data + 0x40ffc0); */
/*         return score_ex; */
/*     } */
/* } */

/* static u32 snes_header_sizes[0x0D] = { */
/*     0x000000, //0 */
/*     0x000800, //1 */
/*     0x001000, //2 */
/*     0x002000, //3 */
/*     0x004000, //4 */
/*     0x008000, //5 */
/*     0x010000, //6 */
/*     0x020000, //7 */
/*     0x040000, //8 */
/*     0x080000, //9 */
/*     0x100000, //A */
/*     0x200000, //B */
/*     0x400000  //C */
/* }; */

/* // Uses the value in the SNES-Rom header (byte 0x17), see snes_header_sizes */
/* u8 r65816_rom_expand(r65816_rom_t* rom, u8 size) { */
/*     if(size > 0x0D) return -1; */
/*     u8* ptr = realloc(rom->data, snes_header_sizes[size]); */
/*     if(ptr == 0) return -1; */
/*     rom->data = ptr; */
/*     rom->header->rom_size = size; */
/*     rom->num_banks = snes_header_sizes[size] / rom->banksize; */
/*     for(int i = 0; i < rom->num_banks; i++) { */
/*         rom->banks[i] = rom->data + i * rom->banksize - 0x8000; */
/*     } */
/*     return 0; */
/* } */
