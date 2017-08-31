#include "precompiled.h"

#include "semantics/objects/viewer_preferences.h"

#include "gotchangpdf/c_viewer_preferences.h"
#include "c_helper.h"

using namespace gotchangpdf;
using namespace gotchangpdf::semantics;

GOTCHANG_PDF_API error_type CALLING_CONVENTION ViewerPreferences_GetHideToolbar(ViewerPreferencesHandle handle, BooleanObjectHandle* result)
{
	ViewerPreferences* obj = reinterpret_cast<ViewerPreferences*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		syntax::BooleanObjectPtr direct;
		auto contains = obj->HideToolbar(direct);
		if (!contains) return GOTCHANG_PDF_ERROR_OBJECT_MISSING;
		auto ptr = direct.AddRefGet();
		*result = reinterpret_cast<BooleanObjectHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCESS;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION ViewerPreferences_GetHideMenubar(ViewerPreferencesHandle handle, BooleanObjectHandle* result)
{
	ViewerPreferences* obj = reinterpret_cast<ViewerPreferences*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		syntax::BooleanObjectPtr direct;
		auto contains = obj->HideMenubar(direct);
		if (!contains) return GOTCHANG_PDF_ERROR_OBJECT_MISSING;
		auto ptr = direct.AddRefGet();
		*result = reinterpret_cast<BooleanObjectHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCESS;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION ViewerPreferences_GetHideWindowUI(ViewerPreferencesHandle handle, BooleanObjectHandle* result)
{
	ViewerPreferences* obj = reinterpret_cast<ViewerPreferences*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		syntax::BooleanObjectPtr direct;
		auto contains = obj->HideWindowUI(direct);
		if (!contains) return GOTCHANG_PDF_ERROR_OBJECT_MISSING;
		auto ptr = direct.AddRefGet();
		*result = reinterpret_cast<BooleanObjectHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCESS;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION ViewerPreferences_GetFitWindow(ViewerPreferencesHandle handle, BooleanObjectHandle* result)
{
	ViewerPreferences* obj = reinterpret_cast<ViewerPreferences*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		syntax::BooleanObjectPtr direct;
		auto contains = obj->FitWindow(direct);
		if (!contains) return GOTCHANG_PDF_ERROR_OBJECT_MISSING;
		auto ptr = direct.AddRefGet();
		*result = reinterpret_cast<BooleanObjectHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCESS;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION ViewerPreferences_GetCenterWindow(ViewerPreferencesHandle handle, BooleanObjectHandle* result)
{
	ViewerPreferences* obj = reinterpret_cast<ViewerPreferences*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		syntax::BooleanObjectPtr direct;
		auto contains = obj->CenterWindow(direct);
		if (!contains) return GOTCHANG_PDF_ERROR_OBJECT_MISSING;
		auto ptr = direct.AddRefGet();
		*result = reinterpret_cast<BooleanObjectHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCESS;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION ViewerPreferences_GetDisplayDocTitle(ViewerPreferencesHandle handle, BooleanObjectHandle* result)
{
	ViewerPreferences* obj = reinterpret_cast<ViewerPreferences*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		syntax::BooleanObjectPtr direct;
		auto contains = obj->DisplayDocTitle(direct);
		if (!contains) return GOTCHANG_PDF_ERROR_OBJECT_MISSING;
		auto ptr = direct.AddRefGet();
		*result = reinterpret_cast<BooleanObjectHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCESS;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION ViewerPreferences_GetNonFullScreenPageMode(ViewerPreferencesHandle handle, NonFullScreenPageMode* result)
{
	ViewerPreferences* obj = reinterpret_cast<ViewerPreferences*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		ViewerPreferences::NonFullScreenPageModeType mode;
		auto contains = obj->NonFullScreenPageMode(mode);
		if (!contains) return GOTCHANG_PDF_ERROR_OBJECT_MISSING;

		switch (mode)
		{
		case ViewerPreferences::NonFullScreenPageModeType::UseNone:
			*result = NonFullScreenPageMode_UseNone; break;
		case ViewerPreferences::NonFullScreenPageModeType::UseOutlines:
			*result = NonFullScreenPageMode_UseOutlines; break;
		case ViewerPreferences::NonFullScreenPageModeType::UseThumbs:
			*result = NonFullScreenPageMode_UseThumbs; break;
		case ViewerPreferences::NonFullScreenPageModeType::UseOC:
			*result = NonFullScreenPageMode_UseOC; break;
		default:
			return GOTCHANG_PDF_ERROR_GENERAL;
		}

		return GOTCHANG_PDF_ERROR_SUCCESS;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION ViewerPreferences_GetDirection(ViewerPreferencesHandle handle, ReadingOrder* result)
{
	ViewerPreferences* obj = reinterpret_cast<ViewerPreferences*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		ViewerPreferences::ReadingOrderType order;
		auto contains = obj->Direction(order);
		if (!contains) return GOTCHANG_PDF_ERROR_OBJECT_MISSING;

		switch (order)
		{
		case ViewerPreferences::ReadingOrderType::LeftToRight:
			*result = ReadingOrder_LeftToRight; break;
		case ViewerPreferences::ReadingOrderType::RightToLeft:
			*result = ReadingOrder_RightToLeft; break;
		default:
			return GOTCHANG_PDF_ERROR_GENERAL;
		}

		return GOTCHANG_PDF_ERROR_SUCCESS;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION ViewerPreferences_GetViewArea(ViewerPreferencesHandle handle, NameObjectHandle* result)
{
	ViewerPreferences* obj = reinterpret_cast<ViewerPreferences*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		syntax::NameObjectPtr direct;
		auto contains = obj->ViewArea(direct);
		if (!contains) return GOTCHANG_PDF_ERROR_OBJECT_MISSING;
		auto ptr = direct.AddRefGet();
		*result = reinterpret_cast<NameObjectHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCESS;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION ViewerPreferences_GetViewClip(ViewerPreferencesHandle handle, NameObjectHandle* result)
{
	ViewerPreferences* obj = reinterpret_cast<ViewerPreferences*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		syntax::NameObjectPtr direct;
		auto contains = obj->ViewClip(direct);
		if (!contains) return GOTCHANG_PDF_ERROR_OBJECT_MISSING;
		auto ptr = direct.AddRefGet();
		*result = reinterpret_cast<NameObjectHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCESS;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION ViewerPreferences_GetPrintArea(ViewerPreferencesHandle handle, NameObjectHandle* result)
{
	ViewerPreferences* obj = reinterpret_cast<ViewerPreferences*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		syntax::NameObjectPtr direct;
		auto contains = obj->PrintArea(direct);
		if (!contains) return GOTCHANG_PDF_ERROR_OBJECT_MISSING;
		auto ptr = direct.AddRefGet();
		*result = reinterpret_cast<NameObjectHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCESS;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION ViewerPreferences_GetPrintClip(ViewerPreferencesHandle handle, NameObjectHandle* result)
{
	ViewerPreferences* obj = reinterpret_cast<ViewerPreferences*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		syntax::NameObjectPtr direct;
		auto contains = obj->PrintClip(direct);
		if (!contains) return GOTCHANG_PDF_ERROR_OBJECT_MISSING;
		auto ptr = direct.AddRefGet();
		*result = reinterpret_cast<NameObjectHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCESS;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION ViewerPreferences_GetPrintScaling(ViewerPreferencesHandle handle, PrintScaling* result)
{
	ViewerPreferences* obj = reinterpret_cast<ViewerPreferences*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		ViewerPreferences::PrintScalingType scaling;
		auto contains = obj->PrintScaling(scaling);
		if (!contains) return GOTCHANG_PDF_ERROR_OBJECT_MISSING;

		switch (scaling)
		{
		case ViewerPreferences::PrintScalingType::AppDefault:
			*result = PrintScaling_AppDefault; break;
		case ViewerPreferences::PrintScalingType::None:
			*result = PrintScaling_None; break;
		default:
			return GOTCHANG_PDF_ERROR_GENERAL;
		}

		return GOTCHANG_PDF_ERROR_SUCCESS;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION ViewerPreferences_GetDuplex(ViewerPreferencesHandle handle, Duplex* result)
{
	ViewerPreferences* obj = reinterpret_cast<ViewerPreferences*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		ViewerPreferences::DuplexType duplex;
		auto contains = obj->Duplex(duplex);
		if (!contains) return GOTCHANG_PDF_ERROR_OBJECT_MISSING;

		switch (duplex)
		{
		case ViewerPreferences::DuplexType::Simplex:
			*result = Duplex_Simplex; break;
		case ViewerPreferences::DuplexType::DuplexFlipShortEdge:
			*result = Duplex_DuplexFlipShortEdge; break;
		case ViewerPreferences::DuplexType::DuplexFlipLongEdge:
			*result = Duplex_DuplexFlipLongEdge; break;
		default:
			return GOTCHANG_PDF_ERROR_GENERAL;
		}

		return GOTCHANG_PDF_ERROR_SUCCESS;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION ViewerPreferences_GetPickTrayByPDFSize(ViewerPreferencesHandle handle, BooleanObjectHandle* result)
{
	ViewerPreferences* obj = reinterpret_cast<ViewerPreferences*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		syntax::BooleanObjectPtr direct;
		auto contains = obj->PickTrayByPDFSize(direct);
		if (!contains) return GOTCHANG_PDF_ERROR_OBJECT_MISSING;
		auto ptr = direct.AddRefGet();
		*result = reinterpret_cast<BooleanObjectHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCESS;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION ViewerPreferences_GetPrintPageRange(ViewerPreferencesHandle handle, PageRangeHandle* result)
{
	ViewerPreferences* obj = reinterpret_cast<ViewerPreferences*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		OutputPageRangePtr direct;
		auto contains = obj->PrintPageRange(direct);
		if (!contains) return GOTCHANG_PDF_ERROR_OBJECT_MISSING;
		auto ptr = direct.AddRefGet();
		*result = reinterpret_cast<PageRangeHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCESS;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION ViewerPreferences_GetNumCopies(ViewerPreferencesHandle handle, IntegerObjectHandle* result)
{
	ViewerPreferences* obj = reinterpret_cast<ViewerPreferences*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		syntax::IntegerObjectPtr direct;
		auto contains = obj->NumCopies(direct);
		if (!contains) return GOTCHANG_PDF_ERROR_OBJECT_MISSING;
		auto ptr = direct.AddRefGet();
		*result = reinterpret_cast<IntegerObjectHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCESS;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION ViewerPreferences_Release(ViewerPreferencesHandle handle)
{
	return ObjectRelease<ViewerPreferences, ViewerPreferencesHandle>(handle);
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION PageRange_GetSize(PageRangeHandle handle, size_type* result)
{
	PageRange* obj = reinterpret_cast<PageRange*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		*result = obj->Size();
		return GOTCHANG_PDF_ERROR_SUCCESS;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION PageRange_GetSubrange(PageRangeHandle handle, size_type at, PageSubRangeHandle* result)
{
	PageRange* obj = reinterpret_cast<PageRange*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto direct = obj->At(at);
		auto ptr = direct.AddRefGet();
		*result = reinterpret_cast<PageSubRangeHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCESS;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION PageSubRange_GetFirstPage(PageSubRangeHandle handle, IntegerObjectHandle* result)
{
	PageRange::SubRange* obj = reinterpret_cast<PageRange::SubRange*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto direct = obj->FirstPage();
		auto ptr = direct.AddRefGet();
		*result = reinterpret_cast<IntegerObjectHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCESS;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION PageSubRange_GetLastPage(PageSubRangeHandle handle, IntegerObjectHandle* result)
{
	PageRange::SubRange* obj = reinterpret_cast<PageRange::SubRange*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto direct = obj->LastPage();
		auto ptr = direct.AddRefGet();
		*result = reinterpret_cast<IntegerObjectHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCESS;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION PageSubRange_Release(PageSubRangeHandle handle)
{
	return ObjectRelease<PageRange::SubRange, PageSubRangeHandle>(handle);
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION PageRange_Release(PageRangeHandle handle)
{
	return ObjectRelease<PageRange, PageRangeHandle>(handle);
}
