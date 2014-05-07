#include "raw_reverse_stream.h"
#include "exception.h"
#include "constants.h"

namespace gotchangpdf
{
	namespace raw
	{
		using namespace exceptions;
		using namespace std;

		ReverseStream::ReverseBuf::ReverseBuf(istream& s)
		{
			/* TODO optimization */

			/* do not mess with invalid streams */
			if (s.eof() || s.fail())
				throw Exception("Input stream is in invalid state");

			/* get current offset for later restoration */
			auto offset = s.tellg();

			/* read whole stream */
			s.seekg(ios_base::beg);

			char buf[Constant::BUFFER_SIZE];

			char* begin = &buf[0];
			char* end = &buf[Constant::BUFFER_SIZE - 1];

			/* chunk that dirty bytes */
			while (s.read(buf, Constant::BUFFER_SIZE))
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
		/*
		int ReverseStream::ReverseBuf::sync()
		{
		// cout << s << ": " << str(); str("");  return !cout;
		}
		*/
		ReverseStream::ReverseBuf::~ReverseBuf()
		{
			//pubsync();
		}
		/*
		streambuf::int_type ReverseStream::ReverseBuf::underflow()
		{
		return  gptr() == egptr() ?
		traits_type::eof() :
		traits_type::to_int_type(*gptr());
		}

		streambuf::int_type ReverseStream::ReverseBuf::uflow()
		{
		if (gptr() == egptr())
		return traits_type::eof();

		return traits_type::to_int_type(*_Gninc());
		}

		streambuf::int_type ReverseStream::ReverseBuf::pbackfail(int_type ch)
		{
		if (gptr() == eback() || (ch != traits_type::eof() && ch != gptr()[-1]))
		return traits_type::eof();

		return traits_type::to_int_type(*_Gndec());
		}

		streamsize ReverseStream::ReverseBuf::showmanyc()
		{
		assert(std::less_equal<const char *>()(gptr(), egptr()));
		return egptr() - gptr();
		}
		*/
		ReverseStream::ReverseStream(std::istream& stream) : std::istream(new ReverseBuf(stream)) {}

		ReverseStream::~ReverseStream() { delete std::istream::rdbuf(); }

		void ReverseStream::Read(char* bytes, unsigned int len)
		{
			std::istream::read(bytes, len);
		}

		unique_ptr<char> ReverseStream::Read(size_t len)
		{
			unique_ptr<char> result(new char[len]);

			/* unique_ptr handles memory in case of exception,
			otherwise try-catch would needed to be involved */
			char *hack = result.get();

			Read(hack, len);

			return result;
		}
	}
}
