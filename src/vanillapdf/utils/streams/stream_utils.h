#ifndef _STREAM_UTILS_H
#define _STREAM_UTILS_H

#include "utils/streams/input_stream_interface.h"
#include "utils/streams/output_stream_interface.h"
#include "utils/streams/input_output_stream_interface.h"

namespace vanillapdf {

class StreamUtils {
public:
	static IOutputStreamPtr OutputStreamFromFile(const std::string& filename);
	static IInputStreamPtr InputStreamFromFile(const std::string& filename);
	static IInputOutputStreamPtr InputOutputStreamFromFile(const std::string& filename);

	static IInputStreamPtr InputStreamFromBuffer(BufferPtr data);
	static BufferPtr InputStreamToBuffer(IInputStreamPtr source);
};

} // vanillapdf

#endif /* _STREAM_UTILS_H*/
