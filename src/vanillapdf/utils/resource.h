#ifndef _RESOURCE_H
#define _RESOURCE_H

#include "utils/buffer.h"

namespace vanillapdf {

enum class ResourceID {
	Undefined = 0,
	MASTER_CERTIFICATE,
	WATERMARK_IMAGE,
	SERIAL_BLACKLIST
};

class Resource {
public:
	static BufferPtr Load(ResourceID id);

private:
	Resource();
};

} // vanillapdf

#endif /* _RESOURCE_H */
