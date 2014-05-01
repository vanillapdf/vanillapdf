#include "BasicStream.h"
#include "Constants.h"

#include <cassert>
#include <algorithm>

namespace Pdf
{
	namespace Streams
	{
		namespace Basic
		{
			using namespace std;

			Stream::Stream(std::istream& stream) : Raw::Stream(stream) {}
			Stream::Stream(const Pdf::Streams::Basic::Stream &other) : Raw::Stream(other) {}

			Character Stream::Peek()
			{
				auto settings = SettingsGet();
				auto pos = tellg();
				char ch;
				do 
				{
					ch = get();
				} while (std::find(settings->skip.begin(), settings->skip.end(), ch) != settings->skip.end());

				seekg(pos);
				return Character(ch);
			}

			Character Stream::Get()
			{
				auto settings = SettingsGet();
				auto pos = tellg();
				char ch;
				do
				{
					ch = get();
				} while (std::find(settings->skip.begin(), settings->skip.end(), ch) != settings->skip.end());

				return Character(ch);
			}

			unique_ptr<CharacterSet> Stream::Readline()
			{
				unique_ptr<CharacterSet> result(new CharacterSet());

				char buf[Constant::BUFFER_SIZE];

				auto begin = &buf[0];
				auto end = &buf[Constant::BUFFER_SIZE - 1];

				getline(buf, Constant::BUFFER_SIZE);
				streamsize read = gcount();

				while (Constant::BUFFER_SIZE == read)
				{
					result->Insert(result->begin(), begin, end);
					getline(buf, Constant::BUFFER_SIZE);
					read = gcount();
				}

				int rd = static_cast<int>(read);
				Character ch(buf[std::max(0, rd - 2)]);
				streamsize pos = ch == Character::WhiteSpace::CARRIAGE_RETURN ? std::max(0, rd - 2) : std::max(0, rd - 1);

				assert(pos >= 0);

				if (static_cast<unsigned char>(Character::WhiteSpace::CARRIAGE_RETURN) == *begin)
					++begin;

				end = &buf[pos];
				result->Insert(result->begin(), begin, end);

				return result;
			}

			void Stream::Unget() { /*TODO*/ unget(); }
		}
	}
}
