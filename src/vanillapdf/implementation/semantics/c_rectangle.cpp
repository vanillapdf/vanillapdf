#include "precompiled.h"
#include "semantics/objects/rectangle.h"

#include "vanillapdf/semantics/c_rectangle.h"
#include "implementation/c_helper.h"

using namespace vanillapdf;
using namespace vanillapdf::syntax;
using namespace vanillapdf::semantics;

VANILLAPDF_API error_type CALLING_CONVENTION Rectangle_Create(RectangleHandle** result) {
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		RectanglePtr rectangle;
		auto ptr = rectangle.AddRefGet();
		*result = reinterpret_cast<RectangleHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Rectangle_GetLowerLeftX(RectangleHandle* handle, bigint_type* result)
{
	Rectangle* obj = reinterpret_cast<Rectangle*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		*result = obj->GetLowerLeftX();
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Rectangle_SetLowerLeftX(RectangleHandle* handle, bigint_type data) {
	Rectangle* obj = reinterpret_cast<Rectangle*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);

	try
	{
		obj->SetLowerLeftX(data);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Rectangle_GetLowerLeftY(RectangleHandle* handle, bigint_type* result)
{
	Rectangle* obj = reinterpret_cast<Rectangle*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		*result = obj->GetLowerLeftY();
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Rectangle_SetLowerLeftY(RectangleHandle* handle, bigint_type data) {
	Rectangle* obj = reinterpret_cast<Rectangle*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);

	try
	{
		obj->SetLowerLeftY(data);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Rectangle_GetUpperRightX(RectangleHandle* handle, bigint_type* result)
{
	Rectangle* obj = reinterpret_cast<Rectangle*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		*result = obj->GetUpperRightX();
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Rectangle_SetUpperRightX(RectangleHandle* handle, bigint_type data) {
	Rectangle* obj = reinterpret_cast<Rectangle*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);

	try
	{
		obj->SetUpperRightX(data);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Rectangle_GetUpperRightY(RectangleHandle* handle, bigint_type* result)
{
	Rectangle* obj = reinterpret_cast<Rectangle*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		*result = obj->GetUpperRightY();
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Rectangle_SetUpperRightY(RectangleHandle* handle, bigint_type data) {
	Rectangle* obj = reinterpret_cast<Rectangle*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);

	try
	{
		obj->SetUpperRightY(data);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Rectangle_ToUnknown(RectangleHandle* handle, IUnknownHandle** result) {
	return SafeObjectConvert<Rectangle, IUnknown, RectangleHandle, IUnknownHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION Rectangle_FromUnknown(IUnknownHandle* handle, RectangleHandle** result) {
	return SafeObjectConvert<IUnknown, Rectangle, IUnknownHandle, RectangleHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION Rectangle_Release(RectangleHandle* handle) {
	return ObjectRelease<Rectangle, RectangleHandle>(handle);
}
