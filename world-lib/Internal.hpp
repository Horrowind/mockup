#pragma once

//////////////////////////////////////////////////////////////////////////////
/// \file Internal.hpp
/// \brief Contains some internal routines and constants
///
/// \addtogroup Internal
///  @{
//////////////////////////////////////////////////////////////////////////////

namespace worldlib
{
	namespace internal
	{
	

		////////////////////////////////////////////////////////////
		/// \brief Returns integer but with a mask applied and shifted right such that there are no 0s to the right of the result.  For example, 0xF8 with a mask of 0xF0 would return 0xF.
		///
		/// \param integer		The integer whose bits you want to get
		/// \param mask			The bits to get
		///
		/// \return The bits of the integer right shifted as far as the mask will allow
		///
		////////////////////////////////////////////////////////////
		template <typename integerType, typename maskType> integerType getBits(integerType integer, maskType mask);



		////////////////////////////////////////////////////////////
		/// \brief Returns the specified header byte from the specified level.
		///
		/// \param romStart		An iterator pointing to the start of the ROM data
		/// \param romEnd		An iterator pointing to the end of the ROM data
		/// \param level		The level to get the header byte from
		/// \param byteNumber		Which header byte to get
		///
		/// \return The level's header byte
		///
		/// \throws std::runtime_error If the ROM did not contain this data (e.g. via invalid pointers or the ROM being cut-off partway through level data or something else weird like that)
		///
		////////////////////////////////////////////////////////////
		template <typename inputIteratorType> std::uint8_t getLevelHeaderByte(inputIteratorType romStart, inputIteratorType romEnd, int level, int byteNumber);

		inline std::uint8_t reverseBits(std::uint8_t byte);
		inline std::uint16_t reverseBits(std::uint16_t value);
		inline std::uint32_t reverseBits(std::uint32_t value);
		inline std::uint64_t reverseBits(std::uint64_t value);


		inline std::uint64_t explodeInt(std::uint32_t i);
		inline std::uint32_t explodeInt(std::uint16_t i);
		inline std::uint16_t explodeInt(std::uint8_t i);

		inline std::uint64_t getBit(std::uint64_t number, int bit);
		inline std::uint32_t getBit(std::uint32_t number, int bit);
		inline std::uint16_t getBit(std::uint16_t number, int bit);
		inline std::uint8_t getBit(std::uint8_t number, int bit);
	}


}

#include "Internal.inl"

//////////////////////////////////////////////////////////////////////////////
///  @}
//////////////////////////////////////////////////////////////////////////////