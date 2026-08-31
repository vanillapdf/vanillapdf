#include "precompiled.h"

#include "syntax/objects/mixed_array_object.h"
#include "utils/streams/output_stream_interface.h"

#include "utils/streams/stream_utils.h"

namespace vanillapdf {
namespace syntax {

MixedArrayObject::MixedArrayObject() {
    m_access_lock = std::unique_ptr<std::recursive_mutex>(pdf_new std::recursive_mutex());
}

MixedArrayObject::MixedArrayObject(const list_type& list) : _list(list) {
    m_access_lock = std::unique_ptr<std::recursive_mutex>(pdf_new std::recursive_mutex());
    for (auto item : _list) {
        item->SetOwner(Object::GetWeakReference());
    }
}

MixedArrayObject::MixedArrayObject(const std::initializer_list<ContainableObjectPtr>& list) : _list(list) {
    m_access_lock = std::unique_ptr<std::recursive_mutex>(pdf_new std::recursive_mutex());
    for (auto item : _list) {
        item->SetOwner(Object::GetWeakReference());
    }
}

MixedArrayObject::MixedArrayObject(const ContainableObject& other, list_type& list)
    : ContainableObject(other), _list(list) {
    m_access_lock = std::unique_ptr<std::recursive_mutex>(pdf_new std::recursive_mutex());
    for (auto item : _list) {
        item->SetOwner(Object::GetWeakReference());
    }
}

MixedArrayObject::size_type MixedArrayObject::GetSize(void) const {
    ACCESS_LOCK_GUARD(m_access_lock);

    return _list.size();
}

ContainableObjectPtr MixedArrayObject::GetValue(size_type at) const {
    ACCESS_LOCK_GUARD(m_access_lock);

    return _list.at(at);
}

ContainableObjectPtr MixedArrayObject::GetValue(size_type at) {
    ACCESS_LOCK_GUARD(m_access_lock);

    return _list.at(at);
}

void MixedArrayObject::SetFile(WeakReference<File> file) {
    ACCESS_LOCK_GUARD(m_access_lock);

    Object::SetFile(file);

    auto size = _list.size();
    for (decltype(size) i = 0; i < size; ++i) {
        auto item = _list[i];
        item->SetFile(file);
    }
}

void MixedArrayObject::SetInitialized(bool initialized) {
    ACCESS_LOCK_GUARD(m_access_lock);

    Versionable::SetInitialized(initialized);

    auto size = _list.size();
    for (decltype(size) i = 0; i < size; ++i) {
        auto item = _list[i];
        item->SetInitialized(initialized);
    }
}

bool MixedArrayObject::IsDirty() const {
    ACCESS_LOCK_GUARD(m_access_lock);

    if (m_version > 0) return true;
    for (const auto& item : _list) {
        if (item->IsDirty()) return true;
    }
    return false;
}

MixedArrayObject* MixedArrayObject::Clone(void) const {
    MixedArrayObjectPtr result(pdf_new MixedArrayObject(), false);

    ACCESS_LOCK_GUARD(m_access_lock);

    for (auto item : _list) {
        auto cloned = ObjectUtils::Clone<ContainableObjectPtr>(item);
        result->Append(cloned);
    }

    CloneBaseProperties(result);
    return result.detach();
}

void MixedArrayObject::Append(ContainableObjectPtr value) {
    ACCESS_LOCK_GUARD(m_access_lock);

    _list.push_back(value);
    value->SetOwner(Object::GetWeakReference());
    IncrementVersion();
}

void MixedArrayObject::Insert(size_type at, ContainableObjectPtr value) {
    ACCESS_LOCK_GUARD(m_access_lock);

    if (at > _list.size()) {
        LOG_ERROR_AND_THROW(InvalidParameterException, "Index was outside the bounds of the array");
    }

    _list.insert(_list.begin() + at, value);
    value->SetOwner(Object::GetWeakReference());
    IncrementVersion();
}

bool MixedArrayObject::Remove(size_type at) {
    ACCESS_LOCK_GUARD(m_access_lock);

    if (at >= _list.size()) {
        return false;
    }

    auto item = _list.begin() + at;

    (*item)->ClearOwner();
    _list.erase(item);
    IncrementVersion();

    return true;
}

void MixedArrayObject::Clear() {
    ACCESS_LOCK_GUARD(m_access_lock);

    for (auto& item : _list) {
        item->ClearOwner();
    }

    _list.clear();
    IncrementVersion();
}

void MixedArrayObject::SetValue(size_type at, ContainableObjectPtr value) {
    ACCESS_LOCK_GUARD(m_access_lock);

    if (at >= _list.size()) {
        LOG_ERROR_AND_THROW(InvalidParameterException, "Index was outside the bounds of the array");
    }

    _list[at] = value;

    value->SetOwner(Object::GetWeakReference());
    IncrementVersion();
}

// stl compatibility
void MixedArrayObject::push_back(ContainableObjectPtr value) {
    ACCESS_LOCK_GUARD(m_access_lock);

    _list.push_back(value);
    IncrementVersion();
}

std::string MixedArrayObject::ToString(void) const {
    ACCESS_LOCK_GUARD(m_access_lock);

    auto stream = StreamUtils::InputOutputStreamFromMemory();
    stream->Write("[");
    bool first = true;
    for (auto item : _list) {
        if (!first) {
            stream->Write(WhiteSpace::SPACE);
        }
        stream->Write(item->ToString());
        first = false;
    }

    stream->Write("]");
    return stream->ToString();
}

void MixedArrayObject::ToPdfStreamInternal(IOutputStreamPtr output) const {
    ACCESS_LOCK_GUARD(m_access_lock);

    output << "[";
    bool first = true;
    for (auto item : _list) {
        output << (first ? "" : " ");
        item->ToPdfStream(output);
        first = false;
    }

    output << "]";
}

void MixedArrayObject::ToPdfStreamUpdateOffset(IOutputStreamPtr output) {
    ACCESS_LOCK_GUARD(m_access_lock);

    UpdateOffset(output);

    // If the object contains attribute, that controls it's serialization
    if (HasOverrideAttribute()) {
        auto override_attribute = GetOverrideAttribute();
        output->Write(override_attribute);
        return;
    }

    output << "[";
    bool first = true;
    for (auto item : _list) {
        output << (first ? "" : " ");
        item->ToPdfStreamUpdateOffset(output);
        first = false;
    }

    output << "]";
}

size_t MixedArrayObject::Hash() const {
    ACCESS_LOCK_GUARD(m_access_lock);

    // FNV-1a combine: element order matters for arrays (e.g. [842 595] vs [595 842])
    size_t result = constant::FNV1A_OFFSET_BASIS;
    for (auto item : _list) {
        result ^= item->Hash();
        result *= constant::FNV1A_PRIME;
    }

    return result;
}

bool MixedArrayObject::Equals(ObjectPtr other) const {
    ACCESS_LOCK_GUARD(m_access_lock);

    if (!ObjectUtils::IsType<MixedArrayObjectPtr>(other)) {
        return false;
    }

    auto other_obj = ObjectUtils::ConvertTo<MixedArrayObjectPtr>(other);

    auto first_size = GetSize();
    auto second_size = other_obj->GetSize();
    if (first_size != second_size) {
        return false;
    }

    for (unsigned int i = 0; i < first_size; ++i) {
        auto first_obj = GetValue(i);
        auto second_obj = other_obj->GetValue(i);
        if (!first_obj->Equals(second_obj)) {
            return false;
        }
    }

    return true;
}

} // syntax
} // vanillapdf
