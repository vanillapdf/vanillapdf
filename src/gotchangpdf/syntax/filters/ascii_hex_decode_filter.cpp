#include "precompiled.h"

#include "utils/character.h"

#include "syntax/filters/ascii_hex_decode_filter.h"

namespace gotchangpdf {
namespace syntax {

char HexPairToByte(const std::string& hex_pair) {
	assert(hex_pair.size() == 2);
	auto val = std::stoi(hex_pair, 0, 16);
	auto converted = ValueConvertUtils::SafeConvert<unsigned char, decltype(val)>(val);
	return reinterpret_cast<char&>(converted);
}

BufferPtr ASCIIHexDecodeFilter::Encode(IInputStreamPtr, types::stream_size, DictionaryObjectPtr parameters/* = DictionaryObjectPtr() */) const {
	throw NotSupportedException("ASCIIHexDecodeFilter encoding is not supported");
}

BufferPtr ASCIIHexDecodeFilter::Decode(IInputStreamPtr src, types::stream_size length, DictionaryObjectPtr parameters/* = DictionaryObjectPtr() */) const {
	BufferPtr result;

	std::string hex_pair;
	for (decltype(length) i = 0; i < length; ++i) {
		auto meta = src->Get();
		if (meta == std::char_traits<char>::eof()) {
			throw GeneralException("Unexpected end of file inside stream");
		}

		auto ch = ValueConvertUtils::SafeConvert<unsigned char>(meta);

		// End of data marker
		if (ch == '>') {
			break;
		}

		// skip whitespaces without any errors
		if (IsWhiteSpace(ch)) {
			continue;
		}

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

BufferPtr ASCIIHexDecodeFilter::Encode(BufferPtr src, DictionaryObjectPtr parameters) const {
	auto stream = src->ToInputStream();
	return Encode(stream, src->size());
}

BufferPtr ASCIIHexDecodeFilter::Decode(BufferPtr src, DictionaryObjectPtr parameters) const {
	auto stream = src->ToInputStream();
	return Decode(stream, src->size());
}

} // syntax
} // gotchangpdf
