#ifndef _PAGE_TREE_NODE_H
#define _PAGE_TREE_NODE_H

#include "fwd.h"
#include "page_node.h"

namespace gotchangpdf
{
	namespace documents
	{
		class PageTreeNode : public PageNode
		{
		public:
			explicit PageTreeNode(DictionaryObjectPtr obj);

			types::integer KidCount(void) const;
			ArrayObjectPtr<PageNodePtr> Kids(void) const;

			virtual inline HighLevelObject::Type GetType() const override { return HighLevelObject::Type::PageTreeNode; }
		};
	}
}

#endif /* _PAGE_TREE_NODE_H */
