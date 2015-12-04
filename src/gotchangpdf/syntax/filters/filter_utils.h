#ifndef _FILTER_UTILS_H
#define _FILTER_UTILS_H

#include "syntax_fwd.h"
#include "flate_decode_filter.h"

namespace gotchangpdf
{
	namespace syntax
	{
		FilterBasePtr GetFilterByName(const NameObjectPtr name);
	}
}

#endif /* _FILTER_UTILS_H */
