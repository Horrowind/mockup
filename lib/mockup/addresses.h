#ifndef MOCKUP_ADDRESSES_HPP
#define MOCKUP_ADDRESSES_HPP


// Table of the  low bytes of the locations of GFX00 - GFX31
const int originalGraphicsFilesLowByteTableLocation = 0x00B992;
const int originalGraphicsFilesLowByteTableLocationPC = 0x003992;

// Table of the high bytes of the locations of GFX00 - GFX31
const int originalGraphicsFilesHighByteTableLocation = 0x00B9C4;
const int originalGraphicsFilesHighByteTableLocationPC = 0x0039C4;

// Table of the bank bytes of the locations of GFX00 - GFX31
const int originalGraphicsFilesBankByteTableLocation = 0x00B9F6;
const int originalGraphicsFilesBankByteTableLocationPC = 0x0039F6;

// List of the FG/BG slots levels can use (4 bytes each, 26 slots)
const int backgroundSlotListTableLocation = 0x00A92B;



#endif //MOCKUP_ADDRESSES_HPP
