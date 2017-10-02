#include "precompiled.h"
#include "ascii_85_decode_filter.h"
#include "utils/character.h"

namespace vanillapdf {
namespace syntax {

BufferPtr ASCII85DecodeFilter::Encode(IInputStreamPtr, types::stream_size, DictionaryObjectPtr parameters /* = DictionaryObjectPtr() */) const {
	throw NotSupportedException("ASCII85DecodeFilter encoding is not supported");
}

BufferPtr ASCII85DecodeFilter::Decode(IInputStreamPtr src, types::stream_size length, DictionaryObjectPtr parameters /* = DictionaryObjectPtr() */) const {
	BufferPtr result;

	int current = 0;
	char sequence[5] = { 0 };

	// Iterate over all elements
	for (decltype(length) i = 0; i < length; ++i) {
		auto meta = src->Get();
		auto next = src->Peek();

		if (meta == std::char_traits<char>::eof()) {
			throw GeneralException("Unexpected end of file inside stream");
		}

		auto ch = ValueConvertUtils::SafeConvert<unsigned char>(meta);

		// End of sequence
		if (ch == '~' && next == '>') {
			break;
		}

		// Silently ignore white space
		if (IsWhiteSpace(ch)) {
			continue;
		}

		// Special case 'z' char is 4 zero bytes
		if (ch == 'z' && current == 0) {
			result->push_back(0);
			result->push_back(0);
			result->push_back(0);
			result->push_back(0);
			continue;
		}

		// Character outside of base 85 range
		if (ch < '!' || ch > 'u') {
			throw GeneralException("Illegal character in ascii 85 filter: " + ch);
		}

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
		LOG_ERROR_GLOBAL << "A final partial group contains only one character";
	} else if (current == 2) {
		int value = sequence[0] * 85 * 85 * 85 * 85 + sequence[1] * 85 * 85 * 85 + 85 * 85 * 85 + 85 * 85 + 85;
		result->push_back(value >> 24 & 0xFF);
	} else if (current == 3) {
		int value = sequence[0] * 85 * 85 * 85 * 85 + sequence[1] * 85 * 85 * 85 + sequence[2] * 85 * 85 + 85 * 85 + 85;
		result->push_back(value >> 24 & 0xFF);
		result->push_back(value >> 16 & 0xFF);
	} else if (current == 4) {
		int value = sequence[0] * 85 * 85 * 85 * 85 + sequence[1] * 85 * 85 * 85 + sequence[2] * 85 * 85 + sequence[3] * 85 + 85;
		result->push_back(value >> 24 & 0xFF);
		result->push_back(value >> 16 & 0xFF);
		result->push_back(value >> 8 & 0xFF);
	}

	return result;
}

BufferPtr ASCII85DecodeFilter::Encode(BufferPtr src, DictionaryObjectPtr parameters) const {
	auto stream = src->ToInputStream();
	return Encode(stream, src->size());
}

BufferPtr ASCII85DecodeFilter::Decode(BufferPtr src, DictionaryObjectPtr parameters) const {
	auto stream = src->ToInputStream();
	return Decode(stream, src->size());
}

} // syntax
} // vanillapdf
