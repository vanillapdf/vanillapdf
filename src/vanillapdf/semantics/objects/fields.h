#ifndef _FIELDS_H
#define _FIELDS_H

#include "semantics/utils/semantics_fwd.h"
#include "semantics/objects/high_level_object.h"
#include "semantics/objects/digital_signature.h"

namespace vanillapdf {
namespace semantics {

class FieldCollection : public HighLevelObject<syntax::ArrayObjectPtr<syntax::DictionaryObjectPtr>> {
public:
    explicit FieldCollection(syntax::ArrayObjectPtr<syntax::DictionaryObjectPtr> root) : HighLevelObject(root) {}

    // Creates an empty field collection. The array is attached as a direct
    // object through InteractiveForm::SetFields.
    static FieldCollectionPtr Create();

    types::size_type GetSize() const;
    FieldPtr At(types::size_type index) const;
};

class Field : public HighLevelObject<syntax::DictionaryObjectPtr> {
public:
    enum Type {
        Undefined = 0,
        NonTerminal,
        Button,
        Text,
        Choice,
        Signature
    };

    // Form of quadding (justification) of variable text fields (Table 222)
    enum class Quadding {
        LeftJustified = 0,
        Centered = 1,
        RightJustified = 2
    };

public:
    explicit Field(syntax::DictionaryObjectPtr root) : HighLevelObject(root) {}
    static FieldPtr Create(syntax::DictionaryObjectPtr root);

    virtual Field::Type GetFieldType() const noexcept = 0;

    // A terminal field has no children other than widget annotations (12.7.3.2).
    // Child fields are recognized by their /T partial name, which widget
    // annotations do not carry.
    static bool IsTerminalDictionary(const syntax::DictionaryObjectPtr& dictionary);
    bool IsTerminal() const;

    // Fully qualified field name: the /T partial names joined with '.' from
    // the root of the hierarchy down to this field (12.7.3.2). Levels without
    // a /T entry do not contribute a segment. Each /T is a text string
    // (7.9.2.2) normalized to UTF-8, so the result is a UTF-8 buffer
    // regardless of how the partial names are encoded in the document.
    BufferPtr GetQualifiedName() const;

    bool GetName(syntax::OutputStringObjectPtr& result) const;

    // Sets the partial field name (/T). Because the PERIOD separates the
    // segments of fully qualified names, a partial name shall not contain
    // one (12.7.3.2) - such a value is an InvalidParameterException
    void SetName(syntax::StringObjectPtr value);

    bool GetAlternateName(syntax::OutputStringObjectPtr& result) const;
    void SetAlternateName(syntax::StringObjectPtr value);

    bool GetFieldFlags(types::big_int& result) const;
    void SetFieldFlags(types::big_int value);

    // Default appearance string (/DA) and quadding (/Q) for variable text
    // fields, resolved through the /Parent chain. The document-wide default
    // lives in the AcroForm dictionary (12.7.3.3) and is exposed by
    // InteractiveForm::GetDefaultAppearance / GetQuadding - when the entry is
    // missing here, the caller applies that fallback. The setters write this
    // field's own dictionary, overriding any inherited value.
    bool GetDefaultAppearance(syntax::OutputStringObjectPtr& result) const;
    void SetDefaultAppearance(syntax::StringObjectPtr value);
    bool GetQuadding(Quadding& result) const;
    void SetQuadding(Quadding value);

    // Maps a raw /Q value to the enumerated form; an unknown value read from
    // the document is a ParseException
    static Quadding ConvertQuadding(const syntax::IntegerObjectPtr& value);

    // Maps the enumerated form back to its raw /Q value; an unknown
    // enumeration value is a ConversionException
    static types::big_int ConvertQuadding(Quadding value);

protected:
    // Resolves an inheritable field attribute - /FT, /Ff, /V and /DV per Table 220,
    // /DA and /Q per 12.7.3.3 - by searching the field dictionary and then walking
    // the /Parent chain. Returns false when the key is present in none of them.
    // Each /Parent shall be an indirect reference (Table 220); a chain hop
    // that is not one ends the search. Visited parent references are tracked
    // by object identity, so a cyclic chain terminates instead of looping.
    static bool FindInheritedEntry(
        const syntax::DictionaryObjectPtr& dictionary,
        const syntax::NameObject& key,
        syntax::OutputObjectPtr& result);

    bool GetInheritedEntry(const syntax::NameObject& key, syntax::OutputObjectPtr& result) const;
};

class NonTerminalField : public Field {
public:
    explicit NonTerminalField(syntax::DictionaryObjectPtr root) : Field(root) {}
    virtual Field::Type GetFieldType() const noexcept override { return Field::Type::NonTerminal; }
};

class ButtonField : public Field {
public:
    explicit ButtonField(syntax::DictionaryObjectPtr root) : Field(root) {}
    virtual Field::Type GetFieldType() const noexcept override { return Field::Type::Button; }

    bool GetValue(syntax::OutputNameObjectPtr& result) const;
    void SetValue(syntax::NameObjectPtr value);
};

class TextField : public Field {
public:
    explicit TextField(syntax::DictionaryObjectPtr root) : Field(root) {}
    virtual Field::Type GetFieldType() const noexcept override { return Field::Type::Text; }

    bool GetValue(syntax::OutputStringObjectPtr& result) const;
    void SetValue(syntax::StringObjectPtr value);
    bool GetDefaultValue(syntax::OutputStringObjectPtr& result) const;
    bool GetMaxLength(syntax::OutputIntegerObjectPtr& result) const;
};

class ChoiceField : public Field {
public:
    explicit ChoiceField(syntax::DictionaryObjectPtr root) : Field(root) {}
    virtual Field::Type GetFieldType() const noexcept override { return Field::Type::Choice; }

    bool GetValue(syntax::OutputStringObjectPtr& result) const;
    void SetValue(syntax::StringObjectPtr value);
    bool GetOptionCount(types::size_type& result) const;
    bool GetOptionAt(types::size_type index, syntax::OutputContainableObjectPtr& result) const;
};

class SignatureField : public Field {
public:
    explicit SignatureField(syntax::DictionaryObjectPtr root) : Field(root) {}
    virtual Field::Type GetFieldType() const noexcept override { return Field::Type::Signature; }

    bool Value(OuputDigitalSignaturePtr& result) const;
};

} // semantics
} // vanillapdf

#endif /* _FIELDS_H */