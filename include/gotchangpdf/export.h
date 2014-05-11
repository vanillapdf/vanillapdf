#ifndef _EXPORT_H
#define _EXPORT_H

#ifdef PROJECT_CONFIGURATION_DLL
	#ifdef PDF_EXPORTS
		#define GOTCHANG_PDF_API __declspec(dllexport)
	#else
		#define GOTCHANG_PDF_API __declspec(dllimport)
	#endif
#else
	#define GOTCHANG_PDF_API
#endif

#endif /* _EXPORT_H */
