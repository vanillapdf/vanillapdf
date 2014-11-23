#ifndef _PAGE_TREE_H
#define _PAGE_TREE_H

#include "high_level_object.h"
#include "smart_ptr.h"
#include "dictionary_object.h"
#include "page_object.h"
#include "page_tree_node.h"

namespace gotchangpdf
{
	namespace documents
	{
		class PageTree : public HighLevelObject<DictionaryObject>
		{
		public:
			//PageTree();
			explicit PageTree(SmartPtr<DictionaryObject> root);

			inline IntegerObject::value_type PageCount(void) const { return _root->KidCount(); }
			inline SmartPtr<PageObject> Page(types::integer number) const { return PageInternal(number); }
			inline SmartPtr<PageObject> operator[](types::integer number) const { return PageInternal(number); }

			virtual inline HighLevelObject::Type GetType() const override { return HighLevelObject::Type::PageTree; }

		private:
			SmartPtr<PageObject> PageInternal(types::integer number) const;
			bool HasTreeChilds(SmartPtr<PageTreeNode> node) const;

			SmartPtr<PageTreeNode> _root;
		};
	}
}

#endif /* _PAGE_TREE_H */
