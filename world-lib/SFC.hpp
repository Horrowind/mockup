#pragma once

namespace worldlib
{

//////////////////////////////////////////////////////////////////////////////
/// \file SFC.hpp
/// \brief Contains various functions for working with the SFC.
///
/// \details Please note that while there are routines for getting data from a ROM, there are no routines for setting it.  
///
/// \addtogroup SFC
///  @{
//////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////
	/// \ingroup SFC
	/// \brief Tests if the ROM uses the SA-1 chip.  
	/// \details Functions use this automatically when determining addresses so you probably won't need to use it manually. Checks 0x7FD5 and 0x7FD6 as PC addresses for their SA-1 status.  Returns true for those addresses.  This is the only place where PC addressing is used. For this reason, this library is not compatible with HiROM.
	///
	/// \param romStart	An iterator pointing to the start of the ROM data
	/// \param romEnd	An iterator pointing to the end of the ROM data
	///
	/// \return True if the ROM should use SA-1 addressing, false otherwise.
	///
	/// \throws DataNotFoundException The ROM was too small to determine mapping, meaning it's most likely corrupted.
	///
	////////////////////////////////////////////////////////////
	template <typename inputIteratorType>
	bool romUsesSA1(inputIteratorType romStart, inputIteratorType romEnd);

	////////////////////////////////////////////////////////////
	/// \ingroup SFC
	/// \brief Converts an address in SFC format and turns it into PC format (assuming no header, as usual).  Only lorom and SA-1 mappings are supported.
	///
	/// \param romStart		An iterator pointing to the start of the ROM data
	/// \param romEnd		An iterator pointing to the end of the ROM data
	/// \param addr			The SFC address to convert
	///
	/// \return A PC address corresponding to the given SFC address.
	///
	/// \throws DataNotFoundException The ROM was too small to determine mapping, meaning it's most likely corrupted.
	/// \throws InvalidAddressException The address cannot be properly converted.
	///
	////////////////////////////////////////////////////////////
	template <typename inputIteratorType>
	inline int SFCToPC(inputIteratorType romStart, inputIteratorType romEnd, int addr);

	////////////////////////////////////////////////////////////
	/// \ingroup SFC
	/// \brief Converts an address in PC format and turns it into SFC format (assuming no header, as usual).  Only lorom and SA-1 mappings are supported.
	///
	/// \param romStart		An iterator pointing to the start of the ROM data
	/// \param romEnd		An iterator pointing to the end of the ROM data
	/// \param addr			The PC address to convert
	///
	/// \return An address in SFC format corresponding to the given PC address.
	///
	/// \throws DataNotFoundException The ROM was too small to determine mapping, meaning it's most likely corrupted.
	/// \throws InvalidAddressException The address cannot be properly converted.
	///
	////////////////////////////////////////////////////////////
	template <typename inputIteratorType>
	inline int PCToSFC(inputIteratorType romStart, inputIteratorType romEnd, int addr);

	////////////////////////////////////////////////////////////
	/// \ingroup SFC
	/// \brief Converts an color in SFC format and turns it into ARGB format.
	///
	/// \param color		The color to convert.
	///
	/// \return An ARGB color corresponding to the given SFC color
	///
	////////////////////////////////////////////////////////////
	inline std::uint32_t SFCToARGB(std::uint16_t color);

	////////////////////////////////////////////////////////////
	/// \ingroup SFC
	/// \brief Converts an color in ARGB format and turns it into SFC format.
	///
	/// \param color		The color to convert.
	///
	/// \return A color in SFC format corresponding to the given ARGB color
	///
	////////////////////////////////////////////////////////////
	inline std::uint16_t ARGBToSFC(std::uint32_t color);


	////////////////////////////////////////////////////////////
	/// \ingroup SFC
	/// \brief Read a single byte from a PC address
	///
	/// \param romStart		An iterator pointing to the start of the ROM data
	/// \param romEnd		An iterator pointing to the end of the ROM data
	/// \param offset		The address to get the data from
	///
	/// \return A single byte
	///
	/// \throws DataNotFoundException The ROM did not contain the address specified.
	///
	////////////////////////////////////////////////////////////
	template <typename inputIteratorType> std::uint8_t readBytePC(inputIteratorType romStart, inputIteratorType romEnd, int offset);

	////////////////////////////////////////////////////////////
	/// \ingroup SFC
	/// \brief Read two bytes from a PC address, correctly de-endianated.
	///
	/// \param romStart		An iterator pointing to the start of the ROM data
	/// \param romEnd		An iterator pointing to the end of the ROM data
	/// \param offset		The address to get the data from
	///
	/// \return A 16-bit value
	///
	/// \throws DataNotFoundException The ROM did not contain the address specified.
	///
	////////////////////////////////////////////////////////////
	template <typename inputIteratorType> std::uint16_t readWordPC(inputIteratorType romStart, inputIteratorType romEnd, int offset);

