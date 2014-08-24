#ifndef _CATALOG_H
#define _CATALOG_H

#include "fwd.h"
#include "page_tree.h"
#include "smart_ptr.h"
#include "high_level_object.h"

namespace gotchangpdf
{
	namespace documents
	{
		class Catalog : public HighLevelObject<DictionaryObject>
		{
		public:
			explicit Catalog(SmartPtr<DictionaryObject> root);

			inline SmartPtr<PageTree> Pages(void) const { return _pages; }

			virtual inline HighLevelObject::Type GetType() const override { return HighLevelObject::Type::Catalog; }

		private:
			SmartPtr<PageTree> _pages;
		};

	}
}

#endif /* _CATALOG_H */
