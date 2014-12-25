#include "catalog.h"
#include "indirect_object.h"
#include "dictionary_object.h"
#include "exception.h"
#include "indirect_object_reference.h"
#include "page_tree.h"

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
