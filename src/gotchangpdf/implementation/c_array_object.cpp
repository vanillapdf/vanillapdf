#include "precompiled.h"

#include "syntax/objects/array_object.h"

#include "gotchangpdf/c_array_object.h"
#include "c_helper.h"

using namespace gotchangpdf;
using namespace gotchangpdf::syntax;

GOTCHANG_PDF_API error_type CALLING_CONVENTION ArrayObject_At(ArrayObjectHandle handle, size_type at, ObjectHandle* result)
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

GOTCHANG_PDF_API error_type CALLING_CONVENTION ArrayObject_Size(ArrayObjectHandle handle, size_type* result)
{
	MixedArrayObject* obj = reinterpret_cast<MixedArrayObject*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	*result = obj->Size();
	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION ArrayObject_Append(ArrayObjectHandle handle, ObjectHandle value)
{
	MixedArrayObject* obj = reinterpret_cast<MixedArrayObject*>(handle);
	Object* data = reinterpret_cast<Object*>(value);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(data);

	try
	{
		auto containable_ptr = dynamic_cast<ContainableObject*>(data);
		if (nullptr == containable_ptr) {
			return GOTCHANG_PDF_ERROR_PARAMETER_VALUE;
		}

		obj->Append(containable_ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION ArrayObject_Insert(ArrayObjectHandle handle, size_type at, ObjectHandle value)
{
	MixedArrayObject* obj = reinterpret_cast<MixedArrayObject*>(handle);
	Object* data = reinterpret_cast<Object*>(value);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(data);

	try
	{
		auto containable_ptr = dynamic_cast<ContainableObject*>(data);
		if (nullptr == containable_ptr) {
			return GOTCHANG_PDF_ERROR_PARAMETER_VALUE;
		}

		obj->Insert(containable_ptr, at);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION ArrayObject_Remove(ArrayObjectHandle handle, size_type at)
{
	MixedArrayObject* obj = reinterpret_cast<MixedArrayObject*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);

	try
	{
		obj->Remove(at);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION ArrayObject_Release(ArrayObjectHandle handle)
{
	return ObjectRelease<MixedArrayObject, ArrayObjectHandle>(handle);
}
