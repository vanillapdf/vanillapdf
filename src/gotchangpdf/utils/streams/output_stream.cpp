#include "precompiled.h"

#include "utils/streams/output_stream.h"

#include "utils/constants.h"
#include "utils/util.h"

namespace gotchangpdf {

OutputStream::OutputStream(std::shared_ptr<std::ostream> stream) : m_stream(stream) {

}

void OutputStream::Write(const Buffer& data) {
	m_stream->write(data.data(), data.size());
}

void OutputStream::Write(const Buffer& data, size_t size) {
	m_stream->write(data.data(), size);
}

void OutputStream::Write(const std::string& data) {
	*m_stream << data;
}

void OutputStream::Write(const char* str) {
	*m_stream << str;
}

void OutputStream::Write(char value) {
	*m_stream << value;
}

void OutputStream::Write(WhiteSpace value) {
	*m_stream << static_cast<char>(value);
}

void OutputStream::Write(Delimiter value) {
	*m_stream << static_cast<char>(value);
}

void OutputStream::Write(int32_t value) {
	*m_stream << value;
}

void OutputStream::Write(uint32_t value) {
	*m_stream << value;
}

void OutputStream::Write(int64_t value) {
	*m_stream << value;
}

void OutputStream::Write(uint64_t value) {
	*m_stream << value;
}

void OutputStream::Flush(void) {
	m_stream->flush();
}

types::stream_size OutputStream::GetOutputPosition(void) {
	return m_stream->tellp();
}

void OutputStream::SetOutputPosition(types::stream_size pos) {
	m_stream->seekp(pos);
}

void OutputStream::SetOutputPosition(types::stream_size pos, std::ios_base::seekdir way) {
	m_stream->seekp(pos, way);
}

} // gotchangpdf
