//#include "asardll.hpp"

//#if !(defined __cplusplus_cli) || (defined __cplusplus_cli && defined _CLR_NOW_USE_ASARDLL)		// Something strange about Asar's functions being defined multiple times when compiled under CLI only, even though asardll.hpp only *declares* stuff...  I don't even know.

//#ifdef __cplusplus_cli
//#pragma message ("__cplusplus_cli defined")
//#endif

//#ifdef _CLR_NOW_USE_ASARDLL
//#pragma message ("_CLR_NOW_USE_ASARDLL defined")
//#endif

//#pragma message("Included patch.inl")


#include <vector>
#include <cctype>

//#ifndef __cplusplus_cli
#include "asardll.hpp"
//#endif

#include "Optional.hpp"

namespace worldlib
{

	namespace internal
	{
		// Returns true if asar returned an error.
		template <typename stringType, typename errorDataOutputIteratorType, typename warningDataOutputIteratorType, typename labelDataOutputIteratorType, typename defineDataOutputIteratorType>
		bool retrieveAsarResults(errorDataOutputIteratorType errorOut, warningDataOutputIteratorType warningOut, labelDataOutputIteratorType labelOut, defineDataOutputIteratorType defineOut)
		{
			int count = 0;
			bool asarError = false;

			const errordata *e = asar_geterrors(&count);
			while (count > 0)
			{
				asarError = true;
				count--;
				*(errorOut++) = PatchErrorData<stringType>(e[count].fullerrdata, e[count].rawerrdata, e[count].block, e[count].filename, e[count].line, e[count].callerfilename, e[count].callerline);
			}

			const errordata *w = asar_getwarnings(&count);
			while (count > 0)
			{
				count--;
				*(warningOut++) = PatchErrorData<stringType>(w[count].fullerrdata, w[count].rawerrdata, w[count].block, w[count].filename, w[count].line, w[count].callerfilename, w[count].callerline);
			}

			const labeldata *l = asar_getalllabels(&count);
			while (count > 0)
			{
				count--;
				*(labelOut++) = PatchLabelData<stringType>(l[count].name, l[count].location);
			}

			const definedata *d = asar_getalldefines(&count);
			while (count > 0)
			{
				count--;
				*(defineOut++) = PatchDefineData<stringType>(d[count].name, d[count].contents);
			}

			return asarError;
		}

	}



	template <typename stringType, typename outputIteratorType, typename errorDataOutputIteratorType, typename warningDataOutputIteratorType, typename labelDataOutputIteratorType, typename defineDataOutputIteratorType>
	outputIteratorType compileToBinary(const char *patchFilepath, outputIteratorType out, errorDataOutputIteratorType &errorDataOutput, warningDataOutputIteratorType &warningDataOutput, labelDataOutputIteratorType &labelDataOutput, defineDataOutputIteratorType &defineDataOutput)
	{
		if (asar_init() == false) { throw DLLLoadException("asar.dll"); }

		int maxLength = asar_maxromsize();
		int resultingLength = 0x8000;

		byte *buffer = (byte *)malloc(maxLength);		// Not using a vector since increasing its size is O(n).

		asar_patch(patchFilepath, (char *)buffer, maxLength, &resultingLength);

		for (int i = 0; i < resultingLength; i++)
		{
			*(out++) = buffer[i];
		}

		std::free(buffer);

		internal::retrieveAsarResults<stringType>(errorDataOutput, warningDataOutput, labelDataOutput, defineDataOutput);

		asar_close();
		return out;
	}

	template <typename stringType, typename outputIteratorType, typename errorDataOutputIteratorType>
	outputIteratorType compileToBinary(const char *patchFilepath, outputIteratorType out, errorDataOutputIteratorType &errorDataOutput)
	{
		std::vector<PatchWarningData<std::string>> warningData;
		std::vector<PatchLabelData<std::string>> labelData;
		std::vector<PatchDefineData<std::string>> defineData;

		return compileToBinary<stringType>(patchFilepath, out, errorDataOutput, std::back_inserter(warningData), std::back_inserter(labelData), std::back_inserter(defineData));
	}

	template <typename stringType, typename outputIteratorType>
	bool compileToBinary(const char *patchFilepath, outputIteratorType &out)
	{
		std::vector<PatchErrorData<std::string>> errorData;
		out = compileToBinary<stringType>(patchFilepath, out, std::back_inserter(errorData));
		return errorData.size() == 0;
	}

