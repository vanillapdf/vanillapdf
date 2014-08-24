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
			explicit PageObject(SmartPtr<DictionaryObject> obj);

			SmartPtr<PageTreeNode> Parent(void) const { return _parent(); }
			SmartPtr<ResourceDictionary> Resources(void) const { return _resources(); }
			SmartPtr<Rectangle> MediaBox(void) const { return _media_box(); }

			virtual inline HighLevelObject::Type GetType() const override { return HighLevelObject::Type::PageObject; }

		private:
			Bind<DictionaryObject, PageTreeNode> _parent;
			Bind<DictionaryObject, ResourceDictionary> _resources;
			Bind<DictionaryObject, Rectangle> _media_box;

			SmartPtr<PageTreeNode> GetParent(SmartPtr<DictionaryObject> obj);
			SmartPtr<ResourceDictionary> GetResources(SmartPtr<DictionaryObject> obj);
			SmartPtr<Rectangle> GetMediaBox(SmartPtr<DictionaryObject> obj);
		};
	}
}

#endif /* _PAGE_H */
