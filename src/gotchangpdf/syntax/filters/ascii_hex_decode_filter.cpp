#include "precompiled.h"
#include "ascii_hex_decode_filter.h"
#include "character.h"

namespace gotchangpdf
{
	namespace syntax
	{
		char HexPairToByte(const std::string& hex_pair)
		{
			assert(hex_pair.size() == 2);
			auto val = std::stoi(hex_pair, 0, 16);
			auto converted = SafeConvert<unsigned char, decltype(val)>(val);
			return reinterpret_cast<char&>(converted);
		}

		BufferPtr ASCIIHexDecodeFilter::Encode(BufferPtr src, DictionaryObjectPtr parameters) const
		{
			return src;
		}

		BufferPtr ASCIIHexDecodeFilter::Decode(BufferPtr src, DictionaryObjectPtr parameters) const
		{
			BufferPtr result;

			auto size = src->size();
			std::string hex_pair;
			for (unsigned int i = 0; i < size; ++i) {
				auto ch = src[i];

				// End of data marker
				if (ch == '>')
					break;

				// skip whitespaces without any errors
				if (IsWhiteSpace(ch))
					continue;

				hex_pair.push_back(ch);
				if (hex_pair.size() == 2) {
					// Compute value
					auto converted = HexPairToByte(hex_pair);

					// insert into result
					result->push_back(converted);

					// clear
					hex_pair.clear();
				}
			}

			if (!hex_pair.empty()) {
				// Missing empty value shall be treated
				// as if there is additional zero in the input
				hex_pair.push_back(0);

				// Compute value
				auto converted = HexPairToByte(hex_pair);

				// insert into result
				result->push_back(converted);
			}

			return result;
		}
	}
}
