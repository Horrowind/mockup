#include <iterator>

#include "Internal.hpp"
#include "Compression.hpp"


namespace worldlib
{
	namespace internal
	{

		// Returns the level's "standard" palette, regardless of its override settings.
		template <typename inputIteratorType, typename outputIteratorType>
		outputIteratorType getLevelStandardPalette(inputIteratorType romStart, inputIteratorType romEnd, outputIteratorType out, int level)
		{
			int levelBackgroundIndex = (getLevelHeaderByte(romStart, romEnd, level, 0) & 0xE0) >> 5;
			int levelForegroundIndex = (getLevelHeaderByte(romStart, romEnd, level, 3) & 0x07) >> 0;
			int levelSpriteIndex =	   (getLevelHeaderByte(romStart, romEnd, level, 3) & 0x38) >> 3;

			int backgroundSwapPaletteLocation = (sharedBackgroundSwapPalettesLocation + levelBackgroundIndex * 24);
			int foregroundSwapPaletteLocation = (sharedForegroundSwapPalettesLocation + levelForegroundIndex * 24);
			int spriteSwapPaletteLocation = (sharedSpriteSwapPalettesLocation + levelSpriteIndex * 24);

			std::vector<std::uint32_t> palettes[0x10];

			// Set everything to black
			for (int y = 0; y < 16; y++) 
			{ 
				palettes[y].resize(16);
				for (int x = 0; x < 16; x++) palettes[y][x] = 0; 
			}

			// Set up the white colors
			for (int i = 0; i < 0x08; i++) palettes[i+0][1] = 0xFFE8F0F8;
			for (int i = 0; i < 0x08; i++) palettes[i+8][1] = 0xFFF8F8F8;


			// Left side of the palette:

			// Set up the swappable background palettes
			for (int x = 2, i = 0; i < 6; x++, i++) palettes[0x0][x] = SFCToARGB(readWordSFC(romStart, romEnd, backgroundSwapPaletteLocation + i * 2 + 0 * 12));
			for (int x = 2, i = 0; i < 6; x++, i++) palettes[0x1][x] = SFCToARGB(readWordSFC(romStart, romEnd, backgroundSwapPaletteLocation + i * 2 + 1 * 12));

			// Set up the swappable foreground palettes
			for (int x = 2, i = 0; i < 6; x++, i++) palettes[0x2][x] = SFCToARGB(readWordSFC(romStart, romEnd, foregroundSwapPaletteLocation + i * 2 + 0 * 12));
			for (int x = 2, i = 0; i < 6; x++, i++) palettes[0x3][x] = SFCToARGB(readWordSFC(romStart, romEnd, foregroundSwapPaletteLocation + i * 2 + 1 * 12));

			// Set up the constant foreground palettes
			for (int x = 2, i = 0; i < 6; x++, i++) palettes[0x4][x] = SFCToARGB(readWordSFC(romStart, romEnd, foregroundSwapPaletteLocation + i * 2 + 0 * 12));
			for (int x = 2, i = 0; i < 6; x++, i++) palettes[0x5][x] = SFCToARGB(readWordSFC(romStart, romEnd, foregroundSwapPaletteLocation + i * 2 + 1 * 12));
			for (int x = 2, i = 0; i < 6; x++, i++) palettes[0x6][x] = SFCToARGB(readWordSFC(romStart, romEnd, foregroundSwapPaletteLocation + i * 2 + 2 * 12));
			for (int x = 2, i = 0; i < 6; x++, i++) palettes[0x7][x] = SFCToARGB(readWordSFC(romStart, romEnd, foregroundSwapPaletteLocation + i * 2 + 3 * 12));

			// Set up the constant sprite palettes
			for (int x = 2, i = 0; i < 6; x++, i++) palettes[0x8][x] = SFCToARGB(readWordSFC(romStart, romEnd, sharedSpritePaletteLocation + i * 2 + 0 * 12));
			for (int x = 2, i = 0; i < 6; x++, i++) palettes[0x9][x] = SFCToARGB(readWordSFC(romStart, romEnd, sharedSpritePaletteLocation + i * 2 + 1 * 12));
			for (int x = 2, i = 0; i < 6; x++, i++) palettes[0xA][x] = SFCToARGB(readWordSFC(romStart, romEnd, sharedSpritePaletteLocation + i * 2 + 2 * 12));
			for (int x = 2, i = 0; i < 6; x++, i++) palettes[0xB][x] = SFCToARGB(readWordSFC(romStart, romEnd, sharedSpritePaletteLocation + i * 2 + 3 * 12));
			for (int x = 2, i = 0; i < 6; x++, i++) palettes[0xC][x] = SFCToARGB(readWordSFC(romStart, romEnd, sharedSpritePaletteLocation + i * 2 + 4 * 12));
			for (int x = 2, i = 0; i < 6; x++, i++) palettes[0xD][x] = SFCToARGB(readWordSFC(romStart, romEnd, sharedSpritePaletteLocation + i * 2 + 5 * 12));

			// Set up the swappable sprite palettes
			for (int x = 2, i = 0; i < 6; x++, i++) palettes[0xE][x] = SFCToARGB(readWordSFC(romStart, romEnd, sharedSpriteSwapPalettesLocation + i * 2 + 0 * 12));
			for (int x = 2, i = 0; i < 6; x++, i++) palettes[0xF][x] = SFCToARGB(readWordSFC(romStart, romEnd, sharedSpriteSwapPalettesLocation + i * 2 + 1 * 12));


			// Right side of the palette:

			// Set up the layer 3 palettes
			for (int x = 8, i = 0; i < 8; x++, i++) palettes[0x0][x] = SFCToARGB(readWordSFC(romStart, romEnd, sharedLayer3ConstPalettesLocation + i * 2 + 0 * 16));
			for (int x = 8, i = 0; i < 8; x++, i++) palettes[0x1][x] = SFCToARGB(readWordSFC(romStart, romEnd, sharedLayer3ConstPalettesLocation + i * 2 + 1 * 16));

			// Set up the berry palettes
			for (int x = 9, i = 0; i < 7; x++, i++) palettes[0x2][x] = SFCToARGB(readWordSFC(romStart, romEnd, sharedBerryPaletteLocation + i * 2 + 0 * 14));
			for (int x = 9, i = 0; i < 7; x++, i++) palettes[0x3][x] = SFCToARGB(readWordSFC(romStart, romEnd, sharedBerryPaletteLocation + i * 2 + 1 * 14));
			for (int x = 9, i = 0; i < 7; x++, i++) palettes[0x4][x] = SFCToARGB(readWordSFC(romStart, romEnd, sharedBerryPaletteLocation + i * 2 + 2 * 14));
			for (int x = 9, i = 0; i < 7; x++, i++) palettes[0x9][x] = SFCToARGB(readWordSFC(romStart, romEnd, sharedBerryPaletteLocation + i * 2 + 0 * 14));
			for (int x = 9, i = 0; i < 7; x++, i++) palettes[0xA][x] = SFCToARGB(readWordSFC(romStart, romEnd, sharedBerryPaletteLocation + i * 2 + 1 * 14));
			for (int x = 9, i = 0; i < 7; x++, i++) palettes[0xB][x] = SFCToARGB(readWordSFC(romStart, romEnd, sharedBerryPaletteLocation + i * 2 + 2 * 14));

			// Set up Mario's palette
			for (int x = 6, i = 0; i < 10; x++, i++) palettes[8][x] = SFCToARGB(readWordSFC(romStart, romEnd, sharedMarioPaletteLocation + i * 2 + 0 * 20));

			// Finally, output everthing
			for (int y = 0; y < 16; y++) for (int x = 0; x < 16; x++) *(out++) = palettes[y][x];

			return out;
		}


