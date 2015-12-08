#include "precompiled.h"
#include "ascii_85_decode_filter.h"
#include "character.h"

namespace gotchangpdf
{
	namespace syntax
	{
		BufferPtr ASCII85DecodeFilter::Encode(BufferPtr src, DictionaryObjectPtr parameters) const
		{
			return src;
		}

		BufferPtr ASCII85DecodeFilter::Decode(BufferPtr src, DictionaryObjectPtr parameters) const
		{
			BufferPtr result;

			int current = 0;
			char sequence[5] = { 0 };

			// Iterate over all elements
			auto size = src->size();
			for (unsigned int i = 0; i < size; ++i) {
				auto ch = src[i];

				// End of sequence
				if (ch == '~' && i < (size - 1) && src[i + 1] == '>') {
					break;
				}

				// Silently ignore white space
				if (IsWhiteSpace(ch))
					continue;

				// Special case 'z' char is 4 zero bytes
				if (ch == 'z' && current == 0) {
					result->push_back(0);
					result->push_back(0);
					result->push_back(0);
					result->push_back(0);
					continue;
				}

				// Character outside of base 85 range
				if (ch < '!' || ch > 'u')
					throw GeneralException("Illegal character in ascii 85 filter: " + ch);

				// Insert to our sequence
				sequence[current] = ch - '!';

				// Sequence complete
				if (current == 4) {
					int value = 0;
					for (int j = 0; j < 5; ++j)
						value = value * 85 + sequence[j];

					// Insert to result buffer
					result->push_back(value >> 24 & 0xFF);
					result->push_back(value >> 16 & 0xFF);
					result->push_back(value >> 8 & 0xFF);
					result->push_back(value & 0xFF);
				}

				// Increment internal counter
				current = (current + 1) % 5;
			}

			assert(current > 1 && "A final partial group contains only one character");
			if (current == 1) {
				LOG_ERROR << "A final partial group contains only one character";
			}
			else if (current == 2) {
				int value = sequence[0] * 85 * 85 * 85 * 85 + sequence[1] * 85 * 85 * 85 + 85 * 85 * 85 + 85 * 85 + 85;
				result->push_back(value >> 24 & 0xFF);
			}
			else if (current == 3) {
				int value = sequence[0] * 85 * 85 * 85 * 85 + sequence[1] * 85 * 85 * 85 + sequence[2] * 85 * 85 + 85 * 85 + 85;
				result->push_back(value >> 24 & 0xFF);
				result->push_back(value >> 16 & 0xFF);
			}
			else if (current == 4) {
				int value = sequence[0] * 85 * 85 * 85 * 85 + sequence[1] * 85 * 85 * 85 + sequence[2] * 85 * 85 + sequence[3] * 85 + 85;
				result->push_back(value >> 24 & 0xFF);
				result->push_back(value >> 16 & 0xFF);
				result->push_back(value >> 8 & 0xFF);
			}

			return result;
		}
	}
}
