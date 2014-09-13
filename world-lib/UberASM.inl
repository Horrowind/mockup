#include <string>
#include <cstring>
#include <cstdlib>
#include <algorithm>
#include <cctype>
#include <sstream>
#include <type_traits>

#include "UberASM.hpp"
#include "Optional.hpp"
#include "Exceptions.hpp"



namespace worldlib
{

	namespace internal
	{
		/*template <typename stringIteratorType> inline int isLabel(stringIteratorType start, stringIteratorType end);
		template <typename stringIteratorType> inline Optional<stringIteratorType> findLabel(stringIteratorType start, stringIteratorType end, bool skipCurrent);
		template <typename stringIteratorType> inline Optional<stringIteratorType> findLevelLabel(stringIteratorType start, stringIteratorType end, int level, bool goPastLabel);
		template <typename stringType, typename tringIteratorType> inline Optional<stringType> getLabel(stringIteratorType start, stringIteratorType end);
		template <typename stringType> inline int getLevelLabelNumber(stringType label);
		template <typename stringIteratorType> inline Optional<stringIteratorType> findAnyLevelLabel(stringIteratorType start, stringIteratorType end);

		template <typename stringType> inline bool codeStartsWithWhitespaceThenComment(stringType code);

		template <typename stringIteratorType> inline bool levelHasCalls(stringIteratorType start, stringIteratorType end, int level);
		template <typename stringIteratorType> inline Optional<stringIteratorType> getLevelStart(stringIteratorType start, stringIteratorType end, int level, bool goPastLabel);
		template <typename stringIteratorType> inline Optional<stringIteratorType> getLevelEnd(stringIteratorType start, stringIteratorType end, int level, bool goPastLabel);
		template <typename stringIteratorType> inline Optional<stringIteratorType> getLevelRoutineCallsStart(stringIteratorType start, stringIteratorType end, int level, bool includeMeta);
		template <typename stringIteratorType> inline Optional<stringIteratorType> getLevelRoutineCallsEnd(stringIteratorType start, stringIteratorType end, int level, bool includeMeta);
		template <typename stringIteratorType> inline Optional<stringIteratorType> getRoutineDefinitionsStart(stringIteratorType start, stringIteratorType end, const stdStringType routineName, bool includeMeta);
		template <typename stringIteratorType> inline Optional<stringIteratorType> getRoutineDefinitionsEnd(stringIteratorType start, stringIteratorType end, const stdStringType routineName, bool includeMeta);
		template <typename stringIteratorType> inline Optional<stringIteratorType> getLevelRoutineCallStart(stringIteratorType start, stringIteratorType end, const stdStringType routineName, int level);
		template <typename stringIteratorType> inline Optional<stringIteratorType> getLevelRoutineCallEnd(stringIteratorType start, stringIteratorType end, const stdStringType routineName, int level);*/

		// Does a "light" conversion from one string type to another.  Wide chars get truncated to normal chars.
		template <typename resultStringType, typename originStringType>
		inline resultStringType convertString(const originStringType &string)
		{
			resultStringType ret;

			unsigned int i = 0;
			while (string[i] != '\0')			// No string.length()--we may be using char pointers.
			{
				ret += string[i];
				i++;
			}
			return ret;
		}

		template <typename stringType>
		inline stringType getNewline()
		{
			stringType unixNewline;
			stringType windowsNewline;
			unixNewline += '\n';
			windowsNewline += '\r';
			windowsNewline += '\n';
#ifdef _WIN32
			return windowsNewline;
#else
			return unixNewline;
#endif
		}

		template <typename stringType, typename stringIteratorType>
		inline stringType getNewline(stringIteratorType start, stringIteratorType end)
		{
			stringType unixNewline;
			stringType windowsNewline;
			unixNewline += '\n';
			windowsNewline += '\r';
			windowsNewline += '\n';

			auto pos = std::find(start, end, '\n');
			if (pos == start)
				return unixNewline;
			else if (pos > start && *(pos - 1) == '\r')
				return windowsNewline;
			else if (pos > start)
				return unixNewline;
			else
				return getNewline<stringType>();
		}

		template <typename stringType>
		inline stringType getNewline(stringType string)
		{
			return getNewline<stringType>(string.begin(), string.end());
		}

