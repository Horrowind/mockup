#include "Internal.hpp"
#include <exception>
#include <iterator>
#include <cstdlib>

#include "Exceptions.hpp"

namespace worldlib
{
	namespace internal { const int LIBRARY_MAJOR = 2; const int LIBRARY_MINOR = 21; }

	template <typename inputIteratorType>
	bool checkROMVersion(inputIteratorType romStart, inputIteratorType romEnd)
	{
		std::string lmStr;			// There's a nice little © hiding in there, which is outside of ASCII.  Luckily I don't think sscanf will care.

		getLunarMagicString(romStart, romEnd, std::back_inserter(lmStr));
		int versionMajor, versionMinor;
		std::sscanf(lmStr.c_str() + 0x14, "%d.%d", &versionMajor, &versionMinor);

		return (versionMajor == internal::LIBRARY_MAJOR && versionMinor == internal::LIBRARY_MINOR);
	}

	template <typename inputIteratorType>
	bool checkROMValid(inputIteratorType romStart, inputIteratorType romEnd)
	{
		int romSize = std::distance(romStart, romEnd);

		std::string lmStr;

		getLunarMagicString(romStart, romEnd, std::back_inserter(lmStr));
		std::string wantStr = "Lunar Magic";
		bool sizeIsGood = romSize >= 0x100000;
		bool lunarMagicked = lmStr.substr(0, 0xB) == wantStr;
		bool isUSROM = readByteSFC(romStart, romEnd, 0xFFD9) == 0x01;
		std::string romTitle;
		getROMTitle(romStart, romEnd, std::back_inserter(romTitle), true);
		bool isSMW = romTitle == "SUPER MARIOWORLD";

		return sizeIsGood && lunarMagicked && isUSROM && isSMW;
	}

	template <typename inputIteratorType, typename outputIteratorType>
	outputIteratorType getLunarMagicString(inputIteratorType romStart, inputIteratorType romEnd, outputIteratorType out)
	{
		inputIteratorType current = romStart;
		inputIteratorType stringStart = romStart;
		inputIteratorType stringEnd = romStart;

		std::advance(current, SFCToPC(romStart, romEnd, 0x0FF0A0));
		std::advance(stringStart, SFCToPC(romStart, romEnd, 0x0FF0A0));
		std::advance(stringEnd, SFCToPC(romStart, romEnd, 0x0FF120));

		if (stringEnd >= romEnd) throw DataNotFoundException(DataNotFoundException::ROMTooSmall);	// Ensure we can actually get the whole string...


		while (current < stringEnd)
		{
			*(out++) = *current;
			current++;
		}

		return out;
	}
}