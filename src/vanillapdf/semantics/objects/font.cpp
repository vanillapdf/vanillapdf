#include "precompiled.h"
#include "semantics/objects/font.h"

#include "syntax/utils/name_constants.h"
#include <codecvt>
#include <locale>

namespace vanillapdf {
namespace semantics {

FontMap::FontMap(syntax::DictionaryObjectPtr obj) : HighLevelObject(obj) {}
FontBase::FontBase(syntax::DictionaryObjectPtr root) : HighLevelObject(root) {}
Type0Font::Type0Font(syntax::DictionaryObjectPtr root) : FontBase(root) {}
Type1Font::Type1Font(syntax::DictionaryObjectPtr root) : FontBase(root) {}
MMType1Font::MMType1Font(syntax::DictionaryObjectPtr root) : FontBase(root) {}
Type3Font::Type3Font(syntax::DictionaryObjectPtr root) : FontBase(root) {}
TrueTypeFont::TrueTypeFont(syntax::DictionaryObjectPtr root) : FontBase(root) {}
CIDFontType0Font::CIDFontType0Font(syntax::DictionaryObjectPtr root) : FontBase(root) {}
CIDFontType2Font::CIDFontType2Font(syntax::DictionaryObjectPtr root) : FontBase(root) {}

FontBase::Type Type0Font::GetFontType() const noexcept { return Type::Type0; }
FontBase::Type Type1Font::GetFontType() const noexcept { return Type::Type1; }
FontBase::Type MMType1Font::GetFontType() const noexcept { return Type::MMType1; }
FontBase::Type Type3Font::GetFontType() const noexcept { return Type::Type3; }
FontBase::Type TrueTypeFont::GetFontType() const noexcept { return Type::TrueType; }
FontBase::Type CIDFontType0Font::GetFontType() const noexcept { return Type::CIDFontType0; }
FontBase::Type CIDFontType2Font::GetFontType() const noexcept { return Type::CIDFontType2; }

bool FontMap::Contains(const syntax::NameObject& name) const {
    return _obj->Contains(name);
}

FontPtr FontMap::Find(const syntax::NameObject& name) const {
    auto dict = _obj->FindAs<syntax::DictionaryObjectPtr>(name);
    return FontBase::Create(dict);
}

FontBase* FontBase::Create(syntax::DictionaryObjectPtr root) {
    // Verify type, if it is included
    if (root->Contains(constant::Name::Type)) {
        syntax::ObjectPtr type_obj = root->Find(constant::Name::Type);
        if (!syntax::ObjectUtils::IsType<syntax::NameObjectPtr>(type_obj)) {
            throw GeneralException("Invalid font type object");
        }

        syntax::NameObjectPtr font_type = syntax::ObjectUtils::ConvertTo<syntax::NameObjectPtr>(type_obj);

        if (font_type != constant::Name::Font) {
            throw GeneralException("Invalid font type: " + font_type->ToString());
        }
    }

    if (!root->Contains(constant::Name::Subtype)) {
        throw GeneralException("Dictionary does not contain subtype");
    }

    syntax::ObjectPtr subtype_obj = root->Find(constant::Name::Subtype);

    if (!syntax::ObjectUtils::IsType<syntax::NameObjectPtr>(subtype_obj)) {
        throw GeneralException("Invalid font subtype object");
    }

    syntax::NameObjectPtr subtype = syntax::ObjectUtils::ConvertTo<syntax::NameObjectPtr>(subtype_obj);

    if (subtype == constant::Name::Type0) {
        auto result = make_unique<Type0Font>(root);
        return result.release();
    }

    if (subtype == constant::Name::Type1) {
        auto result = make_unique<Type1Font>(root);
        return result.release();
    }

    if (subtype == constant::Name::MMType1) {
        auto result = make_unique<MMType1Font>(root);
        return result.release();
    }

    if (subtype == constant::Name::Type3) {
        auto result = make_unique<Type3Font>(root);
        return result.release();
    }

    if (subtype == constant::Name::TrueType) {
        auto result = make_unique<TrueTypeFont>(root);
        return result.release();
    }

    if (subtype == constant::Name::CIDFontType0) {
        auto result = make_unique<CIDFontType0Font>(root);
        return result.release();
    }

    if (subtype == constant::Name::CIDFontType2) {
        auto result = make_unique<CIDFontType2Font>(root);
        return result.release();
    }

    throw GeneralException("Unknown font subtype: " + subtype->ToString());
}

bool FontBase::ToUnicode(OuputUnicodeCharacterMapPtr& result) const {
    if (!m_character_map.empty()) {
        result = m_character_map;
        return true;
    }

    if (!_obj->Contains(constant::Name::ToUnicode)) {
        return false;
    }

    auto stream = _obj->FindAs<syntax::StreamObjectPtr>(constant::Name::ToUnicode);
    m_character_map = make_deferred<UnicodeCharacterMap>(stream);
    result = m_character_map;
    return true;
}

std::unique_ptr<CharacterEncoding> FontBase::GetEncoding() const {
    if (m_encoding) {
        // Return a copy of the cached encoding
        return CharacterEncoding::Create(m_encoding->GetEncodingType());
    }

    // Check if the font has an explicit encoding
    if (_obj->Contains(constant::Name::Encoding)) {
        auto encoding_obj = _obj->Find(constant::Name::Encoding);
        m_encoding = CharacterEncoding::Create(encoding_obj);
    } else {
        // Use the default encoding for this font type
        m_encoding = CharacterEncoding::Create(GetDefaultEncoding());
    }

    // Return a copy
    return CharacterEncoding::Create(m_encoding->GetEncodingType());
}

std::string FontBase::DecodeText(const uint8_t* codes, size_t count) const {
    if (!codes || count == 0) {
        return std::string();
    }

    auto encoding = GetEncoding();
    std::string result;
    result.reserve(count * 4); // Reserve space for UTF-8 encoding

    for (size_t i = 0; i < count; i++) {
        uint32_t unicode = encoding->CodeToUnicode(codes[i]);
        if (unicode == 0) {
            continue; // Skip unmapped characters
        }

        // Convert Unicode code point to UTF-8
        if (unicode <= 0x7F) {
            result.push_back(static_cast<char>(unicode));
        } else if (unicode <= 0x7FF) {
            result.push_back(static_cast<char>(0xC0 | (unicode >> 6)));
            result.push_back(static_cast<char>(0x80 | (unicode & 0x3F)));
        } else if (unicode <= 0xFFFF) {
            result.push_back(static_cast<char>(0xE0 | (unicode >> 12)));
            result.push_back(static_cast<char>(0x80 | ((unicode >> 6) & 0x3F)));
            result.push_back(static_cast<char>(0x80 | (unicode & 0x3F)));
        } else if (unicode <= 0x10FFFF) {
            result.push_back(static_cast<char>(0xF0 | (unicode >> 18)));
            result.push_back(static_cast<char>(0x80 | ((unicode >> 12) & 0x3F)));
            result.push_back(static_cast<char>(0x80 | ((unicode >> 6) & 0x3F)));
            result.push_back(static_cast<char>(0x80 | (unicode & 0x3F)));
        }
    }

    return result;
}

uint32_t FontBase::DecodeCharacter(uint8_t code) const {
    auto encoding = GetEncoding();
    return encoding->CodeToUnicode(code);
}

CharacterEncoding::Type FontBase::GetDefaultEncoding() const {
    return CharacterEncoding::Type::StandardEncoding;
}

// Font-specific default encodings
CharacterEncoding::Type Type1Font::GetDefaultEncoding() const {
    return CharacterEncoding::Type::StandardEncoding;
}

CharacterEncoding::Type MMType1Font::GetDefaultEncoding() const {
    return CharacterEncoding::Type::StandardEncoding;
}

CharacterEncoding::Type TrueTypeFont::GetDefaultEncoding() const {
    return CharacterEncoding::Type::WinAnsiEncoding;
}

} // semantics
} // vanillapdf
