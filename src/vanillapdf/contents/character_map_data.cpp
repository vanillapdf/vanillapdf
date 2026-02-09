#include "precompiled.h"

#include "contents/character_map_data.h"

// Benchmark results (Release, Windows x64 MSVC 17, 16x 3792 MHz, L3 16384 KiB):
//
// Before (custom bit-by-bit arithmetic):
// --------------------------------------------------------------------------------
// Benchmark                                      Time             CPU   Iterations
// --------------------------------------------------------------------------------
// BM_BaseFontRange_Contains                   71.5 ns         71.4 ns     8960000
// BM_BaseFontRange_GetMappedValue              505 ns          504 ns     1120000
// BM_BaseFontRange_GetMappedValue_4byte        665 ns          670 ns     1120000
//
// After (Buffer::ToInteger/FromInteger with native uint32_t arithmetic):
// --------------------------------------------------------------------------------
// Benchmark                                      Time             CPU   Iterations
// --------------------------------------------------------------------------------
// BM_BaseFontRange_Contains                   49.3 ns         48.8 ns     11200000
// BM_BaseFontRange_GetMappedValue              397 ns          392 ns      1792000
// BM_BaseFontRange_GetMappedValue_4byte        512 ns          516 ns      1000000

namespace vanillapdf {
namespace contents {

using namespace vanillapdf::syntax;

bool BaseFontRange::Contains(BufferPtr key) const {
    auto low_buf = m_low->GetValue();

    // Different byte widths never match a range
    if (key->size() != low_buf->size()) {
        return false;
    }

    auto k = key->ToInteger<uint32_t>(endian::big);
    auto low = low_buf->ToInteger<uint32_t>(endian::big);
    auto high = m_high->GetValue()->ToInteger<uint32_t>(endian::big);

    return k >= low && k <= high;
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
    auto low_buf = m_low->GetValue();
    auto k = key->ToInteger<uint32_t>(endian::big);
    auto low = low_buf->ToInteger<uint32_t>(endian::big);
    auto high = m_high->GetValue()->ToInteger<uint32_t>(endian::big);

    if (key->size() != low_buf->size() || k < low || k > high) {
        LOG_ERROR_AND_THROW_GENERAL("Key: is out of range: [{},{}]", key->ToHexString(), low_buf->ToHexString(), m_high->GetValue()->ToHexString());
    }

    auto offset = k - low;

    if (ObjectUtils::IsType<HexadecimalStringObjectPtr>(m_dest)) {
        auto dest_hex = ObjectUtils::ConvertTo<HexadecimalStringObjectPtr>(m_dest);
        auto dest_value = dest_hex->GetValue();
        auto dest = dest_value->ToInteger<uint32_t>(endian::big);

        return Buffer::FromInteger(dest + offset, dest_value->size(), endian::big);
    }

    if (ObjectUtils::IsType<ArrayObjectPtr<HexadecimalStringObjectPtr>>(m_dest)) {
        auto arr = ObjectUtils::ConvertTo<ArrayObjectPtr<HexadecimalStringObjectPtr>>(m_dest);
        auto result_obj = arr->GetValue(offset);
        return result_obj->GetValue();
    }

    LOG_ERROR_AND_THROW_GENERAL("Unknown destination type: {}", static_cast<int>(m_dest->GetObjectType()));
}

} // contents
} // vanillapdf
