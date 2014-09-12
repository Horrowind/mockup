#pragma once

#include <iterator>
#include <vector>
#include <iomanip>
#include <cstdint>
#include <bitset>


//#if !(defined _WORLDLIB_PATCH_HPP)
//#define _WORLDLIB_PATCH_HPP

// Patching functions all rely on the Asar DLL
#ifndef WORLDLIB_IGNORE_DLL_FUNCTIONS

namespace worldlib
{

	//////////////////////////////////////////////////////////////////////////////
	/// \file Patch.hpp
	/// \brief Contains various functions for patching and assembling files
	///
	/// \addtogroup Patch
	///  @{
	//////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////
	/// \brief Contains error data reported from Asar
	////////////////////////////////////////////////////////////
	template <typename stringType>
	struct PatchErrorData
	{

		////////////////////////////////////////////////////////////
		/// \brief Default constructor
		////////////////////////////////////////////////////////////
		PatchErrorData();


		////////////////////////////////////////////////////////////
		/// \brief Constructor taking in various information.  In retrospect I could've just taken in one of Asar's structs, couldn't've I?
		////////////////////////////////////////////////////////////
		PatchErrorData(const char *fed, const char *red, const char *b, const char *fn, int l, const char *cfn, int cl) : fullErrorData((std::string)fed), rawErrorData((std::string)red), block((std::string)b), fileName((std::string)fn), line(l), callerLine(cl)
		{
			if (cfn != nullptr) callerFilename = std::string(cfn);
		};

		////////////////////////////////////////////////////////////
		/// \brief Contains the full error data as reported by Asar
		////////////////////////////////////////////////////////////
		stringType fullErrorData;

		////////////////////////////////////////////////////////////
		/// \brief Contains just the "simple" error data as reported by Asar
		////////////////////////////////////////////////////////////
		stringType rawErrorData;

		////////////////////////////////////////////////////////////
		/// \brief Contains the "descriptive" error data as reported by Asar
		////////////////////////////////////////////////////////////
		stringType block;

		////////////////////////////////////////////////////////////
		/// \brief The file name of the offending file
		////////////////////////////////////////////////////////////
		stringType fileName;

		////////////////////////////////////////////////////////////
		/// \brief The line the error occurred at
		////////////////////////////////////////////////////////////
		int line;

		////////////////////////////////////////////////////////////
		/// \brief The calling file
		////////////////////////////////////////////////////////////
		stringType callerFilename;

		////////////////////////////////////////////////////////////
		/// \brief The line the file was called at in the calling file
		////////////////////////////////////////////////////////////
		int callerLine;
	};

	////////////////////////////////////////////////////////////
	/// \brief Contains warning data reported from Asar
	////////////////////////////////////////////////////////////
	template <typename stringType> using PatchWarningData = PatchErrorData<stringType>;

	////////////////////////////////////////////////////////////
	/// \brief Contains label data reported from Asar
	////////////////////////////////////////////////////////////
	template <typename stringType>
	struct PatchLabelData
	{
		////////////////////////////////////////////////////////////
		/// \brief Default constructor
		////////////////////////////////////////////////////////////
		PatchLabelData();

		////////////////////////////////////////////////////////////
		/// \brief Constructor taking in various information.  And what kind of a word is "couldn't've" anyway.
		////////////////////////////////////////////////////////////
		PatchLabelData(const char *n, int l) : name((std::string)n), location(l) {};

		////////////////////////////////////////////////////////////
		/// \brief The name of the label
		////////////////////////////////////////////////////////////
		stringType name;

		////////////////////////////////////////////////////////////
		/// \brief The location of the label
		////////////////////////////////////////////////////////////
		int location;
	};

	////////////////////////////////////////////////////////////
	/// \brief Contains define data reported from Asar
	////////////////////////////////////////////////////////////
	template <typename stringType>
	struct PatchDefineData
	{
		////////////////////////////////////////////////////////////
		/// \brief Default constructor
		////////////////////////////////////////////////////////////
		PatchDefineData();

		////////////////////////////////////////////////////////////
		/// \brief Constructor taking in various information.
		////////////////////////////////////////////////////////////
		PatchDefineData(const char *n, const char *c) : name((std::string)n), contents(c) {};

		////////////////////////////////////////////////////////////
		/// \brief The name of the define
		////////////////////////////////////////////////////////////
		stringType name;

		////////////////////////////////////////////////////////////
		/// \brief The literal string value of the define
		////////////////////////////////////////////////////////////
		stringType contents;
	};






	/////////////////////////////////////////////////////////////////////////////
	/// \brief Compiles the given ASM file to a binary.
	/// \details The file must exist on disk.  If errorDataOutput has been added to once the function returns, there was an error.  Use the PatchErrorData et al structs for the parameters.
	///
	/// \param patchFilepath	The filename of the patch
	/// \param out			Where to send the resulting data
	/// \param errorDataOutput	Where to send the error data
	/// \param warningDataOutput	Where to send the warning data
	/// \param labelDataOutput	Where to send the label data
	/// \param defineDataOutput	Where to send the define data
	///
	/// \return Returns an iterator to the end of your compiled data.
	///
	/////////////////////////////////////////////////////////////////////////////
	template <typename stringType, typename outputIteratorType, typename errorDataOutputIteratorType, typename warningDataOutputIteratorType, typename labelDataOutputIteratorType, typename defineDataOutputIteratorType>
	outputIteratorType compileToBinary(const char *patchFilepath, outputIteratorType out, errorDataOutputIteratorType &errorDataOutput, warningDataOutputIteratorType &warningDataOutput, labelDataOutputIteratorType &labelDataOutput, defineDataOutputIteratorType &defineDataOutput);

