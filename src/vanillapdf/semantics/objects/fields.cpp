#include "precompiled.h"

#include "semantics/objects/fields.h"

#include "syntax/utils/name_constants.h"

namespace vanillapdf {
namespace semantics {

FieldPtr Field::Create(syntax::DictionaryObjectPtr root) {
    if (!root->Contains(constant::Name::FT)) {
        return make_deferred<NonTerminalField>(root);
    }

    syntax::ObjectPtr type_obj = root->Find(constant::Name::FT);
    if (!syntax::ObjectUtils::IsType<syntax::NameObjectPtr>(type_obj)) {
        LOG_ERROR_AND_THROW_GENERAL("Invalid field type: {}", static_cast<int32_t>(type_obj->GetObjectType()));
    }

    syntax::NameObjectPtr type = syntax::ObjectUtils::ConvertTo<syntax::NameObjectPtr>(type_obj);

    if (type == constant::Name::Btn) {
        return make_deferred<ButtonField>(root);
    }

    if (type == constant::Name::Tx) {
        return make_deferred<TextField>(root);
    }

    if (type == constant::Name::Ch) {
        return make_deferred<ChoiceField>(root);
    }

    if (type == constant::Name::Sig) {
        return make_deferred<SignatureField>(root);
    }

    LOG_ERROR_AND_THROW_GENERAL("Unknown field type: {}", type->ToString());
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
    return Field::Create(obj);
}

} // semantics
} // vanillapdf
