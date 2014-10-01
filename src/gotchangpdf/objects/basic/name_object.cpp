#include "name_object.h"
#include "token.h"

#include <cassert>

namespace gotchangpdf
{
	using namespace lexical;

	NameObject::NameObject() {}
	NameObject::NameObject(const Token& token) : _value(token.Value()) { assert(token.GetType() == Token::Type::NAME_OBJECT); }
	NameObject::NameObject(const Buffer& name) : _value(name) {}

	unsigned long NameObject::Hasher::operator()(const NameObject& t) const
	{
		unsigned long result = 0;
		for (auto & val : t.Value())
		{
			std::hash<char> hash_fn;
			result ^= hash_fn(val);
		}

		return result;
	}
}