		// Returns 1 for a newline, 2 for a Windows newline, and 0 for no newline.
		template <typename stringIteratorType>
		inline int stringStartsWithNewline(stringIteratorType start, stringIteratorType end)
		{
			if (std::distance(start, end) >= 1 && *(start) == '\n')
				return 1;
			else if (std::distance(start, end) >= 2 && *(start) == '\r' && *(start + 1) == '\n')
				return 2;
			else
				return 0;
		}

		template <typename stringType>
		inline int stringStartsWithNewLine(const stringType &str)
		{
			return stringStartsWithNewLine(str.begin(), str.end());
		}

		// Returns 1 for a newline, 2 for a Windows newline, and 0 for no newline.
		template <typename stringIteratorType>
		inline int stringEndsWithNewline(stringIteratorType start, stringIteratorType end)
		{
			if (std::distance(start, end) >= 1 && *(end - 1) == '\n')
				return 1;
			else if (std::distance(start, end) >= 2 && *(end - 2) == '\r' && *(end - 1) == '\n')
				return 2;
			else
				return 0;
		}

		template <typename stringType>
		inline int stringEndsWithNewline(const stringType &str)
		{
			return stringEndsWithNewline(std::begin(str), std::end(str));
		}

		template <typename stringType>
		int getLevelLabelNumber(const stringType &label)
		{
			stringType numStr;
			int parsedNum = -1;

			stringType prefix;
			if (label.length() >= 5)
				prefix = label.substr(0, 5);
			else if (label.length() >= 9)
				prefix = label.substr(0, 9);

			if (prefix != convertString<stringType>("level") && prefix != convertString<stringType>("levelinit"))
				return -1;

			if (label.length() == 7)		// "levelX:"
			{
				numStr = label.substr(5, 1);
			}
			else if (label.length() == 8)		// "levelXX:"
			{
				numStr = label.substr(5, 2);
			}
			else if (label.length() == 9)		// "levelXXX:"
			{
				numStr = label.substr(5, 3);
			}
			else if (label.length() == 11)		// "levelinitX:"
			{
				numStr = label.substr(9, 1);
			}
			else if (label.length() == 12)		// "levelinitXX:"
			{
				numStr = label.substr(9, 2);
			}
			else if (label.length() == 13)		// "levelinitXXX:"
			{
				numStr = label.substr(9, 3);
			}
			else
				return -1;


			typedef typename stringType::value_type stdStringCharType;

			std::basic_stringstream<stdStringCharType> ss;
			ss << std::hex << numStr;
			ss >> parsedNum;

			//std::sscanf(numStr.c_str(), "%X", &parsedNum);
			return parsedNum;
		}


		template <typename stringType>
		inline bool codeStartsWithWhitespace(const stringType &code)
		{
			auto start = std::begin(code);
			auto end = std::end(code);
			if (std::distance(start, end) > 0)
				return std::isspace(code[0]) != 0;
			else
				return true;
		}	


		template <typename stringType>
		inline bool codeEndsWithWhitespace(const stringType &code)
		{
			auto start = std::begin(code);
			auto end = std::end(code);
			if (start == end)
				return true;
			else
				return std::isspace(*(end - 1)) != 0;
		}


		template <typename stringIteratorType>
		inline Optional<stringIteratorType> findAnyLevelLabel(stringIteratorType start, stringIteratorType end, bool skipCurrent)
		{
			typedef std::basic_string<typename std::iterator_traits<stringIteratorType>::value_type> stdStringType;

			if (start == end)
				return Optional<stringIteratorType>();

			auto current = findLabel(start + 1, end, false);

			if (current == end)
				return Optional<stringIteratorType>();


			while (current < end)
			{
				stdStringType labelStr = getLabel(current, end);
				int levelNumber = getLevelLabelNumber(labelStr);

				if (levelNumber >= 0)
					return current;
				else
					current = findLabel(current, end, true);

				if (current == end)
					return Optional<stringIteratorType>();
			}

			return Optional<stringIteratorType>();
		}