	/////////////////////////////////////////////////////////////////////////////
	/// \brief Compiles the given ASM file to a binary.
	///
	/// \details The file must exist on disk.  If errorDataOutput has been added to once the function returns, there was an error.  Use the PatchErrorData et al structs for the parameters.
	///
	/// \param patchFilepath	The filename of the patch
	/// \param out			Where to send the resulting data
	/// \param errorDataOutput	Where to send the error data
	///
	/// \return Returns an iterator to the end of your compiled data.
	///
	/////////////////////////////////////////////////////////////////////////////
	template <typename stringType, typename outputIteratorType, typename errorDataOutputIteratorType>
	outputIteratorType compileToBinary(const char *patchFilepath, outputIteratorType out, errorDataOutputIteratorType &errorDataOutput);

	/////////////////////////////////////////////////////////////////////////////
	/// \brief Compiles the given ASM file to a binary.  The file must exist on disk.  If errorDataOutput has been added to once the function returns, there was an error.  Use the PatchErrorData et al structs for the parameters.
	///
	/// \param patchFilepath	The filename of the patch
	/// \param out			Where to send the resulting data
	///
	/// \return Returns true on success and false on failure
	///
	/////////////////////////////////////////////////////////////////////////////
	template <typename stringType, typename outputIteratorType>
	bool compileToBinary(const char *patchFilepath, outputIteratorType &out);

	/////////////////////////////////////////////////////////////////////////////
	/// \brief "Quick compile".  Just gives you a vector from a string, basically.
	///
	/// \param patchFilepath	The filename of the patch
	/// \param errorDataOutput	Where to send the error data.  Be nice if this weren't necessary, but we need this for error reporting, obviously.
	///
	/// \return Returns a vector containing the compiled data
	///
	/////////////////////////////////////////////////////////////////////////////
	template <typename dataType>
	std::vector<dataType> compileToBinary(const char *patchFilepath, std::vector<PatchErrorData<std::string>> &errorDataOutput);








	/////////////////////////////////////////////////////////////////////////////
	/// \brief Compiles an ASM file onto a ROM.  The ROM must exist in memory as a pointer to chars or unsigned chars.  Or I guess you could use other integer types if you're feeling adventerous...
	///
	/// If errorDataOutput has been added to once the function returns, there was an error.  Use the PatchErrorData et al structs for the parameters.
	///
	/// \param patchFilepath	The filename of the patch
	/// \param romData		The ROM data to modify
	/// \param romSize		The size of the ROM to modify
	/// \param errorDataOutput	Where to send the error data
	/// \param warningDataOutput	Where to send the warning data
	/// \param labelDataOutput	Where to send the label data
	/// \param defineDataOutput	Where to send the define data
	///
	/// \return Returns true on success and false if there was an error
	///
	/////////////////////////////////////////////////////////////////////////////
	template <typename stringType, typename romDataType, typename errorDataOutputIteratorType, typename warningDataOutputIteratorType, typename labelDataOutputIteratorType, typename defineDataOutputIteratorType>
	bool patchToROM(const char *patchFilepath, const romDataType *romData, int romSize, errorDataOutputIteratorType &errorDataOutput, warningDataOutputIteratorType &warningDataOutput, labelDataOutputIteratorType &labelDataOutput, defineDataOutputIteratorType &defineDataOutput);


	/////////////////////////////////////////////////////////////////////////////
	/// \brief Compiles an ASM file onto a ROM.  The ROM must exist in memory as a pointer to chars or unsigned chars.  Or I guess you could use other integer types if you're feeling adventerous...
	///
	/// If errorDataOutput has been added to once the function returns, there was an error.  Use the PatchErrorData et al structs for the parameters.
	///
	/// \param patchFilepath	The filename of the patch
	/// \param romData		The ROM data to modify
	/// \param romSize		The size of the ROM to modify
	/// \param errorDataOutput	Where to send the error data
	///
	/// \return Returns true on success and false if there was an error
	///
	/////////////////////////////////////////////////////////////////////////////
	template <typename stringType, typename romDataType, typename errorDataOutputIteratorType>
	bool patchToROM(const char *patchFilepath, const romDataType *romData, int romSize, errorDataOutputIteratorType &errorDataOutput);
	
	/////////////////////////////////////////////////////////////////////////////
	/// \brief Compiles an ASM file onto a ROM.  The ROM must exist in memory as a pointer to chars or unsigned chars.  Or I guess you could use other integer types if you're feeling adventerous...
	///
	/// If errorDataOutput has been added to once the function returns, there was an error.  Use the PatchErrorData et al structs for the parameters.
	///
	/// \param patchFilepath	The filename of the patch
	/// \param romData		The ROM data to modify
	/// \param romSize		The size of the ROM to modify
	///
	/// \return Returns true on success and false if there was an error
	///
	/////////////////////////////////////////////////////////////////////////////
	template <typename stringType, typename romDataType>
	bool patchToROM(const char *patchFilepath, const romDataType *romData, int romSize);
	
	
	
	

