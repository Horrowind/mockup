#pragma once
#include <exception>
#include <stdexcept>

//////////////////////////////////////////////////////////////////////////////
/// \file Exceptions.hpp
/// \brief Contains the exceptions thrown by the world-lib functions
/// \details Note that most exceptions inherit from std::runtime_exception, for those who gotta catch 'em all.
///
/// \addtogroup Exceptions
///  @{
//////////////////////////////////////////////////////////////////////////////

namespace worldlib
{
	class DataNotFoundException : public std::runtime_error
	{
	public:
		enum Reason
		{
			ROMTooSmall,				// The pointer to the data points outside of the ROM, or else the data is cut off early by a truncated ROM.
			NullPointer,				// The pointer is null (0 or FFFFFF)
			InvalidAssumption			// Tried to request data that had no reason to exist (such as requesting the custom palette of a level that doesn't use one)
		};

		Reason reason;
		DataNotFoundException(Reason reason) : std::runtime_error("This data was invalid for some reason. See the reason value for an explanation.") { this->reason = reason; }
	};

	class CompressedDataException : public std::runtime_error
	{
	public:
		/*enum Reason
		{
			UnrecognizedBitSequence,		// The compressed data was invalid or corrupt
			UnrecognizedFormat,			// The ROM uses a compression scheme that's unrecognized
			DataTooSmall,				// The data ended earlier than expected
			LunarDLLMissingOrInvalid,		// Lunar DLL could not be found or was invalid
			LunarDLLFailure				// Lunar DLL failed to compress the data
		};

		Reason reason;*/

		CompressedDataException(const char *what) : std::runtime_error(what) { ; }
		//CompressedDataException(Reason reason) : std::runtime_error("This data could not be compressed/decompressed for some reason. See the reason value for an explanation.") { this->reason = reason; }
	};

	class UnrecognizedCompressionCode : public CompressedDataException
	{
	public: UnrecognizedCompressionCode() : CompressedDataException("An unrecognized bit sequence was encountered--the compressed data may be corrupt or otherwise invalid.") { ; }
	};

	class UnrecognizedCompressionFormat : public CompressedDataException
	{
	public: UnrecognizedCompressionFormat() : CompressedDataException("This ROM uses a compression scheme that's unrecognized.") { ; }
	};

	class CompressedDataTooSmallException : public CompressedDataException
	{
	public: CompressedDataTooSmallException() : CompressedDataException("The compressed data ended earlier than expected--it may be corrupted.") { ; };
	};

	class DataCompressionFailureException : public CompressedDataException
	{
	public: DataCompressionFailureException() : CompressedDataException("Lunar Compress failed to compress the data for an unknown reason.") { ; };
	};

	class DLLLoadException : public std::runtime_error
	{
	public:
		const char *DLLName;
		DLLLoadException(const char *name) : std::runtime_error("The requested DLL could not be properly loaded, or was otherwise invalid.") { DLLName = name; }
	};


	class IndexedGraphicsException : public std::runtime_error
	{
	public: IndexedGraphicsException() : std::runtime_error("The indexed graphics could not be properly convereted to a bitmap.") { ; }
	};

	class PatchException : public std::runtime_error 
	{
	public: PatchException(const char *what) : std::runtime_error(what) { ; }
	};

	class PatchDefinitionMissingException : public PatchException { public: PatchDefinitionMissingException() : PatchException("The definition could not be found in the patch.") { ; } };
	class PatchDefinitionMalformedException : public PatchException { public: PatchDefinitionMalformedException() : PatchException("The definition was malformed.") { ; } };
	class PatchDefinitionSizeMismatch : public PatchException { public: PatchDefinitionSizeMismatch() : PatchException("The length of the definition's definition was not the same length as the replacement string.") { ; } };
	class PatchLabelMissingException : public PatchException { public: PatchLabelMissingException() : PatchException("The label could not be found in the patch.") { ; } };

	class InvalidAddressException : public std::runtime_error { public: InvalidAddressException() : std::runtime_error("The address was invalid, or could not be converted into a valid address") { ; } };

	class ROMHeaderednessAmbiguityExcpetion : public std::runtime_error { public: ROMHeaderednessAmbiguityExcpetion() : std::runtime_error("Could not determine if the ROM is headered or not.") { ; } };

	class UberASMCodeMissing : public std::runtime_error { public: UberASMCodeMissing() : std::runtime_error("Could not find the uberASM code for this level.") { ; } };

	class TableEntryTypeMismatchException : public PatchException { public: TableEntryTypeMismatchException() : PatchException("The requested type did not match the entry's type.") { ; } };

	class TableEntryMissingException : public PatchException { public: TableEntryMissingException() : PatchException("The requested table entry does not exist.") { ; } };
}

//////////////////////////////////////////////////////////////////////////////
///  @}
//////////////////////////////////////////////////////////////////////////////