#include "precompiled.h"

#include "utils/streams/input_reverse_stream.h"
#include "utils/constants.h"
#include "utils/math_utils.h"
#include "utils/conversion_utils.h"

#include <cassert>
#include <cstring>

namespace vanillapdf {
namespace constant {

const types::size_type REVERSE_BUFFER_SIZE = 2048; // seek with 4096 fails for some reason
const types::size_type REVERSE_BUFFER_PUTBACK_SIZE = 16;

} // constant

InputReverseStream::InputReverseStream(std::shared_ptr<std::istream> stream, types::stream_size size) {
	m_buffer = std::unique_ptr<ReverseBuf>(pdf_new ReverseBuf(stream, size));
	m_stream = std::unique_ptr<std::istream>(pdf_new std::istream(m_buffer.get()));
}

InputReverseStream::ReverseBuf::ReverseBuf(std::shared_ptr<std::istream> stream, types::stream_size size)
	: m_stream(stream), _size(size), _offset(0),
	_put_back(constant::REVERSE_BUFFER_PUTBACK_SIZE),
	_buffer(constant::REVERSE_BUFFER_SIZE + _put_back) {
	// Moved here from initialization list
	// because the it was dependent on buffer
	// initialization and the order was
	// not guaranteed
	_base = _buffer.data();

	// Set last char as begin as well, because we currently have no
	// characters in our buffer
	char *begin = _buffer.data() + _buffer.size() - 1;
	setg(begin, begin, begin);
}

int InputReverseStream::ReverseBuf::sync() {
	//cout << s << ": " << str(); str("");  return !cout;
	return 0;
}

InputReverseStream::ReverseBuf::int_type InputReverseStream::ReverseBuf::underflow() {
	// buffer not exhausted
	if (gptr() > egptr()) {
		return traits_type::to_int_type(*gptr());
	}

	char *start = _buffer.data() + _buffer.size() - 1;

	bool put_back = false;
	if (egptr() == _base) // true when this isn't the first fill
	{
		// Make arrangements for putback characters
		std::memmove(_buffer.data() + _buffer.size() - _put_back, _base, ValueConvertUtils::SafeConvert<size_t>(_put_back));
		start -= _put_back - 1;
		put_back = true;
	}

	// start is now the start of the buffer, proper.
	// Read from fptr_ in to the provided buffer

	std::streamsize to_read;
	if (put_back) {
		assert(_buffer.size() > _put_back);
		auto subtracted = _buffer.size() - _put_back;
		to_read = ValueConvertUtils::SafeConvert<decltype(to_read)>(subtracted);
	} else {
		to_read = ValueConvertUtils::SafeConvert<decltype(to_read)>(_buffer.size());
	}

	if (_offset - to_read < -_size) {
		to_read = SafeAddition<decltype(to_read)>(_size, _offset);
		_offset = -_size;
	} else {
		_offset -= to_read;
	}

	m_stream->seekg(_offset, std::ios::end);
	m_stream->read(_buffer.data(), to_read);
	_base = _buffer.data();

	assert(!m_stream->fail());

	auto size = m_stream->gcount();
	if (size <= 0) {
		return traits_type::eof();
	}

	if (size < to_read) {
		_base = start - size + 1;
		std::memmove(_base, _buffer.data(), ValueConvertUtils::SafeConvert<size_t>(size));
	}

	// Set buffer pointers
	setg(start, start, _base);

	return traits_type::to_int_type(*gptr());
}

InputReverseStream::ReverseBuf::int_type InputReverseStream::ReverseBuf::uflow() {
	auto result = underflow();

	// If underflow failed return immediately
	if (traits_type::eq_int_type(result, traits_type::eof())) {
		return traits_type::eof();
	}

	// Advance the input pointer
	gbump(-1);

	// Return underflow result
	return result;
}

InputReverseStream::ReverseBuf::int_type InputReverseStream::ReverseBuf::pbackfail(int_type ch) {
	if (gptr() == eback() || (ch != traits_type::eof() && ch != gptr()[-1])) {
		return traits_type::eof();
	}

	// Advance single character
	auto result = snextc();
	assert(result != traits_type::eof() && "Unexpected eof");
	return result;
}

std::streamsize InputReverseStream::ReverseBuf::showmanyc() {
	assert(std::less_equal<const char *>()(egptr(), gptr()));
	return gptr() - egptr();
}

InputReverseStream::ReverseBuf::pos_type InputReverseStream::ReverseBuf::seekoff(off_type offset,
	std::ios_base::seekdir dir,
	std::ios_base::openmode mode) {
	if (dir == std::ios_base::end)
		offset += (off_type) (egptr());
	else if (dir == std::ios_base::cur
		&& (mode & std::ios_base::out) == 0)
		offset += (off_type) (eback() - gptr());
	else if (dir != std::ios_base::beg)
		offset = constant::BAD_OFFSET;

	return offset;
}

InputReverseStream::ReverseBuf::pos_type InputReverseStream::ReverseBuf::seekpos(pos_type ptr,
	std::ios_base::openmode) {
	pos_type offset = eback() - gptr() + ptr;
	int converted = ValueConvertUtils::SafeConvert<int, pos_type>(offset);
	gbump(converted);

	return ptr;
}

types::stream_size InputReverseStream::Read(Buffer& result, types::stream_size len) {
	auto length_converted = ValueConvertUtils::SafeConvert<types::size_type>(len);

	assert(result.size() >= length_converted);
	if (result.size() < length_converted) {
		result.resize(length_converted);
	}

	m_stream->read(result.data(), length_converted);
	return m_stream->gcount();
}

BufferPtr InputReverseStream::Read(types::stream_size len) {
	BufferPtr result = make_deferred<Buffer>(len);
	m_stream->read(result->data(), len);
	return result;
}

types::stream_size InputReverseStream::GetInputPosition() {
	assert(!m_stream->fail());

	if (m_stream->eof()) {
		return constant::BAD_OFFSET;
	}

	return m_stream->tellg();
}

void InputReverseStream::SetInputPosition(types::stream_size pos) {
	SetInputPosition(pos, std::ios_base::beg);
}

void InputReverseStream::SetInputPosition(types::stream_size pos, std::ios_base::seekdir way) {
	auto initial_offset = GetInputPosition();

	// of badoff is specified, set eof flag
	if (pos == constant::BAD_OFFSET) {
		m_stream->clear(m_stream->eofbit);
		return;
	}

	// clear eof
	if (m_stream->eof()) {
		m_stream->clear(m_stream->rdstate() & m_stream->eofbit);
	}

	// seek to the actual position
	m_stream->seekg(pos, way);
	assert(!m_stream->fail() && !m_stream->eof());

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

BufferPtr InputReverseStream::Readline(void) {
	BufferPtr result;

	Buffer buffer(constant::BUFFER_SIZE);
	for (;;) {
		m_stream->getline(buffer.data(), constant::BUFFER_SIZE);
		auto read = m_stream->gcount();
		if (0 == read) {
			break;
		}

		auto read_converted = ValueConvertUtils::SafeConvert<Buffer::iterator::difference_type>(read);
		std::reverse(buffer.begin(), buffer.begin() + read_converted);
		result->insert(result.begin(), buffer.begin(), buffer.begin() + read_converted);
	}

	m_stream->unget();

	return result;
}

bool InputReverseStream::Eof(void) const {
	assert(!m_stream->fail());
	return m_stream->eof();
}

InputReverseStream& InputReverseStream::Ignore(void) {
	assert(!m_stream->fail());
	m_stream->ignore();
	return *this;
}

int InputReverseStream::Get(void) {
	assert(!m_stream->fail());
	return m_stream->get();
}

int InputReverseStream::Peek(void) {
	assert(!m_stream->fail());
	return m_stream->peek();
}

InputReverseStream::operator bool(void) const {
	assert(!m_stream->fail());
	return m_stream->operator bool();
}

} // vanillapdf
