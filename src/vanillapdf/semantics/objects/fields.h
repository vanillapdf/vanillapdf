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