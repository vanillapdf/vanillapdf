#ifndef _PAGE_TREE_H
#define _PAGE_TREE_H

#include "high_level_object.h"
#include "deferred.h"
#include "dictionary_object.h"
#include "page_object.h"
#include "page_tree_node.h"

namespace gotchangpdf
{
	namespace documents
	{
		class PageTree : public HighLevelObject<DictionaryObjectPtr>
		{
		public:
			//PageTree();
			explicit PageTree(DictionaryObjectPtr root);

			inline IntegerObject::value_type PageCount(void) const { return _root->KidCount(); }
			inline PageObjectPtr Page(types::integer number) const { return PageInternal(number); }
			inline PageObjectPtr operator[](types::integer number) const { return PageInternal(number); }

			virtual inline HighLevelObject::Type GetType() const override { return HighLevelObject::Type::PageTree; }

		private:
			PageObjectPtr PageInternal(types::integer number) const;
			bool HasTreeChilds(PageTreeNodePtr node) const;

			PageTreeNodePtr _root;
		};

		typedef SmartPtr<PageTree> PageTreePtr;
	}
}

#endif /* _PAGE_TREE_H */
