#pragma once



//////////////////////////////////////////////////////////////////////////////
/// \file asardll.hpp
/// \brief Definition for functions found in Asar's dll.  You do not need to use this file manually.
///
/// \addtogroup Internal
///  @{
//////////////////////////////////////////////////////////////////////////////

#ifdef _WIN32
#define asarfunc __declspec(dllexport)
#endif

#ifndef asarfunc
#define asarfunc extern
#endif

// Under normal C++ function pointers are just declarations, whereas under CLI they're definitions.
// Or something like that...?
// Either way CLI needs this bizarre workaround.
// The function pointers are actually "defined" in AsarScapegoat.cpp for worldlibnet.
#ifdef __cplusplus_cli
#undef asarfunc
#define asarfunc extern
#endif

#ifdef CLI_FORCE_DEFINITIONS
#undef asarfunc
#define asarfunc
#endif

#define expectedapiversion 200

//#include <stdbool.h>

//These structures are returned from various functions.
struct errordata 
{
	const char * fullerrdata;
	const char * rawerrdata;
	const char * block;
	const char * filename;
	int line;
	const char * callerfilename;
	int callerline;
};

struct labeldata 
{
	const char * name;
	int location;
};

struct definedata 
{
	const char * name;
	const char * contents;
};

//Returns the version, in the format major*10000+minor*100+bugfix*1. This means that 1.2.34 would be
// returned as 10234.
int asarfunc (*asar_version)();

//Returns the API version, format major*100+minor. Minor is incremented on backwards compatible
// changes; major is incremented on incompatible changes. Does not have any correlation with the
// Asar version.
//It's not very useful directly, since asar_init() verifies this automatically.
//Calling this one also sets a flag that makes asar_init not instantly return false; this is so
// programs expecting an older API won't do anything unexpected.
int asarfunc(*asar_apiversion)();

//Initializes Asar. Call this before doing anything.
//If it returns false, something went wrong, and you may not use any other Asar functions. This is
//either due to not finding the library, or not finding all expected functions in the library.
bool asar_init();

//Clears out all errors, warnings and printed statements, and clears the file cache. Not really
// useful, since asar_patch() already does this.
bool asarfunc (*asar_reset)();

//Applies a patch. The first argument is a filename (so Asar knows where to look for incsrc'd
// stuff); however, the ROM is in memory.
//This function assumes there are no headers anywhere, neither in romdata nor the sizes. romlen may
// be altered by this function; if this is undesirable, set romlen equal to buflen.
//The return value is whether any errors appeared (false=errors, call asar_geterrors for details).
// If there is an error, romdata and romlen will be left unchanged.
bool asarfunc (*asar_patch)(const char * patchloc, char * romdata, int buflen, int * romlen);

//Returns the maximum possible size of the output ROM from asar_patch(). Giving this size to buflen
// guarantees you will not get any buffer too small errors; however, it is safe to give smaller
// buffers if you don't expect any ROMs larger than 4MB or something.
int asarfunc (*asar_maxromsize)();

//Frees all of Asar's structures and unloads the module. Only asar_init may be called after calling
// this; anything else will lead to segfaults.
void asar_close();

//Get a list of all errors.
//All pointers from these functions are valid only until the same function is called again, or until
// asar_patch, asar_reset or asar_close is called, whichever comes first. Copy the contents if you
// need it for a longer time.
const struct errordata asarfunc* (*asar_geterrors)(int * count);

//Get a list of all warnings.
const struct errordata asarfunc*(*asar_getwarnings)(int * count);

//Get a list of all printed data.
asarfunc const char * const * (*asar_getprints)(int * count);

//Get a list of all labels.
const struct labeldata asarfunc*(*asar_getalllabels)(int * count);

//Get the ROM location of one label. -1 means "not found".
int asarfunc(*asar_getlabelval)(const char * name);

//Gets the value of a define.
const char asarfunc * (*asar_getdefine)(const char * name);

//Gets the values and names of all defines.
const struct definedata asarfunc * (*asar_getalldefines)(int * count);

