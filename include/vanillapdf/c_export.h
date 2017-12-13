#ifndef _C_EXPORT_H
#define _C_EXPORT_H

#include "c_platform.h"

/**
* \file c_export.h
* \brief This file contains macro declarations for importing
* and exporting symbols from library boundaries.
*/

#if defined(COMPILER_MICROSOFT_VISUAL_STUDIO) && defined(ENVIRONMENT_32_BIT)
	#define CALLING_CONVENTION __cdecl
#elif defined(COMPILER_GCC) && defined(ENVIRONMENT_32_BIT)
	#define CALLING_CONVENTION __attribute__ ((cdecl))
#else
	#define CALLING_CONVENTION
#endif

#if defined(VANILLAPDF_CONFIGURATION_DLL) && defined(COMPILER_MICROSOFT_VISUAL_STUDIO)
	#if defined(VANILLAPDF_EXPORTS)
		#define VANILLAPDF_API __declspec(dllexport)
	#else
		#define VANILLAPDF_API __declspec(dllimport)
	#endif
#else
	#define VANILLAPDF_API
#endif

#endif /* _C_EXPORT_H */
