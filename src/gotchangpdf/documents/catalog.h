#ifndef _CATALOG_H
#define _CATALOG_H

#include "fwd.h"
#include "page_tree.h"
#include "object_reference_wrapper.h"

#include <memory>

namespace gotchangpdf
{
	namespace documents
	{
		class Catalog : public boost::intrusive_ref_counter<Catalog>
		{
		public:
			explicit Catalog(const IndirectObject& root);

			inline ObjectReferenceWrapper<PageTree> Pages(void) const { return _pages; }

		private:
			ObjectReferenceWrapper<PageTree> _pages;
		};

	}
}

#endif /* _CATALOG_H */
