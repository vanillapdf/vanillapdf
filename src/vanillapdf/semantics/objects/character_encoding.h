#ifndef _CHARACTER_ENCODING_H
#define _CHARACTER_ENCODING_H

#include "semantics/utils/semantics_fwd.h"
#include "syntax/objects/object.h"
#include "syntax/objects/array_object.h"
#include "syntax/objects/name_object.h"
#include "utils/unknown_interface.h"

#include <unordered_map>
#include <vector>
#include <string>
#include <cstdint>

namespace vanillapdf {
namespace semantics {

/**
 * \brief Base class for all PDF character encodings
 * 
 * This class provides the foundation for handling various PDF character encodings
 * including StandardEncoding, MacRomanEncoding, WinAnsiEncoding, PDFDocEncoding, and MacExpertEncoding.
 */
class CharacterEncoding : public IUnknown {
public:
    enum class Type {
        Undefined = 0,
        StandardEncoding,
        MacRomanEncoding, 
        WinAnsiEncoding,
        PDFDocEncoding,
        MacExpertEncoding,
        SymbolEncoding,
        ZapfDingbatsEncoding,
        Custom
    };

    struct EncodingEntry {
        uint16_t code_point;     // Character code (0-255)
        uint32_t unicode_value;  // Unicode code point
        std::string glyph_name;  // PostScript glyph name
    };

public:
    explicit CharacterEncoding(Type type);
    virtual ~CharacterEncoding() = default;

    /**
     * \brief Get the encoding type
     */
    Type GetEncodingType() const noexcept { return m_type; }

    /**
     * \brief Convert character code to Unicode
     * \param code Character code (0-255)
     * \return Unicode code point, or 0 if not found
     */
    virtual uint32_t CodeToUnicode(uint8_t code) const;

    /**
     * \brief Convert character code to glyph name
     * \param code Character code (0-255)
     * \return PostScript glyph name, or empty string if not found
     */
    virtual std::string CodeToGlyphName(uint8_t code) const;

    /**
     * \brief Apply differences array to modify the encoding
     * \param differences Array object containing encoding differences
     */
    virtual void ApplyDifferences(syntax::ArrayObjectPtr differences);

    /**
     * \brief Create encoding from PDF object
     * \param encoding_obj PDF encoding object (name or dictionary)
     * \return Unique pointer to CharacterEncoding instance
     */
    static std::unique_ptr<CharacterEncoding> Create(syntax::ObjectPtr encoding_obj);

    /**
     * \brief Create encoding by type
     * \param type Encoding type
     * \return Unique pointer to CharacterEncoding instance
     */
    static std::unique_ptr<CharacterEncoding> Create(Type type);

protected:
    /**
     * \brief Initialize the base encoding table
     */
    virtual void InitializeBaseEncoding() = 0;

    /**
     * \brief Set encoding entry
     * \param code Character code
     * \param unicode_value Unicode code point
     * \param glyph_name PostScript glyph name
     */
    void SetEncodingEntry(uint8_t code, uint32_t unicode_value, const std::string& glyph_name);

    /**
     * \brief Get glyph name to Unicode mapping
     * \param glyph_name PostScript glyph name
     * \return Unicode code point, or 0 if not found
     */
    static uint32_t GlyphNameToUnicode(const std::string& glyph_name);

private:
    Type m_type;
    std::vector<EncodingEntry> m_encoding_table;  // Index is character code
    std::unordered_map<std::string, uint32_t> m_glyph_to_unicode;
    bool m_initialized = false;

    void EnsureInitialized();
    static void InitializeGlyphNameMappings();
    static std::unordered_map<std::string, uint32_t> s_glyph_name_to_unicode;
    static bool s_glyph_mappings_initialized;
};

/**
 * \brief Standard PDF encoding (Adobe Standard Encoding)
 */
class StandardEncoding : public CharacterEncoding {
public:
    StandardEncoding() : CharacterEncoding(Type::StandardEncoding) {}

protected:
    void InitializeBaseEncoding() override;
};

/**
 * \brief Mac Roman encoding
 */
class MacRomanEncoding : public CharacterEncoding {
public:
    MacRomanEncoding() : CharacterEncoding(Type::MacRomanEncoding) {}

protected:
    void InitializeBaseEncoding() override;
};

/**
 * \brief Windows ANSI encoding (Code Page 1252)
 */
class WinAnsiEncoding : public CharacterEncoding {
public:
    WinAnsiEncoding() : CharacterEncoding(Type::WinAnsiEncoding) {}

protected:
    void InitializeBaseEncoding() override;
};

/**
 * \brief PDF document encoding
 */
class PDFDocEncoding : public CharacterEncoding {
public:
    PDFDocEncoding() : CharacterEncoding(Type::PDFDocEncoding) {}

protected:
    void InitializeBaseEncoding() override;
};

/**
 * \brief Mac Expert encoding
 */
class MacExpertEncoding : public CharacterEncoding {
public:
    MacExpertEncoding() : CharacterEncoding(Type::MacExpertEncoding) {}

protected:
    void InitializeBaseEncoding() override;
};

/**
 * \brief Symbol encoding
 */
class SymbolEncoding : public CharacterEncoding {
public:
    SymbolEncoding() : CharacterEncoding(Type::SymbolEncoding) {}

protected:
    void InitializeBaseEncoding() override;
};

/**
 * \brief ZapfDingbats encoding
 */
class ZapfDingbatsEncoding : public CharacterEncoding {
public:
    ZapfDingbatsEncoding() : CharacterEncoding(Type::ZapfDingbatsEncoding) {}

protected:
    void InitializeBaseEncoding() override;
};

/**
 * \brief Custom encoding with differences
 */
class CustomEncoding : public CharacterEncoding {
public:
    explicit CustomEncoding(std::unique_ptr<CharacterEncoding> base_encoding);

protected:
    void InitializeBaseEncoding() override;

private:
    std::unique_ptr<CharacterEncoding> m_base_encoding;
};

} // semantics
} // vanillapdf

#endif /* _CHARACTER_ENCODING_H */