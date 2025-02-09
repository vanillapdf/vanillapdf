#include "precompiled.h"

#include "syntax/files/file.h"
#include "contents/character_map_parser.h"

#include "semantics/objects/character_map.h"

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
}

std::unique_ptr<CharacterMapBase> CharacterMapBase::Create(syntax::StreamObjectPtr root, WeakReference<Document> doc) {
    // TODO
    return make_unique<UnicodeCharacterMap>(root);
}

void UnicodeCharacterMap::Initialize() const {
    if (m_initialized) {
        return;
    }

    auto body = _obj->GetBody();
    auto input_stream = body->ToInputStream();

    contents::CharacterMapParser parser(_obj->GetFile(), input_stream);
    m_data = parser.ReadCharacterMapData();
    m_initialized = true;
}

BufferPtr UnicodeCharacterMap::GetMappedValue(BufferPtr key) const {
    Initialize();

    bool in_codespace = false;
    for (auto range : m_data.CodeSpaceRanges) {
        auto begin_range = range.Begin->GetValue();
        auto end_range = range.End->GetValue();

        if (key->ValueEqualLessThan(end_range) && begin_range->ValueEqualLessThan(key)) {
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
    for (auto range : m_data.BaseFontRanges) {
        if (range.Contains(key)) {
            return range.GetMappedValue(key);
        }
    }

    // Check base font chars
    for (auto char_mapping : m_data.BaseFontCharMapping) {
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
