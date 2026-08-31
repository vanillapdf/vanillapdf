#include "precompiled.h"

#include "syntax/exceptions/syntax_exceptions.h"

#include "semantics/utils/semantic_utils.h"

#include <regex>
#include <mutex>
#include <unordered_map>

namespace vanillapdf {
namespace semantics {

Version SemanticUtils::GetVersionFromName(const syntax::NameObjectPtr& name) {
    auto ver_str = name->GetValue()->ToString();

    std::smatch sm;
    std::regex header_regex("([0-9])\\.([0-9])");
    if (!std::regex_match(ver_str, sm, header_regex)) {
        LOG_ERROR_AND_THROW(NotSupportedException, "Unsupported pdf version: {}", ver_str);
    }

    assert(sm.size() == 3);

    if (stoi(sm[1]) == 1) {
        switch (stoi(sm[2])) {
            case 0: return Version::PDF10;
            case 1: return Version::PDF11;
            case 2: return Version::PDF12;
            case 3: return Version::PDF13;
            case 4: return Version::PDF14;
            case 5: return Version::PDF15;
            case 6: return Version::PDF16;
            case 7: return Version::PDF17;
            default: LOG_ERROR_AND_THROW(NotSupportedException, "Unsupported pdf version: {}", ver_str);
        }
    }

    if (stoi(sm[1]) == 2) {
        switch (stoi(sm[2])) {
            case 0: return Version::PDF20;
            default: LOG_ERROR_AND_THROW(NotSupportedException, "Unsupported pdf version: {}", ver_str);
        }
    }

    LOG_ERROR_AND_THROW(NotSupportedException, "Unsupported pdf version: {}", ver_str);
}

using document_map_type = std::unordered_map<syntax::File*, WeakReference<Document>>;
using document_map_ptr_type = std::shared_ptr<document_map_type>;

static std::recursive_mutex document_map_lock;
document_map_ptr_type GetDocumentMapInstance() {
    static document_map_ptr_type document_map;

    if (!document_map) {
        document_map = std::make_shared<document_map_type>();
    }

    return document_map;
}

bool SemanticUtils::HasMappedDocument(WeakReference<syntax::File> file) {
    if (!file.IsActive()) {
        return false;
    }

    std::lock_guard<std::recursive_mutex> locker(document_map_lock);
    auto document_map = GetDocumentMapInstance();

    auto shared = file.GetReference();
    auto found = document_map->find(shared.get());

    // A stale, inactive entry can linger under a File* address (e.g. a partially
    // constructed Document that threw after AddDocumentMapping, or a recycled
    // address). Mirror GetOrCreateDocument and treat such entries as absent, so
    // the create path does not falsely report the file as already opened.
    return (found != document_map->end() && found->second.IsActive());
}

bool SemanticUtils::TryGetMappedDocument(WeakReference<syntax::File> file, OutputDocumentPtr& result) {
    if (!file.IsActive()) {
        throw syntax::FileDisposedException();
    }

    std::lock_guard<std::recursive_mutex> locker(document_map_lock);
    auto document_map = GetDocumentMapInstance();

    auto shared = file.GetReference();
    auto found = document_map->find(shared.get());
    if (found == document_map->end()) {
        return false;
    }

    // The upgrade has to stay inside the registry critical section. ~Document
    // acquires this lock before its storage can be freed, so an upgrade under
    // the lock never touches deallocated memory - handing the weak reference
    // out and upgrading it after the lock is released would be a use after
    // free racing the final Release.
    auto existing = found->second.TryGetReference();
    if (!existing.has_value()) {
        return false;
    }

    result = existing.value();
    return true;
}

void SemanticUtils::AddDocumentMapping(WeakReference<syntax::File> file, WeakReference<Document> value) {
    if (!file.IsActive()) {
        throw syntax::FileDisposedException();
    }

    std::lock_guard<std::recursive_mutex> locker(document_map_lock);
    auto document_map = GetDocumentMapInstance();

    auto shared = file.GetReference();

    if (HasMappedDocument(file)) {
        spdlog::warn("File {} was already present in the document mapping, overriding", shared->GetFilenameString());
    }

    (*document_map)[shared.get()] = value;
}

void SemanticUtils::ReleaseMapping(WeakReference<syntax::File> file, const Document* owner) {
    if (!file.IsActive()) {
        throw syntax::FileDisposedException();
    }

    std::lock_guard<std::recursive_mutex> locker(document_map_lock);
    auto document_map = GetDocumentMapInstance();

    auto shared = file.GetReference();
    auto found = document_map->find(shared.get());
    if (found == document_map->end()) {
        spdlog::debug("File {} had no document mapping to release, a replacement has already claimed and released it", shared->GetFilenameString());
        return;
    }

    // While this document was dying, another thread may have failed to upgrade
    // it, built a replacement and mapped it under the same file. Erasing the
    // entry then would drop that live document out of the registry, and the
    // next open would build a second one beside it.
    if (!found->second.Identity(owner)) {
        spdlog::warn("File {} is mapped to a different document than the one being released, keeping the mapping", shared->GetFilenameString());
        return;
    }

    document_map->erase(found);
}

DocumentPtr SemanticUtils::GetOrCreateDocument(syntax::FilePtr file) {

    // The recursive lock is held across both the lookup and the create, so no
    // other thread can map a competing document in between
    std::lock_guard<std::recursive_mutex> locker(document_map_lock);

    OutputDocumentPtr existing;
    if (TryGetMappedDocument(file, existing)) {
        return existing;
    }

    // Either the file has no mapping, or the mapped document has already
    // committed to destruction and cannot be revived. It is unreachable by
    // now, so replacing it never leaves two live documents sharing one file.
    // The Document constructor calls AddDocumentMapping internally.
    return DocumentPtr(pdf_new Document(file));
}

} // semantics
} // vanillapdf
