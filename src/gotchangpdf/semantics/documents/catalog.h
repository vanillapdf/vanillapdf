#ifndef _CATALOG_H
#define _CATALOG_H

#include "semantics_fwd.h"
#include "high_level_object.h"
#include "page_tree.h"

namespace gotchangpdf
{
	namespace semantics
	{
		class Catalog : public HighLevelObject<syntax::DictionaryObjectPtr>
		{
		public:
			explicit Catalog(syntax::DictionaryObjectPtr root);

			PageTreePtr Pages(void) const;

			virtual inline HighLevelObject::Type GetType() const override { return HighLevelObject::Type::Catalog; }
		};

	}
}

#endif /* _CATALOG_H */
