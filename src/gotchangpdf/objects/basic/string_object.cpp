#include "precompiled.h"
#include "string_object.h"

#include <cassert>

namespace gotchangpdf
{
	using namespace std;
	using namespace lexical;

	LiteralStringObject::LiteralStringObject(BufferPtr value) : StringObject(value)
	{
		// TODO remove <> if contains
	}

	HexadecimalStringObject::HexadecimalStringObject(BufferPtr value)
	{
		_hexadecimal = value->ToString();

		int len = (_hexadecimal.length() / 2) - 2;
		
		for (int i = 0; i < len; ++i)
		{
			int val = stoi(_hexadecimal.substr(i * 2, 2), 0, 16);

			assert(std::numeric_limits<unsigned char>::min() < val &&
				std::numeric_limits<unsigned char>::max() > val);

			/* this can be done, because we are dealing with 2
			hexadecimal chars, which cannot be greater than 0xFF */
			auto parsed = static_cast<char>(val);
			_value->push_back(parsed);
		}
	}

	StringObject::StringObject(BufferPtr value) : _value(value) {}
	StringObject::StringObject() {}
}