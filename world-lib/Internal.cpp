
namespace worldlib
{
	namespace internal
	{

		// LM-related addresses:
		const int customPalettePointerTableLocation = 0x0EF600;			// Location of each level's custom palette, if it has one.
		const int standardExGFXPointerToPointerTableLocation = 0x0FF94F;	// Location of the pointer to the pointers to ExGFX files 80-FF
		const int superExGFXPointerToPointerTableLocation = 0x0FF873;		// Location of the pointer to the pointers to ExGFX files 100-FFF (stored in RATS-protected space)
		const int decompressionTypeLocation = 0x0FFFEB;				// Location of the byte that determines which compression method the ROM uses. 00 and 01 are LZ2, 02 is LZ3.

		// Original SMW-related addresses
		const int sharedBackgroundColorsLocation = 0x00B0A0;			// Location of the 8 background colors (2 bytes long each)

		const int sharedBackgroundSwapPalettesLocation = 0x00B0B0;		// For rows 0 and 1: Location of the 8 background palettes (192 bytes long of 6 colors per row per entry)
		const int sharedForegroundSwapPalettesLocation = 0x00B190;		// For rows 2 and 3: Location of the 8 foreground palettes (24 bytes long each, 12 for each row)
		const int sharedForegroundConstPalettesLocation = 0x00B250;		// For rows 4,5,6,7: Location of the remaining static palettes (12 bytes per row)

		const int sharedLayer3ConstPalettesLocation = 0x00B170;			// Location of the 4 shared layer 3 palettes (8 bytes each, two per row at the end of rows 0 and 1)

		const int sharedSpritePaletteLocation = 0x00B280;			// Location of the 6 shared sprite colors (12 bytes each)
		const int sharedMarioPaletteLocation = 0x00B2DC;			// Location of Mario's palette (20 bytes, colors 6-F of row 8)
		const int sharedSpriteSwapPalettesLocation = 0x00B318;			// For rows E and F: Location of the 8 sprite palettes (24 bytes long each, 12 for each row)

		const int sharedBerryPaletteLocation = 0x00B674;			// For rows 2, 3, 4 (and 9, A, B), colors 9 - F (14 bytes for each row, 3 rows)

		// LM-related addresses:
		const int exgfxBypassListPointerToPointerTable = 0x0FF873;		// Where to find the pointer to the pointers to the ExGFX files list (stored in RATS-protected space). Seems to be tons of other level-related stuff there too?
		const int exgfxBypassOffset = 0x2D00;					// How far into the table the actual ExGFX list is.  Not an address.

		// Original SMW-related addresses
		const int spriteSlotListTableLocation = 0x00A8C3;			// List of the sprite slots levels can use (4 bytes each, 26 slots)
		const int backgroundSlotListTableLocation = 0x00A92B;			// List of the FG/BG slots levels can use (4 bytes each, 26 slots)
		const int layer1PointerTableLocation = 0x05E000;			// Pointer to layer 1 data.  Used because the headers contain important information.
		
		const int originalGraphicsFilesLowByteTableLocation = 0x00B992;		// Table of the low bytes of the locations of GFX00 - GFX31
		const int originalGraphicsFilesHighByteTableLocation = 0x00B9C4;	// Table of the high bytes of the locations of GFX00 - GFX31
		const int originalGraphicsFilesBankByteTableLocation = 0x00B9F6;	// Table of the bank bytes of the locations of GFX00 - GFX31
		const int lowExgfxFilesTableLocation = 0x0FF94F;			// Location of the table of the locations of GFX80 - GFXFFF


		template <typename inputIteratorType> std::uint8_t getLevelHeaderByte(inputIteratorType romStart, inputIteratorType romEnd, int level, int byteNumber)
		{
			int levelAddress = readTrivigintetSFC(romStart, romEnd, level * 3 + layer1PointerTableLocation);

			return readByteSFC(romStart, romEnd, levelAddress + byteNumber);
		}


