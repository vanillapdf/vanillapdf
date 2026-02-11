#include "precompiled.h"

#include "syntax/files/file.h"
#include "contents/character_map_parser.h"

#include "semantics/objects/character_map.h"

// Benchmark results (Release, Windows x64 MSVC 17, 16x 3792 MHz, L3 16384 KiB):
//
// Before (copy-by-value loops, ValueEqualLessThan codespace check, Contains+GetMappedValue double decode):
// --------------------------------------------------------------------------------
// Benchmark                                      Time             CPU   Iterations
// --------------------------------------------------------------------------------
// BM_UnicodeCharacterMap_GetMappedValue        546 ns          544 ns      1120000
//
// After (const auto& loops, ToInteger codespace check, TryGetMappedValue single pass):
// --------------------------------------------------------------------------------
// Benchmark                                      Time             CPU   Iterations
// --------------------------------------------------------------------------------
// BM_UnicodeCharacterMap_GetMappedValue        440 ns          439 ns      1600000

namespace vanillapdf {
namespace semantics {

CharacterMapBase::CharacterMapBase(syntax::StreamObjectPtr root)
    : HighLevelObject(root) {
}

EmbeddedCharacterMap::EmbeddedCharacterMap(syntax::StreamObjectPtr root)
    : CharacterMapBase(root) {
}

UnicodeCharacterMap::UnicodeCharacterMap(syntax::StreamObjectPtr root)
    : CharacterMapBase(root) {
    m_access_lock = std::unique_ptr<std::recursive_mutex>(pdf_new std::recursive_mutex());
}

std::unique_ptr<CharacterMapBase> CharacterMapBase::Create(syntax::StreamObjectPtr root, WeakReference<Document> doc) {
    // TODO
    return make_unique<UnicodeCharacterMap>(root);
}

void UnicodeCharacterMap::Initialize() const {
    if (m_initialized.load(std::memory_order_acquire)) {
        return;
    }

    ACCESS_LOCK_GUARD(m_access_lock);

    // Double-check after acquiring the lock
    if (m_initialized.load(std::memory_order_relaxed)) {
        return;
    }

    auto body = _obj->GetBody();
    auto input_stream = body->ToInputStream();

    contents::CharacterMapParser parser(_obj->GetFile(), input_stream);
    m_data = parser.ReadCharacterMapData();
    m_initialized.store(true, std::memory_order_release);
}

BufferPtr UnicodeCharacterMap::GetMappedValue(BufferPtr key) const {
    Initialize();

    auto k = key->ToInteger<uint32_t>(endian::big);

    bool in_codespace = false;
    for (const auto& range : m_data.CodeSpaceRanges) {
        auto begin = range.Begin->GetValue()->ToInteger<uint32_t>(endian::big);
        auto end = range.End->GetValue()->ToInteger<uint32_t>(endian::big);

        if (k >= begin && k <= end) {
            in_codespace = true;
            break;
        }
    }

    if (!in_codespace) {

        auto error_message = fmt::format("The key {} was not found in the UnicodeCharacterMap codespace range", key->ToHexString());
        spdlog::error(error_message);

        auto codespace_range_size = m_data.CodeSpaceRanges.size();
        for (decltype(codespace_range_size) i = 0; i < codespace_range_size; ++i) {
            spdlog::error("Codespace ranges [{}] {}:{}", i, m_data.CodeSpaceRanges[i].Begin->ToString(), m_data.CodeSpaceRanges[i].End->ToString());
        }

        throw GeneralException(error_message);
    }

    // Check base font ranges
    for (const auto& range : m_data.BaseFontRanges) {
        BufferPtr result;
        if (range.TryGetMappedValue(key, result)) {
            return result;
        }
    }

    // Check base font chars
    for (const auto& char_mapping : m_data.BaseFontCharMapping) {
        if (char_mapping.Source->GetValue() == key) {
            return char_mapping.Destination->GetValue();
        }
    }

    auto error_message = fmt::format("The key {} was not found in the UnicodeCharacterMap", key->ToHexString());
    spdlog::error(error_message);

    spdlog::error("CodeSpaceRanges: {}", m_data.CodeSpaceRanges.size());

    auto codespace_range_size = m_data.CodeSpaceRanges.size();
    for (decltype(codespace_range_size) i = 0; i < codespace_range_size; ++i) {
        spdlog::error("Codespace range [{}] {}:{}", i, m_data.CodeSpaceRanges[i].Begin->ToString(), m_data.CodeSpaceRanges[i].End->ToString());
    }

    spdlog::error("BaseFontRanges: {}", m_data.BaseFontRanges.size());

    auto font_range_size = m_data.BaseFontRanges.size();
    for (decltype(font_range_size) i = 0; i < font_range_size; ++i) {
        spdlog::error("Font range [{}] {}:{}", i, m_data.BaseFontRanges[i].GetRangeLow()->ToString(), m_data.BaseFontRanges[i].GetRangeHigh()->ToString());
    }

    spdlog::error("BaseFontCharMapping: {}", m_data.BaseFontCharMapping.size());

    auto font_char_size = m_data.BaseFontCharMapping.size();
    for (decltype(font_char_size) i = 0; i < font_char_size; ++i) {
        spdlog::error("Font char mapping [{}] {}:{}", i, m_data.BaseFontCharMapping[i].Source->ToString(), m_data.BaseFontCharMapping[i].Destination->ToString());
    }

    throw GeneralException(error_message);
}

} // semantics
} // vanillapdf
