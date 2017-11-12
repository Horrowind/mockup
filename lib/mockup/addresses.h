#ifndef MOCKUP_ADDRESSES_HPP
#define MOCKUP_ADDRESSES_HPP

#define sfc2ram(addr) (addr - 0x7E0000)

#define SIZE_OF_GFX_32 744
#define SIZE_OF_GFX_33 384

// Table of the  low bytes of the locations of GFX00 - GFX31
const u32 gfx0031_table_low_sfc = 0x00B992;

// Table of the high bytes of the locations of GFX00 - GFX31
const u32 gfx0031_table_high_sfc  = 0x00B9C4;

// Table of the bank bytes of the locations of GFX00 - GFX31
const u32 gfx0031_table_bank_sfc  = 0x00B9F6;

// List of the FG/BG slots levels can use (4 bytes each, 26 slots)
const u32 level_bg_slot_list_table_sfc  = 0x00A92B;

const u32 level_layer1_data_table_sfc  = 0x05E000;
const u32 level_layer2_data_table_sfc  = 0x05E600;
const u32 level_sprite_data_table_sfc  = 0x05EC00;

const u32 level_map16_low_ram = sfc2ram(0x7EC800);
const u32 level_map16_high_ram = sfc2ram(0x7FC800);

const u32 level_load_routine_sfc = 0x05801E;
const u32 level_load_routine_end_sfc = 0x0583B8;

const u32 level_load_tiles_routine_end_sfc =  0x058091;
const u32 level_load_tiles_routine_end2_sfc = 0x0586E2;
const u32 level_load_tiles_routine_new_object_sfc =  0x0586C5;

const u32 level_bg_area_color_data_table_sfc  = 0x00B0A0;

const u32 level_current_screen_ram  = sfc2ram(0x7E1928);
const u32 level_header_bg_color_ram = sfc2ram(0x7E192F);
const u32 level_header_tileset_ram = sfc2ram(0x7E1931);
const u32 level_header_sprite_tileset_ram = sfc2ram(0x7E192B);
const u32 level_tiles_low_start_sfc  = 0x7EC800;
const u32 level_tiles_low_end_sfc  = 0x7EFFFF;
const u32 level_tiles_high_start_sfc  = 0x7FC800;
const u32 level_tiles_high_end_sfc  = 0x7FFFFF;

const u32 map16_fg_tiles_table_sfc  = 0x0D8000;
const u32 map16_fg_tiles_zero_offset_sfc = 0x0D0000; // Is actually used in code
const u32 map16_bg_tiles_table_sfc  = 0x0D9100;

#endif //MOCKUP_ADDRESSES_HPP
