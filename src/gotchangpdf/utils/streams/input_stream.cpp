#include "precompiled.h"

#include "utils/streams/input_stream.h"

#include "utils/constants.h"
#include "utils/util.h"

namespace gotchangpdf {

InputStream::InputStream(std::shared_ptr<std::istream> stream) : m_stream(stream) {
}

BufferPtr InputStream::Read(size_t len) {
	BufferPtr result = make_deferred<Buffer>(len);
	m_stream->read(result->data(), len);
	return result;
}

types::stream_size InputStream::Read(Buffer& result, size_t len) {
	assert(result.size() >= len);
	if (result.size() < len) {
		result.resize(len);
	}

	m_stream->read(result.data(), len);
	return m_stream->gcount();
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

	// of badoff is specified, set eof flag
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
			assert(pos == verify_offset); verify_offset;
		}

		if (way == std::ios_base::cur) {
			// verify if the position is correct
			assert(initial_offset + pos == verify_offset);
			verify_offset;
			initial_offset;
		}
	}
}

void InputStream::SetInputPosition(types::stream_size pos) {
	SetInputPosition(pos, std::ios_base::beg);
}

BufferPtr InputStream::Readline(void) {
	BufferPtr result;

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

InputStream& InputStream::Ignore(void) {
	assert(!m_stream->eof());
	m_stream->ignore();
	assert(!m_stream->fail());
	return *this;
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

InputStream::operator bool(void) const {
	assert(!m_stream->fail());
	return m_stream->operator bool();
}

} // gotchangpdf
