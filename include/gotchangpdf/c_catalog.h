#ifndef _C_CATALOG_H
#define _C_CATALOG_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	GOTCHANG_PDF_API error_type CALLING_CONVENTION Catalog_GetPages(CatalogHandle handle, PPageTreeHandle result);

#ifdef __cplusplus
};
#endif

#endif /* _C_CATALOG_H */