	/////////////////////////////////////////////////////////////////////////////
	/// \brief Given a string to a patch, finds a definition and gives the location of its value.
	///
	/// \param patchStart			An iterator pointing to the patch's start
	/// \param patchEnd			An iterator pointing to the patch's end
	/// \param definitionStringStart	An iterator pointing to a string containing the name of the definition you want to replace, not including the "!"
	/// \param definitionStringEnd		An iterator pointing to the definition's end
	/// \param definitionValueStart		After calling, an iterator pointing to the start of the string containing the value of the definition.
	/// \param definitionValueEnd		After calling, an iterator pointing to the end of the string containing the value of the definition.
	///
	/// \throws PatchDefinitionMissingException The patch definition could not be found.
	/// \throws PatchDefinitionMalformedException The patch definition was found, but was malformed in some way.
	///
	/////////////////////////////////////////////////////////////////////////////
	template <typename patchStringIteratorType, typename definitionStringIteratorType, typename definitionValueIteratorType>
	void getPatchDefinitionValue(patchStringIteratorType patchStart, patchStringIteratorType patchEnd, definitionStringIteratorType definitionStringStart, definitionStringIteratorType definitionStringEnd, definitionValueIteratorType& definitionValueStart, definitionValueIteratorType& definitionValueEnd);

	
	
	
	/////////////////////////////////////////////////////////////////////////////
	/// \brief Given a string to a patch, finds a definition (e.g. "!true = $00") and replaces it with a new string. 
	/// \details It's highly recommended to look at the version that just takes normal strings instead of iterators so that this function doesn't take up like 5 lines of back_insert_iterator boilerplate.
	///
	/// \param patchStart		An iterator pointing to the patch's start
	/// \param patchEnd		An iterator pointing to the patch's end
	/// \param definitionStart	An iterator pointing to a string containing the name of the definition you want to replace, not including the "!"
	/// \param definitionEnd	An iterator pointing to the definition's end
	/// \param valueToChangeToStart	An iterator pointing to a string containing the name of the value you want to replace the original definition's value with
	/// \param valueToChangeToEnd	An iterator pointing to the value's end
	///
	/// \throws PatchDefinitionMissingException The patch definition could not be found.
	/// \throws PatchDefinitionMalformedException The patch definition was found, but was malformed in some way.
	/// \throws PatchDefinitionSizeMismatch The definition's value's length (in code points) is not equal to the length of the string you want to replace it with.  E.G. trying to replace "!value = $0000" with "$50" instead of "$0050"
	///
	/////////////////////////////////////////////////////////////////////////////
	template <typename patchStringIteratorType, typename definitionStringIteratorType, typename replacementStringIteratorType>
	void replacePatchDefinition(patchStringIteratorType patchStart, patchStringIteratorType patchEnd, definitionStringIteratorType definitionStart, definitionStringIteratorType definitionEnd, replacementStringIteratorType valueToChangeToStart, replacementStringIteratorType valueToChangeToEnd);

	/////////////////////////////////////////////////////////////////////////////
	/// \brief Given a string to a patch, finds a definition (e.g. "!true = $00") and replaces it with a number, printed as a hex value (with a $ preceding it). 
	/// \details It's highly recommended to look at the version that just takes normal strings instead of iterators so that this function doesn't take up like 5 lines of back_insert_iterator boilerplate.
	///
	/// \param patchStart		An iterator pointing to the patch's start
	/// \param patchEnd		An iterator pointing to the patch's end
	/// \param definitionStart	An iterator pointing to a string containing the name of the definition you want to replace, not including the "!"
	/// \param definitionEnd	An iterator pointing to the definition's end
	/// \param valueToChangeTo	The numeric value to use (will be converted to a hex value with a '$' preceding it)
	/// \param digits		How mny digits long the value should be (for example, use 4 to convert "$50" to "$0050")
	///
	/// \throws PatchDefinitionMissingException The patch definition could not be found.
	/// \throws PatchDefinitionMalformedException The patch definition was found, but was malformed in some way.
	/// \throws PatchDefinitionSizeMismatch The definition's value's length (in code points) is not equal to the length of the string you want to replace it with.  E.G. trying to replace "!value = $0000" with "$50" instead of "$0050"
	///
	/////////////////////////////////////////////////////////////////////////////
	template <typename patchStringIteratorType, typename definitionStringIteratorType>
	void replacePatchDefinition(patchStringIteratorType patchStart, patchStringIteratorType patchEnd, definitionStringIteratorType definitionStart, definitionStringIteratorType definitionEnd, int valueToChangeTo, int digits);

	/////////////////////////////////////////////////////////////////////////////
	/// \brief Given a string to a patch, finds a definition (e.g. "!true = $00") and replaces it with a new string.
	/// \details The length of the new definition's value must be equal to the length of the old definition's value
	///
	/// \param patch		The patch that should have its definition changed
	/// \param definitionToChange	A string containing the name of the definition you want to replace, not including the "!"
	/// \param valueToChangeTo	A string containing the name of the value you want to replace the original definition's value with
	///
	/// \throws PatchDefinitionMissingException The patch definition could not be found.
	/// \throws PatchDefinitionMalformedException The patch definition was found, but was malformed in some way.
	/// \throws PatchDefinitionSizeMismatch The definition's value's length (in code points) is not equal to the length of the string you want to replace it with.  E.G. trying to replace "!value = $0000" with "$50" instead of "$0050"
	///
	/////////////////////////////////////////////////////////////////////////////
	template <typename patchStringType, typename definitionStringType, typename replacementStringType>
	void replacePatchDefinition(patchStringType &patch, const definitionStringType &definitionToChange, const replacementStringType &valueToChangeTo);

