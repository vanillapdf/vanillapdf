#ifndef _PAGE_H
#define _PAGE_H

#include "fwd.h"
#include "page_node.h"
#include "dictionary_object.h"
#include "resource_dictionary.h"
#include "rectangle.h"
#include "page_tree_node.h"
#include "bind.h"
#include "smart_ptr.h"

namespace gotchangpdf
{
	namespace documents
	{
		class PageObject : public PageNode
		{
		public:
			explicit PageObject(DictionaryObjectPtr obj);

			PageTreeNodePtr Parent(void) const { return _parent(); }
			ResourceDictionaryPtr Resources(void) const { return _resources(); }
			RectanglePtr MediaBox(void) const { return _media_box(); }

			virtual inline HighLevelObject::Type GetType() const override { return HighLevelObject::Type::PageObject; }

		private:
			Bind<PageTreeNodePtr> _parent;
			Bind<ResourceDictionaryPtr> _resources;
			Bind<RectanglePtr> _media_box;

			PageTreeNodePtr GetParent(DictionaryObjectPtr obj);
			ResourceDictionaryPtr GetResources(DictionaryObjectPtr obj);
			RectanglePtr GetMediaBox(DictionaryObjectPtr obj);
		};

		typedef SmartPtr<PageObject> PageObjectPtr;
	}
}

#endif /* _PAGE_H */
