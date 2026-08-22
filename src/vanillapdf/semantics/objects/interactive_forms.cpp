#include "precompiled.h"

#include "semantics/objects/signature_flags.h"
#include "semantics/objects/interactive_forms.h"
#include "semantics/objects/fields.h"
#include "semantics/objects/document.h"

#include "syntax/exceptions/syntax_exceptions.h"
#include "syntax/files/file.h"
#include "syntax/objects/array_object.h"
#include "syntax/utils/name_constants.h"

#include "utils/buffer.h"

#include <map>

namespace vanillapdf {
namespace semantics {

InteractiveForm::InteractiveForm(syntax::DictionaryObjectPtr root) : HighLevelObject(root) {
    m_cache_lock = std::unique_ptr<std::recursive_mutex>(pdf_new std::recursive_mutex());
}

InteractiveFormPtr InteractiveForm::Create(DocumentPtr document) {
    auto file = document->GetFile();

    syntax::DictionaryObjectPtr raw_dictionary;

    auto new_entry = file->AllocateNewEntry();
    new_entry->SetReference(raw_dictionary);
    new_entry->SetFile(file);
    new_entry->SetInitialized();

    raw_dictionary->SetFile(file);
    raw_dictionary->SetInitialized();

    return make_deferred<InteractiveForm>(raw_dictionary);
}

void InteractiveForm::SetFields(FieldCollectionPtr value) {
    ACCESS_LOCK_GUARD(m_cache_lock);

    _obj->Insert(constant::Name::Fields, value->GetObject(), true);
    InvalidateFieldCache();
}

void InteractiveForm::AddField(FieldPtr value) {
    auto field_dictionary = value->GetObject();

    // The /Fields array holds indirect references (Table 218) - a direct
    // dictionary cannot be referenced and would serialize as a dangling 0 0 R
    if (!field_dictionary->IsIndirect()) {
        LOG_ERROR_AND_THROW(InvalidParameterException, "The field dictionary shall be an indirect object - allocate a cross-reference entry for it first");
    }

    // The cache lock spans the whole mutation, so the create-if-missing
    // check and the append are atomic with respect to concurrent mutators
    // and cache builds on this form
    ACCESS_LOCK_GUARD(m_cache_lock);

    if (!_obj->Contains(constant::Name::Fields)) {
        syntax::MixedArrayObjectPtr mixed_array;
        mixed_array->SetFile(_obj->GetFile());
        mixed_array->SetInitialized();

        _obj->Insert(constant::Name::Fields, mixed_array);
    }

    auto root_fields = _obj->FindAs<syntax::ArrayObjectPtr<syntax::IndirectReferenceObjectPtr>>(constant::Name::Fields);
    auto field_reference = make_deferred<syntax::IndirectReferenceObject>(field_dictionary);
    root_fields->Append(field_reference);

    InvalidateFieldCache();
}

void InteractiveForm::BuildFieldCache() const {
    if (!_obj->Contains(constant::Name::Fields)) {
        m_cache_built = true;
        return;
    }

    // Both /Fields and /Kids shall contain indirect references (Table 218,
    // Table 220), so the tree is walked as references and every node is
    // dereferenced only after the cycle check - the same way PageTree types
    // its kids array.
    auto root_fields = _obj->FindAs<syntax::ArrayObjectPtr<syntax::IndirectReferenceObjectPtr>>(constant::Name::Fields);

    // Malformed documents can link /Kids in a cycle. Visited nodes are
    // tracked by their object identity, the same way Field guards its
    // /Parent chain walk.
    std::map<syntax::IndirectReferenceId, bool> visited;

    for (auto field_reference : root_fields) {
        BuildFieldCacheInternal(field_reference, visited);
    }

    m_cache_built = true;
}

void InteractiveForm::BuildFieldCacheInternal(
    syntax::IndirectReferenceObjectPtr node_reference,
    std::map<syntax::IndirectReferenceId, bool>& visited) const {

    auto object_number = node_reference->GetReferencedObjectNumber();
    auto generation_number = node_reference->GetReferencedGenerationNumber();
    syntax::IndirectReferenceId node_id(object_number, generation_number);

    auto found = visited.find(node_id);
    if (found != visited.end() && found->second) {
        spdlog::warn("Cyclic /Kids entry while enumerating form fields");
        return;
    }

    visited[node_id] = true;

    auto node = node_reference->GetReferencedObjectAs<syntax::DictionaryObjectPtr>();

    if (Field::IsTerminalDictionary(node)) {
        m_field_cache.push_back(node);
        return;
    }

    // Only field dictionaries are hierarchy nodes - sibling widget
    // annotations stay attached to their field (12.7.3.2)
    auto kids = node->FindAs<syntax::ArrayObjectPtr<syntax::IndirectReferenceObjectPtr>>(constant::Name::Kids);
    for (auto kid_reference : kids) {
        auto kid = kid_reference->GetReferencedObjectAs<syntax::DictionaryObjectPtr>();
        if (!kid->Contains(constant::Name::T)) {
            continue;
        }

        BuildFieldCacheInternal(kid_reference, visited);
    }
}

void InteractiveForm::InvalidateFieldCache() {
    ACCESS_LOCK_GUARD(m_cache_lock);

    m_field_cache.clear();
    m_cache_built = false;
}

types::size_type InteractiveForm::GetFieldCount() const {
    ACCESS_LOCK_GUARD(m_cache_lock);

    if (!m_cache_built) {
        BuildFieldCache();
    }

    return m_field_cache.size();
}

FieldPtr InteractiveForm::GetField(types::size_type index) const {
    ACCESS_LOCK_GUARD(m_cache_lock);

    if (!m_cache_built) {
        BuildFieldCache();
    }

    if (index >= m_field_cache.size()) {
        LOG_ERROR_AND_THROW(syntax::ObjectMissingException, "Field index out of range: {}", index);
    }

    return Field::Create(m_field_cache[index]);
}

bool InteractiveForm::TryFindField(std::string_view qualified_name, OuputFieldPtr& result) const {
    ACCESS_LOCK_GUARD(m_cache_lock);

    if (!m_cache_built) {
        BuildFieldCache();
    }

    for (const auto& field_dictionary : m_field_cache) {
        auto field = Field::Create(field_dictionary);
        auto field_qualified_name = field->GetQualifiedName();
        if (field_qualified_name->ToString() == qualified_name) {
            result = field;
            return true;
        }
    }

    return false;
}

void InteractiveForm::SetSignatureFlags(SignatureFlagsPtr value) {
    _obj->Insert(constant::Name::SigFlags, value->GetObject(), true);
}

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

bool InteractiveForm::GetDefaultAppearance(syntax::OutputStringObjectPtr& result) const {
    if (!_obj->Contains(constant::Name::DA)) {
        return false;
    }

    result = _obj->FindAs<syntax::StringObjectPtr>(constant::Name::DA);
    return true;
}

void InteractiveForm::SetDefaultAppearance(syntax::StringObjectPtr value) {
    _obj->Insert(constant::Name::DA, value, true);
}

bool InteractiveForm::GetQuadding(Field::Quadding& result) const {
    if (!_obj->Contains(constant::Name::Q)) {
        return false;
    }

    auto quadding = _obj->FindAs<syntax::IntegerObjectPtr>(constant::Name::Q);
    result = Field::ConvertQuadding(quadding);
    return true;
}

void InteractiveForm::SetQuadding(Field::Quadding value) {
    auto quadding = make_deferred<syntax::IntegerObject>(Field::ConvertQuadding(value));
    _obj->Insert(constant::Name::Q, quadding, true);
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
