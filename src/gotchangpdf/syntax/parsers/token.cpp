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
		Token::Token(BufferPtr chars) : _type(Evaluate(chars)), _value(chars) { assert(_value->size() > 0); }

		Token::Token(Type type, BufferPtr chars) : _type(type), _value(chars)
		{
			if (_type == Token::Type::UNKNOWN) {
				assert(_value->size() > 0);
				_type = Evaluate(_value);
			}
		}

		Token::Type Token::Evaluate(BufferPtr chars) { return TokenDictionary::Find(chars); }
	}
}
