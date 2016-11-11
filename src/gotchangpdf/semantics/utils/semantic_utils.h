#ifndef _SEMANTIC_UTILS_H
#define _SEMANTIC_UTILS_H

#include "semantics_fwd.h"
#include "document.h"
#include "version.h"
#include "name_object.h"

namespace gotchangpdf
{
	namespace semantics
	{
		class SemanticUtils
		{
		public:
			static Version GetVersionFromName(const syntax::NameObjectPtr& name);
			static WeakReference<Document> GetMappedDocument(std::weak_ptr<syntax::File> file);
			static void AddDocumentMapping(std::weak_ptr<syntax::File> file, WeakReference<Document> value);
			static void ReleaseMapping(std::weak_ptr<syntax::File> file);
		};
	}
}

#endif /* _SEMANTIC_UTILS_H */
