#include "precompiled.h"
#include "filter.h"

#include "deferred.h"
#include "name_object.h"
#include "flate_decode_filter.h"

namespace gotchangpdf
{
	namespace filters
	{		
		FilterPtr Filter::GetByName(const NameObjectPtr name)
		{
			if (name->Equals(constant::Name::FlateDecode))
				return new FlateDecodeFilter();

			throw exceptions::Exception("Unknown filter type: " + name->Value()->ToString());
		}
	}
}
