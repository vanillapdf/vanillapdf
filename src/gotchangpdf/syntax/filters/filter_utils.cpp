#include "precompiled.h"
#include "filter_utils.h"

#include "deferred.h"
#include "name_object.h"

namespace gotchangpdf
{
	namespace syntax
	{
		FilterPtr GetFilterByName(const NameObjectPtr name)
		{
			if (name->Equals(constant::Name::FlateDecode))
				return FlateDecodeFilter();

			throw syntax::Exception("Unknown filter type: " + name->Value()->ToString());
		}
	}
}
