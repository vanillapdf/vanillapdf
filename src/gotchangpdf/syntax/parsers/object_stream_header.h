#ifndef _OBJECT_STREAM_HEADER_H
#define _OBJECT_STREAM_HEADER_H

#include "constants.h"

#include <vector>

namespace gotchangpdf
{
	namespace syntax
	{
		struct ObjectStreamHeader {
			types::integer object_number;
			types::stream_offset offset;
		};

		using ObjectStreamHeaders = std::vector < ObjectStreamHeader > ;
	}
}

#endif /* _OBJECT_STREAM_HEADER_H */
