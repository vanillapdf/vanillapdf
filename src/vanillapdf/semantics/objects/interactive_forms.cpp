#include "precompiled.h"

#include "semantics/objects/signature_flags.h"
#include "semantics/objects/interactive_forms.h"
#include "semantics/objects/field_tree.h"
#include "semantics/objects/fields.h"
#include "semantics/objects/document.h"

#include "syntax/files/file.h"
#include "syntax/objects/array_object.h"
#include "syntax/utils/name_constants.h"

namespace vanillapdf {
namespace semantics {

InteractiveForm::InteractiveForm(syntax::DictionaryObjectPtr root) : HighLevelObject(root) {
    m_access_lock = std::unique_ptr<std::recursive_mutex>(pdf_new std::recursive_mutex());
}

InteractiveFormPtr InteractiveForm::Create(DocumentPtr document) {
    return Create(document->GetFile());
}

InteractiveFormPtr InteractiveForm::Create(syntax::FilePtr file) {
    syntax::DictionaryObjectPtr raw_dictionary;

    auto new_entry = file->AllocateNewEntry();
    new_entry->SetReference(raw_dictionary);
    new_entry->SetFile(file);
    new_entry->SetInitialized();

    raw_dictionary->SetFile(file);
    raw_dictionary->SetInitialized();

    return make_deferred<InteractiveForm>(raw_dictionary);
}

bool InteractiveForm::GetFieldTree(OutputFieldTreePtr& result) const {
    ACCESS_LOCK_GUARD(m_access_lock);

    if (!m_field_tree.empty()) {
        result = m_field_tree;
        return true;
    }

    if (!_obj->Contains(constant::Name::Fields)) {
        return false;
    }

    auto fields = _obj->FindAs<syntax::ArrayObjectPtr<syntax::IndirectReferenceObjectPtr>>(constant::Name::Fields);
    m_field_tree = make_deferred<FieldTree>(fields);

    result = m_field_tree;
    return true;
}

void InteractiveForm::SetFieldTree(FieldTreePtr value) {
    ACCESS_LOCK_GUARD(m_access_lock);

    auto fields = value->GetObject();

    // The array carries the file it was created for (FieldTree::Create,
    // or the parser), and Insert sets the owner only. A hierarchy of
    // another document holds references to that document's objects;
    // installed here they would serialize as this document's object
    // numbers, dangling. A detached form dictionary has no file to compare
    // against.
    auto tree_file = fields->Data()->GetFile();
    auto form_file = _obj->GetFile();
    if (!tree_file.IsEmpty() && !form_file.IsEmpty() && !tree_file.Identity(form_file)) {
        LOG_ERROR_AND_THROW(InvalidParameterException, "The field hierarchy belongs to a different document than the form");
    }

    _obj->Insert(constant::Name::Fields, fields->Data(), true);

    // The installed instance is the one handed out from now on, so that the
    // caller's handle and the form share a single cache
    m_field_tree = value;
}

void InteractiveForm::SetSignatureFlags(SignatureFlagsPtr value) {
    ACCESS_LOCK_GUARD(m_access_lock);

    _obj->Insert(constant::Name::SigFlags, value->GetObject(), true);
}

bool InteractiveForm::GetFields(OuputFieldCollectionPtr& result) const {
    ACCESS_LOCK_GUARD(m_access_lock);

    if (!_obj->Contains(constant::Name::Fields)) {
        return false;
    }

    auto fields = _obj->FindAs<syntax::ArrayObjectPtr<syntax::DictionaryObjectPtr>>(constant::Name::Fields);
    result = make_deferred<FieldCollection>(fields);
    return true;
}

bool InteractiveForm::GetSignatureFlags(OutputSignatureFlagsPtr& result) const {
    ACCESS_LOCK_GUARD(m_access_lock);

    if (!_obj->Contains(constant::Name::SigFlags)) {
        return false;
    }

    auto flags = _obj->FindAs<syntax::IntegerObjectPtr>(constant::Name::SigFlags);
    result = make_deferred<SignatureFlags>(flags);
    return true;
}

bool InteractiveForm::GetNeedAppearances(bool& result) const {
    ACCESS_LOCK_GUARD(m_access_lock);

    if (!_obj->Contains(constant::Name::NeedAppearances)) {
        return false;
    }

    auto value = _obj->FindAs<syntax::BooleanObjectPtr>(constant::Name::NeedAppearances);
    result = value->GetValue();
    return true;
}

bool InteractiveForm::GetDefaultAppearance(syntax::OutputStringObjectPtr& result) const {
    ACCESS_LOCK_GUARD(m_access_lock);

    if (!_obj->Contains(constant::Name::DA)) {
        return false;
    }

    result = _obj->FindAs<syntax::StringObjectPtr>(constant::Name::DA);
    return true;
}

void InteractiveForm::SetDefaultAppearance(syntax::StringObjectPtr value) {
    ACCESS_LOCK_GUARD(m_access_lock);

    _obj->Insert(constant::Name::DA, value, true);
}

bool InteractiveForm::GetQuadding(Field::Quadding& result) const {
    ACCESS_LOCK_GUARD(m_access_lock);

    if (!_obj->Contains(constant::Name::Q)) {
        return false;
    }

    auto quadding = _obj->FindAs<syntax::IntegerObjectPtr>(constant::Name::Q);
    result = Field::ConvertQuadding(quadding);
    return true;
}

void InteractiveForm::SetQuadding(Field::Quadding value) {
    ACCESS_LOCK_GUARD(m_access_lock);

    auto quadding = make_deferred<syntax::IntegerObject>(Field::ConvertQuadding(value));
    _obj->Insert(constant::Name::Q, quadding, true);
}

bool InteractiveForm::ResolveDefaultAppearance(const FieldPtr& field, syntax::OutputStringObjectPtr& result) const {
    return field->GetDefaultAppearance(result) || GetDefaultAppearance(result);
}

Field::Quadding InteractiveForm::ResolveQuadding(const FieldPtr& field) const {
    Field::Quadding quadding = Field::Quadding::LeftJustified;
    if (field->GetQuadding(quadding) || GetQuadding(quadding)) {
        return quadding;
    }

    return Field::Quadding::LeftJustified;
}

void InteractiveForm::SetNeedAppearances(bool value) {
    ACCESS_LOCK_GUARD(m_access_lock);

    if (_obj->Contains(constant::Name::NeedAppearances)) {
        bool removed = _obj->Remove(constant::Name::NeedAppearances);
        assert(removed && "Unable to remove existing item"); UNUSED(removed);
    }
    auto bool_obj = make_deferred<syntax::BooleanObject>(value);
    _obj->Insert(constant::Name::NeedAppearances, bool_obj);
}

SignatureFlagsPtr InteractiveForm::CreateSignatureFlags() {
    ACCESS_LOCK_GUARD(m_access_lock);

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
