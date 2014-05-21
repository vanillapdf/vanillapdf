#ifndef _CATALOG_H
#define _CATALOG_H

#include "fwd.h"
#include "page_tree.h"
#include "object_reference_wrapper.h"
#include "high_level_object.h"

namespace gotchangpdf
{
	namespace documents
	{
		class Catalog : public HighLevelObject
		{
		public:
			explicit Catalog(const DictionaryObject& root);

			inline ObjectReferenceWrapper<PageTree> Pages(void) const { return _pages; }

			virtual inline HighLevelObject::Type GetType() const override { return HighLevelObject::Type::Catalog; }

		private:
			ObjectReferenceWrapper<PageTree> _pages;
		};

	}
}

#endif /* _CATALOG_H */