	/////////////////////////////////////////////////////////////////////////////
	/// \brief Given a string to a patch, finds a definition (e.g. "!true = $00") and replaces it with a number
	/// \details The number is printed as a hex value (with a $ preceding it).
	///
	/// \param patch		The patch that should have its definition changed
	/// \param definitionToChange	A string containing the name of the definition you want to replace, not including the "!"
	/// \param valueToChangeTo	The numeric value to use (will be converted to a hex value with a '$' preceding it)
	/// \param digits		How mny digits long the value should be (for example, use 4 to convert "$50" to "$0050")
	///
	/// \throws PatchDefinitionMissingException The patch definition could not be found.
	/// \throws PatchDefinitionMalformedException The patch definition was found, but was malformed in some way.
	/// \throws PatchDefinitionSizeMismatch The definition's value's length (in code points) is not equal to the length of the string you want to replace it with.  E.G. trying to replace "!value = $0000" with "$50" instead of "$0050"
	///
	/////////////////////////////////////////////////////////////////////////////
	template <typename patchStringType, typename definitionStringType>
	void replacePatchDefinition(patchStringType &patch, const definitionStringType &definitionToChange, int valueToChangeTo, int digits);

	template <typename StringCharType>
	class Table;

	/////////////////////////////////////////////////////////////////////////////
	/// \brief Replaces a patch's table
	/// \details Given a string to a patch and a table, replaces the patch's table with the new table
	///
	/// \param patch		The patch that should have its table changed
	/// \param table		The table to use
	/// \param reparse		Set to true if the patch's string has been modified since the table was created.  If it has been modified and this is left as false, the table may overwrite the wrong part of the string, or you may segfault from trying to use invalidated iterators!
	///
	/// \throws PatchLabelMissingException The table's label could not be found.
	///
	/////////////////////////////////////////////////////////////////////////////

	template <typename StringCharType>
	void replacePatchTable(std::basic_string<StringCharType> &patch, Table<StringCharType> &table, bool reparse);

	/////////////////////////////////////////////////////////////////////////////
	/// \brief Replaces a patch's table
	/// \details Given a string to a patch and a table, replaces the patch's table with the new table
	///
	/// \param patch		The patch that should have its table changed
	/// \param table		The table to use
	///
	/// \throws PatchLabelMissingException The table's label could not be found.
	///
	/////////////////////////////////////////////////////////////////////////////

	template <typename StringCharType>
	void replacePatchTable(std::basic_string<StringCharType> &patch, Table<StringCharType> &table);




	/////////////////////////////////////////////////////////////////////////////
	/// \brief Tests if a given string is a label.
	/// \details Probably not all that useful on its own; findLabel might be what you're looking for.
	///
	/// \param start		The start of the string you think might be a label
	/// \param end			The end of the patch
	///
	/// \returns			0 if the string is not a label, the length of the label otherwise.
	///
	/////////////////////////////////////////////////////////////////////////////
	template <typename stringIteratorType>
	inline int isLabel(stringIteratorType start, stringIteratorType end);

	/////////////////////////////////////////////////////////////////////////////
	/// \brief Tests if a given string is a label.
	/// \details Probably not all that useful on its own; findLabel might be what you're looking for.
	///
	/// \param str			The string to test
	///
	/// \returns			0 if the string is not a label, the length of the label otherwise.
	///
	/////////////////////////////////////////////////////////////////////////////
	template <typename stringType>
	inline int isLabel(const stringType &str);

	/////////////////////////////////////////////////////////////////////////////
	/// \brief Finds a label in an ASM file
	/// \details There is no version of this function that returns a string on its own because that is not often useful;
	///  you'll usually want to get the string *and* its position so that you can loop through a file searching for labels.
	///  Use this function in conjunction with getLabel for that purpose.
	///
	/// \param start		The start of wherever you want to search for a label
	/// \param end			The end of the patch
	/// \param skipThis		If true, then if start points to a label already, this label will be skipped and the next label, if one exists, will be returned.  Use this inside loops if you're looking for a specific label.
	///
	/// \returns			An iterator pointing to the label.  If the iterator is equal to end, then no label was found.
	///
	/////////////////////////////////////////////////////////////////////////////
	template <typename stringIteratorType>
	inline stringIteratorType findLabel(stringIteratorType start, stringIteratorType end, bool skipThis);


	/////////////////////////////////////////////////////////////////////////////
	/// \brief Finds a specific label in an ASM file
	/// \details There is no version of this function that returns a string on its own because that is not often useful;
	///  you'll usually want to get the string *and* its position so that you can loop through a file searching for labels.
	///  Use this function in conjunction with getLabel for that purpose.
	///
	/// \param start		The start of wherever you want to search for a label
	/// \param end			The end of the patch
	/// \param labelNameStart	The start of the name of the label you want to search for
	/// \param labelNameEnd		The end of the name of the label you want to search for
	///
	/// \returns			An iterator pointing to the label.  If the iterator is equal to end, then no label was found.
	///
	/////////////////////////////////////////////////////////////////////////////
	template <typename stringIteratorType>
	inline stringIteratorType findLabel(stringIteratorType start, stringIteratorType end, stringIteratorType labelNameStart, stringIteratorType labelNameEnd);


