#ifndef _OUTPUT_STREAM_H
#define _OUTPUT_STREAM_H

#include "utils/streams/output_stream_interface.h"

namespace vanillapdf {

class OutputStream : public virtual IOutputStream {
public:
	explicit OutputStream(std::shared_ptr<std::ostream> stream);

	virtual void Write(const Buffer& data) override;
	virtual void Write(const Buffer& data, types::stream_size size) override;
	virtual void Write(const std::string& data) override;
	virtual void Write(const char* str) override;
	virtual void Write(char value) override;
	virtual void Write(WhiteSpace value) override;
	virtual void Write(Delimiter value) override;
	virtual void Write(int32_t value) override;
	virtual void Write(uint32_t value) override;
	virtual void Write(int64_t value) override;
	virtual void Write(uint64_t value) override;
	virtual void Flush(void) override;

	virtual types::stream_size GetOutputPosition(void) override;
	virtual void SetOutputPosition(types::stream_size pos) override;
	virtual void SetOutputPosition(types::stream_size pos, std::ios_base::seekdir way) override;

protected:
	std::shared_ptr<std::ostream> m_stream;
};

} // vanillapdf

#endif /* _OUTPUT_STREAM_H*/
