#include "precompiled.h"
#include "raw_stream.h"

#include "exception.h"
#include "constants.h"
#include "util.h"

namespace gotchangpdf
{
	namespace syntax
	{
		Stream::Stream(CharacterSource & stream)
			: CharacterSource(stream.rdbuf()) {}

		Stream::~Stream() {}

		Stream::Stream(const Stream & other)
			: CharacterSource(other.rdbuf()) {}

		BufferPtr Stream::read(size_t len)
		{
			BufferPtr result(len);
			CharacterSource::read(result->data(), len);
			return result;
		}

		void Stream::read(Buffer& result, size_t len)
		{
			result.resize(len);
			CharacterSource::read(result.data(), len);
		}

		BufferPtr Stream::readline(void)
		{
			BufferPtr result;

			for (;;) {
				auto new_line = get();
				if (new_line == std::char_traits<char>::eof()) {
					break;
				}

				if (new_line == '\r') {
					int line_feed = peek();
					if (line_feed == '\n') {
						ignore();
					}

					break;
				}

				if (new_line == '\n') {
					break;
				}

				auto converted = ValueConvertUtils::SafeConvert<unsigned char>(new_line);
				result->push_back(converted);
			}

			return result;
		}
	}
}
