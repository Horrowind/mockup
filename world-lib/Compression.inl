
#include <exception>
#include <iterator>
#include <limits>
#include <cstdint>
#include <algorithm>
#include "Internal.hpp"

//#define _SFCLIB_INTEGER_ITERATOR_ASSERT(T) static_assert(std::numeric_limits<std::iterator_traits<T>::value_type>::is_integer == true, "The iterator type must have a value_type that is an integer.  8-bit integers recommended.")

namespace worldlib
{

	namespace internal
	{

		////////////////////////////////////////////////////////////
		/// \brief Returns the next byte in the compressed file.
		/// \details This function automatically checks if the end of the file has been passed and throws if that is the case.
		///
		/// \param start		An iterator pointing to where we're at in the data.
		/// \param end			An iterator pointing to the end of the compressed data or any valid point after that (for example, the end of the ROM).
		/// \param compressedSize	A pointer to an int containing a running total of the compressed size.  Will be incremented if not nullptr.
		///
		/// \return The next byte of the sequence
		///
		/// \throws CompressedDataTooSmallException The end of the ROM was reached.
		///
		////////////////////////////////////////////////////////////
		template <typename inputIteratorType>
		inline auto getCompressionByte(inputIteratorType &start, inputIteratorType end, int *compressedSize) -> decltype(*start)
		{
			if (start >= end) throw CompressedDataTooSmallException();

			if (compressedSize != nullptr) *compressedSize += 1;

			return *(start++);
		}
	}

template <typename inputIteratorType, typename outputIteratorType>
outputIteratorType decompressLZ2(inputIteratorType start, inputIteratorType end, outputIteratorType out, int *compressedSize, int *decompressedSize)
{
	//_SFCLIB_INTEGER_ITERATOR_ASSERT(inputIteratorType);

	enum CommandType
	{
		DirectCopy = 0,
		ByteFill = 1,
		WordFill = 2,
		IncreasingFill = 3,
		Repeat = 4,
		Unused1 = 5,
		Unused2 = 6,
		LongCommand = 7
	};
	
	// Used internally.  On success out is modified.
	std::vector<typename std::iterator_traits<inputIteratorType>::value_type> resultBuffer;


	if (compressedSize != nullptr)
		*compressedSize = 0;


	while (start != end)
	{
		if (decompressedSize != nullptr)
			*decompressedSize = resultBuffer.size();

		int runLength;
		auto headerByte = internal::getCompressionByte(start, end, compressedSize);
		if (headerByte == 0xFF) break;
		CommandType commandType = static_cast<CommandType>((headerByte & 0xE0) >> 5);

		if (commandType == LongCommand)
		{
			auto secondHeaderByte = internal::getCompressionByte(start, end, compressedSize);
			commandType = static_cast<CommandType>((headerByte & 0x1C) >> 2);
			runLength = ((headerByte & 0x3) << 8) | secondHeaderByte;
		}
		else
		{
			runLength = headerByte & 0x1F;
		}

		if (commandType == DirectCopy)					// The data for this chunk is uncompressed
		{
			while (runLength >= 0)
			{
				auto byte = internal::getCompressionByte(start, end, compressedSize);
				resultBuffer.push_back(byte);
				runLength--;
			}
		}
		else if (commandType == ByteFill)				// The data for this chunk is one stream of one byte
		{
			auto fillByte = internal::getCompressionByte(start, end, compressedSize);
			while (runLength >= 0)
			{
				resultBuffer.push_back(fillByte);
				runLength--;
			}
		}
		else if (commandType == WordFill)				// The data for this chunk is one stream of two alternating bytes.
		{
			auto fillByte1 = internal::getCompressionByte(start, end, compressedSize);
			auto fillByte2 = internal::getCompressionByte(start, end, compressedSize);
			bool useFirst = true;
			while (runLength >= 0)
			{
				if (useFirst)
					resultBuffer.push_back(fillByte1);
				else
					resultBuffer.push_back(fillByte2);
				useFirst = !useFirst;
				runLength--;
			}

		}
		else if (commandType == IncreasingFill)				// The data for this chunk is one byte increasing in value
		{
			auto fillByte = internal::getCompressionByte(start, end, compressedSize);
			while (runLength >= 0)
			{
				resultBuffer.push_back(fillByte);
				fillByte++;
				fillByte &= 0xFF;
				runLength--;
			}
		}
		else if (commandType == Repeat)					// The data for this chunk copies previously written data
		{
			int offset = (internal::getCompressionByte(start, end, compressedSize) << 0x8) | (internal::getCompressionByte(start, end, compressedSize));	// Big endian, for some reason...
			int i = 0;

			while (runLength >= 0)
			{
				resultBuffer.push_back(resultBuffer[offset + i]);
				i++;
				runLength--;
			}
		}
		else
		{
			throw UnrecognizedCompressionCode();
		}
	}

	for (unsigned int i = 0; i < resultBuffer.size(); i++)
		*(out++) = resultBuffer[i];

	return out;
}



template <typename inputIteratorType, typename outputIteratorType>
outputIteratorType decompressLZ3(inputIteratorType start, inputIteratorType end, outputIteratorType out, int *compressedSize, int *decompressedSize)
{
	//_SFCLIB_INTEGER_ITERATOR_ASSERT(inputIteratorType);

	enum CommandType
	{
		DirectCopy = 0,
		ByteFill = 1,
		WordFill = 2,
		ZeroFill = 3,
		Repeat = 4,
		BitReverseRepeat = 5,
		BackwardsRepeat = 6,
		LongCommand = 7
	};

	// Used internally.  On success out is modified.
	std::vector<typename std::iterator_traits<inputIteratorType>::value_type> resultBuffer;

	if (compressedSize != nullptr)
		*compressedSize = 0;


	while (start != end)
	{
		int runLength;
		auto headerByte = internal::getCompressionByte(start, end, compressedSize);
		if (headerByte == 0xFF) break;
		CommandType commandType = static_cast<CommandType>((headerByte & 0xE0) >> 5);

		if (commandType == LongCommand)
		{
			auto secondHeaderByte = internal::getCompressionByte(start, end, compressedSize);
			commandType = static_cast<CommandType>((headerByte & 0x1C) >> 2);
			runLength = ((headerByte & 0x3) << 8) | secondHeaderByte;
		}
		else
		{
			runLength = headerByte & 0x1F;
		}

		if (commandType == DirectCopy)					// The data for this chunk is uncompressed
		{
			while (runLength >= 0)
			{
				resultBuffer.push_back(internal::getCompressionByte(start, end, compressedSize));
				runLength--;
			}
		}
		else if (commandType == ByteFill)				// The data for this chunk is one stream of one byte
		{
			auto fillByte = internal::getCompressionByte(start, end, compressedSize);
			while (runLength >= 0)
			{
				resultBuffer.push_back(fillByte);
				runLength--;
			}
		}
		else if (commandType == WordFill)				// The data for this chunk is one stream of two alternating bytes.
		{
			auto fillByte1 = internal::getCompressionByte(start, end, compressedSize);
			auto fillByte2 = internal::getCompressionByte(start, end, compressedSize);
			bool useFirst = true;
			while (runLength >= 0)
			{
				if (useFirst)
					resultBuffer.push_back(fillByte1);
				else
					resultBuffer.push_back(fillByte2);
				useFirst = !useFirst;
				runLength--;
			}

		}
		else if (commandType == ZeroFill)				// The data for this chunk a string of zeros
		{
			auto fillByte = 0;
			while (runLength >= 0)
			{
				resultBuffer.push_back(fillByte);
				runLength--;
			}
		}
		else if (commandType == Repeat || commandType == BitReverseRepeat || commandType == BackwardsRepeat)			// The data for this chunk copies previously written data
		{
			int offset = 0;
			int incrementDirection = 1;

			auto firstByte = internal::getCompressionByte(start, end, compressedSize);

			// The offset is either relative to the current buffer position or a fixed point from the beginning.
			if ((firstByte & 0x80) == 0x80) 
				offset = resultBuffer.size() - (firstByte & 0x7F) - 1;
			else
				offset = (firstByte & 0x7F) * 0x100 + internal::getCompressionByte(start, end, compressedSize);

			if (commandType == Repeat || commandType == BitReverseRepeat)
				incrementDirection = 1;
			else if (commandType == BackwardsRepeat) 
				incrementDirection = -1;

			bool reverseBits = commandType == BitReverseRepeat;

			int i = 0;
			while (runLength >= 0)
			{
				auto byte = resultBuffer[offset + i];
				if (reverseBits) 
					byte = internal::reverseBits(byte);
				resultBuffer.push_back(byte);
				i += incrementDirection;
				runLength--;
			}
		}
		else
		{
			throw UnrecognizedCompressionCode();
		}
	}

	for (unsigned int i = 0; i < resultBuffer.size(); i++)
		*(out++) = resultBuffer[i];

	if (decompressedSize != nullptr)
		*decompressedSize = resultBuffer.size();

	return out;
}

template <typename romIteratorType>
int getROMCompressionType(romIteratorType romStart, romIteratorType romEnd)
{
	return readByteSFC(romStart, romEnd, internal::decompressionTypeLocation);
}

template <typename romIteratorType, typename inputIteratorType, typename outputIteratorType>
outputIteratorType decompressData(romIteratorType romStart, romIteratorType romEnd, inputIteratorType compressedDataStart, inputIteratorType compressedDataEnd, outputIteratorType out, int *compressedSize, int *decompressedSize)
{

	auto compressionType = internal::decompressionTypeLocation > (romStart - romEnd)
		? 0 
		: readByteSFC(romStart, romEnd, internal::decompressionTypeLocation);
	if (compressionType == 0 || compressionType == 1)
		return decompressLZ2(compressedDataStart, compressedDataEnd, out, compressedSize, decompressedSize);
	else if (compressionType == 2)
		return decompressLZ3(compressedDataStart, compressedDataEnd, out, compressedSize, decompressedSize);
	else
       	throw UnrecognizedCompressionFormat();
}

#ifndef WORLDLIB_IGNORE_DLL_FUNCTIONS
namespace internal
{
	template <typename inputIteratorType, typename outputIteratorType>
	outputIteratorType compressGeneral(inputIteratorType rawDataStart, inputIteratorType rawDataEnd, outputIteratorType out, int compressionType, int *compressedSize)
	{
		if (LunarLoadDLL() == false) throw DLLLoadException("Lunar Compress.dll");

		char *raw = (char *)std::malloc(0x10000);
		auto size = LunarRecompress(&*rawDataStart, raw, std::distance(rawDataStart, rawDataEnd), 0x10000, compressionType, 0);

		if (size == 0) throw DataCompressionFailureException();

		for (unsigned int i = 0; i < size; i++)*(out++) = raw[i];

		if (compressedSize != nullptr) *compressedSize = size;

		std::free(raw);

		LunarUnloadDLL();

		return out;
	}
}

template <typename inputIteratorType, typename outputIteratorType>
outputIteratorType compressLZ2(inputIteratorType rawDataStart, inputIteratorType rawDataEnd, outputIteratorType out, int *compressedSize)
{
	return internal::compressGeneral(rawDataStart, rawDataEnd, out, LC_LZ2, compressedSize);
}

template <typename inputIteratorType, typename outputIteratorType>
outputIteratorType compressLZ3(inputIteratorType rawDataStart, inputIteratorType rawDataEnd, outputIteratorType out, int *compressedSize)
{
	return internal::compressGeneral(rawDataStart, rawDataEnd, out, LC_LZ3, compressedSize);
}

template <typename romIteratorType, typename inputIteratorType, typename outputIteratorType>
outputIteratorType compressData(romIteratorType romStart, romIteratorType romEnd, inputIteratorType rawDataStart, inputIteratorType rawDataEnd, outputIteratorType out, int *compressedSize)
{
	auto compressionType = readByteSFC(romStart, romEnd, internal::decompressionTypeLocation);

	if (compressionType == 0 || compressionType == 1)
		return compressLZ2(rawDataStart, rawDataEnd, out, compressedSize);
	else if (compressionType == 2)
		return compressLZ3(rawDataStart, rawDataEnd, out, compressedSize);
	else
		throw UnrecognizedCompressionFormat();
}

#endif

}
