#ifndef _PAGE_H
#define _PAGE_H

#include "fwd.h"
#include "page_node.h"

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
	}
}

#endif /* _PAGE_H */
