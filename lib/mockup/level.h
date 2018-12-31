#ifndef MOCKUP_LEVEL_H
#define MOCKUP_LEVEL_H

#define SPRITES

#include "wdc65816/wdc65816.h"
#include "gfx_store.h"
#include "layer.h"
#include "level.h"
#include "palette.h"
#include "tiles.h"
#include "tileset.h"

typedef struct {
    u16 x;
    u16 y;
    u16 tile;
    u16 is_top;
} ObjectTile;

typedef struct {
    u16 x;
    u16 y;
    u8 num;
    u8 settings;

    u16  bb_xmin;
    u16  bb_ymin;
    u16  bb_xmax;
    u16  bb_ymax;
    u16  zindex;
    union {
        u16* tiles;
        struct {
            ObjectTile* object_tiles;
            int         object_tiles_length;
        };
    };
} ObjectPC;

typedef struct {
    ObjectPC* objects;
    u16 length;
} ObjectListPC;


#ifdef SPRITES
typedef struct {
    u8 y1         : 4;
    u8 extra_bits : 2;
    u8 screen1    : 1;
    u8 y2         : 1;
    u8 x          : 4;
    u8 screen2    : 4;
    u8 number;
} Sprite;

typedef struct {
    int x;
    int y;
    u8 flip_x : 1;
    u8 flip_y : 1;
    
    u8 palette;
    u16 tile_num;
} SpriteTile;

typedef struct {
    u8 entry_009E;
    u8 entry_00AA;
    u8 entry_00B6;
    u8 entry_00C2;
    u8 entry_00D8;
    u8 entry_00E4;
    u8 entry_14C8;
    u8 entry_14D4;
    u8 entry_14E0;
    u8 entry_14EC;
    u8 entry_14F8;
    u8 entry_1504;
    u8 entry_1510;
    u8 entry_151C;
    u8 entry_1528;
    u8 entry_1534;
    u8 entry_1540;
    u8 entry_154C;
    u8 entry_1558;
    u8 entry_1564;
    u8 entry_1570;
    u8 entry_157C;
    u8 entry_1588;
    u8 entry_1594;
    u8 entry_15A0;
    u8 entry_15AC;
    u8 entry_15B8;
    u8 entry_15C4;
    u8 entry_15D0;
    u8 entry_15DC;
    u8 entry_15EA;
    u8 entry_15F6;
    u8 entry_1602;
    u8 entry_160E;
    u8 entry_161A;
    u8 entry_1626;
    u8 entry_1632;
    u8 entry_163E;
    u8 entry_164A;
    u8 entry_1656;
    u8 entry_1662;
    u8 entry_166E;
    u8 entry_167A;
    u8 entry_1686;
    u8 entry_186C;
    u8 entry_187B;
    u8 entry_190F;
    u8 entry_1FE2;
} SpriteTableEntries;

typedef struct {
    int x, y;
    u8 number;
    u8 extra_bits;
    int num_tiles;
    SpriteTile* tiles;
    SpriteTableEntries table_entries;
} SpritePC;

typedef struct {
    int length;
    SpritePC* data;   
} SpriteListPC;
#endif

typedef struct {
    u8 screens         : 5;
    u8 bg_palette      : 3;

    u8 level_mode      : 5;
    u8 bg_color        : 3;
    
    u8 sprite_set      : 4;
    u8 music           : 3;
    u8 layer3_priority : 1;

    u8 fg_palette      : 3;
    u8 sprite_palette  : 3;
    u8 time            : 2;
    
    u8 tile_set        : 4;
    u8 vert_scroll     : 2;
    u8 item_memory     : 2;
} LevelHeader;

typedef struct {
    LevelHeader  header;
    ObjectListPC layer1_objects;
#ifdef SPRITES
    SpriteListPC sprites;
    Tileset      sprite_tileset;
    Map8         sprite_map8;
#endif
    union {
        ObjectListPC layer2_objects;
        Layer16      layer2_background;
    };
    u8 has_layer2_bg      : 1;
    u8 has_layer2_objects : 1;
    u8 is_vertical_level  : 1;
    u8 is_boss_level      : 1;

    u32  background_color;
    
    Palette palette;
    Tileset tileset;
    Map8    map8;
    Map16PC map16_fg;
    Map16PC map16_bg;
    int     num_level;
    int     height;
    int     width;


    Wdc65816MapperBuilder* rom;

} LevelPC;


void level_init(LevelPC* l, Wdc65816MapperBuilder* rom, int num_level, GFXStore* gfx_store);
void level_init_by_address(LevelPC* l, Wdc65816MapperBuilder* rom, int num_level, GFXStore* gfx_store,
                           u32 level_layer1_data_addr_sfc,
                           u32 level_layer2_data_addr_sfc,
                           u32 level_sprite_data_addr_sfc);

void level_deinit(LevelPC* l);

int  level_ObjectListPCo_level_data(ObjectListPC* object_list, u8* output_data, int max_length);
void level_move_object(LevelPC* l, int index, u16 x, u16 y, GFXStore* gfx_store);

void level_animate(LevelPC* l, u8 frame, GFXStore* gfx_store);


#endif //MOCKUP_LEVEL
