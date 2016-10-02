#ifndef _C_RESOURCE_DICTIONARY_H
#define _C_RESOURCE_DICTIONARY_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	GOTCHANG_PDF_API error_type CALLING_CONVENTION ResourceDictionary_GetFontMap(ResourceDictionaryHandle handle, PFontMapHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ResourceDictionary_Release(ResourceDictionaryHandle handle);

#ifdef __cplusplus
};
#endif

#endif /* _C_RESOURCE_DICTIONARY_H */
