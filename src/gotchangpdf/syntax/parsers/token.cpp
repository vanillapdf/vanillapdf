#include "precompiled.h"
#include "token.h"

#include "exception.h"
#include "token_dictionary.h"

#include <cassert>

namespace gotchangpdf
{
	namespace syntax
	{
		Token::Token(Type type) : _type(type) { assert(type != Type::UNKNOWN); }
		Token::Token(Type type, BufferPtr chars) : _type(type), _value(chars) {}
	}
}
