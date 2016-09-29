#include "precompiled.h"
#include "raw_reverse_stream.h"

#include "exception.h"
#include "constants.h"
#include "util.h"

#include <cassert>

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

		ReverseStream::ReverseBuf::ReverseBuf(CharacterSource & s, types::stream_size size)
			: _source(s), _size(size),
			_offset(0),
			_put_back(constant::REVERSE_BUFFER_PUTBACK_SIZE),
			_buffer(constant::REVERSE_BUFFER_SIZE + _put_back),
			_base(_buffer.data())
		{
			char *end = &_buffer.front() + _buffer.size();
			setg(end, end, end);
		}

		int ReverseStream::ReverseBuf::sync()
		{
			//cout << s << ": " << str(); str("");  return !cout;
			return 0;
		}

		ReverseStream::ReverseBuf::int_type ReverseStream::ReverseBuf::underflow()
		{
			if (gptr() > egptr()) // buffer not exhausted
				return traits_type::to_int_type(*gptr());

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
			if (size <= 0)
				return traits_type::eof();

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
			if (gptr() == egptr())
				return traits_type::eof();

			// We need post decrement, which is not available
			auto current = *gptr();
			_Gndec();
			return traits_type::to_int_type(current);
		}

		ReverseStream::ReverseBuf::int_type ReverseStream::ReverseBuf::pbackfail(int_type ch)
		{
			if (gptr() == eback() || (ch != traits_type::eof() && ch != gptr()[-1]))
				return traits_type::eof();

			return traits_type::to_int_type(*_Gninc());
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
				offset = _BADOFF;

			return (pos_type(offset));
		}

		ReverseStream::ReverseBuf::pos_type ReverseStream::ReverseBuf::seekpos(pos_type ptr,
			ios_base::openmode)
		{
			streamoff offset = (streamoff)ptr;
			gbump((int)(eback() - gptr() + offset));

			return (streampos(offset));
		}

		ReverseStream::ReverseStream(CharacterSource & stream, types::stream_size size) : CharacterSource(pdf_new ReverseBuf(stream, size)) {}

		ReverseStream::~ReverseStream() { delete CharacterSource::rdbuf(); }

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
			auto result = eof() ? std::_BADOFF : tellg();
			assert(!fail());
			return result;
		}

		void ReverseStream::SetPosition(types::stream_size pos)
		{
			// of badoff is specified, set eof flag
			if (pos == std::_BADOFF) {
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
