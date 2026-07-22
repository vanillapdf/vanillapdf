#include "precompiled.h"

#include "syntax/files/file.h"
#include "syntax/objects/dictionary_object.h"
#include "syntax/utils/output_pointer.h"
#include "syntax/exceptions/syntax_exceptions.h"

#include "utils/streams/output_stream_interface.h"

#include "utils/streams/stream_utils.h"

namespace vanillapdf {
namespace syntax {

DictionaryObject::DictionaryObject() {
    m_access_lock = std::unique_ptr<std::recursive_mutex>(pdf_new std::recursive_mutex());
}

DictionaryObject* DictionaryObject::Clone(void) const {
    DictionaryObjectPtr result(pdf_new DictionaryObject(), false);

    ACCESS_LOCK_GUARD(m_access_lock);

    for (auto& item : _list) {
        auto name = ObjectUtils::Clone<NameObjectPtr>(item.first);
        auto value = ObjectUtils::Clone<ContainableObjectPtr>(item.second);
        result->Insert(name, value);
    }

    CloneBaseProperties(result);
    return result.detach();
}

void DictionaryObject::SetFile(WeakReference<File> file) {

    ACCESS_LOCK_GUARD(m_access_lock);

    Object::SetFile(file);
    for (auto it = _list.begin(); it != _list.end(); ++it) {
        auto item = it->second;
        item->SetFile(file);
    }
}

void DictionaryObject::SetInitialized(bool initialized) {
    ACCESS_LOCK_GUARD(m_access_lock);

    Versionable::SetInitialized(initialized);
    for (auto it = _list.begin(); it != _list.end(); ++it) {
        auto item = it->second;
        item->SetInitialized(initialized);
    }
}

bool DictionaryObject::IsDirty() const {
    ACCESS_LOCK_GUARD(m_access_lock);

    if (m_version > 0) return true;
    for (const auto& item : _list) {
        if (item.first->IsDirty()) return true;
        if (item.second->IsDirty()) return true;
    }
    return false;
}

std::string DictionaryObject::ToString(void) const {
    ACCESS_LOCK_GUARD(m_access_lock);

    auto stream = StreamUtils::InputOutputStreamFromMemory();
    stream->WriteLine("<<");
    for (auto item : _list) {
        stream->Write(item.first->ToString());
        stream->Write(WhiteSpace::SPACE);
        stream->WriteLine(item.second->ToString());
    }

    stream->Write(">>");
    return stream->ToString();
}

void DictionaryObject::ToPdfStreamInternal(IOutputStreamPtr output) const {
    ACCESS_LOCK_GUARD(m_access_lock);

    output << "<<";
    bool first = true;
    for (auto item : _list) {
        output << (first ? "" : " ");
        item.first->ToPdfStream(output);
        output << " ";
        item.second->ToPdfStream(output);
        first = false;
    }

    output << ">>";
}

void DictionaryObject::ToPdfStreamUpdateOffset(IOutputStreamPtr output) {
    ACCESS_LOCK_GUARD(m_access_lock);

    UpdateOffset(output);

    // If the object contains attribute, that controls it's serialization
    if (HasOverrideAttribute()) {
        auto override_attribute = GetOverrideAttribute();
        output->Write(override_attribute);
        return;
    }

    output << "<<";
    bool first = true;
    for (std::pair<NameObjectPtr, ContainableObjectPtr> item : _list) {
        output << (first ? "" : " ");
        item.first->ToPdfStreamUpdateOffset(output);
        output << " ";
        item.second->ToPdfStreamUpdateOffset(output);
        first = false;
    }

    output << ">>";
}

ContainableObjectPtr DictionaryObject::Find(const NameObject& name) const {
    NameObjectPtr temp = make_deferred<NameObject>(name);
    return Find(temp);
}

ContainableObjectPtr DictionaryObject::Find(const NameObjectPtr name) const {

    ACCESS_LOCK_GUARD(m_access_lock);

    auto result = _list.find(name);
    if (result == _list.end()) {
        LOG_ERROR_AND_THROW(InvalidParameterException, "Item with name {} was not found in dictionary", name->ToString());
    }

    return result->second;
}

bool DictionaryObject::TryFind(const NameObject& name, OutputContainableObjectPtr& result) const {
    NameObjectPtr temp = make_deferred<NameObject>(name);
    return TryFind(temp, result);
}

bool DictionaryObject::TryFind(const NameObjectPtr name, OutputContainableObjectPtr& result) const {

    ACCESS_LOCK_GUARD(m_access_lock);

    auto item = _list.find(name);
    if (item == _list.end()) {
        return false;
    }

    result = item->second;
    return true;
}

bool DictionaryObject::Remove(const NameObject& name) {
    NameObjectPtr temp = make_deferred<NameObject>(name);
    return Remove(temp);
}

bool DictionaryObject::Remove(const NameObjectPtr name) {

    ACCESS_LOCK_GUARD(m_access_lock);

    auto found = _list.find(name);
    if (found == _list.end()) {
        return false;
    }

    // The trick here is that assignment
    // creates a non-const copy.
    // Be careful to preserve
    // the object state
    auto found_key = found->first;
    auto found_value = found->second;

    found_key->ClearOwner();
    found_value->ClearOwner();
    _list.erase(found);
    IncrementVersion();

    return true;
}

void DictionaryObject::Insert(const NameObject& name, ContainableObjectPtr value, bool overwrite) {
    NameObjectPtr temp = make_deferred<NameObject>(name);
    Insert(temp, value, overwrite);
}

void DictionaryObject::Insert(NameObjectPtr name, ContainableObjectPtr value, bool overwrite) {
    InsertInternal(name, value, overwrite, true /* mark_dirty */);
}

void DictionaryObject::ReplaceSerializationEntry(const NameObject& name, ContainableObjectPtr value) {
    NameObjectPtr temp = make_deferred<NameObject>(name);
    ReplaceSerializationEntry(temp, value);
}

void DictionaryObject::ReplaceSerializationEntry(NameObjectPtr name, ContainableObjectPtr value) {
    // Stamps a write-time derived entry (e.g. /Length) without dirtying the object - see the
    // header declaration and issue #460.
    InsertInternal(name, value, true /* overwrite */, false /* mark_dirty */);
}

void DictionaryObject::InsertInternal(NameObjectPtr name, ContainableObjectPtr value, bool overwrite, bool mark_dirty) {

    ACCESS_LOCK_GUARD(m_access_lock);

    auto found = _list.find(name);
    if (found != _list.end()) {
        if (!overwrite) {
            throw DuplicateKeyException("The key " + name->ToString() + " was already present in the dictionary");
        }

        // Only a genuine, dirtying overwrite is worth flagging.
        if (mark_dirty) {
            spdlog::info("Overwriting dictionary entry for key: {}", name->ToString());
        }

        // Preserve the state of the existing objects before removing them
        auto found_key = found->first;
        auto found_value = found->second;

        found_key->ClearOwner();
        found_value->ClearOwner();

        _list.erase(found);
    }

    auto pair = std::make_pair(name, value);
    _list.insert(pair);

    name->SetOwner(Object::GetWeakReference());
    value->SetOwner(Object::GetWeakReference());

    if (mark_dirty) {
        IncrementVersion();
    }
}

bool DictionaryObject::Contains(const NameObject& name) const {
    NameObjectPtr temp = make_deferred<NameObject>(name);
    return Contains(temp);
}

bool DictionaryObject::Contains(const NameObjectPtr name) const {

    ACCESS_LOCK_GUARD(m_access_lock);
    return (_list.find(name) != _list.end());
}

DictionaryObject::~DictionaryObject() {
    Clear();
}

size_t DictionaryObject::Hash() const {
    ACCESS_LOCK_GUARD(m_access_lock);

    // XOR is order-independent, which is correct for dictionaries since key
    // order is not significant in PDF. Each key-value pair is combined with
    // FNV-1a multiply before XOR to avoid cross-pair cancellation
    // (e.g. {A: 1, B: 2} vs {A: 2, B: 1}).
    size_t result = constant::FNV1A_OFFSET_BASIS;
    for (auto item : _list) {
        size_t pair_hash = item.first->Hash();
        pair_hash ^= item.second->Hash();
        pair_hash *= constant::FNV1A_PRIME;
        result ^= pair_hash;
    }

    return result;
}

bool DictionaryObject::Equals(ObjectPtr other) const {
    ACCESS_LOCK_GUARD(m_access_lock);

    if (!ObjectUtils::IsType<DictionaryObjectPtr>(other)) {
        return false;
    }

    auto other_obj = ObjectUtils::ConvertTo<DictionaryObjectPtr>(other);

    auto first_size = size();
    auto second_size = other_obj->size();
    if (first_size != second_size) {
        return false;
    }

    auto this_iterator = _list.begin();
    auto other_iterator = other_obj.begin();

    for (; this_iterator != _list.end(); this_iterator++, other_iterator++) {
        auto this_pair = *this_iterator;
        auto other_pair = *other_iterator;

        auto this_name = this_pair.first;
        auto other_name = other_pair.first;

        if (!this_name->Equals(*other_name)) {
            return false;
        }

        auto this_value = this_pair.second;
        auto other_value = other_pair.second;

        if (!this_value->Equals(other_value)) {
            return false;
        }
    }

    return true;
}

void DictionaryObject::Merge(const DictionaryObject& other) {
    ACCESS_LOCK_GUARD(m_access_lock);

    // TODO: https://github.com/vanillapdf/vanillapdf/issues/270 - SetOwner on merged entries blocked by const Deferred key in std::map
    // Simple insert overriding conflicting entries
    for (auto item : other) {
        _list.insert(item);
    }

    IncrementVersion();
}

void DictionaryObject::Clear() {
    ACCESS_LOCK_GUARD(m_access_lock);

    for (auto item : _list) {

        // The trick here is that assignment
        // creates a non-const copy.
        // Be careful to preserve
        // the object state
        auto item_key = item.first;
        auto item_value = item.second;

        item_key->ClearOwner();
        item_value->ClearOwner();
    }

    _list.clear();

    IncrementVersion();
}

DictionaryObject::size_type DictionaryObject::GetSize() const {
    ACCESS_LOCK_GUARD(m_access_lock);

    return _list.size();
}

} // syntax
} // vanillapdf
