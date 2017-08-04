#ifndef _INPUT_OUTPUT_STREAM_INTERFACE_H
#define _INPUT_OUTPUT_STREAM_INTERFACE_H

#include "utils/utils_fwd.h"

#include "syntax/streams/input_stream_interface.h"
#include "syntax/streams/output_stream_interface.h"

namespace gotchangpdf {
namespace syntax {

class IInputOutputStream : public virtual IInputStream, public virtual IOutputStream {
};

} // syntax
} // gotchangpdf

#endif /* _INPUT_OUTPUT_STREAM_INTERFACE_H */
