#pragma once
#include <iterator>
#include <cstdint>

//////////////////////////////////////////////////////////////////////////////
/// \file ColorBackInserter.hpp
/// \brief Contains the definition for a class that deals with a color iterator
///
/// \addtogroup Level
///  @{
//////////////////////////////////////////////////////////////////////////////

namespace worldlib
{

	enum class ColorOrder : int;



	////////////////////////////////////////////////////////////
	/// \brief Iterator that behaves like std::back_insert_iterator.
	/// Basically, the = operator takes a full 32-bit color and rearranges it as necessary.
	/// Depending on the iterator type, it will either split up the 32-bit colors into their individual components (in case you need to interface with something that expects a pointer to chars and don't want to have to worry about endianness), or just insert it normally.
	////////////////////////////////////////////////////////////
	template<class ContainerType>
	class ColorBackInserterIterator : std::iterator<std::output_iterator_tag, void, void, void, void>
	{
	protected:
		////////////////////////////////////////////////////////////
		/// \brief The container this iterator belongs to
		////////////////////////////////////////////////////////////
		ContainerType *myContainer;


		////////////////////////////////////////////////////////////
		/// \brief The order incoming colors are put into
		////////////////////////////////////////////////////////////
		int colorOrder;

	public:


		////////////////////////////////////////////////////////////
		/// \brief The data type this iterator points to
		////////////////////////////////////////////////////////////
		typedef typename ContainerType::value_type typePointedTo;

		////////////////////////////////////////////////////////////
		/// \brief Creates a ColorBackInsertIterator from the specified container and tells it how to arrange the colors.
		///
		///
		/// \param otherContainer	The container to insert into
		/// \param order		The order to insert into.  For example, ColorOrder::ARGB.
		///
		////////////////////////////////////////////////////////////
		ColorBackInserterIterator(ContainerType& otherContainer, ColorOrder order) : myContainer(std::addressof(otherContainer)) { colorOrder = (int)order; }

		////////////////////////////////////////////////////////////
		/// \brief Inserts the color into the iterator's container.
		/// \details Automatically converts the color into the format requested when the iterator was constructed.
		/// If the container contains 8-bit data, then the 32-bit color is split into 4 bytes in the requested order.
		/// If the container contains 32-bit data, then the data is simply inserted after being rearranged in the requested order.
		///
		/// \param value ARGB color value to insert.
		///
		/// \return The current structure (*this)
		///
		////////////////////////////////////////////////////////////
		ColorBackInserterIterator<ContainerType> &operator=(std::uint32_t value)
		{
			std::uint8_t colors[] = { (value & 0xFF000000) >> 24, (value & 0x00FF0000) >> 16, (value & 0x0000FF00) >> 8, (value & 0x000000FF) >> 0 };

			std::uint8_t slot1 = colors[(colorOrder & 0xC0) >> 6];
			std::uint8_t slot2 = colors[(colorOrder & 0x30) >> 4];
			std::uint8_t slot3 = colors[(colorOrder & 0x0C) >> 2];
			std::uint8_t slot4 = colors[(colorOrder & 0x03) >> 0];

			if (std::numeric_limits<typePointedTo>::max() - std::numeric_limits<typePointedTo>::min() == std::numeric_limits<std::uint8_t>::max())
			{
				myContainer->push_back(slot1);
				myContainer->push_back(slot2);
				myContainer->push_back(slot3);
				myContainer->push_back(slot4);
			}
			else
			{
				myContainer->push_back((slot1 << 24) | (slot2 << 16) | (slot3 << 8) | (slot4 << 0));
			}
			return *this;
		}

		////////////////////////////////////////////////////////////
		/// \brief Does nothing.  As with a normal std::back_insert_iterator, simply exists to satisfy the requirements of an iterator.
		////////////////////////////////////////////////////////////
		ColorBackInserterIterator<ContainerType> &operator*() { return (*this); }

		////////////////////////////////////////////////////////////
		/// \brief Does nothing.  As with a normal std::back_insert_iterator, simply exists to satisfy the requirements of an iterator.
		////////////////////////////////////////////////////////////
		ColorBackInserterIterator<ContainerType> &operator++(){ return (*this); }

		////////////////////////////////////////////////////////////
		/// \brief Does nothing.  As with a normal std::back_insert_iterator, simply exists to satisfy the requirements of an iterator.
		////////////////////////////////////////////////////////////
		ColorBackInserterIterator<ContainerType> operator++(int) { return (*this); }
	};



