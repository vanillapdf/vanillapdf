#ifndef _DOCUMENT_H
#define _DOCUMENT_H

#include "semantics_fwd.h"
#include "catalog.h"
#include "file.h"
#include "dictionary_object.h"
#include "name_object.h"

#include <string>

namespace gotchangpdf
{
	namespace semantics
	{
		class Document : public IUnknown
		{
		public:
			Document(const std::string& filename) : _holder(pdf_new syntax::File(filename)) { _holder->Value()->Initialize(_holder->Value()); }
			Document(syntax::FileHolderPtr holder) : _holder(holder) { assert(holder->Value()); }

			CatalogPtr GetDocumentCatalog(void) const
			{
				syntax::XrefBaseVisitor visitor;

				auto chain = _holder->Value()->GetXrefChain();
				auto xref_variant = chain->Begin()->Value();
				auto xref = xref_variant.apply_visitor(visitor);
				auto dictionary = xref->GetTrailerDictionary();
				auto root = dictionary->FindAs<syntax::DictionaryObjectPtr>(constant::Name::Root);
				return Catalog(root);
			}

		private:
			syntax::FileHolderPtr _holder;
		};
	}
}

#endif /* _DOCUMENT_H */
