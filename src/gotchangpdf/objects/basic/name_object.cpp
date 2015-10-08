#include "precompiled.h"
#include "name_object.h"

#include <cassert>

namespace gotchangpdf
{
	using namespace lexical;

	std::ostream& operator<<(std::ostream& os, const NameObject& name)
	{
		os << name.Value()->ToString();
		return os;
	}
}

namespace std
{
	size_t hash<gotchangpdf::NameObject>::operator()(const gotchangpdf::NameObject& name) const
	{
		size_t result = 0;
		for (auto & val : *name.Value())
		{
			std::hash<char> hash_fn;
			result ^= hash_fn(val);
		}

		return result;
	}
}
