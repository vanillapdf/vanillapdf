#ifndef _DOCUMENT_H
#define _DOCUMENT_H

#include "semantics_fwd.h"
#include "catalog.h"
#include "file.h"
#include "dictionary_object.h"
#include "name_object.h"
#include "document_info.h"
#include "date.h"

#include <string>

namespace gotchangpdf
{
	namespace semantics
	{
		class Document : public IUnknown
		{
		public:
			Document(const std::string& filename);
			Document(syntax::FileHolderPtr holder);

			CatalogPtr GetDocumentCatalog(void) const;
			bool GetDocumentInfo(OutputDocumentInfoPtr& result) const;
			void Save(const std::string& path);
			void SaveIncremental(const std::string& path);

			syntax::FileHolderPtr GetFile() const { return _holder; }

			void AppendContent(const Document& other);

		private:
			syntax::FileHolderPtr _holder;

			// Cache
			mutable OutputCatalogPtr m_catalog;
			mutable OutputDocumentInfoPtr m_info;
		};
	}
}

#endif /* _DOCUMENT_H */
