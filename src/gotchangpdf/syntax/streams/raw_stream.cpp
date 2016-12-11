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

		types::stream_size Stream::GetPosition()
		{
			return eof() ? constant::BAD_OFFSET : tellg();
		}

		void Stream::SetPosition(types::stream_size pos)
		{
			// of badoff is specified, set eof flag
			if (pos == constant::BAD_OFFSET) {
				setstate(eofbit);
				return;
			}

			// clear eof
			if (eof() || fail()) {
				clear();
			}

			// seek to the actual position
			seekg(pos);

			// clear fail flags in case we accessed EOF
			if (fail()) {
				clear(rdstate() & failbit);
			}
			else {
				// verify if the position is correct
				auto verify_offset = GetPosition();
				assert(pos == verify_offset); verify_offset;
			}
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
