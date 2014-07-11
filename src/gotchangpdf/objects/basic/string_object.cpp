#include "string_object.h"
#include "token.h"

#include <cassert>

namespace gotchangpdf
{
	using namespace std;
	using namespace lexical;

	LiteralString::LiteralString(const Token& token) : StringObject(token.value()) {}
	LiteralString::LiteralString(const Buffer& value) : StringObject(value)
	{
		// TODO remove <> if contains
	}

	HexadecimalString::HexadecimalString(const Token& token)
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

	HexadecimalString::HexadecimalString(const Buffer& value)
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

	StringObject::StringObject(const Buffer& value) : _value(value) {}
	StringObject::StringObject() {}
}