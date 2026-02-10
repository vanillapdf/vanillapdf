#ifndef _FILE_STREAM_INPUT_OUTPUT_STREAM_H
#define _FILE_STREAM_INPUT_OUTPUT_STREAM_H

#include "utils/streams/input_output_stream_interface.h"
#include "utils/streams/file_stream_input_stream.h"
#include "utils/streams/file_stream_output_stream.h"

namespace vanillapdf {

#if defined(COMPILER_MICROSOFT_VISUAL_STUDIO)

    // Visual studio triggers warning about dominance:
    // "warning C4250: 'FileStreamInputOutputStream': inherits 'FileStreamInputStream::Read' via dominance"

    // https://stackoverflow.com/questions/6864550/c-inheritance-via-dominance-warning

    // While IInputStream and IOutputStream is already implemented and propagated via dominance.
    // Other compilers do not trigger any warning and I also believe it's harmless.

    #pragma warning (push)
    #pragma warning (disable : 4250)

#endif /* COMPILER_MICROSOFT_VISUAL_STUDIO */

class FileStreamInputOutputStream : public IInputOutputStream, public FileStreamInputStream, public FileStreamOutputStream {
public:
    explicit FileStreamInputOutputStream(std::shared_ptr<FILE> file);
};

inline FileStreamInputOutputStream::FileStreamInputOutputStream(std::shared_ptr<FILE> file) : FileStreamInputStream(file), FileStreamOutputStream(file) {

}

#if defined(COMPILER_MICROSOFT_VISUAL_STUDIO)
    #pragma warning (pop)
#endif /* COMPILER_MICROSOFT_VISUAL_STUDIO */

} // vanillapdf

#endif /* _FILE_STREAM_INPUT_OUTPUT_STREAM_H */
