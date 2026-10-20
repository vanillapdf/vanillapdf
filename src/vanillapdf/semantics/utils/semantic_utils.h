#ifndef _SEMANTIC_UTILS_H
#define _SEMANTIC_UTILS_H

#include "semantics/utils/semantics_fwd.h"
#include "semantics/objects/document.h"

#include "syntax/objects/name_object.h"

namespace vanillapdf {
namespace semantics {

class SemanticUtils {
public:
    static Version GetVersionFromName(const syntax::NameObjectPtr& name);
    static bool HasMappedDocument(WeakReference<syntax::File> file);

    /**
     * @brief Atomically find and upgrade the document mapped to the given file.
     *
     * The lookup and the weak reference upgrade happen inside one registry
     * critical section. The upgrade must not escape it: ~Document acquires the
     * registry lock before its storage can be freed, so only an upgrade under
     * that lock is guaranteed to never touch deallocated memory (see the
     * contract on IUnknown::TryAddRef). Returns false when the file has no
     * mapping or the mapped document has already committed to destruction.
     */
    static bool TryGetMappedDocument(WeakReference<syntax::File> file, OutputDocumentPtr& result);

    static void AddDocumentMapping(WeakReference<syntax::File> file, WeakReference<Document> value);
    static void ReleaseMapping(WeakReference<syntax::File> file, const Document* owner);

    /**
     * @brief Atomically get existing or create new document for the given file.
     *
     * This method ensures thread-safe document creation by holding a lock
     * during the entire check-and-create operation, preventing race conditions
     * when multiple threads attempt to open the same file simultaneously.
     */
    static DocumentPtr GetOrCreateDocument(syntax::FilePtr file);
};

} // semantics
} // vanillapdf

#endif /* _SEMANTIC_UTILS_H */
