#ifndef _C_CATALOG_H
#define _C_CATALOG_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#include "c_version.h"

#ifdef __cplusplus
extern "C"
{
#endif

	GOTCHANG_PDF_API error_type CALLING_CONVENTION Catalog_GetPages(CatalogHandle handle, PPageTreeHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Catalog_GetVersion(CatalogHandle handle, PPDFVersion result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Catalog_GetExtensions(CatalogHandle handle, PDeveloperExtensionsHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Catalog_Release(CatalogHandle handle);

#ifdef __cplusplus
};
#endif

#endif /* _C_CATALOG_H */