//Parses all defines in the parameter. The parameter controls whether it'll learn new defines in
// this string if it finds any. Note that it may emit errors.
const char asarfunc * (*asar_resolvedefines)(const char * data, bool learnnew);

//Parses a string containing math. It automatically assumes global scope (no namespaces), and has
// access to all functions and labels from the last call to asar_patch. Remember to check error to
// see if it's successful (NULL) or if it failed (non-NULL, contains a descriptive string). It does
// not affect asar_geterrors.
double asarfunc(*asar_math)(const char * math, const char ** error);







void asar_getlib();
bool asar_loadraw();
void asar_closelib();

#ifdef _WIN32
#include <tchar.h>		// Needed to get Windows to play nice with chars and wchars and tchars and crap

#include <windows.h>

#undef max			// Yay windows.h let's define crap that messes with simple STL functions
#undef min			// I mean seriously it's not like max's all that hard to define on our own.

#define getlib() LoadLibrary(_T("asar.dll"))
#define loadraw(name, target) {*((int(**)())&target)=(int(*)())GetProcAddress((HINSTANCE)asardll, name); if (target == nullptr) return false; }
#define closelib(var) FreeLibrary((HINSTANCE)var)

#else
#include <dlfcn.h>
#include <stdio.h>
#ifdef __APPLE__
#define EXTENSION ".dylib"
#else
#define EXTENSION ".so"
#endif
inline void * getlib()
{
	char libname[256];
	const char * names[] = { "./libasar"EXTENSION, "libasar", NULL };
	for (int i = 0; names[i]; i++)
	{
		void * rval = dlopen(names[i], RTLD_LAZY);
		const char*e = dlerror(); if (e)puts(e);
		if (rval) return rval;
	}
	return NULL;
}
#define loadraw(name, target) *(void **)(&target)=dlsym(asardll, name); require(target)
#define closelib(var) dlclose(var)
#endif


static void * asardll = NULL;

static bool(*asar_i_init)();
static void(*asar_i_close)();

inline bool asar_init()
{
#define require(b) if (!(b)) { asardll=NULL; return false; }
	if (asardll) 
		return true;
	asardll = getlib();
	
	if (asardll == nullptr) return false;

	loadraw("asar_init", asar_i_init);			if (asar_i_init == nullptr) return false;
	loadraw("asar_close", asar_i_close);			if (asar_i_close == nullptr) return false;

	loadraw("asar_version", asar_version);			if (asar_version == nullptr) return false;
	loadraw("asar_apiversion", asar_apiversion);		if (asar_apiversion == nullptr) return false;
	loadraw("asar_reset", asar_reset);			if (asar_reset == nullptr) return false;
	loadraw("asar_patch", asar_patch);			if (asar_patch == nullptr) return false;
	loadraw("asar_maxromsize", asar_maxromsize);		if (asar_maxromsize == nullptr) return false;
	loadraw("asar_geterrors", asar_geterrors);		if (asar_geterrors == nullptr) return false;
	loadraw("asar_getwarnings", asar_getwarnings);		if (asar_getwarnings == nullptr) return false;
	loadraw("asar_getprints", asar_getprints);		if (asar_getprints == nullptr) return false;
	loadraw("asar_getalllabels", asar_getalllabels);	if (asar_getalllabels == nullptr) return false;
	loadraw("asar_getlabelval", asar_getlabelval);		if (asar_getlabelval == nullptr) return false;
	loadraw("asar_getdefine", asar_getdefine);		if (asar_getdefine == nullptr) return false;
	loadraw("asar_getalldefines", asar_getalldefines);	if (asar_getalldefines == nullptr) return false;
	loadraw("asar_math", asar_math);			if (asar_math == nullptr) return false;



	if (asar_apiversion() < expectedapiversion || (asar_apiversion() / 100) > (expectedapiversion / 100)) 
		return false;

	if (asar_i_init() == false) 
	{
		asardll = nullptr;
		return false;
	}

	return true;
}

inline void asar_close()
{
	if (!asardll) 
		return;
	asar_i_close();
	closelib(asardll);
	asardll = NULL;
}

//////////////////////////////////////////////////////////////////////////////
///  @}
//////////////////////////////////////////////////////////////////////////////