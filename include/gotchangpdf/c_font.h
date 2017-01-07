#ifndef _C_FONT_H
#define _C_FONT_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	typedef enum
	{
		FontType_Composite = 0,
		FontType_Type1,
		FontType_Type3
	} FontType, *PFontType;

	GOTCHANG_PDF_API error_type CALLING_CONVENTION Font_Type(FontHandle handle, PFontType result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Font_Release(FontHandle handle);

	GOTCHANG_PDF_API error_type CALLING_CONVENTION Font_ToComposite(FontHandle handle, PCompositeFontHandle result);

	GOTCHANG_PDF_API error_type CALLING_CONVENTION CompositeFont_GetUnicodeMap(CompositeFontHandle handle, PUnicodeCharacterMapHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION CompositeFont_Release(CompositeFontHandle handle);

#ifdef __cplusplus
};
#endif

#endif /* _C_FONT_H */
