#include "precompiled.h"

#include "semantics/objects/signature_flags.h"
#include "semantics/objects/interactive_forms.h"

#include "syntax/files/file.h"
#include "syntax/utils/name_constants.h"

namespace vanillapdf {
namespace semantics {

InteractiveForm::InteractiveForm(syntax::DictionaryObjectPtr root) : HighLevelObject(root) {}

bool InteractiveForm::GetFields(OuputFieldCollectionPtr& result) const {
    if (!_obj->Contains(constant::Name::Fields)) {
        return false;
    }

    auto fields = _obj->FindAs<syntax::ArrayObjectPtr<syntax::DictionaryObjectPtr>>(constant::Name::Fields);
    result = make_deferred<FieldCollection>(fields);
    return true;
}

bool InteractiveForm::GetSignatureFlags(OutputSignatureFlagsPtr& result) const {
    if (!_obj->Contains(constant::Name::SigFlags)) {
        return false;
    }

    auto flags = _obj->FindAs<syntax::IntegerObjectPtr>(constant::Name::SigFlags);
    result = make_deferred<SignatureFlags>(flags);
    return true;
}

bool InteractiveForm::GetNeedAppearances(bool& result) const {
    if (!_obj->Contains(constant::Name::NeedAppearances)) {
        return false;
    }

    auto value = _obj->FindAs<syntax::BooleanObjectPtr>(constant::Name::NeedAppearances);
    result = value->GetValue();
    return true;
}

void InteractiveForm::SetNeedAppearances(bool value) {
    if (_obj->Contains(constant::Name::NeedAppearances)) {
        bool removed = _obj->Remove(constant::Name::NeedAppearances);
        assert(removed && "Unable to remove existing item"); UNUSED(removed);
    }
    auto bool_obj = make_deferred<syntax::BooleanObject>(value);
    _obj->Insert(constant::Name::NeedAppearances, bool_obj);
}

FieldCollectionPtr InteractiveForm::CreateFields() {
    if (!_obj->Contains(constant::Name::Fields)) {
        syntax::MixedArrayObjectPtr mixed_array;
        mixed_array->SetFile(_obj->GetFile());
        mixed_array->SetInitialized();

        _obj->Insert(constant::Name::Fields, mixed_array);
    }

    auto fields = _obj->FindAs<syntax::ArrayObjectPtr<syntax::DictionaryObjectPtr>>(constant::Name::Fields);
    return make_deferred<FieldCollection>(fields);
}

SignatureFlagsPtr InteractiveForm::CreateSignatureFlags() {
    if (!_obj->Contains(constant::Name::SigFlags)) {
        syntax::IntegerObjectPtr signature_flags;
        signature_flags->SetFile(_obj->GetFile());
        signature_flags->SetInitialized();

        _obj->Insert(constant::Name::SigFlags, signature_flags);
    }

    auto flags = _obj->FindAs<syntax::IntegerObjectPtr>(constant::Name::SigFlags);
    return make_deferred<SignatureFlags>(flags);
}

} // semantics
} // vanillapdf