	template <typename stringType, typename romDataType, typename errorDataOutputIteratorType, typename warningDataOutputIteratorType, typename labelDataOutputIteratorType, typename defineDataOutputIteratorType>
	bool patchToROM(const char *patchFilepath, const romDataType *romDataType, int romSize, errorDataOutputIteratorType &errorDataOutput, warningDataOutputIteratorType &warningDataOutput, labelDataOutputIteratorType &labelDataOutput, defineDataOutputIteratorType &defineDataOutput)
	{
		if (asar_init() == false) { throw DLLLoadException("asar.dll"); }

		int resultingLength = romSize;

		asar_patch(patchFilepath, (char *)(romDataType), romSize, &resultingLength);

		bool failure = internal::retrieveAsarResults<stringType>(errorDataOutput, warningDataOutput, labelDataOutput, defineDataOutput);

		asar_close();

		return !failure;
	}

	template <typename stringType, typename romDataType, typename errorDataOutputIteratorType>
	bool patchToROM(const char *patchFilepath, const romDataType *romData, int romSize, errorDataOutputIteratorType &errorDataOutput)
	{
		std::vector<PatchWarningData<std::string>> warningData;
		std::vector<PatchLabelData<std::string>> labelData;
		std::vector<PatchDefineData<std::string>> defineData;

		return patchToROM<stringType>(patchFilepath, romData, romSize, errorDataOutput, std::back_inserter(warningData), std::back_inserter(labelData), std::back_inserter(defineData));
	}

	template <typename stringType, typename romDataType>
	bool patchToROM(const char *patchFilepath, const romDataType *romData, int romSize)
	{
		std::vector<PatchErrorData<std::string>> errorData;
		return patchToROM<stringType>(patchFilepath, romData, romSize, std::back_inserter(errorData));
	}

	template <typename patchStringType, typename definitionStringType>
	void replacePatchDefinition(patchStringType &patch, const definitionStringType &definitionToChange, int valueToChangeTo, int digits)
	{
		replacePatchDefinition(std::begin(patch), std::end(patch), std::begin(definitionToChange), std::end(definitionToChange), valueToChangeTo, digits);
	}

	template <typename patchStringType, typename definitionStringType, typename replacementStringType>
	void replacePatchDefinition(patchStringType &patch, const definitionStringType &definitionToChange, const replacementStringType &valueToChangeTo)
	{
		replacePatchDefinition(std::begin(patch), std::end(patch), std::begin(definitionToChange), std::end(definitionToChange), std::begin(valueToChangeTo), std::end(valueToChangeTo));
	}



	template <typename patchStringIteratorType, typename definitionStringIteratorType>
	void replacePatchDefinition(patchStringIteratorType patchStart, patchStringIteratorType patchEnd, definitionStringIteratorType definitionStart, definitionStringIteratorType definitionEnd, int valueToChangeTo, int digits)
	{
		/*char buffer1[10];
		char buffer2[10];
		sprintf(buffer1, "$%%0%dX", digits);
		sprintf(buffer2, buffer1, valueToChangeTo);*/
		typedef std::remove_const<std::remove_reference<decltype(*patchStart)>::type>::type patchStringCharType;

		std::basic_stringstream<patchStringCharType> ss;
		ss << "$" << std::hex << std::setw(digits) << std::uppercase << std::setfill((patchStringCharType)'0') << valueToChangeTo;

		std::basic_string<patchStringCharType> bufferString = ss.str();;

		replacePatchDefinition(patchStart, patchEnd, definitionStart, definitionEnd, bufferString.begin(), bufferString.end());
	}

