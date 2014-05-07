#ifndef _PAGE_TREE_H
#define _PAGE_TREE_H

#include "fwd.h"
#include "integer_object.h"
#include "array_object.h"

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
			boost::intrusive_ptr<IntegerObject> _count;
			boost::intrusive_ptr<ArrayObject> _kids;
		};
	}
}

#endif /* _PAGE_TREE_H */