		template <typename stringIteratorType>
		inline Optional<stringIteratorType> findLevelLabel(stringIteratorType start, stringIteratorType end, int level, bool goPastLabel)
		{
			typedef std::basic_string<typename std::iterator_traits<stringIteratorType>::value_type> stdStringType;


			auto current = findAnyLevelLabel(start, end, false);

			while (current.get() < end)
			{
				stdStringType labelStr = getLabel(current.get(), end);
				int levelNumber = getLevelLabelNumber(labelStr);
				int offset = 0;
				if (goPastLabel)
					offset = labelStr.length();

				if (levelNumber >= 0 && levelNumber == level)
					return current.get() + offset;
				else
					current = findAnyLevelLabel(current.get() + labelStr.length(), end, false);

				if (current.isValid() == false)
				{
					auto definitionStartString = convertString<stdStringType>(";***AUTOMATIC DEFINITION START***");

					auto definitionsStart = std::search(start, end, definitionStartString.begin(), definitionStartString.end()); //uberASMFile.find(";***AUTOMATIC DEFINITION START***");
					if (definitionsStart == end)
						return end;
					else
						return definitionsStart;
				}
			}
			return Optional<stringIteratorType>();
		}

		template <typename stringIteratorType>
		inline Optional<stringIteratorType> getLevelStart(stringIteratorType start, stringIteratorType end, int level, bool goPastLabel)
		{
			return findLevelLabel(start, end, level, goPastLabel);
		}


		template <typename stringIteratorType>
		inline Optional<stringIteratorType> getLevelEnd(stringIteratorType start, stringIteratorType end, int level, bool goPastLabel)
		{
			typedef std::basic_string<typename std::iterator_traits<stringIteratorType>::value_type> stdStringType;

			auto levelStart = findLevelLabel(start, end, level, goPastLabel);
			if (levelStart.isValid() == false)
				throw PatchLabelMissingException();

			auto definitionStartString = convertString<stdStringType>(";***AUTOMATIC DEFINITION START***");

			auto definitionsStart = std::search(start, end, definitionStartString.begin(), definitionStartString.end());  //uberASMFile.find(";***AUTOMATIC DEFINITION START***");

			Optional<stringIteratorType> nextLevelLabel = findAnyLevelLabel(levelStart.get(), end, true);

			stringIteratorType levelEnd;
			if (nextLevelLabel.isValid())
				levelEnd = nextLevelLabel.get();
			else if (definitionsStart != end)
				levelEnd = definitionsStart;
			else
				levelEnd = end;

			return levelEnd;
		}


		template <typename stringIteratorType>
		inline Optional<stringIteratorType> getLevelRoutineCallsStart(stringIteratorType start, stringIteratorType end, int level, bool includeMeta)
		{
			typedef std::basic_string<typename std::iterator_traits<stringIteratorType>::value_type> stdStringType;

			auto levelStart = getLevelStart(start, end, level, true);
			auto levelEnd = getLevelEnd(start, end, level, false);
			if (levelStart.isValid() == false || levelEnd.isValid() == false)
				throw PatchLabelMissingException();

			stdStringType wholeLevelCode(levelStart.get(), levelEnd.get());
			stdStringType startString = convertString<stdStringType>(";***AUTOMATIC CALLS START***");

			auto startPos = wholeLevelCode.find(startString);

			if (startPos == -1)
				return Optional<stringIteratorType>();
			else
				return levelStart.get() + startPos + (includeMeta? 0 : startString.length());
		}

		template <typename stringIteratorType>
		inline Optional<stringIteratorType> getLevelRoutineCallsEnd(stringIteratorType start, stringIteratorType end, int level, bool includeMeta)
		{
			typedef std::basic_string<typename std::iterator_traits<stringIteratorType>::value_type> stdStringType;

			auto levelStart = getLevelStart(start, end, level, true);
			auto levelEnd = getLevelEnd(start, end, level, false);
			if (levelStart.isValid() == false || levelEnd.isValid() == false)
				throw PatchLabelMissingException();
			stdStringType wholeLevelCode(levelStart.get(), levelEnd.get());
			stdStringType endString = convertString<stdStringType>(";***AUTOMATIC CALLS END***");

			auto startPos = wholeLevelCode.find(endString);

			if (startPos == -1)
				return Optional<stringIteratorType>();
			else
				return levelStart.get() + startPos + (includeMeta? 0 : endString.length());
		}


