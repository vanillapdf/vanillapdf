#include "precompiled.h"
#include "syntax/streams/raw_reverse_stream.h"

#include "utils/constants.h"
#include "utils/util.h"

#include <cassert>
#include <cstring>

namespace gotchangpdf
{
	namespace constant
	{
		const int REVERSE_BUFFER_SIZE = 2048; // seek with 4096 fails for some reason
		const int REVERSE_BUFFER_PUTBACK_SIZE = 16;
	}

	namespace syntax
	{
		using namespace std;

		ReverseStream::ReverseStream(CharacterSource & stream, types::stream_size size)
			: CharacterSource(pdf_new ReverseBuf(stream, size)) {}

		ReverseStream::~ReverseStream()
		{
			delete CharacterSource::rdbuf();
		}

		ReverseStream::ReverseBuf::ReverseBuf(CharacterSource & s, types::stream_size size)
			: _source(s), _size(size), _offset(0),
			_put_back(constant::REVERSE_BUFFER_PUTBACK_SIZE),
			_buffer(constant::REVERSE_BUFFER_SIZE + _put_back)
		{
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

		int ReverseStream::ReverseBuf::sync()
		{
			//cout << s << ": " << str(); str("");  return !cout;
			return 0;
		}

		ReverseStream::ReverseBuf::int_type ReverseStream::ReverseBuf::underflow()
		{
			// buffer not exhausted
			if (gptr() > egptr()) {
				return traits_type::to_int_type(*gptr());
			}

			char *start = _buffer.data() + _buffer.size() - 1;

			bool put_back = false;
			if (egptr() == _base) // true when this isn't the first fill
			{
				// Make arrangements for putback characters
				std::memmove(_buffer.data() + _buffer.size() - _put_back, _base, _put_back);
				start -= _put_back - 1;
				put_back = true;
			}

			// start is now the start of the buffer, proper.
			// Read from fptr_ in to the provided buffer

			auto to_read = put_back ? _buffer.size() - _put_back : _buffer.size();
			if (_offset - to_read < -_size) {
				to_read = ValueConvertUtils::SafeConvert<decltype(to_read)>(_size + _offset);
				_offset = -_size;
			}
			else {
				_offset -= to_read;
			}

			_source.seekg(_offset, std::ios::end);
			_source.read(_buffer.data(), to_read);
			_base = _buffer.data();

			assert(!_source.fail());

			auto size = _source.gcount();
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

		ReverseStream::ReverseBuf::int_type ReverseStream::ReverseBuf::uflow()
		{
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

		ReverseStream::ReverseBuf::int_type ReverseStream::ReverseBuf::pbackfail(int_type ch)
		{
			if (gptr() == eback() || (ch != traits_type::eof() && ch != gptr()[-1])) {
				return traits_type::eof();
			}

			// Advance single character
			auto result = snextc();
			assert(result != traits_type::eof() && "Unexpected eof");
			return result;
		}

		streamsize ReverseStream::ReverseBuf::showmanyc()
		{
			assert(std::less_equal<const char *>()(egptr(), gptr()));
			return gptr() - egptr();
		}

		ReverseStream::ReverseBuf::pos_type ReverseStream::ReverseBuf::seekoff(off_type offset,
			ios_base::seekdir dir,
			ios_base::openmode mode)
		{
			if (dir == ios_base::end)
				offset += (off_type)(egptr());
			else if (dir == ios_base::cur
				&& (mode & ios_base::out) == 0)
				offset += (off_type)(eback() - gptr());
			else if (dir != ios_base::beg)
				offset = constant::BAD_OFFSET;

			return offset;
		}

		ReverseStream::ReverseBuf::pos_type ReverseStream::ReverseBuf::seekpos(pos_type ptr,
			ios_base::openmode)
		{
			pos_type offset = eback() - gptr() + ptr;
			int converted = ValueConvertUtils::SafeConvert<int, pos_type>(offset);
			gbump(converted);

			return ptr;
		}

		void ReverseStream::read(Buffer& result, size_t len)
		{
			result.resize(len);
			CharacterSource::read(result.data(), len);
		}

		BufferPtr ReverseStream::read(size_t len)
		{
			BufferPtr result(len);
			CharacterSource::read(result->data(), len);
			return result;
		}

		types::stream_size ReverseStream::GetPosition()
		{
			assert(!fail());

			if (eof()) {
				return constant::BAD_OFFSET;
			}

			return tellg();
		}

		void ReverseStream::SetPosition(types::stream_size pos)
		{
			// of badoff is specified, set eof flag
			if (pos == constant::BAD_OFFSET) {
				clear(eofbit);
				return;
			}

			// clear eof
			if (eof()) {
				clear(rdstate() & eofbit);
			}

			// seek to the actual position
			seekg(pos);
			assert(!fail() && !eof());

			auto verify_offset = GetPosition();
			assert(pos == verify_offset); verify_offset;
		}

		BufferPtr ReverseStream::readline(void)
		{
			Buffer result;

			Buffer buffer(constant::BUFFER_SIZE);
			for (;;) {
				getline(buffer.data(), constant::BUFFER_SIZE);
				auto read = gcount();
				if (0 == read) {
					break;
				}

				auto read_converted = ValueConvertUtils::SafeConvert<Buffer::iterator::difference_type>(read);
				std::reverse(buffer.begin(), buffer.begin() + read_converted);
				result.insert(result.begin(), buffer.begin(), buffer.begin() + read_converted);
			}

			unget();

			return result;
		}
	}
}
