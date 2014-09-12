#pragma once

//////////////////////////////////////////////////////////////////////////////
/// \file LunarMagic.hpp
/// \brief Contains various functions for working with Lunar Magic hacks
///
/// \addtogroup LunarMagic
///  @{
//////////////////////////////////////////////////////////////////////////////

namespace worldlib
{

	////////////////////////////////////////////////////////////
	/// \ingroup LunarMagic
	/// \brief Returns true if the library's version matches Lunar Magic's version.  If it returns false, you should warn the user about possible data corruption if you use any features that require compliance with hardcoded LM values.
	///
	/// \param romStart		An iterator pointing to the start of the ROM data
	/// \param romEnd		An iterator pointing to the end of the ROM data
	///
	/// \return True if the library is guaranteed to work with the version of Lunar Magic the user is using.
	///
	/// \throws DataNotFoundException	The ROM did not contain a necessary address or was too small to determine mapping. In either case it's most likely corrupted. 
	///
	////////////////////////////////////////////////////////////
	template <typename inputIteratorType>
	bool checkROMVersion(inputIteratorType romStart, inputIteratorType romEnd);
	
	////////////////////////////////////////////////////////////
	/// \ingroup LunarMagic
	/// \brief Tests if the library works on this version of the ROM
	/// \details This means that the ROM has been modified by Lunar Magic, expanded, is a US ROM (i.e. not a Japanese or European ROM), and its title is "SUPER MARIOWORLD      ".
	///
	/// \param romStart		An iterator pointing to the start of the ROM data
	/// \param romEnd		An iterator pointing to the end of the ROM data
	///
	/// \return True if the correct conditions apply
	///
	/// \throws DataNotFoundException	The ROM did not contain a necessary address or was too small to determine mapping. In either case it's most likely corrupted. 
	///
	////////////////////////////////////////////////////////////
	template <typename inputIteratorType>
	bool checkROMValid(inputIteratorType romStart, inputIteratorType romEnd);

	////////////////////////////////////////////////////////////
	/// \ingroup LunarMagic
	/// \brief Gets the string Lunar Magic inserts into the ROM as an identifier.  General format seems to be "Lunar Magic Version 2.21 Public ©2013 FuSoYa, Defender of Relm http://fusoya.eludevisibility.org                                ".  Probably won't need to use this, since checkROMModified and checkROMVersion should provide the same functionality.
	///
	/// \param romStart		An iterator pointing to the start of the ROM data
	/// \param romEnd		An iterator pointing to the end of the ROM data
	/// \param out			Where to send the data
	///
	/// \return Iterator pointing to the end of your string data
	///
	/// \throws DataNotFoundException	The ROM did not contain a necessary address or was too small to determine mapping. In either case it's most likely corrupted. 
	///
	////////////////////////////////////////////////////////////
	template <typename inputIteratorType, typename outputIteratorType>
	outputIteratorType getLunarMagicString(inputIteratorType romStart, inputIteratorType romEnd, outputIteratorType out);

}

#include "LunarMagic.inl"


//////////////////////////////////////////////////////////////////////////////
///  @}
//////////////////////////////////////////////////////////////////////////////