		// Returns the level's custom palette, regardless of its override settings.  If there is no palette, an exception is thrown.
		template <typename inputIteratorType, typename outputIteratorType>
		outputIteratorType getLevelCustomPalette(inputIteratorType romStart, inputIteratorType romEnd, outputIteratorType out, int level)
		{
			auto address = readTrivigintetSFC(romStart, romEnd, customPalettePointerTableLocation + level * 3);

			if (address == 0) throw DataNotFoundException(DataNotFoundException::InvalidAssumption);

			address += 2;		// Skip the background color.

			for (int i = 0; i < 512; i+=2)
			{
				auto color = readWordSFC(romStart, romEnd, address + i);
				*(out++) = SFCToARGB(color);
			}

			return out;
		}
	}

	template <typename inputIteratorType, typename colorIteratorType>
	void setLevelCustomPalette(inputIteratorType romStart, inputIteratorType romEnd, colorIteratorType paletteStart, colorIteratorType paletteEnd, int level)
	{
		auto address = readTrivigintetSFC(romStart, romEnd, internal::customPalettePointerTableLocation + level * 3);

		if (address == 0) throw DataNotFoundException(DataNotFoundException::InvalidAssumption);

		address += 2;		// Skip the background color.

		int colorsInserted = 0;
		while (paletteStart != paletteEnd && colorsInserted < 256)
		{
			std::uint16_t color = ARGBToSFC(*paletteStart);
			writeWordSFC(romStart, romEnd, address + colorsInserted * 2, color);
			colorsInserted++;
			paletteStart++;
		}
	}

