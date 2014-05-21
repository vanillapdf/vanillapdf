#ifndef _PAGE_H
#define _PAGE_H

#include "fwd.h"
#include "page_node.h"
#include "dictionary_object.h"
#include "resource_dictionary.h"

namespace gotchangpdf
{
	namespace documents
	{
		class PageObject : public PageNode
		{
		public:
			//PageObject();
			explicit PageObject(const DictionaryObject& obj);

			//inline ObjectReferenceWrapper<IndirectObjectReference> Parent(void) const { return _parent; }
			inline ObjectReferenceWrapper<ResourceDictionary> Resources(void) const { return _resources; }

			virtual inline HighLevelObject::Type GetType() const override { return HighLevelObject::Type::PageObject; }

		private:
			ObjectReferenceWrapper<DictionaryObject> _dictionary;
			ObjectReferenceWrapper<ResourceDictionary> _resources;
		};
	}
}

#endif /* _PAGE_H */