	/////////////////////////////////////////////////////////////////////////////
	/// \brief Finds a specific label in an ASM file
	/// \details Version of findLabel that works with strings
	///
	/// \param patchString		The entire patch
	/// \param tableNameString	The name of the table
	///
	/// \returns			An iterator pointing to the label.  If the iterator is equal to end, then no label was found.
	///
	/////////////////////////////////////////////////////////////////////////////
	template <typename StringCharType>
	inline typename std::basic_string<StringCharType>::iterator findLabel(std::basic_string<StringCharType> &patchString, std::basic_string<StringCharType> &tableNameString);

	

	/////////////////////////////////////////////////////////////////////////////
	/// \brief Gets the label at a position, if one exists
	/// \details Best combined with findLabel.  e.g. getLabel(findLabel(searchStart, fileEnd, false), fileEnd)
	///
	/// \param start		The start of the label string
	/// \param end			The end of the patch
	///
	/// \returns			A string containing the label, if one exists.  Otherwise, an empty string.
	///
	/////////////////////////////////////////////////////////////////////////////
	template <typename stringIteratorType>
	inline typename std::basic_string<typename std::iterator_traits<stringIteratorType>::value_type> getLabel(stringIteratorType start, stringIteratorType end)
	{
		typedef typename std::basic_string<typename std::iterator_traits<stringIteratorType>::value_type> stringType;
		int length = isLabel(start, end);
		if (length > 0)
		{
			return stringType(start, start + length);
		}
		else
			return stringType();
	}

	/////////////////////////////////////////////////////////////////////////////
	/// \brief Represents a row of values in a table
	/// \details The row of values is literally just a vector with an optional comment string "glued" onto it.
	///
	/////////////////////////////////////////////////////////////////////////////
	template <typename StringCharType>
	class TableRow
	{
		int intWidth;

	public:
		/////////////////////////////////////////////////////////////////////////////
		/// \brief Represents an entry of a row of a table.
		/// \details The entry can be a hex, decimal, or binary value, all lumped together under the "integer" type, or a string "identifier" type.
		///
		/////////////////////////////////////////////////////////////////////////////
		class TableEntry
		{
		public:	/////////////////////////////////////////////////////////////////////////////
			/// \brief The type of this entry
			/// \details Decimal, Hexadecimal, and Binary are all integers. Identifier is a string.  Unknown is just what the name suggests, and shouldn't come up.
			///
			/////////////////////////////////////////////////////////////////////////////
			enum class ValueType
			{
				Unknown,
				Decimal,
				Hexadecimal,
				Binary,
				Identifier
			};
		private:
			std::basic_string<StringCharType> raw;
			ValueType valueType;
			std::uint32_t intValue;
		public:
			/////////////////////////////////////////////////////////////////////////////
			/// \brief Default constructor
			/// \details Sets the value type to "unknown".  Under normal cirtumstances you shouldn't need this.
			///
			/////////////////////////////////////////////////////////////////////////////
			TableEntry() 
			{ 
				valueType = ValueType::Unknown; 
				intValue = 0; 
			}
			/////////////////////////////////////////////////////////////////////////////
			/// \brief Constructor for an entry whose value is known
			/// \details Under normal cirtumstances you shouldn't need this.
			///
			/// \param rawString		The raw string that resulted in this entry
			/// \param entryValueType	The type of data this entry is
			/// \param integerValue		If the type is an integer type, the value of that integer
			///
			/////////////////////////////////////////////////////////////////////////////
			TableEntry(std::basic_string<StringCharType> &rawString, ValueType entryValueType, std::uint32_t integerValue = 0)
			{
				raw = rawString; 
				this->valueType = entryValueType;
				intValue = integerValue;
			}
			/////////////////////////////////////////////////////////////////////////////
			/// \brief Constructor for a basic two digit hex value
			/// \details Under normal cirtumstances you shouldn't need this.
			///
			/// \param integerValue		If the type is an integer type, the value of that integer
			/// \param width		The width of the integer.  1 = byte, 2 = word, etc.
			///
			/////////////////////////////////////////////////////////////////////////////
			TableEntry(std::uint32_t integerValue, int width)
			{
				valueType = ValueType::Hexadecimal;
				intValue = integerValue;
				std::basic_stringstream<StringCharType> ss;
				ss << "$" << std::hex << std::setw(width * 2) << std::uppercase << std::setfill((StringCharType)'0') << integerValue;
				raw = ss.str();
			}

			/////////////////////////////////////////////////////////////////////////////
			/// \brief Converts the entry to a string based on its type
			/// \details Properly formats the entry based on type.  For example, a hex type will output $00.  A binary type %00000000.  The width adjusts these as well.
			///
			/// \param intWidth		How wide the entry should be printed.  1 = byte, 2 = word, etc.
			///
			/////////////////////////////////////////////////////////////////////////////
			std::basic_string<StringCharType> toString(int intWidth)
			{
				std::basic_stringstream<StringCharType> ss;

				if (isInteger())
				{
					if (valueType == ValueType::Hexadecimal)
						ss << '$' << std::hex << std::uppercase << std::setfill((StringCharType)'0') << std::setw(intWidth * 2) << intValue;
					else if (valueType == ValueType::Decimal)
						ss << intValue;
					else if (valueType == ValueType::Binary)
					{
						ss << '%';
						std::basic_stringstream<StringCharType> binaryStream;
						binaryStream << std::bitset<32>(intValue);
						std::basic_string<StringCharType> binaryString;
						binaryString = binaryStream.str();
						if (intWidth <= 4)
							binaryString = binaryString.substr((4 - intWidth) * 8);
						ss << binaryString;
					}
						
				}
				else
					ss << raw;
				return ss.str();
			}