	template <typename inputIteratorType, typename outputIteratorType>
	outputIteratorType getLevelPalette(inputIteratorType romStart, inputIteratorType romEnd, outputIteratorType out, int level)
	{
		auto customPalettePointer = readTrivigintetSFC(romStart, romEnd, internal::customPalettePointerTableLocation + level * 3);
		if (customPalettePointer == 0 || customPalettePointer == 0xFFFFFF)
			return internal::getLevelStandardPalette(romStart, romEnd, out, level);
		else
			return internal::getLevelCustomPalette(romStart, romEnd, out, level);
	}


	template <typename inputIteratorType>
	std::uint32_t getLevelBackgroundColor(inputIteratorType romStart, inputIteratorType romEnd, int level)
	{
		if (readTrivigintetSFC(romStart, romEnd, internal::customPalettePointerTableLocation + level * 3) == 0)
		{
			auto byte = internal::getLevelHeaderByte(romStart, romEnd, level, 2);
			internal::getBits(byte, 0xE0);
			return SFCToARGB(readWordSFC(romStart, romEnd, internal::sharedBackgroundColorsLocation + byte));
		}
		else
		{
			auto address = readTrivigintetSFC(romStart, romEnd, internal::customPalettePointerTableLocation + level * 3);
			if (address == 0) throw DataNotFoundException(DataNotFoundException::InvalidAssumption);

			return SFCToARGB(readWordSFC(romStart, romEnd, address));
		}
	}


	template <typename inputIteratorType, typename outputIteratorType>
	outputIteratorType getLevelGraphicsSlots(inputIteratorType romStart, inputIteratorType romEnd, outputIteratorType out, int level)
	{
		getLevelBackgroundGraphicsSlots(romStart, romEnd, out, level);
		getLevelSpriteGraphicsSlots(romStart, romEnd, out, level);
		*(out++) = getLevelAnimatedTileAreaGraphicsSlot(romStart, romEnd, level);
		return out;
	}

	

	template <typename inputIteratorType>
	bool levelUsesGFXBypass(inputIteratorType romStart, inputIteratorType romEnd, int level)
	{
		bool usesBypass;
		int address = readTrivigintetSFC(romStart, romEnd, internal::exgfxBypassListPointerToPointerTable);
		if (address == 0xFFFFFF)
		{
			usesBypass = false;
		}
		else
		{
			address += internal::exgfxBypassOffset + level * 0x20;
			usesBypass = (readByteSFC(romStart, romEnd, address + 1) & 0x80) == 0x80;
		}
		return usesBypass;
	}

	template <typename inputIteratorType>
	bool levelUsesCustomPalette(inputIteratorType romStart, inputIteratorType romEnd, int level)
	{
		auto customPalettePointer = readTrivigintetSFC(romStart, romEnd, internal::customPalettePointerTableLocation + level * 3);
		if (customPalettePointer == 0 || customPalettePointer == 0xFFFFFF)
			return false;
		else
			return true;
	}

