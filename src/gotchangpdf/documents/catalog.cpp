#include "precompiled.h"
#include "catalog.h"

#include "constants.h"
#include "dictionary_object.h"
#include "name_object.h"
#include "exception.h"
#include "page_tree.h"

namespace gotchangpdf
{
	namespace documents
	{
		using namespace constant;

		Catalog::Catalog(DictionaryObjectPtr root) : HighLevelObject(root)
		{
			if (*root->FindAs<NameObjectPtr>(Name::Type) != Name::Catalog)
				throw exceptions::Exception("TODO");

			auto pages = root->FindAs<IndirectObjectReferencePtr>(Name::Pages);
			auto page_root = pages->GetReferencedObjectAs<DictionaryObjectPtr>();

			_pages = PageTreePtr(new PageTree(page_root));
		}
	}
}
