#ifndef _C_PAGE_LABELS_H
#define _C_PAGE_LABELS_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	typedef enum
	{
		NumberingStyle_Decimal = 0,
		NumberingStyle_UpperRoman,
		NumberingStyle_LowerRoman,
		NumberingStyle_UpperLetters,
		NumberingStyle_LowerLetters
	} NumberingStyle, *PNumberingStyle;

	GOTCHANG_PDF_API error_type CALLING_CONVENTION PageLabels_Contains(PageLabelsHandle handle, integer_type page_number, out_boolean_type result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION PageLabels_At(PageLabelsHandle handle, integer_type page_number, PPageLabelHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION PageLabels_Release(PageLabelsHandle handle);

	GOTCHANG_PDF_API error_type CALLING_CONVENTION PageLabel_P(PageLabelHandle handle, PStringObjectHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION PageLabel_St(PageLabelHandle handle, PIntegerObjectHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION PageLabel_S(PageLabelHandle handle, PNumberingStyle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION PageLabel_Release(PageLabelHandle handle);

#ifdef __cplusplus
};
#endif

#endif /* _C_PAGE_LABELS_H */
