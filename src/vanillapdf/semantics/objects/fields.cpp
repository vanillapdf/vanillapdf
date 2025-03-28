#include "precompiled.h"

#include "semantics/objects/fields.h"

#include "syntax/utils/name_constants.h"

namespace vanillapdf {
namespace semantics {

Field::Field(syntax::DictionaryObjectPtr root) : HighLevelObject(root) {}
NonTerminalField::NonTerminalField(syntax::DictionaryObjectPtr root) : Field(root) {}
ButtonField::ButtonField(syntax::DictionaryObjectPtr root) : Field(root) {}
TextField::TextField(syntax::DictionaryObjectPtr root) : Field(root) {}
ChoiceField::ChoiceField(syntax::DictionaryObjectPtr root) : Field(root) {}
SignatureField::SignatureField(syntax::DictionaryObjectPtr root) : Field(root) {}
FieldCollection::FieldCollection(syntax::ArrayObjectPtr<syntax::DictionaryObjectPtr> root) : HighLevelObject(root) {}

std::unique_ptr<Field> Field::Create(syntax::DictionaryObjectPtr root) {
    if (!root->Contains(constant::Name::FT)) {
        return make_unique<NonTerminalField>(root);
    }

    syntax::ObjectPtr type_obj = root->Find(constant::Name::FT);
    if (!syntax::ObjectUtils::IsType<syntax::NameObjectPtr>(type_obj)) {
        throw GeneralException("Invalid field type");
    }

    syntax::NameObjectPtr type = syntax::ObjectUtils::ConvertTo<syntax::NameObjectPtr>(type_obj);

    if (type == constant::Name::Btn) {
        return make_unique<ButtonField>(root);
    }

    if (type == constant::Name::Tx) {
        return make_unique<TextField>(root);
    }

    if (type == constant::Name::Ch) {
        return make_unique<ChoiceField>(root);
    }

    if (type == constant::Name::Sig) {
        return make_unique<SignatureField>(root);
    }

    throw GeneralException("Unknown field type");
}

bool SignatureField::Value(OuputDigitalSignaturePtr& result) const {
    if (!_obj->Contains(constant::Name::V)) {
        return false;
    }

    auto value_obj = _obj->FindAs<syntax::DictionaryObjectPtr>(constant::Name::V);
    auto digital_signature = make_deferred<DigitalSignature>(value_obj);
    result = digital_signature;
    return true;
}

types::size_type FieldCollection::GetSize() const {
    return _obj->GetSize();
}

FieldPtr FieldCollection::At(types::size_type index) const {
    auto obj = _obj->GetValue(index);
    auto unique = Field::Create(obj);
    auto raw_ptr = unique.release();
    return FieldPtr(raw_ptr);
}

} // semantics
} // vanillapdf
