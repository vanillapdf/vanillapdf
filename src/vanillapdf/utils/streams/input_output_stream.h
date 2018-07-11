#ifndef _INPUT_OUTPUT_STREAM_H
#define _INPUT_OUTPUT_STREAM_H

#include "utils/streams/input_output_stream_interface.h"
#include "utils/streams/input_stream.h"
#include "utils/streams/output_stream.h"

namespace vanillapdf {

#if defined(COMPILER_MICROSOFT_VISUAL_STUDIO)

	// Visual studio triggers warning about dominance:
	// "warning C4250: 'InputOutputStream': inherits 'InputStream::Read' via dominance"

	// https://stackoverflow.com/questions/6864550/c-inheritance-via-dominance-warning

	// While IInputStream and IOutputStream is already implemented and propagated via dominance.
	// Other compilers do not trigger any warning and I also believe it's harmless.

	#pragma warning (push)
	#pragma warning (disable : 4250)

#endif /* COMPILER_MICROSOFT_VISUAL_STUDIO */

class InputOutputStream : public IInputOutputStream, public InputStream, public OutputStream {
public:
	explicit InputOutputStream(std::shared_ptr<std::iostream> stream);
};

inline InputOutputStream::InputOutputStream(std::shared_ptr<std::iostream> stream) : InputStream(stream), OutputStream(stream) {

}

#if defined(COMPILER_MICROSOFT_VISUAL_STUDIO)
	#pragma warning (pop)
#endif /* COMPILER_MICROSOFT_VISUAL_STUDIO */

} // vanillapdf

#endif /* _INPUT_OUTPUT_STREAM_H */