	////////////////////////////////////////////////////////////
	/// \ingroup SFC
	/// \brief Read three bytes from a PC address, correctly de-endianated.
	///
	/// \param romStart		An iterator pointing to the start of the ROM data
	/// \param romEnd		An iterator pointing to the end of the ROM data
	/// \param offset		The address to get the data from
	///
	/// \return A 24-bit value
	///
	/// \throws DataNotFoundException The ROM did not contain the address specified.
	///
	////////////////////////////////////////////////////////////
	template <typename inputIteratorType> std::uint32_t readTrivigintetPC(inputIteratorType romStart, inputIteratorType romEnd, int offset);

	////////////////////////////////////////////////////////////
	/// \ingroup SFC
	/// \brief Read a single byte from an address in SFC format
	///
	/// \param romStart		An iterator pointing to the start of the ROM data
	/// \param romEnd		An iterator pointing to the end of the ROM data
	/// \param address		The address to get the data from
	///
	/// \return A single byte
	///
	/// \throws DataNotFoundException The ROM did not contain the address specified.
	/// \throws DataNotFoundException The ROM was too small to determine mapping, meaning it's most likely corrupted.
	/// \throws InvalidAddressException The address cannot be properly converted.
	///
	////////////////////////////////////////////////////////////
	template <typename inputIteratorType> std::uint8_t readByteSFC(inputIteratorType romStart, inputIteratorType romEnd, int address);

	////////////////////////////////////////////////////////////
	/// \ingroup SFC
	/// \brief Read two bytes from an address in SFC format, correctly de-endianated.
	///
	/// \param romStart		An iterator pointing to the start of the ROM data
	/// \param romEnd		An iterator pointing to the end of the ROM data
	/// \param address		The address to get the data from
	///
	/// \return A 16-bit value
	///
	/// \throws DataNotFoundException The ROM did not contain the address specified.
	/// \throws DataNotFoundException The ROM was too small to determine mapping, meaning it's most likely corrupted.
	/// \throws InvalidAddressException The address cannot be properly converted.
	///
	////////////////////////////////////////////////////////////
	template <typename inputIteratorType> std::uint16_t readWordSFC(inputIteratorType romStart, inputIteratorType romEnd, int address);

	////////////////////////////////////////////////////////////
	/// \ingroup SFC
	/// \brief Read three bytes from an address in SFC format, correctly de-endianated.
	///
	/// \param romStart		An iterator pointing to the start of the ROM data
	/// \param romEnd		An iterator pointing to the end of the ROM data
	/// \param address		The address to get the data from
	///
	/// \return A 24-bit value
	///
	/// \throws DataNotFoundException The ROM did not contain the address specified.
	/// \throws DataNotFoundException The ROM was too small to determine mapping, meaning it's most likely corrupted.
	/// \throws InvalidAddressException The address cannot be properly converted.
	///
	////////////////////////////////////////////////////////////
	template <typename inputIteratorType> std::uint32_t readTrivigintetSFC(inputIteratorType romStart, inputIteratorType romEnd, int address);


	////////////////////////////////////////////////////////////
	/// \ingroup SFC
	/// \brief Write a single byte to a a PC address
	///
	/// \param romStart		An iterator pointing to the start of the ROM data
	/// \param romEnd		An iterator pointing to the end of the ROM data
	/// \param offset		The address to get the data from
	/// \param value		The value to write
	///
	/// \throws DataNotFoundException The ROM did not contain the address specified.
	///
	////////////////////////////////////////////////////////////
	template <typename inputIteratorType> void writeBytePC(inputIteratorType romStart, inputIteratorType romEnd, int offset, std::uint8_t value);

	////////////////////////////////////////////////////////////
	/// \ingroup SFC
	/// \brief Write two bytes to a PC address, correctly de-endianated.
	///
	/// \param romStart		An iterator pointing to the start of the ROM data
	/// \param romEnd		An iterator pointing to the end of the ROM data
	/// \param offset		The address to get the data from
	/// \param value		The value to write
	///
	/// \throws DataNotFoundException The ROM did not contain the address specified.
	///
	////////////////////////////////////////////////////////////
	template <typename inputIteratorType> void writeWordPC(inputIteratorType romStart, inputIteratorType romEnd, int offset, std::uint16_t value);

	////////////////////////////////////////////////////////////
	/// \ingroup SFC
	/// \brief Write three bytes to a PC address, correctly de-endianated.
	///
	/// \param romStart		An iterator pointing to the start of the ROM data
	/// \param romEnd		An iterator pointing to the end of the ROM data
	/// \param offset		The address to get the data from
	/// \param value		The value to write
	///
	/// \throws DataNotFoundException The ROM did not contain the address specified.
	///
	////////////////////////////////////////////////////////////
	template <typename inputIteratorType> void writeTrivigintetPC(inputIteratorType romStart, inputIteratorType romEnd, int offset, std::uint32_t value);

