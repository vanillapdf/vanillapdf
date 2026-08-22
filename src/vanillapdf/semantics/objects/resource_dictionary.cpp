#include "precompiled.h"
#include "semantics/objects/resource_dictionary.h"
#include "semantics/objects/xobject.h"

#include "syntax/utils/name_constants.h"

namespace vanillapdf {
namespace semantics {

ResourceDictionary::ResourceDictionary(syntax::DictionaryObjectPtr obj) : HighLevelObject(obj) {
    m_cache_lock = std::unique_ptr<std::recursive_mutex>(pdf_new std::recursive_mutex());
}

bool ResourceDictionary::Font(OutputFontMapPtr& result) const {
    if (!_obj->Contains(constant::Name::Font)) {
        return false;
    }

    auto dict = _obj->FindAs<syntax::DictionaryObjectPtr>(constant::Name::Font);
    result = make_deferred<FontMap>(dict);
    return true;
}

void ResourceDictionary::BuildXObjectNameCache() const {
    if (!_obj->Contains(constant::Name::XObject)) {
        m_cache_built = true;
        return;
    }

    auto xobject_dictionary = _obj->FindAs<syntax::DictionaryObjectPtr>(constant::Name::XObject);
    for (const auto& entry : xobject_dictionary) {
        m_xobject_name_cache.push_back(entry.first);
    }

    m_cache_built = true;
}

void ResourceDictionary::InvalidateXObjectNameCache() {
    ACCESS_LOCK_GUARD(m_cache_lock);

    m_xobject_name_cache.clear();
    m_cache_built = false;
}

types::size_type ResourceDictionary::GetXObjectCount() const {
    ACCESS_LOCK_GUARD(m_cache_lock);

    if (!m_cache_built) {
        BuildXObjectNameCache();
    }

    return m_xobject_name_cache.size();
}

syntax::NameObjectPtr ResourceDictionary::GetXObjectName(types::size_type index) const {
    ACCESS_LOCK_GUARD(m_cache_lock);

    if (!m_cache_built) {
        BuildXObjectNameCache();
    }

    if (index >= m_xobject_name_cache.size()) {
        LOG_ERROR_AND_THROW(InvalidParameterException, "XObject name index is out of range: {}", index);
    }

    return m_xobject_name_cache[index];
}

bool ResourceDictionary::TryFindXObject(const syntax::NameObject& name, OutputXObjectPtr& result) const {
    if (!_obj->Contains(constant::Name::XObject)) {
        return false;
    }

    auto xobject_dictionary = _obj->FindAs<syntax::DictionaryObjectPtr>(constant::Name::XObject);
    if (!xobject_dictionary->Contains(name)) {
        return false;
    }

    auto xobject_stream = xobject_dictionary->FindAs<syntax::StreamObjectPtr>(name);
    result = XObjectBase::Create(xobject_stream);
    return true;
}

void ResourceDictionary::AddXObject(const syntax::NameObject& name, XObjectPtr value) {
    if (!_obj->Contains(constant::Name::XObject)) {
        syntax::DictionaryObjectPtr new_xobject_dictionary;
        _obj->Insert(constant::Name::XObject, new_xobject_dictionary);
    }

    auto xobject_dictionary = _obj->FindAs<syntax::DictionaryObjectPtr>(constant::Name::XObject);

    // XObjects are stream objects, which are always indirect,
    // so the resource entry stores a reference
    syntax::IndirectReferenceObjectPtr reference = make_deferred<syntax::IndirectReferenceObject>(value->GetObject());
    xobject_dictionary->Insert(name, reference, true);

    InvalidateXObjectNameCache();
}

bool ResourceDictionary::RemoveXObject(const syntax::NameObject& name) {
    if (!_obj->Contains(constant::Name::XObject)) {
        return false;
    }

    auto xobject_dictionary = _obj->FindAs<syntax::DictionaryObjectPtr>(constant::Name::XObject);
    if (!xobject_dictionary->Contains(name)) {
        return false;
    }

    bool removed = xobject_dictionary->Remove(name);
    assert(removed && "Unable to remove existing item");

    InvalidateXObjectNameCache();
    return removed;
}

} // semantics
} // vanillapdf
