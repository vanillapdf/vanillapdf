#ifndef _RESOURCE_DICTIONARY_H
#define _RESOURCE_DICTIONARY_H

#include "fwd.h"
#include "high_level_object.h"
#include "dictionary_object.h"

namespace gotchangpdf
{
	namespace documents
	{
		class ResourceDictionary : public HighLevelObject<DictionaryObject>
		{
		public:
			//PageObject();
			explicit ResourceDictionary(DictionaryObjectPtr obj) : HighLevelObject(obj) {}

			//inline ObjectReferenceWrapper<IndirectObjectReference> Parent(void) const { return _parent; }
			//inline ObjectReferenceWrapper<DictionaryObject> Resources(void) const { return _resources; }

			virtual inline Type GetType() const override { return HighLevelObject::Type::ResourceDictionary; }
		};

		typedef SmartPtr<ResourceDictionary> ResourceDictionaryPtr;
	}
}

#endif /* _RESOURCE_DICTIONARY_H */
