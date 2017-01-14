#ifndef _OBJECT_STREAM_ENTRY_H
#define _OBJECT_STREAM_ENTRY_H

#include "utils/constants.h"
#include "syntax/objects/object.h"

#include <vector>

namespace gotchangpdf
{
	namespace syntax
	{
		struct ObjectStreamEntry
		{
			types::big_uint object_number;
			types::stream_offset offset;
			ObjectPtr object;
		};

		using ObjectStreamEntries = std::vector<ObjectStreamEntry>;
	}
}

#endif /* _OBJECT_STREAM_ENTRY_H */
