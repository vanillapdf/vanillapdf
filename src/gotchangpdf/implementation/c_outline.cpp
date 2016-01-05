#include "precompiled.h"
#include "outline.h"
#include "file.h"

#include "c_outline.h"
#include "c_helper.h"

using namespace gotchangpdf;
using namespace gotchangpdf::semantics;

GOTCHANG_PDF_API error_type CALLING_CONVENTION OutlineBase_GetType(OutlineBaseHandle handle, POutlineType result)
{
	OutlineBase* obj = reinterpret_cast<OutlineBase*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		LOG_HIGH_OBJECT_SCOPE(obj);

		switch (obj->OutlineType()) {
		case OutlineBase::Type::Outline:
			*result = OutlineType_Outline; break;
		case OutlineBase::Type::Item:
			*result = OutlineType_Item; break;
		default:
			return GOTCHANG_PDF_ERROR_GENERAL;
		}

		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_SCOPE_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION OutlineBase_ToOutline(OutlineBaseHandle handle, POutlineHandle result)
{
	return SafeObjectConvertNoScope<OutlineBase, Outline, OutlineBaseHandle, OutlineHandle>(handle, result);
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION OutlineBase_ToOutlineItem(OutlineBaseHandle handle, POutlineItemHandle result)
{
	return SafeObjectConvertNoScope<OutlineBase, OutlineItem, OutlineBaseHandle, OutlineItemHandle>(handle, result);
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION OutlineBase_Release(OutlineBaseHandle handle)
{
	return HighObjectRelease<OutlineBase, OutlineBaseHandle>(handle);
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Outline_GetFirst(OutlineHandle handle, POutlineItemHandle result)
{
	Outline* obj = reinterpret_cast<Outline*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		LOG_HIGH_OBJECT_SCOPE(obj);
		try
		{
			OutlineItemPtr direct;
			auto contains = obj->First(direct);
			if (!contains) return GOTCHANG_PDF_ERROR_OPTIONAL_ENTRY_MISSING;
			auto ptr = direct.AddRefGet();
			*result = reinterpret_cast<OutlineItemHandle>(ptr);
			return GOTCHANG_PDF_ERROR_SUCCES;
		} CATCH_GOTCHNGPDF_EXCEPTIONS
	} CATCH_SCOPE_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Outline_GetLast(OutlineHandle handle, POutlineItemHandle result)
{
	Outline* obj = reinterpret_cast<Outline*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		LOG_HIGH_OBJECT_SCOPE(obj);
		try
		{
			OutlineItemPtr direct;
			auto contains = obj->Last(direct);
			if (!contains) return GOTCHANG_PDF_ERROR_OPTIONAL_ENTRY_MISSING;
			auto ptr = direct.AddRefGet();
			*result = reinterpret_cast<OutlineItemHandle>(ptr);
			return GOTCHANG_PDF_ERROR_SUCCES;
		} CATCH_GOTCHNGPDF_EXCEPTIONS
	} CATCH_SCOPE_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Outline_GetCount(OutlineHandle handle, PIntegerHandle result)
{
	Outline* obj = reinterpret_cast<Outline*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		LOG_HIGH_OBJECT_SCOPE(obj);
		try
		{
			syntax::IntegerObjectPtr direct;
			auto contains = obj->Count(direct);
			if (!contains) return GOTCHANG_PDF_ERROR_OPTIONAL_ENTRY_MISSING;
			auto ptr = direct.AddRefGet();
			*result = reinterpret_cast<IntegerHandle>(ptr);
			return GOTCHANG_PDF_ERROR_SUCCES;
		} CATCH_GOTCHNGPDF_EXCEPTIONS
	} CATCH_SCOPE_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Outline_Release(OutlineHandle handle)
{
	return HighObjectRelease<Outline, OutlineHandle>(handle);
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION OutlineItem_GetTitle(OutlineItemHandle handle, PStringHandle result)
{
	OutlineItem* obj = reinterpret_cast<OutlineItem*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		LOG_HIGH_OBJECT_SCOPE(obj);
		try
		{
			auto value = obj->Title();
			auto ptr = value.AddRefGet();
			*result = reinterpret_cast<StringHandle>(ptr);
			return GOTCHANG_PDF_ERROR_SUCCES;
		} CATCH_GOTCHNGPDF_EXCEPTIONS
	} CATCH_SCOPE_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION OutlineItem_GetParent(OutlineItemHandle handle, POutlineBaseHandle result)
{
	OutlineItem* obj = reinterpret_cast<OutlineItem*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		LOG_HIGH_OBJECT_SCOPE(obj);
		try
		{
			auto value = obj->Parent();
			auto ptr = value.AddRefGet();
			*result = reinterpret_cast<OutlineBaseHandle>(ptr);
			return GOTCHANG_PDF_ERROR_SUCCES;
		} CATCH_GOTCHNGPDF_EXCEPTIONS
	} CATCH_SCOPE_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION OutlineItem_GetPrev(OutlineItemHandle handle, POutlineItemHandle result)
{
	OutlineItem* obj = reinterpret_cast<OutlineItem*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		LOG_HIGH_OBJECT_SCOPE(obj);
		try
		{
			OutlineItemPtr direct;
			auto contains = obj->Prev(direct);
			if (!contains) return GOTCHANG_PDF_ERROR_OPTIONAL_ENTRY_MISSING;
			auto ptr = direct.AddRefGet();
			*result = reinterpret_cast<OutlineItemHandle>(ptr);
			return GOTCHANG_PDF_ERROR_SUCCES;
		} CATCH_GOTCHNGPDF_EXCEPTIONS
	} CATCH_SCOPE_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION OutlineItem_GetNext(OutlineItemHandle handle, POutlineItemHandle result)
{
	OutlineItem* obj = reinterpret_cast<OutlineItem*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		LOG_HIGH_OBJECT_SCOPE(obj);
		try
		{
			OutlineItemPtr direct;
			auto contains = obj->Next(direct);
			if (!contains) return GOTCHANG_PDF_ERROR_OPTIONAL_ENTRY_MISSING;
			auto ptr = direct.AddRefGet();
			*result = reinterpret_cast<OutlineItemHandle>(ptr);
			return GOTCHANG_PDF_ERROR_SUCCES;
		} CATCH_GOTCHNGPDF_EXCEPTIONS
	} CATCH_SCOPE_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION OutlineItem_GetFirst(OutlineItemHandle handle, POutlineItemHandle result)
{
	OutlineItem* obj = reinterpret_cast<OutlineItem*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		LOG_HIGH_OBJECT_SCOPE(obj);
		try
		{
			OutlineItemPtr direct;
			auto contains = obj->First(direct);
			if (!contains) return GOTCHANG_PDF_ERROR_OPTIONAL_ENTRY_MISSING;
			auto ptr = direct.AddRefGet();
			*result = reinterpret_cast<OutlineItemHandle>(ptr);
			return GOTCHANG_PDF_ERROR_SUCCES;
		} CATCH_GOTCHNGPDF_EXCEPTIONS
	} CATCH_SCOPE_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION OutlineItem_GetLast(OutlineItemHandle handle, POutlineItemHandle result)
{
	OutlineItem* obj = reinterpret_cast<OutlineItem*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		LOG_HIGH_OBJECT_SCOPE(obj);
		try
		{
			OutlineItemPtr direct;
			auto contains = obj->Last(direct);
			if (!contains) return GOTCHANG_PDF_ERROR_OPTIONAL_ENTRY_MISSING;
			auto ptr = direct.AddRefGet();
			*result = reinterpret_cast<OutlineItemHandle>(ptr);
			return GOTCHANG_PDF_ERROR_SUCCES;
		} CATCH_GOTCHNGPDF_EXCEPTIONS
	} CATCH_SCOPE_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION OutlineItem_GetCount(OutlineItemHandle handle, PIntegerHandle result)
{
	OutlineItem* obj = reinterpret_cast<OutlineItem*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		LOG_HIGH_OBJECT_SCOPE(obj);
		try
		{
			syntax::IntegerObjectPtr direct;
			auto contains = obj->Count(direct);
			if (!contains) return GOTCHANG_PDF_ERROR_OPTIONAL_ENTRY_MISSING;
			auto ptr = direct.AddRefGet();
			*result = reinterpret_cast<IntegerHandle>(ptr);
			return GOTCHANG_PDF_ERROR_SUCCES;
		} CATCH_GOTCHNGPDF_EXCEPTIONS
	} CATCH_SCOPE_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION OutlineItem_GetColor(OutlineItemHandle handle, POutlineItemColorHandle result)
{
	OutlineItem* obj = reinterpret_cast<OutlineItem*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		LOG_HIGH_OBJECT_SCOPE(obj);
		try
		{
			OutlineItemColorPtr direct;
			auto contains = obj->Color(direct);
			if (!contains) return GOTCHANG_PDF_ERROR_OPTIONAL_ENTRY_MISSING;
			auto ptr = direct.AddRefGet();
			*result = reinterpret_cast<OutlineItemColorHandle>(ptr);
			return GOTCHANG_PDF_ERROR_SUCCES;
		} CATCH_GOTCHNGPDF_EXCEPTIONS
	} CATCH_SCOPE_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION OutlineItem_GetFlags(OutlineItemHandle handle, POutlineItemFlagsHandle result)
{
	OutlineItem* obj = reinterpret_cast<OutlineItem*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		LOG_HIGH_OBJECT_SCOPE(obj);
		try
		{
			OutlineItemFlagsPtr direct;
			auto contains = obj->Flags(direct);
			if (!contains) return GOTCHANG_PDF_ERROR_OPTIONAL_ENTRY_MISSING;
			auto ptr = direct.AddRefGet();
			*result = reinterpret_cast<OutlineItemFlagsHandle>(ptr);
			return GOTCHANG_PDF_ERROR_SUCCES;
		} CATCH_GOTCHNGPDF_EXCEPTIONS
	} CATCH_SCOPE_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION OutlineItem_Release(OutlineItemHandle handle)
{
	return HighObjectRelease<OutlineItem, OutlineItemHandle>(handle);
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION OutlineItemColor_GetRed(OutlineItemColorHandle handle, PIntegerHandle result)
{
	OutlineItemColor* obj = reinterpret_cast<OutlineItemColor*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		//LOG_HIGH_OBJECT_SCOPE(obj);
		try
		{
			auto value = obj->Red();
			auto ptr = value.AddRefGet();
			*result = reinterpret_cast<IntegerHandle>(ptr);
			return GOTCHANG_PDF_ERROR_SUCCES;
		} CATCH_GOTCHNGPDF_EXCEPTIONS
	} CATCH_SCOPE_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION OutlineItemColor_GetGreen(OutlineItemColorHandle handle, PIntegerHandle result)
{
	OutlineItemColor* obj = reinterpret_cast<OutlineItemColor*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		//LOG_HIGH_OBJECT_SCOPE(obj);
		try
		{
			auto value = obj->Green();
			auto ptr = value.AddRefGet();
			*result = reinterpret_cast<IntegerHandle>(ptr);
			return GOTCHANG_PDF_ERROR_SUCCES;
		} CATCH_GOTCHNGPDF_EXCEPTIONS
	} CATCH_SCOPE_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION OutlineItemColor_GetBlue(OutlineItemColorHandle handle, PIntegerHandle result)
{
	OutlineItemColor* obj = reinterpret_cast<OutlineItemColor*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		//LOG_HIGH_OBJECT_SCOPE(obj);
		try
		{
			auto value = obj->Blue();
			auto ptr = value.AddRefGet();
			*result = reinterpret_cast<IntegerHandle>(ptr);
			return GOTCHANG_PDF_ERROR_SUCCES;
		} CATCH_GOTCHNGPDF_EXCEPTIONS
	} CATCH_SCOPE_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION OutlineItemColor_Release(OutlineItemColorHandle handle)
{
	OutlineItemColor* obj = reinterpret_cast<OutlineItemColor*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);

	try
	{
		//LOG_HIGH_OBJECT_SCOPE(obj);
		obj->Release();
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_SCOPE_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION OutlineItemFlags_IsItalic(OutlineItemFlagsHandle handle, out_boolean_type result)
{
	OutlineItemFlags* obj = reinterpret_cast<OutlineItemFlags*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		LOG_HIGH_OBJECT_SCOPE(obj);
		*result = obj->Italic();
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_SCOPE_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION OutlineItemFlags_IsBold(OutlineItemFlagsHandle handle, out_boolean_type result)
{
	OutlineItemFlags* obj = reinterpret_cast<OutlineItemFlags*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		LOG_HIGH_OBJECT_SCOPE(obj);
		*result = obj->Bold();
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_SCOPE_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION OutlineItemFlags_Release(OutlineItemFlagsHandle handle)
{
	return HighObjectRelease<OutlineItemFlags, OutlineItemFlagsHandle>(handle);
}
