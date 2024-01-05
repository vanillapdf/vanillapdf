#include "precompiled.h"

#include "utils/streams/stream_utils.h"
#include "utils/streams/output_stream.h"

#include "utils/constants.h"

namespace vanillapdf {

OutputStream::OutputStream(std::shared_ptr<std::ostream> stream) : m_stream(stream) {
	if (m_stream == nullptr) {
		throw GeneralException("Could not create output stream");
	}

	m_output_lock = std::shared_ptr<std::recursive_mutex>(pdf_new std::recursive_mutex());
}

void OutputStream::Write(const Buffer& data) {
	m_stream->write(data.data(), data.size());
}

void OutputStream::Write(const Buffer& data, types::stream_size size) {
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

void OutputStream::Write(unsigned char value) {
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

void OutputStream::SetOutputPosition(types::stream_size pos, SeekDirection way) {
	m_stream->seekp(pos, StreamUtils::ConvertFromSeekDirection(way));
}

void OutputStream::ExclusiveOutputLock() {
	m_output_lock->lock();
}

void OutputStream::ExclusiveOutputUnlock() {
	m_output_lock->unlock();
}

} // vanillapdf
