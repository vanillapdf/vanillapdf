#include "StringObject.h"
#include "Token.h"

#include <cassert>

namespace Pdf
{
	using namespace std;
	using namespace Lexical;

	LiteralString::LiteralString(const Token& token) : StringObject(Object::Type::LiteralString, token.value()) {}
	LiteralString::LiteralString(const Buffer& value) : StringObject(Object::Type::LiteralString, value)
	{
		// TODO remove <> if contains
	}

	HexadecimalString::HexadecimalString(const Token& token) : StringObject(Object::Type::HexadecimalString)
	{
		auto buffer = token.value();
		_hexadecimal = buffer.ToString();

		int len = (_hexadecimal.length() / 2) - 2;

		for (int i = 0; i < len; ++i)
		{
			int val = stoi(_hexadecimal.substr(i * 2, 2), 0, 16);
			Character ch = Character(val);
			_value.Append(ch);
		}
	}

	HexadecimalString::HexadecimalString(const Buffer& value) : StringObject(Object::Type::HexadecimalString)
	{
		_hexadecimal = value.ToString();

		int len = (_hexadecimal.length() / 2) - 2;
		
		for (int i = 0; i < len; ++i)
		{
			int val = stoi(_hexadecimal.substr(i * 2, 2), 0, 16);
			Character ch = Character(val);
			_value.Append(ch);
		}
	}

	StringObject::StringObject(Type type, const Buffer& value) : Object(type), _value(value) { assert(type == Object::Type::HexadecimalString || type == Object::Type::LiteralString); }
	StringObject::StringObject(Type type) : Object(type) { assert(type == Object::Type::HexadecimalString || type == Object::Type::LiteralString); }
}