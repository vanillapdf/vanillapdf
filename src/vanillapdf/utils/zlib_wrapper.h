#ifndef _ZLIB_WRAPPER_H
#define _ZLIB_WRAPPER_H

#include "utils/utils_fwd.h"
#include "utils/constants.h"

#include "utils/streams/input_stream_interface.h"

namespace vanillapdf {

class ZlibWrapper {
public:
    static BufferPtr Deflate(const Buffer& input);
    static BufferPtr Inflate(const Buffer& input);

    static BufferPtr Deflate(IInputStreamPtr input, types::stream_size length);
    static BufferPtr Inflate(IInputStreamPtr input, types::stream_size length);

private:
    ZlibWrapper() = default;
};

} // vanillapdf

#endif /* _ZLIB_WRAPPER_H */
