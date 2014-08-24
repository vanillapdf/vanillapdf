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

			inline IntegerObject::ValueType PageCount(void) const { return _root->KidCount(); }
			inline SmartPtr<PageObject> Page(unsigned int number) const { return PageInternal(number); }
			inline SmartPtr<PageObject> operator[](unsigned int number) const { return PageInternal(number); }

			virtual inline HighLevelObject::Type GetType() const override { return HighLevelObject::Type::PageTree; }

		private:
			SmartPtr<PageObject> PageInternal(unsigned int number) const;
			bool HasTreeChilds(SmartPtr<PageTreeNode> node) const;

			SmartPtr<PageTreeNode> _root;
		};
	}
}

#endif /* _PAGE_TREE_H */
