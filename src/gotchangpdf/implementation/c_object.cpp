#include "object.h"
#include "c_object.h"

using namespace gotchangpdf;

GOTCHANG_PDF_API const char* Object_TypeName(ObjectType type)
{
	return Object::TypeName(static_cast<Object::Type>(type));
}

GOTCHANG_PDF_API ObjectType CALLING_CONVENTION Object_Type(ObjectHandle handle)
{
	Object* obj = reinterpret_cast<Object*>(handle);
	return static_cast<ObjectType>(obj->GetType());
}

GOTCHANG_PDF_API void CALLING_CONVENTION Object_Release(ObjectHandle handle)
{
	Object* obj = reinterpret_cast<Object*>(handle);
	obj->Release();
}
