#include "precompiled.h"

#include "semantics/objects/fields.h"

#include "syntax/exceptions/syntax_exceptions.h"
#include "syntax/objects/array_object.h"
#include "syntax/utils/name_constants.h"

#include "utils/buffer.h"
#include "utils/text_string_encoding.h"

#include <fmt/ranges.h>

namespace vanillapdf {
namespace semantics {

// Follows a single /Parent hop with the shared safety rules. Table 220:
// /Parent shall be an indirect reference. A direct object in its place is an
// embedded copy, not a link - the actual parent node is reachable from
// /Fields as an indirect object - so following it would inspect the wrong
// dictionary. Visited parents are tracked by their object identity, the same
// way DereferenceHelper guards a single dereference against cycles.
static bool TryGetParentDictionary(
    const syntax::DictionaryObjectPtr& current,
    std::map<syntax::IndirectReferenceId, bool>& visited,
    syntax::DictionaryObjectPtr& result) {

    if (!current->Contains(constant::Name::Parent)) {
        return false;
    }

    auto parent_obj = current->Find(constant::Name::Parent);
    if (!syntax::ObjectUtils::IsType<syntax::IndirectReferenceObjectPtr>(parent_obj)) {
        spdlog::warn("Field /Parent is not an indirect reference while walking the field hierarchy");
        return false;
    }

    auto parent_reference = syntax::ObjectUtils::ConvertTo<syntax::IndirectReferenceObjectPtr>(parent_obj);

    auto object_number = parent_reference->GetReferencedObjectNumber();
    auto generation_number = parent_reference->GetReferencedGenerationNumber();
    syntax::IndirectReferenceId parent_id(object_number, generation_number);

    auto found = visited.find(parent_id);
    if (found != visited.end() && found->second) {
        spdlog::warn("Cyclic /Parent chain while walking the field hierarchy");
        return false;
    }

    visited[parent_id] = true;

    result = parent_reference->GetReferencedObjectAs<syntax::DictionaryObjectPtr>();
    return true;
}

bool Field::FindInheritedEntry(
    const syntax::DictionaryObjectPtr& dictionary,
    const syntax::NameObject& key,
    syntax::OutputObjectPtr& result) {

    std::map<syntax::IndirectReferenceId, bool> visited;

    syntax::DictionaryObjectPtr current = dictionary;

    for (;;) {
        if (current->Contains(key)) {
            result = current->Find(key);
            return true;
        }

        if (!TryGetParentDictionary(current, visited, current)) {
            return false;
        }
    }
}

bool Field::GetInheritedEntry(const syntax::NameObject& key, syntax::OutputObjectPtr& result) const {
    return FindInheritedEntry(_obj, key, result);
}

bool Field::IsFieldDictionary(const syntax::DictionaryObjectPtr& dictionary) {
    return dictionary->Contains(constant::Name::T)
        || dictionary->Contains(constant::Name::Kids)
        || dictionary->Contains(constant::Name::FT);
}

Field::ChildEntryType Field::ClassifyChildEntry(const syntax::ObjectPtr& entry, syntax::OutputDictionaryObjectPtr& dictionary) {

    // The reference is required, not just the dictionary behind it: a
    // direct dictionary has no object number for its kids to name in
    // /Parent, and the mutators could never address it
    if (!syntax::ObjectUtils::IsType<syntax::IndirectReferenceObjectPtr>(entry)
        || !syntax::ObjectUtils::IsType<syntax::DictionaryObjectPtr>(entry)) {
        spdlog::warn("Field hierarchy entry of type {} is not an indirect reference to a dictionary and is skipped", static_cast<int32_t>(entry->GetObjectType()));
        return ChildEntryType::Malformed;
    }

    dictionary = syntax::ObjectUtils::ConvertTo<syntax::DictionaryObjectPtr>(entry);
    return IsFieldDictionary(dictionary) ? ChildEntryType::Field : ChildEntryType::Widget;
}

bool Field::IsTerminalDictionary(const syntax::DictionaryObjectPtr& dictionary) {
    if (!dictionary->Contains(constant::Name::Kids)) {
        return true;
    }

    auto kids = dictionary->FindAs<syntax::MixedArrayObjectPtr>(constant::Name::Kids);
    for (auto entry : kids) {
        syntax::OutputDictionaryObjectPtr kid;
        if (ClassifyChildEntry(entry, kid) == ChildEntryType::Field) {
            return false;
        }
    }

    return true;
}

bool Field::IsTerminal() const {
    return IsTerminalDictionary(_obj);
}

types::size_type Field::GetChildCount() const {
    if (!_obj->Contains(constant::Name::Kids)) {
        return 0;
    }

    types::size_type count = 0;

    auto kids = _obj->FindAs<syntax::MixedArrayObjectPtr>(constant::Name::Kids);
    for (auto entry : kids) {
        syntax::OutputDictionaryObjectPtr kid;
        if (ClassifyChildEntry(entry, kid) == ChildEntryType::Field) {
            count += 1;
        }
    }

    return count;
}

FieldPtr Field::GetChild(types::size_type index) const {
    if (_obj->Contains(constant::Name::Kids)) {
        types::size_type current = 0;

        auto kids = _obj->FindAs<syntax::MixedArrayObjectPtr>(constant::Name::Kids);
        for (auto entry : kids) {
            syntax::OutputDictionaryObjectPtr kid;
            if (ClassifyChildEntry(entry, kid) != ChildEntryType::Field) {
                continue;
            }

            if (current == index) {
                return Create(kid);
            }

            current += 1;
        }
    }

    LOG_ERROR_AND_THROW(syntax::ObjectMissingException, "Child field index out of range: {}", index);
}

bool Field::GetParent(OuputFieldPtr& result) const {
    std::map<syntax::IndirectReferenceId, bool> visited;

    syntax::DictionaryObjectPtr parent_dictionary;
    if (!TryGetParentDictionary(_obj, visited, parent_dictionary)) {
        return false;
    }

    result = Create(parent_dictionary);
    return true;
}

BufferPtr Field::GetQualifiedName() const {
    std::map<syntax::IndirectReferenceId, bool> visited;

    syntax::DictionaryObjectPtr current = _obj;

    // The /Parent chain climbs from this field towards the root, so the
    // partial names are collected leaf-first and joined in reverse
    std::vector<std::string> partial_names;

    do {
        if (current->Contains(constant::Name::T)) {
            auto partial_name = current->FindAs<syntax::StringObjectPtr>(constant::Name::T);
            auto partial_name_buffer = partial_name->GetValue();

            // /T is a text string (7.9.2.2) - normalizing every segment to
            // UTF-8 lets PDFDocEncoding and UTF-16BE partial names join into
            // a single coherent buffer
            auto partial_name_utf8 = TextStringEncoding::ToUtf8(partial_name_buffer->ToStringView());

            // Because the PERIOD is used as a separator for fully qualified
            // names, a partial name shall not contain a PERIOD (12.7.3.2)
            if (partial_name_utf8.find('.') != std::string::npos) {
                spdlog::warn("Partial field name \"{}\" contains a PERIOD, making the fully qualified name ambiguous", partial_name_utf8);
            }

            partial_names.push_back(partial_name_utf8);
        }
    } while (TryGetParentDictionary(current, visited, current));

    auto qualified_name = fmt::format("{}", fmt::join(partial_names.rbegin(), partial_names.rend(), "."));
    return make_deferred_container<Buffer>(qualified_name.begin(), qualified_name.end());
}

FieldPtr Field::Create(syntax::DictionaryObjectPtr root) {

    // /FT is inheritable - a terminal field merged into its widget annotation
    // frequently carries the field type only on the parent field
    syntax::OutputObjectPtr type_entry;
    if (!FindInheritedEntry(root, constant::Name::FT, type_entry)) {
        return make_deferred<NonTerminalField>(root);
    }

    syntax::ObjectPtr type_obj = type_entry;
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

void Field::SetName(syntax::StringObjectPtr value) {
    auto name_buffer = value->GetValue();
    auto name_utf8 = TextStringEncoding::ToUtf8(name_buffer->ToStringView());

    // Because the PERIOD is used as a separator for fully qualified names,
    // a partial name shall not contain a PERIOD (12.7.3.2)
    if (name_utf8.find('.') != std::string::npos) {
        LOG_ERROR_AND_THROW(InvalidParameterException, "Partial field name \"{}\" shall not contain a PERIOD", name_utf8);
    }

    _obj->Insert(constant::Name::T, value, true);
}

bool Field::GetAlternateName(syntax::OutputStringObjectPtr& result) const {
    if (!_obj->Contains(constant::Name::TU)) {
        return false;
    }

    result = _obj->FindAs<syntax::StringObjectPtr>(constant::Name::TU);
    return true;
}

void Field::SetAlternateName(syntax::StringObjectPtr value) {
    _obj->Insert(constant::Name::TU, value, true);
}

bool Field::GetFieldFlags(types::big_int& result) const {

    // /Ff is inheritable (Table 220)
    syntax::OutputObjectPtr flags_entry;
    if (!GetInheritedEntry(constant::Name::Ff, flags_entry)) {
        return false;
    }

    syntax::ObjectPtr flags_obj = flags_entry;
    auto flags = syntax::ObjectUtils::ConvertTo<syntax::IntegerObjectPtr>(flags_obj);
    result = flags->GetIntegerValue();
    return true;
}

void Field::SetFieldFlags(types::big_int value) {
    _obj->Remove(constant::Name::Ff);
    auto flags = make_deferred<syntax::IntegerObject>(value);
    _obj->Insert(constant::Name::Ff, flags);
}

bool Field::GetValueObject(syntax::OutputObjectPtr& result) const {

    // /V is inheritable (Table 220)
    return GetInheritedEntry(constant::Name::V, result);
}

bool Field::GetDefaultValueObject(syntax::OutputObjectPtr& result) const {

    // /DV is inheritable (Table 220)
    return GetInheritedEntry(constant::Name::DV, result);
}

bool Field::GetDefaultAppearance(syntax::OutputStringObjectPtr& result) const {

    // /DA falls back through the /Parent chain (12.7.3.3); the AcroForm
    // document default is the caller's fallback
    syntax::OutputObjectPtr appearance_entry;
    if (!GetInheritedEntry(constant::Name::DA, appearance_entry)) {
        return false;
    }

    syntax::ObjectPtr appearance_obj = appearance_entry;
    result = syntax::ObjectUtils::ConvertTo<syntax::StringObjectPtr>(appearance_obj);
    return true;
}

void Field::SetDefaultAppearance(syntax::StringObjectPtr value) {
    _obj->Insert(constant::Name::DA, value, true);
}

bool Field::GetQuadding(Quadding& result) const {

    // /Q falls back through the /Parent chain (12.7.3.3); the AcroForm
    // document default is the caller's fallback
    syntax::OutputObjectPtr quadding_entry;
    if (!GetInheritedEntry(constant::Name::Q, quadding_entry)) {
        return false;
    }

    syntax::ObjectPtr quadding_obj = quadding_entry;
    auto quadding = syntax::ObjectUtils::ConvertTo<syntax::IntegerObjectPtr>(quadding_obj);
    result = ConvertQuadding(quadding);
    return true;
}

void Field::SetQuadding(Quadding value) {
    auto quadding = make_deferred<syntax::IntegerObject>(ConvertQuadding(value));
    _obj->Insert(constant::Name::Q, quadding, true);
}

Field::Quadding Field::ConvertQuadding(const syntax::IntegerObjectPtr& value) {
    switch (value->GetIntegerValue()) {
        case 0:
            return Quadding::LeftJustified;
        case 1:
            return Quadding::Centered;
        case 2:
            return Quadding::RightJustified;
        default:
            LOG_ERROR_AND_THROW(syntax::ParseException, "Unknown quadding value: {}", value->GetIntegerValue());
    }
}

types::big_int Field::ConvertQuadding(Quadding value) {
    switch (value) {
        case Quadding::LeftJustified:
            return 0;
        case Quadding::Centered:
            return 1;
        case Quadding::RightJustified:
            return 2;
        default:
            LOG_ERROR_AND_THROW(ConversionException, "Unknown quadding value: {}", static_cast<int32_t>(value));
    }
}

// ButtonField properties

bool ButtonField::GetValue(syntax::OutputNameObjectPtr& result) const {

    // /V is inheritable (Table 220)
    syntax::OutputObjectPtr value_entry;
    if (!GetInheritedEntry(constant::Name::V, value_entry)) {
        return false;
    }

    syntax::ObjectPtr value_obj = value_entry;
    result = syntax::ObjectUtils::ConvertTo<syntax::NameObjectPtr>(value_obj);
    return true;
}

void ButtonField::SetValue(syntax::NameObjectPtr value) {
    _obj->Remove(constant::Name::V);
    _obj->Insert(constant::Name::V, value);
}

// TextField properties

bool TextField::GetValue(syntax::OutputStringObjectPtr& result) const {

    // /V is inheritable (Table 220)
    syntax::OutputObjectPtr value_entry;
    if (!GetInheritedEntry(constant::Name::V, value_entry)) {
        return false;
    }

    syntax::ObjectPtr value_obj = value_entry;
    result = syntax::ObjectUtils::ConvertTo<syntax::StringObjectPtr>(value_obj);
    return true;
}

void TextField::SetValue(syntax::StringObjectPtr value) {
    _obj->Remove(constant::Name::V);
    _obj->Insert(constant::Name::V, value);
}

bool TextField::GetDefaultValue(syntax::OutputStringObjectPtr& result) const {

    // /DV is inheritable (Table 220)
    syntax::OutputObjectPtr value_entry;
    if (!GetInheritedEntry(constant::Name::DV, value_entry)) {
        return false;
    }

    syntax::ObjectPtr value_obj = value_entry;
    result = syntax::ObjectUtils::ConvertTo<syntax::StringObjectPtr>(value_obj);
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

    // /V is inheritable (Table 220)
    syntax::OutputObjectPtr value_entry;
    if (!GetInheritedEntry(constant::Name::V, value_entry)) {
        return false;
    }

    syntax::ObjectPtr value_obj = value_entry;
    result = syntax::ObjectUtils::ConvertTo<syntax::StringObjectPtr>(value_obj);
    return true;
}

void ChoiceField::SetValue(syntax::StringObjectPtr value) {
    _obj->Remove(constant::Name::V);
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

    // /V is inheritable (Table 220)
    syntax::OutputObjectPtr value_entry;
    if (!GetInheritedEntry(constant::Name::V, value_entry)) {
        return false;
    }

    syntax::ObjectPtr value_obj = value_entry;
    auto value_dictionary = syntax::ObjectUtils::ConvertTo<syntax::DictionaryObjectPtr>(value_obj);
    auto digital_signature = make_deferred<DigitalSignature>(value_dictionary);
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
