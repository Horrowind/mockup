#pragma once

#include <string>

namespace worldlib
{

	//////////////////////////////////////////////////////////////////////////////
	/// \file UberASM.hpp
	/// \brief Contains various functions for working with uberASM and adding code on a per-level basis.
	///
	/// \addtogroup UberASM
	///  @{
	//////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////
	/// \brief Represents how a routine can be called
	////////////////////////////////////////////////////////////
	enum RoutineCallType
	{
		JSR,
		JSL
	};

	/////////////////////////////////////////////////////////////////////////////
	/// \brief Tells a level that it should call a certain routine.
	/// \details Note that you should ensure that the routine is defined as well.  Also note that these calls run before 
	///
	/// \param uberASMFile		The the uberASM file to modify. level_code.asm and level_init_code.asm are both valid.
	/// \param codeLabel		The code label to const codeLabelStringType &codeLabel.  "UploadGFX" or something like that.
	/// \param level		The level to const codeLabelStringType &codeLabel
	/// \param callType		How to call the routine; either with a JSR or a JSL.
	///
	/// \throws std::runtime_error If there was an error with the file (such as missing level information)
	///
	/////////////////////////////////////////////////////////////////////////////
	template <typename uberASMFileType, typename codeLabelStringType>
	inline void addRoutineCall(uberASMFileType &uberASMFile, int level, const codeLabelStringType &codeLabel, RoutineCallType callType);

	/////////////////////////////////////////////////////////////////////////////
	/// \brief Removes a level's call to a given routine, if it calls it.
	///
	/// \param uberASMFile		The the uberASM file to modify. level_code.asm and level_init_code.asm are both valid.
	/// \param codeLabel		The code label to const codeLabelStringType &codeLabel.  "UploadGFX" or something like that.
	/// \param level		The level to const codeLabelStringType &codeLabel
	///
	/// \throws std::runtime_error If there was an error with the file (such as missing level information)
	///
	/////////////////////////////////////////////////////////////////////////////
	template <typename uberASMFileType, typename codeLabelStringType>
	inline void removeRoutineCall(uberASMFileType &uberASMFile, int level, const codeLabelStringType &codeLabel);

	/////////////////////////////////////////////////////////////////////////////
	/// \brief Defines a routine for levels to const codeLabelStringType &codeLabel.
	/// \details If the routine ends in JLS, it can be const codeLabelStringType &codeLabeld in both level and level_init.
	///
	/// \param uberASMFile		The the uberASM file to modify. level_code.asm and level_init_code.asm are both valid.
	/// \param codeLabel		The code label to const codeLabelStringType &codeLabel.  "UploadGFX" or something like that.
	/// \param code			The code to assign to the label
	///
	/// \throws std::runtime_error If there was an error with the file (such as missing level information)
	///
	/////////////////////////////////////////////////////////////////////////////
	template <typename uberASMFileType, typename codeLabelStringType, typename codeStringType>
	inline void addRoutineDefinition(uberASMFileType &uberASMFile, const codeLabelStringType &codeLabel, const codeStringType &code);

	/////////////////////////////////////////////////////////////////////////////
	/// \brief Removes a routine's definition, if it exists
	/// \details Remember to call this for both level and level_init if necessary
	///
	/// \param uberASMFile		The the uberASM file to modify. level_code.asm and level_init_code.asm are both valid.
	/// \param codeLabel		The code label to const codeLabelStringType &codeLabel.  "UploadGFX" or something like that.
	///
	/// \throws std::runtime_error If there was an error with the file (such as missing level information)
	///
	/////////////////////////////////////////////////////////////////////////////
	template <typename uberASMFileType, typename codeLabelStringType>
	inline void removeRoutineDefinition(uberASMFileType &uberASMFile, const codeLabelStringType &codeLabel);


	/////////////////////////////////////////////////////////////////////////////
	/// \brief Determines if the level calls the specified routine.
	/// \details IMPORTANT: This only holds true for routines added through this library.
	///
	/// \param uberASMFile		The the uberASM file to modify. level_code.asm and level_init_code.asm are both valid.
	/// \param codeLabel		The code label to const codeLabelStringType &codeLabel.  "UploadGFX" or something like that.
	/// \param level		The level to test.
	///
	/// \return True if the level calls the specified routine, false otherwise
	///
	/// \throws std::runtime_error If there was an error with the file (such as missing level information)
	///
	/////////////////////////////////////////////////////////////////////////////
	template <typename uberASMFileType, typename codeLabelStringType>
	inline bool levelCallsRoutine(const uberASMFileType &uberASMFile, int level, const codeLabelStringType &codeLabel);

	/////////////////////////////////////////////////////////////////////////////
	/// \brief Determines if a given routine is defined.
	/// \details IMPORTANT: This only holds true for routines added through this library.  Also, be sure to call this for both level and level_init if necessary.
	///
	/// \param uberASMFile		The the uberASM file to modify. level_code.asm and level_init_code.asm are both valid.
	/// \param codeLabel		The code label to const codeLabelStringType &codeLabel.  "UploadGFX" or something like that.
	///
	/// \return True if the the routine is defined in the file.
	///
	/// \throws std::runtime_error If there was an error with the file (such as missing level information)
	///
	/////////////////////////////////////////////////////////////////////////////
	template <typename uberASMFileType, typename codeLabelStringType>
	inline bool routineIsDefined(const uberASMFileType &uberASMFile, const codeLabelStringType &codeLabel);


	/////////////////////////////////////////////////////////////////////////////
	/// \brief Returns the code a level runs
	/// \details This does NOT include routines added through this library with the above functions.
	///
	/// \param uberASMFile		The the uberASM file to modify. level_code.asm and level_init_code.asm are both valid.
	/// \param level		The level number to const codeLabelStringType &codeLabel.
	///
	/// \return The code associated with this level
	///
	/// \throws std::runtime_error If there was an error with the file (such as missing level information)
	///
	/////////////////////////////////////////////////////////////////////////////
	template <typename uberASMFileType>
	inline uberASMFileType getLevelCode(const uberASMFileType &uberASMFile, int level);

	/////////////////////////////////////////////////////////////////////////////
	/// \brief Sets the code a level runs.
	/// \details This does NOT include routines added through this library with the above functions.  They will remain untouched, no matter what.
	///
	/// \param uberASMFile		The the uberASM file to modify. level_code.asm and level_init_code.asm are both valid.
	/// \param level		The level number to const codeLabelStringType &codeLabel.
	/// \param code			The code to replace the current level's code with.
	///
	/// \return The code associated with this level
	///
	/// \throws std::runtime_error If there was an error with the file (such as missing level information)
	///
	/////////////////////////////////////////////////////////////////////////////
	template <typename uberASMFileType, typename codeStringType>
	inline void setLevelCode(uberASMFileType &uberASMFile, int level, const codeStringType &code);



	//////////////////////////////////////////////////////////////////////////////
	///  @}
	//////////////////////////////////////////////////////////////////////////////
}

#include "UberASM.inl"

