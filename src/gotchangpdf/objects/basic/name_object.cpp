#include "name_object.h"
#include "token.h"

#include <cassert>

namespace gotchangpdf
{
	using namespace lexical;

	NameObject::NameObject() {}
	NameObject::NameObject(const Token& token) : _value(token.value()) { assert(token.type() == Token::Type::NAME_OBJECT); }
	NameObject::NameObject(const Buffer& name) : _value(name) {}
	bool NameObject::operator==(const NameObject& other) const { return _value == other._value; }
	bool NameObject::operator!=(const NameObject& other) const { return _value != other._value; }
	const Buffer& NameObject::Value() const { return _value; }
}
