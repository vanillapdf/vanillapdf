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

	typedef enum
	{
		PageLayout_SinglePage = 0,
		PageLayout_OneColumn,
		PageLayout_TwoColumnLeft,
		PageLayout_TwoColumnRight,
		PageLayout_TwoPageLeft,
		PageLayout_TwoPageRight
	} PageLayout, *PPageLayout;

	GOTCHANG_PDF_API error_type CALLING_CONVENTION Catalog_GetPages(CatalogHandle handle, PPageTreeHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Catalog_GetPageLabels(CatalogHandle handle, PPageLabelsHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Catalog_GetPageLayout(CatalogHandle handle, PPageLayout result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Catalog_GetVersion(CatalogHandle handle, PPDFVersion result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Catalog_GetExtensions(CatalogHandle handle, PDeveloperExtensionsHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Catalog_GetViewerPreferences(CatalogHandle handle, PViewerPreferencesHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Catalog_GetOutlines(CatalogHandle handle, POutlineHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Catalog_GetDestinations(CatalogHandle handle, PNamedDestinationsHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Catalog_Release(CatalogHandle handle);

#ifdef __cplusplus
};
#endif

#endif /* _C_CATALOG_H */
