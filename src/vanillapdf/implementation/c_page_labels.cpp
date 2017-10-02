#include "precompiled.h"

#include "semantics/objects/page_labels.h"

#include "vanillapdf/c_page_labels.h"
#include "c_helper.h"

using namespace vanillapdf;
using namespace vanillapdf::semantics;

VANILLAPDF_API error_type CALLING_CONVENTION PageLabels_Contains(PageLabelsHandle handle, size_type page_number, boolean_type* result)
{
	PageLabels* obj = reinterpret_cast<PageLabels*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		*result = obj->Contains(page_number);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION PageLabels_At(PageLabelsHandle handle, size_type page_number, PageLabelHandle* result)
{
	PageLabels* obj = reinterpret_cast<PageLabels*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto page = obj->Find(page_number);
		auto ptr = page.AddRefGet();
		*result = reinterpret_cast<PageLabelHandle>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION PageLabels_Release(PageLabelsHandle handle)
{
	PageLabels* obj = reinterpret_cast<PageLabels*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);

	obj->Release();
	return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION PageLabel_Prefix(PageLabelHandle handle, StringObjectHandle* result)
{
	PageLabel* obj = reinterpret_cast<PageLabel*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);

	try
	{
		syntax::StringObjectPtr prefix;
		auto contains = obj->Prefix(prefix);
		if (!contains) return VANILLAPDF_ERROR_OBJECT_MISSING;
		auto ptr = prefix.AddRefGet();
		*result = reinterpret_cast<StringObjectHandle>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION PageLabel_Start(PageLabelHandle handle, IntegerObjectHandle* result)
{
	PageLabel* obj = reinterpret_cast<PageLabel*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);

	try
	{
		syntax::IntegerObjectPtr start;
		auto contains = obj->Start(start);
		if (!contains) return VANILLAPDF_ERROR_OBJECT_MISSING;
		auto ptr = start.AddRefGet();
		*result = reinterpret_cast<IntegerObjectHandle>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION PageLabel_Style(PageLabelHandle handle, NumberingStyle* result)
{
	PageLabel* obj = reinterpret_cast<PageLabel*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);

	try
	{
		PageLabel::NumberingStyle style;
		auto contains = obj->Style(style);
		if (!contains) return VANILLAPDF_ERROR_OBJECT_MISSING;
		switch (style) {
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
			return VANILLAPDF_ERROR_GENERAL;
		}

		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION PageLabel_Release(PageLabelHandle handle)
{
	return ObjectRelease<PageLabel, PageLabelHandle>(handle);
}