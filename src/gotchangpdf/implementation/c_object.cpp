#include "object.h"
#include "c_object.h"

#include "c_helper.h"

using namespace gotchangpdf;

GOTCHANG_PDF_API error_type Object_TypeName(enum ObjectType type, out_string_type result)
{
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		*result = Object::TypeName(static_cast<Object::Type>(type));
		return GOTCHANG_PDF_ERROR_SUCCES;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_Type(ObjectHandle handle, PObjectType result)
{
	Object* obj = reinterpret_cast<Object*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		*result = static_cast<ObjectType>(obj->GetType());
		return GOTCHANG_PDF_ERROR_SUCCES;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_Release(ObjectHandle handle)
{
	Object* obj = reinterpret_cast<Object*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);

	try
	{
		obj->Release();
		return GOTCHANG_PDF_ERROR_SUCCES;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}