		template <typename stringIteratorType, typename stringType>
		inline Optional<stringIteratorType> getRoutineDefinitionsStart(stringIteratorType start, stringIteratorType end, const stringType &routineName, bool includeMeta)
		{
			typedef std::basic_string<typename std::iterator_traits<stringIteratorType>::value_type> stdStringType;

			stdStringType startString = convertString<stdStringType>(";***AUTOMATIC DEFINITION START***");

			
			auto current = start;

			stdStringType labelToSearchFor = convertString<stdStringType>(routineName) +convertString<stdStringType>(":");

			stringIteratorType currentFindPos;

			while (current < end)
			{
				//static_assert(std::is_same<decltype(start), stdStringType::const_iterator>::value, "!!!");
				currentFindPos = std::search(current, end, startString.begin(), startString.end()); //uberASMFile.find(startString, current - uberASMFile.begin());
				if (currentFindPos == end)
					return Optional<stringIteratorType>();

				current = currentFindPos;

				auto labelPos = findLabel(current, end, false);
				if (labelPos != end)						// This *should* always be valid.  If it's not, something's gone wrong, but we'll ignore it just in case...
				{
					auto labelName = getLabel(labelPos, end);
					if (labelName == labelToSearchFor)
					{

						if (includeMeta == true)			// Get rid of the preceding newlines, if they exists.  They *should* always exist, but, y'know...
						{
							if (current - start >= 4 && *(current - 4) == '\r' && *(current - 3) == '\n' && *(current - 2) == '\r' && *(current - 1) == '\n')	// Two Windows newlines
								current -= 4;
							else if (current - start >= 2 && *(current - 2) == '\n' && *(current - 1) == '\n')								// Two Unix newlines
								current -= 2;
							else if (current - start >= 2 && *(current - 2) == '\r' && *(current - 1) == '\n')								// One Windows newline
								current -= 2;
							else if (current - start >= 1 && *(current - 1) == '\n')										// One Unix newline
								current -= 1;
						}
						else
						{
							current += startString.length();
						}

						return current;

					}
				}

				current++;							// Needed to skip past the current *** tag.
			}

			return Optional<stringIteratorType>();

		}

		template <typename stringIteratorType, typename stringType>
		inline Optional<stringIteratorType> getRoutineDefinitionsEnd(stringIteratorType start, stringIteratorType end, const stringType &routineName, bool includeMeta)
		{
			typedef std::basic_string<typename std::iterator_traits<stringIteratorType>::value_type> stdStringType;

			stdStringType endString = convertString<stdStringType>(";***AUTOMATIC DEFINITION END***");

			auto startPos = getRoutineDefinitionsStart(start, end, routineName, includeMeta);
			if (startPos.isValid() == false)
				return Optional<stringIteratorType>();

			auto endPos = std::search(startPos.get(), end, endString.begin(), endString.end()); //uberASMFile.find(endString, startPos.get() - uberASMFile.begin());
			if (endPos == end)
				return Optional<stringIteratorType>();

			if (includeMeta)
				return endPos + endString.length();
			else
				return endPos;
		}

		template <typename stringIteratorType, typename stringType>
		inline Optional<stringIteratorType> getLevelRoutineCallStart(stringIteratorType start, stringIteratorType end, const stringType &routineName, int level)
		{
			typedef std::basic_string<typename std::iterator_traits<stringIteratorType>::value_type> stdStringType;

			stdStringType routineCallJSR = convertString<stdStringType>("JSR ") + convertString<stdStringType>(routineName);
			stdStringType routineCallJSL = convertString<stdStringType>("JSL ") + convertString<stdStringType>(routineName);

			auto callsStart = internal::getLevelRoutineCallsStart(start, end, level, false);
			auto callsEnd = internal::getLevelRoutineCallsEnd(start, end, level, false);

			if (callsStart.isValid() == false || callsEnd.isValid() == false)
				return Optional<stringIteratorType>();

			auto labelPositionJSR = std::search(callsStart.get(), end, routineCallJSR.begin(), routineCallJSR.end()); //uberASMFile.find(routineCallJSR, callsStart.get() - uberASMFile.begin());
			auto labelPositionJSL = std::search(callsStart.get(), end, routineCallJSL.begin(), routineCallJSL.end()); //uberASMFile.find(routineCallJSL, callsStart.get() - uberASMFile.begin());

			if (labelPositionJSR != end)
				return labelPositionJSR;
			else if (labelPositionJSL != end)
				return labelPositionJSL;
			else
				return Optional<stringIteratorType>();
		}

