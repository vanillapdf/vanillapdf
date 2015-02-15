#include "precompiled.h"
#include "raw_stream.h"

#include "exception.h"
#include "constants.h"
#include "character.h"
#include "util.h"

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

		BufferPtr Stream::read(types::uinteger len)
		{
			Buffer result(len);
			CharacterSource::read(result.data(), len);
			return result;
		}

		char Stream::get_hex()
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

		BufferPtr Stream::readline(void)
		{
			Buffer result;

			while (!eof()) {
				auto val = get();
				assert(val != std::char_traits<char>::eof());

				if (!IsInRange<decltype(val), char>(val))
					throw exceptions::Exception("Value is out of range");

				auto value = static_cast<char>(val);

				if (Equals(value, WhiteSpace::CARRIAGE_RETURN)) {
					int val2 = peek();
					if (IsInRange<decltype(val2), char>(val2) && Equals(static_cast<char>(val2), WhiteSpace::LINE_FEED)) {
						get();
					}

					break;
				}

				if (Equals(value, WhiteSpace::LINE_FEED)) {
					break;
				}

				result.push_back(value);
			}

			return result;
		}
	}
}
