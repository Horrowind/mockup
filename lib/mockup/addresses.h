#ifndef MOCKUP_ADDRESSES_HPP
#define MOCKUP_ADDRESSES_HPP


//const int 

#define SIZE_OF_GFX_32 744
#define SIZE_OF_GFX_33 384

// Table of the  low bytes of the locations of GFX00 - GFX31
const int gfx0031_table_low_sfc  = 0x00B992;
const int gfx0031_table_low_pc   = 0x003992;

// Table of the high bytes of the locations of GFX00 - GFX31
const int gfx0031_table_high_sfc = 0x00B9C4;
const int gfx0031_table_high_pc  = 0x0039C4;

// Table of the bank bytes of the locations of GFX00 - GFX31
const int gfx0031_table_bank_sfc = 0x00B9F6;
const int gfx0031_table_bank_pc  = 0x0039F6;

// List of the FG/BG slots levels can use (4 bytes each, 26 slots)
const int backgroundSlotListTableLocation = 0x00A92B;



#endif //MOCKUP_ADDRESSES_HPP