			/////////////////////////////////////////////////////////////////////////////
			/// \brief Tests if the entry is of integer type
			/// \details Returns true if the type is hexadecimal, decimal, or binary
			/////////////////////////////////////////////////////////////////////////////
			bool isInteger() { return (valueType == ValueType::Hexadecimal || valueType == ValueType::Decimal || valueType == ValueType::Binary); }
			/////////////////////////////////////////////////////////////////////////////
			/// \brief Tests if the entry is of identifier type
			/////////////////////////////////////////////////////////////////////////////
			bool isIdentifier() { return (valueType == ValueType::Identifier); }
			/////////////////////////////////////////////////////////////////////////////
			/// \brief Tests if the entry is of hexadecimal type
			/////////////////////////////////////////////////////////////////////////////
			bool isHexadecimal() { return (valueType == ValueType::Hexadecimal); }
			/////////////////////////////////////////////////////////////////////////////
			/// \brief Tests if the entry is of decimal type
			/////////////////////////////////////////////////////////////////////////////
			bool isDecimal() { return (valueType == ValueType::Decimal); }
			/////////////////////////////////////////////////////////////////////////////
			/// \brief Tests if the entry is of binary type
			/////////////////////////////////////////////////////////////////////////////
			bool isBinary() { return (valueType == ValueType::Binary); }
			/////////////////////////////////////////////////////////////////////////////
			/// \brief Returns the integer value of the entry
			/// \throws TableEntryTypeMismatchException The entry is not of integer type
			/////////////////////////////////////////////////////////////////////////////
			std::uint32_t getIntegerValue() { if (isInteger()) return intValue; else throw TableEntryTypeMismatchException(); }
			/////////////////////////////////////////////////////////////////////////////
			/// \brief Returns the identifier value of the entry
			/// \throws TableEntryTypeMismatchException The entry is not of identifier type
			/////////////////////////////////////////////////////////////////////////////
			std::basic_string<StringCharType> getIdentifierValue() { if (isIdentifier()) return raw; else throw TableEntryTypeMismatchException(); }
			/////////////////////////////////////////////////////////////////////////////
			/// \brief Changes the type to decimal
			/////////////////////////////////////////////////////////////////////////////
			void setToDecimal(std::uint32_t newValue) { raw.clear(); valueType = ValueType::Decimal; intValue = newValue; }
			/////////////////////////////////////////////////////////////////////////////
			/// \brief Changes the type to hexadecimal
			/////////////////////////////////////////////////////////////////////////////
			void setToHexadecimal(std::uint32_t newValue) { raw.clear(); valueType = ValueType::Hexadecimal; intValue = newValue; }
			/////////////////////////////////////////////////////////////////////////////
			/// \brief Changes the type to binary
			/////////////////////////////////////////////////////////////////////////////
			void setToBinary(std::uint32_t newValue) { raw.clear(); valueType = ValueType::Binary; intValue = newValue; }
			/////////////////////////////////////////////////////////////////////////////
			/// \brief Changes the type to identifier
			/// \param newString	The value of the identifier
			/////////////////////////////////////////////////////////////////////////////
			void setToIdentifier(std::basic_string<StringCharType> &newString) { valueType = ValueType::Identifier; raw = newString; }
		};
		
		
		std::vector<TableEntry> internalValues;				/// The list of entries for this row
		std::basic_string<StringCharType> commentString;		/// The optional comment string at the end of the row
		std::basic_string<StringCharType> spaceBeforeComment;		/// Any trailing whitespace after the row


		/////////////////////////////////////////////////////////////////////////////
		/// \brief Sets the width of the row's values.  
		/// \param value The new width to use. 1 = db, 2 = dw, 3 = dl, 4 = dq.
		/////////////////////////////////////////////////////////////////////////////
		void setIntegerWidth(int value) { intWidth = value; }
		/////////////////////////////////////////////////////////////////////////////
		/// \brief Gets the width of the row's values.  
		/// \returns The width used. 1 = db, 2 = dw, 3 = dl, 4 = dq.
		/////////////////////////////////////////////////////////////////////////////
		int getIntegerWidth() { return intWidth; }

		/////////////////////////////////////////////////////////////////////////////
		/// \brief Default constructor.
		/////////////////////////////////////////////////////////////////////////////
		TableRow() { intWidth = 1; }

		/////////////////////////////////////////////////////////////////////////////
		/// \brief Default constructor that also sets the width of the entries.
		/// \param integerWidth The width to use. 1 = db, 2 = dw, 3 = dl, 4 = dq.
		/////////////////////////////////////////////////////////////////////////////
		TableRow(int integerWidth) { this->intWidth = integerWidth; }

