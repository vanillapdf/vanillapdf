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
			explicit PageObject(ObjectReferenceWrapper<DictionaryObject> obj);

			//inline ObjectReferenceWrapper<IndirectObjectReference> Parent(void) const { return _parent; }
			ObjectReferenceWrapper<ResourceDictionary> Resources(void) const;

			virtual inline HighLevelObject::Type GetType() const override { return HighLevelObject::Type::PageObject; }

		private:
			mutable ObjectReferenceWrapper<ResourceDictionary> _resources;
		};
	}
}

#endif /* _PAGE_H */
