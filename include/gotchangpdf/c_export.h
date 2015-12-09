#ifndef _C_EXPORT_H
#define _C_EXPORT_H

#define CALLING_CONVENTION __cdecl

#ifdef PROJECT_CONFIGURATION_DLL
	#ifdef PDF_EXPORTS
		#define GOTCHANG_PDF_API __declspec(dllexport)
	#else
		#define GOTCHANG_PDF_API __declspec(dllimport)
	#endif
#else
	#define GOTCHANG_PDF_API
#endif

#endif /* _C_EXPORT_H */