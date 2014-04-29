#include "NameObject.h"
#include "Token.h"

#include <cassert>

namespace Pdf
{
	using namespace Lexical;

	NameObject::NameObject() : Object(Object::Type::NameObject) {}
	NameObject::NameObject(const Token& token) : Object(Object::Type::NameObject), _value(token.value()) { assert(token.type() == Token::Type::NAME_OBJECT); }
	NameObject::NameObject(const CharacterSet& name) : _value(name) {}
	bool NameObject::operator==(const NameObject& other) const { return _value == other._value; }

	const ICharacterSet& NameObject::Value() const { return _value; }
}