		/////////////////////////////////////////////////////////////////////////////
		/// \brief Converts the entire row to a string
		/// \details Includes the "db" (etc.) at the start and the comment at the end.  No newline is appended.
		/////////////////////////////////////////////////////////////////////////////
		std::basic_string<StringCharType> toString()
		{
			std::basic_stringstream<StringCharType> ss;
			if (intWidth <= 1)
				ss << "db ";
			else if (intWidth == 2)
				ss << "dw ";
			else if (intWidth == 3)
				ss << "dl ";
			else if (intWidth >= 4)
				ss << "dq ";

			for (unsigned int i = 0; i < internalValues.size(); i++)
			{
				ss << internalValues[i].toString(intWidth);
				if (i != internalValues.size() - 1)
					ss << ",";
			}

			if (commentString.size() > 0)
				ss << spaceBeforeComment << ";" << commentString;

			return ss.str();
		}
	};


	/////////////////////////////////////////////////////////////////////////////
	/// \brief Represents a table in a patch
	/// \details Under most circumstances you'll want to just deal with this and not muck around with TableRow or TableRow::TableEntry.
	/////////////////////////////////////////////////////////////////////////////
	template <typename StringCharType>
	class Table
	{
		std::basic_string<StringCharType> newline;
	public:
		std::basic_string<StringCharType> tableName;							/// The name of the table
		std::vector<TableRow<StringCharType>> rows;							/// A list of the table's rows
		typename std::basic_string<StringCharType>::const_iterator originalStringIteratorStart;		/// The original starting position of the table
		typename std::basic_string<StringCharType>::const_iterator originalStringIteratorEnd;		/// The original ending position of the table


		/////////////////////////////////////////////////////////////////////////////
		/// \brief Returns the length of the table
		/// \details As this manually counts the number of entries per row this is an O(n) operation.  Probably only worth considering if you're worried about tight loop performance.
		/////////////////////////////////////////////////////////////////////////////
		unsigned int count()
		{
			unsigned int ret = 0;
			for (unsigned int i = 0; i < rows.size(); i++)
				ret += rows[i].internalValues.size();
			return ret;
		}

		/////////////////////////////////////////////////////////////////////////////
		/// \brief Sets the entry at the specified position in the table to a decimal value.
		/// \throws TableEntryMissingException	The index is beyond the range of the table's length.
		/////////////////////////////////////////////////////////////////////////////
		void setDecimalAt(unsigned int index, std::uint32_t value)
		{
			at(index).setToDecimal(value);
		}

		/////////////////////////////////////////////////////////////////////////////
		/// \brief Sets the entry at the specified position in the table to a hexadecimal value.
		/// \throws TableEntryMissingException	The index is beyond the range of the table's length.
		/////////////////////////////////////////////////////////////////////////////
		void setHexadecimalAt(unsigned int index, std::uint32_t value)
		{
			at(index).setToHexadecimal(value);
		}

		/////////////////////////////////////////////////////////////////////////////
		/// \brief Sets the entry at the specified position in the table to a binary value.
		/// \throws TableEntryMissingException	The index is beyond the range of the table's length.
		/////////////////////////////////////////////////////////////////////////////
		void setBinaryAt(unsigned int index, std::uint32_t value)
		{
			at(index).setToBinary(value);
		}

		/////////////////////////////////////////////////////////////////////////////
		/// \brief Gets the integer at the specified position in the table
		/// \returns The integer value of the table entry at that position
		/// \throws TableEntryMissingException		The index is beyond the range of the table's length.
		/// \throws TableEntryTypeMismatchException	The value at the index is not of integer type.
		/////////////////////////////////////////////////////////////////////////////
		std::uint32_t getIntegerAt(unsigned int index)
		{
			return at(index).getIntegerValue();
		}

		/////////////////////////////////////////////////////////////////////////////
		/// \brief Gets the identifier at the specified position in the table
		/// \returns The identifier value of the table entry at that position
		/// \throws TableEntryMissingException		The index is beyond the range of the table's length.
		/// \throws TableEntryTypeMismatchException	The value at the index is not of identifier type.
		/////////////////////////////////////////////////////////////////////////////
		std::basic_string<StringCharType> getIdentifierAt(unsigned int index)
		{
			return at(index).getIdentifierValue();
		}

		/////////////////////////////////////////////////////////////////////////////
		/// \brief Sets the entry at the specified position in the table to an identifier value.
		/// \throws TableEntryMissingException	The index is beyond the range of the table's length.
		/////////////////////////////////////////////////////////////////////////////
		void setIdentifierAt(unsigned int index, std::basic_string<StringCharType> &identifierString)
		{
			at(index).setToIdentifier(identifierString);
		}
		

		/////////////////////////////////////////////////////////////////////////////
		/// \brief Gets the entry at the specified position in the table
		/// \returns The table entry at that position
		/// \throws TableEntryMissingException		The index is beyond the range of the table's length.
		/////////////////////////////////////////////////////////////////////////////
		typename TableRow<StringCharType>::TableEntry &at(unsigned int index)
		{
			unsigned int x = 0, y = 0;

			while (index > 0)
			{
				x++;
				index--;
				if (y >= rows.size())
				{
					throw TableEntryMissingException();
				}
				if (x >= rows[y].internalValues.size())
				{
					x = 0;
					y++;
				}
			}

			return rows.at(y).internalValues[x];
		}

		/////////////////////////////////////////////////////////////////////////////
		/// \brief Converts the entire table to a string
		/// \details This includes the table's name. It should look just like it was found if no changes were made.
		/// \returns The table in string form.
		/////////////////////////////////////////////////////////////////////////////
		std::basic_string<StringCharType> toString()
		{
			std::basic_string<StringCharType> ret;
			if (tableName.size() > 0)
			{
				ret += tableName;
				ret += ':';
				ret += newline;
			}

			for (unsigned int i = 0; i < rows.size(); i++)
				ret += rows[i].toString() + newline;

			return ret;
		}

