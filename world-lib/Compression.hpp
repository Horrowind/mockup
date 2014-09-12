#pragma once

#include <vector>

#ifndef WORLDLIB_IGNORE_DLL_FUNCTIONS
#include <Windows.h>
#include "../DLLcode/LunarDLL.h"
#undef max
#undef min
#endif

#include "Exceptions.hpp"

namespace worldlib
{


//////////////////////////////////////////////////////////////////////////////
/// \file Compression.hpp
/// \brief Contains various functions for decompressing data
///
/// \addtogroup Decompression
///  @{
//////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////
	/// \brief Decompresses data compressed in the LZ2 format.  
	/// \details In general you'll want the functions in level.hpp related to getting graphics files instead, but this may be useful if you have your own data compressed like this.
	///
	/// \param compressedDataStart	An iterator pointing to the beginning of the compressed data
	/// \param compressedDataEnd	An iterator pointing to the end of the compressed data or any valid point after that (for example, the end of the ROM).
	/// \param out			Where to output the data
	/// \param compressedSize	Will contain the size of the compressed data after the function ends if it is not nullptr
	/// \param decompressedSize	Will contain the size of the decompressed data after the function ends if it is not nullptr
	///
	/// \return Iterator pointing to the end of your decompressed data
	///
	/// \throws CompressedDataTooSmallException The ROM ended partway through the decompression sequence
	/// \throws UnrecognizedCompressionCode An unrecognized bit sequence was encountered.
	///
	/// \see decompressGraphicsFile, decompressData
	///
	////////////////////////////////////////////////////////////
	template <typename inputIteratorType, typename outputIteratorType>
	outputIteratorType decompressLZ2(inputIteratorType compressedDataStart, inputIteratorType compressedDataEnd, outputIteratorType out, int *compressedSize = nullptr, int *decompressedSize = nullptr);

	////////////////////////////////////////////////////////////
	/// \brief Decompresses data compressed in the LZ3 format.  
	/// \details In general you'll want the functions in level.hpp related to getting graphics files instead, but this may be useful if you have your own data compressed like this.
	///
	/// \param compressedDataStart	An iterator pointing to the beginning of the compressed data
	/// \param compressedDataEnd	An iterator pointing to the end of the compressed data or any valid point after that (for example, the end of the ROM).
	/// \param out			Where to output the data
	/// \param compressedSize	Will contain the size of the compressed data after the function ends if it is not nullptr
	/// \param decompressedSize	Will contain the size of the decompressed data after the function ends if it is not nullptr
	///
	/// \return Iterator pointing to the end of your decompressed data
	///
	/// \throws CompressedDataTooSmallException The ROM ended partway through the decompression sequence
	/// \throws UnrecognizedCompressionCode An unrecognized bit sequence was encountered.
	///
	/// \see decompressGraphicsFile, decompressData
	///
	////////////////////////////////////////////////////////////
	template <typename inputIteratorType, typename outputIteratorType>
	outputIteratorType decompressLZ3(inputIteratorType compressedDataStart, inputIteratorType compressedDataEnd, outputIteratorType out, int *compressedSize = nullptr, int *decompressedSize = nullptr);

	////////////////////////////////////////////////////////////
	/// \brief Decompresses data compressed in the format the ROM uses 
	///
	/// \param romStart		An iterator pointing to the start of your ROM data
	/// \param romEnd		An iterator pointing to the end of your ROM data
	/// \param compressedDataStart	An iterator pointing to the beginning of the compressed data
	/// \param compressedDataEnd	An iterator pointing to the end of the compressed data or any valid point after that (for example, the end of the ROM).
	/// \param out			Where to output the data
	/// \param compressedSize	Will contain the size of the compressed data after the function ends if it is not nullptr
	/// \param decompressedSize	Will contain the size of the decompressed data after the function ends if it is not nullptr
	///
	/// \return Iterator pointing to the end of your decompressed data
	///
	/// \throws DataNotFoundException The ROM was too small and did not contain the byte that determines the compression format.
	/// \throws DataNotFoundException The ROM was too small to determine mapping, meaning it's most likely corrupted.
	/// \throws CompressedDataTooSmallException The ROM ended partway through the decompression sequence.
	/// \throws UnrecognizedCompressionCode An unrecognized bit sequence was encountered.
	///
	/// \see decompressGraphicsFile
	///
	////////////////////////////////////////////////////////////
	template <typename romIteratorType, typename inputIteratorType, typename outputIteratorType>
	outputIteratorType decompressData(romIteratorType romStart, romIteratorType romEnd, inputIteratorType compressedDataStart, inputIteratorType compressedDataEnd, outputIteratorType out, int *compressedSize = nullptr, int *decompressedSize = nullptr);
	
	////////////////////////////////////////////////////////////
	/// \brief Gets the compression scheme used by the ROM
	///
	/// \param romStart		An iterator pointing to the start of your ROM data
	/// \param romEnd		An iterator pointing to the end of your ROM data
	///
	/// \return 0 for the original game code, 1 for speed optimizations, 2 for compression optimizations.
	///
	/// \throws DataNotFoundException The ROM was too small and did not contain the byte that determines the compression format.
	/// \throws DataNotFoundException The ROM was too small to determine mapping, meaning it's most likely corrupted.
	///
	/// \see decompressGraphicsFile
	///
	////////////////////////////////////////////////////////////
	template <typename romIteratorType>
	int getROMCompressionType(romIteratorType romStart, romIteratorType romEnd);


