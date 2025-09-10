#include "precompiled.h"
#include "semantics/objects/character_encoding.h"
#include "syntax/utils/name_constants.h"
#include "syntax/objects/dictionary_object.h"
#include "syntax/objects/name_object.h"
#include "syntax/objects/numeric_object.h"

namespace vanillapdf {
namespace semantics {

// Static member initialization
std::unordered_map<std::string, uint32_t> CharacterEncoding::s_glyph_name_to_unicode;
bool CharacterEncoding::s_glyph_mappings_initialized = false;

CharacterEncoding::CharacterEncoding(Type type) 
    : m_type(type), m_encoding_table(256) {
}

uint32_t CharacterEncoding::CodeToUnicode(uint8_t code) const {
    const_cast<CharacterEncoding*>(this)->EnsureInitialized();
    return m_encoding_table[code].unicode_value;
}

std::string CharacterEncoding::CodeToGlyphName(uint8_t code) const {
    const_cast<CharacterEncoding*>(this)->EnsureInitialized();
    return m_encoding_table[code].glyph_name;
}

void CharacterEncoding::EnsureInitialized() {
    if (!m_initialized) {
        InitializeBaseEncoding();
        m_initialized = true;
    }
}

void CharacterEncoding::SetEncodingEntry(uint8_t code, uint32_t unicode_value, const std::string& glyph_name) {
    m_encoding_table[code] = {code, unicode_value, glyph_name};
}

void CharacterEncoding::ApplyDifferences(syntax::ArrayObjectPtr differences) {
    if (!differences) return;
    
    EnsureInitialized();
    
    size_t i = 0;
    while (i < differences->GetSize()) {
        auto obj = differences->At(i);
        
        // First element should be a number (starting code)
        auto number_obj = syntax::ObjectUtils::ConvertTo<syntax::NumberObjectPtr>(obj);
        if (!number_obj) break;
        
        int start_code = static_cast<int>(number_obj->GetValue());
        i++;
        
        // Following elements are glyph names
        int current_code = start_code;
        while (i < differences->GetSize() && current_code <= 255) {
            auto name_obj = syntax::ObjectUtils::ConvertTo<syntax::NameObjectPtr>(differences->At(i));
            if (!name_obj) break;
            
            std::string glyph_name = name_obj->ToString();
            uint32_t unicode_value = GlyphNameToUnicode(glyph_name);
            
            SetEncodingEntry(static_cast<uint8_t>(current_code), unicode_value, glyph_name);
            
            current_code++;
            i++;
        }
    }
}

std::unique_ptr<CharacterEncoding> CharacterEncoding::Create(syntax::ObjectPtr encoding_obj) {
    if (!encoding_obj) {
        return Create(Type::StandardEncoding);
    }
    
    // Handle name object (predefined encoding)
    if (auto name_obj = syntax::ObjectUtils::ConvertTo<syntax::NameObjectPtr>(encoding_obj)) {
        std::string name = name_obj->ToString();
        
        if (name == "StandardEncoding") {
            return Create(Type::StandardEncoding);
        } else if (name == "MacRomanEncoding") {
            return Create(Type::MacRomanEncoding);
        } else if (name == "WinAnsiEncoding") {
            return Create(Type::WinAnsiEncoding);
        } else if (name == "PDFDocEncoding") {
            return Create(Type::PDFDocEncoding);
        } else if (name == "MacExpertEncoding") {
            return Create(Type::MacExpertEncoding);
        } else if (name == "SymbolEncoding") {
            return Create(Type::SymbolEncoding);
        } else if (name == "ZapfDingbatsEncoding") {
            return Create(Type::ZapfDingbatsEncoding);
        }
    }
    
    // Handle dictionary object (custom encoding with differences)
    if (auto dict_obj = syntax::ObjectUtils::ConvertTo<syntax::DictionaryObjectPtr>(encoding_obj)) {
        std::unique_ptr<CharacterEncoding> base_encoding;
        
        // Check for BaseEncoding
        if (dict_obj->Contains(constant::Name::BaseEncoding)) {
            auto base_obj = dict_obj->Find(constant::Name::BaseEncoding);
            base_encoding = Create(base_obj);
        } else {
            base_encoding = Create(Type::StandardEncoding);
        }
        
        auto custom_encoding = std::make_unique<CustomEncoding>(std::move(base_encoding));
        
        // Apply differences if present
        if (dict_obj->Contains(constant::Name::Differences)) {
            auto diff_obj = dict_obj->FindAs<syntax::ArrayObjectPtr>(constant::Name::Differences);
            custom_encoding->ApplyDifferences(diff_obj);
        }
        
        return std::move(custom_encoding);
    }
    
    return Create(Type::StandardEncoding);
}

std::unique_ptr<CharacterEncoding> CharacterEncoding::Create(Type type) {
    switch (type) {
        case Type::StandardEncoding:
            return std::make_unique<StandardEncoding>();
        case Type::MacRomanEncoding:
            return std::make_unique<MacRomanEncoding>();
        case Type::WinAnsiEncoding:
            return std::make_unique<WinAnsiEncoding>();
        case Type::PDFDocEncoding:
            return std::make_unique<PDFDocEncoding>();
        case Type::MacExpertEncoding:
            return std::make_unique<MacExpertEncoding>();
        case Type::SymbolEncoding:
            return std::make_unique<SymbolEncoding>();
        case Type::ZapfDingbatsEncoding:
            return std::make_unique<ZapfDingbatsEncoding>();
        default:
            return std::make_unique<StandardEncoding>();
    }
}

uint32_t CharacterEncoding::GlyphNameToUnicode(const std::string& glyph_name) {
    InitializeGlyphNameMappings();
    
    auto it = s_glyph_name_to_unicode.find(glyph_name);
    if (it != s_glyph_name_to_unicode.end()) {
        return it->second;
    }
    
    return 0; // Not found
}

// Custom encoding implementation
CustomEncoding::CustomEncoding(std::unique_ptr<CharacterEncoding> base_encoding)
    : CharacterEncoding(Type::Custom), m_base_encoding(std::move(base_encoding)) {
}

void CustomEncoding::InitializeBaseEncoding() {
    if (m_base_encoding) {
        // Copy the base encoding table
        for (int i = 0; i < 256; i++) {
            uint32_t unicode = m_base_encoding->CodeToUnicode(static_cast<uint8_t>(i));
            std::string glyph_name = m_base_encoding->CodeToGlyphName(static_cast<uint8_t>(i));
            SetEncodingEntry(static_cast<uint8_t>(i), unicode, glyph_name);
        }
    }
}

} // semantics
} // vanillapdf