	template <typename inputIteratorType, typename outputIteratorType>
	outputIteratorType getLevelBackgroundGraphicsSlots(inputIteratorType romStart, inputIteratorType romEnd, outputIteratorType out, int level)
	{
		bool usesBypass = levelUsesGFXBypass(romStart, romEnd, level);

		if (!usesBypass)
		{
			int tileset = internal::getBits(internal::getLevelHeaderByte(romStart, romEnd, level, 4), 0x0F);
			auto address = internal::backgroundSlotListTableLocation + tileset * 4;
			*(out++) = readByteSFC(romStart, romEnd, address + 0x00);
			*(out++) = readByteSFC(romStart, romEnd, address + 0x01);
			*(out++) = readByteSFC(romStart, romEnd, address + 0x02);
			*(out++) = readByteSFC(romStart, romEnd, address + 0x03);
			*(out++) = 0x7F;
			*(out++) = 0x7F;
		}
		else
		{
			auto address = readTrivigintetSFC(romStart, romEnd, internal::exgfxBypassListPointerToPointerTable) + internal::exgfxBypassOffset + level * 0x20;
			*(out++) = readWordSFC(romStart, romEnd, address + 0x10);
			*(out++) = readWordSFC(romStart, romEnd, address + 0x0E);
			*(out++) = readWordSFC(romStart, romEnd, address + 0x0C);
			*(out++) = readWordSFC(romStart, romEnd, address + 0x0A);
			*(out++) = readWordSFC(romStart, romEnd, address + 0x08);
			*(out++) = readWordSFC(romStart, romEnd, address + 0x06);
		}

		return out;
	}


	template <typename inputIteratorType>
	void setLevelSingleGraphicsSlot(inputIteratorType romStart, inputIteratorType romEnd, int level, GFXSlots slotToSet, std::uint16_t value)
	{
		bool usesBypass = levelUsesGFXBypass(romStart, romEnd, level);
		if (usesBypass == false)
			throw DataNotFoundException(DataNotFoundException::InvalidAssumption);


		auto address = readTrivigintetSFC(romStart, romEnd, internal::exgfxBypassListPointerToPointerTable) + internal::exgfxBypassOffset + level * 0x20;


		switch (slotToSet)
		{
		case GFXSlots::FG1: writeWordSFC(romStart, romEnd, address + 0x10, value); break;
		case GFXSlots::FG2: writeWordSFC(romStart, romEnd, address + 0x0E, value); break;
		case GFXSlots::BG1: writeWordSFC(romStart, romEnd, address + 0x0C, value); break;
		case GFXSlots::FG3: writeWordSFC(romStart, romEnd, address + 0x0A, value); break;
		case GFXSlots::BG2: writeWordSFC(romStart, romEnd, address + 0x08, value); break;
		case GFXSlots::BG3: writeWordSFC(romStart, romEnd, address + 0x06, value); break;

		case GFXSlots::SP1: writeWordSFC(romStart, romEnd, address + 0x18, value); break;
		case GFXSlots::SP2: writeWordSFC(romStart, romEnd, address + 0x16, value); break;
		case GFXSlots::SP3: writeWordSFC(romStart, romEnd, address + 0x14, value); break;
		case GFXSlots::SP4: writeWordSFC(romStart, romEnd, address + 0x12, value); break;

		case GFXSlots::AN2: writeWordSFC(romStart, romEnd, address + 0x1A, value); break;
		}
	}

	template <typename inputIteratorType, typename outputIteratorType>
	outputIteratorType getLevelSpriteGraphicsSlots(inputIteratorType romStart, inputIteratorType romEnd, outputIteratorType out, int level)
	{
		bool usesBypass = levelUsesGFXBypass(romStart, romEnd, level);
		if (!usesBypass)
		{
			int tileset = internal::getBits(internal::getLevelHeaderByte(romStart, romEnd, level, 2), 0x0F);
			auto address = internal::spriteSlotListTableLocation + tileset * 4;
			*(out++) = readByteSFC(romStart, romEnd, address + 0x00);
			*(out++) = readByteSFC(romStart, romEnd, address + 0x01);
			*(out++) = readByteSFC(romStart, romEnd, address + 0x02);
			*(out++) = readByteSFC(romStart, romEnd, address + 0x03);
		}
		else
		{
			auto address = readTrivigintetSFC(romStart, romEnd, internal::exgfxBypassListPointerToPointerTable) + internal::exgfxBypassOffset + level * 0x20;
			*(out++) = readWordSFC(romStart, romEnd, address + 0x18);
			*(out++) = readWordSFC(romStart, romEnd, address + 0x16);
			*(out++) = readWordSFC(romStart, romEnd, address + 0x14);
			*(out++) = readWordSFC(romStart, romEnd, address + 0x12);
		}

		return out;
	}

