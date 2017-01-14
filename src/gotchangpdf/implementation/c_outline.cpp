#include "precompiled.h"
#include "outline.h"
#include "file.h"

#include "gotchangpdf/c_outline.h"
#include "c_helper.h"

using namespace gotchangpdf;
using namespace gotchangpdf::semantics;

GOTCHANG_PDF_API error_type CALLING_CONVENTION OutlineBase_GetType(OutlineBaseHandle handle, OutlineType* result)
{
	OutlineBase* obj = reinterpret_cast<OutlineBase*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	switch (obj->OutlineType()) {
	case OutlineBase::Type::Outline:
		*result = OutlineType_Outline; break;
	case OutlineBase::Type::Item:
		*result = OutlineType_Item; break;
	default:
		return GOTCHANG_PDF_ERROR_GENERAL;
	}

	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION OutlineBase_ToOutline(OutlineBaseHandle handle, OutlineHandle* result)
{
	return SafeObjectConvert<OutlineBase, Outline, OutlineBaseHandle, OutlineHandle>(handle, result);
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION OutlineBase_ToOutlineItem(OutlineBaseHandle handle, OutlineItemHandle* result)
{
	return SafeObjectConvert<OutlineBase, OutlineItem, OutlineBaseHandle, OutlineItemHandle>(handle, result);
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION OutlineBase_Release(OutlineBaseHandle handle)
{
	return ObjectRelease<OutlineBase, OutlineBaseHandle>(handle);
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Outline_GetFirst(OutlineHandle handle, OutlineItemHandle* result)
{
	Outline* obj = reinterpret_cast<Outline*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		OutputOutlineItemPtr direct;
		auto contains = obj->First(direct);
		if (!contains) return GOTCHANG_PDF_ERROR_OPTIONAL_ENTRY_MISSING;
		auto ptr = direct.AddRefGet();
		*result = reinterpret_cast<OutlineItemHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Outline_GetLast(OutlineHandle handle, OutlineItemHandle* result)
{
	Outline* obj = reinterpret_cast<Outline*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		OutputOutlineItemPtr direct;
		auto contains = obj->Last(direct);
		if (!contains) return GOTCHANG_PDF_ERROR_OPTIONAL_ENTRY_MISSING;
		auto ptr = direct.AddRefGet();
		*result = reinterpret_cast<OutlineItemHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Outline_GetCount(OutlineHandle handle, IntegerObjectHandle* result)
{
	Outline* obj = reinterpret_cast<Outline*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		syntax::IntegerObjectPtr direct;
		auto contains = obj->Count(direct);
		if (!contains) return GOTCHANG_PDF_ERROR_OPTIONAL_ENTRY_MISSING;
		auto ptr = direct.AddRefGet();
		*result = reinterpret_cast<IntegerObjectHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Outline_Release(OutlineHandle handle)
{
	return ObjectRelease<Outline, OutlineHandle>(handle);
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION OutlineItem_GetTitle(OutlineItemHandle handle, StringObjectHandle* result)
{
	OutlineItem* obj = reinterpret_cast<OutlineItem*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto value = obj->Title();
		auto ptr = value.AddRefGet();
		*result = reinterpret_cast<StringObjectHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION OutlineItem_GetParent(OutlineItemHandle handle, OutlineBaseHandle* result)
{
	OutlineItem* obj = reinterpret_cast<OutlineItem*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto value = obj->Parent();
		auto ptr = value.AddRefGet();
		*result = reinterpret_cast<OutlineBaseHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION OutlineItem_GetPrev(OutlineItemHandle handle, OutlineItemHandle* result)
{
	OutlineItem* obj = reinterpret_cast<OutlineItem*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		OutputOutlineItemPtr direct;
		auto contains = obj->Prev(direct);
		if (!contains) return GOTCHANG_PDF_ERROR_OPTIONAL_ENTRY_MISSING;
		auto ptr = direct.AddRefGet();
		*result = reinterpret_cast<OutlineItemHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION OutlineItem_GetNext(OutlineItemHandle handle, OutlineItemHandle* result)
{
	OutlineItem* obj = reinterpret_cast<OutlineItem*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		OutputOutlineItemPtr direct;
		auto contains = obj->Next(direct);
		if (!contains) return GOTCHANG_PDF_ERROR_OPTIONAL_ENTRY_MISSING;
		auto ptr = direct.AddRefGet();
		*result = reinterpret_cast<OutlineItemHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION OutlineItem_GetFirst(OutlineItemHandle handle, OutlineItemHandle* result)
{
	OutlineItem* obj = reinterpret_cast<OutlineItem*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		OutputOutlineItemPtr direct;
		auto contains = obj->First(direct);
		if (!contains) return GOTCHANG_PDF_ERROR_OPTIONAL_ENTRY_MISSING;
		auto ptr = direct.AddRefGet();
		*result = reinterpret_cast<OutlineItemHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION OutlineItem_GetLast(OutlineItemHandle handle, OutlineItemHandle* result)
{
	OutlineItem* obj = reinterpret_cast<OutlineItem*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		OutputOutlineItemPtr direct;
		auto contains = obj->Last(direct);
		if (!contains) return GOTCHANG_PDF_ERROR_OPTIONAL_ENTRY_MISSING;
		auto ptr = direct.AddRefGet();
		*result = reinterpret_cast<OutlineItemHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION OutlineItem_GetCount(OutlineItemHandle handle, IntegerObjectHandle* result)
{
	OutlineItem* obj = reinterpret_cast<OutlineItem*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		syntax::IntegerObjectPtr direct;
		auto contains = obj->Count(direct);
		if (!contains) return GOTCHANG_PDF_ERROR_OPTIONAL_ENTRY_MISSING;
		auto ptr = direct.AddRefGet();
		*result = reinterpret_cast<IntegerObjectHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION OutlineItem_GetColor(OutlineItemHandle handle, OutlineItemColorHandle* result)
{
	OutlineItem* obj = reinterpret_cast<OutlineItem*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		OutputOutlineItemColorPtr direct;
		auto contains = obj->Color(direct);
		if (!contains) return GOTCHANG_PDF_ERROR_OPTIONAL_ENTRY_MISSING;
		auto ptr = direct.AddRefGet();
		*result = reinterpret_cast<OutlineItemColorHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION OutlineItem_GetFlags(OutlineItemHandle handle, OutlineItemFlagsHandle* result)
{
	OutlineItem* obj = reinterpret_cast<OutlineItem*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		OutputOutlineItemFlagsPtr direct;
		auto contains = obj->Flags(direct);
		if (!contains) return GOTCHANG_PDF_ERROR_OPTIONAL_ENTRY_MISSING;
		auto ptr = direct.AddRefGet();
		*result = reinterpret_cast<OutlineItemFlagsHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION OutlineItem_Release(OutlineItemHandle handle)
{
	return ObjectRelease<OutlineItem, OutlineItemHandle>(handle);
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION OutlineItemColor_GetRed(OutlineItemColorHandle handle, IntegerObjectHandle* result)
{
	OutlineItemColor* obj = reinterpret_cast<OutlineItemColor*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto value = obj->Red();
		auto ptr = value.AddRefGet();
		*result = reinterpret_cast<IntegerObjectHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION OutlineItemColor_GetGreen(OutlineItemColorHandle handle, IntegerObjectHandle* result)
{
	OutlineItemColor* obj = reinterpret_cast<OutlineItemColor*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto value = obj->Green();
		auto ptr = value.AddRefGet();
		*result = reinterpret_cast<IntegerObjectHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION OutlineItemColor_GetBlue(OutlineItemColorHandle handle, IntegerObjectHandle* result)
{
	OutlineItemColor* obj = reinterpret_cast<OutlineItemColor*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto value = obj->Blue();
		auto ptr = value.AddRefGet();
		*result = reinterpret_cast<IntegerObjectHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION OutlineItemColor_Release(OutlineItemColorHandle handle)
{
	return ObjectRelease<OutlineItemColor, OutlineItemColorHandle>(handle);
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION OutlineItemFlags_IsItalic(OutlineItemFlagsHandle handle, boolean_type* result)
{
	OutlineItemFlags* obj = reinterpret_cast<OutlineItemFlags*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	*result = obj->Italic();
	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION OutlineItemFlags_IsBold(OutlineItemFlagsHandle handle, boolean_type* result)
{
	OutlineItemFlags* obj = reinterpret_cast<OutlineItemFlags*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	*result = obj->Bold();
	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION OutlineItemFlags_Release(OutlineItemFlagsHandle handle)
{
	return ObjectRelease<OutlineItemFlags, OutlineItemFlagsHandle>(handle);
}
