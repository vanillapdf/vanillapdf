#ifndef _FIELDS_H
#define _FIELDS_H

#include "semantics/utils/semantics_fwd.h"
#include "semantics/objects/high_level_object.h"
#include "semantics/objects/digital_signature.h"

namespace vanillapdf {
namespace semantics {

class FieldCollection : public HighLevelObject<syntax::ArrayObjectPtr<syntax::DictionaryObjectPtr>> {
public:
    explicit FieldCollection(syntax::ArrayObjectPtr<syntax::DictionaryObjectPtr> root);
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
    explicit Field(syntax::DictionaryObjectPtr root);
    static std::unique_ptr<Field> Create(syntax::DictionaryObjectPtr root);

    virtual Field::Type GetFieldType() const noexcept = 0;
};

class NonTerminalField : public Field {
public:
    explicit NonTerminalField(syntax::DictionaryObjectPtr root);
    virtual Field::Type GetFieldType() const noexcept override { return Field::Type::NonTerminal; }
};

class ButtonField : public Field {
public:
    explicit ButtonField(syntax::DictionaryObjectPtr root);
    virtual Field::Type GetFieldType() const noexcept override { return Field::Type::Button; }
};

class TextField : public Field {
public:
    explicit TextField(syntax::DictionaryObjectPtr root);
    virtual Field::Type GetFieldType() const noexcept override { return Field::Type::Text; }
};

class ChoiceField : public Field {
public:
    explicit ChoiceField(syntax::DictionaryObjectPtr root);
    virtual Field::Type GetFieldType() const noexcept override { return Field::Type::Choice; }
};

class SignatureField : public Field {
public:
    explicit SignatureField(syntax::DictionaryObjectPtr root);
    virtual Field::Type GetFieldType() const noexcept override { return Field::Type::Signature; }

    bool Value(OuputDigitalSignaturePtr& result) const;
};

} // semantics
} // vanillapdf

#endif /* _FIELDS_H */