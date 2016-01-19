#ifndef _PAGE_TREE_H
#define _PAGE_TREE_H

#include "semantics_fwd.h"
#include "high_level_object.h"
#include "page_tree_node.h"
#include "page_object.h"

namespace gotchangpdf
{
	namespace semantics
	{
		class PageTree : public HighLevelObject<syntax::DictionaryObjectPtr>
		{
		public:
			//PageTree();
			explicit PageTree(syntax::DictionaryObjectPtr root);

			types::integer PageCount(void) const { return _root->KidCount(); }
			PageObjectPtr Page(types::integer number) const { return PageInternal(number); }
			PageObjectPtr operator[](types::integer number) const { return PageInternal(number); }

		private:
			PageObjectPtr PageInternal(types::integer number) const;
			bool HasTreeChilds(PageTreeNodePtr node) const;

			PageTreeNodePtr _root;
		};
	}
}

#endif /* _PAGE_TREE_H */