		template <typename integerType, typename maskType> integerType getBits(integerType integer, maskType mask)
		{
			integer &= mask;
			while ((mask & 1) == 0)
			{
				mask >>= 1;
				integer >>= 1;
			}

			return integer;
		}

		inline std::uint8_t reverseBits(std::uint8_t byte)
		{
			return static_cast<std::uint8_t>(((byte * 0x80200802ULL) & 0x0884422110ULL) * 0x0101010101ULL >> 32);
		}

		inline std::uint16_t reverseBits(std::uint16_t value)
		{
			return reverseBits((std::uint8_t)((value & 0xFF) >> 8)) | (reverseBits((std::uint8_t)(value & 0xFF)) << 8);
		}

		inline std::uint32_t reverseBits(std::uint32_t value)
		{
			return reverseBits((std::uint16_t)((value & 0xFF) >> 8)) | (reverseBits((std::uint16_t)(value & 0xFF)) << 8);
		}

		inline std::uint64_t reverseBits(std::uint64_t value)
		{
			return reverseBits((std::uint32_t)((value & 0xFF) >> 8)) | (reverseBits((std::uint32_t)(value & 0xFF)) << 8);
		}

		inline std::uint64_t explodeInt(std::uint32_t k)
		{
			std::uint64_t j = 0;
			std::uint64_t i = k;					// Needed since shifts >= 32 are undefined for 32-bit integers.
			j |= ((i & 0x00000001) << 0);
			j |= ((i & 0x00000002) << 1);
			j |= ((i & 0x00000004) << 2);
			j |= ((i & 0x00000008) << 3);
			j |= ((i & 0x00000010) << 4);
			j |= ((i & 0x00000020) << 5);
			j |= ((i & 0x00000040) << 6);
			j |= ((i & 0x00000080) << 7);
			j |= ((i & 0x00000100) << 8);
			j |= ((i & 0x00000200) << 9);
			j |= ((i & 0x00000400) << 10);
			j |= ((i & 0x00000800) << 11);
			j |= ((i & 0x00001000) << 12);
			j |= ((i & 0x00002000) << 13);
			j |= ((i & 0x00004000) << 14);
			j |= ((i & 0x00008000) << 15);
			j |= ((i & 0x00010000) << 16);
			j |= ((i & 0x00020000) << 17);
			j |= ((i & 0x00040000) << 18);
			j |= ((i & 0x00080000) << 19);
			j |= ((i & 0x00100000) << 20);
			j |= ((i & 0x00200000) << 21);
			j |= ((i & 0x00400000) << 22);
			j |= ((i & 0x00800000) << 23);
			j |= ((i & 0x01000000) << 24);
			j |= ((i & 0x02000000) << 25);
			j |= ((i & 0x04000000) << 26);
			j |= ((i & 0x08000000) << 27);
			j |= ((i & 0x10000000) << 28);
			j |= ((i & 0x20000000) << 29);
			j |= ((i & 0x40000000) << 30);
			j |= ((i & 0x80000000) << 31);
			return j;
		}

		inline std::uint32_t explodeInt(std::uint16_t i)
		{
			std::uint32_t j = 0;
			j |= ((i & 0x0001) << 0);
			j |= ((i & 0x0002) << 1);
			j |= ((i & 0x0004) << 2);
			j |= ((i & 0x0008) << 3);
			j |= ((i & 0x0010) << 4);
			j |= ((i & 0x0020) << 5);
			j |= ((i & 0x0040) << 6);
			j |= ((i & 0x0080) << 7);
			j |= ((i & 0x0100) << 8);
			j |= ((i & 0x0200) << 9);
			j |= ((i & 0x0400) << 10);
			j |= ((i & 0x0800) << 11);
			j |= ((i & 0x1000) << 12);
			j |= ((i & 0x2000) << 13);
			j |= ((i & 0x4000) << 14);
			j |= ((i & 0x8000) << 15);
			return j;
		}

