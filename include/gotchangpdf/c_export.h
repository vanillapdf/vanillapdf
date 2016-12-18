#ifndef _C_EXPORT_H
#define _C_EXPORT_H

#if defined(_MSC_VER) && defined(_M_IX86)
	#define CALLING_CONVENTION __cdecl
#elif defined(__GNUC__) && defined(__i386)
	#define CALLING_CONVENTION __attribute__ ((cdecl))
#else
	#define CALLING_CONVENTION
#endif

#if defined(GOTCHANG_PDF_CONFIGURATION_DLL) && defined(_MSC_VER)
	#if defined(GOTCHANG_PDF_EXPORTS)
		#define GOTCHANG_PDF_API __declspec(dllexport)
	#else
		#define GOTCHANG_PDF_API __declspec(dllimport)
	#endif
#else
	#define GOTCHANG_PDF_API
#endif

#endif /* _C_EXPORT_H */