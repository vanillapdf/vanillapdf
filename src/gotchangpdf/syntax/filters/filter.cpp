#include "precompiled.h"
#include "filter.h"
#include "flate_decode_filter.h"
#include "ascii_85_decode_filter.h"

namespace gotchangpdf
{
	namespace syntax
	{
		FilterBasePtr FilterBase::GetFilterByName(const NameObjectPtr name)
		{
			if (name->Equals(constant::Name::FlateDecode))
				return FlateDecodeFilterPtr();
			else if (name->Equals(constant::Name::ASCII85Decode))
				return ASCII85DecodeFilterPtr();

			throw syntax::Exception("Unknown filter type: " + name->Value()->ToString());
		}
	}
}
