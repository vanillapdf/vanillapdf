#include "BasicReverseStream.h"
#include "Constants.h"

#include <cassert>

namespace Pdf
{
	namespace Streams
	{
		namespace Basic
		{
			using namespace std;

			ReverseStream::ReverseStream(std::istream& stream) : Raw::ReverseStream(stream) {}

			Character ReverseStream::Peek()
			{
				/*
				auto settings = SettingsGet();
				auto pos = tellg();
				char ch;
				do
				{
				ch = peek();
				} while (std::find(settings->skip.begin(), settings->skip.end(), ch) != settings->skip.end());

				seekg(pos);
				return Character(ch);
				*/

				return Character(peek());
			}

			Character ReverseStream::Get()
			{
				/*
				auto settings = SettingsGet();
				char ch;
				do
				{
				ch = get();
				} while (std::find(settings->skip.begin(), settings->skip.end(), ch) != settings->skip.end());
				*/

				return Character(get());
			}

			shared_ptr<Buffer> ReverseStream::Readline(void)
			{
				shared_ptr<Buffer> result(new Buffer());

				char buf[Constant::BUFFER_SIZE];

				auto begin = &buf[0];
				auto end = &buf[Constant::BUFFER_SIZE - 1];

				getline(buf, Constant::BUFFER_SIZE);
				auto read = gcount();

				while (Constant::BUFFER_SIZE == read)
				{
					Buffer tmp(begin, end);
					tmp.Reverse();
					result->Insert(0, tmp);
					getline(buf, Constant::BUFFER_SIZE);
					read = gcount();
				}

				end = &buf[read];

				Buffer tmp(begin, end);
				tmp.Reverse();
				result->Insert(0, tmp);

				unget();

				return result;
			}

			void ReverseStream::Unget() { unget(); }
		}
	}
}
