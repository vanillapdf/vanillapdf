#ifndef _C_FONT_MAP_H
#define _C_FONT_MAP_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	GOTCHANG_PDF_API error_type CALLING_CONVENTION FontMap_Contains(FontMapHandle handle, NameObjectHandle name_handle, out_boolean_type result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION FontMap_Find(FontMapHandle handle, NameObjectHandle name_handle, PFontHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION FontMap_Release(FontMapHandle handle);

#ifdef __cplusplus
};
#endif

#endif /* _C_FONT_MAP_H */
