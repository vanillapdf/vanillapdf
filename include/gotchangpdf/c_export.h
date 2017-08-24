#ifndef _C_EXPORT_H
#define _C_EXPORT_H

#include "c_platform.h"

/**
* \file c_export.h
* This file contains macro declarations for importing
* and exporting symbols from library boundaries.
*/

#if defined(COMPILER_MICROSOFT_VISUAL_STUDIO) && defined(ENVIRONMENT_32_BIT)
	#define CALLING_CONVENTION __cdecl
#elif defined(COMPILER_GCC) && defined(ENVIRONMENT_32_BIT)
	#define CALLING_CONVENTION __attribute__ ((cdecl))
#else
	#define CALLING_CONVENTION
#endif

#if defined(GOTCHANG_PDF_CONFIGURATION_DLL) && defined(COMPILER_MICROSOFT_VISUAL_STUDIO)
	#if defined(GOTCHANG_PDF_EXPORTS)
		#define GOTCHANG_PDF_API __declspec(dllexport)
	#else
		#define GOTCHANG_PDF_API __declspec(dllimport)
	#endif
#else
	#define GOTCHANG_PDF_API
#endif

#endif /* _C_EXPORT_H */
