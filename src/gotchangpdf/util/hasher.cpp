#include "hasher.h"

#include "name_object.h"

namespace std
{
	size_t hash<gotchangpdf::NameObjectPtr>::operator()(const gotchangpdf::NameObjectPtr& name) const
	{
		unsigned long result = 0;
		for (auto & val : name->Value())
		{
			std::hash<char> hash_fn;
			result ^= hash_fn(val);
		}

		return result;
	}
}

