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

		Catalog::Catalog(const IndirectObject& root)
		{
			auto dict = dynamic_wrapper_cast<DictionaryObject>(root.GetObject());

			if (*dict->Find<NameObject>(Name::Type) != Name::Catalog)
				throw Exception("TODO");

			_pages = PageTree(*dict->Find<IndirectObjectReference>(Name::Pages)->GetReference());
		}
	}
}
