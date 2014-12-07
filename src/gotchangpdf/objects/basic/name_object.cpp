#include "name_object.h"
#include "token.h"

#include <cassert>

namespace gotchangpdf
{
	using namespace lexical;

	NameObject::NameObject(const Token& token) : _value(token.Value()) { assert(token.GetType() == Token::Type::NAME_OBJECT); }
	NameObject::NameObject(const Buffer& name) : _value(name) {}
}

namespace std
{
	size_t hash<gotchangpdf::NameObject>::operator()(const gotchangpdf::NameObject& name) const
	{
		size_t result = 0;
		for (auto & val : name.Value())
		{
			std::hash<char> hash_fn;
			result ^= hash_fn(val);
		}

		return result;
	}
}
