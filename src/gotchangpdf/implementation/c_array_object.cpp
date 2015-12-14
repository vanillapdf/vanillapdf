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
		LOG_OBJECT_SCOPE(obj);

		try
		{
			auto direct = obj->At(at);
			auto base = ObjectUtils::GetObjectBase(direct);
			auto ptr = base.AddRefGet();
			*result = reinterpret_cast<ObjectHandle>(ptr);
			return GOTCHANG_PDF_ERROR_SUCCES;
		} CATCH_GOTCHNGPDF_EXCEPTIONS
	} CATCH_SCOPE_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION ArrayObject_Size(ArrayHandle handle, out_integer_type result)
{
	MixedArrayObject* obj = reinterpret_cast<MixedArrayObject*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		LOG_OBJECT_SCOPE(obj);

		*result = obj->Size();
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_SCOPE_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION ArrayObject_Release(ArrayHandle handle)
{
	return ObjectRelease<MixedArrayObject, ArrayHandle>(handle);
}
