#ifndef _PAGE_TREE_H
#define _PAGE_TREE_H

#include "fwd.h"
#include "integer_object.h"
#include "array_object.h"
#include "object_reference_wrapper.h"

namespace gotchangpdf
{
	namespace documents
	{
		class PageTree
		{
		public:
			PageTree();
			explicit PageTree(const IndirectObject& obj);

		private:
			ObjectReferenceWrapper<IntegerObject> _count;
			ObjectReferenceWrapper<ArrayObject> _kids;
		};
	}
}

#endif /* _PAGE_TREE_H */
