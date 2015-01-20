#ifndef _CATALOG_H
#define _CATALOG_H

#include "fwd.h"
#include "high_level_object.h"
#include "page_tree.h"

namespace gotchangpdf
{
	namespace documents
	{
		class Catalog : public HighLevelObject<DictionaryObjectPtr>
		{
		public:
			explicit Catalog(DictionaryObjectPtr root);

			inline PageTreePtr Pages(void) const { return _pages; }

			virtual inline HighLevelObject::Type GetType() const override { return HighLevelObject::Type::Catalog; }

		private:
			PageTreePtr _pages;
		};

	}
}

#endif /* _CATALOG_H */
