#ifndef _INPUT_STREAM_H
#define _INPUT_STREAM_H

#include "syntax/streams/input_stream_interface.h"

namespace gotchangpdf {
namespace syntax {

class InputStream : public IInputStream {
public:
	explicit InputStream(std::shared_ptr<std::istream> stream);

	virtual BufferPtr Read(size_t len) override;
	virtual void Read(Buffer& result, size_t len) override;
	virtual BufferPtr Readline(void) override;
	virtual types::stream_size GetPosition() override;
	virtual void SetPosition(types::stream_size pos) override;
	virtual void SetPosition(types::stream_size pos, std::ios_base::seekdir way) override;

	virtual bool Eof(void) const override;
	virtual InputStream& Ignore(void) override;
	virtual int Get(void) override;
	virtual int Peek(void) override;

	virtual operator bool(void) const override;

protected:
	std::shared_ptr<std::istream> m_stream;
};

} // syntax
} // gotchangpdf

#endif /* _INPUT_STREAM_H*/
