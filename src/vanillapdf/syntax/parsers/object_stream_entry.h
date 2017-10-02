#ifndef _OBJECT_STREAM_ENTRY_H
#define _OBJECT_STREAM_ENTRY_H

#include "utils/constants.h"
#include "syntax/objects/object.h"

#include <vector>

namespace vanillapdf {
namespace syntax {

struct ObjectStreamEntry {
	types::big_uint object_number;
	types::stream_offset offset;
	ObjectPtr object;
};

using ObjectStreamEntries = std::vector<ObjectStreamEntry>;

} // syntax
} // vanillapdf

#endif /* _OBJECT_STREAM_ENTRY_H */
