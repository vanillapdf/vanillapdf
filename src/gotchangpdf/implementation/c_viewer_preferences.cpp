#include "precompiled.h"
#include "viewer_preferences.h"
#include "file.h"

#include "gotchangpdf/c_viewer_preferences.h"
#include "c_helper.h"

using namespace gotchangpdf;
using namespace gotchangpdf::semantics;

GOTCHANG_PDF_API error_type CALLING_CONVENTION ViewerPreferences_GetHideToolbar(ViewerPreferencesHandle handle, PBooleanObjectHandle result)
{
	ViewerPreferences* obj = reinterpret_cast<ViewerPreferences*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		syntax::BooleanObjectPtr direct;
		auto contains = obj->HideToolbar(direct);
		if (!contains) return GOTCHANG_PDF_ERROR_OPTIONAL_ENTRY_MISSING;
		auto ptr = direct.AddRefGet();
		*result = reinterpret_cast<BooleanObjectHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION ViewerPreferences_GetHideMenubar(ViewerPreferencesHandle handle, PBooleanObjectHandle result)
{
	ViewerPreferences* obj = reinterpret_cast<ViewerPreferences*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		syntax::BooleanObjectPtr direct;
		auto contains = obj->HideMenubar(direct);
		if (!contains) return GOTCHANG_PDF_ERROR_OPTIONAL_ENTRY_MISSING;
		auto ptr = direct.AddRefGet();
		*result = reinterpret_cast<BooleanObjectHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION ViewerPreferences_GetHideWindowUI(ViewerPreferencesHandle handle, PBooleanObjectHandle result)
{
	ViewerPreferences* obj = reinterpret_cast<ViewerPreferences*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		syntax::BooleanObjectPtr direct;
		auto contains = obj->HideWindowUI(direct);
		if (!contains) return GOTCHANG_PDF_ERROR_OPTIONAL_ENTRY_MISSING;
		auto ptr = direct.AddRefGet();
		*result = reinterpret_cast<BooleanObjectHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION ViewerPreferences_GetFitWindow(ViewerPreferencesHandle handle, PBooleanObjectHandle result)
{
	ViewerPreferences* obj = reinterpret_cast<ViewerPreferences*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		syntax::BooleanObjectPtr direct;
		auto contains = obj->FitWindow(direct);
		if (!contains) return GOTCHANG_PDF_ERROR_OPTIONAL_ENTRY_MISSING;
		auto ptr = direct.AddRefGet();
		*result = reinterpret_cast<BooleanObjectHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION ViewerPreferences_GetCenterWindow(ViewerPreferencesHandle handle, PBooleanObjectHandle result)
{
	ViewerPreferences* obj = reinterpret_cast<ViewerPreferences*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		syntax::BooleanObjectPtr direct;
		auto contains = obj->CenterWindow(direct);
		if (!contains) return GOTCHANG_PDF_ERROR_OPTIONAL_ENTRY_MISSING;
		auto ptr = direct.AddRefGet();
		*result = reinterpret_cast<BooleanObjectHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION ViewerPreferences_GetDisplayDocTitle(ViewerPreferencesHandle handle, PBooleanObjectHandle result)
{
	ViewerPreferences* obj = reinterpret_cast<ViewerPreferences*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		syntax::BooleanObjectPtr direct;
		auto contains = obj->DisplayDocTitle(direct);
		if (!contains) return GOTCHANG_PDF_ERROR_OPTIONAL_ENTRY_MISSING;
		auto ptr = direct.AddRefGet();
		*result = reinterpret_cast<BooleanObjectHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION ViewerPreferences_GetNonFullScreenPageMode(ViewerPreferencesHandle handle, PNonFullScreenPageMode result)
{
	ViewerPreferences* obj = reinterpret_cast<ViewerPreferences*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		ViewerPreferences::NonFullScreenPageModeType mode;
		auto contains = obj->NonFullScreenPageMode(mode);
		if (!contains) return GOTCHANG_PDF_ERROR_OPTIONAL_ENTRY_MISSING;

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

		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION ViewerPreferences_GetDirection(ViewerPreferencesHandle handle, PReadingOrder result)
{
	ViewerPreferences* obj = reinterpret_cast<ViewerPreferences*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		ViewerPreferences::ReadingOrderType order;
		auto contains = obj->Direction(order);
		if (!contains) return GOTCHANG_PDF_ERROR_OPTIONAL_ENTRY_MISSING;

		switch (order)
		{
		case ViewerPreferences::ReadingOrderType::LeftToRight:
			*result = ReadingOrder_LeftToRight; break;
		case ViewerPreferences::ReadingOrderType::RightToLeft:
			*result = ReadingOrder_RightToLeft; break;
		default:
			return GOTCHANG_PDF_ERROR_GENERAL;
		}

		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION ViewerPreferences_GetViewArea(ViewerPreferencesHandle handle, PNameObjectHandle result)
{
	ViewerPreferences* obj = reinterpret_cast<ViewerPreferences*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		syntax::NameObjectPtr direct;
		auto contains = obj->ViewArea(direct);
		if (!contains) return GOTCHANG_PDF_ERROR_OPTIONAL_ENTRY_MISSING;
		auto ptr = direct.AddRefGet();
		*result = reinterpret_cast<NameObjectHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION ViewerPreferences_GetViewClip(ViewerPreferencesHandle handle, PNameObjectHandle result)
{
	ViewerPreferences* obj = reinterpret_cast<ViewerPreferences*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		syntax::NameObjectPtr direct;
		auto contains = obj->ViewClip(direct);
		if (!contains) return GOTCHANG_PDF_ERROR_OPTIONAL_ENTRY_MISSING;
		auto ptr = direct.AddRefGet();
		*result = reinterpret_cast<NameObjectHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION ViewerPreferences_GetPrintArea(ViewerPreferencesHandle handle, PNameObjectHandle result)
{
	ViewerPreferences* obj = reinterpret_cast<ViewerPreferences*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		syntax::NameObjectPtr direct;
		auto contains = obj->PrintArea(direct);
		if (!contains) return GOTCHANG_PDF_ERROR_OPTIONAL_ENTRY_MISSING;
		auto ptr = direct.AddRefGet();
		*result = reinterpret_cast<NameObjectHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION ViewerPreferences_GetPrintClip(ViewerPreferencesHandle handle, PNameObjectHandle result)
{
	ViewerPreferences* obj = reinterpret_cast<ViewerPreferences*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		syntax::NameObjectPtr direct;
		auto contains = obj->PrintClip(direct);
		if (!contains) return GOTCHANG_PDF_ERROR_OPTIONAL_ENTRY_MISSING;
		auto ptr = direct.AddRefGet();
		*result = reinterpret_cast<NameObjectHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION ViewerPreferences_GetPrintScaling(ViewerPreferencesHandle handle, PPrintScaling result)
{
	ViewerPreferences* obj = reinterpret_cast<ViewerPreferences*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		ViewerPreferences::PrintScalingType scaling;
		auto contains = obj->PrintScaling(scaling);
		if (!contains) return GOTCHANG_PDF_ERROR_OPTIONAL_ENTRY_MISSING;

		switch (scaling)
		{
		case ViewerPreferences::PrintScalingType::AppDefault:
			*result = PrintScaling_AppDefault; break;
		case ViewerPreferences::PrintScalingType::None:
			*result = PrintScaling_None; break;
		default:
			return GOTCHANG_PDF_ERROR_GENERAL;
		}

		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION ViewerPreferences_GetDuplex(ViewerPreferencesHandle handle, PDuplex result)
{
	ViewerPreferences* obj = reinterpret_cast<ViewerPreferences*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		ViewerPreferences::DuplexType duplex;
		auto contains = obj->Duplex(duplex);
		if (!contains) return GOTCHANG_PDF_ERROR_OPTIONAL_ENTRY_MISSING;

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

		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION ViewerPreferences_GetPickTrayByPDFSize(ViewerPreferencesHandle handle, PBooleanObjectHandle result)
{
	ViewerPreferences* obj = reinterpret_cast<ViewerPreferences*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		syntax::BooleanObjectPtr direct;
		auto contains = obj->PickTrayByPDFSize(direct);
		if (!contains) return GOTCHANG_PDF_ERROR_OPTIONAL_ENTRY_MISSING;
		auto ptr = direct.AddRefGet();
		*result = reinterpret_cast<BooleanObjectHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION ViewerPreferences_GetPrintPageRange(ViewerPreferencesHandle handle, PPageRangeHandle result)
{
	ViewerPreferences* obj = reinterpret_cast<ViewerPreferences*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		OutputPageRangePtr direct;
		auto contains = obj->PrintPageRange(direct);
		if (!contains) return GOTCHANG_PDF_ERROR_OPTIONAL_ENTRY_MISSING;
		auto ptr = direct.AddRefGet();
		*result = reinterpret_cast<PageRangeHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION ViewerPreferences_GetNumCopies(ViewerPreferencesHandle handle, PIntegerObjectHandle result)
{
	ViewerPreferences* obj = reinterpret_cast<ViewerPreferences*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		syntax::IntegerObjectPtr direct;
		auto contains = obj->NumCopies(direct);
		if (!contains) return GOTCHANG_PDF_ERROR_OPTIONAL_ENTRY_MISSING;
		auto ptr = direct.AddRefGet();
		*result = reinterpret_cast<IntegerObjectHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION ViewerPreferences_Release(ViewerPreferencesHandle handle)
{
	return ObjectRelease<ViewerPreferences, ViewerPreferencesHandle>(handle);
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION PageRange_GetSize(PageRangeHandle handle, out_integer_type result)
{
	PageRange* obj = reinterpret_cast<PageRange*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		*result = obj->Size();
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION PageRange_GetSubrange(PageRangeHandle handle, integer_type at, PPageSubRangeHandle result)
{
	PageRange* obj = reinterpret_cast<PageRange*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto direct = obj->At(at);
		auto ptr = direct.AddRefGet();
		*result = reinterpret_cast<PageSubRangeHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION PageSubRange_GetFirstPage(PageSubRangeHandle handle, PIntegerObjectHandle result)
{
	PageRange::SubRange* obj = reinterpret_cast<PageRange::SubRange*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto direct = obj->FirstPage();
		auto ptr = direct.AddRefGet();
		*result = reinterpret_cast<IntegerObjectHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION PageSubRange_GetLastPage(PageSubRangeHandle handle, PIntegerObjectHandle result)
{
	PageRange::SubRange* obj = reinterpret_cast<PageRange::SubRange*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto direct = obj->LastPage();
		auto ptr = direct.AddRefGet();
		*result = reinterpret_cast<IntegerObjectHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
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
