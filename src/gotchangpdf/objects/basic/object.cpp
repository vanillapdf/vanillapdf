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
		case Type::IndirectObjectReference:
			return "IndirectReference";
		case Type::IndirectObject:
			return "IndirectObject";
		default:
			return nullptr;
		}
	}
}
