#include "precompiled.h"
#include "page_labels.h"

#include "c_page_labels.h"
#include "c_helper.h"

using namespace gotchangpdf;
using namespace gotchangpdf::semantics;

GOTCHANG_PDF_API error_type CALLING_CONVENTION PageLabels_Contains(PageLabelsHandle handle, integer_type page_number, out_boolean_type result)
{
	PageLabels* obj = reinterpret_cast<PageLabels*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);
	//LOG_WEAK_FILE_SCOPE(obj->GetFile());

	try
	{
		*result = obj->Contains(page_number);
		return GOTCHANG_PDF_ERROR_SUCCES;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION PageLabels_At(PageLabelsHandle handle, integer_type page_number, PPageLabelHandle result)
{
	PageLabels* obj = reinterpret_cast<PageLabels*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);
	//LOG_WEAK_FILE_SCOPE(obj->GetFile());

	try
	{
		auto page = obj->Find(page_number);
		auto ptr = page.AddRefGet();
		*result = reinterpret_cast<PageLabelHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION PageLabels_Release(PageLabelsHandle handle)
{
	PageLabels* obj = reinterpret_cast<PageLabels*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	//LOG_WEAK_FILE_SCOPE(obj->GetFile());

	obj->Release();
	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION PageLabel_P(PageLabelHandle handle, PStringHandle result)
{
	PageLabel* obj = reinterpret_cast<PageLabel*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);

	try
	{
		auto p = obj->P();
		auto ptr = p.AddRefGet();
		*result = reinterpret_cast<StringHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION PageLabel_St(PageLabelHandle handle, PIntegerHandle result)
{
	PageLabel* obj = reinterpret_cast<PageLabel*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);

	try
	{
		auto st = obj->St();
		auto ptr = st.AddRefGet();
		*result = reinterpret_cast<IntegerHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION PageLabel_S(PageLabelHandle handle, PNumberingStyle result)
{
	PageLabel* obj = reinterpret_cast<PageLabel*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);

	try
	{
		switch (obj->S()) {
		case PageLabel::NumberingStyle::Decimal:
			*result = NumberingStyle_Decimal; break;
		case PageLabel::NumberingStyle::UpperRoman:
			*result = NumberingStyle_UpperRoman; break;
		case PageLabel::NumberingStyle::LowerRoman:
			*result = NumberingStyle_LowerRoman; break;
		case PageLabel::NumberingStyle::UpperLetters:
			*result = NumberingStyle_UpperLetters; break;
		case PageLabel::NumberingStyle::LowerLetters:
			*result = NumberingStyle_LowerLetters; break;
		default:
			return GOTCHANG_PDF_ERROR_GENERAL;
		}

		return GOTCHANG_PDF_ERROR_SUCCES;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION PageLabel_Release(PageLabelHandle handle)
{
	PageLabel* obj = reinterpret_cast<PageLabel*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	//LOG_WEAK_FILE_SCOPE(obj->GetFile());

	obj->Release();
	return GOTCHANG_PDF_ERROR_SUCCES;
}