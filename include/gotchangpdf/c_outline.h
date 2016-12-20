#ifndef _C_OUTLINE_H
#define _C_OUTLINE_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	typedef enum
	{
		OutlineType_Outline,
		OutlineType_Item
	} OutlineType, *POutlineType;

	GOTCHANG_PDF_API error_type CALLING_CONVENTION OutlineBase_GetType(OutlineBaseHandle handle, POutlineType result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION OutlineBase_ToOutline(OutlineBaseHandle handle, POutlineHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION OutlineBase_ToOutlineItem(OutlineBaseHandle handle, POutlineItemHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION OutlineBase_Release(OutlineBaseHandle handle);

	GOTCHANG_PDF_API error_type CALLING_CONVENTION Outline_GetFirst(OutlineHandle handle, POutlineItemHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Outline_GetLast(OutlineHandle handle, POutlineItemHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Outline_GetCount(OutlineHandle handle, PIntegerObjectHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Outline_Release(OutlineHandle handle);

	GOTCHANG_PDF_API error_type CALLING_CONVENTION OutlineItem_GetTitle(OutlineItemHandle handle, PStringObjectHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION OutlineItem_GetParent(OutlineItemHandle handle, POutlineBaseHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION OutlineItem_GetPrev(OutlineItemHandle handle, POutlineItemHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION OutlineItem_GetNext(OutlineItemHandle handle, POutlineItemHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION OutlineItem_GetFirst(OutlineItemHandle handle, POutlineItemHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION OutlineItem_GetLast(OutlineItemHandle handle, POutlineItemHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION OutlineItem_GetCount(OutlineItemHandle handle, PIntegerObjectHandle result);
	//GOTCHANG_PDF_API error_type CALLING_CONVENTION OutlineItem_GetDestinations(OutlineItemHandle handle, PIntegerObjectHandle result);
	//GOTCHANG_PDF_API error_type CALLING_CONVENTION OutlineItem_GetActions(OutlineItemHandle handle, PIntegerObjectHandle result);
	//GOTCHANG_PDF_API error_type CALLING_CONVENTION OutlineItem_GetStructureElement(OutlineItemHandle handle, PIntegerObjectHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION OutlineItem_GetColor(OutlineItemHandle handle, POutlineItemColorHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION OutlineItem_GetFlags(OutlineItemHandle handle, POutlineItemFlagsHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION OutlineItem_Release(OutlineItemHandle handle);

	GOTCHANG_PDF_API error_type CALLING_CONVENTION OutlineItemColor_GetRed(OutlineItemColorHandle handle, PIntegerObjectHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION OutlineItemColor_GetGreen(OutlineItemColorHandle handle, PIntegerObjectHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION OutlineItemColor_GetBlue(OutlineItemColorHandle handle, PIntegerObjectHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION OutlineItemColor_Release(OutlineItemColorHandle handle);

	GOTCHANG_PDF_API error_type CALLING_CONVENTION OutlineItemFlags_IsItalic(OutlineItemFlagsHandle handle, out_boolean_type result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION OutlineItemFlags_IsBold(OutlineItemFlagsHandle handle, out_boolean_type result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION OutlineItemFlags_Release(OutlineItemFlagsHandle handle);

#ifdef __cplusplus
};
#endif

#endif /* _C_OUTLINE_H */
