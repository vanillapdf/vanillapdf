#ifndef _PAGE_H
#define _PAGE_H

#include "fwd.h"
#include "page_node.h"
#include "dictionary_object.h"
#include "resource_dictionary.h"
#include "rectangle.h"
#include "page_tree_node.h"
#include "bind.h"

namespace gotchangpdf
{
	namespace documents
	{
		class PageObject : public PageNode
		{
		public:
			//PageObject();
			explicit PageObject(ObjectReferenceWrapper<DictionaryObject> obj);

			ObjectReferenceWrapper<PageTreeNode> Parent(void) const { return _parent(); }
			ObjectReferenceWrapper<ResourceDictionary> Resources(void) const { return _resources(); }
			ObjectReferenceWrapper<Rectangle> MediaBox(void) const { return _media_box(); }

			virtual inline HighLevelObject::Type GetType() const override { return HighLevelObject::Type::PageObject; }

		private:
			Bind<DictionaryObject, PageTreeNode> _parent;
			Bind<DictionaryObject, ResourceDictionary> _resources;
			Bind<DictionaryObject, Rectangle> _media_box;

			ObjectReferenceWrapper<PageTreeNode> GetParent(ObjectReferenceWrapper<DictionaryObject> obj);
			ObjectReferenceWrapper<ResourceDictionary> GetResources(ObjectReferenceWrapper<DictionaryObject> obj);
			ObjectReferenceWrapper<Rectangle> GetMediaBox(ObjectReferenceWrapper<DictionaryObject> obj);
		};
	}
}

#endif /* _PAGE_H */