	// Compression functions all rely on the Lunar Compress DLL
	#ifndef WORLDLIB_IGNORE_DLL_FUNCTIONS

	namespace internal
	{
		////////////////////////////////////////////////////////////
		/// \brief Compresses data compressed in any format compatible with Lunar Compress
		///
		/// \param rawDataStart		An iterator pointing to the beginning of the raw data to compress
		/// \param rawDataEnd		An iterator pointing to the end of the raw data to compress
		/// \param out			Where to output the compressed data
		/// \param compressionType	The compression type. See LunarDLL.h
		/// \param compressedSize	Will contain the size of the compressed data after the function ends if it is not nullptr
		///
		/// \return Iterator pointing to the end of your compressed data
		///
		/// \throws DLLLoadException Lunar Compress.dll could not be found, or was invalid.
		/// \throws CompressedDataException Lunar Compress.dll could not compress the data.
		///
		////////////////////////////////////////////////////////////
		template <typename inputIteratorType, typename outputIteratorType>
		outputIteratorType compressGeneral(inputIteratorType rawDataStart, inputIteratorType rawDataEnd, outputIteratorType out, int compressionType, int *compressedSize);
	}

	////////////////////////////////////////////////////////////
	/// \brief Compresses data compressed in the LZ2 format.
	/// \details Please note that this function requires Lunar Compress.dll.  See also compressData, which takes into account the ROM's current compression type.
	///
	/// \param rawDataStart		An iterator pointing to the beginning of the raw data to compress
	/// \param rawDataEnd		An iterator pointing to the end of the raw data to compress
	/// \param out			Where to output the compressed data
	/// \param compressedSize	Will contain the size of the compressed data after the function ends if it is not nullptr
	///
	/// \return Iterator pointing to the end of your compressed data
	///
	/// \throws DLLLoadException Lunar Compress.dll could not be found, or was invalid.
	/// \throws CompressedDataException Lunar Compress.dll could not compress the data.
	///
	/// \see compressData
	///
	////////////////////////////////////////////////////////////
	template <typename inputIteratorType, typename outputIteratorType>
	outputIteratorType compressLZ2(inputIteratorType rawDataStart, inputIteratorType rawDataEnd, outputIteratorType out, int *compressedSize);

	////////////////////////////////////////////////////////////
	/// \brief Compresses data compressed in the LZ3 format.
	/// \details Please note that this function requires Lunar Compress.dll.  See also compressData, which takes into account the ROM's current compression type.
	///
	/// \param rawDataStart		An iterator pointing to the beginning of the raw data to compress
	/// \param rawDataEnd		An iterator pointing to the end of the raw data to compress
	/// \param out			Where to output the compressed data
	/// \param compressedSize	Will contain the size of the compressed data after the function ends if it is not nullptr
	///
	/// \return Iterator pointing to the end of your compressed data
	///
	/// \throws DLLLoadException Lunar Compress.dll could not be found, or was invalid.
	/// \throws CompressedDataException Lunar Compress.dll could not compress the data.
	///
	/// \see compressData
	///
	////////////////////////////////////////////////////////////
	template <typename inputIteratorType, typename outputIteratorType>
	outputIteratorType compressLZ3(inputIteratorType rawDataStart, inputIteratorType rawDataEnd, outputIteratorType out, int *compressedSize);

	////////////////////////////////////////////////////////////
	/// \brief Compresses data compressed in the compression format your ROM uses format.
	/// \details Please note that this function requires Lunar Compress.dll.  See also compressData, which takes into account the ROM's current compression type.
	///
	/// \param romStart		An iterator pointing to the start of your ROM data
	/// \param romEnd		An iterator pointing to the end of your ROM data
	/// \param rawDataStart		An iterator pointing to the beginning of the raw data to compress
	/// \param rawDataEnd		An iterator pointing to the end of the raw data to compress
	/// \param out			Where to output the compressed data
	/// \param compressedSize	Will contain the size of the compressed data after the function ends if it is not nullptr
	///
	/// \return Iterator pointing to the end of your compressed data
	///
	/// \throws DataNotFoundException The ROM was too small and did not contain the byte that determines the compression format.
	/// \throws DataNotFoundException The ROM was too small to determine mapping, meaning it's most likely corrupted.
	/// \throws DLLLoadException Lunar Compress.dll could not be found, or was invalid.
	/// \throws CompressedDataException Lunar Compress.dll could not compress the data.
	///
	////////////////////////////////////////////////////////////
	template <typename romIteratorType, typename inputIteratorType, typename outputIteratorType>
	outputIteratorType compressData(romIteratorType romStart, romIteratorType romEnd, inputIteratorType rawDataStart, inputIteratorType rawDataEnd, outputIteratorType out, int *compressedSize);


	#endif


//////////////////////////////////////////////////////////////////////////////
///  @}
//////////////////////////////////////////////////////////////////////////////
}

#include "Compression.inl"