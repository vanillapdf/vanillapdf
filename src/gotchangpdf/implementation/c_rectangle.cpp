#include "precompiled.h"
#include "rectangle.h"

#include "c_rectangle.h"
#include "c_helper.h"

using namespace gotchangpdf;
using namespace gotchangpdf::semantics;

GOTCHANG_PDF_API error_type CALLING_CONVENTION Rectangle_LowerLeftX(RectangleHandle handle, PIntegerHandle result)
{
	Rectangle* obj = reinterpret_cast<Rectangle*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto direct = obj->LowerLeftX();
		auto ptr = direct.AddRefGet();
		*result = reinterpret_cast<IntegerHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Rectangle_LowerLeftY(RectangleHandle handle, PIntegerHandle result)
{
	Rectangle* obj = reinterpret_cast<Rectangle*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto direct = obj->LowerLeftY();
		auto ptr = direct.AddRefGet();
		*result = reinterpret_cast<IntegerHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Rectangle_UpperRightX(RectangleHandle handle, PIntegerHandle result)
{
	Rectangle* obj = reinterpret_cast<Rectangle*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto direct = obj->UpperRightX();
		auto ptr = direct.AddRefGet();
		*result = reinterpret_cast<IntegerHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Rectangle_UpperRightY(RectangleHandle handle, PIntegerHandle result)
{
	Rectangle* obj = reinterpret_cast<Rectangle*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto direct = obj->UpperRightY();
		auto ptr = direct.AddRefGet();
		*result = reinterpret_cast<IntegerHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Rectangle_Release(RectangleHandle handle)
{
	return ObjectRelease<Rectangle, RectangleHandle>(handle);
}
