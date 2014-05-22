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

		Catalog::Catalog(ObjectReferenceWrapper<DictionaryObject> root) : HighLevelObject(root)
		{
			if (*root->FindAs<NameObject>(Name::Type) != Name::Catalog)
				throw Exception("TODO");

			auto pages = *root->FindAs<IndirectObjectReference>(Name::Pages);
			auto page_root = pages.GetReferencedObjectAs<DictionaryObject>();

			_pages = ObjectReferenceWrapper<PageTree>(new PageTree(page_root));
		}
	}
}

GOTCHANG_PDF_API PageTreeHandle CALLING_CONVENTION Catalog_GetPages(CatalogHandle handle)
{
	gotchangpdf::documents::Catalog* obj = reinterpret_cast<gotchangpdf::documents::Catalog*>(handle);
	auto pages = obj->Pages();
	return reinterpret_cast<PageTreeHandle>(pages.AddRefGet());
}
