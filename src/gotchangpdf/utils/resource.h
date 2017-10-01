#ifndef _RESOURCE_H
#define _RESOURCE_H

#include "utils/buffer.h"

namespace gotchangpdf {

enum class ResourceID {
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

} // gotchangpdf

#endif /* _RESOURCE_H */
