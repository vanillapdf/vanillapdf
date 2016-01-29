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
				auto val = get();
				if (val == std::char_traits<char>::eof()) {
					throw GeneralException("Readling encountered end of stream");
				}

				auto value = SafeConvert<char>(val);

				if (value == '\r') {
					int val2 = peek();
					if (IsInRange<decltype(val2), char>(val2) && (static_cast<char>(val2) == '\n')) {
						get();
					}

					break;
				}

				if (value == '\n') {
					break;
				}

				result->push_back(value);
			}

			return result;
		}
	}
}
