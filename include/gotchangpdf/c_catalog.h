#ifndef _C_CATALOG_H
#define _C_CATALOG_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	typedef enum PDFVersionTag {
		PDFVersion_10 = 0,
		PDFVersion_11 = 1,
		PDFVersion_12 = 2,
		PDFVersion_13 = 3,
		PDFVersion_14 = 4,
		PDFVersion_15 = 5,
		PDFVersion_16 = 6,
		PDFVersion_17 = 7
	} PDFVersion, *PPDFVersion;

	GOTCHANG_PDF_API error_type CALLING_CONVENTION Catalog_GetPages(CatalogHandle handle, PPageTreeHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Catalog_GetVersion(CatalogHandle handle, PPDFVersion result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Catalog_Release(CatalogHandle handle);

#ifdef __cplusplus
};
#endif

#endif /* _C_CATALOG_H */
