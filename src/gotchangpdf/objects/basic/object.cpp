#include "object.h"
#include "exception.h"
#include "export.h"
#include "indirect_object.h"

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
		::boost::intrusive_ptr_release(this);
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

	#pragma region DllInterface

	const char* IObject::TypeName(Type type)
	{
		return Object::TypeName(type);
	}

	IObject::Type IObject::GetType(void) const
	{
		auto removed = const_cast<IObject*>(this);
		auto obj = reinterpret_cast<Object*>(removed);

		return obj->GetType();
	}

	void IObject::Release()
	{
		auto removed = const_cast<IObject*>(this);
		auto obj = reinterpret_cast<Object*>(removed);

		return obj->Release();
	}

	IObject::~IObject() {}

	#pragma endregion
}