	template <typename patchStringIteratorType, typename definitionStringIteratorType, typename definitionValueIteratorType>
	void getPatchDefinitionValue(patchStringIteratorType patchStart, patchStringIteratorType patchEnd, definitionStringIteratorType definitionStringStart, definitionStringIteratorType definitionStringEnd, definitionValueIteratorType& definitionValueStart, definitionValueIteratorType& definitionValueEnd)
	{
		typedef std::remove_const<std::remove_reference<decltype(*patchStart)>::type>::type patchStringCharType;
		typedef std::remove_const<std::remove_reference<decltype(*definitionStringStart)>::type>::type definitionStringCharType;
		typedef std::remove_const<std::remove_reference<decltype(*definitionValueStart)>::type>::type definitionValueCharType;

		std::basic_string<patchStringCharType> searchStr;
		searchStr = std::basic_string<patchStringCharType>(1, '!');
		searchStr += std::basic_string<definitionStringCharType>(definitionStringStart, definitionStringEnd);
		std::basic_string<patchStringCharType>::iterator index = std::search(patchStart, patchEnd, searchStr.begin(), searchStr.end() - 1);

		if (index == patchEnd) 
			PatchDefinitionMissingException();

		index += searchStr.length();

		bool foundEquals = false;

		while (index < patchEnd)
		{
			if (*index == '=')
			{
				foundEquals = true;
				break;
			}
			else if (*index == '\n' || !std::isspace(*index))
			{
				throw PatchDefinitionMalformedException();
			}
			index++;
		}

		index++;

		if (foundEquals == false) throw PatchDefinitionMalformedException();

		bool foundDefinitionName = false;

		while (index < patchEnd)
		{
			if (*index == '\n')
			{
				throw PatchDefinitionMalformedException();
			}
			else if (!std::isspace(*index))
			{
				foundDefinitionName = true;
				definitionValueStart = index;
				break;
			}
			index++;
		}
		if (foundDefinitionName == false) throw PatchDefinitionMalformedException();


		auto originalDefinitionStart = index;
		int charsToReplace = 0;

		while (index < patchEnd)
		{
			if (!std::isspace(*index))
			{
				foundDefinitionName = true;
			}

			if (std::isspace(*index))
			{
				definitionValueEnd = index;
				break;
			}
			index++;
			charsToReplace++;
		}
	}


	template <typename patchStringIteratorType, typename definitionStringIteratorType, typename replacementStringIteratorType>
	void replacePatchDefinition(patchStringIteratorType patchStart, patchStringIteratorType patchEnd, definitionStringIteratorType definitionStart, definitionStringIteratorType definitionEnd, replacementStringIteratorType valueToChangeToStart, replacementStringIteratorType valueToChangeToEnd)
	{
		patchStringIteratorType valueToChangeFromStart, valueToChangeFromEnd;
		getPatchDefinitionValue<patchStringIteratorType>(patchStart, patchEnd, definitionStart, definitionEnd, valueToChangeFromStart, valueToChangeFromEnd);

		int charsToReplace = std::distance(valueToChangeFromStart, valueToChangeFromEnd);
		int newDefinitionLength = std::distance(valueToChangeToStart, valueToChangeToEnd);

		if (charsToReplace != newDefinitionLength)
			throw PatchDefinitionSizeMismatch(); //std::runtime_error("The value for the patch definition was not the same size as the given string to replce it with.");

		std::copy_n(valueToChangeToStart, charsToReplace, valueToChangeFromStart);
	}





	template <typename dataType>
	std::vector<dataType> compileToBinary(const char *patchFilepath, std::vector<PatchErrorData<std::string>> &errorDataOutput)
	{
		std::vector<dataType> ret;
		compileToBinary(filePath, std::back_inserter(ret), errorDataOutput);
	}






	template <typename stringIteratorType>
	inline int isLabel(stringIteratorType start, stringIteratorType end)
	{
		bool foundAlphaStart = false;
		bool foundColon = false;

		auto current = start;
		int length = 0;

		while (current < end)
		{
			if (foundAlphaStart == false)
			{
				if (std::isalpha(*current) || *current == '_')
				{
					foundAlphaStart = true;
					length++;
				}
				else
				{
					break;
				}
			}
			else
			{
				if (*current == ':')
				{
					foundColon = true;
					length++;
					break;
				}
				if (!std::isalnum(*current) && *current != '_')
				{
					break;
				}
				else
					length++;
			}

			current++;
		}

		if (foundAlphaStart && foundColon)
			return length - 1;
		else
			return 0;
	}


	template <typename stringType>
	inline int isLabel(const stringType &str)
	{
		return isLabel(str.begin(), str.end());
	}

	template <typename stringIteratorType>
	inline stringIteratorType findLabel(stringIteratorType start, stringIteratorType end, bool skipThis)
	{
		stringIteratorType current = start;

		bool inComment = false;

		while (current < end)
		{
			if (*current == ';')
				inComment = true;
			if (*current == '\n')
				inComment = false;

			if (inComment == false)
			{

				if (int length = isLabel(current, end))
				{
					if (skipThis == true)
					{
						skipThis = false;
						current += length + 1;
					}
					else
						return current;
				}
			}

			current++;
		}
		return end;
	}


