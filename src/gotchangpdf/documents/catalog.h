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

			PageTreePtr Pages(void) const;

			virtual inline HighLevelObject::Type GetType() const override { return HighLevelObject::Type::Catalog; }
		};

	}
}

#endif /* _CATALOG_H */
