#ifndef _FILTER_UTILS_H
#define _FILTER_UTILS_H

#include "fwd.h"
#include "flate_decode_filter.h"

#include <boost/variant/variant.hpp>

namespace gotchangpdf
{
	namespace filters
	{
		typedef boost::variant<
			FlateDecodeFilterPtr
		> FilterPtr;

		FilterPtr GetFilterByName(const NameObjectPtr name);
	}
}

#endif /* _FILTER_UTILS_H */
