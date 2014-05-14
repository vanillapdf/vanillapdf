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
		class Catalog
		{
		public:
			explicit Catalog(const IndirectObject& root);

			inline ObjectReferenceWrapper<PageTree> Pages(void) const { return _pages; }

		private:
			ObjectReferenceWrapper<PageTree> _pages;

			mutable long _intrusive_ref_count;

			template <typename T>
			friend void ::boost::intrusive_ptr_add_ref(T*);

			template <typename T>
			friend void ::boost::intrusive_ptr_release(T*);
		};

	}
}

#endif /* _CATALOG_H */