		inline std::uint16_t explodeInt(std::uint8_t i)
		{
			std::uint16_t j = 0;
			j |= ((i & 0x01) << 0);
			j |= ((i & 0x02) << 1);
			j |= ((i & 0x04) << 2);
			j |= ((i & 0x08) << 3);
			j |= ((i & 0x10) << 4);
			j |= ((i & 0x20) << 5);
			j |= ((i & 0x40) << 6);
			j |= ((i & 0x80) << 7);
			return j;
		}


		inline std::uint64_t getBit(std::uint64_t number, int bit)
		{
			const static std::uint64_t getBitBitTable[64] = {
				0x0000000000000001, 0x0000000000000002, 0x0000000000000004, 0x0000000000000008,
				0x0000000000000010, 0x0000000000000020, 0x0000000000000040, 0x0000000000000080,
				0x0000000000000100, 0x0000000000000200, 0x0000000000000400, 0x0000000000000800,
				0x0000000000001000, 0x0000000000002000, 0x0000000000004000, 0x0000000000008000,
				0x0000000000010000, 0x0000000000020000, 0x0000000000040000, 0x0000000000080000,
				0x0000000000100000, 0x0000000000200000, 0x0000000000400000, 0x0000000000800000,
				0x0000000001000000, 0x0000000002000000, 0x0000000004000000, 0x0000000008000000,
				0x0000000010000000, 0x0000000020000000, 0x0000000040000000, 0x0000000080000000,
				0x0000000100000000, 0x0000000200000000, 0x0000000400000000, 0x0000000800000000,
				0x0000001000000000, 0x0000002000000000, 0x0000004000000000, 0x0000008000000000,
				0x0000010000000000, 0x0000020000000000, 0x0000040000000000, 0x0000080000000000,
				0x0000100000000000, 0x0000200000000000, 0x0000400000000000, 0x0000800000000000,
				0x0001000000000000, 0x0002000000000000, 0x0004000000000000, 0x0008000000000000,
				0x0010000000000000, 0x0020000000000000, 0x0040000000000000, 0x0080000000000000,
				0x0100000000000000, 0x0200000000000000, 0x0400000000000000, 0x0800000000000000,
				0x1000000000000000, 0x2000000000000000, 0x4000000000000000, 0x8000000000000000 };

			return ((number & getBitBitTable[bit]) >> bit);
		}

		inline std::uint32_t getBit(std::uint32_t number, int bit)
		{
			const static std::uint32_t getBitBitTable[32] = {
				0x00000001, 0x00000002, 0x00000004, 0x00000008,
				0x00000010, 0x00000020, 0x00000040, 0x00000080,
				0x00000100, 0x00000200, 0x00000400, 0x00000800,
				0x00001000, 0x00002000, 0x00004000, 0x00008000,
				0x00010000, 0x00020000, 0x00040000, 0x00080000,
				0x00100000, 0x00200000, 0x00400000, 0x00800000,
				0x01000000, 0x02000000, 0x04000000, 0x08000000,
				0x10000000, 0x20000000, 0x40000000, 0x80000000 };

			return ((number & getBitBitTable[bit]) >> bit);
		}

		inline std::uint16_t getBit(std::uint16_t number, int bit)
		{
			const static std::uint16_t getBitBitTable[16] = {
				0x0001, 0x0002, 0x0004, 0x0008,
				0x0010, 0x0020, 0x0040, 0x0080,
				0x0100, 0x0200, 0x0400, 0x0800,
				0x1000, 0x2000, 0x4000, 0x8000 };

			return ((number & getBitBitTable[bit]) >> bit);
		}

		inline std::uint8_t getBit(std::uint8_t number, int bit)
		{
			const static std::uint8_t getBitBitTable[8] = {
				0x01, 0x02, 0x04, 0x08,
				0x10, 0x20, 0x40, 0x80};

			return ((number & getBitBitTable[bit]) >> bit);
		}
	}

}