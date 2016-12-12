#include "precompiled.h"
#include "character_map_data.h"

namespace gotchangpdf
{
	namespace syntax
	{
		bool BaseFontRange::Contains(BufferPtr key) const
		{
			bool above_low = ValueEqualLessThan(m_low->GetValue(), key);
			bool below_high = ValueEqualLessThan(key, m_high->GetValue());
			return (above_low && below_high);
		}

		bool BaseFontRange::ValueEqualLessThan(BufferPtr source, BufferPtr dest) const
		{
			auto src_size = source->size();
			auto dest_size = dest->size();

			if (src_size != dest_size) {
				return (src_size < dest_size);
			}

			for (decltype(src_size) i = 0; i < src_size; ++i) {
				auto src_byte = source[i];
				auto dest_byte = dest[i];

				if (src_byte != dest_byte) {
					return (src_byte < dest_byte);
				}
			}

			return true;
		}

		BufferPtr BaseFontRange::GetMappedValue(BufferPtr key) const
		{
			if (!Contains(key)) {
				throw GeneralException("Out of range");
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
				auto result_obj = arr->At(diff);
				return result_obj->GetValue();
			}

			throw GeneralException("Unknown destination type");
		}

		uint8_t BaseFontRange::Difference(uint8_t source, uint8_t dest, bool& borrow) const
		{
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
					}
					else {
						src_bit = 0;
						borrow = false;
					}
				}

				uint8_t result_bit = 0;
				if (src_bit < dest_bit) {
					assert(!borrow);

					result_bit = 1;
					borrow = true;
				}
				else {
					result_bit = src_bit - dest_bit;
				}

				if (1 == result_bit) {
					result |= bit_position;
				}

				bit_position = bit_position << 1;
			}

			return result;
		}

		types::integer BaseFontRange::Difference(BufferPtr source, BufferPtr dest) const
		{
			types::integer result = 0;
			bool borrow = false;

			auto src_size = source->size();
			auto dest_size = dest->size();

			auto longer_size = std::max(src_size, dest_size);
			assert(longer_size <= sizeof(types::integer));

			for (decltype(longer_size) i = 0; i < longer_size; --i) {
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
				result = (result << 8) + diff;
			}

			assert(!borrow && "This function does not support negative result");
			return result;
		}

		BufferPtr BaseFontRange::Increment(BufferPtr data, types::native_uint count) const
		{
			BufferPtr result;

			auto size = data->size();
			types::native_uint increment = count;
			for (decltype(size) i = size - 1; i >= 0; --i) {
				auto item = data[i];

				using unsigned_type = std::make_unsigned<decltype(item)>::type;
				auto unsigned_item = reinterpret_cast<unsigned_type&>(item);

				unsigned_type diff = std::numeric_limits<unsigned_type>::max() - unsigned_item;

				unsigned_type new_value;
				if (diff > increment) {
					types::native_uint int_value = SafeAddition<types::native_uint>(unsigned_item, increment);
					new_value = ValueConvertUtils::SafeConvert<unsigned_type>(int_value);
					increment = 0;
				}
				else {
					types::native_uint int_value = increment - diff;
					new_value = ValueConvertUtils::SafeConvert<unsigned_type>(int_value);
					increment = 1;
				}

				result->insert(result->begin(), new_value);
			}

			// Check if there was overflow all the way
			// Example: 0xFF + 1 would be 0 instead of 0x100
			if (1 == increment) {
				result->insert(result->begin(), 1);
			}

			return result;
		}
	}
}
