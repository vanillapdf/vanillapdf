#ifndef _PAGE_H
#define _PAGE_H

#include "semantics_fwd.h"
#include "page_node.h"

namespace gotchangpdf
{
	namespace semantics
	{
		class PageObject : public PageNodeBase
		{
		public:
			explicit PageObject(syntax::DictionaryObjectPtr obj);

			PageTreeNodePtr Parent(void) const;
			ResourceDictionaryPtr Resources(void) const;
			RectanglePtr MediaBox(void) const;
			bool Contents(ContentsPtr& result) const;

			virtual NodeType GetNodeType(void) const noexcept override { return NodeType::Object; }
		};
	}
}

#endif /* _PAGE_H */
