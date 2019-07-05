#ifndef MOCKUP_H
#define MOCKUP_H

#define BASE_GUI
#include "base/base.h"

#include "mockup/smw.h"
#include "nuts/nuts.h"

typedef struct {
    String name;
    Buffer buffer;
} NamedBuffer;

define_hashmap(NamedBufferMap, named_buffer_map, NamedBuffer);

typedef struct {
    GuiContext context;
    SMW smw;

    NutsAST ast;
    NutsAssembler assembler;
    NutsParser parser;
    Wdc65816Mapper rom;
    NutsErrorList error_list;

    struct {
        GuiImageData palette[512];
        GuiImageData map8[512];
        GuiImageData sprite_map8[512];
        GuiImageData map16_fg[512];
        GuiImageData map16_bg[512];
        GuiImageData level[512];
    } assets;

    NamedBufferMap buffer_map;
    Buffer rom_buffer;

    Arena arena;
    Arena temp_arena;
    FreeList free_list;
} Mockup;

void do_assemble(Mockup* mockup, Arena* temp_arena);

#endif //MOCKUP_H