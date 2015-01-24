#include "precompiled.h"
#include "object.h"

#include "exception.h"

#include <cassert>

namespace gotchangpdf
{
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
		default:
			return nullptr;
		}
	}
}
