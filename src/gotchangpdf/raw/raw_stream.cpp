#include "raw_stream.h"
#include "exception.h"
#include "constants.h"

#include <limits>
#include <streambuf>
#include <boost/iostreams/device/back_inserter.hpp>

namespace gotchangpdf
{
	namespace raw
	{
		namespace io = boost::iostreams;
		using namespace character;

		Stream::Stream(CharacterSource & stream)
			: CharacterSource(new FilteringBuffer(stream)) {}

		Stream::Stream(const raw::Stream & other)
			: CharacterSource(other.rdbuf()) {}


		Stream::~Stream() { delete CharacterSource::rdbuf(); }

		Buffer Stream::read(unsigned int len)
		{
			Buffer result;
			result.resize(len);
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

		Buffer Stream::readline(void)
		{
			Buffer result;

			char buf[constant::BUFFER_SIZE];

			auto begin = &buf[0];
			auto end = &buf[constant::BUFFER_SIZE - 1];

			getline(buf, constant::BUFFER_SIZE);
			std::streamsize read = gcount();

			while (constant::BUFFER_SIZE == read)
			{
				result.insert(result.begin(), begin, end);
				getline(buf, constant::BUFFER_SIZE);
				read = gcount();
			}

			int rd = static_cast<int>(read);
			char ch(buf[std::max(0, rd - 2)]);
			std::streamsize pos = (Equals(ch, WhiteSpace::CARRIAGE_RETURN) ? std::max(0, rd - 2) : std::max(0, rd - 1));

			assert(pos >= 0);

			if (Equals(*begin, WhiteSpace::CARRIAGE_RETURN))
				++begin;

			end = &buf[pos];
			result.insert(result.begin(), begin, end);

			return result;
		}
	}
}
