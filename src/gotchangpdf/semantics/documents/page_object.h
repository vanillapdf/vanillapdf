#ifndef _PAGE_H
#define _PAGE_H

#include "semantics_fwd.h"
#include "page_node.h"
#include "rectangle.h"
#include "resource_dictionary.h"

namespace gotchangpdf
{
	namespace semantics
	{
		class PageObject : public PageNodeBase
		{
		public:
			explicit PageObject(syntax::DictionaryObjectPtr obj);

			static std::unique_ptr<PageObject> Create(DocumentPtr document);
			static std::unique_ptr<PageObject> Create(syntax::DictionaryObjectPtr obj);

			PageTreeNodePtr GetParent(void) const;
			void SetParent(PageTreeNodePtr parent);

			ResourceDictionaryPtr GetResources(void) const;
			void SetResources(ResourceDictionaryPtr resources);

			RectanglePtr GetMediaBox(void) const;
			void SetMediaBox(RectanglePtr);

			//RectanglePtr GetAnnotations(void) const;
			//void SetAnnotations(RectanglePtr annots);

			bool GetContents(OutputContentsPtr& result) const;
			void SetContents(ContentsPtr contents);

			virtual NodeType GetNodeType(void) const noexcept override { return NodeType::Object; }
		};
	}
}

#endif /* _PAGE_H */