		/////////////////////////////////////////////////////////////////////////////
		/// \brief Constructs a table
		/// \param start		The start of the patch to get the table from
		/// \param end			The end of the patch to get the table from
		/// \param tableLabelNameStart	The start of the string containing the label that contains the table data
		/// \param tableLabelNameEnd	The end of the string containing the label that contains the data
		/////////////////////////////////////////////////////////////////////////////
		template <typename StringIteratorType>
		Table(StringIteratorType start, StringIteratorType end, StringIteratorType tableLabelNameStart, StringIteratorType tableLabelNameEnd);



		/////////////////////////////////////////////////////////////////////////////
		/// \brief Constructs an empty table
		/// \param newlineSample	The newline to use for this table (\\r\\n or \\n)
		/////////////////////////////////////////////////////////////////////////////
		Table(std::basic_string<StringCharType> &newlineSample) { newline = newlineSample; }


		/////////////////////////////////////////////////////////////////////////////
		/// \brief Resizes the table to contain a set number of elements
		/// \details The number of entries per row, if new rows are added, is determined by the width of the first row in the table.
		/// \param newCount		How many entries the table should have
		/// \param defaultValue		The default integer value of the new entries
		/// \param intWidth		The width of the new rows.  1 = db, 2 = dw, 3 = dl, 4 = dq.
		/////////////////////////////////////////////////////////////////////////////
		void resize(unsigned int newCount, std::uint32_t defaultValue, int intWidth)
		{
			if (rows.size() == 0)
				resize(newCount, defaultValue, intWidth, 16);
			else
				resize(newCount, defaultValue, intWidth, rows[0].internalValues.size());
		}


		/////////////////////////////////////////////////////////////////////////////
		/// \brief Resizes the table to contain a set number of elements, allowing the specification of a preferred row width.
		/// \param newCount		How many entries the table should have
		/// \param defaultValue		The default integer value of the new entries
		/// \param intWidth		The width of the new rows.  1 = db, 2 = dw, 3 = dl, 4 = dq
		/// \param rowWidth		How many entries each newly added row should have 
		/////////////////////////////////////////////////////////////////////////////
		void resize(unsigned int newCount, std::uint32_t defaultValue, int intWidth, int rowWidth)
		{
 			auto currentCount = count();

			if (currentCount > newCount)
			{
				//auto currentRowCountLeft = rowWidth - rows[rows.size()- 1].internalValues.size();
				auto totalExcessCount = currentCount - newCount;
				//auto allButLastRowCount = currentCount - rows[rows.size() - 1].internalValues.size();
				auto lastRowCount = rows[rows.size() - 1].internalValues.size();

				if (totalExcessCount >= lastRowCount)
				{
					rows.pop_back();
					resize(newCount, defaultValue, intWidth, rowWidth);
				}
				else
				{
					rows[rows.size() - 1].internalValues.resize(lastRowCount - totalExcessCount);
				}

			}
			else if (currentCount < newCount)
			{
				auto newSpacesNeeded = newCount - currentCount;
				decltype(newSpacesNeeded) currentRowCountLeft;

				if (rows.size() == 0)
				{
					rows.push_back(TableRow<StringCharType>(intWidth));
				}
				
				currentRowCountLeft = rowWidth - rows[rows.size() - 1].internalValues.size();

				if (currentRowCountLeft == 0)
				{
					rows.push_back(TableRow<StringCharType>(intWidth));			// We need more entries but the final row has no space.
					resize(newCount, defaultValue, intWidth, rowWidth);		// Add a new row and retry.
				}
				else if (newSpacesNeeded <= currentRowCountLeft)			// We have enough space in our current row.  Resize.	
					rows[rows.size() - 1].internalValues.resize(newSpacesNeeded + rows[rows.size() - 1].internalValues.size(), TableRow<StringCharType>::TableEntry(defaultValue, intWidth));
				else
				{
					rows[rows.size() - 1].internalValues.resize(rowWidth, TableRow<StringCharType>::TableEntry(defaultValue, intWidth));
					resize(newCount, defaultValue, intWidth, rowWidth);

				}

			}
		}

		/////////////////////////////////////////////////////////////////////////////
		/// \brief Returns the row containing the requested entry
		/// \param index		The entry you're looking for.  The row containing this entry will be returned.
		/////////////////////////////////////////////////////////////////////////////
		TableRow<StringCharType> &getRowContainingIndex(int index)
		{
			unsigned int x = 0, y = 0;

			while (index > 0)
			{
				x++;
				index--;
				if (y >= rows.size())
				{
					throw TableEntryMissingException();
				}
				if (x >= rows[y].internalValues.size())
				{
					x = 0;
					y++;
				}
			}

			return rows.at(y);
		}



	};

	//template <typename StringIteratorType>
	//Table<typename std::iterator_traits<StringIteratorType>::value_type> GetTable(StringIteratorType start, StringIteratorType end, StringIteratorType tableLabelNameStart, StringIteratorType tableLableNameEnd);

//////////////////////////////////////////////////////////////////////////////
///  @}
//////////////////////////////////////////////////////////////////////////////
}

#include "Patch.inl"


#endif
//#elif defined _CLR_NOW_USE_ASARDLL

//#include "Patch.inl"
//#endif