	template <typename inputIteratorType>
	std::uint16_t getLevelAnimatedTileAreaGraphicsSlot(inputIteratorType romStart, inputIteratorType romEnd, int level)
	{
		bool usesBypass = levelUsesGFXBypass(romStart, romEnd, level);
		if (!usesBypass)
		{
			return 0x007F;
		}
		else
		{
			int address = readTrivigintetSFC(romStart, romEnd, internal::exgfxBypassListPointerToPointerTable) + internal::exgfxBypassOffset + level * 0x20;
			return readWordSFC(romStart, romEnd, address + 0x1A);
		}
	}


	template <typename inputIteratorType>
	std::uint16_t getLevelSingleGraphicsSlot(inputIteratorType romStart, inputIteratorType romEnd, int level, GFXSlots slotToGet)
	{
		std::vector<std::uint16_t> gfxSlots;
		getLevelGraphicsSlots(romStart, romEnd, std::back_inserter(gfxSlots), level);
		return gfxSlots[(int)slotToGet];
	}

	template <typename inputIteratorType>
	int getAddressOfGraphicsFile(inputIteratorType romStart, inputIteratorType romEnd, int file)
	{
		int address = 0;
		if (file >= 0 && file <= 0x31)
			address = readByteSFC(romStart, romEnd, internal::originalGraphicsFilesLowByteTableLocation + file) | (readByteSFC(romStart, romEnd, internal::originalGraphicsFilesHighByteTableLocation + file) << 8) | (readByteSFC(romStart, romEnd, internal::originalGraphicsFilesBankByteTableLocation + file) << 16);
		else if (file >= 0x80 && file <= 0xFF)
			address = readTrivigintetSFC(romStart, romEnd, readTrivigintetSFC(romStart, romEnd, internal::standardExGFXPointerToPointerTableLocation) + (file - 0x80) * 3);
		else if (file >= 0x100 && file <= 0xFFF)
			address = readTrivigintetSFC(romStart, romEnd, readTrivigintetSFC(romStart, romEnd, internal::superExGFXPointerToPointerTableLocation) + (file - 0x100) * 3);
		else if (file == 0x7F)
			return -1;
		else
			throw std::invalid_argument("ExGFX file is not valid.");

		if (address == 0 || address == 0xFFFFFF)
			throw DataNotFoundException(DataNotFoundException::NullPointer);

		return address;
	}

	template <typename inputIteratorType>
	bool romContainsGraphicsFile(inputIteratorType romStart, inputIteratorType romEnd, int file)
	{
		int address = 0;
		if (file >= 0 && file <= 0x31)
			address = readByteSFC(romStart, romEnd, internal::originalGraphicsFilesLowByteTableLocation + file) | (readByteSFC(romStart, romEnd, internal::originalGraphicsFilesHighByteTableLocation + file) << 8) | (readByteSFC(romStart, romEnd, internal::originalGraphicsFilesBankByteTableLocation + file) << 16);
		else if (file >= 0x80 && file <= 0xFF)
			address = readTrivigintetSFC(romStart, romEnd, readTrivigintetSFC(romStart, romEnd, internal::standardExGFXPointerToPointerTableLocation) + (file - 0x80) * 3);
		else if (file >= 0x100 && file <= 0xFFF)
			address = readTrivigintetSFC(romStart, romEnd, readTrivigintetSFC(romStart, romEnd, internal::superExGFXPointerToPointerTableLocation) + (file - 0x100) * 3);
		else if (file == 0x7F)
			return true;
		else
			return false;

		if (address == 0 || address == 0xFFFFFF)
			return false;

		return true;
	}

	template <typename inputIteratorType, typename outputIteratorType>
	outputIteratorType decompressGraphicsFile(inputIteratorType romStart, inputIteratorType romEnd, outputIteratorType out, int file, int *compressedSize, int *decompressedSize)
	{
		return decompressData(romStart, romEnd, romStart + SFCToPC(romStart, romEnd, getAddressOfGraphicsFile(romStart, romEnd, file)), romEnd, out, compressedSize, decompressedSize);
	}




