#ifndef _C_DEVELOPER_EXTENSIONS_H
#define _C_DEVELOPER_EXTENSIONS_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#include "c_version.h"

#ifdef __cplusplus
extern "C"
{
#endif

	GOTCHANG_PDF_API error_type CALLING_CONVENTION DeveloperExtensionsIterator_GetKey(DeveloperExtensionsIteratorHandle handle, PNameObjectHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION DeveloperExtensionsIterator_GetValue(DeveloperExtensionsIteratorHandle handle, PDeveloperExtensionHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION DeveloperExtensionsIterator_IsValid(DeveloperExtensionsIteratorHandle handle, DeveloperExtensionsHandle parent, out_boolean_type result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION DeveloperExtensionsIterator_Next(DeveloperExtensionsIteratorHandle handle);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION DeveloperExtensionsIterator_Release(DeveloperExtensionsIteratorHandle handle);

	GOTCHANG_PDF_API error_type CALLING_CONVENTION DeveloperExtensions_Iterator(DeveloperExtensionsHandle handle, PDeveloperExtensionsIteratorHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION DeveloperExtensions_Release(DeveloperExtensionsHandle handle);

	GOTCHANG_PDF_API error_type CALLING_CONVENTION DeveloperExtension_GetBaseVersion(DeveloperExtensionHandle handle, PPDFVersion result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION DeveloperExtension_GetExtensionLevel(DeveloperExtensionHandle handle, PIntegerObjectHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION DeveloperExtension_Release(DeveloperExtensionHandle handle);

#ifdef __cplusplus
};
#endif

#endif /* _C_DEVELOPER_EXTENSIONS_H */
