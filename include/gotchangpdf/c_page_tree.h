#ifndef _C_PAGE_TREE_H
#define _C_PAGE_TREE_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	GOTCHANG_PDF_API error_type CALLING_CONVENTION PageTree_GetPageCount(PageTreeHandle handle, out_integer_type result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION PageTree_GetPage(PageTreeHandle handle, integer_type at, PPageObjectHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION PageTree_Release(PageTreeHandle handle);

#ifdef __cplusplus
};
#endif

#endif /* _C_PAGE_TREE_H */
