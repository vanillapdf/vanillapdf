#include "object.h"
#include "exception.h"

#include <cassert>

namespace gotchangpdf
{
	Object::Object() /*_intrusive_ref_count(0)*/ {}
	Object::~Object() {}

	const char* Object::TypeName(Type type)
	{
		// TODO this should be a single line solution using preprocessor

		switch (type)
		{
		case Type::Unknown:
			return "Unknown";
		case Type::Array:
			return "Array";
		case Type::Boolean:
			return "Boolean";
		case Type::Dictionary:
			return "Dictionary";
		case Type::Function:
			return "Function";
		case Type::Integer:
			return "Integer";
		case Type::Name:
			return "Name";
			//case Type::NameTree:
			//	return "NameTree";
		case Type::Null:
			return "Null";
			//case Type::NumberTree:
			//	return "NumberTree";
		case Type::Real:
			return "Real";
			//case Type::Rectangle:
			//	return "Rectangle";
		case Type::Stream:
			return "Stream";
		case Type::HexadecimalString:
			return "HexadecimalString";
		case Type::LiteralString:
			return "LiteralString";
		case Type::IndirectReference:
			return "IndirectReference";
		case Type::Indirect:
			return "Indirect";
		default:
			return nullptr;
		}
	}
}
