#ifndef _C_PAGE_OBJECT_H
#define _C_PAGE_OBJECT_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	GOTCHANG_PDF_API error_type CALLING_CONVENTION PageObject_CreateFromDocument(DocumentHandle handle, PPageObjectHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION PageObject_CreateFromObject(DictionaryObjectHandle handle, PPageObjectHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION PageObject_GetContents(PageObjectHandle handle, PContentsHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION PageObject_GetParent(PageObjectHandle handle, PPageTreeHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION PageObject_GetResources(PageObjectHandle handle, PResourceDictionaryHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION PageObject_GetAnnotations(PageObjectHandle handle, PPageAnnotationsHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION PageObject_GetMediaBox(PageObjectHandle handle, PRectangleHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION PageObject_Release(PageObjectHandle handle);

#ifdef __cplusplus
};
#endif

#endif /* _C_PAGE_OBJECT_H */
