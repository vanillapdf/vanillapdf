#ifndef _INPUT_STREAM_H
#define _INPUT_STREAM_H

#include "utils/streams/input_stream_interface.h"

namespace gotchangpdf {

class InputStream : public virtual IInputStream {
public:
	explicit InputStream(std::shared_ptr<std::istream> stream);

	virtual BufferPtr Read(types::size_type len) override;
	virtual types::stream_size Read(Buffer& result, types::size_type len) override;
	virtual BufferPtr Readline(void) override;
	virtual types::stream_size GetInputPosition() override;
	virtual void SetInputPosition(types::stream_size pos) override;
	virtual void SetInputPosition(types::stream_size pos, std::ios_base::seekdir way) override;

	virtual bool Eof(void) const override;
	virtual InputStream& Ignore(void) override;
	virtual int Get(void) override;
	virtual int Peek(void) override;

	virtual operator bool(void) const override;

protected:
	std::shared_ptr<std::istream> m_stream;
};

} // gotchangpdf

#endif /* _INPUT_STREAM_H*/
