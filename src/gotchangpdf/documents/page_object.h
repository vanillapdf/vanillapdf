#ifndef _PAGE_H
#define _PAGE_H

#include "fwd.h"
#include "page_node.h"
#include "dictionary_object.h"
#include "resource_dictionary.h"
#include "rectangle.h"
#include "page_tree_node.h"
#include "smart_ptr.h"

namespace gotchangpdf
{
	namespace documents
	{
		class PageObject : public PageNode
		{
		public:
			explicit PageObject(DictionaryObjectPtr obj);

			PageTreeNodePtr Parent(void) const;
			ResourceDictionaryPtr Resources(void) const;
			RectanglePtr MediaBox(void) const;

			virtual inline HighLevelObject::Type GetType() const override { return HighLevelObject::Type::PageObject; }
		};

		typedef SmartPtr<PageObject> PageObjectPtr;
	}
}

#endif /* _PAGE_H */