		template <typename stringIteratorType, typename stringType>
		inline Optional<stringIteratorType> getLevelRoutineCallEnd(stringIteratorType start, stringIteratorType end, const stringType &routineName, int level)
		{
			typedef std::basic_string<typename std::iterator_traits<stringIteratorType>::value_type> stdStringType;

			stdStringType routine = convertString<stdStringType>("JSR ") + convertString<stdStringType>(routineName) + internal::getNewline<stdStringType>(start, end);
			auto callStart = getLevelRoutineCallStart(start, end, routineName, level);
			if (callStart.isValid() == false)
				return Optional<stringIteratorType>();

			return callStart.get() + routine.length();
		}

	}


	template <typename uberASMFileType, typename codeLabelStringType>
	inline void addRoutineCall(uberASMFileType &uberASMFile, int level, const codeLabelStringType &codeLabel, RoutineCallType callType)
	{
		typedef uberASMFileType stdStringType;

		stdStringType routineString;
		if (callType == JSR)
			routineString = internal::convertString<stdStringType>("JSR ");
		else if (callType == JSL)
			routineString = internal::convertString<stdStringType>("JSL ");

		routineString += codeLabel + internal::getNewline<stdStringType>(uberASMFile.begin(), uberASMFile.end());

		auto callSectionEnd = internal::getLevelRoutineCallsEnd(uberASMFile.begin(), uberASMFile.end(), level, true);
			
		if (callSectionEnd.isValid() == false)
		{
			// There's no section for this level.  Add it.
			stdStringType sectionString = internal::getNewline<stdStringType>(uberASMFile) + internal::convertString<stdStringType>(";***AUTOMATIC CALLS START***") + internal::getNewline(uberASMFile);
			sectionString += routineString;
			sectionString += internal::convertString<stdStringType>(";***AUTOMATIC CALLS END***") + internal::getNewline(uberASMFile);
			auto levelCodeStart = internal::getLevelStart(uberASMFile.begin(), uberASMFile.end(), level, true);
			if (levelCodeStart.isValid() == false)
				throw PatchLabelMissingException();

			uberASMFile.insert(levelCodeStart.get() - uberASMFile.begin(), sectionString);
		}
		else
		{
			uberASMFile.insert(callSectionEnd.get() - uberASMFile.begin(), routineString);
		}
	}


	template <typename uberASMFileType, typename codeLabelStringType>
	inline void removeRoutineCall(uberASMFileType &uberASMFile, const codeLabelStringType &codeLabel, int level)
	{
		auto routineStart = internal::getLevelRoutineCallStart(uberASMFile.begin(), uberASMFile.end(), codeLabel, level);
		auto routineEnd = internal::getLevelRoutineCallEnd(uberASMFile.begin(), uberASMFile.end(), codeLabel, level);
		if (routineStart.isValid() == false || routineEnd.isValid() == false)
			return;

		if (routineStart.isValid() && routineEnd.isValid())
			uberASMFile.erase(routineStart.get(), routineEnd.get());
	}


	template <typename uberASMFileType, typename codeLabelStringType, typename codeStringType>
	inline void addRoutineDefinition(uberASMFileType &uberASMFile, const codeLabelStringType &codeLabel, const codeStringType &code)
	{
		typedef uberASMFileType stdStringType;

		removeRoutineDefinition(uberASMFile, codeLabel);
		stdStringType routineText = internal::getNewline(uberASMFile) + internal::getNewline(uberASMFile) + internal::convertString<stdStringType>(";***AUTOMATIC DEFINITION START***") + internal::getNewline(uberASMFile);
		routineText += internal::convertString<stdStringType>(codeLabel) +internal::convertString<stdStringType>(":") + internal::getNewline(uberASMFile);
		routineText += code + internal::getNewline(uberASMFile);
		routineText += internal::convertString<stdStringType>(";***AUTOMATIC DEFINITION END***");

		uberASMFile += routineText;
	}

