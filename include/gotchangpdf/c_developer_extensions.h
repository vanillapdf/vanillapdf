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

	GOTCHANG_PDF_API error_type CALLING_CONVENTION DeveloperExtensions_GetBaseVersion(DeveloperExtensionsHandle handle, PPDFVersion result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION DeveloperExtensions_GetExtensionLevel(DeveloperExtensionsHandle handle, PIntegerHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION DeveloperExtensions_Release(DeveloperExtensionsHandle handle);

#ifdef __cplusplus
};
#endif

#endif /* _C_DEVELOPER_EXTENSIONS_H */
