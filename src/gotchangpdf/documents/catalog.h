#ifndef _CATALOG_H
#define _CATALOG_H

#include "fwd.h"
#include "page_tree.h"

namespace gotchangpdf
{
	namespace documents
	{
		class Catalog
		{
		public:
			explicit Catalog(const IndirectObject& root);

		private:
			PageTree _pages;
		};

	}
}

#endif /* _CATALOG_H */
