#ifndef _DOCUMENT_H
#define _DOCUMENT_H

#include "semantics/utils/semantics_fwd.h"
#include "semantics/objects/catalog.h"
#include "syntax/files/file.h"
#include "syntax/objects/dictionary_object.h"
#include "syntax/objects/name_object.h"
#include "semantics/objects/document_info.h"
#include "semantics/objects/date.h"

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
			~Document();

			CatalogPtr GetDocumentCatalog(void);
			bool GetDocumentInfo(OutputDocumentInfoPtr& result);
			void Save(const std::string& path);
			void SaveIncremental(const std::string& path);

			syntax::FileHolderPtr GetFile() const { return _holder; }

			void AppendContent(DocumentPtr other);

		private:
			syntax::FileHolderPtr _holder;

			// Cache
			mutable OutputCatalogPtr m_catalog;
			mutable OutputDocumentInfoPtr m_info;

			OutputNamedDestinationsPtr CreateNamedDestinations(CatalogPtr catalog);
			OutputNameDictionaryPtr CreateNameDictionary(CatalogPtr catalog);
			OutputNameTreePtr<DestinationPtr> CreateNameTreeDestinations(NameDictionaryPtr dictionary);

			void FixDestinationPage(DocumentPtr other, syntax::ObjectPtr page);
		};
	}
}

#endif /* _DOCUMENT_H */
