#include "object.h"
#include "exception.h"
#include "gotchangpdf.h"

#include <cassert>

namespace gotchangpdf
{
	Object::Object(Type type) : _type(type), _intrusive_ref_count(0) {}
	Object::Object() : _type(Type::Unknown), _intrusive_ref_count(0) {}
	Object::~Object() {}

	Object::Type Object::GetType(void) const
	{
		// TODO validate
		//typeid(*this).name();

		return _type;
	}

	void Object::Release()
	{
		boost::intrusive_ptr_release(this);
	}

	const char* Object::TypeName(Type type)
	{
		// TODO this should be a single line solution using preprocessor

		switch (type)
		{
		case Type::Unknown:
			return "Unknown";
		case Type::ArrayObject:
			return "ArrayObject";
		case Type::Boolean:
			return "Boolean";
		case Type::DictionaryObject:
			return "DictionaryObject";
		case Type::Function:
			return "Function";
		case Type::IntegerObject:
			return "IntegerObject";
		case Type::NameObject:
			return "NameObject";
			//case Type::NameTree:
			//	return "NameTree";
		case Type::NullObject:
			return "NullObject";
			//case Type::NumberTree:
			//	return "NumberTree";
		case Type::RealObject:
			return "RealObject";
			//case Type::Rectangle:
			//	return "Rectangle";
		case Type::StreamObject:
			return "StreamObject";
		case Type::HexadecimalString:
			return "HexadecimalString";
		case Type::LiteralString:
			return "LiteralString";
		case Type::IndirectReference:
			return "IndirectReference";
		case Type::IndirectObject:
			return "IndirectObject";
		default:
			return nullptr;
		}
	}

}

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
