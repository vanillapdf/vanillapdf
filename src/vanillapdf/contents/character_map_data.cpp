#include "precompiled.h"

#include "contents/character_map_data.h"

#include "utils/math_utils.h"

namespace vanillapdf {
namespace contents {

using namespace vanillapdf::syntax;

bool BaseFontRange::Contains(BufferPtr key) const {
	bool above_low = ValueEqualLessThan(m_low->GetValue(), key);
	bool below_high = ValueEqualLessThan(key, m_high->GetValue());
	return (above_low && below_high);
}

bool BaseFontRange::ValueEqualLessThan(BufferPtr source, BufferPtr dest) const {
	return source->ValueEqualLessThan(dest);
}

BufferPtr BaseFontRange::GetMappedValue(BufferPtr key) const {
	try {
		return GetMappedValueInternal(key);
	} catch (ExceptionBase& ex) {
		spdlog::error("Could not get mapped value for key \"{}\": {}", key->ToHexString(), ex.what());

		// Continue error processing
		throw;
	}
}

BufferPtr BaseFontRange::GetMappedValueInternal(BufferPtr key) const {
	if (!Contains(key)) {

		auto error_msg = fmt::format("Key: is out of range: [{},{}]", key->ToHexString(), m_low->GetValue()->ToHexString(), m_high->GetValue()->ToHexString());

		spdlog::error(error_msg);
		throw GeneralException(error_msg);
	}

	if (ObjectUtils::IsType<HexadecimalStringObjectPtr>(m_dest)) {
		auto dest_hex = ObjectUtils::ConvertTo<HexadecimalStringObjectPtr>(m_dest);
		auto dest_value = dest_hex->GetValue();
		auto low_value = m_low->GetValue();
		auto diff = Difference(key, low_value);
		return Increment(dest_value, diff);
	}

	if (ObjectUtils::IsType<ArrayObjectPtr<HexadecimalStringObjectPtr>>(m_dest)) {
		auto arr = ObjectUtils::ConvertTo<ArrayObjectPtr<HexadecimalStringObjectPtr>>(m_dest);
		auto low_value = m_low->GetValue();
		auto diff = Difference(key, low_value);
		auto result_obj = arr->GetValue(diff);
		return result_obj->GetValue();
	}

	std::stringstream error_stream;
	error_stream << "Unknown destination type: " << (int)m_dest->GetObjectType();

	throw GeneralException(error_stream.str());
}

uint8_t BaseFontRange::Difference(uint8_t source, uint8_t dest, bool& borrow) const {
	uint8_t result = 0;
	uint8_t bit_position = 0x01;
	for (int i = 0; i < 8; ++i) {
		uint8_t src_bit = (source & bit_position) != 0;
		uint8_t dest_bit = (dest & bit_position) != 0;

		assert(src_bit <= 1);
		assert(dest_bit <= 1);

		if (borrow) {
			if (0 == src_bit) {
				src_bit = 1;
				// keep borrowed
			} else {
				src_bit = 0;
				borrow = false;
			}
		}

		uint8_t result_bit = 0;
		if (src_bit < dest_bit) {
			assert(!borrow);

			result_bit = 1;
			borrow = true;
		} else {
			result_bit = src_bit - dest_bit;
		}

		if (1 == result_bit) {
			result |= bit_position;
		}

		bit_position = bit_position << 1;
	}

	return result;
}

uint32_t BaseFontRange::Difference(BufferPtr source, BufferPtr dest) const {
	uint32_t result = 0;
	bool borrow = false;

	auto src_size = source->size();
	auto dest_size = dest->size();

	auto longer_size = std::max(src_size, dest_size);
	assert(longer_size <= sizeof(result));

	for (decltype(longer_size) i = 0; i < longer_size; ++i) {
		using buffer_type = Buffer::value_type;
		using unsigned_buffer_type = std::make_unsigned<buffer_type>::type;

		unsigned_buffer_type src_value = 0;
		unsigned_buffer_type dest_value = 0;

		if (i < src_size) {
			src_value = source[src_size - i - 1];
		}

		if (i < dest_size) {
			dest_value = dest[dest_size - i - 1];
		}

		uint8_t diff = Difference(src_value, dest_value, borrow);
		result += (diff << (i * 8));
	}

	assert(!borrow && "This function does not support negative result");
	return result;
}

BufferPtr BaseFontRange::Increment(BufferPtr data, uint32_t count) const {
	try {
		return IncrementInternal(data, count);
	} catch (ExceptionBase& ex) {
		spdlog::error("Could not increment \"{}\" by {}: {}", data->ToHexString(), count, ex.what());

		// Continue error processing
		throw;
	}
}

BufferPtr BaseFontRange::IncrementInternal(BufferPtr data, uint32_t count) const {
	BufferPtr result;

	auto data_size = data->size();
	for (decltype(data_size) i = 0; i < data_size; ++i) {
		auto item = data[data_size - i - 1];

		using unsigned_type = std::make_unsigned<decltype(item)>::type;
		auto unsigned_item = reinterpret_cast<unsigned_type&>(item);

		unsigned_type diff = std::numeric_limits<unsigned_type>::max() - unsigned_item;
		uint32_t current_increment = (count >> (i * 8)) & 0xFF;

		unsigned_type new_value;
		if (diff >= current_increment) {
			uint32_t int_value = SafeAddition<uint32_t>(unsigned_item, current_increment);
			new_value = ValueConvertUtils::SafeConvert<unsigned_type>(int_value);
			count -= current_increment << (i * 8);
		} else {
			uint32_t int_value = (unsigned_item + current_increment) % 256;
			new_value = ValueConvertUtils::SafeConvert<unsigned_type>(int_value);

			// This should work as borrow
			count -= current_increment << (i * 8);
			count += (1 << ((i + 1) * 8));
		}

		result->insert(result->begin(), new_value);
	}

	// Check if there was overflow all the way
	// Example: 0xFF + 1 would be 0 instead of 0x100
	if (count > 0) {

		// There could be multiple bytes to insert
		for (uint32_t i = 0; i < sizeof(count); ++i) {
			uint32_t current_value = count % ((i + 1) * 256);
			uint8_t current_char_value = ValueConvertUtils::SafeConvert<uint8_t>(current_value);

			if (current_char_value != 0) {
				result->insert(result->begin(), current_char_value);
			}
		}
	}

	return result;
}

} // syntax
} // vanillapdf