	template <typename StringCharType>
	inline typename std::basic_string<StringCharType>::iterator findLabel(std::basic_string<StringCharType> &patchString, std::basic_string<StringCharType> &tableNameString)
	{
		return findLabel(patchString.begin(), patchString.end(), tableNameString.begin(), tableNameString.end());
	}

	template <typename stringIteratorType>
	inline stringIteratorType findLabel(stringIteratorType start, stringIteratorType end, stringIteratorType labelNameStart, stringIteratorType labelNameEnd)
	{
		auto current = findLabel(start, end, false);

		//std::basic_string<typename std::iterator_traits<StringIteratorType>::value_type> tableLabelName(tableLabelNameStart, tableLableNameEnd);

		while (true)
		{
			if (current == end)
				return end;

			if (std::equal(labelNameStart, labelNameEnd, current))
				break;

			current = findLabel(current, end, true);
		}

		return current;
	}

	namespace internal
	{
		template <typename StringIteratorType>
		Optional<TableRow<typename std::iterator_traits<StringIteratorType>::value_type>> createTableRow(StringIteratorType &oldCurrent, StringIteratorType end)
		{
			int intWidth = 0;
			StringIteratorType current = oldCurrent;
			Optional<TableRow<typename std::iterator_traits<StringIteratorType>::value_type>> default;
			TableRow<typename std::iterator_traits<StringIteratorType>::value_type> ret;
			
			bool inComment = false;
			while (current < end)
			{
				auto currentChar = *current;
				if (inComment == true && currentChar == '\n')
					inComment = false;
				else if (currentChar == ';')
					inComment = true;
				else if (std::isspace(currentChar) == false && inComment == false)
					break;
				current++;
			}

			if (current == end) return default;


			if (!(*current == 'd' || *current == 'D')) return default;
			current++;
			if (current == end) return default;
			if (*current == 'b' || *current == 'B') intWidth = 1;
			else if (*current == 'w' || *current == 'W') intWidth = 2;
			else if (*current == 'l' || *current == 'L') intWidth = 3;
			else if (*current == 'q' || *current == 'Q') intWidth = 4;
			else return default;

			current++;
			if (current == end) return default;
			if (std::isspace(*current) == false) return default;

			current++;
			if (current == end) return default;


			std::basic_string<typename std::iterator_traits<StringIteratorType>::value_type> currentRawString;
			std::basic_string<typename std::iterator_traits<StringIteratorType>::value_type> currentWhitespaceString;
			std::basic_string<typename std::iterator_traits<StringIteratorType>::value_type> currentCommentString;
			std::uint32_t currentIntValue = 0;
			bool onFirstChar = true;

			enum Parsing { none, hex, dec, bin, identifier, comment };
			Parsing parsing = none;

			//bool isDec = false;
			//bool isHex = false;
			//bool isBin = false;

			while (current < end)
			{
				if (onFirstChar)
				{
					currentRawString.clear();
				}


				if (parsing == comment && *current != '\n' && *current != '\r')
				{
					currentCommentString += *current;
					current++;
				}
				else if (parsing == comment && (*current == '\n' || *current == '\r'))
				{
					parsing = none;
					break;
				}
				else if (*current == '$' && onFirstChar)
				{
					currentRawString += *current;
					parsing = hex;
					//isHex = true;
					current++;
					onFirstChar = false;
				}
				else if (*current == '%' && onFirstChar)
				{
					currentRawString += *current;
					parsing = bin;
					//isBin = true;
					current++;
					onFirstChar = false;
				}
				else if (std::isdigit(*current) && onFirstChar)
				{
					currentRawString += *current;
					parsing = dec;
					//isDec = true;
					onFirstChar = false;
				}
				else if (std::isdigit(*current) && parsing == dec)
				{
					currentRawString += *current;
					currentIntValue *= 10;
					currentIntValue += (*current) - '0';
					current++;
				}
				else if (std::isdigit(*current) && parsing == hex)
				{
					currentRawString += *current;
					currentIntValue *= 16;
					currentIntValue += (*current) - '0';
					current++;
				}
				else if (*current == '0' || *current == '1' && parsing == bin)
				{
					currentRawString += *current;
					currentIntValue *= 2;
					currentIntValue += (*current) - '0';
					current++;
				}
				else if (std::isalpha(*current) && parsing == hex)
				{
					currentRawString += *current;
					currentIntValue *= 16;
					currentIntValue += (*current) - 'A' + 10;
					current++;
				}
				else if (std::isalpha(*current) || *current == '_' || *current == '%' || (parsing == identifier && std::isdigit(*current)))
				{
					currentRawString += *current;
					parsing = identifier;
					onFirstChar = false;
					current++;
				}
				else if (std::isspace(*current) && *current != '\n'&& *current != '\r')
				{
					currentWhitespaceString += *current;
					current++;
				}
				else if (*current == ';' || *current == ',' || *current == '\n' || *current == '\r')
				{
					if (currentRawString.length() > 0)
					{
						if (parsing == hex)
							ret.internalValues.push_back(TableRow<typename std::iterator_traits<StringIteratorType>::value_type>::TableEntry(currentRawString, TableRow<typename std::iterator_traits<StringIteratorType>::value_type>::TableEntry::ValueType::Hexadecimal, currentIntValue));
						else if (parsing == dec)
							ret.internalValues.push_back(TableRow<typename std::iterator_traits<StringIteratorType>::value_type>::TableEntry(currentRawString, TableRow<typename std::iterator_traits<StringIteratorType>::value_type>::TableEntry::ValueType::Decimal, currentIntValue));
						else if (parsing == bin)
							ret.internalValues.push_back(TableRow<typename std::iterator_traits<StringIteratorType>::value_type>::TableEntry(currentRawString, TableRow<typename std::iterator_traits<StringIteratorType>::value_type>::TableEntry::ValueType::Binary, currentIntValue));
						else
							ret.internalValues.push_back(TableRow<typename std::iterator_traits<StringIteratorType>::value_type>::TableEntry(currentRawString, TableRow<typename std::iterator_traits<StringIteratorType>::value_type>::TableEntry::ValueType::Identifier));
					}
					
					currentRawString.clear();
					currentIntValue = 0;
					onFirstChar = true;

					if (*current != ';' && parsing != comment)
						currentWhitespaceString.clear();
					
					if (*current ==';')
					{
						parsing = comment;
					}
					else if (parsing == comment && *current != '\n')
					{
						currentCommentString += *current;
					}
					else if (*current == '\n' || *current == '\r')
					{
						parsing = none;
						break;
					}

					current++;
				}
				else
				{
					return default;
				}

			}

			ret.spaceBeforeComment = currentWhitespaceString;
			ret.commentString = currentCommentString;
			oldCurrent = current;
			ret.setIntegerWidth(intWidth);
			return ret;
		}
	}

