#include "precompiled.h"
#include "file.h"

#include "c_array_object.h"
#include "c_helper.h"

using namespace gotchangpdf;
using namespace gotchangpdf::syntax;

GOTCHANG_PDF_API error_type CALLING_CONVENTION ArrayObject_At(ArrayHandle handle, integer_type at, PObjectHandle result)
{
	MixedArrayObject* obj = reinterpret_cast<MixedArrayObject*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto direct = obj->At(at);
		auto base = ObjectUtils::GetObjectBase(direct);
		auto ptr = base.AddRefGet();
		*result = reinterpret_cast<ObjectHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION ArrayObject_Size(ArrayHandle handle, out_integer_type result)
{
	MixedArrayObject* obj = reinterpret_cast<MixedArrayObject*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	*result = obj->Size();
	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION ArrayObject_Append(ArrayHandle handle, ObjectHandle value)
{
	MixedArrayObject* obj = reinterpret_cast<MixedArrayObject*>(handle);
	Object* data = reinterpret_cast<Object*>(value);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(data);

	try
	{
		auto containable_ptr = dynamic_cast<ContainableObject*>(data);
		if (nullptr == containable_ptr)
			return GOTCHANG_PDF_ERROR_PARAMETER_VALUE;

		obj->Append(containable_ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION ArrayObject_Insert(ArrayHandle handle, integer_type at, ObjectHandle value)
{
	MixedArrayObject* obj = reinterpret_cast<MixedArrayObject*>(handle);
	Object* data = reinterpret_cast<Object*>(value);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(data);

	try
	{
		auto containable_ptr = dynamic_cast<ContainableObject*>(data);
		if (nullptr == containable_ptr)
			return GOTCHANG_PDF_ERROR_PARAMETER_VALUE;

		obj->Insert(containable_ptr, at);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION ArrayObject_Remove(ArrayHandle handle, integer_type at)
{
	MixedArrayObject* obj = reinterpret_cast<MixedArrayObject*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);

	try
	{
		obj->Remove(at);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION ArrayObject_Release(ArrayHandle handle)
{
	return ObjectRelease<MixedArrayObject, ArrayHandle>(handle);
}
