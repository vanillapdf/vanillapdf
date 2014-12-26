#ifndef _C_PAGE_OBJECT_H
#define _C_PAGE_OBJECT_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	GOTCHANG_PDF_API error_type CALLING_CONVENTION PageObject_Release(PageObjectHandle handle);

#ifdef __cplusplus
};
#endif

#endif /* _C_PAGE_OBJECT_H */
