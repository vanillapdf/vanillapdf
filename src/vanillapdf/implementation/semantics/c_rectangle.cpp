#include "precompiled.h"
#include "semantics/objects/rectangle.h"

#include "vanillapdf/semantics/c_rectangle.h"
#include "implementation/c_helper.h"

using namespace vanillapdf;
using namespace vanillapdf::semantics;

VANILLAPDF_API error_type CALLING_CONVENTION Rectangle_LowerLeftX(RectangleHandle* handle, IntegerObjectHandle** result)
{
	Rectangle* obj = reinterpret_cast<Rectangle*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto direct = obj->LowerLeftX();
		auto ptr = direct.AddRefGet();
		*result = reinterpret_cast<IntegerObjectHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Rectangle_LowerLeftY(RectangleHandle* handle, IntegerObjectHandle** result)
{
	Rectangle* obj = reinterpret_cast<Rectangle*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto direct = obj->LowerLeftY();
		auto ptr = direct.AddRefGet();
		*result = reinterpret_cast<IntegerObjectHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Rectangle_UpperRightX(RectangleHandle* handle, IntegerObjectHandle** result)
{
	Rectangle* obj = reinterpret_cast<Rectangle*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto direct = obj->UpperRightX();
		auto ptr = direct.AddRefGet();
		*result = reinterpret_cast<IntegerObjectHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Rectangle_UpperRightY(RectangleHandle* handle, IntegerObjectHandle** result)
{
	Rectangle* obj = reinterpret_cast<Rectangle*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto direct = obj->UpperRightY();
		auto ptr = direct.AddRefGet();
		*result = reinterpret_cast<IntegerObjectHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Rectangle_Release(RectangleHandle* handle)
{
	return ObjectRelease<Rectangle, RectangleHandle>(handle);
}