	template <typename uberASMFileType, typename codeLabelStringType>
	inline void removeRoutineDefinition(uberASMFileType &uberASMFile, const codeLabelStringType &codeLabel)
	{
		typedef uberASMFileType stdStringType;

		auto routineStart = internal::getRoutineDefinitionsStart(uberASMFile.begin(), uberASMFile.end(), codeLabel, true);
		auto routineEnd = internal::getRoutineDefinitionsEnd(uberASMFile.begin(), uberASMFile.end(), codeLabel, true);
		if (routineStart.isValid() && routineEnd.isValid())
			uberASMFile.erase(routineStart.get(), routineEnd.get());
	}


	template <typename uberASMFileType, typename codeLabelStringType>
	inline bool levelCallsRoutine(const uberASMFileType &uberASMFile, int level, const codeLabelStringType &codeLabel)
	{
		typedef uberASMFileType stdStringType;

		auto routineStart = internal::getLevelRoutineCallStart(uberASMFile.begin(), uberASMFile.end(), codeLabel, level);
		auto routineEnd = internal::getLevelRoutineCallEnd(uberASMFile.begin(), uberASMFile.end(), codeLabel, level);
		if (routineStart.isValid() && routineEnd.isValid())
			return true;
		else
			return false;
	}

	template <typename uberASMFileType, typename codeLabelStringType>
	inline bool routineIsDefined(const uberASMFileType &uberASMFile, const codeLabelStringType &codeLabel)
	{
		typedef uberASMFileType stdStringType;

		auto routineStart = internal::getRoutineDefinitionsStart(uberASMFile.begin(), uberASMFile.end(), codeLabel, true);
		auto routineEnd = internal::getRoutineDefinitionsEnd(uberASMFile.begin(), uberASMFile.end(), codeLabel, true);
		if (routineStart.isValid() && routineEnd.isValid())
			return true;
		else
			return false;
	}


	template <typename uberASMFileType>
	inline uberASMFileType getLevelCode(const uberASMFileType &uberASMFile, int level)
	{
		typedef uberASMFileType stdStringType;

		auto codeStart = internal::getLevelRoutineCallsEnd(uberASMFile.begin(), uberASMFile.end(), level, false);
		if (codeStart.isValid() == false)
			codeStart = internal::getLevelStart(uberASMFile.begin(), uberASMFile.end(), level, true);
		auto codeEnd = internal::getLevelEnd(uberASMFile.begin(), uberASMFile.end(), level, true);

		if (codeStart.isValid() == false || codeEnd.isValid() == false)
			throw PatchLabelMissingException();

		stdStringType code = stdStringType(codeStart.get(), codeEnd.get());
		//if (int newlineLength = internal::stringStartsWithNewline(code))
		//	code.erase(0, newlineLength);
		return code;
	}

	template <typename uberASMFileType, typename codeStringType>
	inline void setLevelCode(uberASMFileType &uberASMFile, int level, const codeStringType &code)
	{
		typedef uberASMFileType stdStringType;

		auto codeStart = internal::getLevelRoutineCallsEnd(uberASMFile.begin(), uberASMFile.end(), level, false);
		if (codeStart.isValid() == false)
			codeStart = internal::getLevelStart(uberASMFile.begin(), uberASMFile.end(), level, true);
		auto codeEnd = internal::getLevelEnd(uberASMFile.begin(), uberASMFile.end(), level, true);

		stdStringType actualCode = code;

		// Add a newline to the code if it doesn't start with one (so we don't get level0:LDA $19 or something), but only if the code does not start with whitespace.
		if (internal::codeStartsWithWhitespace(code) == false)
		{
			if (internal::stringStartsWithNewline(actualCode.begin(), actualCode.end()) == false)
				actualCode = internal::getNewline(uberASMFile) + actualCode;
		}

		// Add a newline to the code it it doesn't end with one (same as above, but also with comments to worry about)
		if (internal::stringEndsWithNewline(code) == false)
		{
			if (internal::stringEndsWithNewline(actualCode.begin(), actualCode.end()) == false)
				actualCode += internal::getNewline(uberASMFile);
		}

		if (codeStart.isValid() && codeEnd.isValid())
			uberASMFile.replace(codeStart.get(), codeEnd.get(), actualCode);
		else
			throw UberASMCodeMissing();
	}

}