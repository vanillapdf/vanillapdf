#include "precompiled.h"

#include "syntax/parsers/token.h"

#include <cassert>

namespace vanillapdf {
namespace syntax {

Token::Token(Type type) : _type(type) { assert(type != Type::UNKNOWN); }
Token::Token(Type type, std::string_view chars) : _type(type), _value(chars) {}

} // syntax
} // vanillapdf
