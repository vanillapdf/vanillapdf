#ifndef _C_PAGE_TREE_H
#define _C_PAGE_TREE_H

#include "c_export.h"
#include "c_handles.h"

#ifdef __cplusplus
extern "C"
{
#endif

	GOTCHANG_PDF_API int CALLING_CONVENTION PageTree_GetPageCount(PageTreeHandle);
	GOTCHANG_PDF_API PageObjectHandle CALLING_CONVENTION PageTree_GetPage(PageTreeHandle, int at);
	GOTCHANG_PDF_API void CALLING_CONVENTION PageTree_Release(PageTreeHandle);

#ifdef __cplusplus
};
#endif

#endif /* _C_PAGE_TREE_H */
