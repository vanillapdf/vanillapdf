#include "precompiled.h"

#include "semantics/objects/fields.h"

#include "syntax/exceptions/syntax_exceptions.h"
#include "syntax/utils/name_constants.h"

namespace vanillapdf {
namespace semantics {

FieldPtr Field::Create(syntax::DictionaryObjectPtr root) {
    if (!root->Contains(constant::Name::FT)) {
        return make_deferred<NonTerminalField>(root);
    }

    syntax::ObjectPtr type_obj = root->Find(constant::Name::FT);
    if (!syntax::ObjectUtils::IsType<syntax::NameObjectPtr>(type_obj)) {
        LOG_ERROR_AND_THROW(syntax::ObjectMissingException, "Invalid field type: {}", static_cast<int32_t>(type_obj->GetObjectType()));
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

    LOG_ERROR_AND_THROW(syntax::ObjectMissingException, "Unknown field type: {}", type->ToString());
}

// Field base class properties

bool Field::GetName(syntax::OutputStringObjectPtr& result) const {
    if (!_obj->Contains(constant::Name::T)) {
        return false;
    }

    result = _obj->FindAs<syntax::StringObjectPtr>(constant::Name::T);
    return true;
}

bool Field::GetAlternateName(syntax::OutputStringObjectPtr& result) const {
    if (!_obj->Contains(constant::Name::TU)) {
        return false;
    }

    result = _obj->FindAs<syntax::StringObjectPtr>(constant::Name::TU);
    return true;
}

bool Field::GetFieldFlags(types::big_int& result) const {
    if (!_obj->Contains(constant::Name::Ff)) {
        return false;
    }

    auto flags = _obj->FindAs<syntax::IntegerObjectPtr>(constant::Name::Ff);
    result = flags->GetIntegerValue();
    return true;
}

void Field::SetFieldFlags(types::big_int value) {
    if (_obj->Contains(constant::Name::Ff)) {
        bool removed = _obj->Remove(constant::Name::Ff);
        assert(removed && "Unable to remove existing item"); UNUSED(removed);
    }
    auto flags = make_deferred<syntax::IntegerObject>(value);
    _obj->Insert(constant::Name::Ff, flags);
}

// ButtonField properties

bool ButtonField::GetValue(syntax::OutputNameObjectPtr& result) const {
    if (!_obj->Contains(constant::Name::V)) {
        return false;
    }

    result = _obj->FindAs<syntax::NameObjectPtr>(constant::Name::V);
    return true;
}

void ButtonField::SetValue(syntax::NameObjectPtr value) {
    if (_obj->Contains(constant::Name::V)) {
        bool removed = _obj->Remove(constant::Name::V);
        assert(removed && "Unable to remove existing item"); UNUSED(removed);
    }
    _obj->Insert(constant::Name::V, value);
}

// TextField properties

bool TextField::GetValue(syntax::OutputStringObjectPtr& result) const {
    if (!_obj->Contains(constant::Name::V)) {
        return false;
    }

    result = _obj->FindAs<syntax::StringObjectPtr>(constant::Name::V);
    return true;
}

void TextField::SetValue(syntax::StringObjectPtr value) {
    if (_obj->Contains(constant::Name::V)) {
        bool removed = _obj->Remove(constant::Name::V);
        assert(removed && "Unable to remove existing item"); UNUSED(removed);
    }
    _obj->Insert(constant::Name::V, value);
}

bool TextField::GetDefaultValue(syntax::OutputStringObjectPtr& result) const {
    if (!_obj->Contains(constant::Name::DV)) {
        return false;
    }

    result = _obj->FindAs<syntax::StringObjectPtr>(constant::Name::DV);
    return true;
}

bool TextField::GetMaxLength(syntax::OutputIntegerObjectPtr& result) const {
    if (!_obj->Contains(constant::Name::MaxLen)) {
        return false;
    }

    result = _obj->FindAs<syntax::IntegerObjectPtr>(constant::Name::MaxLen);
    return true;
}

// ChoiceField properties

bool ChoiceField::GetValue(syntax::OutputStringObjectPtr& result) const {
    if (!_obj->Contains(constant::Name::V)) {
        return false;
    }

    result = _obj->FindAs<syntax::StringObjectPtr>(constant::Name::V);
    return true;
}

void ChoiceField::SetValue(syntax::StringObjectPtr value) {
    if (_obj->Contains(constant::Name::V)) {
        bool removed = _obj->Remove(constant::Name::V);
        assert(removed && "Unable to remove existing item"); UNUSED(removed);
    }
    _obj->Insert(constant::Name::V, value);
}

bool ChoiceField::GetOptionCount(types::size_type& result) const {
    if (!_obj->Contains(constant::Name::Opt)) {
        return false;
    }

    auto opts = _obj->FindAs<syntax::MixedArrayObjectPtr>(constant::Name::Opt);
    result = opts->GetSize();
    return true;
}

bool ChoiceField::GetOptionAt(types::size_type index, syntax::OutputContainableObjectPtr& result) const {
    if (!_obj->Contains(constant::Name::Opt)) {
        return false;
    }

    auto opts = _obj->FindAs<syntax::MixedArrayObjectPtr>(constant::Name::Opt);
    result = opts->GetValue(index);
    return true;
}

// SignatureField properties

bool SignatureField::Value(OuputDigitalSignaturePtr& result) const {
    if (!_obj->Contains(constant::Name::V)) {
        return false;
    }

    auto value_obj = _obj->FindAs<syntax::DictionaryObjectPtr>(constant::Name::V);
    auto digital_signature = make_deferred<DigitalSignature>(value_obj);
    result = digital_signature;
    return true;
}

FieldCollectionPtr FieldCollection::Create() {
    syntax::ArrayObjectPtr<syntax::DictionaryObjectPtr> fields;
    return make_deferred<FieldCollection>(fields);
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
