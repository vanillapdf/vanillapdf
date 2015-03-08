#include "precompiled.h"
#include "file.h"

#include "c_object.h"
#include "c_helper.h"

using namespace gotchangpdf;

GOTCHANG_PDF_API error_type Object_TypeName(enum ObjectType type, out_string_type result)
{
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);
	try
	{
		auto value = static_cast<Object::Type>(type);
		*result = Object::TypeName(value);
		return GOTCHANG_PDF_ERROR_SUCCES;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_Type(ObjectHandle handle, PObjectType result)
{
	Object* obj = reinterpret_cast<Object*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);
	LOG_SCOPE(obj->GetFile()->GetFilename());

	try
	{
		*result = static_cast<ObjectType>(obj->GetType());
		return GOTCHANG_PDF_ERROR_SUCCES;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_GetOffset(ObjectHandle handle, out_offset_type result)
{
	Object* obj = reinterpret_cast<Object*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);
	LOG_SCOPE(obj->GetFile()->GetFilename());

	try
	{
		*result = obj->GetOffset();
		return GOTCHANG_PDF_ERROR_SUCCES;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_Release(ObjectHandle handle)
{
	Object* obj = reinterpret_cast<Object*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	LOG_SCOPE(obj->GetFile()->GetFilename());

	obj->Release();
	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_ToArray(ObjectHandle handle, PArrayHandle result)
{
	Object* obj = reinterpret_cast<Object*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	LOG_SCOPE(obj->GetFile()->GetFilename());

	try
	{
		MixedArrayObject* converted = dynamic_cast<MixedArrayObject*>(obj);
		*result = reinterpret_cast<ArrayHandle>(converted);
		return GOTCHANG_PDF_ERROR_SUCCES;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_ToDictionary(ObjectHandle handle, PDictionaryHandle result)
{
	Object* obj = reinterpret_cast<Object*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	LOG_SCOPE(obj->GetFile()->GetFilename());

	try
	{
		DictionaryObject* converted = dynamic_cast<DictionaryObject*>(obj);
		*result = reinterpret_cast<DictionaryHandle>(converted);
		return GOTCHANG_PDF_ERROR_SUCCES;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_ToStream(ObjectHandle handle, PStreamHandle result)
{
	Object* obj = reinterpret_cast<Object*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	LOG_SCOPE(obj->GetFile()->GetFilename());

	try
	{
		StreamObject* converted = dynamic_cast<StreamObject*>(obj);
		*result = reinterpret_cast<StreamHandle>(converted);
		return GOTCHANG_PDF_ERROR_SUCCES;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_ToIndirectReference(ObjectHandle handle, PIndirectReferenceHandle result)
{
	Object* obj = reinterpret_cast<Object*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	LOG_SCOPE(obj->GetFile()->GetFilename());

	try
	{
		IndirectObjectReference* converted = dynamic_cast<IndirectObjectReference*>(obj);
		*result = reinterpret_cast<IndirectReferenceHandle>(converted);
		return GOTCHANG_PDF_ERROR_SUCCES;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_ToInteger(ObjectHandle handle, PIntegerHandle result)
{
	Object* obj = reinterpret_cast<Object*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	LOG_SCOPE(obj->GetFile()->GetFilename());

	try
	{
		IntegerObject* converted = dynamic_cast<IntegerObject*>(obj);
		*result = reinterpret_cast<IntegerHandle>(converted);
		return GOTCHANG_PDF_ERROR_SUCCES;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_ToName(ObjectHandle handle, PNameHandle result)
{
	Object* obj = reinterpret_cast<Object*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	LOG_SCOPE(obj->GetFile()->GetFilename());

	try
	{
		NameObject* converted = dynamic_cast<NameObject*>(obj);
		*result = reinterpret_cast<NameHandle>(converted);
		return GOTCHANG_PDF_ERROR_SUCCES;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}
