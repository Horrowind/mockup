#include <string>
#include <cstdint>
#include <vector>

namespace worldlib
{
	template <typename StringCharType>
	class TableToken
	{
		enum Type
		{
			Identifier,
			Hex,
			Dec,
			Whitespace,
			Comment
		};
		int width;
		std::basic_string<StringCharType> stringRepresentation;

		std::uint32_t integerValue;
	public:
		bool isIntegerType();
		bool isIdentifierType();
		bool isHex();
		bool isDec();
		int getWidth();
		void setWidth(int newWidth);

		std::basic_string<StringCharType> getStringRepresentation();
		void setFullRepresentation(std::basic_string<StringCharType> &string);
		void setFullRepresentation(std::basic_string<StringCharType> &string, int width);
		void setStringRepresentation(std::basic_string<StringCharType> &string);
		void setStringRepresentation(std::basic_string<StringCharType> &string, int width);
		std::uint32_t getIntegerRepresentation();
	};

	template <typename StringCharType>
	class Table
	{
		std::vector<TableEntry> entries;

	};



}