	////////////////////////////////////////////////////////////
	/// \ingroup SFC
	/// \brief Write a single byte to an address in SFC format
	///
	/// \param romStart		An iterator pointing to the start of the ROM data
	/// \param romEnd		An iterator pointing to the end of the ROM data
	/// \param offset		The address to get the data from
	/// \param value		The value to write
	///
	/// \throws DataNotFoundException The ROM did not contain the address specified.
	/// \throws DataNotFoundException The ROM was too small to determine mapping, meaning it's most likely corrupted.
	/// \throws InvalidAddressException The address cannot be properly converted.
	///
	////////////////////////////////////////////////////////////
	template <typename inputIteratorType> void writeByteSFC(inputIteratorType romStart, inputIteratorType romEnd, int offset, std::uint8_t value);

	////////////////////////////////////////////////////////////
	/// \ingroup SFC
	/// \brief Write two bytes to an address in SFC format, correctly de-endianated.
	///
	/// \param romStart		An iterator pointing to the start of the ROM data
	/// \param romEnd		An iterator pointing to the end of the ROM data
	/// \param offset		The address to get the data from
	/// \param value		The value to write
	///
	/// \throws DataNotFoundException The ROM did not contain the address specified.
	/// \throws DataNotFoundException The ROM was too small to determine mapping, meaning it's most likely corrupted.
	/// \throws InvalidAddressException The address cannot be properly converted.
	///
	////////////////////////////////////////////////////////////
	template <typename inputIteratorType> void writeWordSFC(inputIteratorType romStart, inputIteratorType romEnd, int offset, std::uint16_t value);

	////////////////////////////////////////////////////////////
	/// \ingroup SFC
	/// \brief Write three bytes to an address in SFC format, correctly de-endianated.
	///
	/// \param romStart		An iterator pointing to the start of the ROM data
	/// \param romEnd		An iterator pointing to the end of the ROM data
	/// \param offset		The address to get the data from
	/// \param value		The value to write
	///
	/// \throws DataNotFoundException The ROM did not contain the address specified.
	/// \throws DataNotFoundException The ROM was too small to determine mapping, meaning it's most likely corrupted.
	/// \throws InvalidAddressException The address cannot be properly converted.
	///
	////////////////////////////////////////////////////////////
	template <typename inputIteratorType> void writeTrivigintetSFC(inputIteratorType romStart, inputIteratorType romEnd, int offset, std::uint32_t value);

	////////////////////////////////////////////////////////////
	/// \ingroup SFC
	/// \brief Returns the game's title in the ROM header (not 0x200 byte header at the start).
	///
	/// \param romStart		An iterator pointing to the start of the ROM data
	/// \param romEnd		An iterator pointing to the end of the ROM data
	/// \param out			An iterator to the data to write to
	/// \param includeEndingSpaces	If not set, then ending spaces will be trimmed.
	///
	/// \return The game's title from the ROM header
	///
	/// \throws DataNotFoundException The ROM was too small and didn't contain a title (!?).
	/// \throws DataNotFoundException The ROM was too small to determine mapping, meaning it's most likely corrupted.
	///
	////////////////////////////////////////////////////////////
	template <typename inputIteratorType, typename outputIteratorType> outputIteratorType getROMTitle(inputIteratorType romStart, inputIteratorType romEnd, outputIteratorType out, bool includeEndingSpaces);

	////////////////////////////////////////////////////////////
	/// \ingroup SFC
	/// \brief Returns true if the ROM is headered.
	///
	/// \param dataStart		An iterator pointing to the start of the raw data that may or may not be headered.
	/// \param dataEnd		An iterator pointing to the end of the raw data
	///
	/// \return True if the ROM is headered, false otherwise.
	///
	/// \throws ROMHeaderednessAmbiguityExcpetion The ROM's size is not divisible by 0x8000, or the ROM's size minus 0x200 is not divisible by 0x8000.
	///
	////////////////////////////////////////////////////////////
	template <typename inputIteratorType> bool isROMHeadered(inputIteratorType dataStart, inputIteratorType dataEnd);


	////////////////////////////////////////////////////////////
	/// \ingroup SFC
	/// \brief Returns the start of the actual ROM data accounting for headers.
	/// \details In other words, if the ROM is headered, this returns dataStart + 0x200.  Otherwise, just dataStart.
	///
	/// \param dataStart		An iterator pointing to the start of the raw data that may or may not be headered.
	/// \param dataEnd		An iterator pointing to the end of the raw data
	///
	/// \return The start of the actual ROM data.
	///
	/// \throws ROMHeaderednessAmbiguityExcpetion The ROM's size is not divisible by 0x8000, or the ROM's size minus 0x200 is not divisible by 0x8000.
	///
	////////////////////////////////////////////////////////////
	template <typename inputIteratorType> inputIteratorType getROMStart(inputIteratorType dataStart, inputIteratorType dataEnd);

//////////////////////////////////////////////////////////////////////////////
///  @}
//////////////////////////////////////////////////////////////////////////////
}

#include "SFC.inl"