	template <typename graphicsInputIteratorType, typename paletteInputIteratorType, typename outputIteratorType>
	outputIteratorType indexedImageToBitmap(graphicsInputIteratorType graphicsStart, graphicsInputIteratorType graphicsEnd, paletteInputIteratorType paletteStart, paletteInputIteratorType paletteEnd, int bpp, bool flipX, bool flipY, int paletteNumber, outputIteratorType out)
	{
		auto current = graphicsStart;
		auto byteCount = std::distance(graphicsStart, graphicsEnd);
		int bytesPerTile = 8 * bpp;
		int height = 8;
		int width = byteCount / bytesPerTile;
		if (byteCount % bytesPerTile != 0) width++;
		width *= 8;
		
		int rowToUse[64] =  { 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7,
				        0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7,
					0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7,
					0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7 };
		int planeToUse[64] =  { 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1,
					2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3,
					4, 5, 4, 5, 4, 5, 4, 5, 4, 5, 4, 5, 4, 5, 4, 5, 
					6, 7, 6, 7, 6, 7, 6, 7, 6, 7, 6, 7, 6, 7, 6, 7 };

		int colorsPerBPP[] = { 0, 2, 4, 8, 16, 32, 64, 128, 256 };		// How many colors we have access to per bpp value.


		int maxTableValue = 0;
		if (bpp == 8)		maxTableValue = 0x3F;
		else if (bpp == 4)	maxTableValue = 0x1F;
		else			maxTableValue = 0x0F;

		std::vector<std::uint64_t> currentTile(8);					// Holds 8 64-bit integers.  Every 8 bits is the color for that column and row of the tile.
												// Ex: currentTile[0] contains the colors for row 0.

		std::vector<std::uint64_t> allTiles;						// Holds all the tiles we've completed so far.  Same format as above, so every 8 entries is a tile.
		
		int i = 0;
		while (current < graphicsEnd)
		{
			if (currentTile.size() == 0) currentTile.resize(8);

			auto byte = *current;
			auto exploded = internal::explodeInt(internal::explodeInt(internal::explodeInt(byte)));	// Get the bits for each color (i.e. explodes the byte so that each bit gets its own byte)
			
			if (flipX)
				exploded = internal::reverseBits(exploded);

			exploded <<= planeToUse[i & maxTableValue];				// Shift the bits into their correct plane
			currentTile[rowToUse[i & maxTableValue]] |= exploded;			// And or them to the current value.

			i++;
			current++;

			if ((i & maxTableValue) == 0)							// If we've completed a tile...
			{
				for (auto v : currentTile) allTiles.push_back(v);		// Add it to the list of completed tiles.
				currentTile.clear();						// And reset the current tile.
				currentTile.reserve(8);
			}
		}

		if (currentTile.size() != 0)							// If we have some leftover data (i.e. there was a tile missing some information)
		{
			for (auto v : currentTile) allTiles.push_back(v);			// Add it to the list of completed tiles.  I'll be missing some information, but it's all we can do.
		}

		
		std::vector<std::uint32_t> finalResult;
		finalResult.resize(height * width);
		i = 0;

		for (auto v : allTiles)
		{
			std::uint64_t indixes[8] = {	internal::getBits(v, 0xFF00000000000000),
							internal::getBits(v, 0x00FF000000000000),
							internal::getBits(v, 0x0000FF0000000000),
							internal::getBits(v, 0x000000FF00000000),
							internal::getBits(v, 0x00000000FF000000),
							internal::getBits(v, 0x0000000000FF0000),
							internal::getBits(v, 0x000000000000FF00),
							internal::getBits(v, 0x00000000000000FF) };

			for (int j = 0; j < 8; j++)
			{
				int y = j;
				if (flipY) y = 7 - j;

				auto colorIterator = paletteStart;
				std::advance(colorIterator, static_cast<std::uint8_t>(indixes[j]) + colorsPerBPP[bpp] * paletteNumber);

				if (colorIterator >= paletteEnd)
					throw IndexedGraphicsException(); //std::runtime_error("Graphics file contained a color that was out of the range of the palette.");

				auto color = *colorIterator;

				if (indixes[y] == 0)
					color &= 0x00FFFFFF;				// Make color 0 invisible.

				finalResult[i * 8 + y] = color;
			}

			i++;
		}

		for (auto v : finalResult)
			*(out++) = v;

		return out;
	}


