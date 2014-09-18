#include "basic_reverse_stream.h"
#include "constants.h"
#include "exception.h"

#include <cassert>

namespace gotchangpdf
{
	namespace basic
	{
		using namespace exceptions;

		ReverseStream::ReverseStream(std::istream& stream) : raw::ReverseStream(stream) {}

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

		Character ReverseStream::GetHex()
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

		Buffer ReverseStream::Readline(void)
		{
			Buffer result;

			char buf[constant::BUFFER_SIZE];

			auto begin = &buf[0];
			auto end = &buf[constant::BUFFER_SIZE - 1];

			getline(buf, constant::BUFFER_SIZE);
			auto read = gcount();

			while (constant::BUFFER_SIZE == read)
			{
				Buffer tmp(begin, end);
				tmp.Reverse();
				result.Insert(0, tmp);
				getline(buf, constant::BUFFER_SIZE);
				read = gcount();
			}

			end = &buf[read];

			Buffer tmp(begin, end);
			tmp.Reverse();
			result.Insert(0, tmp);

			unget();

			return result;
		}

		void ReverseStream::Unget() { unget(); }
	}
}
