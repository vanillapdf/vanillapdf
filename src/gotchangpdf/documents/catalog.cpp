#include "catalog.h"
#include "indirect_object.h"
#include "dictionary_object.h"
#include "exception.h"
#include "indirect_object_reference.h"
#include "page_tree.h"

#include "c_catalog.h"

namespace gotchangpdf
{
	namespace documents
	{
		using namespace constant;
		using namespace exceptions;

		Catalog::Catalog(DictionaryObjectPtr root) : HighLevelObject(root)
		{
			if (*root->FindAs<NameObjectPtr>(Name::Type) != Name::Catalog)
				throw Exception("TODO");

			auto pages = root->FindAs<IndirectObjectReferencePtr>(Name::Pages);
			auto page_root = pages->GetReferencedObjectAs<DictionaryObjectPtr>();

			_pages = PageTreePtr(new PageTree(page_root));
		}
	}
}

GOTCHANG_PDF_API PageTreeHandle CALLING_CONVENTION Catalog_GetPages(CatalogHandle handle)
{
	gotchangpdf::documents::Catalog* obj = reinterpret_cast<gotchangpdf::documents::Catalog*>(handle);
	auto pages = obj->Pages();
	return reinterpret_cast<PageTreeHandle>(pages.AddRefGet());
}
