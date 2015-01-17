#include "raw_stream.h"

#include "exception.h"
#include "constants.h"
#include "character.h"

namespace gotchangpdf
{
	namespace raw
	{
		using namespace character;

#ifdef USE_BOOST_FILTERING_STREAMS
		Stream::Stream(CharacterSource & stream)
			: CharacterSource(new FilteringBuffer(stream)) {}

		Stream::~Stream() { delete CharacterSource::rdbuf(); }
#else
		Stream::Stream(CharacterSource & stream)
			: CharacterSource(stream.rdbuf()) {}

		Stream::~Stream() {}
#endif

		Stream::Stream(const raw::Stream & other)
			: CharacterSource(other.rdbuf()) {}

		BufferPtr Stream::read(unsigned int len)
		{
			Buffer result(len);
			CharacterSource::read(result.data(), len);
			return result;
		}

		char Stream::get_hex()
		{
			char val = get();

			if ('0' <= val && val <= '9')
				return val - '0';
			if ('a' <= val && val <= 'f')
				return val + 10 - 'a';
			if ('A' <= val && val <= 'F')
				return val + 10 - 'A';

			throw exceptions::Exception("Unknown hexadecimal character " + val);
		}

		BufferPtr Stream::readline(void)
		{
			Buffer result;

			while (!eof()) {
				int val = get();
				assert(val != std::char_traits<char>::eof());

				if (Equals(val, WhiteSpace::CARRIAGE_RETURN)) {
					int val2 = peek();
					if (Equals(val2, WhiteSpace::LINE_FEED)) {
						get();
					}

					break;
				}

				if (Equals(val, WhiteSpace::LINE_FEED)) {
					break;
				}

				assert(std::numeric_limits<char>::min() <= val &&
					std::numeric_limits<char>::max() >= val);
				char ch = static_cast<char>(val);

				result.push_back(ch);
			}

			return result;
		}
	}
}
