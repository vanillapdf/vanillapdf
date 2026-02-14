#ifndef _STREAM_UTILS_H
#define _STREAM_UTILS_H

#include "utils/streams/seek_direction.h"
#include "utils/streams/input_stream_interface.h"
#include "utils/streams/output_stream_interface.h"
#include "utils/streams/input_output_stream_interface.h"

#include <ios>

namespace vanillapdf {

class StreamUtils {
public:
    static IOutputStreamPtr OutputStreamFromFile(const std::string& filename);
    static IInputStreamPtr InputStreamFromFile(const std::string& filename);

    static IInputOutputStreamPtr InputOutputStreamFromFile(const std::string& filename);
    static IInputOutputStreamPtr InputOutputStreamFromMemory();

    static IInputStreamPtr InputStreamFromBuffer(BufferPtr data);
    static BufferPtr InputStreamToBuffer(IInputStreamPtr source);

    static SeekDirection ConvertToSeekDirection(std::ios_base::seekdir value);
    static std::ios_base::seekdir ConvertFromSeekDirection(SeekDirection value);

    static IInputOutputStreamPtr CreateFileStream(const std::string& path, std::ios_base::openmode mode);
    static IInputOutputStreamPtr CreateMemoryBufferStream(const std::string& path, std::ios_base::openmode mode);

private:
    static std::shared_ptr<std::fstream> OpenFileStream(const std::string& path, std::ios_base::openmode mode);
};

} // vanillapdf

#endif /* _STREAM_UTILS_H*/
