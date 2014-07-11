#ifndef _PAGE_TREE_H
#define _PAGE_TREE_H

#include "high_level_object.h"
#include "object_reference_wrapper.h"
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
			explicit PageTree(ObjectReferenceWrapper<DictionaryObject> root);

			inline IntegerObject::ValueType PageCount(void) const { return _root->KidCount(); }
			inline ObjectReferenceWrapper<PageObject> Page(unsigned int number) const { return PageInternal(number); }
			inline ObjectReferenceWrapper<PageObject> operator[](unsigned int number) const { return PageInternal(number); }

			virtual inline HighLevelObject::Type GetType() const override { return HighLevelObject::Type::PageTree; }

		private:
			ObjectReferenceWrapper<PageObject> PageInternal(unsigned int number) const;

			mutable ObjectReferenceWrapper<PageTreeNode> _root;
		};
	}
}

#endif /* _PAGE_TREE_H */
