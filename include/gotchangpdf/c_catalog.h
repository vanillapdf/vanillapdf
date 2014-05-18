#ifndef _C_CATALOG_H
#define _C_CATALOG_H

#include "c_export.h"
#include "c_handles.h"

#ifdef __cplusplus
extern "C"
{
#endif

	GOTCHANG_PDF_API PageTreeHandle CALLING_CONVENTION Catalog_GetPages(CatalogHandle);

#ifdef __cplusplus
};
#endif

#endif /* _C_CATALOG_H */
