#ifndef _OUTPUT_STREAM_INTERFACE_H
#define _OUTPUT_STREAM_INTERFACE_H

#include "utils/utils_fwd.h"
#include "utils/buffer.h"
#include "utils/character.h"

#include <ios>

namespace vanillapdf {

class IOutputStream : public virtual IUnknown {
public:
	virtual void Write(const Buffer& data) = 0;
	virtual void Write(const Buffer& data, types::stream_size size) = 0;
	virtual void Write(const std::string& data) = 0;
	virtual void Write(const char* str) = 0;
	virtual void Write(char value) = 0;
	virtual void Write(WhiteSpace value) = 0;
	virtual void Write(Delimiter value) = 0;
	virtual void Write(int32_t value) = 0;
	virtual void Write(uint32_t value) = 0;
	virtual void Write(int64_t value) = 0;
	virtual void Write(uint64_t value) = 0;
	virtual void Flush(void) = 0;

	virtual types::stream_size GetOutputPosition(void) = 0;
	virtual void SetOutputPosition(types::stream_size pos) = 0;
	virtual void SetOutputPosition(types::stream_size pos, std::ios_base::seekdir way) = 0;

	virtual ~IOutputStream() {};
};

inline IOutputStream& operator<<(IOutputStream& os, const std::string& value) {
	os.Write(value);
	return os;
}

inline IOutputStream& operator<<(IOutputStream& os, const Buffer& value) {
	os.Write(value);
	return os;
}

inline IOutputStream& operator<<(IOutputStream& os, const char* value) {
	os.Write(value);
	return os;
}

inline IOutputStream& operator<<(IOutputStream& os, char value) {
	os.Write(value);
	return os;
}

inline IOutputStream& operator<<(IOutputStream& os, WhiteSpace value) {
	os.Write(value);
	return os;
}

inline IOutputStream& operator<<(IOutputStream& os, Delimiter value) {
	os.Write(value);
	return os;
}

inline IOutputStream& operator<<(IOutputStream& os, int32_t value) {
	os.Write(value);
	return os;
}

inline IOutputStream& operator<<(IOutputStream& os, uint32_t value) {
	os.Write(value);
	return os;
}

inline IOutputStream& operator<<(IOutputStream& os, int64_t value) {
	os.Write(value);
	return os;
}

inline IOutputStream& operator<<(IOutputStream& os, uint64_t value) {
	os.Write(value);
	return os;
}

} // vanillapdf

#endif /* _OUTPUT_STREAM_INTERFACE_H */
