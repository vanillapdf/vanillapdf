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
	static WeakReference<Document> GetMappedDocument(WeakReference<syntax::File> file);
	static void AddDocumentMapping(WeakReference<syntax::File> file, WeakReference<Document> value);
	static void ReleaseMapping(WeakReference<syntax::File> file);
};

} // semantics
} // vanillapdf

#endif /* _SEMANTIC_UTILS_H */
