#include "precompiled.h"
#include "object.h"

#include "exception.h"

#include <cassert>

namespace gotchangpdf
{
	namespace syntax
	{
		ObjectPtr::ObjectPtr() : Deferred<Object>(NullObject::GetInstance())
		{

		}

		const char* Object::TypeName(Type type)
		{
			// TODO this should be a single line solution using preprocessor

			switch (type)
			{
			case Type::Array:
				return "Array";
			case Type::Boolean:
				return "Boolean";
			case Type::Dictionary:
				return "Dictionary";
			case Type::Integer:
				return "Integer";
			case Type::Name:
				return "Name";
			case Type::Null:
				return "Null";
			case Type::Real:
				return "Real";
			case Type::Stream:
				return "Stream";
			case Type::String:
				return "String";
			case Type::IndirectReference:
				return "IndirectReference";
			default:
				return nullptr;
			}
		}
	}
}