	template <typename graphicsInputIteratorType, typename paletteInputIteratorType, typename outputIteratorType>
	outputIteratorType indexedImageToBitmap(graphicsInputIteratorType graphicsFileStart, graphicsInputIteratorType graphicsFileEnd, paletteInputIteratorType paletteStart, paletteInputIteratorType paletteEnd, int tilesInOneRow, int bpp, int x, int y, int width, int height, bool flipX, bool flipY, int paletteNumber, outputIteratorType out, int *resultingWidth, int *resultingHeight)
	{

		if (graphicsFileStart == graphicsFileEnd)
		{
			if (resultingHeight != nullptr) *resultingHeight = 0;
			if (resultingWidth != nullptr) *resultingWidth = 0;
			return out;
		}

		auto current = graphicsFileStart;
		int advanceBy[] = { 0, 8, 16, 24, 32, 40, 48, 56, 64 };

		int tileX = 0;
		int tileY = 0;
		std::vector<std::vector<std::uint32_t>> entireBitmap(1);

		while (current < graphicsFileEnd)
		{
			std::vector<std::uint32_t> thisTile;
			indexedImageToBitmap(current, current + advanceBy[bpp], paletteStart, paletteEnd, bpp, flipX, flipY, paletteNumber, std::back_inserter(thisTile));

			int subX = 0;
			int subY = 0;
			for (auto v : thisTile)
			{
				if ((int)entireBitmap.size() <= tileY * 8 + subY)
					entireBitmap.resize(tileY * 8 + subY + 1); 
				if ((int)entireBitmap[tileY * 8 + subY].size() <= tileX * 8 + subX)
					entireBitmap[tileY * 8 + subY].resize(tileX * 8 + subX + 1);

				entireBitmap[tileY * 8 + subY][tileX * 8 + subX] = v;

				subX++;
				if (subX == 8)
				{
					subX = 0;
					subY++;
				}
			}

			tileX++;
			if (tileX == tilesInOneRow)
			{
				tileX = 0;
				tileY++;
			}

			current += advanceBy[bpp];
		}

		unsigned int j, i;

		for (j = 0; j < (unsigned int)height; j++)
		{
			for (i = 0; i < (unsigned int)width; i++)
			{
				if (j + y >= (int)entireBitmap.size())
				{
					if (height != -1) 
						*(out++) = 0;
					else
					{
						goto finished;				// Break out of the outermost for loop.
					}
				}
				else if (i + x >= (int)entireBitmap[j + y].size())
				{
					if (width == -1)				// Only stop early on the first row.  Fill out properly everywhere else
					{
						width = i;
						break;
					}
					else
						*(out++) = 0;
				}
				else
					*(out++) = entireBitmap[j + y][i + x];
			}
		}

	finished:

		if (resultingHeight != nullptr) *resultingHeight = j - y;
		if (resultingWidth != nullptr) *resultingWidth = width - x;

		return out;
	}


	template <typename graphicsInputIteratorType, typename paletteInputIteratorType, typename outputIteratorType>
	outputIteratorType indexedImageToBitmap(graphicsInputIteratorType graphicsFileStart, graphicsInputIteratorType graphicsFileEnd, paletteInputIteratorType paletteStart, paletteInputIteratorType paletteEnd, int tilesInOneRow, int bpp, int paletteNumber, outputIteratorType out, int *resultingWidth, int *resultingHeight)
	{
		return indexedImageToBitmap(graphicsFileStart, graphicsFileEnd, paletteStart, paletteEnd, tilesInOneRow, bpp, 0, 0, -1, -1, false, false, paletteNumber, out, resultingWidth, resultingHeight);
	}

	template <typename graphicsInputIteratorType, typename paletteInputIteratorType, typename outputIteratorType>
	outputIteratorType indexedImageToBitmap(graphicsInputIteratorType graphicsFileStart, graphicsInputIteratorType graphicsFileEnd, paletteInputIteratorType paletteStart, paletteInputIteratorType paletteEnd, int bpp, int paletteNumber, outputIteratorType out, int *resultingWidth, int *resultingHeight)
	{
		return indexedImageToBitmap(graphicsFileStart, graphicsFileEnd, paletteStart, paletteEnd, 0x10, bpp, paletteNumber, out, resultingWidth, resultingHeight);
	}
}
