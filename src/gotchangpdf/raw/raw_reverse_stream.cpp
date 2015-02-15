#include "precompiled.h"
#include "raw_reverse_stream.h"

#include "exception.h"
#include "constants.h"
#include "util.h"

#include <cassert>

namespace gotchangpdf
{
	namespace raw
	{
		using namespace exceptions;
		using namespace std;

		ReverseStream::ReverseBuf::ReverseBuf(CharacterSource & s)// : _source(s)
		{
			/* TODO optimization */

			/* get current offset for later restoration */
			auto offset = s.tellg();

			/* read whole stream */
			s.seekg(ios_base::beg);

			/* do not mess with invalid streams */
			if (s.eof() || s.fail())
				throw Exception("Input stream is in invalid state");

			char buf[constant::BUFFER_SIZE];
			auto begin = &buf[0];
			auto end = &buf[constant::BUFFER_SIZE - 1];

			/* chunk that dirty bytes */
			while (s.read(buf, constant::BUFFER_SIZE))
			{
				reverse(begin, end);
				_buffer.insert(_buffer.begin(), begin, end);
			}

			/* read whats left in the buffer */
			end = &buf[s.gcount()];
			reverse(begin, end);
			_buffer.insert(_buffer.begin(), begin, end);

			/* set our internal buffer */
			setg(_buffer.data(), _buffer.data(), _buffer.data() + _buffer.size());

			/* preserve clean state */
			s.clear();
			s.seekg(offset);
		}

		ReverseStream::ReverseBuf::~ReverseBuf()
		{
			//pubsync();
		}

		ReverseStream::ReverseBuf::pos_type ReverseStream::ReverseBuf::seekoff(off_type offset,
			ios_base::seekdir dir,
			ios_base::openmode mode)
		{
			if (dir == ios_base::end)
				offset += (off_type)(egptr());
			else if (dir == ios_base::cur
				&& (mode & ios_base::out) == 0)
				offset += (off_type)(gptr() - eback());
			else if (dir != ios_base::beg)
				offset = _BADOFF;

			return (pos_type(offset));
		}

		ReverseStream::ReverseBuf::pos_type ReverseStream::ReverseBuf::seekpos(pos_type ptr,
			ios_base::openmode)
		{
			streamoff offset = (streamoff)ptr;
			gbump((int)(gptr() - eback() + offset));

			return (streampos(offset));
		}

		/*
		int ReverseStream::ReverseBuf::sync()
		{
		// cout << s << ": " << str(); str("");  return !cout;
		}
		*/
		/*
		ReverseStream::ReverseBuf::int_type ReverseStream::ReverseBuf::underflow()
		{
			if (gptr() > egptr()) // buffer not exhausted
				return traits_type::to_int_type(*gptr());

			Character * base = _buffer.data();
			Character * start = base;

			if (eback() == base) // true when this isn't the first fill
			{
				// Make arrangements for putback characters
				std::memmove(base, egptr() - put_back_, put_back_);
				start += put_back_;
			}

			// start is now the start of the buffer, proper.
			// Read from fptr_ in to the provided buffer

			//_offset -= constant::BUFFER_SIZE;
			_offset = 0;

			_source.seekg(_offset);
			_source.read(start, constant::BUFFER_SIZE - (start - base));

			assert(!_source.fail());

			if (_source.eof())
				return traits_type::eof();

			// Set buffer pointers
			setg(_buffer.data() + _buffer.size(), _buffer.data() + _buffer.size() - 1, _buffer.data());

			return traits_type::to_int_type(*gptr());
		}

		ReverseStream::ReverseBuf::int_type ReverseStream::ReverseBuf::uflow()
		{
			if (gptr() == egptr())
				return traits_type::eof();

			return traits_type::to_int_type(*_Gndec());
		}

		ReverseStream::ReverseBuf::int_type ReverseStream::ReverseBuf::pbackfail(int_type ch)
		{
			if (gptr() == eback() || (ch != traits_type::eof() && ch != gptr()[-1]))
				return traits_type::eof();

			return traits_type::to_int_type(*_Gninc());
		}

		streamsize ReverseStream::ReverseBuf::showmanyc()
		{
			assert(std::less_equal<const Character *>()(egptr(), gptr()));
			return gptr() - egptr();
		}
		*/
		ReverseStream::ReverseStream(CharacterSource & stream) : CharacterSource(new ReverseBuf(stream)) {}

		ReverseStream::~ReverseStream() { delete CharacterSource::rdbuf(); }

		BufferPtr ReverseStream::read(types::uinteger len)
		{
			Buffer result(len);
			CharacterSource::read(result.data(), len);
			return result;
		}

		BufferPtr ReverseStream::readline(void)
		{
			Buffer result;

			char buf[constant::BUFFER_SIZE];

			auto begin = &buf[0];
			auto end = &buf[constant::BUFFER_SIZE - 1];

			getline(buf, constant::BUFFER_SIZE);
			auto read = gcount();

			while (constant::BUFFER_SIZE == read)
			{
				std::reverse(begin, end);
				result.insert(result.begin(), begin, end);
				getline(buf, constant::BUFFER_SIZE);
				read = gcount();
			}

			end = &buf[read];

			std::reverse(begin, end);
			result.insert(result.begin(), begin, end);

			unget();

			return result;
		}

		char ReverseStream::get_hex()
		{
			auto val = get();

			if (!IsInRange<decltype(val), char>(val))
				throw exceptions::Exception("Value is out of range");

			char ch = static_cast<char>(val);

			if ('0' <= ch && ch <= '9')
				return ch - '0';
			if ('a' <= ch && ch <= 'f')
				return ch + 10 - 'a';
			if ('A' <= ch && ch <= 'F')
				return ch + 10 - 'A';

			throw exceptions::Exception("Unknown hexadecimal character " + val);
		}
	}
}
