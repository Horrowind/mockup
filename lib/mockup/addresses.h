#ifndef MOCKUP_ADDRESSES_HPP
#define MOCKUP_ADDRESSES_HPP


#define sfc2pc(addr) ((addr&0x7F0000)>>1|(addr&0x7FFF))
#define sfc2ram(addr) (addr - 0x7E0000)

#define SIZE_OF_GFX_32 744
#define SIZE_OF_GFX_33 384

// Table of the  low bytes of the locations of GFX00 - GFX31
#define gfx0031_table_low_sfc 0x00B992
#define gfx0031_table_low_pc    sfc2pc(gfx0031_table_low_sfc)

// Table of the high bytes of the locations of GFX00 - GFX31
#define gfx0031_table_high_sfc  0x00B9C4
#define gfx0031_table_high_pc   sfc2pc(gfx0031_table_high_sfc)

// Table of the bank bytes of the locations of GFX00 - GFX31
#define gfx0031_table_bank_sfc  0x00B9F6
#define gfx0031_table_bank_pc   sfc2pc(gfx0031_table_bank_sfc)

// List of the FG/BG slots levels can use (4 bytes each, 26 slots)
#define level_bg_slot_list_table_sfc  0x00A92B

#define level_layer1_data_table_sfc  0x05E000
#define level_layer1_data_table_pc  sfc2pc(level_layer1_data_table_sfc)
#define level_layer2_data_table_sfc  0x05E600
#define level_layer2_data_table_pc  sfc2pc(level_layer2_data_table_sfc)
#define level_sprites_data_table_sfc  0x05EC00
#define level_sprites_data_table_pc  sfc2pc(level_sprites_data_table_sfc)

#define level_map16_low_ram  sfc2ram(0x7EC800)
#define level_map16_high_ram  sfc2ram(0x7FC800)

#define level_load_routine_sfc  0x05801E
#define level_load_routine_end_sfc  0x0583B8

#define level_load_tiles_routine_end_sfc   0x058091
#define level_load_tiles_routine_end2_sfc  0x0586E2
#define level_load_tiles_routine_new_object_sfc  0x0586C5

#define level_bg_area_color_data_table_sfc  0x00B0A0
#define level_bg_area_color_data_table_pc  sfc2pc(level_bg_area_color_data_table_sfc)

#define level_current_screen_ram  sfc2ram(0x7E1928)
#define level_header_bg_color_ram  sfc2ram(0x7E192F)
#define level_header_tileset_ram  sfc2ram(0x7E1931)

#define level_tiles_low_start_sfc  0x7EC800
#define level_tiles_low_end_sfc  0x7EFFFF
#define level_tiles_high_start_sfc  0x7FC800
#define level_tiles_high_end_sfc  0x7FFFFF

#define map16_fg_tiles_table_sfc  0x0D8000
#define map16_fg_tiles_zero_offset_sfc  0x0D0000 // Is actually used in code
#define map16_bg_tiles_table_sfc  0x0D9100

#undef snes2pc

#endif //MOCKUP_ADDRESSES_HPP
