#include "object.h"
#include "exception.h"
#include "export.h"

#include <cassert>

namespace gotchangpdf
{
	Object::Object(Type type) : _type(type), _intrusive_ref_count(0) {}
	Object::Object() : _type(Type::Unknown), _intrusive_ref_count(0) {}
	Object::~Object() {}

	IObject::Type Object::GetType(void) const
	{
		// TODO validate
		//typeid(*this).name();

		return _type;
	}

	void Object::Release()
	{
		boost::intrusive_ptr_release(this);
	}
}
/*
namespace boost
{
	void intrusive_ptr_add_ref(Pdf::Object* obj)
	{
		++obj->_intrusive_ref_count;
	}

	void intrusive_ptr_release(Pdf::Object* obj)
	{
		if (--obj->_intrusive_ref_count == 0)
			delete obj;
	}
}
*/
using namespace gotchangpdf;

GOTCHANG_PDF_API const char* Object_TypeName(ObjectType type)
{
	return IObject::TypeName(static_cast<IObject::Type>(type));
}

GOTCHANG_PDF_API ObjectType CALLING_CONVENTION Object_Type(ObjectHandle handle)
{
	Object* obj = reinterpret_cast<Object*>(handle);
	return static_cast<ObjectType>(obj->GetType());
}

GOTCHANG_PDF_API void CALLING_CONVENTION Object_Release(ObjectHandle handle)
{
	Object* obj = reinterpret_cast<Object*>(handle);
	boost::intrusive_ptr_release(obj);
}
