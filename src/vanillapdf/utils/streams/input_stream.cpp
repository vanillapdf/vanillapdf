#include "precompiled.h"

#include "utils/streams/input_stream.h"

#include "utils/constants.h"

namespace vanillapdf {

InputStream::InputStream(std::shared_ptr<std::istream> stream) : m_stream(stream) {
	if (m_stream == nullptr) {
		throw GeneralException("Could not create input stream");
	}
}

BufferPtr InputStream::Read(types::stream_size len) {
	BufferPtr result = make_deferred_container<Buffer>(len);
	m_stream->read(result->data(), len);

	// Check the read data size
	types::stream_size bytes_read = m_stream->gcount();
	assert(bytes_read <= len);

	// Trim the buffer in case there is not enough data
	if (bytes_read < len) {
		auto bytes_read_converted = ValueConvertUtils::SafeConvert<types::size_type>(bytes_read);
		result->resize(bytes_read_converted);
	}

	return result;
}

types::stream_size InputStream::Read(char* result, types::stream_size len) {
	m_stream->read(result, len);
	return m_stream->gcount();
}

types::stream_size InputStream::Read(Buffer& result, types::stream_size len) {
	auto length_converted = ValueConvertUtils::SafeConvert<types::size_type>(len);

	assert(result.size() >= length_converted);
	if (result.size() < length_converted) {
		result.resize(length_converted);
	}

	return Read(result.data(), length_converted);
}

types::stream_size InputStream::GetInputPosition() {
	assert(!m_stream->fail());

	if (m_stream->eof()) {
		return constant::BAD_OFFSET;
	}

	return m_stream->tellg();
}

void InputStream::SetInputPosition(types::stream_size pos, std::ios_base::seekdir way) {
	auto initial_offset = GetInputPosition();

	// if badoff is specified, set eof flag
	if (pos == constant::BAD_OFFSET) {
		m_stream->setstate(m_stream->eofbit);
		return;
	}

	// clear eof
	if (m_stream->eof() || m_stream->fail()) {
		m_stream->clear();
	}

	// seek to the actual position
	m_stream->seekg(pos, way);

	// clear fail flags in case we accessed EOF
	if (m_stream->fail()) {
		m_stream->clear(m_stream->rdstate() & m_stream->failbit);
	} else {
		auto verify_offset = GetInputPosition();
		if (way == std::ios_base::beg) {
			// verify if the position is correct
			assert(pos == verify_offset);
		}

		if (way == std::ios_base::cur) {
			// verify if the position is correct
			assert(initial_offset + pos == verify_offset);
		}

		UNUSED(verify_offset);
		UNUSED(initial_offset);
	}
}

void InputStream::SetInputPosition(types::stream_size pos) {
	SetInputPosition(pos, std::ios_base::beg);
}

BufferPtr InputStream::Readline(void) {
	BufferPtr result;

	bool stream_failed = m_stream->fail();
	assert(!stream_failed && "Stream is in failed state");
	if (stream_failed) {
		throw GeneralException("Stream is in failed state");
	}

	bool stream_eof = m_stream->eof();
	assert(!stream_eof && "Stream reached eof");
	if (stream_eof) {
		throw GeneralException("Stream reached eof");
	}

	for (;;) {
		auto eof_test = m_stream->peek();
		if (eof_test == std::char_traits<char>::eof()) {
			break;
		}

		auto new_line = m_stream->get();
		if (new_line == '\r') {
			int line_feed = m_stream->peek();
			if (line_feed == '\n') {
				m_stream->ignore();
			}

			break;
		}

		if (new_line == '\n') {
			break;
		}

		auto converted = ValueConvertUtils::SafeConvert<unsigned char>(new_line);
		result->push_back(converted);
	}

	return result;
}

bool InputStream::Eof(void) const {
	assert(!m_stream->fail());
	return m_stream->eof();
}

bool InputStream::Ignore(void) {
	assert(!m_stream->eof());
	m_stream->ignore();
	assert(!m_stream->fail());
	return m_stream->operator bool();
}

int InputStream::Get(void) {
	assert(!m_stream->eof());
	int result = m_stream->get();
	assert(!m_stream->fail());
	return result;
}

int InputStream::Peek(void) {
	assert(!m_stream->eof());
	int result = m_stream->peek();
	assert(!m_stream->fail());
	return result;
}

bool InputStream::IsFail(void) const {
	return m_stream->fail();
}

InputStream::operator bool(void) const {
	assert(!m_stream->fail());
	return m_stream->operator bool();
}

} // vanillapdf
