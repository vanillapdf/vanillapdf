#include "precompiled.h"
#include "raw_stream.h"

#include "exception.h"
#include "constants.h"
#include "util.h"

namespace gotchangpdf
{
	namespace raw
	{
		Stream::Stream(CharacterSource & stream)
			: CharacterSource(stream.rdbuf()) {}

		Stream::~Stream() {}

		Stream::Stream(const raw::Stream & other)
			: CharacterSource(other.rdbuf()) {}

		BufferPtr Stream::read(types::uinteger len)
		{
			Buffer result(len);
			CharacterSource::read(result.data(), len);
			return result;
		}

		void Stream::read(BufferPtr& result, types::uinteger len)
		{
			result->resize(len);
			CharacterSource::read(result->data(), len);
		}

		BufferPtr Stream::readline(void)
		{
			Buffer result;

			while (!eof()) {
				auto val = get();
				assert(val != std::char_traits<char>::eof());

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

				result.push_back(value);
			}

			return result;
		}
	}
}
