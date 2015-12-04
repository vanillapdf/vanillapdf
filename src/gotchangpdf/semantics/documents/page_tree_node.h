#ifndef _PAGE_TREE_NODE_H
#define _PAGE_TREE_NODE_H

#include "semantics_fwd.h"
#include "page_node.h"
#include "page_node_utils.h"

namespace gotchangpdf
{
	namespace semantics
	{
		class PageTreeNode : public PageNodeBase
		{
		public:
			explicit PageTreeNode(syntax::DictionaryObjectPtr obj);

			types::integer KidCount(void) const;
			syntax::ArrayObjectPtr<PageNodeBasePtr> Kids(void) const;

			virtual inline HighLevelObject::Type GetType() const override { return HighLevelObject::Type::PageTreeNode; }
		};
	}
}

#endif /* _PAGE_TREE_NODE_H */