	template <typename StringCharType> template <typename StringIteratorType>
	Table<typename StringCharType>::Table(StringIteratorType start, StringIteratorType end, StringIteratorType tableLabelNameStart, StringIteratorType tableLabelNameEnd)
	{
		/*auto current = findLabel(start, end, false);

		std::basic_string<typename std::iterator_traits<StringIteratorType>::value_type> tableLabelName(tableLabelNameStart, tableLableNameEnd);

		while (true)
		{
			if (current == end)
				throw std::runtime_error("Could not find label to create table from.");

			auto currentLabelString = getLabel(current, end);

			if (currentLabelString == tableLabelName)
				break;

			current = findLabel(current, end, true);
		}*/

		auto current = findLabel(start, end, tableLabelNameStart, tableLabelNameEnd);
		if (current == end)
			throw PatchLabelMissingException();

		originalStringIteratorStart = current;

		current += std::distance(tableLabelNameStart, tableLabelNameEnd) + 1;


		while (true)
		{
			auto oldCurrent = current;
			auto currentTableRow = internal::createTableRow(current, end);
			if (oldCurrent == current || currentTableRow.isValid() == false)
				break;

			rows.push_back(currentTableRow.get());
		}
		originalStringIteratorEnd = current;

		newline = internal::getNewline<std::basic_string<typename std::iterator_traits<StringIteratorType>::value_type>>(start, end);
		tableName = std::basic_string<typename std::iterator_traits<StringIteratorType>::value_type>(tableLabelNameStart, tableLabelNameEnd);
	}


	template <typename StringCharType>
	void replacePatchTable(std::basic_string<StringCharType> &patch, Table<StringCharType> &table)
	{
		replacePatchTable(patch, table, true);
	}

