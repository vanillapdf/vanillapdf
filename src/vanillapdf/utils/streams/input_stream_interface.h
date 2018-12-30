#ifndef _INPUT_STREAM_INTERFACE_H
#define _INPUT_STREAM_INTERFACE_H

#include "utils/utils_fwd.h"
#include "utils/buffer.h"

#include <ios>

namespace vanillapdf {

class IInputStream : public virtual IUnknown {
public:
	virtual types::stream_size Read(Buffer& result, types::stream_size len) = 0;
	virtual BufferPtr Read(types::stream_size len) = 0;
	virtual BufferPtr Readline(void) = 0;
	virtual types::stream_size GetInputPosition() = 0;
	virtual void SetInputPosition(types::stream_size pos) = 0;
	virtual void SetInputPosition(types::stream_size pos, std::ios_base::seekdir way) = 0;

	virtual bool Eof(void) const = 0;
	virtual bool Ignore(void) = 0;
	virtual int Get(void) = 0;
	virtual int Peek(void) = 0;

	virtual bool IsFail(void) const = 0;

	virtual operator bool(void) const = 0;

	virtual ~IInputStream() {};
};

} // vanillapdf

#endif /* _INPUT_STREAM_INTERFACE_H */
