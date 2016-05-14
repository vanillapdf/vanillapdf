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
			Document(const std::string& filename) : _holder(filename) { _holder->Value()->Initialize(); }
			Document(syntax::FileHolderPtr holder) : _holder(holder) { assert(holder->Value()); }

			CatalogPtr GetDocumentCatalog(void) const
			{
				auto chain = _holder->Value()->GetXrefChain();
				auto xref = chain->Begin()->Value();
				auto dictionary = xref->GetTrailerDictionary();
				auto root = dictionary->FindAs<syntax::DictionaryObjectPtr>(constant::Name::Root);
				return Catalog(root);
			}

			bool GetDocumentInfo(OutputDocumentInfoPtr& result) const
			{
				auto chain = _holder->Value()->GetXrefChain();
				auto xref = chain->Begin()->Value();
				auto dictionary = xref->GetTrailerDictionary();

				if (!dictionary->Contains(constant::Name::Info))
					return false;

				auto info = dictionary->FindAs<syntax::DictionaryObjectPtr>(constant::Name::Info);
				result = DocumentInfoPtr(info);
				return true;
			}

		private:
			syntax::FileHolderPtr _holder;
		};
	}
}

#endif /* _DOCUMENT_H */
