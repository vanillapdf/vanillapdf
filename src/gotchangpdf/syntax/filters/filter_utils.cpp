#include "precompiled.h"
#include "filter_utils.h"

#include "name_object.h"

namespace gotchangpdf
{
	namespace syntax
	{
		FilterBasePtr GetFilterByName(const NameObjectPtr name)
		{
			if (name->Equals(constant::Name::FlateDecode))
				return FlateDecodeFilterPtr();

			throw syntax::Exception("Unknown filter type: " + name->Value()->ToString());
		}
	}
}
