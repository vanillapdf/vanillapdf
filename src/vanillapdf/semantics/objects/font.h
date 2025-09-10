#ifndef _FONT_H
#define _FONT_H

#include "semantics/utils/semantics_fwd.h"
#include "semantics/objects/high_level_object.h"
#include "semantics/objects/character_map.h"
#include "semantics/objects/character_encoding.h"

#include "syntax/objects/dictionary_object.h"

namespace vanillapdf {
namespace semantics {

class FontMap : public HighLevelObject<syntax::DictionaryObjectPtr> {
public:
    explicit FontMap(syntax::DictionaryObjectPtr obj);
    bool Contains(const syntax::NameObject& name) const;
    FontPtr Find(const syntax::NameObject& name) const;
};

class FontBase : public HighLevelObject<syntax::DictionaryObjectPtr> {
public:
    enum class Type {
        Undefined = 0,
        Type0,
        Type1,
        MMType1,
        Type3,
        TrueType,
        CIDFontType0,
        CIDFontType2
    };

public:
    explicit FontBase(syntax::DictionaryObjectPtr root);
    static FontBase* Create(syntax::DictionaryObjectPtr root);

    virtual Type GetFontType() const noexcept = 0;

    bool ToUnicode(OuputUnicodeCharacterMapPtr& result) const;
    
    /**
     * \brief Get the character encoding for this font
     */
    std::unique_ptr<CharacterEncoding> GetEncoding() const;
    
    /**
     * \brief Decode character codes to Unicode text
     * \param codes Array of character codes
     * \param count Number of character codes
     * \return Unicode text string
     */
    std::string DecodeText(const uint8_t* codes, size_t count) const;
    
    /**
     * \brief Decode a single character code to Unicode
     * \param code Character code
     * \return Unicode code point
     */
    uint32_t DecodeCharacter(uint8_t code) const;

protected:
    /**
     * \brief Get the base encoding type for this font type
     * Default implementation returns StandardEncoding
     */
    virtual CharacterEncoding::Type GetDefaultEncoding() const;

private:
    mutable OuputUnicodeCharacterMapPtr m_character_map;
    mutable std::unique_ptr<CharacterEncoding> m_encoding;
};

class Type0Font : public FontBase {
public:
    explicit Type0Font(syntax::DictionaryObjectPtr root);

    virtual Type GetFontType() const noexcept override;

    //syntax::NameObjectPtr BaseFont() const;
    //syntax::ObjectPtr Encoding() const;
    //syntax::ArrayObjectPtr<syntax::DictionaryObjectPtr> DescendantFonts() const;
};

class Type1Font : public FontBase {
public:
    explicit Type1Font(syntax::DictionaryObjectPtr root);
    virtual Type GetFontType() const noexcept override;

protected:
    virtual CharacterEncoding::Type GetDefaultEncoding() const override;
};

class MMType1Font : public FontBase {
public:
    explicit MMType1Font(syntax::DictionaryObjectPtr root);
    virtual Type GetFontType() const noexcept override;

protected:
    virtual CharacterEncoding::Type GetDefaultEncoding() const override;
};

class Type3Font : public FontBase {
public:
    explicit Type3Font(syntax::DictionaryObjectPtr root);
    virtual Type GetFontType() const noexcept override;
};

class TrueTypeFont : public FontBase {
public:
    explicit TrueTypeFont(syntax::DictionaryObjectPtr root);
    virtual Type GetFontType() const noexcept override;

protected:
    virtual CharacterEncoding::Type GetDefaultEncoding() const override;
};

class CIDFontType0Font : public FontBase {
public:
    explicit CIDFontType0Font(syntax::DictionaryObjectPtr root);
    virtual Type GetFontType() const noexcept override;
};

class CIDFontType2Font : public FontBase {
public:
    explicit CIDFontType2Font(syntax::DictionaryObjectPtr root);
    virtual Type GetFontType() const noexcept override;
};

} // semantics
} // vanillapdf

#endif /* _FONT_H */