	template <typename StringCharType>
	void replacePatchTable(std::basic_string<StringCharType> &patch, Table<StringCharType> &table, bool reparse)
	{
		auto tableString = table.toString();

		if (reparse == false)
			patch.replace(table.originalStringIteratorStart, table.originalStringIteratorEnd, tableString.begin(), tableString.end());
		else
		{
			auto tempTable = Table<StringCharType>(patch.begin(), patch.end(), table.tableName.begin(), table.tableName.end());
			patch.replace(tempTable.originalStringIteratorStart, tempTable.originalStringIteratorEnd, tableString.begin(), tableString.end());
		}

		// What follows is a big batch of stupid caused by forgetting that the original iterators are kept in the table itself.

		/*auto tableStart = findLabel(patch, table.tableName);
		if (tableStart == patch.end())
			throw PatchLabelMissingException();



		// Go all the way until the next non-table thing.
		// A table thing is:
		// a line that starts with db,
		// a line that starts with dw,
		// a line that starts with dl,
		// a line that starts with dq,
		// a line that starts with ;
		// Anything else ends the table.

		enum LineStatus
		{
			Empty,
			ValidTableData,
			InvalidTableData,
			ContainsOnlyWhitespace,
			ContainsOnlyComment,
			ContainsD,
			ContainsDB
		};
		
		std::vector<std::basic_string<StringCharType>::iterator> allLines;
		std::vector<LineStatus> allLineStatuses;

	

		typename std::basic_string<StringCharType>::iterator current = std::find(tableStart, patch.end(), '\n');
		if (current != patch.end()) 
			current++;

		while (current < patch.end())
		{
			if (*current == '\n') { allLines.push_back(current); allLineStatuses.push_back(ContainsOnlyWhitespace); }
			current++;
		}

		for (unsigned int i = 0; i < allLines.size(); i++)
		{
			auto lineEnd = (i == allLines.size() - 1)? patch.end() : allLines[i + 1];

			current = allLines[i];

			while (current < lineEnd)
			{
				if (allLineStatuses[i] == ContainsOnlyWhitespace)
				{
					if (std::isspace(*current)) 
						allLineStatuses[i] = ContainsOnlyWhitespace;
					else if (*current == 'd' || *current == 'D') 
						allLineStatuses[i] = ContainsD;
					else if (*current == ';') 
						allLineStatuses[i] = ContainsOnlyComment;
					else 
					{ 
						allLineStatuses[i] = InvalidTableData; 
						break;
					}
				}
				else if (allLineStatuses[i] == ContainsD)
				{
					if (*current == 'b' || *current == 'w' || *current == 'l' || *current == 'q' || *current == 'B' || *current == 'W' || *current == 'L' || *current == 'Q')
						allLineStatuses[i] = ContainsDB;
					else
					{
						allLineStatuses[i] = InvalidTableData;
						break;
					}
				}
				else if (allLineStatuses[i] == ContainsDB)
				{
					volatile auto currentChar = *current;

					if (std::isspace(currentChar) != 0)
						allLineStatuses[i] = ValidTableData;
					else
					{
						allLineStatuses[i] = InvalidTableData;
						break;
					}
				}
				
				current++;
			}
		}

		int tableStartLine = 0;
		int tableEmptyLine = -1;
		int tableEndLine = -1;

		auto tableEnd = tableStart;

		for (unsigned int i = 0; i < allLines.size(); i++)
		{
			if (allLineStatuses[i] == InvalidTableData)
			{
				if (tableEmptyLine == -1)
					tableEnd = allLines[i];
				else
					tableEnd = allLines[tableEmptyLine];
				break;
			}
			if (allLineStatuses[i] == ContainsOnlyWhitespace && tableEmptyLine == -1)
			{
				tableEmptyLine = i;
			}
			if (allLineStatuses[i] == ValidTableData)
			{
				tableEmptyLine = -1;
			}
		}

		auto tableString = table.ToString();

		patch.replace(tableStart, tableEnd, tableString.begin(), tableString.end());*/
	}

	/*template <typename StringIteratorType>
	Table<typename std::iterator_traits<StringIteratorType>::value_type> getTable(StringIteratorType start, StringIteratorType end, StringIteratorType tableLabelNameStart, StringIteratorType tableLableNameEnd)
	{
	}*/
}

//#endif