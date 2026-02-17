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

public:
    explicit Field(syntax::DictionaryObjectPtr root) : HighLevelObject(root) {}
    static FieldPtr Create(syntax::DictionaryObjectPtr root);

    virtual Field::Type GetFieldType() const noexcept = 0;

    bool GetName(syntax::OutputStringObjectPtr& result) const;
    bool GetAlternateName(syntax::OutputStringObjectPtr& result) const;
    bool GetFieldFlags(types::big_int& result) const;
    void SetFieldFlags(types::big_int value);

    // text string overloads (encoding-aware)
    bool GetNameText(PdfTextStringPtr& result) const;
    bool GetAlternateNameText(PdfTextStringPtr& result) const;
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

    // text string overloads (encoding-aware)
    bool GetValueText(PdfTextStringPtr& result) const;
    void SetValueText(PdfTextStringPtr text);
    bool GetDefaultValueText(PdfTextStringPtr& result) const;
};

class ChoiceField : public Field {
public:
    explicit ChoiceField(syntax::DictionaryObjectPtr root) : Field(root) {}
    virtual Field::Type GetFieldType() const noexcept override { return Field::Type::Choice; }

    bool GetValue(syntax::OutputStringObjectPtr& result) const;
    void SetValue(syntax::StringObjectPtr value);
    bool GetOptionCount(types::size_type& result) const;
    bool GetOptionAt(types::size_type index, syntax::OutputContainableObjectPtr& result) const;

    // text string overloads (encoding-aware)
    bool GetValueText(PdfTextStringPtr& result) const;
    void SetValueText(PdfTextStringPtr text);
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