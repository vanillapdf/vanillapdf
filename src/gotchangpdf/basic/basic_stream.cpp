#include "basic_stream.h"
#include "constants.h"
#include "exception.h"

#include <cassert>
#include <algorithm>

namespace gotchangpdf
{
	namespace basic
	{
		using namespace std;
		using namespace exceptions;

		Stream::Stream(CharacterSource & stream) : raw::Stream(stream) {}
		Stream::Stream(const Stream & other) : raw::Stream(other) {}

		Character Stream::Peek()
		{
			auto settings = BasicSettingsGet();
			auto pos = tellg();
			char ch;
			do
			{
				ch = get();
			} while (std::find(settings.skip.begin(), settings.skip.end(), ch) != settings.skip.end());

			seekg(pos);
			return Character(ch);
		}

		Character Stream::Get()
		{
			auto settings = BasicSettingsGet();
			auto pos = tellg();
			char ch;
			do
			{
				ch = get();
			} while (std::find(settings.skip.begin(), settings.skip.end(), ch) != settings.skip.end());

			return Character(ch);
		}

		Character Stream::GetHex()
		{
			Character result = Get();
			char val = result.Value();

			if ('0' <= val && val <= '9')
				return Character(val - '0');
			if ('a' <= val && val <= 'f')
				return Character(val + 10 - 'a');
			if ('A' <= val && val <= 'F')
				return Character(val + 10 - 'A');

			throw Exception("Unknown hexadecimal character " + val);
		}

		Buffer Stream::Readline()
		{
			Buffer result;

			Character buf[constant::BUFFER_SIZE];

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
			Character ch(buf[std::max(0, rd - 2)]);
			std::streamsize pos = (ch.Equals(Character::WhiteSpace::CARRIAGE_RETURN) ? std::max(0, rd - 2) : std::max(0, rd - 1));

			assert(pos >= 0);

			if (static_cast<Character::ValueType>(Character::WhiteSpace::CARRIAGE_RETURN) == *begin)
				++begin;

			end = &buf[pos];
			result.insert(result.begin(), begin, end);

			return result;
		}

		void Stream::Unget() { /*TODO*/ unget(); }
	}
}
