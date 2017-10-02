#ifndef _INPUT_OUTPUT_STREAM_INTERFACE_H
#define _INPUT_OUTPUT_STREAM_INTERFACE_H

#include "utils/utils_fwd.h"
#include "utils/streams/input_stream_interface.h"
#include "utils/streams/output_stream_interface.h"

namespace vanillapdf {

class IInputOutputStream : public virtual IInputStream, public virtual IOutputStream {
};

} // vanillapdf

#endif /* _INPUT_OUTPUT_STREAM_INTERFACE_H */