	////////////////////////////////////////////////////////////
	/// \relates ColorBackInserterIterator
	/// \brief Returns a ColorBackInserterIterator.  
	/// \details Convenience function in the same vein of std::back_inserter
	///		
	/// \param container		The container to insert into.  For example, a std::vector<unsigned char> or a std::vector<std::uint32_t>.
	/// \param order		The order to insert into.  For example, ColorOrder::ARGB.	
	///
	////////////////////////////////////////////////////////////
	template<class containerType> 
	inline ColorBackInserterIterator<containerType> ColorBackInserter(containerType &container, ColorOrder order)
	{
		return (ColorBackInserterIterator<containerType>(container, order));
	}

	////////////////////////////////////////////////////////////
	/// \relates ColorBackInserterIterator
	/// \brief The order colors are stored in a ColorBackInsertIterator.
	/// \details Colors specified multiple times (for example, AAAA), will just copy that part.
	////////////////////////////////////////////////////////////
	enum class ColorOrder : int
	{
		AAAA = 0x00,
		AAAR = 0x01,
		AAAG = 0x02,
		AAAB = 0x03,
		AARA = 0x04,
		AARR = 0x05,
		AARG = 0x06,
		AARB = 0x07,
		AAGA = 0x08,
		AAGR = 0x09,
		AAGG = 0x0A,
		AAGB = 0x0B,
		AABA = 0x0C,
		AABR = 0x0D,
		AABG = 0x0E,
		AABB = 0x0F,
		ARAA = 0x10,
		ARAR = 0x11,
		ARAG = 0x12,
		ARAB = 0x13,
		ARRA = 0x14,
		ARRR = 0x15,
		ARRG = 0x16,
		ARRB = 0x17,
		ARGA = 0x18,
		ARGR = 0x19,
		ARGG = 0x1A,
		ARGB = 0x1B,
		ARBA = 0x1C,
		ARBR = 0x1D,
		ARBG = 0x1E,
		ARBB = 0x1F,
		AGAA = 0x20,
		AGAR = 0x21,
		AGAG = 0x22,
		AGAB = 0x23,
		AGRA = 0x24,
		AGRR = 0x25,
		AGRG = 0x26,
		AGRB = 0x27,
		AGGA = 0x28,
		AGGR = 0x29,
		AGGG = 0x2A,
		AGGB = 0x2B,
		AGBA = 0x2C,
		AGBR = 0x2D,
		AGBG = 0x2E,
		AGBB = 0x2F,
		ABAA = 0x30,
		ABAR = 0x31,
		ABAG = 0x32,
		ABAB = 0x33,
		ABRA = 0x34,
		ABRR = 0x35,
		ABRG = 0x36,
		ABRB = 0x37,
		ABGA = 0x38,
		ABGR = 0x39,
		ABGG = 0x3A,
		ABGB = 0x3B,
		ABBA = 0x3C,
		ABBR = 0x3D,
		ABBG = 0x3E,
		ABBB = 0x3F,
		RAAA = 0x40,
		RAAR = 0x41,
		RAAG = 0x42,
		RAAB = 0x43,
		RARA = 0x44,
		RARR = 0x45,
		RARG = 0x46,
		RARB = 0x47,
		RAGA = 0x48,
		RAGR = 0x49,
		RAGG = 0x4A,
		RAGB = 0x4B,
		RABA = 0x4C,
		RABR = 0x4D,
		RABG = 0x4E,
		RABB = 0x4F,
		RRAA = 0x50,
		RRAR = 0x51,
		RRAG = 0x52,
		RRAB = 0x53,
		RRRA = 0x54,
		RRRR = 0x55,
		RRRG = 0x56,
		RRRB = 0x57,
		RRGA = 0x58,
		RRGR = 0x59,
		RRGG = 0x5A,
		RRGB = 0x5B,
		RRBA = 0x5C,
		RRBR = 0x5D,
		RRBG = 0x5E,
		RRBB = 0x5F,
		RGAA = 0x60,
		RGAR = 0x61,
		RGAG = 0x62,
		RGAB = 0x63,
		RGRA = 0x64,
		RGRR = 0x65,
		RGRG = 0x66,
		RGRB = 0x67,
		RGGA = 0x68,
		RGGR = 0x69,
		RGGG = 0x6A,
		RGGB = 0x6B,
		RGBA = 0x6C,
		RGBR = 0x6D,
		RGBG = 0x6E,
		RGBB = 0x6F,
		RBAA = 0x70,
		RBAR = 0x71,
		RBAG = 0x72,
		RBAB = 0x73,
		RBRA = 0x74,
		RBRR = 0x75,
		RBRG = 0x76,
		RBRB = 0x77,
		RBGA = 0x78,
		RBGR = 0x79,
		RBGG = 0x7A,
		RBGB = 0x7B,
		RBBA = 0x7C,
		RBBR = 0x7D,
		RBBG = 0x7E,
		RBBB = 0x7F,
		GAAA = 0x80,
		GAAR = 0x81,
		GAAG = 0x82,
		GAAB = 0x83,
		GARA = 0x84,
		GARR = 0x85,
		GARG = 0x86,
		GARB = 0x87,
		GAGA = 0x88,
		GAGR = 0x89,
		GAGG = 0x8A,
		GAGB = 0x8B,
		GABA = 0x8C,
		GABR = 0x8D,
		GABG = 0x8E,
		GABB = 0x8F,
		GRAA = 0x90,
		GRAR = 0x91,
		GRAG = 0x92,
		GRAB = 0x93,
		GRRA = 0x94,
		GRRR = 0x95,
		GRRG = 0x96,
		GRRB = 0x97,
		GRGA = 0x98,
		GRGR = 0x99,
		GRGG = 0x9A,
		GRGB = 0x9B,
		GRBA = 0x9C,
		GRBR = 0x9D,
		GRBG = 0x9E,
		GRBB = 0x9F,
		GGAA = 0xA0,
		GGAR = 0xA1,
		GGAG = 0xA2,
		GGAB = 0xA3,
		GGRA = 0xA4,
		GGRR = 0xA5,
		GGRG = 0xA6,
		GGRB = 0xA7,
		GGGA = 0xA8,
		GGGR = 0xA9,
		GGGG = 0xAA,
		GGGB = 0xAB,
		GGBA = 0xAC,
		GGBR = 0xAD,
		GGBG = 0xAE,
		GGBB = 0xAF,
		GBAA = 0xB0,
		GBAR = 0xB1,
		GBAG = 0xB2,
		GBAB = 0xB3,
		GBRA = 0xB4,
		GBRR = 0xB5,
		GBRG = 0xB6,
		GBRB = 0xB7,
		GBGA = 0xB8,
		GBGR = 0xB9,
		GBGG = 0xBA,
		GBGB = 0xBB,
		GBBA = 0xBC,
		GBBR = 0xBD,
		GBBG = 0xBE,
		GBBB = 0xBF,
		BAAA = 0xC0,
		BAAR = 0xC1,
		BAAG = 0xC2,
		BAAB = 0xC3,
		BARA = 0xC4,
		BARR = 0xC5,
		BARG = 0xC6,
		BARB = 0xC7,
		BAGA = 0xC8,
		BAGR = 0xC9,
		BAGG = 0xCA,
		BAGB = 0xCB,
		BABA = 0xCC,
		BABR = 0xCD,
		BABG = 0xCE,
		BABB = 0xCF,
		BRAA = 0xD0,
		BRAR = 0xD1,
		BRAG = 0xD2,
		BRAB = 0xD3,
		BRRA = 0xD4,
		BRRR = 0xD5,
		BRRG = 0xD6,
		BRRB = 0xD7,
		BRGA = 0xD8,
		BRGR = 0xD9,
		BRGG = 0xDA,
		BRGB = 0xDB,
		BRBA = 0xDC,
		BRBR = 0xDD,
		BRBG = 0xDE,
		BRBB = 0xDF,
		BGAA = 0xE0,
		BGAR = 0xE1,
		BGAG = 0xE2,
		BGAB = 0xE3,
		BGRA = 0xE4,
		BGRR = 0xE5,
		BGRG = 0xE6,
		BGRB = 0xE7,
		BGGA = 0xE8,
		BGGR = 0xE9,
		BGGG = 0xEA,
		BGGB = 0xEB,
		BGBA = 0xEC,
		BGBR = 0xED,
		BGBG = 0xEE,
		BGBB = 0xEF,
		BBAA = 0xF0,
		BBAR = 0xF1,
		BBAG = 0xF2,
		BBAB = 0xF3,
		BBRA = 0xF4,
		BBRR = 0xF5,
		BBRG = 0xF6,
		BBRB = 0xF7,
		BBGA = 0xF8,
		BBGR = 0xF9,
		BBGG = 0xFA,
		BBGB = 0xFB,
		BBBA = 0xFC,
		BBBR = 0xFD,
		BBBG = 0xFE,
		BBBB = 0xFF,

	};


}


//////////////////////////////////////////////////////////////////////////////
///  @}
//////////////////////////////////////////////////////////////////////////////