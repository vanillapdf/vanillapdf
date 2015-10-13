#include "precompiled.h"
#include "filter_utils.h"

#include "deferred.h"
#include "name_object.h"

namespace gotchangpdf
{
	namespace filters
	{
		FilterPtr GetFilterByName(const NameObjectPtr name)
		{
			if (name->Equals(constant::Name::FlateDecode))
				return FlateDecodeFilter();

			throw exceptions::Exception("Unknown filter type: " + name->Value()->ToString());
		}
	}
}
